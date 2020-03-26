#include "anaTreeConverter.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"

//********************************************************************
anaTreeConverter::anaTreeConverter():InputConverter("anatreepmtrack/anatree"){
//********************************************************************

  _spill = NULL;

  _isMC = false;
  _softwareVersion = "";

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;

}

//********************************************************************
bool anaTreeConverter::Initialize(){
//********************************************************************
  
  std::string folder= "anatreepmtrack";

  AddChain(folder+"/anatree");
  eventsTree = GetChain(folder+"/anatree");

  fChain = eventsTree;

  // Set branch addresses and branch pointers

  if (!fChain) return false;
  fCurrent = -1;

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("evttime", &evttime, &b_evttime);
   fChain->SetBranchAddress("efield", efield, &b_efield);
   fChain->SetBranchAddress("t0", &t0, &b_t0);
   fChain->SetBranchAddress("ntracks_reco", &ntracks_reco, &b_ntracks_reco);
   fChain->SetBranchAddress("ntrkhits", ntrkhits, &b_ntrkhits);
   fChain->SetBranchAddress("trkid", trkid, &b_trkid);
   fChain->SetBranchAddress("trkstartx", trkstartx, &b_trkstartx);
   fChain->SetBranchAddress("trkstarty", trkstarty, &b_trkstarty);
   fChain->SetBranchAddress("trkstartz", trkstartz, &b_trkstartz);
   fChain->SetBranchAddress("trkendx", trkendx, &b_trkendx);
   fChain->SetBranchAddress("trkendy", trkendy, &b_trkendy);
   fChain->SetBranchAddress("trkendz", trkendz, &b_trkendz);
   fChain->SetBranchAddress("trkstartdcosx", trkstartdcosx, &b_trkstartdcosx);
   fChain->SetBranchAddress("trkstartdcosy", trkstartdcosy, &b_trkstartdcosy);
   fChain->SetBranchAddress("trkstartdcosz", trkstartdcosz, &b_trkstartdcosz);
   fChain->SetBranchAddress("trkenddcosx", trkenddcosx, &b_trkenddcosx);
   fChain->SetBranchAddress("trkenddcosy", trkenddcosy, &b_trkenddcosy);
   fChain->SetBranchAddress("trkenddcosz", trkenddcosz, &b_trkenddcosz);
   fChain->SetBranchAddress("trkx", trkx, &b_trkx);
   fChain->SetBranchAddress("trky", trky, &b_trky);
   fChain->SetBranchAddress("trkz", trkz, &b_trkz);
   fChain->SetBranchAddress("trktheta_xz", trktheta_xz, &b_trktheta_xz);
   fChain->SetBranchAddress("trktheta_yz", trktheta_yz, &b_trktheta_yz);
   fChain->SetBranchAddress("trketa_xy", trketa_xy, &b_trketa_xy);
   fChain->SetBranchAddress("trketa_zy", trketa_zy, &b_trketa_zy);
   fChain->SetBranchAddress("trktheta", trktheta, &b_trktheta);
   fChain->SetBranchAddress("trkphi", trkphi, &b_trkphi);
   fChain->SetBranchAddress("trkd2", trkd2, &b_trkd2);
   fChain->SetBranchAddress("trkdedx2", trkdedx2, &b_trkdedx2);
   fChain->SetBranchAddress("trkdqdx", trkdqdx, &b_trkdqdx);
   fChain->SetBranchAddress("trkpitch", trkpitch, &b_trkpitch);
   fChain->SetBranchAddress("trkpitchHit", trkpitchHit, &b_trkpitchHit);
   fChain->SetBranchAddress("trkkinE", trkkinE, &b_trkkinE);
   fChain->SetBranchAddress("trkrange", trkrange, &b_trkrange);
   fChain->SetBranchAddress("trkTPC", trkTPC, &b_trkTPC);
   fChain->SetBranchAddress("trkplaneid", trkplaneid, &b_trkplaneid);
   fChain->SetBranchAddress("trkresrg", trkresrg, &b_trkresrg);
   fChain->SetBranchAddress("trkPosx", trkPosx, &b_trkPosx);
   fChain->SetBranchAddress("trkPosy", trkPosy, &b_trkPosy);
   fChain->SetBranchAddress("trkPosz", trkPosz, &b_trkPosz);
   fChain->SetBranchAddress("trklen", trklen, &b_trklen);
   fChain->SetBranchAddress("trklen_L", trklen_L, &b_trklen_L);
   fChain->SetBranchAddress("trkdQdxSum", trkdQdxSum, &b_trkdQdxSum);
   fChain->SetBranchAddress("trkdQdxAverage", trkdQdxAverage, &b_trkdQdxAverage);
   fChain->SetBranchAddress("trkdEdxSum", trkdEdxSum, &b_trkdEdxSum);
   fChain->SetBranchAddress("trkdEdxAverage", trkdEdxAverage, &b_trkdEdxAverage);
   fChain->SetBranchAddress("trkMCTruthT0", trkMCTruthT0, &b_trkMCTruthT0);
   fChain->SetBranchAddress("trkMCTruthTrackID", trkMCTruthTrackID, &b_trkMCTruthTrackID);
   fChain->SetBranchAddress("trkPhotonCounterT0", trkPhotonCounterT0, &b_trkPhotonCounterT0);
   fChain->SetBranchAddress("trkPhotonCounterID", trkPhotonCounterID, &b_trkPhotonCounterID);
   fChain->SetBranchAddress("trkPhotonCounterConf", trkPhotonCounterConf, &b_trkPhotonCounterConf);
   fChain->SetBranchAddress("nMCParticles", &nMCParticles, &b_nMCParticles);
   fChain->SetBranchAddress("trkid_MC", trkid_MC, &b_trkid_MC);
   fChain->SetBranchAddress("trkpdg_MC", trkpdg_MC, &b_trkpdg_MC);
   fChain->SetBranchAddress("trkndaughters_MC", trkndaughters_MC, &b_trkndaughters_MC);
   fChain->SetBranchAddress("nTPCHits_MC", nTPCHits_MC, &b_nTPCHits_MC);
   fChain->SetBranchAddress("StartInTPC_MC", StartInTPC_MC, &b_StartInTPC_MC);
   fChain->SetBranchAddress("EndInTPC_MC", EndInTPC_MC, &b_EndInTPC_MC);
   fChain->SetBranchAddress("trkMother_MC", trkMother_MC, &b_trkMother_MC);
   fChain->SetBranchAddress("trkNumDaughters_MC", trkNumDaughters_MC, &b_trkNumDaughters_MC);
   fChain->SetBranchAddress("trkFirstDaughter_MC", trkFirstDaughter_MC, &b_trkFirstDaughter_MC);
   fChain->SetBranchAddress("trkPrimary_MC", trkPrimary_MC, &b_trkPrimary_MC);
   fChain->SetBranchAddress("StartTime_MC", StartTime_MC, &b_StartTime_MC);
   fChain->SetBranchAddress("trkstartx_MC", trkstartx_MC, &b_trkstartx_MC);
   fChain->SetBranchAddress("trkstarty_MC", trkstarty_MC, &b_trkstarty_MC);
   fChain->SetBranchAddress("trkstartz_MC", trkstartz_MC, &b_trkstartz_MC);
   fChain->SetBranchAddress("trkendx_MC", trkendx_MC, &b_trkendx_MC);
   fChain->SetBranchAddress("trkendy_MC", trkendy_MC, &b_trkendy_MC);
   fChain->SetBranchAddress("trkendz_MC", trkendz_MC, &b_trkendz_MC);
   fChain->SetBranchAddress("trkenergy_MC", trkenergy_MC, &b_trkenergy_MC);
   fChain->SetBranchAddress("EnergyDeposited_MC", EnergyDeposited_MC, &b_EnergyDeposited_MC);
   fChain->SetBranchAddress("trkmom_MC", trkmom_MC, &b_trkmom_MC);
   fChain->SetBranchAddress("trkmom_XMC", trkmom_XMC, &b_trkmom_XMC);
   fChain->SetBranchAddress("trkmom_YMC", trkmom_YMC, &b_trkmom_YMC);
   fChain->SetBranchAddress("trkmom_ZMC", trkmom_ZMC, &b_trkmom_ZMC);
   fChain->SetBranchAddress("trkstartdoc_XMC", trkstartdoc_XMC, &b_trkstartdoc_XMC);
   fChain->SetBranchAddress("trkstartdoc_YMC", trkstartdoc_YMC, &b_trkstartdoc_YMC);
   fChain->SetBranchAddress("trkstartdoc_ZMC", trkstartdoc_ZMC, &b_trkstartdoc_ZMC);
   fChain->SetBranchAddress("mcpos_x", mcpos_x, &b_mcpos_x);
   fChain->SetBranchAddress("mcpos_y", mcpos_y, &b_mcpos_y);
   fChain->SetBranchAddress("mcpos_z", mcpos_z, &b_mcpos_z);
   fChain->SetBranchAddress("mcang_x", mcang_x, &b_mcang_x);
   fChain->SetBranchAddress("mcang_y", mcang_y, &b_mcang_y);
   fChain->SetBranchAddress("mcang_z", mcang_z, &b_mcang_z);
   fChain->SetBranchAddress("trktheta_xz_MC", trktheta_xz_MC, &b_trktheta_xz_MC);
   fChain->SetBranchAddress("trktheta_yz_MC", trktheta_yz_MC, &b_trktheta_yz_MC);
   fChain->SetBranchAddress("trktheta_MC", trktheta_MC, &b_trktheta_MC);
   fChain->SetBranchAddress("trkphi_MC", trkphi_MC, &b_trkphi_MC);
   fChain->SetBranchAddress("trketa_xy_MC", trketa_xy_MC, &b_trketa_xy_MC);
   fChain->SetBranchAddress("trketa_zy_MC", trketa_zy_MC, &b_trketa_zy_MC);
   fChain->SetBranchAddress("trkTPCLen_MC", trkTPCLen_MC, &b_trkTPCLen_MC);
   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);
   fChain->SetBranchAddress("nhits2", &nhits2, &b_nhits2);
   fChain->SetBranchAddress("nclust", &nclust, &b_nclust);
   fChain->SetBranchAddress("hit_plane", hit_plane, &b_hit_plane);
   fChain->SetBranchAddress("hit_tpc", hit_tpc, &b_hit_tpc);
   fChain->SetBranchAddress("hit_wire", hit_wire, &b_hit_wire);
   fChain->SetBranchAddress("hit_channel", hit_channel, &b_hit_channel);
   fChain->SetBranchAddress("hit_peakT", hit_peakT, &b_hit_peakT);
   fChain->SetBranchAddress("hit_charge", hit_charge, &b_hit_charge);
   fChain->SetBranchAddress("hit_ph", hit_ph, &b_hit_ph);
   fChain->SetBranchAddress("hit_trkid", hit_trkid, &b_hit_trkid);
   fChain->SetBranchAddress("flash_total", &flash_total, &b_flash_total);
   fChain->SetBranchAddress("flash_time", flash_time, &b_flash_time);
   fChain->SetBranchAddress("flash_width", flash_width, &b_flash_width);
   fChain->SetBranchAddress("flash_abstime", flash_abstime, &b_flash_abstime);
   fChain->SetBranchAddress("flash_YCentre", flash_YCentre, &b_flash_YCentre);
   fChain->SetBranchAddress("flash_YWidth", flash_YWidth, &b_flash_YWidth);
   fChain->SetBranchAddress("flash_ZCentre", flash_ZCentre, &b_flash_ZCentre);
   fChain->SetBranchAddress("flash_ZWidth", flash_ZWidth, &b_flash_ZWidth);
   fChain->SetBranchAddress("flash_TotalPE", flash_TotalPE, &b_flash_TotalPE);
   fChain->SetBranchAddress("ntrigs", &ntrigs, &b_ntrigs);
   fChain->SetBranchAddress("trig_time", trig_time, &b_trig_time);
   fChain->SetBranchAddress("trig_id", trig_id, &b_trig_id);

  return true;
}

