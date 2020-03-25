#include "pionSelection.hxx"
#include "EventBoxDUNE.hxx"
#include "protoDuneSelectionUtils.hxx"

//********************************************************************
pionSelection::pionSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxDUNE) {
//********************************************************************

}

//********************************************************************
void pionSelection::DefineSteps(){
//********************************************************************

  // Steps must be added in the right order
  // if "true" is added to the constructor of the step,
  // the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kAction, "find main track",    new FindBeamPionTrackAction());  
  AddStep(StepBase::kCut,    "beam pion",          new BeamPionCut());
  AddStep(StepBase::kCut,    "beam is track",      new BeamIsTrackCut());
  AddStep(StepBase::kCut,    "beam geometric",     new BeamPionGeometricCut());
  AddStep(StepBase::kCut,    "pion track end",     new PionEndsAPA3Cut());
  AddStep(StepBase::kCut,    "no pion daughter",   new NoPionDaughterCut());

  //Add a split to the trunk with 2 branches.
  AddSplit(2);
  //First branch is for pi0 showers
  AddStep(0, StepBase::kCut, "pi0 showers",        new PionCexCut());
  //Second branch is for pi absortion
  AddStep(1, StepBase::kCut, "pi absorption",      new PionAbsorptionCut());
  // Set the branch aliases to the three branches
  SetBranchAlias(0,"pi0 shower",     0);
  SetBranchAlias(1,"pi absorption",  1);

  // By default the preselection correspond to cuts 0-4
  SetPreSelectionAccumLevel(4);
}

//**************************************************
bool FindBeamPionTrackAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

   
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB); 

  // Get the array of tracks from the event
  AnaParticleB** tracks = static_cast<AnaEventB*>(&event)->Particles;
  int nTracks           = static_cast<AnaEventB*>(&event)->nParticles;

  box.MainTrack = NULL;
  
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
bool BeamPionCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)boxB;
  AnaBeam* beam = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);

  if (event.GetIsMC()){
    if(beam->BeamParticle){
      if (beam->BeamParticle->TrueObject){
        if (static_cast<AnaTrueParticle*>(beam->BeamParticle->TrueObject)->PDG==211 || static_cast<AnaTrueParticle*>(beam->BeamParticle->TrueObject)->PDG==-13) return true;
      }
    }
    else return false;
  }
  else{
    for(int i = 0; i < (int)beam->PDGs.size(); i++){
      if (beam->PDGs[i] == 211)return true;
    }
    return false;
  }
}

//**************************************************
bool BeamIsTrackCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
  
  (void)event;

  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  //if the main track exists cast it
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  
  //if the seltrk is a track, accept it
  if (part->ReconPDG[0]==13) return true;
  else return false;
}

//**************************************************
bool BeamPionGeometricCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  //For MC from Owen Goodwins studies
  double xlow = -3.; double xhigh = 7.; double ylow = -8.; double yhigh = 7.; double zlow = 27.5; double zhigh = 32.5; double coslow = 0.93;

  //For Data from Owen Goodwin
  double data_xlow = 0., data_xhigh = 10., data_ylow= -5., data_yhigh= 10., data_zlow=30., data_zhigh=35., data_coslow=.93;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  //if the main track exists cast it
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);

  if (event.GetIsMC()){ 

    AnaBeam* beam = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);
    if (!beam->BeamParticle) return false;
    //if the beam part exists cast it
    AnaParticle* beampart = static_cast<AnaParticle*>(beam->BeamParticle);
    AnaTrueParticle* truepart = static_cast<AnaTrueParticle*>(beampart->TrueObject);
    if (!truepart) return false;
    Float_t mcdx   = part->PositionStart[0]-(truepart->Position[0]-truepart->Position[2]*(truepart->Direction[0]/truepart->Direction[2]));
    Float_t mcdy   = part->PositionStart[1]-(truepart->Position[1]-truepart->Position[2]*(truepart->Direction[1]/truepart->Direction[2]));
    Float_t mcdz   = part->PositionStart[2];
    Float_t mcdcos = part->DirectionStart[0]*truepart->Direction[0]+part->DirectionStart[1]*truepart->Direction[1]+part->DirectionStart[2]*truepart->Direction[2];
    if(mcdx < xlow || mcdx > xhigh || mcdy < ylow || mcdy > yhigh || mcdz < zlow || mcdz > zhigh || mcdcos < coslow)return false;
    else return true;
  }
  else{
    AnaBeam* beam = static_cast<AnaBeam*>(static_cast<AnaEventB*>(&event)->Beam);
    if(beam->nMomenta != 1 || beam->nTracks != 1)return false;
    if (!beam->BeamParticle) return false;
    //if the beam part exists cast it
    AnaParticle* beampart = static_cast<AnaParticle*>(beam->BeamParticle);

    Float_t dadx   = part->PositionStart[0]-beampart->PositionEnd[0];
    Float_t dady   = part->PositionStart[1]-beampart->PositionEnd[1];
    Float_t dadz   = part->PositionStart[2];
    Float_t dadcos = part->DirectionStart[0]*beampart->DirectionEnd[0]+part->DirectionStart[1]*beampart->DirectionEnd[1]+part->DirectionStart[2]*beampart->DirectionEnd[2];
    if(dadx < data_xlow || dadx > data_xhigh || dady < data_ylow || dady > data_yhigh || dadz < data_zlow || dadz > data_zhigh || dadcos < data_coslow)return false;
    else return true;
  }
}

