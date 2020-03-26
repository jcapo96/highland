#ifndef FiducialVolumeDefinition_h
#define FiducialVolumeDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace FVDef {

  //----- FiducialVolume volume definitions ---

  /// Fiducial volume cut for Subdet1_1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminSubdet1_1[3];

  /// Fiducial volume cut for Subdet1_1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxSubdet1_1[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminSubdet1_2[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxSubdet1_2[3];

  /// Dump Fiducial Volume definitions
  void DumpFV();
}

#endif
