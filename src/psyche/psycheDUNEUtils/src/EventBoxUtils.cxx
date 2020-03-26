#include "EventBoxUtils.hxx"
#include "CutUtils.hxx"

//********************************************************************
void boxUtils::FillTracksWithSubdet2(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processSubdet1_1 = false;
  bool processSubdet1_2 = false;
  bool processSubdet2  = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kSubdet1_1 || det==SubDetId::kSubdet1) && !EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1]) processSubdet1_1 = true;
  if ((det==SubDetId::kSubdet1_2 || det==SubDetId::kSubdet1) && !EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2]) processSubdet1_2 = true;

  if (!processSubdet1_1 && !processSubdet1_2) return;

  AnaTrackB* selTracks[NMAXPARTICLES];
  int nSubdet2 = anaUtils::GetAllTracksUsingSubdet2(event, selTracks);

  if (!EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2]){
    processSubdet2= true;
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2], nSubdet2);
  }

  if (processSubdet1_1){
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV], nSubdet2);

    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV], nSubdet2);

    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1], nSubdet2);
    if(!EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]){
      EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1], nSubdet2);
    }else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1], 
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]+nSubdet2,
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]);
    }

  }
  if (processSubdet1_2){
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV], nSubdet2);

    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV], nSubdet2);

    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2], nSubdet2);
    if(!EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]){
      EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2], nSubdet2);
    }else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2], 
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]+nSubdet2,
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]);
    }
  }

  
  bool inSubdet1_1 = false;
  bool inSubdet1_2 = false;

  //loop over tpc tracks
  for (Int_t i=0;i<nSubdet2; ++i){
    AnaTrackB* track = selTracks[i];
    if (track->nSubdet2Segments==0){
      std::cout << "Warning. This track has no Subdet2 segments" << std::endl;
      continue;
    }

    if (processSubdet2){
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2]++] = track;
      if(processSubdet1_1)
        EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]++] = track;
      if(processSubdet1_2)
        EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]++] = track;
    }
    // Does it have Subdet1 as well ? here not...
    if (processSubdet1_1 && anaUtils::TrackUsesDet(*track, SubDetId::kSubdet1_1))
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1]++] = track;

    if (processSubdet1_2 && anaUtils::TrackUsesDet(*track, SubDetId::kSubdet1_2))
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2]++] = track;
        
    // Apply the fiducial cut
    if (processSubdet1_1)
      inSubdet1_1 = cutUtils::FiducialCut(*track, SubDetId::kSubdet1_1);
    if (processSubdet1_2 && !inSubdet1_1 )
      inSubdet1_2 = cutUtils::FiducialCut(*track, SubDetId::kSubdet1_2);

    if      (inSubdet1_1)
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV]++] = track;
    else if (inSubdet1_2)
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV]++] = track;

    // Apply the track quality cut
    if (!cutUtils::TrackQualityCut(*track)) continue;
    
    if      (inSubdet1_1)
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV]++] = track;
    else if (inSubdet1_2)
      EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV]++] = track;

    
  }
  if (processSubdet1_1){
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_1FV],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_1FV],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_1],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]+nSubdet2);

  }
  if (processSubdet1_2){
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2InSubdet1_2FV],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualitySubdet2InSubdet1_2FV],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2AndSubdet1_2],
                          nSubdet2);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]+nSubdet2);

  }

}

