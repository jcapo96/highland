#include "BrokenTrackDataCorrection.hxx"
#include "DataClasses.hxx"
#include "pionSelectionUtils.hxx"
#include <cassert>

//#define DEBUG

//********************************************************************
BrokenTrackDataCorrection::BrokenTrackDataCorrection(){
//********************************************************************

}

//********************************************************************
void BrokenTrackDataCorrection::Apply(AnaSpillC& spillC){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // No correction for data
  if (spill.GetIsMC()) return;

  // Loop over all bunches
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);

    // Loop over all relevant tracks in this buch
    for (UInt_t itrk = 0; itrk<bunch->Particles.size(); itrk++){      
      AnaParticle* part = static_cast<AnaParticle*>(bunch->Particles[itrk]);
      
      // The un-corrected particle
      const AnaParticle* original = static_cast<const AnaParticle*>(part->Original);
      if (!original) continue; //?

      // Require mother to end at APA border
      if (part->PositionEnd[2]>225 && part->PositionEnd[2]<233 && part->Daughters.size()>0){

        // Loop over daughter particles
        for (std::vector<AnaRecObjectC*>::iterator it=part->Daughters.begin();it!=part->Daughters.end();it++){
          AnaParticle* dau = static_cast<AnaParticle*>(*it);

          // Take the first one close and aligned to mother
          if (fabs(part->PositionEnd[0]-dau->PositionStart[0])<20 &&
              fabs(part->PositionEnd[1]-dau->PositionStart[1])<20 &&
              fabs(part->PositionEnd[2]-dau->PositionStart[2])<20 &&
              fabs(part->DirectionEnd[0]-dau->DirectionStart[0])<0.3 &&
              fabs(part->DirectionEnd[1]-dau->DirectionStart[1])<0.3 &&
              fabs(part->DirectionEnd[2]-dau->DirectionStart[2])<0.3){

            // Add the daughter to the mother particle
            pionSelUtils::AddParticles(part,dau);
            break;
          }
        }
      }      
    }
  }
}