//********************************************************************
anaTreeConverter::~anaTreeConverter(){
//********************************************************************

  if (!fChain) return;

  if (eventsTree         ) delete   eventsTree          ->GetCurrentFile();
}

//****************************************************************************
bool anaTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "anaTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

  /*
  TChain BasicHeaderForFile("HeaderDir/BasicHeader");
  BasicHeaderForFile.AddFile(inputString.c_str());
  if (BasicHeaderForFile.GetEntries() == 0){
    std::cout << "      ----> This file does not contain any entries. IGNORED !!!!" << std::endl;
    return true;
  }
  */

  // Chain only the directories we are interested in

  if (eventsTree            )          eventsTree->AddFile(inputString.c_str());

  // Read one entry from the tree tree such that Run and Subrun are available
  eventsTree->GetEntry(eventsTree->GetEntries() - 1);

  // Make sure the current file has not the same run and subrun number as the previous
  if (_previousRunID==run &&  _previousSubrunID==subrun && _previousRefEventID>= event){
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "anaTreeConverter::AddFileToTChain(). Current file has the same run and subrun as the previous" << std::endl;
    std::cout << "                                           and no higher event number !!!" << std::endl;
    std::cout << "   - this file:     " << inputString << std::endl;
    std::cout << "   - previous file: " << _previousFile << std::endl;
    std::cout << "Please verify the input file list !!!" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    exit(1);
  }

  // The previous attributes
  _previousFile         = inputString;
  _previousRunID        = run;
  _previousSubrunID     = subrun;
  _previousRefEventID   = event;
  
  // Set the data/MC mode and return false when mixing data and MC files
  _isMC = (nMCParticles!=0); 
  if (!header().SetIsMC(_isMC)) return false;

  _softwareVersion = "v0r0";

  // Sets the software version for this file
  return header().SetSoftwareVersion(_softwareVersion);
}


