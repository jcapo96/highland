#include "stoppingKaonSelection.hxx"
#include "EventBoxDUNE.hxx"
#include "protoDuneSelectionUtils.hxx"

//********************************************************************
stoppingKaonSelection::stoppingKaonSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxDUNE) {
//********************************************************************

}

//********************************************************************
void stoppingKaonSelection::DefineSteps(){
//********************************************************************

  // Steps must be added in the right order
  // if "true" is added to the constructor of the step,
  // the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kCut,    "> 0 tracks",         new AtLeastOneTrackCut());     
  AddStep(StepBase::kAction, "find true vertex",   new FindTrueVertexAction_proto());  
  AddStep(StepBase::kAction, "find main track",    new FindMainTrackAction());  
  AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  // action from duneExampleAnalysis package
  AddStep(StepBase::kCut,    "kaon range",         new KaonRangeCut());
  AddStep(StepBase::kCut,    "> 1 track",          new MoreThanOneTrackCut());
  AddStep(StepBase::kCut,    "PIDA",               new PIDACut());  
  
  SetBranchAlias(0,"trunk");
}

//**************************************************
bool AtLeastOneTrackCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

  (void)box;

  // Check we have at least one reconstructed track
  (void)box;
  return (static_cast<AnaEventB*>(&event)->nParticles>0);
}

//**************************************************
bool FindTrueVertexAction_proto::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB); 
  
  if (static_cast<AnaEventB*>(&event)->nTrueVertices>0)
    box.TrueVertex = static_cast<AnaEventB*>(&event)->TrueVertices[0];

  return true;
}

//**************************************************
bool FindMainTrackAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  // The kaon candidate will be the most upstream track (lowest z stating position)
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB); 

  // Get the array of tracks from the event
  AnaParticleB** tracks = static_cast<AnaEventB*>(&event)->Particles;
  int nTracks           = static_cast<AnaEventB*>(&event)->nParticles;
  
  //  Float_t z_min=100000;  
  for (Int_t i=0;i<nTracks; ++i){    
    /*    if (tracks[i]->PositionStart[2]<z_min){
      z_min = tracks[i]->PositionStart[2];
      box.MainTrack = tracks[i];
    }
    */
    if (static_cast<AnaParticle*>(tracks[i])->Charge==-8888){
      box.MainTrack = static_cast<AnaParticle*>(tracks[i]);
      break;
    }
  }
  
  return true;
}

//**************************************************
bool KaonRangeCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  
  Float_t length = static_cast<AnaParticle*>(box.MainTrack)->Length;  
  if (fabs(length-200)<10) return true;
  else return false;
}

//**************************************************
bool MoreThanOneTrackCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

  (void)box;
  return (static_cast<AnaEventB*>(&event)->nParticles>1);
}

//**************************************************
bool PIDACut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  
  Float_t pida = protoDuneSelUtils::ComputePIDA(*static_cast<AnaParticle*>(box.MainTrack));  
  if (fabs(pida-15.)<4) return true;
  else return false;
}


//**************************************************
void stoppingKaonSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxDUNE])
    event.EventBoxes[EventBoxId::kEventBoxDUNE] = new EventBoxDUNE();

  boxUtils::FillLongTracks(event,static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
void stoppingKaonSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kSubdet1_1);
}

