
// System include(s):
#include <memory>

// Tool include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODCore/ShallowCopy.h"

// Local include(s):
#include "ZAnalysis/ElectronCalibAlg.h"
#include "ZCheck.h"

// Declare the class to ROOT:
ClassImp( ZAna::ElectronCalibAlg )

namespace ZAna {

   ElectronCalibAlg::ElectronCalibAlg()
      : EL::Algorithm(),
        m_elCalibTool( "CP::EgammaCalibrationAndSmearingTool/"
                       "ElectronCalibTool" ) {

   }

   EL::StatusCode ElectronCalibAlg::setupJob( EL::Job& job ) {

      // Make it clear that we use an xAOD input:
      ZCHECK( "setupJob", xAOD::Init() );
      job.useXAOD();

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ElectronCalibAlg::initialize() {

      // Initialise the electron calibration tool:
      ZCHECK( "initialize",
              m_elCalibTool.setProperty( "ESModel",
                                         "es2016data_mc15c" ) );
      const auto randRN = EgammaCalibPeriodRunNumbersExample::run_2016;
      ZCHECK( "initialize",
              m_elCalibTool.setProperty( "randomRunNumber", randRN ) );
      ZCHECK( "initialize", m_elCalibTool.initialize() );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ElectronCalibAlg::execute() {

      // Access the TEvent/TStore objects:
      xAOD::TEvent* event = wk()->xaodEvent();
      xAOD::TStore* store = wk()->xaodStore();

      // Retrieve the electrons:
      const xAOD::ElectronContainer* electrons = nullptr;
      ZCHECK( "execute", event->retrieve( electrons, "Electrons" ) );

      // Create a shallow copy of them:
      auto shallowCopy = xAOD::shallowCopyContainer( *electrons );
      // Make sure that we take ownership of the objects created in memory:
      std::unique_ptr< xAOD::ElectronContainer >
         electronsCopy( shallowCopy.first );
      std::unique_ptr< xAOD::ShallowAuxContainer >
         electronsShallowAux( shallowCopy.second );

      // Calibrate the shallow copies:
      for( xAOD::Electron* el : *electronsCopy ) {
         ZCHECK( "execute", m_elCalibTool->applyCorrection( *el ) );
      }

      // Record the calibrated electrons into the transient store:
      ZCHECK( "execute", store->record( std::move( electronsCopy ),
                                        "CalibratedElectrons" ) );
      ZCHECK( "execute", store->record( std::move( electronsShallowAux ),
                                        "CalibratedElectronsAux." ) );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

} // namespace ZAna