//*****************************************************************************
Int_t anaTreeConverter::ReadEntries(Long64_t& entry) {
//*****************************************************************************
  
  Int_t entry_temp = eventsTree->GetEntry(entry);

  //-------- Increase the cache size to speed up reading Branches ----------
  static bool first = false;
  if (first){
    if( eventsTree ) {
      eventsTree->SetCacheSize(200000000);
      eventsTree->AddBranchToCache("*",kTRUE);
    }
    first=false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t anaTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
//*****************************************************************************

  static std::string currentfilename("");

  // Read contents of entry (which correspond to one Spill)
  if (!fChain) return 0;

  std::string filename(eventsTree->GetFile()->GetName());

  if( filename != currentfilename ) {
    std::cout << " Running on file: " << filename << std::endl;
    currentfilename = filename;
    
    //load geometry 
    //    ND::hgman().LoadGeometry(filename);
  }

  Int_t entry_temp = ReadEntries(entry);

  if (entry_temp > 0) {
    
    // Create an instance of the Spill
    spill = MakeSpill();
    
    // Cast it to AnaSpill
    _spill = static_cast<AnaSpill*>(spill);
    
    FillInfo(_spill);
  }
  else{
    std::cout << "Failed in reading entry " << entry << std::endl;
  }
  

  entry++;

  return entry_temp;
}

//********************************************************************
void anaTreeConverter::IncrementPOTBySpill() {
//********************************************************************
  
//  bool bySpillInMC = false;
  
  //  if (!_isMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
  //    anaUtils::IncrementPOTBySpill(*_spill,header());
}

//*****************************************************************************
void anaTreeConverter::FillInfo(AnaSpill* spill){
//*****************************************************************************
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *static_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = run;
  info.SubRun = subrun;
  info.Event  = event;
  info.IsMC   = _isMC;
  info.EventTime = evttime;

  spill->DataQuality = MakeDataQuality();
  spill->Beam = MakeBeam();

  spill->GeomID = (UInt_t)ND::hgman().GetCurrentGeomID();
  
  // beam related information
  FillBeamInfo(static_cast<AnaBeam*>(spill->Beam));

  // data quality info
  FillDQInfo(static_cast<AnaDataQuality*>(spill->DataQuality));

  // trigger information
  FillTriggerInfo(&(spill->Trigger));

  // True vertex information
  FillTrueInfo(spill);

  AnaBunch* bunch = MakeBunch();
  spill->Bunches.push_back(bunch);

  // All information about each bunch
  FillBunchInfo(spill->TrueParticles, bunch);

}

//*****************************************************************************
void anaTreeConverter::FillDQInfo(AnaDataQuality* dq){
//*****************************************************************************

    dq->GoodDaq   = true;
}

//*****************************************************************************
void anaTreeConverter::FillBeamInfo(AnaBeam* beam){
//*****************************************************************************

    beam->GoodSpill = true;
}

//*****************************************************************************
void anaTreeConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  trigger->nTriggers = ntrigs;
  anaUtils::CreateArray(trigger->Time, ntrigs);
  anaUtils::CreateArray(trigger->ID,   ntrigs);


  for (Int_t i=0;i<ntrigs;i++){
    trigger->Time[i] = trig_time[i];
    trigger->ID[i]   = trig_id[i];
  }
}

