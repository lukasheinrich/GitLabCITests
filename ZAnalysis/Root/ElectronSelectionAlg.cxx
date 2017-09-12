
// System include(s):
#include <memory>
#include <cmath>

// Tool include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

// EDM include(s):
#include "AthContainers/ConstDataVector.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"

// Local include(s):
#include "ZAnalysis/ElectronSelectionAlg.h"
#include "ZCheck.h"

// Declare the class to ROOT:
ClassImp( ZAna::ElectronSelectionAlg )

namespace ZAna {

   ElectronSelectionAlg::ElectronSelectionAlg()
      : EL::Algorithm(), m_elSelectorTool( "AsgElectronLikelihoodTool/"
                                           "ElectronSelectorTool" ) {

   }

   EL::StatusCode ElectronSelectionAlg::setupJob( EL::Job& job ) {

      // Make it clear that we use an xAOD input:
      ZCHECK( "setupJob", xAOD::Init() );
      job.useXAOD();

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ElectronSelectionAlg::initialize() {

      // Initialise the electron selector tool:
      ZCHECK( "initialize",
              m_elSelectorTool.setProperty( "WorkingPoint",
                                            "MediumLHElectron" ) );
      ZCHECK( "initialize", m_elSelectorTool.initialize() );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ElectronSelectionAlg::execute() {

      // Access the TEvent/TStore objects:
      xAOD::TEvent* event = wk()->xaodEvent();
      xAOD::TStore* store = wk()->xaodStore();

      // Retrieve the event info:
      const xAOD::EventInfo* ei = nullptr;
      ZCHECK( "execute", event->retrieve( ei, "EventInfo" ) );

      // Retrieve the calibrated electrons:
      const xAOD::ElectronContainer* electrons = nullptr;
      ZCHECK( "execute", event->retrieve( electrons, "CalibratedElectrons" ) );

      // Helper typedef:
      typedef ConstDataVector< xAOD::ElectronContainer > CEC_t;

      // Create a view container of selected electrons:
      auto selectedElectrons = std::make_unique< CEC_t >( SG::VIEW_ELEMENTS );

      // Retrieve the vertex container:
      const xAOD::VertexContainer* vertices = nullptr;
      ZCHECK( "execute", event->retrieve( vertices, "PrimaryVertices" ) );

      // Pick out the primary vertex:
      const xAOD::Vertex* priVtx = nullptr;
      for( const xAOD::Vertex* vtx : *vertices ) {
         if( vtx->vertexType() == xAOD::VxType::PriVtx ) {
            priVtx = vtx;
            break;
         }
      }

      // Good electrons can only be found if we have a good primary vertex:
      if( priVtx ) {

         // Loop over the calibrated electrons:
         for( const xAOD::Electron* el : *electrons ) {

            // Apply some basic kinematic cuts first:
            if( ( el->pt() < 20000.0 ) ||
                ( std::abs( el->caloCluster()->etaBE( 2 ) ) > 2.47 ) ) {
               continue;
            }

            // Apply a transverse impact parameter significance cut:
            const double d0sig =
               xAOD::TrackingHelpers::d0significance( el->trackParticle(),
                                                      ei->beamPosSigmaX(),
                                                      ei->beamPosSigmaY(),
                                                      ei->beamPosSigmaXY() );
            if( d0sig > 5.0 ) {
               continue;
            }

            // Apply a longitudinal impact parameter cut:
            const double deltaZ0 = std::abs( el->trackParticle()->z0() +
                                             el->trackParticle()->vz() -
                                             priVtx->z() );
            const double z0Sig = ( deltaZ0 *
                                   std::sin( el->trackParticle()->theta() ) );
            if( z0Sig > 0.5 ) {
               continue;
            }

            // Apply a likelihood selection:
            if( ! m_elSelectorTool->accept( el ) ) {
               continue;
            }

            // The electron is selected:
            selectedElectrons->push_back( el );
         }
      }

      // Record the selected electron container:
      ZCHECK( "execute", store->record( std::move( selectedElectrons ),
                                        "SelectedElectrons" ) );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

} // namespace ZAna
