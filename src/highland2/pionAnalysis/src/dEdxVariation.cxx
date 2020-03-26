#include "dEdxVariation.hxx"
#include "DataClasses.hxx"
#include "EventBoxPD.hxx"
#include <cassert>

//#define DEBUG

//********************************************************************
dEdxVariation::dEdxVariation(): EventVariationBase(),BinnedParams(std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data","dEdx", BinnedParams::k1D_SYMMETRIC){
//********************************************************************

  // Read the systematic source parameters from the data files
  SetNParameters(GetNBins());
}

//********************************************************************
void dEdxVariation::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  // Loop over all relevant tracks for this variation
  for (Int_t itrk = 0; itrk < box->nRelevantRecObjects; itrk++){

    AnaParticle* part = static_cast<AnaParticle*>(box->RelevantRecObjects[itrk]);

    // The un-corrected particle
    const AnaParticle* original = static_cast<const AnaParticle*>(part->Original);

    if (!part->TrueObject)            continue; //?
    if (!original)                    continue; //?

    AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(part->TrueObject);
    
    // We need the errors part of the data file but as well the relative uncertainty for sigma
    Float_t mean_corr, mean_var;
    Int_t mean_index;
    
    // Note that the momentum changes if the mom resoltion, scale and bfield are also anabled.
    if (!GetBinValues(abs(truePart->PDG), mean_corr,  mean_var,  mean_index))  return;

    for (Int_t i=0;i<3;i++){
      for (Int_t j=0;j<part->NHitsPerPlane[i];j++){
        part->dEdx[i][j] = original->dEdx[i][j] *(1 +  mean_var*toy.GetToyVariations(_index)->Variations[mean_index]/mean_corr);
      }
    }
  }
}

//********************************************************************
bool dEdxVariation::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaParticle* part = static_cast<AnaParticle*>(box->RelevantRecObjects[itrk]);
    const AnaParticle* original = static_cast<const AnaParticle*>(part->Original);
    if (!original)   continue;

    for (Int_t i=0;i<3;i++){
      for (Int_t j=0;j<part->NHitsPerPlane[i];j++){
        part->dEdx[i][j] = original->dEdx[i][j];
      }
    }

  }

  // Don't reset the spill to corrected
  return false;
}

//**************************************************
bool dEdxVariation::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
  //**************************************************

  (void)event;

  // True track should always exist
  if (!track.TrueObject) return false;

  AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(track.TrueObject);

  // only consider true protons, pions, muons and electrons
  if      (abs(truePart->PDG) == 211 ) return true;      
  else if (abs(truePart->PDG) == 2212) return true;
  else if (abs(truePart->PDG) == 13)   return true;
  else if (abs(truePart->PDG) == 11)   return true;
  else if (abs(truePart->PDG) == 321)  return true;

  return false;
}

//********************************************************************
Int_t dEdxVariation::GetRelevantRecObjectGroups(const SelectionBase& sel, Int_t* IDs) const{
  //********************************************************************

  Int_t ngroups=0;
  for (UInt_t b=0; b<sel.GetNBranches(); b++){
    IDs[ngroups++] = EventBoxPD::kLongTracks;
  }

  return ngroups;
}

