// Dear emacs, this is -*- c++ -*-
#ifndef ZANALYSIS_ELECTRONCALIBALG_H
#define ZANALYSIS_ELECTRONCALIBALG_H

// Tool include(s):
#include "EventLoop/Algorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "ElectronPhotonFourMomentumCorrection/IEgammaCalibrationAndSmearingTool.h"

namespace ZAna {

   /// Algorithm calibrating electrons for the analysis
   ///
   /// This is a simple algorithm creating a shallow copy of the electrons
   /// coming from the input file, and calibrating them, and putting them
   /// into the transient store.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class ElectronCalibAlg : public EL::Algorithm {

   public:
      /// Default algorithm constructor
      ElectronCalibAlg();

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
      asg::AnaToolHandle< CP::IEgammaCalibrationAndSmearingTool >
      m_elCalibTool; //!

      // Declare the class to ROOT:
      ClassDef( ZAna::ElectronCalibAlg, 1 )

   }; // class ElectronCalibAlg

} // namespace ZAna

#endif // ZANALYSIS_ELECTRONCALIBALG_H
