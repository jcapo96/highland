#ifndef EventBoxTracker_hxx
#define EventBoxTracker_hxx

#include "BaseDataClasses.hxx"


class EventBoxTracker:public EventBoxB{
 public :

  enum RecObjectGroupEnum{
    kTracksUnassigned=0,
    kTracksWithSubdet2,
    kTracksWithSubdet2InSubdet1_1FV,
    kTracksWithSubdet2InSubdet1_2FV,
    kTracksWithGoodQualitySubdet2InSubdet1_1FV,
    kTracksWithGoodQualitySubdet2InSubdet1_2FV,
    kTracksWithSubdet2AndSubdet1_1,
    kTracksWithSubdet2AndSubdet1_2,
    kTracksWithSubdet1_1AndNoSubdet2,
    kTracksWithSubdet1_2AndNoSubdet2,
    kTracksWithSubdet1_1,
    kTracksWithSubdet1_2,
    kTracksWithSubdet2orSubdet1_1,
    kTracksWithSubdet2orSubdet1_2, 
  };
  
  enum TrueObjectGroupEnum{
    kTrueParticlesUnassigned=0,
    kTrueParticlesChargedInSubdet2InBunch,
    kTrueParticlesChargedInSubdet1_1AndNoSubdet2InBunch,
    kTrueParticlesChargedInSubdet1_2AndNoSubdet2InBunch,
    kTrueParticlesChargedInSubdet2orSubdet1InBunch, 
  };

  EventBoxTracker();
  virtual ~EventBoxTracker();
  
};

#endif
