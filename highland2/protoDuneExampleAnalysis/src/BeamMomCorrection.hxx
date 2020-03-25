#ifndef BeamMomCorrection_h
#define BeamMomCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"


/* 
This class defines a correction that affects the reconstructed beam Momentum.
This correction is applied to data only
*/

class BeamMomCorrection: public CorrectionBase {

public:
  
  BeamMomCorrection();

  virtual ~BeamMomCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);
 
};

#endif
