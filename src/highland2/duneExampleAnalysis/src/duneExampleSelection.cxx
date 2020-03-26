#include "duneExampleSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxDUNE.hxx"
#include "EventBoxUtils.hxx"
#include "SystematicUtils.hxx"
#include "duneSelectionUtils.hxx"
#include "DataClasses.hxx"

//********************************************************************
duneExampleSelection::duneExampleSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxDUNE) {
//********************************************************************

  // Initialize systematic tuning parameters
  //  systTuning::Initialize();
}

//********************************************************************
void duneExampleSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
    AddStep(StepBase::kCut,    "fiducial",           new FiducialCut(SubDetId::kSubdet1_2));  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "find true vertex",   new FindTrueVertexAction());  
    //    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kCut,    "muon PID",           new MuonPIDCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2. 
    // It means that if any of the three first cuts (0,1,2) is not passed 
    // the loop over toys will be broken ===> A single toy will be run
    SetPreSelectionAccumLevel(2);

    // Step and Cut numbers needed by CheckRedoSelection
    _MuonPIDCutIndex            = GetCutNumber("muon PID");
    _MuonPIDStepIndex           = GetStepNumber("muon PID");
    _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
    _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void duneExampleSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kSubdet1_2);
}

//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

  (void)box;

  // Check we have at least one reconstructed track in the Subdet2
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxDUNE];  
  return (EventBox->nRecObjectsInGroup[EventBoxDUNE::kLongTracks]>0);
}

//**************************************************
bool FiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // This cut check the existence of track with position inside the Fiducial Volume and 
  // with a minimum number of hits

  // Cast the ToyBox to the appropriate type
  ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

  if (!box.MainTrack) return false;

  return cutUtils::FiducialCut(*box.MainTrack, _detector);
}


//**************************************************
bool TrackQualityFiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // This cut check the existence of track with position inside the Fiducial Volume and 
  // with a minimum number of hits

  // Cast the ToyBox to the appropriate type
  ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

  return (box.Vertex);
}

//**************************************************
bool MuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

    if (!box.MainTrack) return false;
    if (static_cast<AnaParticle*>(box.MainTrack)->AveragedEdx>0.5) return false;
    return true;
}

//**************************************************
bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

    // reset the vertex 
    if (box.Vertex) delete box.Vertex;
    box.Vertex = NULL;

    if (!box.MainTrack) return false;


    // Create a vertex 
    box.Vertex = new AnaVertex();
    anaUtils::CreateArray(box.Vertex->Particles, 1);

    // With a single track for the moment
    box.Vertex->nParticles = 0;
    box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;

    // and position the start position of the Longest track
    for(int i = 0; i < 4; ++i){
        box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
    }

    if (box.Vertex->Particles[0]->GetTrueParticle())
      box.Vertex->TrueVertex = box.Vertex->Particles[0]->GetTrueParticle()->TrueVertex;


    return true;
}

//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

    if (!box.Vertex) return false;
    box.TrueVertex = box.Vertex->TrueVertex;

    return true;
}


//**************************************************
bool FindLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxDUNE& box = *static_cast<ToyBoxDUNE*>(&boxB); 

    // Find the longest tracks among all tracks passing the quality criteria
    duneSelUtils::FindLeadingTracks(event,box);

    // For this selection the main track is the Longest track
    box.MainTrack = box.Ltrack;

    return true;
}

//**************************************************
bool duneExampleSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)recObj;

  /*
  AnaParticle* part = static_cast<AnaParticle*>(recObj);
 
  // True track should always exist
  if (!part->TrueObject) return false;


  if (systId == SystId::kFgdPid){
    if (!anaUtils::TrackUsesOnlyDet(*track,static_cast<SubDetId::SubDetEnum>(GetDetectorFV()))) return false;
  }
  else if(systId == SystId::kTpcClusterEff){
    if (track->nSubdet2Segments == 0) return false;
    //    AnaSubdet2ParticleB* tpcTrack = static_cast<AnaSubdet2ParticleB*>(track->Subdet2Segments[0]);
    AnaSubdet2ParticleB* Subdet2Track = static_cast<AnaSubdet2ParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestSubdet2(*track));      
    if (!Subdet2Track) return false;
    // use 17 and 18 for numu analysis, 35 and 36 for nue analyses
    if (Subdet2Track->NNodes == 17 || Subdet2Track->NNodes == 18)
      return true;  
    return false;
  } 
  */
  return true;
}

//**************************************************
bool duneExampleSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)trueObj;

  /*
  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);


  // Subdet2 track eff
  if (systId == SystId::kTpcTrackEff){
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }  
  else if (systId == SystId::kFgdTrackEff){
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }  
  else if (systId == SystId::kFgdHybridTrackEff){    
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }
  */
  return true;
}

//**************************************************
bool duneExampleSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)track;



  /*
  // Cast the ToyBox to the appropriate type
  const ToyBoxDUNE& box = *static_cast<const ToyBoxDUNE*>(&boxB); 


  if(systId == SystId::kChargeIDEff){
    if (!systTuning::ChargeIDEff_MAINTRACK_ONLY) return true;
    // At first order the inclusive selection only depends on the charge ID of the muon candidate 
    if (track != box.MainTrack) return false;
  } 
  */
  return true;
}

