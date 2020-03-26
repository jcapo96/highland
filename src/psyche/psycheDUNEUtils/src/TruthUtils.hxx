#ifndef TruthUtils_h
#define TruthUtils_h

#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    /// Get the number of true michel electrons
    Int_t GetNMichelElectrons(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det = SubDetId::kSubdet1_1);

    /// Return the true linear length traversed in the Subdet2
    Float_t GetTrueLinearLengthInSubdet2(const AnaTrueParticleB& trueTrack, Float_t& distz);
      
    /// Whether a true track crosses a Subdet1 so to be able to deposit charge in at least two layers
    bool TrueParticleCrossesSubdet1(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kSubdet1);
    
    /// Whether a true track crosses a Subdet2 so to be able to reconstruct an object 
    bool TrueParticleCrossesSubdet2(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kSubdet2);

    /// Whether a true track enters a given sub-detector 
    bool TrueParticleEntersDet(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det);
    
    /// Subdet1 detectors crossed
    int GetSubdet1DetCrossed(const AnaTrueParticleB*       track, SubDetId::SubDetEnum det[]);   
}
#endif