//*****************************************************************************
void anaTreeConverter::FillTrueInfo(AnaSpill* spill){
//*****************************************************************************

  // Fill the true vertices vector
  spill->TrueVertices.clear();
  int nVertices = std::min((int)NMAXTRUEVERTICES, 1);
  for (int i=0;i<nVertices;i++){
    AnaTrueVertex* trueVertex = MakeTrueVertex();
    FillTrueVertexInfo(i, trueVertex);
    spill->TrueVertices.push_back(trueVertex);    
  }

  // Fill the true particles vector
  spill->TrueParticles.clear();
  int nParts = std::min((int)NMAXTRUEPARTICLES, nMCParticles);
  for (int i=0;i<nParts;i++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueParticleInfo(spill->TrueVertices, i, truePart);
    spill->TrueParticles.push_back(truePart);    
  }


}

//*****************************************************************************
void anaTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
  bunch->Particles.clear();
  bunch->Vertices.clear();

  for (Int_t i=0;i<ntracks_reco;i++){
    AnaParticle* part = MakeParticle();
    FillParticleInfo(trueParticles, i, part);

    bunch->Particles.push_back(part);
  }
  /*
  for (int i=0;i<NVertices;i++){
    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(i, vertex, bunch);
    bunch->Vertices.push_back(vertex);
  }
  */


}

