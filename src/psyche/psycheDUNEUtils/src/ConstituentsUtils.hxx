#ifndef ConstituentsUtils_h
#define ConstituentsUtils_h

#include "DetectorDefinition.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------

    /// Whether the specified position is in the volume of the given detector.
    /// Accepted detectors are kSubdet1_1, kSubdet1_2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
    bool InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos);

    /// Whether the specified position is in the fiducial volume of the given detector.
    /// Accepted detectors are kSubdet1_1, kSubdet1_2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
    /// The fiducial volume is specified using the FVdefmin and FVdefmax vectors. These
    /// vectors are the amount of space to *exclude*, compared to the nominal side of
    /// the detector.
    bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax);
    bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos);


    /// Return the detector in which the position is.
    SubDetId::SubDetEnum GetDetector(const Float_t* pos);


    Int_t GetOneSegmentPerSubdet2(AnaSubdet2ParticleB* in[], Int_t nseg, AnaSubdet2ParticleB* out[]);

    /// For tracks that start in the Subdet1, get the closest Subdet2 in the direction of the track.
    SubDetId::SubDetEnum GetClosestSubdet2(const AnaTrackB& track);

    /// Get the vector of AnaParticle segment that uses the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Return the number of entries in the input array, the number of tracks found
    int GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* selTracks[]);

    /// Method to get the subtrack with most nodes in a given detector
    AnaParticleB* GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    /// Combined function to address NuMu selection needs as efficiently as possible - gets the Subdet2 segment with the most nodes in the Subdet2 closest to the start of the global track
    AnaParticleB* GetSegmentWithMostNodesInClosestSubdet2(const AnaTrackB& track);

    /// Smae as above but closest to a given point
    AnaParticleB* GetSegmentWithMostNodesInClosestSubdet2(const AnaTrackB& track, const Float_t* pos);

    /// Get the AnaParticle segment that uses the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    AnaParticleB* GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    bool HasTrackUsingSubdet2(const AnaEventB& event);

    /// Get all the true traj. in the bunch that are charged and crosses the Subdet2 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* traj[]);
    /// Get all the true traj. in the bunch that are charged and crosses the Subdet1
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInSubdet1InBunch(const AnaEventB& event, AnaTrueParticleB* traj[],SubDetId::SubDetEnum det);
    /// Get all the true traj. in the bunch that are charged and crosses the Subdet2 with a length > 1/4 of the Subdet2 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllBigEnoughChargedTrajInSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
    /// Get all the true traj. in the bunch that are charged and crosses the Subdet2 and the Subdet1 (Subdet2_1-Subdet1_1, Subdet1_1-Subdet2_2, Subdet2_2-Subdet1_2, Subdet1_2-Subdet2_3)
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInSubdet2Subdet1InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
    /// Get all the true traj. in the bunch that are charged and crosses the the Subdet1 but not the Subdet2
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInSubdet1AndNoSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det);

    /// Get all the tracks using a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Get all the tracks not using a specific detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksNotUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the Subdet1, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet1(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the Subdet2, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet2(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the Subdet2 or the Subdet1, sorted by decreasing number of hits
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet1orSubdet2(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the Subdet1 and no Subdet2, sorted by decreasing NHits.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSubdet1AndNoSubdet2(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum fgddet);

    /// Returns the number of tracks using both the Subdet2 and the subdetector 'det'.
    int GetNTracksUsingSubdet2AndDet(const AnaEventB& event, SubDetId::SubDetEnum det);

    /// Return an integer corresponding to the array index of the track in the old local detector enumeration
    int GetLocalDetEnum(SubDetId::SubDetEnum det, int i);

}
#endif

//  LocalWords:  ifndef
