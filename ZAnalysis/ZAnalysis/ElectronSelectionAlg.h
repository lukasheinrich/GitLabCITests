// Dear emacs, this is -*- c++ -*-
#ifndef ZANALYSIS_ELECTRONSELECTIONALG_H
#define ZANALYSIS_ELECTRONSELECTIONALG_H

// Tool include(s):
#include "EventLoop/Algorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "PATCore/IAsgSelectionTool.h"

namespace ZAna {

   /// Algorithm selecting good electrons for the analysis
   ///
   /// This algorithm is used to select the good electrons from the calibrated
   /// container.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class ElectronSelectionAlg : public EL::Algorithm {

   public:
      /// Default algorithm constructor
      ElectronSelectionAlg();

      /// @name Functions inherited from EL::Algorithm
      /// @{

      /// Function helping to set up the job
      EL::StatusCode setupJob( EL::Job& job );

      /// Function initialising the algorithm
      EL::StatusCode initialize();

      /// Function executing the algorithm
      EL::StatusCode execute();

      /// @}

   private:
      /// The tool performing the electron calibration
      asg::AnaToolHandle< IAsgSelectionTool > m_elSelectorTool; //!

      // Declare the class to ROOT:
      ClassDef( ZAna::ElectronSelectionAlg, 1 )

   }; // class ElectronSelectionAlg

} // namespace ZAna

#endif // ZANALYSIS_ELECTRONSELECTIONALG_H
