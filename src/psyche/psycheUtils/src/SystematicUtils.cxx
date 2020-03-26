#include "SystematicUtils.hxx"

#include <algorithm>
#include <stdio.h>
#include <math.h>


/*
//! [SystematicUtils_eff-like]
\htmlonly
The way efficiency-like systematics are computed is based on studies comparing data and MC predictions in well known control samples. 
Tracking and matching efficiencies can be easily computed using the redundancy between detectors. For example, the TPC2 track efficiency can be computed 
using tracks with segments in FGD1 and FGD2. Similarly, the FGD1 track efficiency can be computed using tracks with segments in TPC1 and TPC2. 
As these special requirements in general are not satisfied in the analysis sample, control samples are necessary. 
As will be shown below, a very useful control sample is the one of through-going muons, consisting of events in which a single muon track crosses the 
entire tracker. These muons, emerging from interactions in the sand surrounding the detector (sand muons), in the P0D or in the magnet, cover a limited 
phase space (in general they have small angle and high energy). Furthermore, as only a single track is present in those events, the effect of other tracks 
that may vary the efficiency cannot be taken into account. For these reasons it is 
possible that the efficiencies computed using those control samples do not correspond to the ones of the analysis samples.
Thus, a model to extrapolate the control sample efficiency to the analysis sample is needed. The simplest model is the one assuming that the ratio between 
the efficiencies in data and MC is the same in both the analysis and control samples. This is a reasonable assumption and will be used in this section. 
The efficiency in the MC analysis sample can be computed using the truth information (given a true GEANT4 trajectory, it is always possible to know whether 
it has been reconstructed or not), while the predicted efficiency in the analysis data sample can be computed as follows: 

  \[ \varepsilon_{data} = \frac{^{CS}_{data}}{\varepsilon^{CS}_{MC}} \varepsilon_{MC} \]

where \(\varepsilon^{CS}_{data}\) and \(\varepsilon^{CS}_{MC}\)  are efficiencies in the control samples and \(\varepsilon_{MC}\) is the efficiency in the MC 
analysis sample. The statistical error of the efficiency computed using the control samples (\(\sigma_{\varepsilon^{CS}_{data}}\) and \(\sigma_{\varepsilon^{CS}_{MC}}\) 
for data and MC, respectively) must be taken into account when propagating the systematic error. 
Thus, the variation for the predicted data efficiency (\(\varepsilon^{\prime}_{data}\)) is given by: 

\[
  \varepsilon^{\prime}_{data} = \frac{\varepsilon^{CS}_{data} + \delta_{data} \cdot \sigma_{\varepsilon^{CS}_{data}}}
  {\varepsilon^{CS}_{MC} + \delta_{MC} \cdot \sigma_{\varepsilon^{CS}_{MC}}}
\]


where \(\delta_{data}\) and \(\delta_{MC}\) are the variations in number of standard deviations in the data and MC control samples, respectively, and can 
assume both positive and negative values.

In order to convert the track-level efficiencies mentioned above into event-level efficiencies, which could be directly applied as event weights, the 
following method is used. For each MC event a loop over all relevant GEANT4 truth trajectories is done. If the trajectory is successfully reconstructed 
it contributes to the efficiency calculation and therefore it is weighted by the ratio between data and MC efficiencies, in such a way that the corrected 
efficiency is the one of the data. The weight to be applied in this case is: 

\[
  W_{\mathrm{eff}} = \frac{\varepsilon^{\prime}_{data}}{\varepsilon_{MC}}
\]


where \(\varepsilon^{\prime}_{data}\) is given by Eq. \ref{eq:effprime}.
If, on the contrary, the truth trajectory is not successfully reconstructed, it contributes to the inefficiency and is weighted by the ratio of data 
and MC inefficiencies. In this case the weight to be applied is given by:

\[
  W_{\mathrm{ineff}} = \frac{1 - \varepsilon^{\prime}_{data}}{1 - \varepsilon_{MC}}
\]
\endhtmlonly

//! [SystematicUtils_eff-like]
*/


//********************************************************************
std::vector<std::vector<Int_t> > systUtils::BinnedParamsOffsets;
//********************************************************************

//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, Float_t variation_mc,  Float_t variation_data, const BinnedParamsParams& params){
    //********************************************************************

    Weight_h weight = 1;
    // P_DATA_ANA=meanDATA/meanMC*meanMCANA
    // weight = P_DATA_ANA/P_MC_ANA
    // weight = meanDATA/meanMC*meanMCANA/meanMCANA
    // weight = meanDATA/meanMC
    // the effweight are indpendent of the efficiency of the analysis sample
    // this is NOT the case for inefficiency weights!

    Float_t PvarDATA=1, PvarMC=1;
    if(variation_data>0) PvarDATA=std::min(params.meanDATA+params.sigmaDATAh*variation_data,(Float_t) 1.);
    else                 PvarDATA=std::max(params.meanDATA+params.sigmaDATAl*variation_data,(Float_t) 0.);
    
    if(variation_mc>0)   PvarMC=std::min(params.meanMC+params.sigmaMCh*variation_mc,(Float_t) 1.);
    else                 PvarMC=std::max(params.meanMC+params.sigmaMCl*variation_mc,(Float_t) 0.);

    if (found){
        if(PvarMC!=0)
            weight.Systematic = PvarDATA/PvarMC;        
        else 
            weight.Systematic = 1.;
        if(params.meanMC!=0)
            weight.Correction = params.meanDATA/params.meanMC;
        else
            weight.Correction = 1.;
    }
    else{
        Float_t Pvar=PvarDATA/PvarMC*params.meanMCANA;
        Float_t Pnom=params.meanDATA/params.meanMC*params.meanMCANA;
        if(Pvar>1) Pvar=1.;
        if((1-params.meanMCANA)!=0){
            weight.Systematic = (1-Pvar)/(1-params.meanMCANA);
            weight.Correction = (1-Pnom)/(1-params.meanMCANA);
        }
        else 
            weight =1;

    }

    return weight;
}


