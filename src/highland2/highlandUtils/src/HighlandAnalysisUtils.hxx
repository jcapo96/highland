#ifndef HighlandAnalysisUtils_h
#define HighlandAnalysisUtils_h

#include "BaseDataClasses.hxx"
#include "DUNEAnalysisUtils.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "DataClasses.hxx"
#include "InputManager.hxx"
#include "VersionManager.hxx"
#include "ProductionUtils.hxx"


/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace anaUtils{

    template<size_t SIZE, class T> inline size_t size_array(T (&)[SIZE]) {
      return SIZE;
    }

    /// Get all the tracks in a given bunch using a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingDet(const AnaBunchB& bunch, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the given bunch (AnaBunchB) that use the Subdet1 and no Subdet2, sorted by decreasing NHits.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet1AndNoSubdet2(const AnaBunchB& bunch, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the given bunch (AnaBunchB) that use the Subdet1 or Subdet2, not sorted.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet1orSubdet2(const AnaBunchB& bunch, AnaTrackB* selTracks[]);

    /// Get all the tracks in a given bunch using only a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingOnlyDet(const AnaBunchB& bunch, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Function to recompute the pull for a Subdet2 track segment including the kaon hypothesis
    void ComputeSubdet2PullIncludingKaon(const AnaSubdet2ParticleB &track, Float_t* pulls);
    
    /// The likelihood of a track being a given particle hypothesis including kaons, 
    /// based on the pull values of all Subdet2 segments in the track.
    ///
    /// hypo is one of:
    /// * 0 - muon
    /// * 1 - electron
    /// * 2 - proton
    /// * 3 - pion
    /// * 4 - kaon
    Float_t GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Int_t hypo);
    
    /// Get all likelihoods including kaons
    void GetPIDLikelihoodIncludingKaon(const AnaTrackB&, Float_t* hypo);
        
    /// An utility function that takes a set of tracks and returns a corresponding global vertex found in the event
    /// valid GV will be returned in case both tracks are its constituents (may be others),  the first found will be returned
    /// note! it is assumed the GV for an event  are ordered by PrimaryIndex, see TN-49
    AnaVertexB* GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks);

  /// Get all daoughers of a true particle;
  std::vector<AnaTrueParticle*> GetTrueDaughters(std::vector<AnaTrueParticleB*>& trueParticles, const AnaTrueParticle* truePart, bool recursive=false);
  }
#endif


