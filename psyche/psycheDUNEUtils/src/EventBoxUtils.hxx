#ifndef BoxUtils_h
#define BoxUtils_h

#include "DUNEAnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxId.hxx"
#include "EventBoxTracker.hxx"

namespace boxUtils{
  
  /// Fill in the EventBox several arrays of tracks with Subdet2
  void FillTracksWithSubdet2(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kSubdet1);

  /// Fill in the EventBox the array of tracks using Subdet1
  void FillTracksWithSubdet1(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kSubdet1);

  /// Fill in the EventBox the array of true tracks passing through Subdet2
  void FillTrajsChargedInSubdet2(AnaEventB& event);

  /// Fill in the EventBox the array of true tracks passing through Subdet1 and no Subdet2
  void FillTrajsChargedInSubdet1AndNoSubdet2(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kSubdet1);

  /// Fill in the EventBox the array of true tracks passing through Subdet1 or Subdet2
  void FillTrajsChargedInSubdet2orSubdet1(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kSubdet1);

}

#endif
