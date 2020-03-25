#ifndef BrokenTrackDataCorrection_h
#define BrokenTrackDataCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"

class BrokenTrackDataCorrection: public CorrectionBase {

public:
  
  BrokenTrackDataCorrection();

  virtual ~BrokenTrackDataCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);

protected:
  
};

#endif
