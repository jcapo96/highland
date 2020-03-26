#include "TruthUtils.hxx"
#include "ConstituentsUtils.hxx"


//**************************************************
Int_t anaUtils::GetNMichelElectrons(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det){
  //**************************************************

  // out of FV
  if ( ! InFiducialVolume(det, trueVertex.Position)) return -999;

  /// particles coming out the vertex
  Int_t NMich=0;

  for (int tit = 0; tit < trueVertex.nTrueParticles; tit++) {
    AnaTrueParticleB* true_track = trueVertex.TrueParticles[tit];

    //if not a pi+/-, a e+/-, a mu+/-
    if( abs(true_track->GParentPDG)!=211) continue;
    if( abs(true_track->ParentPDG)!=13  ) continue;
    if( abs(true_track->PDG)!=11  )       continue;

    NMich++;    
  }

  //  std::cout<<" nb of michel electrons "<<NMich << " " << trueVertex.nTrueParticles <<std::endl;

  return  NMich;
}

//**************************************************
Float_t anaUtils::GetTrueLinearLengthInSubdet2(const AnaTrueParticleB& trueTrack, Float_t& distz){
  //**************************************************

  Float_t dist = -9999999;
  distz = -999;
  for (Int_t idet = 0; idet < trueTrack.nDetCrossings; idet++) {
    //i.e crossing the active part of the tpc
    if (!trueTrack.DetCrossings[idet]->Detector)
      continue;
    if (SubDetId::GetDetectorUsed(trueTrack.DetCrossings[idet]->Detector, SubDetId::kSubdet2) && trueTrack.DetCrossings[idet]->InActive) {
      Float_t dist_temp = anaUtils::GetSeparationSquared(trueTrack.DetCrossings[idet]->ExitPosition, trueTrack.DetCrossings[idet]->EntrancePosition);
      Float_t distz_temp = fabs(trueTrack.DetCrossings[idet]->ExitPosition[2] - trueTrack.DetCrossings[idet]->EntrancePosition[2]);
      if (dist_temp > dist)
        dist = dist_temp;
      if (distz_temp > distz)
        distz = distz_temp;

    }
  }

  return sqrt(dist);
}

//********************************************************************
bool anaUtils::TrueParticleEntersDet(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;
   
  //check that a trajectory crosses volume
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    if(!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det)) continue;

    return true;

  }

  return false;

}

//********************************************************************
bool anaUtils::TrueParticleCrossesSubdet1(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  //for the moment consider the minimum separation in the direction perpendicular to the beam as:
  // 48 mm (iron) mm (we are intersted for tracks that come from inside,  so should cross the first iron block)
  // as it comes from oaAnalysis only one point is saved for SMRD,  as I understand the code it should be the exit point
  // so make a cut in X/Y distance between the point and particular surface inner border  

  if  (det!=SubDetId::kSubdet1 && !SubDetId::IsSubdet1Detector(det))
    return false;


  //loop through det crossings
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    AnaDetCrossingB* cross = track->DetCrossings[idet];
    if(!cross)
      continue;

    // i.e crossing the active part of the Subdet1
    if (!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) || !track->DetCrossings[idet]->InActive)
      continue;

    //the separation should be done using the z position, since the fgd is separated by layer in z,
    //making the z position the reconstructed quantity to be cut on

    //    Float_t sep = fabs(track->DetCrossings[idet]->EntrancePosition[2] - track->DetCrossings[idet]->ExitPosition[2]);

    //should be at least two layers
    //    if(sep>DetDef::fgdXYModuleWidth)
    //      return true;

  }

  return false;

}

//********************************************************************
bool anaUtils::TrueParticleCrossesSubdet2(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  if  (det!=SubDetId::kSubdet2 && !SubDetId::IsSubdet2Detector(det))
    return false;

  Float_t dist=-999999.;

  //loop through det crossings
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    AnaDetCrossingB* cross = track->DetCrossings[idet];
    if(!cross)
      continue;

    // i.e crossing the active part of the Subdet1
    if (!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) || !track->DetCrossings[idet]->InActive)
      continue;
    
    //i.e crossing the active part of the tpc
    if(SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) && track->DetCrossings[idet]->InActive){
        Float_t sep = anaUtils::GetSeparationSquared(track->DetCrossings[idet]->EntrancePosition, track->DetCrossings[idet]->ExitPosition);
        if(sep>dist) dist=sep;
      }
  }
  
  if(dist>62500)//bigger than the ~1/4 of the width of the Subdet2
      return true;
      
  return false;

}

//**************************************************
int anaUtils::GetSubdet1DetCrossed(const AnaTrueParticleB* track, SubDetId::SubDetEnum det[]) {
  //**************************************************
  int count = 0;

  if (!track)
    return count;

  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    // i.e crossing the active part of the Subdet1
    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kSubdet1_1) 
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kSubdet1_1;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kSubdet1_2) 
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kSubdet1_2;

  }

  return count;
}





