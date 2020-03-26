#ifndef SystematicUtils_h
#define SystematicUtils_h

#include "BinnedParams.hxx"
#include "ToyExperiment.hxx"
#include "WeightType.hxx"
#include "SystematicBase.hxx"

#define useNewWeights 1

namespace systUtils{
  
  /// A double vector to give the correspondence between a certain systematic and a vector of offsets that may be applied to various
  /// BinnedParams sources used inside it 
  extern std::vector<std::vector<Int_t> > BinnedParamsOffsets;

  Weight_h ComputeEffLikeWeight(bool found, Float_t variation_mc, Float_t variation_data, const BinnedParamsParams& params);
  Weight_h ComputeEffLikeWeight(bool found, Float_t variation, const BinnedParamsParams& params);

  Weight_h ComputeEffLikeWeight(bool found, const ToyExperiment& toy, Int_t systIndex, Int_t parIndex, const BinnedParamsParams& params); 
  
  /// Get the number of syst params to be associated with this BinnedParams, 
  /// bool flag is in order to diferentiate between two methods of calculating weights for eff-like systs:
  /// (assuming throwing one or two parameters per bin)
  Int_t GetNumberSystParams(BinnedParams& params, bool isEffLike = false);
  
  /// A method to register a BinnedParams to a given systematic 
  /// This will associate a certain offset to a BinnedParams,  depending
  /// on the previous BinnedParams (and hence systematic parameters) already
  /// registered and will further increase the offset. One has to provide the actual 
  /// number of params so to be flexible for some possible non-standard usage 
  void AddBinnedParamsOffsetToSystematic(const SystematicBase& syst, BinnedParams& params, Int_t NParams);
  
  /// A method to retrieve an offset for a given BinnedParams with registered for a given systematic
  Int_t GetBinnedParamsOffsetForSystematic(const SystematicBase& syst, BinnedParams& params);
  

}
#endif




