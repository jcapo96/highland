#include "BeamMomSmearingCorrection.hxx"
#include "DUNEAnalysisUtils.hxx"
#include "DataClasses.hxx"
#include "EventBoxDUNE.hxx"
#include <cassert>
#include "Parameters.hxx"

//#define DEBUG

//********************************************************************
BeamMomSmearingCorrection::BeamMomSmearingCorrection(){
//********************************************************************

  _seedValue = ND::params().GetParameterI("protoDuneExampleAnalysis.Corrections.SeedValue");
  // Random generator
  _random.SetSeed(_seedValue);  
}

//********************************************************************
void BeamMomSmearingCorrection::Apply(AnaSpillC& spillC){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // No correction for data
  if (!spill.GetIsMC())
    return;

  // Get beam
  AnaBeam* beam = static_cast<AnaBeam*>(spill.Beam);

  // Get particle
  AnaParticle* part = static_cast<AnaParticle*>(beam->BeamParticle);
  if (!part)                        return; //?
    
  float smearing = 0.04;
      
  part->Momentum = static_cast<AnaTrueParticle*>(part->TrueObject)->Momentum + _random.Gaus(0,smearing);
  beam->BeamMomentum = part->Momentum;
  
  //std::cout << "old momentum " << static_cast<AnaTrueParticle*>(part->TrueObject)->Momentum << " || new momentum " << beam->BeamMomentum << std::endl;

}


