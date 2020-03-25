#ifndef DetectorMassWeight_h
#define DetectorMassWeight_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the detector mass introduced in the MC

class DetectorMassWeight: public EventWeightBase, public BinnedParams {
public:
  
  DetectorMassWeight();
  virtual ~DetectorMassWeight() {}
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
protected:

  Float_t _mass_corr;
  Float_t _mass_err;
  
};

#endif
