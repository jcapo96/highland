#ifndef dEdxDataCorrection_h
#define dEdxDataCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"
#include "TH1F.h"

/// This class defines a correction that affects the reconstructed TPC Momentum  of an 
/// AnaTpcSegment as well as the global (AnaTrack) track Momentum. It basically applies an additional smearing.
/// This correction is applied to MC only

class dEdxDataCorrection: public CorrectionBase {

public:
  
  dEdxDataCorrection();

  virtual ~dEdxDataCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);

  Float_t ComputeCalibratedDqDx(Float_t prim_dqdx, Float_t prim_hitx);
  
protected:

  TH1F *cali_factor;
  
};

#endif