//**************************************************
bool duneExampleSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)trueObj;

  /*
  // Cast the ToyBox to the appropriate type
  const ToyBoxDUNE& box = *static_cast<const ToyBoxDUNE*>(&boxB); 


  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);


  if(systId == SystId::kTpcTrackEff){
    if (!systTuning::TpcTrackEff_MAINTRACK_ONLY) return true;
    if (box.MainTrack->GetTrueParticle()){
      // At first order the inclusive selection only depends on the tracking efficiency of the muon candidate.
      if (trueTrack->ID  == box.MainTrack->GetTrueParticle()->ID) return true; 
      // Consider also the case in which the muon candidate is not a true muon but this track it is
      if (trueTrack->PDG == 13 && box.MainTrack->GetTrueParticle()->PDG!=13) return true;      

      // Otherwise don't consider this TrueTrack
      return false;
    }
  } 
  */
  return true;
}

//**************************************************
bool duneExampleSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)box;
  (void)systId;

  /*
  if      (systId==SystId::kFgdTrackEff)   // No FGD track eff
    return false;
  else if (systId==SystId::kFgdHybridTrackEff)  // No FGD hybrid track eff
    return false;
  else if (systId==SystId::kMichelEleEff)  // No michel electron Systematic
    return false;
  else if (systId==SystId::kSIPion){ 
    // check whether should be applied what the main track is a true pion or its product
    if (!systTuning::SIPion_APPLY_MAINTRACK) return false;
  }
  else if (systId==SystId::kSIProton){  
    //check whether should be still applied when the main track is a true proton or its product: TESTING PURPOSES
    if (!systTuning::SIProton_APPLY_MAINTRACK) return false;
  }
  */
  return true;
}

//**************************************************
void duneExampleSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxDUNE])
    event.EventBoxes[EventBoxId::kEventBoxDUNE] = new EventBoxDUNE();

  boxUtils::FillLongTracks(event,       static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  //  boxUtils::FillTrajsChargedInSubdet2(event);
  //  boxUtils::FillTrajsChargedInSubdet1AndNoSubdet2(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
bool duneExampleSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  (void)redoFromStep;

  return true;

  //  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 

  // Cast the ToyBox to the appropriate type
  //  const ToyBoxDUNE& PreviousToyBox = *static_cast<const ToyBoxDUNE*>(&PreviousToyBoxB); 

  // allways return true by now. TODO
  //  redoFromStep =0;
  //  return true;
  // Relevant properties of the previous toy are saved in the PreviousToyBox that is passed as parameter
  // - i.e  PreviousToyBox->HMNtrack is the HMNtrack of the previous toy
  // - i.e  PreviousToyBox->AccumLevel[i] is the accum level of the previous toy 
  //         for branch i in this selection

  // Only redo the selection when the HMN track changes identity or the PID cut has a different effect:

  // nothing to do if there is no HMN track
  //  if (!PreviousToyBox.HMNtrack) return false;

  /* TODO: This is not enough since the third HM track could become the HM track. Substituted by the code below
  // The momentum of SHMN track is larger than the one of HMN track
  // We have not yet called FindLeadingTracks, so HMN and SHMN are from previous toy, however we have already modified the 
  // momentum of those tracks because this method is called after the variation systematics
  if (PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    if (PreviousToyBox.SHMNtrack){
      if (PreviousToyBox.SHMNtrack->Momentum > PreviousToyBox.HMNtrack->Momentum){
        // We should redo the selection starting from the FindLeadingTracks step
        redoFromStep = _FindLeadingTracksStepIndex;
        return true;
      }
    }
  }
  */

  /*
  // TODO: The code below implies calling cutUtils::FindLeadingTracks(event, box) twice. Can we avoid that ?
  if (PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    ToyBoxDUNE box;
    trackerSelUtils::FindLeadingTracks(event, box);

    // Redo the selection if any of the leading tracks changes identity
    if (PreviousToyBox.SHMNtrack!=box.SHMNtrack || 
        PreviousToyBox.SHMPtrack!=box.SHMPtrack || 
        PreviousToyBox.HMNtrack !=box.HMNtrack  || 
        PreviousToyBox.HMPtrack !=box.HMPtrack  ||
        PreviousToyBox.SHMtrack !=box.SHMtrack  ||
        PreviousToyBox.HMtrack  !=box.HMtrack){
      
      redoFromStep = _FindLeadingTracksStepIndex;
      return true;
    }
  }

  // When the HMN track does not change identity, Redo the selection if the effect of the PID cut is different.
  // We have previously saved in the EventBox the AccumLevel of the previous toy for each branch. 
  // PreviousToyBox->AccumLevel[0]>_MuonPIDCutIndex means that the PID cut was passed, so we check whether the cut 
  // was passed in the previous toy and not in the current one, or the opposit, it was not passed before 
  // and it is passed now. 

  if (PreviousToyBox.MaxAccumLevel >= _MuonPIDCutIndex){
    bool pidCut = cutUtils::MuonPIDCut(*(PreviousToyBox.HMNtrack), !versionUtils::prod6_systematics);
    if  (( pidCut && (PreviousToyBox.AccumLevel[0] == _MuonPIDCutIndex)) ||
         (!pidCut && (PreviousToyBox.AccumLevel[0]  > _MuonPIDCutIndex))){
      // We should redo the selection starting from the MuonPIDCut step
      redoFromStep = _MuonPIDStepIndex;
      return true;
    }
  }
  */  
  return false;
}

