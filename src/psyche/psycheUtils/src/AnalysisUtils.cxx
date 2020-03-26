#include "AnalysisUtils.hxx"
#include <unistd.h>

//**************************************************
Float_t anaUtils::GetSeparationSquared(const Float_t* pos1, const Float_t* pos2){
//**************************************************
    return ((pos1[0] - pos2[0])*(pos1[0] - pos2[0]) + (pos1[1] - pos2[1])*(pos1[1] - pos2[1]) + (pos1[2] - pos2[2])*(pos1[2] - pos2[2]));
}

//********************************************************************
std::vector<Float_t> anaUtils::ComputeWeights(const AnaEventB& event){
//********************************************************************

    std::vector<Float_t> weights;

    //  Only beam flux reweight for the moment
    weights.push_back(event.Weight);

    return weights;
}

//********************************************************************
AnaTrueParticleB* anaUtils::GetTrueParticleByID(const AnaEventB& event, int ID) {
  //********************************************************************

  for (int i = 0; i < event.nTrueParticles; i++) {
    if (event.TrueParticles[i]->ID == ID) {
      return event.TrueParticles[i];
    }
  }

  return NULL;
}

//**************************************************
std::vector<AnaTrackB*> anaUtils::MergeUniqueTracks(const std::vector<AnaTrackB*>& tracks1, const std::vector<AnaTrackB*>& tracks2) {
//**************************************************
    std::vector<AnaTrackB*> merged;

    for (std::vector<AnaTrackB*>::const_iterator it = tracks1.begin(); it != tracks1.end(); it++) {
        if (std::find(merged.begin(), merged.end(), *it) == merged.end()) {
            merged.push_back(*it);
        }
    }

    for (std::vector<AnaTrackB*>::const_iterator it = tracks2.begin(); it != tracks2.end(); it++) {
        if (std::find(merged.begin(), merged.end(), *it) == merged.end()) {
            merged.push_back(*it);
        }
    }

    return merged;
}
