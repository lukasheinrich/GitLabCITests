// Dear emacs, this is -*- c++ -*-
#ifndef ZANALYSIS_ZCHECK_H
#define ZANALYSIS_ZCHECK_H

// Silence warnings about unused functions:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

// Status code include(s):
#include "EventLoop/StatusCode.h"
#include "AsgTools/StatusCode.h"
#include "PATInterfaces/CorrectionCode.h"
#include "xAODRootAccess/tools/TReturnCode.h"

/// Helper macro checking return values in EL::Algorithm-s
///
/// Usage: ZCHECK( "initialize", somePossiblyFailingFunction() );
///
#define ZCHECK( LOC, EXP )                                      \
   do {                                                         \
      const auto sc = EXP;                                      \
      if( isFailure( sc ) ) {                                   \
         Error( #LOC, "Failed to execute\"%s\"", #EXP );        \
         return EL::StatusCode::FAILURE;                        \
      }                                                         \
   } while( 0 )

namespace {

   /// Function checking if an EL::StatusCode is in failure mode
   bool isFailure( const EL::StatusCode& sc ) {
      return ( sc == EL::StatusCode::FAILURE );
   }

   /// Function checking if a StatusCode is in failure mode
   bool isFailure( const StatusCode& sc ) {
      return sc.isFailure();
   }

   /// Function checking if a CorrectionCode is in failure mode
   bool isFailure( const CP::CorrectionCode& cc ) {
      return ( cc == CP::CorrectionCode::Error );
   }

   /// Function checking if an xAOD::TReturnCode is in failure mode
   bool isFailure( const xAOD::TReturnCode& rc ) {
      return rc.isFailure();
   }

} // private namespace

// Undo the GCC modifications:
#pragma GCC diagnostic pop

#endif // ZANALYSIS_ZCHECK_H
