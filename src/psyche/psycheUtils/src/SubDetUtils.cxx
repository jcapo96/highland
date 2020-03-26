#include "SubDetUtils.hxx"


//**************************************************
//Int_t anaUtils::GetNSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************
    // for a given track compute the number of segments in a given detector
  //  return SubDetId::GetNSegmentsInDet(track.Detector, det);
//}

//**************************************************
int anaUtils::GetAllTrajInBunch(const AnaEventB& event, AnaTrueParticleB* trajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      trajInBunch[count] = event.TrueParticles[i];
      ++count;
    }
    return count;
}


//**************************************************
int anaUtils::GetAllChargedTrajInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      
      if(event.TrueParticles[i]->Charge!=0){
        chargedtrajInBunch[count] = event.TrueParticles[i];
        ++count;
      }
    }
    return count;
}

//**************************************************
bool anaUtils::HasTrackUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detector, det)){
            return true;
        }
    }

    return false;
}

//**************************************************
int anaUtils::GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detector, det)){
            selTracks[count] = track;
            count++;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllTracksNotUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (!SubDetId::GetDetectorUsed(track->Detector, det)) {
            selTracks[count] = track;
            ++count;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllTracksUsingOnlyDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (TrackUsesOnlyDet(*track, det)) {
            selTracks[count] = track;
            ++count;
        }
    }

    return count;
}


//**************************************************
int anaUtils::GetAllTracksUsingOnlyDetFromList(const AnaEventB& event, AnaTrackB* selTracks[], 
    SubDetId::SubDetEnum det[], int nDets) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        for (int j = 0; j < nDets; j++){
            if (TrackUsesOnlyDet(*track, det[j])) {
                selTracks[count++] = track;
                break;
            }
        }
    }

    return count;
}


//**************************************************
bool anaUtils::TrackUsesDets(const AnaTrackB& track, SubDetId::SubDetEnum dets[], int nDets) {
//**************************************************
    return SubDetId::GetDetectorArrayUsed(track.Detector, dets, nDets);
}

//**************************************************
bool anaUtils::TrackUsesDet(const AnaTrackB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::GetDetectorUsed(track.Detector, det);
}

//**************************************************
bool anaUtils::TrackUsesDet(const AnaParticleB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::GetDetectorUsed(track.Detector, det);
}

//**************************************************
bool anaUtils::TrackUsesOnlyDet(const AnaTrackB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::TrackUsesOnlyDet(track.Detector, det);
}

//************************************************** 
bool anaUtils::TrackUsesOnlyDet(const AnaParticleB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::TrackUsesOnlyDet(track.Detector, det);
}


//**************************************************
bool anaUtils::HasParticleUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    for (int it = 0; it < event.nParticles; it++) {
        AnaParticleB* particle = static_cast<AnaParticleB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(particle->Detector, det)){
            return true;
        }
    }

    return false;
}

//**************************************************
int anaUtils::GetAllParticlesUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaParticleB* selParticles[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaParticleB* particle = event.Particles[it];
        if (SubDetId::GetDetectorUsed(particle->Detector, det)){
            selParticles[count] = particle;
            count++;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllParticlesNotUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaParticleB* selParticles[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaParticleB* particle = event.Particles[it];
        if (!SubDetId::GetDetectorUsed(particle->Detector, det)) {
            selParticles[count] = particle;
            ++count;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllParticlesUsingOnlyDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaParticleB* selParticles[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaParticleB* particle = event.Particles[it];
        if (ParticleUsesOnlyDet(*particle, det)) {
            selParticles[count] = particle;
            ++count;
        }
    }

    return count;
}


//**************************************************
bool anaUtils::ParticleUsesDets(const AnaParticleB& particle, SubDetId::SubDetEnum dets[], int nDets) {
//**************************************************
    return SubDetId::GetDetectorArrayUsed(particle.Detector, dets, nDets);
}

//**************************************************
bool anaUtils::ParticleUsesDet(const AnaParticleB& particle, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::GetDetectorUsed(particle.Detector, det);
}

//**************************************************
bool anaUtils::ParticleUsesOnlyDet(const AnaParticleB& particle, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::TrackUsesOnlyDet(particle.Detector, det);
}

