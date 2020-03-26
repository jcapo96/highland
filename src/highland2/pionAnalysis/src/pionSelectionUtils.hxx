#ifndef pionSelectionUtils_h
#define pionSelectionUtils_h

#include "CutUtils.hxx"
#include "ToyBoxPD.hxx"
#include "EventBoxId.hxx"

namespace pionSelUtils{

  /// Computes the range momentum
  Float_t ComputeRangeMomentum(double trkrange, int pdg);

  /// Computes the CSDARange
  Float_t ComputeCSDARange(double beammom, int pdg);

  /// Compute PIDA
  Float_t ComputePIDA(const AnaParticle& part);

  /// Computes the kinetic energy
  Float_t ComputeKineticEnergy(const AnaParticle &part);

  /// Compute dedx from dqdx
  Float_t ComputeDeDxFromDqDx(Float_t dqdx);

  /// Compute dqdx from dedx
  Float_t ComputeDqDxFromDeDx(Float_t dedx);

  /// Extrapolate the length of a track to a given Z
  Float_t* ExtrapolateToZ(AnaParticle* part, Float_t z, Float_t* posz);

  /// Compute the average dEdx for several resrange bins
  void ComputeBinnedDeDx(const AnaParticle* part, Float_t max_resrange, Int_t nbins, Float_t** avg_dedx);

  /// Find the beam true particle 
  AnaTrueParticle* FindBeamTrueParticle(const AnaSpillB& spill);  

  // Add part2 to part1
  void AddParticles(AnaParticle* part1, AnaParticle* part2);

  // Compute distances between daughters and vertex
  std::vector<double> ComputeDistanceToVertex(AnaParticle* part);
}

#endif
