
// ROOT include(s):
#include <TChain.h>

// Tool include(s):
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/LocalDriver.h"
#include "xAODRootAccess/Init.h"

// Local include(s):
#include "ZAnalysis/ElectronCalibAlg.h"
#include "ZAnalysis/ElectronSelectionAlg.h"
#include "ZAnalysis/ZAnalysisAlg.h"

void LocalJob() {

   // Set up a TChain on some local files:
   ::TChain chain( "CollectionTree" );
   chain.Add( "/afs/cern.ch/work/k/krasznaa/public/hssip/"
              "data16_13TeV.00304494.physics_Main.merge.AOD.r9264_p3083/*" );

   // Use it to set up a sample with:
   SH::SampleHandler sh;
   sh.add( SH::makeFromTChain( "xAOD", chain ) );
   sh.setMetaString( "nc_tree", chain.GetName() );
   sh.print();

   // Create an EventLoop job:
   EL::Job job;
   job.sampleHandler( sh );

   // Set up the analysis algorithms:
   job.algsAdd( new ZAna::ElectronCalibAlg() );
   job.algsAdd( new ZAna::ElectronSelectionAlg() );
   job.algsAdd( new ZAna::ZAnalysisAlg() );

   // Run the job using the direct driver:
   //EL::DirectDriver driver;
   //driver.submit( job, "data16_13TeV" );

   // Run the job using the local driver:
   EL::LocalDriver driver;
   driver.submit( job, "data16_13TeV" );

   // Return gracefully:
   return;
}
