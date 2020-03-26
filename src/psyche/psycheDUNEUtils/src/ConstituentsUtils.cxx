#include "ConstituentsUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "BaseDataClasses.hxx"
#include "DetectorDefinition.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "SubDetId.hxx"

#include <bitset>

namespace anaUtils {}


//**************************************************
Int_t anaUtils::GetOneSegmentPerSubdet2(AnaSubdet2ParticleB* in[], Int_t nseg_in, AnaSubdet2ParticleB* out[]) {
//**************************************************

  // This method takes as input all Subdet2 segments in a track and returns an array with only one segment per Subdet2, the one with more nodes

    Int_t nhits_max[3]={0,0,0};
    Int_t itrack_nhits_max[3]={-1,-1,-1};
    for(Int_t iseg=0; iseg< nseg_in; iseg++){
      Int_t subdet2 = SubDetId::GetSubdet2(in[iseg]->Detector)-1;
      if (in[iseg]->NHits > nhits_max[subdet2]){
        nhits_max[subdet2] = in[iseg]->NHits;
        itrack_nhits_max[subdet2]=iseg;
      }      
    }

    int nseg_out = 0;    
    for(Int_t i=0;i< 3;i++){
      if (itrack_nhits_max[i]!=-1)
        out[nseg_out++]=in[itrack_nhits_max[i]];
    }
    
    return nseg_out;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetClosestSubdet2(const AnaTrackB& track){
//**************************************************

    // returns the Subdet2 closest to the track start point
    // simply use the linear distance for now 

  /*
    SubDetId::SubDetEnum subdet2_closest = SubDetId::kInvalid;

    Float_t dist = 9999999.;

    for(int i = 0; i < track.nSubdet1Segments; ++i){
        AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
        Float_t dist_tmp = GetSeparationSquared(track.PositionStart, subdet2_track->PositionStart);
        if(dist_tmp < dist){
            dist = dist_tmp;
            subdet2_closest = SubDetId::GetSubdetectorEnum(subdet2_track->Detector);
        }
    }
    return subdet2_closest;  
  */

  AnaParticleB* Subdet2Segment = anaUtils::GetSegmentWithMostNodesInClosestSubdet2(track);
  
  if (Subdet2Segment) return SubDetId::GetSubdetectorEnum(Subdet2Segment->Detector);
  else return SubDetId::kInvalid;
}

//**************************************************
int anaUtils::GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* segments[]){
//**************************************************
    if (det == SubDetId::kInvalid) {
        return 0;
    }
    if(!SubDetId::GetDetectorUsed(track.Detector, det)){
        return 0;
    }

    int count = 0;

    // Return segments for complete detector subsystems (all Subdet2 etc.) first
    switch(det){
        case SubDetId::kSubdet2 :
            std::copy(&track.Subdet1Segments[0], &track.Subdet1Segments[track.nSubdet1Segments], segments);
            return track.nSubdet1Segments;
            break;
        case SubDetId::kSubdet1 :
            std::copy(&track.Subdet1Segments[0], &track.Subdet1Segments[track.nSubdet1Segments], segments);
            return track.nSubdet1Segments;
            break;
        default:

            if(SubDetId::IsSubdet2Detector(det)){
                for(int i = 0; i < track.nSubdet1Segments; ++i){
                    AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
                    if(SubDetId::GetDetectorUsed(subdet2_track->Detector, det)){
                        segments[count] = subdet2_track;
                        ++count;
                    }
                }
            }
            else if(SubDetId::IsSubdet1Detector(det)){
                for(int i = 0; i < track.nSubdet1Segments; ++i){
                    AnaSubdet1ParticleB* subdet1_track = track.Subdet1Segments[i];
                    if(SubDetId::GetDetectorUsed(subdet1_track->Detector, det)){
                        segments[count] = subdet1_track;
                        ++count;
                    }
                }
            }
            return count;
    }
    return count;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestSubdet2(const AnaTrackB& track){
//**************************************************

  return GetSegmentWithMostNodesInClosestSubdet2(track, track.PositionStart);
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestSubdet2(const AnaTrackB& track, const Float_t* pos){
//**************************************************

    int subdet2_closest = SubDetId::kInvalid;
    int subdet2 = SubDetId::kInvalid;

    AnaParticleB* subtrack[3] = {NULL, NULL, NULL};

    Float_t dist = 9999999.;
    int nHits[3] = {0,0,0};

    for(int i = 0; i < track.nSubdet1Segments; ++i){
        AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
        Float_t dist_tmp = std::min(
            GetSeparationSquared(pos, subdet2_track->PositionStart), 
            GetSeparationSquared(pos, subdet2_track->PositionEnd)
            );
        subdet2 = SubDetId::GetSubdet2(subdet2_track->Detector);
       
        if(dist_tmp < dist){
            dist = dist_tmp;
            subdet2_closest = subdet2;
        }
        // Subdet2 number is not zero ordered
        if(subdet2_track->NHits > nHits[subdet2-1]){
            nHits[subdet2-1] = subdet2_track->NHits;
            subtrack[subdet2-1] = subdet2_track;
        }
    }

    if(subdet2_closest != (int)SubDetId::kInvalid) return subtrack[subdet2_closest-1];

    return NULL;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

    if (det == SubDetId::kInvalid) {
        return NULL;
    }
    if(!SubDetId::GetDetectorUsed(track.Detector, det)){
        return NULL;
    }

    int nHits = 0;
    AnaParticleB* subtrack = NULL;

    switch(det){
        case SubDetId::kSubdet2 :
            for(int i = 0; i < track.nSubdet1Segments; ++i){
                AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
                if(subdet2_track->NHits > nHits){
                    nHits = subdet2_track->NHits;
                    subtrack = subdet2_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kSubdet1 :
            for(int i = 0; i < track.nSubdet1Segments; ++i){
                AnaSubdet1ParticleB* subdet1_track = track.Subdet1Segments[i];
                if(subdet1_track->NHits > nHits){
                    nHits = subdet1_track->NHits;
                    subtrack = subdet1_track;
                }
            }
            return subtrack;
            break;
        default:
            if(SubDetId::IsSubdet2Detector(det)){
                for(int i = 0; i < track.nSubdet1Segments; ++i){
                    AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
                    if(SubDetId::GetDetectorUsed(subdet2_track->Detector, det)){
                        if(subdet2_track->NHits > nHits){
                            nHits = subdet2_track->NHits;
                            subtrack = subdet2_track;
                        }
                    }
                }
                return subtrack;
            }
            else if(SubDetId::IsSubdet1Detector(det)){
                for(int i = 0; i < track.nSubdet1Segments; ++i){
                    AnaSubdet1ParticleB* subdet1_track = track.Subdet1Segments[i];
                    if(SubDetId::GetDetectorUsed(subdet1_track->Detector, det)){
                        if(subdet1_track->NHits > nHits){
                            nHits = subdet1_track->NHits;
                            subtrack = subdet1_track;
                        }
                    }
                }
                return subtrack;
            }
            return NULL;
    }
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

    if(SubDetId::IsSubdet1Detector(det)){
        for(int i = 0; i < track.nSubdet1Segments; ++i){
            AnaSubdet1ParticleB* subdet2_track = track.Subdet1Segments[i];
            if(SubDetId::GetDetectorUsed(subdet2_track->Detector, det)){
                return subdet2_track;
            }
        }
    }
    if(SubDetId::IsSubdet2Detector(det)){
        for(int i = 0; i < track.nSubdet2Segments; ++i){
            AnaSubdet2ParticleB* subdet1_track = track.Subdet2Segments[i];
            if(SubDetId::GetDetectorUsed(subdet1_track->Detector, det)){
                return subdet1_track;
            }
        }
    }
    return NULL;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetDetector(const Float_t* pos){
//**************************************************  
    for(Int_t it = 0; it != static_cast<Int_t>(SubDetId::kInvalidSubdetector); it++) {
        SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(it);
        if (anaUtils::InDetVolume(det, pos))
            return det;
    }
    return SubDetId::kInvalid;
}

//**************************************************
bool anaUtils::InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************

    Float_t null[3] = {0.,0.,0.};
 
    //account for a case when a "general" volume is provided
    switch(det){
        case SubDetId::kSubdet1:
            return (InFiducialVolume(SubDetId::kSubdet1_1, pos, null, null) || InFiducialVolume(SubDetId::kSubdet1_2, pos, null, null));
            break;
        case SubDetId::kSubdet1_1:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet1_1, pos, null, null);
            break;
        case SubDetId::kSubdet1_2:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet1_2, pos, null, null);
            break;
        case SubDetId::kSubdet2:
            return (
                InFiducialVolume(SubDetId::kSubdet2_1, pos, null, null) ||
                InFiducialVolume(SubDetId::kSubdet2_2, pos, null, null) ||
                InFiducialVolume(SubDetId::kSubdet2_3, pos, null, null) 
                );
            break;
        case SubDetId::kSubdet2_1:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_1, pos, null, null);
            break;
        case SubDetId::kSubdet2_2:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_2, pos, null, null);
            break;
        case SubDetId::kSubdet2_3:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_3, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InDetVolume() No Volume set for " << det << std::endl;
            return false;
            break;
    }

}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************


    


    Float_t null[3] = {0.,0.,0.};
    switch(det){
        case SubDetId::kSubdet1:
            return (InFiducialVolume(SubDetId::kSubdet1_1,pos) || InFiducialVolume(SubDetId::kSubdet1_2,pos));
            break;
        case SubDetId::kSubdet1_1:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet1_1, pos, FVDef::FVdefminSubdet1_1, FVDef::FVdefmaxSubdet1_1);
            break;
        case SubDetId::kSubdet1_2:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet1_2, pos, FVDef::FVdefminSubdet1_2, FVDef::FVdefmaxSubdet1_2);
            break;
        case SubDetId::kSubdet2:
            return (
                InFiducialVolume(SubDetId::kSubdet2_1, pos, null, null) ||
                InFiducialVolume(SubDetId::kSubdet2_2, pos, null, null) ||
                InFiducialVolume(SubDetId::kSubdet2_3, pos, null, null) 
                );
            break;
        case SubDetId::kSubdet2_1:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_1, pos, null, null);
            break;
        case SubDetId::kSubdet2_2:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_2, pos, null, null);
            break;
        case SubDetId::kSubdet2_3:
            return anaUtils::InFiducialVolume(SubDetId::kSubdet2_3, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InFiducialVolume() No Fiducial Volume set for " << det << std::endl;
            return false;
            break;
    }
}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax){
//**************************************************

    switch(det){
        case SubDetId::kSubdet1_1:
            if (pos[0] > DetDef::Subdet1_1min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::Subdet1_1max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::Subdet1_1min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::Subdet1_1max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::Subdet1_1min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::Subdet1_1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kSubdet1_2:
            if (pos[0] > DetDef::Subdet1_2min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::Subdet1_2max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::Subdet1_2min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::Subdet1_2max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::Subdet1_2min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::Subdet1_2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kSubdet2_1:
            if (pos[0] > DetDef::Subdet2_1min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::Subdet2_1max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::Subdet2_1min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::Subdet2_1max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::Subdet2_1min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::Subdet2_1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kSubdet2_2:
            if (pos[0] > DetDef::Subdet2_2min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::Subdet2_2max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::Subdet2_2min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::Subdet2_2max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::Subdet2_2min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::Subdet2_2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kSubdet2_3:
            if (pos[0] > DetDef::Subdet2_3min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::Subdet2_3max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::Subdet2_3min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::Subdet2_3max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::Subdet2_3min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::Subdet2_3max[2]-FVdefmax[2])
                return true;
            break;
        default:
            std::cout << "Error:  anaUtils::InFiducialVolume() given an unknown subdetector enumeration: " << det << std::endl;

    }

    return false;

}


//**************************************************
int anaUtils::GetAllChargedTrajInSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;
      Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the subdet2
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet2) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        // 30* 30 originally
        if((dist)>900 && event.TrueParticles[i]->Momentum>5){//bigger than 3 Subdet2 hits (30*30 is faster that sqrt(dist)), and momentum > 5 MeV 
	  chargedtrajInBunch[count] = event.TrueParticles[i];
	  ++count;
        }
    }
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInSubdet1InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************
    /* 
     * we need here to select in-Subdet1 tracks that potentially should have been reconstruced
     * by Subdet1 iso recon (the function name is confusing);
     * this involves putting some min requirements for the true tracks:
     * since Subdet1 iso recon requires a track to extend for at least 4 Z layers (i.e. having hits in five consequitive layers)
     * in order to be reconstruced this requirement should be applied for the true tracks as well.
     * In principle one can use the geometry info to retrieve layers that true entrance and exit point correspond to
     * but it can be time taking,  so we use an approximation: a true trajectory should have a length in Z at least of the one of 4 Subdet1 layers:
     * so 4 cm

     */

  int count = 0;
  for (Int_t i = 0; i < event.nTrueParticles; i++) {
    if(!event.TrueParticles[i]->TrueVertex) continue;
    if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
    if(event.TrueParticles[i]->Charge==0)continue;
    Float_t dist = -9999999;
    for (Int_t idet = 0; idet < event.TrueParticles[i]->nDetCrossings; idet++) {
      // i.e crossing the active part of the Subdet1
      if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet1)){
        if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, det) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
          //the separation should be done using the z position, since the subdet1 is separated by layer in z,
          //making the z position the reconstructed quantity to be cut on
          Float_t sep = fabs(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition[2] - event.TrueParticles[i]->DetCrossings[idet]->ExitPosition[2]);
          if(sep>dist) dist=sep;
        }
      }
    }
    // apply the cut (this cut is only valid for Subdet1!)
    if (dist > 40){
      chargedtrajInBunch[count] = event.TrueParticles[i];
      ++count;
    }
  }

  return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInSubdet1AndNoSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************  
    AnaTrueParticleB* trajInBunchInSubdet1x[NMAXTRUEPARTICLES];
    Int_t ntrajInBunchInSubdet1x = anaUtils::GetAllChargedTrajInSubdet1InBunch(event, trajInBunchInSubdet1x,det);

    Int_t count = 0;
    for (Int_t i = 0; i < ntrajInBunchInSubdet1x; i++) {
      Float_t dist=-999999.;
      for(Int_t idet=0;idet<trajInBunchInSubdet1x[i]->nDetCrossings;idet++){
        //i.e crossing the active part of the subdet2
        if(SubDetId::GetDetectorUsed(trajInBunchInSubdet1x[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet2) && trajInBunchInSubdet1x[i]->DetCrossings[idet]->InActive) {
          Float_t sep = GetSeparationSquared(trajInBunchInSubdet1x[i]->DetCrossings[idet]->EntrancePosition, trajInBunchInSubdet1x[i]->DetCrossings[idet]->ExitPosition);
          
          if(sep>dist) dist=sep;
        }
      }
      
      bool cross_subdet2 = false;
      // 250*250 originally
      if(dist>62500)//bigger than the ~1/4 of the width of the Subdet2
        cross_subdet2 = true;
      
      if (!cross_subdet2){
        chargedtrajInBunch[count] = trajInBunchInSubdet1x[i];
        ++count;
      }
    }
    
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInSubdet2Subdet1InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i<event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

        Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of one of the Subdet1s
          if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet1)){
            for(Int_t idet1=0;idet1<event.TrueParticles[i]->nDetCrossings;idet1++){
              //look for Subdet2_1-Subdet1_1, Subdet1_1-Subdet2_2, Subdet2_2-Subdet1_2, Subdet1_2-Subdet2_3 trajectories
              if((SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet1_1) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kSubdet2_1) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kSubdet2_2))) || 
                 (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet1_2) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kSubdet2_2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kSubdet2_3)))) 
                {
                  Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet1]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet1]->ExitPosition);
                  if(sep>dist) dist=sep;
                }
            }
          }
        }
        
        // 10*10 originally, now 100
        if(dist>100){
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }
    }

    return count;

}

