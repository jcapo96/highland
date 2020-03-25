#ifndef CutUtils_h
#define CutUtils_h

#include "DUNEAnalysisUtils.hxx"
#include "BaseDataClasses.hxx"

namespace cutUtils{

  /// Whether the closest Subdet2 segment has > 18 nodes.
  bool TrackQualityCut(const AnaParticleB& part);

  /// Whether the track starts in the specified fiducial volume. FGD, Subdet1_1, Subdet1_2
  /// and P0D fiducial volumes are supported, as well as TrECal and DsECal active 
  /// volumes. The detector volumes are defined in DetDef::fgd1min and similar 
  /// variables. The amount by which the detector volume is shrunk to form the 
  /// fiducial volume is specified by FVdefminSubdet1_1 and similar variables.
  bool FiducialCut(const AnaParticleB& track, const SubDetId::SubDetEnum det = SubDetId::kSubdet1_1);
  bool FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det = SubDetId::kSubdet1_1);

  /// Whether the track is muon-like, as defined for the NuMu analysis.
  bool MuonPIDCut(const AnaParticleB& part);

  /// Whether the track is proton-like
  bool ProtonPIDCut(const AnaParticleB& part);

  /// Retuns true if the two tracks start within the input tolerance 
  bool CommonVertexCut(const AnaParticleB& track1, const AnaParticleB& track2, int cutx, int cuty, int cutz);
}

#endif