//*****************************************************************************
void anaTreeConverter::FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  (void)ivertex;

  // TODO

  for (Int_t i=0;i<nMCParticles;i++){
    if (trkPrimary_MC[i]){
      trueVertex->Position[0] = trkstartx_MC[i];
      trueVertex->Position[1] = trkstarty_MC[i];
      trueVertex->Position[2] = trkstartz_MC[i];
      trueVertex->LeptonPDG = trkpdg_MC[i];
      trueVertex->LeptonMom = trkmom_MC[i];
      return;
    }
  }
}


//*****************************************************************************
void anaTreeConverter::FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part){
//*****************************************************************************

  part->UniqueID  = trkid[itrk];
  part->NHits     = ntrkhits[itrk];
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);

  part->PositionStart[0]  = trkstartx[itrk];
  part->PositionStart[1]  = trkstarty[itrk];
  part->PositionStart[2]  = trkstartz[itrk];

  part->PositionEnd[0]  = trkendx[itrk];
  part->PositionEnd[1]  = trkendy[itrk];
  part->PositionEnd[2]  = trkendz[itrk];

  part->DirectionStart[0] = trkstartdcosx[itrk];
  part->DirectionStart[1] = trkstartdcosy[itrk];
  part->DirectionStart[2] = trkstartdcosz[itrk];

  part->DirectionEnd[0] = trkenddcosx[itrk];
  part->DirectionEnd[1] = trkenddcosy[itrk];
  part->DirectionEnd[2] = trkenddcosz[itrk];

  part->Length = trklen[itrk];

  part->AveragedEdx = trkdEdxAverage[itrk];

  part->TrueObject= FindTrueParticle(itrk,trueParticles);
      
}

//*****************************************************************************
AnaTrueObjectC* anaTreeConverter::FindTrueParticle(Int_t itrk, std::vector<AnaTrueParticleB*>& trueParticles){
//*****************************************************************************

  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == trkMCTruthTrackID[itrk]){
      return trueParticles[i];
    }
  }

  return NULL;
}

//*****************************************************************************
void anaTreeConverter::FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, Int_t ipart, AnaTrueParticle* truePart){
//*****************************************************************************

    truePart->ID = trkid_MC[ipart];

    //    truePart->Charge = ;

    truePart->Momentum = trkmom_MC[ipart];
    if (trkmom_MC[ipart]>0){
      truePart->Direction[0] = trkmom_XMC[ipart]/trkmom_MC[ipart];
      truePart->Direction[1] = trkmom_YMC[ipart]/trkmom_MC[ipart];
      truePart->Direction[2] = trkmom_ZMC[ipart]/trkmom_MC[ipart];
    }

    truePart->Position[0] = trkstartx_MC[ipart];
    truePart->Position[1] = trkstarty_MC[ipart];
    truePart->Position[2] = trkstartz_MC[ipart];
    truePart->Position[3] = StartTime_MC[ipart];

    truePart->PositionEnd[0] = trkendx_MC[ipart];
    truePart->PositionEnd[1] = trkendy_MC[ipart];
    truePart->PositionEnd[2] = trkendz_MC[ipart];
    truePart->PositionEnd[3] = 0;

    truePart->PDG = trkpdg_MC[ipart];      


    truePart->ParentPDG = 0;
    Int_t GParentID=-1;
    for (Int_t i=0;i<nMCParticles;i++){
      if (trkid_MC[i] == trkMother_MC[ipart]){
        truePart->ParentPDG = trkpdg_MC[i];      
        truePart->ParentID = trkid_MC[i];
        GParentID = trkMother_MC[i];
        break;
      }
    }

    truePart->GParentPDG = 0;
    for (Int_t i=0;i<nMCParticles;i++){
      if (trkid_MC[i] == GParentID){
        truePart->GParentPDG = trkpdg_MC[i];      
        break;
      }
    }

}
