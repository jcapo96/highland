#ifndef HighlandAnalysisUtils_h
#define HighlandAnalysisUtils_h

#include "BaseDataClasses.hxx"
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

        
    /// An utility function that takes a set of tracks and returns a corresponding global vertex found in the event
    /// valid GV will be returned in case both tracks are its constituents (may be others),  the first found will be returned
    /// note! it is assumed the GV for an event  are ordered by PrimaryIndex, see TN-49
    AnaVertexB* GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks);

  /// Get all daoughers of a true particle;
  std::vector<AnaTrueParticle*> GetTrueDaughters(std::vector<AnaTrueParticleB*>& trueParticles, const AnaTrueParticle* truePart, bool recursive=false);
  }
#endif


