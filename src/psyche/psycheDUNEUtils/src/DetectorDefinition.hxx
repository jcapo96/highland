#ifndef DetectorDefinition_h
#define DetectorDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace DetDef {

  //----- Detector volume definitions ---

  /// Minimum of the SUBDET1_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet1_1min[3];

  /// Maximum of the SUBDET1_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet1_1max[3];

  /// Minimum of the SUBDET1_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet1_2min[3];

  /// Maximum of the SUBDET1_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet1_2max[3];



  /// Minimum of the SUBDET2_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_1min[3];

  /// Maximum of the SUBDET2_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_1max[3];

  /// Minimum of the SUBDET2_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_2min[3];

  /// Maximum of the SUBDET2_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_2max[3];

  /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_3min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Subdet2_3max[3];
  
  
  /// Dump volume definitions
  void DumpVolumes();  
}

#endif
