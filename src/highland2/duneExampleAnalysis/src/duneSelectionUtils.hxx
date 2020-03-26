#ifndef duneSelectionUtils_h
#define duneSelectionUtils_h

#include "CutUtils.hxx"
#include "ToyBoxDUNE.hxx"
#include "EventBoxId.hxx"

namespace duneSelUtils{

  /// Fill the leading tracks in the event
  bool FindLeadingTracks(const AnaEventC& event, ToyBoxB& ToyBox);
}

#endif
