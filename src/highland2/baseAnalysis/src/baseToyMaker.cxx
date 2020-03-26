#include "baseToyMaker.hxx"
#include "Parameters.hxx"

//******************************************************************
baseToyMaker::baseToyMaker(UInt_t seed, bool zero_var):ToyMaker(){
//******************************************************************

  _binnedPDF = new BinnedGaussian(10,0,1);  

  // Set the seed
  _RandomGenerator.SetSeed(seed);

  // Use a random generator for each systematic
  _individualRandomGenerator = (bool) (ND::params().GetParameterI("baseAnalysis.Systematics.IndividualRandomGenerator")); 

  if (_individualRandomGenerator){
    for (UInt_t isyst = 0; isyst<NMAXSYSTEMATICS;isyst++){
      SystematicBase* syst = _systematics[isyst];
      if (!syst) continue;
      
      _seeds[isyst] = syst->GetIndex() + seed*NMAXSYSTEMATICS;
      _RandomGenerators[isyst].SetSeed(_seeds[isyst]);
    }
  }

  // Save the random Seed
  _seed = seed;

  // Apply 0 variation
  _zero_var = zero_var;
}

//******************************************************************
void baseToyMaker::FillToyExperiment(ToyExperiment& toy){
//******************************************************************
  
  // Set the same weight (1) for al toys. This will be later normalized to the number of toys
  Float_t weight = 1.;


  for (UInt_t isyst = 0; isyst<NMAXSYSTEMATICS;isyst++){
    SystematicBase* syst = _systematics[isyst];
    if (!syst) continue;

    // Create the proper structure for the ToyExperiment adding each of the toyVariations
    toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

    // Loop over parameters for this systematic
    for (UInt_t ipar = 0;ipar<syst->GetNParameters();ipar++){
      Float_t var = 0;
      // When the option _zero_var is enabled all variations are 0
      if (!_zero_var){
        if (_individualRandomGenerator){
          if      (syst->PDF() == SystematicBase::kUniform)  var = _RandomGenerators[isyst].Uniform(0.,1.);
          else if (syst->PDF() == SystematicBase::kGaussian) var = _RandomGenerators[isyst].Gaus(0.,1.);	
        }
        else{
          if      (syst->PDF() == SystematicBase::kUniform)  var = _RandomGenerator.Uniform(0.,1.);
          else if (syst->PDF() == SystematicBase::kGaussian) var = _RandomGenerator.Gaus(0.,1.);	
        }
      }      
      toy.SetToyVariation(isyst,ipar,var,weight);
    }
  }
}

