#include "CryoWallBeamMomCorrection.hxx"
#include "DUNEAnalysisUtils.hxx"
#include "DataClasses.hxx"
#include "EventBoxDUNE.hxx"
#include <cassert>

//#define DEBUG

//********************************************************************
CryoWallBeamMomCorrection::CryoWallBeamMomCorrection(){
//********************************************************************

}

//********************************************************************
void CryoWallBeamMomCorrection::Apply(AnaSpillC& spillC){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // Get beam
  AnaBeam* beam = static_cast<AnaBeam*>(spill.Beam);
  if (!beam)                        return; //?
    
  double corr = 0.0215;
      
  beam->BeamMomentumInTPC = beam->BeamMomentum - corr;
  
  //std::cout << "old momentum " << beam->BeamMomentum << " || new momentum " << beam->BeamMomentumInTPC << std::endl;

}


