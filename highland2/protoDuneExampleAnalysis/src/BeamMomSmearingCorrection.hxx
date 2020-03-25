#ifndef BeamMomSmearingCorrection_h
#define BeamMomSmearingCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"
#include <TRandom3.h>

/* 
This class defines a correction that simulates the MC reconstructed momentum.
This correction is applied to MC only.
*/

class BeamMomSmearingCorrection: public CorrectionBase {

public:
  
  BeamMomSmearingCorrection();

  virtual ~BeamMomSmearingCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);

protected:
  
  Int_t _seedValue;

  TRandom3 _random;  
};

#endif