//**************************************************
bool PionEndsAPA3Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  double cutAPA3_Z = 226.;

  // Cast the ToyBox to the appropriate type
  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  if(part->PositionEnd[2] < cutAPA3_Z)return true;
  else return false;  
}

//**************************************************
bool NoPionDaughterCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;
  double cut_daughter_track_distance = 10;
  bool noPion = true;

  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  //if the main track exists cast it
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  //if the maint track has no daughters has no pion daughters
  if((int)part->Daughters.size()==0)return true;
  //compute distance between daughters and vertex
  std::vector<double> distance = protoDuneSelUtils::ComputeDistanceToVertex(part);

  //dummy implementation
  for(int i = 0; i < (int)part->Daughters.size(); i++){
    if(static_cast<AnaParticle*>(part->Daughters[i])->UniqueID != -999 && static_cast<AnaParticle*>(part->Daughters[i])->CNNscore[0] > 0.35 && static_cast<AnaParticle*>(part->Daughters[i])->Chi2Proton/static_cast<AnaParticle*>(part->Daughters[i])->Chi2ndf > 50 && distance[i] < cut_daughter_track_distance){
      noPion = false;
      break;
    }
  }
  return noPion;  
}

//**************************************************
bool PionCexCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
  
  (void)event;

  bool cex = false;
  double cut_daughter_shower_distance_low = 2.;
  double cut_daughter_shower_distance_high = 100.;
  int cut_nHits_shower_low = 12;
  int cut_nHits_shower_high = 1000;

  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  //if the main track exists cast it
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  //compute distance between daughters and vertex
  std::vector<double> distance = protoDuneSelUtils::ComputeDistanceToVertex(part);
  
  for(int i = 0; i < (int)part->Daughters.size(); i++){
    if(static_cast<AnaParticle*>(part->Daughters[i])->UniqueID != -999 && static_cast<AnaParticle*>(part->Daughters[i])->CNNscore[0] < 0.35 && static_cast<AnaParticle*>(part->Daughters[i])->Chi2Proton/static_cast<AnaParticle*>(part->Daughters[i])->Chi2ndf > 50 && distance[i] > cut_daughter_shower_distance_low && distance[i] < cut_daughter_shower_distance_high && static_cast<AnaParticle*>(part->Daughters[i])->NHits > cut_nHits_shower_low && static_cast<AnaParticle*>(part->Daughters[i])->NHits < cut_nHits_shower_high){
      cex = true;
      break;
    }
  }
  return cex;
}

//**************************************************
bool PionAbsorptionCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  bool abs = true;
  double cut_daughter_shower_distance_low = 2.;
  double cut_daughter_shower_distance_high = 100.;
  int cut_nHits_shower_low = 12;
  int cut_nHits_shower_high = 1000;

  ToyBoxPD& box = *static_cast<ToyBoxPD*>(&boxB);   
  if (!box.MainTrack) return false;
  //if the main track exists cast it
  AnaParticle* part = static_cast<AnaParticle*>(box.MainTrack);
  //compute distance between daughters and vertex
  std::vector<double> distance = protoDuneSelUtils::ComputeDistanceToVertex(part);
  
  for(int i = 0; i < (int)part->Daughters.size(); i++){
    if(static_cast<AnaParticle*>(part->Daughters[i])->UniqueID != -999 && static_cast<AnaParticle*>(part->Daughters[i])->CNNscore[0] < 0.35 && distance[i] > cut_daughter_shower_distance_low && distance[i] < cut_daughter_shower_distance_high && static_cast<AnaParticle*>(part->Daughters[i])->NHits > cut_nHits_shower_low && static_cast<AnaParticle*>(part->Daughters[i])->NHits < cut_nHits_shower_high){
      abs = false;
      break;
    }
  }
  return abs;
}

//**************************************************
void pionSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxDUNE])
    event.EventBoxes[EventBoxId::kEventBoxDUNE] = new EventBoxDUNE();

  boxUtils::FillLongTracks(event,static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
void pionSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kSubdet1_1);
}

