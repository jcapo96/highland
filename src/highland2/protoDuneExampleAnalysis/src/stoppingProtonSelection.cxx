#include "stoppingProtonSelection.hxx"
#include "EventBoxDUNE.hxx"
#include "protoDuneSelectionUtils.hxx"

//********************************************************************
stoppingProtonSelection::stoppingProtonSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxDUNE) {
//********************************************************************

}

//********************************************************************
void stoppingProtonSelection::DefineSteps(){
//********************************************************************

  // Steps must be added in the right order
  // if "true" is added to the constructor of the step,
  // the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kAction, "find main track",    new FindBeamTrackAction());  
  AddStep(StepBase::kCut,    "beam protom",        new BeamProtonCut());
  AddStep(StepBase::kCut,    "beam track in TPC",  new CandidateExistsCut());
  AddStep(StepBase::kCut,    "seltrk angle cut",   new BeamProtonAngleCut());
  AddStep(StepBase::kCut,    "proton CSDA range",  new ProtonCSDARangeCut());
  
  SetBranchAlias(0,"trunk");
}

//**************************************************
bool FindBeamTrackAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  // The kaon candidate will be the most upstream track (lowest z stating position)
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB); 

  // Get the array of tracks from the event
  AnaParticleB** tracks = static_cast<AnaEventB*>(&event)->Particles;
  int nTracks           = static_cast<AnaEventB*>(&event)->nParticles;

  AnaBeam* beam = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);
  if (!beam->BeamParticle) return true;

  Int_t ncand=0;
  for (Int_t i=0;i<nTracks; ++i){
    AnaParticle* part = static_cast<AnaParticle*>(tracks[i]);

    if (event.GetIsMC()){ 
      if (static_cast<AnaParticle*>(tracks[i])->Charge==-8888){
        box.MainTrack = static_cast<AnaParticle*>(tracks[i]);
        ncand++;
        break;
      }      
    }
    else{
      Float_t dx = part->PositionStart[0]-beam->BeamParticle->PositionEnd[0];
      Float_t dy = part->PositionStart[1]-beam->BeamParticle->PositionEnd[1];
      //    std::cout << dx << " " << dy << " " << part->Length << " " << part->DirectionStart[2] << std::endl;
      if (dx>-5 && dx<25 && dy>-10 && dy<10 && part->PositionStart[2]<100 && part->DirectionStart[2]>0.7 ){//&& part->Length>10){
        //    if (dx>-5 && dx<25 && dy>-10 && dy<10 && part->PositionStart[2]<100 && part->DirectionStart[2]>0.7 && part->Length>10){   // Cut for run 5210
        box.MainTrack = part;
        ncand++;
        break;
      }
    }
  }
  //  std::cout << ncand << std::endl;
  
  return true;
}

//**************************************************
bool BeamProtonCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)boxB;
  
  AnaBeam* beam = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);

  if (event.GetIsMC()){
    if (beam->BeamParticle){
      if (beam->BeamParticle->TrueObject)
        if (static_cast<AnaTrueParticle*>(beam->BeamParticle->TrueObject)->PDG==2212) return true;
    }
    return false;
  }
  else{
    if (beam->TOF>170 && beam->TOF<210) return true;
    else return false;    
  }
}

//**************************************************
bool CandidateExistsCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB); 
  
  return box.MainTrack;
}

//**************************************************
bool BeamProtonAngleCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  
  //Cast the particle and define vector
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  double beamdir[3] = {-0.18,-0.20,0.96};//beam direction
  double partdir[3] = {part->DirectionStart[0],part->DirectionStart[1],part->DirectionStart[2]};

  //Cut
  if((beamdir[0]*partdir[0]+beamdir[1]*partdir[1]+beamdir[2]*partdir[2])>0.93)return true;
  else return false;  
}

//**************************************************
bool ProtonCSDARangeCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  
  //check if it exists a beam particle
  AnaBeam* beam         = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);
  AnaParticleMomB* beamPart = beam->BeamParticle;
  if(beamPart){
    Float_t mom = beamPart->Momentum;
    //if (beamPart->TrueObject) mom =  static_cast<AnaTrueParticle*>(beamPart->TrueObject)->Momentum;
    Float_t length = static_cast<AnaParticle*>(box.MainTrack)->Length;
    Float_t csdarange = protoDuneSelUtils::ComputeCSDARange(mom*1000, 2212);
    if (csdarange<=0) return false;
    if (length/csdarange>0.69 && length/csdarange<1.05) return true;
  }

  return false;  
}

//**************************************************
bool ProtonPIDACut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
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
void stoppingProtonSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxDUNE])
    event.EventBoxes[EventBoxId::kEventBoxDUNE] = new EventBoxDUNE();

  boxUtils::FillLongTracks(event,static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
void stoppingProtonSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kSubdet1_1);
}

