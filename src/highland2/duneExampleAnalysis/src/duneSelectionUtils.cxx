#include "duneSelectionUtils.hxx"
#include "EventBoxDUNE.hxx"


//********************************************************************
bool duneSelUtils::FindLeadingTracks(const AnaEventC& event, ToyBoxB& boxB){
//********************************************************************

    // Get the highest momentum track (HM),
    // the highest momentum negative track (HMN) and the 
    // highest momentum positive track (HMP)
    // from all tracks passing the quality and fiducial cut. 

    // this function cannot be in anaUtils because it needs the cuts

    // Cast the ToyBox to the appropriate type
    ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

    //    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    EventBoxDUNE::RecObjectGroupEnum groupID = EventBoxDUNE::kLongTracks;

    // Retrieve the EventBoxDUNE
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];

    AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[groupID];
    int nTracks=EventBox->nRecObjectsInGroup[groupID];

    // Reset to NULL all pointers
    box.Ltrack = box.SLtrack = box. TLtrack = NULL;

    Float_t l_hits=0;
    Float_t sl_hits=0;
    Float_t tl_hits=0;

    for (Int_t i=0;i<nTracks; ++i){
      AnaParticleB* track = static_cast<AnaParticleB*>(selTracks[i]);
      
      // Find the Longest and second longest tracks
      if (track->NHits > l_hits){
        if (box.Ltrack){
          box.SLtrack = box.Ltrack;
          sl_hits= l_hits;
        }
        box.Ltrack = track;
        l_hits= track->NHits;
      }
      else if (track->NHits >sl_hits || !box.SLtrack){
        box.SLtrack = track;
        sl_hits= track->NHits;
      }
      else if (track->NHits >tl_hits || !box.TLtrack){
        box.TLtrack = track;
        tl_hits= track->NHits;
      }

    }

    return (box.Ltrack);  
}
