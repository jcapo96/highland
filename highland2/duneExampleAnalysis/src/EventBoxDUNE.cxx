#include "EventBoxDUNE.hxx"
#include "CutUtils.hxx"

//********************************************************************
EventBoxDUNE::EventBoxDUNE():EventBoxB(){
//******************************************************************** 

}

//********************************************************************
EventBoxDUNE::~EventBoxDUNE(){
//********************************************************************

} 

//********************************************************************
void boxUtils::FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

  // TODO. We need a better method to get the tracks (returning a variable size array). Otherwise we have to guess how many tracks we have
  AnaTrackB* selTracks[NMAXPARTICLES*10];
  int nLong =anaUtils::GetAllTracksUsingDet(event,det, selTracks);

  nLong = std::min(nLong, (Int_t)NMAXPARTICLES);

  EventBox->nRecObjectsInGroup[EventBoxDUNE::kAllTracks]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxDUNE::kAllTracks], nLong);
    
  EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracks]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxDUNE::kLongTracks], nLong);

  EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracksInFV]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxDUNE::kLongTracksInFV], nLong);


  //loop over fgd tracks
  for (Int_t i=0;i<nLong; ++i){
    AnaParticleB* track = selTracks[i];
    EventBox->RecObjectsInGroup[EventBoxDUNE::kAllTracks][EventBox->nRecObjectsInGroup[EventBoxDUNE::kAllTracks]++] = track;
    if (track->NHits>20){      
      EventBox->RecObjectsInGroup[EventBoxDUNE::kLongTracks][EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracks]++] = track;
      if (cutUtils::FiducialCut(*track, det))
        EventBox->RecObjectsInGroup[EventBoxDUNE::kLongTracksInFV][EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracksInFV]++] = track;
    }
  }

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxDUNE::kAllTracks], 
                        EventBox->nRecObjectsInGroup[EventBoxDUNE::kAllTracks],
                        nLong);
  
  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxDUNE::kLongTracks], 
                        EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracks],
                        nLong);

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBoxDUNE::kLongTracksInFV], 
                        EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracksInFV],
                        nLong);
}

