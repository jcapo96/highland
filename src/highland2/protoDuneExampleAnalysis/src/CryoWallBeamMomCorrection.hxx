#ifndef CryoWallBeamMomCorrection_h
#define CryoWallBeamMomCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"


/* 
This class defines a correction that affects the reconstructed beam Momentum for 
data and MC. It applies the effect of passing through cryostat wall and beam window
*/

class CryoWallBeamMomCorrection: public CorrectionBase {

public:
  
  CryoWallBeamMomCorrection();

  virtual ~CryoWallBeamMomCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);
 
};

#endif