//********************************************************************
void boxUtils::FillTracksWithSubdet1(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processSubdet1_1 = false;
  bool processSubdet1_2 = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kSubdet1_1 || det==SubDetId::kSubdet1) && !EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2]) processSubdet1_1 = true;
  if ((det==SubDetId::kSubdet1_2 || det==SubDetId::kSubdet1) && !EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2]) processSubdet1_2 = true;
  if (!processSubdet1_1 && !processSubdet1_2) return;

  AnaTrackB* selTracks[NMAXPARTICLES];
  int nSubdet1 =anaUtils::GetAllTracksUsingDet(event,det, selTracks);
  
  if (processSubdet1_1){
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2], nSubdet1);
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1], nSubdet1);
    if(!EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]){
      EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1], nSubdet1);
    }
    else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1], 
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]+nSubdet1,
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]);
    }
  }
  if (processSubdet1_2){
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2], nSubdet1);
    EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2], nSubdet1);
    if(!EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]){
      EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2], nSubdet1);
    }
    else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2],
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]+nSubdet1,
                            EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]);
    }
  }
  

  //loop over fgd tracks
  for (Int_t i=0;i<nSubdet1; ++i){
    AnaTrackB* track = selTracks[i];
    if (track->nSubdet1Segments==0){
      std::cout << "Warning. This track has no Subdet1 segments" << std::endl;
      continue;
    }
    if (processSubdet1_1){
      if (SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet1_1)){
        if (!SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet2)){
          EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2]++] = track;
          //tracks with Subdet2 already saved, we only need to add those that are not in the tpc
          EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]++] = track;
        }
        EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1]++] = track;
      }
    }
    if (processSubdet1_2){
      if (SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet1_2)){
        if (!SubDetId::GetDetectorUsed(track->Detector, SubDetId::kSubdet2)){
          EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2]++] = track;
          //tracks with Subdet2 already saved, we only need to add those that are not in the tpc
          EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]++] = track;
        }
        EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2][EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2]++] = track;
        
      }
    }
  }
  if (processSubdet1_1){
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2], 
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1AndNoSubdet2],
                          nSubdet1);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1],         
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_1],
                          nSubdet1);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1],    
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_1]+nSubdet1);
  }
  if (processSubdet1_2){
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2], 
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2AndNoSubdet2],
                          nSubdet1);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2],         
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet1_2],
                          nSubdet1);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2],    
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2],
                          EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithSubdet2orSubdet1_2]+nSubdet1);
  }

}

//********************************************************************
void boxUtils::FillTrajsChargedInSubdet2(AnaEventB& event){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // Don't fill it when already filled by other selection
  if (EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2InBunch]) return;

  AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
  Int_t nSubdet2 = anaUtils::GetAllChargedTrajInSubdet2InBunch(event, trajs);
  if(NMAXTRUEPARTICLES<(UInt_t)nSubdet2) nSubdet2=NMAXTRUEPARTICLES;
  anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2InBunch],nSubdet2);
  anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2InBunch],trajs,nSubdet2);
  EventBox->nTrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2InBunch] = nSubdet2;
}

//********************************************************************
void boxUtils::FillTrajsChargedInSubdet1AndNoSubdet2(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processSubdet1_1 = false;
  bool processSubdet1_2 = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kSubdet1_1 || det==SubDetId::kSubdet1) && !EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_1AndNoSubdet2InBunch]) processSubdet1_1 = true;
  if ((det==SubDetId::kSubdet1_2 || det==SubDetId::kSubdet1) && !EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_2AndNoSubdet2InBunch]) processSubdet1_2 = true;

  if (!processSubdet1_1 && !processSubdet1_2) return;

  if (processSubdet1_1){
    AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
    Int_t nSubdet1 = anaUtils::GetAllChargedTrajInSubdet1AndNoSubdet2InBunch(event, trajs, SubDetId::kSubdet1_1);
    if(NMAXTRUEPARTICLES<(UInt_t)nSubdet1) nSubdet1=NMAXTRUEPARTICLES;    
    anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_1AndNoSubdet2InBunch],nSubdet1);
    anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_1AndNoSubdet2InBunch],trajs,nSubdet1);
    EventBox->nTrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_1AndNoSubdet2InBunch] = nSubdet1;
  }
  if (processSubdet1_2){
    AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
    Int_t nSubdet1 = anaUtils::GetAllChargedTrajInSubdet1AndNoSubdet2InBunch(event, trajs, SubDetId::kSubdet1_2);
    if(NMAXTRUEPARTICLES<(UInt_t)nSubdet1) nSubdet1=NMAXTRUEPARTICLES;    
    anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_2AndNoSubdet2InBunch],nSubdet1);
    anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_2AndNoSubdet2InBunch],trajs,nSubdet1);
    EventBox->nTrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet1_2AndNoSubdet2InBunch] = nSubdet1;
  }
}

//********************************************************************
void boxUtils::FillTrajsChargedInSubdet2orSubdet1(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************
  
  (void)det;
  
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // Don't fill it when already filled by other selection
  if (EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2orSubdet1InBunch]) return;

  AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
  Int_t count = anaUtils::GetAllChargedTrajInSubdet2Subdet1InBunch(event, trajs);
  if((UInt_t)count>NMAXTRUEPARTICLES) count = NMAXTRUEPARTICLES;
  anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2orSubdet1InBunch],count);
  anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2orSubdet1InBunch],trajs,count);
  EventBox->nTrueObjectsInGroup[EventBoxTracker::kTrueParticlesChargedInSubdet2orSubdet1InBunch] = count;
}