//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, Float_t variation, const BinnedParamsParams& params){
//********************************************************************

    Weight_h weight = 1;
    // P_DATA_ANA=meanDATA/meanMC*meanMCANA
    // weight = P_DATA_ANA/P_MC_ANA
    // weight = meanDATA/meanMC*meanMCANA/meanMCANA
    // weight = meanDATA/meanMC
    // the effweight are indpendent of the efficiency of the analysis sample
    // this is NOT the case for inefficiency weights!

    Float_t PDATA=params.meanDATA, PMC=params.meanMC;
    double ratio = 1;
    if(params.meanMC!=0)
      ratio = params.meanDATA/params.meanMC;

    // this way we take the biggest stat error from error bars.
    double PMC_error    = std::max(params.sigmaMCh,   params.sigmaMCl);
    double PDATA_error  = std::max(params.sigmaDATAh, params.sigmaDATAl);

    //stat err of MC are independant from the stat error of the data...
    double errstat_ratio_2 = ratio*ratio*((PDATA_error*PDATA_error)/(PDATA*PDATA) + (PMC_error*PMC_error)/(PMC*PMC));

    double errsyst_ratio = ratio-1; 
    double errsyst_ratio_2 = errsyst_ratio*errsyst_ratio; 
    double err_ratio     = sqrt(errstat_ratio_2+errsyst_ratio_2);
    // to cross-check that it gives the same error than the other method
    // double err_ratio     = sqrt(pow(errstat_ratio,2));
    
    double Pvar_ratio=ratio+variation*err_ratio;

    // Also not sure about this, shouldn't it be:
    // double Pvar_ratio = variation*err_ratio + 1;
    //
    // Otherwise we are adding the data-MC difference in to the weight twice
    // MR: it depends what you want to do... if you want to compare to the nominal you will count it twice and in this case I agree that it is 1 that should be used
    //     or we decide, to correct the MC so that it looks like the data (as we do for the variation in fact...)
    if (found){
      weight.Systematic = Pvar_ratio;
      weight.Correction = ratio;
    }
    else{
      Float_t Pvar=Pvar_ratio*params.meanMCANA;
      Float_t Pnom=ratio     *params.meanMCANA;
      if(Pvar>1) Pvar=1.;
      if((1-params.meanMCANA)!=0){
        weight.Systematic = (1-Pvar)/(1-params.meanMCANA);
        weight.Correction = (1-Pnom)/(1-params.meanMCANA);
      }
      else 
        weight =1;
      
    }

    return weight;
}



//********************************************************************
Weight_h systUtils::ComputeEffLikeWeight(bool found, const ToyExperiment& toy, Int_t systIndex, Int_t parIndex, const BinnedParamsParams& params){
//********************************************************************
  
#if useNewWeights 
      return systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(systIndex)->Variations[parIndex], params);// new way including data-mc diff
#else
      return systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(systIndex)->Variations[2*parIndex],toy.Variations[2*parIndex+1], params);
#endif

}

//********************************************************************
Int_t systUtils::GetNumberSystParams(BinnedParams& params, bool isEffLike){
//********************************************************************

  if (!isEffLike)
    return params.GetNBins(); 
// To Review whether we want to change the number of bins
//#if useNewWeights 
//  return params.GetNBins(); 
//#else
  return 2 * params.GetNBins(); 
///#endif

}


//********************************************************************
void systUtils::AddBinnedParamsOffsetToSystematic(const SystematicBase& syst, BinnedParams& params, Int_t NParams){
//********************************************************************

  // Get the index of the systematic
  Int_t syst_index = syst.GetIndex();
    
  // Check whether the vector can accomodate the index or already was called 
  if (syst_index >= (Int_t)BinnedParamsOffsets.size())
    // Resize the vector accordingly if needed
    BinnedParamsOffsets.resize(syst_index+1);
 
  // Add zero offset if it is the first try
  if (BinnedParamsOffsets[syst_index].size() == 0)
    BinnedParamsOffsets[syst_index].push_back(0);
  
  // Retrieve the current vector of offsets for the systematic
  // Use the current (last) index of it to associate with a given BinnedParams
  params.SetBinnedParamsIndex(BinnedParamsOffsets[syst_index].size() - 1);
  
  // Use BinnedParams to retrieve the necessary systematic params to
  // set the offset for the next registered BinnedParams 
  BinnedParamsOffsets[syst_index].push_back(BinnedParamsOffsets[syst_index].back() + NParams);

}

//********************************************************************
Int_t systUtils::GetBinnedParamsOffsetForSystematic(const SystematicBase& syst, BinnedParams& params){
//********************************************************************
 if (syst.GetIndex() >= (Int_t)BinnedParamsOffsets.size()){
   std::cout << " systUtils::GetBinnedParamsOffsetForSystematic(): systematic " << syst.Name() 
     << " was not registered,  will abort " << std::endl;
   exit(1);
 }
 
 if (params.GetBinnedParamsIndex() < 0 || params.GetBinnedParamsIndex() >= (Int_t)(BinnedParamsOffsets[syst.GetIndex()].size() - 1)){
   std::cout << " systUtils::GetBinnedParamsOffsetForSystematic(): BinnedParams " << params.GetDataName() 
     << " was not registered for systematic " << syst.GetName() << " will abort " << std::endl;
   exit(1);
 }
 
 // Retrieve the offset
 return BinnedParamsOffsets[syst.GetIndex()][params.GetBinnedParamsIndex()];
}

