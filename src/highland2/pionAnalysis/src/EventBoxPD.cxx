#include "EventBoxPD.hxx"
#include "CutUtils.hxx"

//********************************************************************
EventBoxPD::EventBoxPD():EventBoxB(){
//******************************************************************** 

}

//********************************************************************
EventBoxPD::~EventBoxPD(){
//********************************************************************

} 

//********************************************************************
void boxUtils::FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxPD];

  // TODO. We need a better method to get the tracks (returning a variable size array). Otherwise we have to guess how many tracks we have
  AnaTrackB* selTracks[NMAXPARTICLES*10];
  int nLong =anaUtils::GetAllTracksUsingDet(event,det, selTracks);

  nLong = std::min(nLong, (Int_t)NMAXPARTICLES);

  EventBox->nRecObjectsInGroup[EventBoxPD::kAllTracks]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxPD::kAllTracks], nLong);
    
  EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracks]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxPD::kLongTracks], nLong);

  EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracksInFV]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxPD::kLongTracksInFV], nLong);


  //loop over fgd tracks
  for (Int_t i=0;i<nLong; ++i){
    AnaParticleB* track = selTracks[i];
    EventBox->RecObjectsInGroup[EventBoxPD::kAllTracks][EventBox->nRecObjectsInGroup[EventBoxPD::kAllTracks]++] = track;
    if (track->NHits>20){      
      EventBox->RecObjectsInGroup[EventBoxPD::kLongTracks][EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracks]++] = track;
      if (cutUtils::FiducialCut(*track, det))
        EventBox->RecObjectsInGroup[EventBoxPD::kLongTracksInFV][EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracksInFV]++] = track;
    }
  }

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxPD::kAllTracks], 
                        EventBox->nRecObjectsInGroup[EventBoxPD::kAllTracks],
                        nLong);
  
  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxPD::kLongTracks], 
                        EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracks],
                        nLong);

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxPD::kLongTracksInFV], 
                        EventBox->nRecObjectsInGroup[EventBoxPD::kLongTracksInFV],
                        nLong);
}

