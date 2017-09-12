
// System include(s):
#include <algorithm>
#include <memory>

// ROOT include(s):
#include <TLorentzVector.h>
#include <TH1.h>

// Tool include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "CxxUtils/fpcompare.h"

// EDM include(s):
#include "xAODBase/IParticle.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"

// Local include(s):
#include "ZAnalysis/ZAnalysisAlg.h"
#include "ZCheck.h"

// Declare the class to ROOT:
ClassImp( ZAna::ZAnalysisAlg )

namespace {

   /// Function used for ordering the electrons according to their pT
   bool ptSort( const xAOD::IParticle* p1, const xAOD::IParticle* p2 ) {
      return CxxUtils::fpcompare::greater( p1->pt(), p2->pt() );
   }

} // private namespace

namespace ZAna {

   ZAnalysisAlg::ZAnalysisAlg()
      : EL::Algorithm(), m_grlTool( "GoodRunsListSelectionTool" ),
        m_zMassHist( nullptr ) {

   }

   EL::StatusCode ZAnalysisAlg::setupJob( EL::Job& job ) {

      // Make it clear that we use an xAOD input:
      ZCHECK( "setupJob", xAOD::Init() );
      job.useXAOD();

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ZAnalysisAlg::histInitialize() {

      // Make sure that statistical uncertainties are kept:
      TH1::SetDefaultSumw2();

      // Create the Z candidate mass histogram:
      m_zMassHist = new TH1D( "zMass",
                              "Mass of the Z candidate;m (GeV/c^{2});Entries",
                              40, 20.0, 120.0 );
      wk()->addOutput( m_zMassHist );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ZAnalysisAlg::initialize() {

      // Get the event info, to be able to tell whether we're running on
      // data or MC.
      const xAOD::EventInfo* ei = nullptr;
      ZCHECK( "initialize", wk()->xaodEvent()->retrieve( ei, "EventInfo" ) );

      // Initialise the GRL tool:
      if( ei->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {
         ZCHECK( "initialize", m_grlTool.setProperty( "PassThrough", true ) );
      } else {
         ZCHECK( "initialize", m_grlTool.setProperty( "PassThrough", false ) );
         const std::vector< std::string > grls = {
            "GoodRunsLists/data16_13TeV/20161101/"
            "data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects"
            "-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml"
         };
         ZCHECK( "initialize",
                 m_grlTool.setProperty( "GoodRunsListVec", grls ) );
      }
      ZCHECK( "initialize", m_grlTool.initialize() );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

   EL::StatusCode ZAnalysisAlg::execute() {

      // Access the TEvent/TStore objects:
      xAOD::TEvent* event = wk()->xaodEvent();

      // Make sure that the event comes from a good lumi-block:
      if( ! m_grlTool->passRunLB() ) {
         return EL::StatusCode::SUCCESS;
      }

      // Retrieve the selected electrons:
      const xAOD::ElectronContainer* electrons = nullptr;
      ZCHECK( "execute", event->retrieve( electrons, "SelectedElectrons" ) );

      // If there aren't at least two electrons, stop here:
      if( electrons->size() < 2 ) {
         return EL::StatusCode::SUCCESS;
      }

      // Helper typedef:
      typedef ConstDataVector< xAOD::ElectronContainer > CEC_t;

      // Make a copy of it:
      auto sortedElectrons = std::make_unique< CEC_t >( electrons->begin(),
                                                        electrons->end(),
                                                        SG::VIEW_ELEMENTS );

      // Now sort the copy according to the pT of the electrons:
      std::stable_sort( sortedElectrons->begin(), sortedElectrons->end(),
                        ::ptSort );

      // Take the two highest pT electrons:
      const xAOD::Electron* el1 = ( *sortedElectrons )[ 0 ];
      const xAOD::Electron* el2 = ( *sortedElectrons )[ 1 ];

      // Create a Z candidate from them:
      const ::TLorentzVector zCand = el1->p4() + el2->p4();

      // And now save it:
      static const double MeVtoGeV = 0.001;
      m_zMassHist->Fill( zCand.M() * MeVtoGeV );

      // Return gracefully:
      return EL::StatusCode::SUCCESS;
   }

} // namespace ZAna
