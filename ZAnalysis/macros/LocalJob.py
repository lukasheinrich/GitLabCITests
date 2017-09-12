#!/usr/bin/env python
#
# Script running the analysis from Python.
#

# Set up (Py)ROOT:
import ROOT
ROOT.xAOD.Init().ignore()

# Set up a chain on some local files:
chain = ROOT.TChain( "CollectionTree" )
chain.Add( "/afs/cern.ch/work/k/krasznaa/public/hssip/"
           "data16_13TeV.00304494.physics_Main.merge.AOD.r9264_p3083/*" )

# Use it to set up a sample with:
sh = ROOT.SH.SampleHandler()
sh.add( ROOT.SH.makeFromTChain( "xAOD", chain ) )
sh.setMetaString( "nc_tree", chain.GetName() )

# Create an EventLoop job:
job = ROOT.EL.Job()
job.sampleHandler( sh )

# Set up the analysis algorithms:
job.algsAdd( ROOT.ZAna.ElectronCalibAlg() )
job.algsAdd( ROOT.ZAna.ElectronSelectionAlg() )
job.algsAdd( ROOT.ZAna.ZAnalysisAlg() )

# Run the job:
#driver = ROOT.EL.DirectDriver()
driver = ROOT.EL.LocalDriver()
driver.submit( job, "data16_13TeV" )
