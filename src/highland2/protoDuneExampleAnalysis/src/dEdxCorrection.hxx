#ifndef dEdxCorrection_h
#define dEdxCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"
#include "BinnedParams.hxx"


/// This class defines a correction that affects the reconstructed TPC Momentum  of an 
/// AnaTpcSegment as well as the global (AnaTrack) track Momentum. It basically applies an additional smearing.
/// This correction is applied to MC only

class dEdxCorrection: public BinnedParams, public CorrectionBase {

public:
  
  dEdxCorrection();

  virtual ~dEdxCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);
 
};

#endif
