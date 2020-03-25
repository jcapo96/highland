#ifndef PIDUtils_h
#define PIDUtils_h

#include "ConstituentsUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------
  
    /// Function to recompute the pull for a Subdet2 track segment
    Float_t ComputeSubdet2Pull(const AnaSubdet2ParticleB &track, const std::string& particle);

    /// Function to recompute the pull for a Subdet2 track segment for all hypotheses
    void ComputeSubdet2Pull(const AnaSubdet2ParticleB &track, Float_t* pulls);

    /// Function to recompute all the pull for a Subdet2 track segment and save them into the segment
    void RecomputeSubdet2Pulls(AnaSubdet2ParticleB &track);

    /// Compute the expected dEdx for a particle
    Float_t ExpecteddEdx(const AnaParticleMomB &part, ParticleId::ParticleEnum partID);

    /// Compute the expected dEdx for a given momentum and particle type
    Float_t ExpecteddEdx(Float_t mom, ParticleId::ParticleEnum partID);

    /// The likelihood of a track being a given particle hypothesis, based on the
    /// pull values of all Subdet2 segments in the track.
    ///
    /// hypo is one of:
    /// * 0 - muon
    /// * 1 - electron
    /// * 2 - proton
    /// * 3 - pion
    Float_t GetPIDLikelihood(const AnaTrackB& track, Int_t hypo);

    /// Get all likelihoods
    void GetPIDLikelihood(const AnaTrackB&, Float_t* hypo);

    /// Get the likelihood for MIP: (like_mu+like_pi)/(1-like_p)
    Float_t GetPIDLikelihoodMIP(const AnaTrackB &track);

    /// A function that is not currently used, and will be documented when it is.
    Float_t GetPIDPrior(const AnaTrackB& track, Int_t hypo);
}
#endif
