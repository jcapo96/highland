#ifndef EventBoxDUNE_hxx
#define EventBoxDUNE_hxx

#include "BaseDataClasses.hxx"
#include "EventBoxUtils.hxx"


class EventBoxDUNE:public EventBoxB{
 public :

  enum RecObjectGroupEnum{
    kTracksUnassigned=0,
    kAllTracks,      // All tracks
    kLongTracks,      // Tracks with more than 20 hits
    kLongTracksInFV   // Tracks with more than 20 hits in FV
  };
  
  enum TrueObjectGroupEnum{
    kTrueParticlesUnassigned=0,
    kTrueParticlesChargedInTPCInBunch,
  };

  EventBoxDUNE();
  virtual ~EventBoxDUNE();
};


namespace boxUtils{

  /// Fill in the EventBox several arrays of tracks with Subdet2
  void FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kSubdet1);

}

#endif
