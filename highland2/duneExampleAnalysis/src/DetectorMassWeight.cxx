#include "DetectorMassWeight.hxx"
#include "DUNEAnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "ToyBoxDUNE.hxx"

//********************************************************************
DetectorMassWeight::DetectorMassWeight():EventWeightBase(),BinnedParams(std::string(getenv("DUNEEXAMPLEANALYSISROOT"))+"/data","DetectorMass", BinnedParams::k1D_SYMMETRIC){
//********************************************************************

  SetNParameters(GetNBins());
  GetParametersForBin(0, _mass_corr, _mass_err);
}

//********************************************************************
Weight_h DetectorMassWeight::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventC, const ToyBoxB& boxB){
//********************************************************************

  (void)eventC;

  // Cast the ToyBox to the appropriate type
  const ToyBoxDUNE& box = *static_cast<const ToyBoxDUNE*>(&boxB); 

  Weight_h eventWeight=1;

  // True vertex should exist
  if (!box.TrueVertex) return eventWeight;         
  
  // Depending on the detector
  if( box.DetectorFV==SubDetId::kSubdet1_1){   // Event selected in Subdet1_1
    if (anaUtils::InDetVolume(SubDetId::kSubdet1_1, box.TrueVertex->Position)){  // TrueVertex in the detector
      eventWeight.Systematic = 1 + _mass_corr + _mass_err * toy.GetToyVariations(_index)->Variations[0];
      eventWeight.Correction = 1 + _mass_corr;
    }
  }
  else if( box.DetectorFV==SubDetId::kSubdet1_2){   // Event selected in Subdet1_2
    if (anaUtils::InDetVolume(SubDetId::kSubdet1_2, box.TrueVertex->Position)){  // TrueVertex in the detector
      eventWeight.Systematic = 1 + _mass_corr + _mass_err * toy.GetToyVariations(_index)->Variations[0];
      eventWeight.Correction = 1 + _mass_corr;
    }
  }

  return eventWeight;
}

