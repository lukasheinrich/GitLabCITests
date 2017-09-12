// Dear emacs, this is -*- c++ -*-
#ifndef ZANALYSIS_LINKDEF_H
#define ZANALYSIS_LINKDEF_H

// Some common definitions:
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

// Declare the class(es) to generate dictionaries for:
#pragma link C++ namespace ZAna;
#pragma link C++ class ZAna::ElectronCalibAlg+;
#pragma link C++ class ZAna::ElectronSelectionAlg+;
#pragma link C++ class ZAna::ZAnalysisAlg+;

#endif // ZANALYSIS_LINKDEF_H
