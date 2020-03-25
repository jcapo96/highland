#include "dEdxCorrection.hxx"
#include "DataClasses.hxx"
#include <cassert>

//#define DEBUG

//********************************************************************
dEdxCorrection::dEdxCorrection():BinnedParams(std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data","dEdx", BinnedParams::k1D_SYMMETRIC){
//********************************************************************

}

//********************************************************************
void dEdxCorrection::Apply(AnaSpillC& spillC){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // No correction for data
  if (!spill.GetIsMC())
    return;

  // Loop over all bunches
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);

    // Loop over all relevant tracks for this variation
    for (UInt_t itrk = 0; itrk<bunch->Particles.size(); itrk++){
      
      AnaParticle* part = static_cast<AnaParticle*>(bunch->Particles[itrk]);
      
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
          part->dEdx[i][j] = original->dEdx[i][j] * mean_corr;
        }
      }
    }
  }
}

