#include "SystematicUtils.hxx"

#include <algorithm>
#include <stdio.h>
#include <math.h>


/*
//! [SystematicUtils_eff-like]
\htmlonly
The way efficiency-like systematics are computed is based on studies comparing data and MC predictions in well known control samples. 
Tracking and matching efficiencies can be easily computed using the redundancy between detectors. However, this redundacy cannot be always satisfied in the analysis sample 
and control samples are necessary. In general control samples do not cover the same phase space as the analysis sample and/or have events with a different topology.  
For these reasons it is possible that the efficiencies computed using those control samples do not correspond to the ones of the analysis samples.
Thus, a model to extrapolate the control sample efficiency to the analysis sample is needed. The simplest model is the one assuming that the ratio between 
the efficiencies in data and MC is the same in both the analysis and control samples. This is a reasonable assumption and will be used in this section. 
The efficiency in the MC analysis sample can be computed using the truth information (given a true GEANT4 trajectory, it is always possible to know whether 
it has been reconstructed or not), while the predicted efficiency in the analysis data sample can be computed as follows: 

  \[ \varepsilon_{data} = \frac{\varepsilon^{CS}_{data}}{\varepsilon^{CS}_{MC}} \varepsilon_{MC}  =  r_{CS} \cdot \varepsilon_{MC} \]

where \(\varepsilon^{CS}_{data}\) and \(\varepsilon^{CS}_{MC}\)  are efficiencies in the control samples and \(\varepsilon_{MC}\) is the efficiency in the MC 
analysis sample. As deduced from the previous equation \(r_{CS}\) is the data/MC ratio of efficiencies in the control sample.  

In order to convert the track-level efficiencies mentioned above into event-level efficiencies, which could be directly applied as event weights, the 
following method is used. For each MC event a loop over all relevant GEANT4 truth trajectories is done. If the trajectory is successfully reconstructed 
it contributes to the efficiency calculation and therefore it is weighted by the ratio between data and MC efficiencies, in such a way that the corrected 
efficiency is the one of the data. The weight to be applied in this case is: 

\[
  W_{\mathrm{eff}} = \frac{\varepsilon_{data}}{\varepsilon_{MC}} = r_{CS}
\]

If, on the contrary, the truth trajectory is not successfully reconstructed, it contributes to the inefficiency and is weighted by the ratio of data 
and MC inefficiencies. In this case the weight to be applied is given by:

\[
  W_{\mathrm{ineff}} = \frac{1 - \varepsilon_{data}}{1 - \varepsilon_{MC}} = \frac{1 - r_{CS} \cdot \varepsilon_{MC}}{1 - \varepsilon_{MC}} 
\]


Notice, that while the efficiency weight is independent of the efficiency on the MC analysis sample ( \(\varepsilon_{MC}\)), the inefficiency weight depends on it.  
In the previous equations \( W_{\mathrm{eff}}\) and \( W_{\mathrm{ineff}}\) are nominal weights. When propagating systematics a variation needs to be done. The varied weights would be: 

\[
  W^{\prime}_{\mathrm{eff}} = r^{\prime}_{CS}
\]


\[
  W^{\prime}_{\mathrm{ineff}} = \frac{1 - r^{\prime}_{CS} \cdot \varepsilon_{MC}}{1 - \varepsilon_{MC}} 
\]



The statistical error of the efficiency computed using the control samples (\(\sigma_{\varepsilon^{CS}_{data}}\) and \(\sigma_{\varepsilon^{CS}_{MC}}\) 
for data and MC, respectively) must be taken into account when doing those variations. Efficiencies in data and MC can be varied independently: 

\[
  r^{\prime}_{CS} =  \frac{\varepsilon^{CS}_{data} + \delta_{data} \cdot \sigma_{\varepsilon^{CS}_{data}}}
  {\varepsilon^{CS}_{MC} + \delta_{MC} \cdot \sigma_{\varepsilon^{CS}_{MC}}}
\]

where \(\delta_{data}\) and \(\delta_{MC}\) are the variations in number of standard deviations in the data and MC control samples, respectively, and can 
assume both positive and negative values.

Another option is to do variations in a single parameter instead of two, in the ratio of efficiencies. 

\[
    r^{\prime}_{CS} = r_{CS} + \delta \cdot \sigma_{r_{CS}}
\]

In this case the error on the ratio \(\sigma_{r_{CS}}\) should be computed using the errors on the data and MC control samples. 


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
    // eff_d_ana =eff_d/eff_mc*eff_mc_ana
    // weight = P_DATA_ANA/P_MC_ANA
    // weight = meanDATA/meanMC*meanMCANA/meanMCANA
    // weight = meanDATA/meanMC
    // the effweight are indpendent of the efficiency of the analysis sample
    // this is NOT the case for inefficiency weights!

    Float_t eff_mc_ana = params.meanMCANA;  // efficiency of the MC analysis sample
    Float_t eff_d=params.meanDATA;  // efficiency of the data CONTROL sample
    Float_t eff_mc=params.meanMC;   // efficiency of the MC   CONTROL sample

    double eff_ratio = 1;  // control sample efficiency ratio
    if(eff_mc!=0)
      //eff_ratio = eff_mc/eff_d;
      eff_ratio = eff_d/eff_mc;
    
    // this way we take the biggest stat error from error bars.
    double eff_mc_error = std::max(params.sigmaMCh,   params.sigmaMCl);
    double eff_d_error  = std::max(params.sigmaDATAh, params.sigmaDATAl);

    //stat err of MC are independant from the stat error of the data...
    double errstat_eff_ratio_2 = eff_ratio*eff_ratio*((eff_d_error*eff_d_error)/(eff_d*eff_d) + (eff_mc_error*eff_mc_error)/(eff_mc*eff_mc));

    // TODO. Add a systematic error on that ratio. being conservatives we take the difference to 1 as systematic error
    double errsyst_eff_ratio = eff_ratio-1; 
    double errsyst_eff_ratio_2 = errsyst_eff_ratio*errsyst_eff_ratio; 
    double err_eff_ratio     = sqrt(errstat_eff_ratio_2+errsyst_eff_ratio_2);
    // to cross-check that it gives the same error than the other method
    // double err_eff_ratio     = sqrt(pow(errstat_eff_ratio,2));
    
    double varied_eff_ratio=eff_ratio + variation*err_eff_ratio;

    // Also not sure about this, shouldn't it be:
    // double varied_eff_ratio = variation*err_eff_ratio + 1;
    //
    // Otherwise we are adding the data-MC difference in to the weight twice
    // MR: it depends what you want to do... if you want to compare to the nominal you will count it twice and in this case I agree that it is 1 that should be used
    //     or we decide, to correct the MC so that it looks like the data (as we do for the variation in fact...)
    if (found){
      weight.Systematic = varied_eff_ratio;
      weight.Correction =        eff_ratio;
    }
    else{
      Float_t varied_eff_d_ana=varied_eff_ratio*eff_mc_ana;
      Float_t        eff_d_ana=       eff_ratio*eff_mc_ana;
      if(varied_eff_d_ana>1) varied_eff_d_ana=1.;
      if((1-eff_mc_ana)!=0){
        weight.Systematic = (1-varied_eff_d_ana)/(1-eff_mc_ana);
        weight.Correction = (1-       eff_d_ana)/(1-eff_mc_ana);
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