//**************************************************
int anaUtils::GetAllBigEnoughChargedTrajInSubdet2InBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

      Float_t dist=0;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the subdet2
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kSubdet2) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        //250*250 originally 
        if(dist>62500){//bigger than the ~1/4 of the width of the Subdet2
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }

    }
    return count;
}

//**************************************************
int anaUtils::GetAllTracksUsingSubdet1AndNoSubdet2(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum subdet1det) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (!SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet2) && SubDetId::GetDetectorUsed(track->Detector, subdet1det)) {
            selTracks[count] = track;
            ++count;
        }
    }

    // Sort by decreasing number of hits
    std::sort(&selTracks[0], &selTracks[count], AnaParticleB::CompareNHits);

    return count;
}


//**************************************************
int anaUtils::GetAllTracksUsingSubdet1orSubdet2(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet2) || SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet1)) {
            selTracks[count] = track;
            ++count;
        }
    }

    // Sort by decreasing number of hits
    std::sort(&selTracks[0], &selTracks[count], AnaParticleB::CompareNHits);

    return count;
}

//**************************************************
bool anaUtils::HasTrackUsingSubdet2(const AnaEventB& event) {
//**************************************************
  return anaUtils::HasTrackUsingDet(event, SubDetId::kSubdet2);
}

//**************************************************
int anaUtils::GetAllTracksUsingSubdet1(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kSubdet1, selTracks);
}

//**************************************************
int anaUtils::GetAllTracksUsingSubdet2(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kSubdet2, selTracks);
}

//**************************************************
int anaUtils::GetNTracksUsingSubdet2AndDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    int count = 0;

    SubDetId::SubDetEnum dets[2];
    dets[0] = SubDetId::kSubdet2;
    dets[1] = det;

    // Loop over all tracks
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (anaUtils::TrackUsesDets(*track, dets, 2)){
            count ++;
        }
    }

    return count;
}
