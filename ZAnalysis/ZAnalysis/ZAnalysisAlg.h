// Dear emacs, this is -*- c++ -*-
#ifndef ZANALYSISTEST_ZANALYSISALG_H
#define ZANALYSISTEST_ZANALYSISALG_H

// Tool include(s):
#include "EventLoop/Algorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h"

// Forward declaration(s):
class TH1;

/// Namespace holding all of the ZAnalysis code
namespace ZAna {

   /// Algorithm performing the meat of the analysis
   ///
   /// This algorithm is used to apply some basic event selections, and then
   /// combine selected electrons into Z candidates.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class ZAnalysisAlg : public EL::Algorithm {

   public:
      /// Default algorithm constructor
      ZAnalysisAlg();

      /// @name Functions inherited from EL::Algorithm
      /// @{

      /// Function helping to set up the job
      EL::StatusCode setupJob( EL::Job& job );

      /// Function run at the start of the job, to initialise the histograms
      EL::StatusCode histInitialize();

      /// Function initialising the algorithm
      EL::StatusCode initialize();

      /// Function executing the algorithm
      EL::StatusCode execute();

      /// @}

   private:
      /// The good runs list selector
      asg::AnaToolHandle< IGoodRunsListSelectionTool > m_grlTool; //!

      /// The Z candidate mass histogram
      TH1* m_zMassHist;

      // Declare the class to ROOT:
      ClassDef( ZAna::ZAnalysisAlg, 1 )

   }; // class ZAnalysisAlg

} // namespace ZAna

#endif // ZANALYSISTEST_ZANALYSISALG_H
