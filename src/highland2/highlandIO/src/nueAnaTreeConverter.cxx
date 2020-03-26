#include "nueAnaTreeConverter.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"

//********************************************************************
nueAnaTreeConverter::nueAnaTreeConverter():InputConverter("nuetreedc/nueana"){
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
bool nueAnaTreeConverter::Initialize(){
//********************************************************************
  
  std::string folder= "nuetreedc";

  AddChain(folder+"/nueana");
  eventsTree = GetChain(folder+"/nueana");

  fChain = eventsTree;

  // Set branch addresses and branch pointers

  if (!fChain) return false;
  fCurrent = -1;

  //  anaUtils::ConfigureTreeBranch(fChain, "run", &run, &b_run);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("evttime", &evttime, &b_evttime);
   fChain->SetBranchAddress("taulife", &taulife, &b_taulife);
   fChain->SetBranchAddress("isdata", &isdata, &b_isdata);
   fChain->SetBranchAddress("ntracks_reco", &ntracks_reco, &b_ntracks_reco);
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
   fChain->SetBranchAddress("trklen", trklen, &b_trklen);
   fChain->SetBranchAddress("trkg4id", trkg4id, &b_trkg4id);
   fChain->SetBranchAddress("trkg4pdg", trkg4pdg, &b_trkg4pdg);
   fChain->SetBranchAddress("trkg4startx", trkg4startx, &b_trkg4startx);
   fChain->SetBranchAddress("trkg4starty", trkg4starty, &b_trkg4starty);
   fChain->SetBranchAddress("trkg4startz", trkg4startz, &b_trkg4startz);
   fChain->SetBranchAddress("trkg4initdedx", trkg4initdedx, &b_trkg4initdedx);
   fChain->SetBranchAddress("nshws", &nshws, &b_nshws);
   fChain->SetBranchAddress("shwid", shwid, &b_shwid);
   fChain->SetBranchAddress("shwdcosx", shwdcosx, &b_shwdcosx);
   fChain->SetBranchAddress("shwdcosy", shwdcosy, &b_shwdcosy);
   fChain->SetBranchAddress("shwdcosz", shwdcosz, &b_shwdcosz);
   fChain->SetBranchAddress("shwstartx", shwstartx, &b_shwstartx);
   fChain->SetBranchAddress("shwstarty", shwstarty, &b_shwstarty);
   fChain->SetBranchAddress("shwstartz", shwstartz, &b_shwstartz);
   fChain->SetBranchAddress("shwenergy", shwenergy, &b_shwenergy);
   fChain->SetBranchAddress("shwdedx", shwdedx, &b_shwdedx);
   fChain->SetBranchAddress("shwbestplane", shwbestplane, &b_shwbestplane);
   fChain->SetBranchAddress("shwg4id", shwg4id, &b_shwg4id);
   /*
   fChain->SetBranchAddress("flash_total", &flash_total, &b_flash_total);
   fChain->SetBranchAddress("flash_time", flash_time, &b_flash_time);
   fChain->SetBranchAddress("flash_width", flash_width, &b_flash_width);
   fChain->SetBranchAddress("flash_abstime", flash_abstime, &b_flash_abstime);
   fChain->SetBranchAddress("flash_YCenter", flash_YCenter, &b_flash_YCenter);
   fChain->SetBranchAddress("flash_YWidth", flash_YWidth, &b_flash_YWidth);
   fChain->SetBranchAddress("flash_ZCenter", flash_ZCenter, &b_flash_ZCenter);
   fChain->SetBranchAddress("flash_ZWidth", flash_ZWidth, &b_flash_ZWidth);
   fChain->SetBranchAddress("flash_TotalPE", flash_TotalPE, &b_flash_TotalPE);
   */
   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);

   //   fChain->SetBranchAddress("hit_plane", hit_plane, &b_hit_plane);
   //   fChain->SetBranchAddress("hit_wire", hit_wire, &b_hit_wire);
   //   fChain->SetBranchAddress("hit_channel", hit_channel, &b_hit_channel);
   //   fChain->SetBranchAddress("hit_peakT", hit_peakT, &b_hit_peakT);

   fChain->SetBranchAddress("hit_charge", hit_charge, &b_hit_charge);

   //   fChain->SetBranchAddress("hit_summedADC", hit_summedADC, &b_hit_summedADC);
   //   fChain->SetBranchAddress("hit_startT", hit_startT, &b_hit_startT);
   //   fChain->SetBranchAddress("hit_endT", hit_endT, &b_hit_endT);
   fChain->SetBranchAddress("hit_trkkey", hit_trkkey, &b_hit_trkkey);
   //   fChain->SetBranchAddress("hit_dQds", hit_dQds, &b_hit_dQds);

   fChain->SetBranchAddress("hit_dEds", hit_dEds, &b_hit_dEds);

   //   fChain->SetBranchAddress("hit_resrange", hit_resrange, &b_hit_resrange);
   fChain->SetBranchAddress("hit_shwkey", hit_shwkey, &b_hit_shwkey);
   //   fChain->SetBranchAddress("infidvol", &infidvol, &b_infidvol);

   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("vtx", vtx, &b_vtx);
   fChain->SetBranchAddress("vtxrecomc", &vtxrecomc, &b_vtxrecomc);
   fChain->SetBranchAddress("vtxrecomcx", &vtxrecomcx, &b_vtxrecomcx);
   fChain->SetBranchAddress("vtxrecomcy", &vtxrecomcy, &b_vtxrecomcy);
   fChain->SetBranchAddress("vtxrecomcz", &vtxrecomcz, &b_vtxrecomcz);
   fChain->SetBranchAddress("mcevts_truth", &mcevts_truth, &b_mcevts_truth);
   fChain->SetBranchAddress("nuPDG_truth", &nuPDG_truth, &b_nuPDG_truth);
   fChain->SetBranchAddress("ccnc_truth", &ccnc_truth, &b_ccnc_truth);
   fChain->SetBranchAddress("mode_truth", &mode_truth, &b_mode_truth);
   fChain->SetBranchAddress("enu_truth", &enu_truth, &b_enu_truth);
   fChain->SetBranchAddress("Q2_truth", &Q2_truth, &b_Q2_truth);
   //   fChain->SetBranchAddress("W_truth", &W_truth, &b_W_truth);
   //   fChain->SetBranchAddress("X_truth", &X_truth, &b_X_truth);
   //   fChain->SetBranchAddress("Y_truth", &Y_truth, &b_Y_truth);
   //   fChain->SetBranchAddress("hitnuc_truth", &hitnuc_truth, &b_hitnuc_truth);
   fChain->SetBranchAddress("target_truth", &target_truth, &b_target_truth);
   fChain->SetBranchAddress("nuvtxx_truth", &nuvtxx_truth, &b_nuvtxx_truth);
   fChain->SetBranchAddress("nuvtxy_truth", &nuvtxy_truth, &b_nuvtxy_truth);
   fChain->SetBranchAddress("nuvtxz_truth", &nuvtxz_truth, &b_nuvtxz_truth);
   fChain->SetBranchAddress("nu_dcosx_truth", &nu_dcosx_truth, &b_nu_dcosx_truth);
   fChain->SetBranchAddress("nu_dcosy_truth", &nu_dcosy_truth, &b_nu_dcosy_truth);
   fChain->SetBranchAddress("nu_dcosz_truth", &nu_dcosz_truth, &b_nu_dcosz_truth);
   fChain->SetBranchAddress("lep_mom_truth", &lep_mom_truth, &b_lep_mom_truth);
   fChain->SetBranchAddress("lep_dcosx_truth", &lep_dcosx_truth, &b_lep_dcosx_truth);
   fChain->SetBranchAddress("lep_dcosy_truth", &lep_dcosy_truth, &b_lep_dcosy_truth);
   fChain->SetBranchAddress("lep_dcosz_truth", &lep_dcosz_truth, &b_lep_dcosz_truth);
   fChain->SetBranchAddress("t0_truth", &t0_truth, &b_t0_truth);
   fChain->SetBranchAddress("no_primaries", &no_primaries, &b_no_primaries);
   fChain->SetBranchAddress("geant_list_size", &geant_list_size, &b_geant_list_size);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("Eng", Eng, &b_Eng);
   fChain->SetBranchAddress("Px", Px, &b_Px);
   fChain->SetBranchAddress("Py", Py, &b_Py);
   fChain->SetBranchAddress("Pz", Pz, &b_Pz);
   fChain->SetBranchAddress("StartPointx", StartPointx, &b_StartPointx);
   fChain->SetBranchAddress("StartPointy", StartPointy, &b_StartPointy);
   fChain->SetBranchAddress("StartPointz", StartPointz, &b_StartPointz);
   fChain->SetBranchAddress("EndPointx", EndPointx, &b_EndPointx);
   fChain->SetBranchAddress("EndPointy", EndPointy, &b_EndPointy);
   fChain->SetBranchAddress("EndPointz", EndPointz, &b_EndPointz);
   fChain->SetBranchAddress("Startdcosx", Startdcosx, &b_Startdcosx);
   fChain->SetBranchAddress("Startdcosy", Startdcosy, &b_Startdcosy);
   fChain->SetBranchAddress("Startdcosz", Startdcosz, &b_Startdcosz);
   fChain->SetBranchAddress("NumberDaughters", NumberDaughters, &b_NumberDaughters);
   fChain->SetBranchAddress("Mother", Mother, &b_Mother);
   fChain->SetBranchAddress("TrackId", TrackId, &b_TrackId);
   fChain->SetBranchAddress("process_primary", process_primary, &b_process_primary);
   /*
   fChain->SetBranchAddress("G4Process", &G4Process, &b_G4Process);
   fChain->SetBranchAddress("G4FinalProcess", &G4FinalProcess, &b_G4FinalProcess);
   fChain->SetBranchAddress("ptype_flux", &ptype_flux, &b_ptype_flux);
   fChain->SetBranchAddress("pdpx_flux", &pdpx_flux, &b_pdpx_flux);
   fChain->SetBranchAddress("pdpy_flux", &pdpy_flux, &b_pdpy_flux);
   fChain->SetBranchAddress("pdpz_flux", &pdpz_flux, &b_pdpz_flux);
   fChain->SetBranchAddress("pntype_flux", &pntype_flux, &b_pntype_flux);
   fChain->SetBranchAddress("vx_flux", &vx_flux, &b_vx_flux);
   fChain->SetBranchAddress("vy_flux", &vy_flux, &b_vy_flux);
   fChain->SetBranchAddress("vz_flux", &vz_flux, &b_vz_flux);
   */
  return true;
}

//********************************************************************
nueAnaTreeConverter::~nueAnaTreeConverter(){
//********************************************************************

  if (!fChain) return;

  if (eventsTree         ) delete   eventsTree          ->GetCurrentFile();
}

//****************************************************************************
bool nueAnaTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "nueAnaTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

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
    std::cout << "nueAnaTreeConverter::AddFileToTChain(). Current file has the same run and subrun as the previous" << std::endl;
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
  _isMC = (!isdata); 
  if (!header().SetIsMC(_isMC)) return false;

  _softwareVersion = "v0r0";
 
  // Sets the software version for this file
  return header().SetSoftwareVersion(_softwareVersion);
}


//*****************************************************************************
Int_t nueAnaTreeConverter::ReadEntries(Long64_t& entry) {
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
Int_t nueAnaTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
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
void nueAnaTreeConverter::IncrementPOTBySpill() {
//********************************************************************
  
//  bool bySpillInMC = false;
  
  //  if (!_isMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
  //    anaUtils::IncrementPOTBySpill(*_spill,header());
}

//*****************************************************************************
void nueAnaTreeConverter::FillInfo(AnaSpill* spill){
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
void nueAnaTreeConverter::FillDQInfo(AnaDataQuality* dq){
//*****************************************************************************

    dq->GoodDaq   = true;
}

//*****************************************************************************
void nueAnaTreeConverter::FillBeamInfo(AnaBeam* beam){
//*****************************************************************************

    beam->GoodSpill = true;
}

//*****************************************************************************
void nueAnaTreeConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  (void)trigger;
/*
  trigger->nTriggers = ntrigs;
  anaUtils::CreateArray(trigger->Time, ntrigs);
  anaUtils::CreateArray(trigger->ID,   ntrigs);


  for (Int_t i=0;i<ntrigs;i++){
    trigger->Time[i] = trig_time[i];
    trigger->ID[i]   = trig_id[i];
  }
*/
}

//*****************************************************************************
void nueAnaTreeConverter::FillTrueInfo(AnaSpill* spill){
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
  int nParts = std::min((int)NMAXTRUEPARTICLES, geant_list_size);
  for (int i=0;i<nParts;i++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueParticleInfo(spill->TrueVertices, i, truePart);
    spill->TrueParticles.push_back(truePart);    
  }

}

//*****************************************************************************
void nueAnaTreeConverter::FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  
  (void)ivertex;
  // TODO. A single vertex for the moment, corresponding to the neutrino interaction (there is only one per entry in the file)

/*  NOT USED FOR THE MOMENT
   fChain->SetBranchAddress("ccnc_truth", &ccnc_truth, &b_ccnc_truth);
   fChain->SetBranchAddress("W_truth", &W_truth, &b_W_truth);
   fChain->SetBranchAddress("X_truth", &X_truth, &b_X_truth);
   fChain->SetBranchAddress("Y_truth", &Y_truth, &b_Y_truth);
   fChain->SetBranchAddress("hitnuc_truth", &hitnuc_truth, &b_hitnuc_truth);
*/

  trueVertex->ID       = 0;
  trueVertex->NuPDG    = nuPDG_truth;
  trueVertex->NuEnergy = enu_truth*units::GeV;

  trueVertex->Position[0] = nuvtxx_truth*units::cm;
  trueVertex->Position[1] = nuvtxy_truth*units::cm;
  trueVertex->Position[2] = nuvtxz_truth*units::cm;

  trueVertex->NuDir[0] = nu_dcosx_truth;
  trueVertex->NuDir[1] = nu_dcosy_truth;
  trueVertex->NuDir[2] = nu_dcosz_truth;
    
  /*
  vector<simb::MCParticle>& parts =  MCNeutrinos->obj[ivertex].fPartList;  

  trueVertex->nTrueParticles = 0;
  anaUtils::CreateArray(trueVertex->TrueParticles, parts.size());

  for (UInt_t i=0;i<parts.size();i++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueParticleInfo(trueVertex, parts[i], truePart);
    trueVertex->TrueParticles[trueVertex->nTrueParticles++] = truePart;
  }
  */
  
  // mode_truth=0 --> QE or E
  // mode_truth=1 --> RES
  // mode_truth=2 --> DIS
  // mode_truth=3 --> COH
  // mode_truth=4 -->

  // ccnc_truth=0 --> CC
  // ccnc_truth=1 --> NC

  trueVertex->ReacCode  = mode_truth + ccnc_truth*10;
  trueVertex->TargetPDG = target_truth;
  trueVertex->LeptonPDG = 0;  // not available
  trueVertex->Q2        = Q2_truth*units::GeV*units::GeV;
  trueVertex->LeptonMom = lep_mom_truth*units::GeV;

  trueVertex->LeptonDir[0] = lep_dcosx_truth;
  trueVertex->LeptonDir[1] = lep_dcosy_truth;
  trueVertex->LeptonDir[2] = lep_dcosz_truth;
}


//*****************************************************************************
void nueAnaTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
  bunch->Particles.clear();
  bunch->Vertices.clear();

  _nhits_trk.resize(ntracks_reco);
  _dedx_trk.resize(ntracks_reco);
  _nhits_shw.resize(nshws);
  _dedx_shw.resize(nshws);

  for (Int_t i=0;i<ntracks_reco;i++){
    _nhits_trk[i]=0;
    _dedx_trk[i]=0;
  }
  for (Int_t i=0;i<nshws;i++){
    _nhits_shw[i]=0;
    _dedx_shw[i]=0;
  }

  for (Int_t i=0;i<nhits;i++){
    if (hit_trkkey[i]>-1 && hit_trkkey[i]<ntracks_reco){
      _nhits_trk[ hit_trkkey[i] ]++;
      _dedx_trk[ hit_trkkey[i] ] += hit_dEds[i]*units::MeV/units::cm;
    }
    if (hit_shwkey[i]>-1 && hit_shwkey[i]<nshws){
      _nhits_shw[ hit_shwkey[i] ]++;
      _dedx_shw[ hit_shwkey[i] ] += hit_charge[i];  //TODO
    }
  }


  for (Int_t i=0;i<ntracks_reco;i++){
    AnaParticle* part = MakeParticle();
    FillParticleInfo(trueParticles, i, part);
    bunch->Particles.push_back(part);
  }

  for (Int_t i=0;i<nshws;i++){
    AnaParticle* part = MakeParticle();
    FillParticleInfoFromShower(trueParticles, i, part);
    bunch->Particles.push_back(part);
  }


  for (int i=0;i<nvtx;i++){
    AnaVertex* vertex = MakeVertex();
    FillVertexInfo(i, vertex);
    bunch->Vertices.push_back(vertex);
  }



}

//*****************************************************************************
void nueAnaTreeConverter::FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part){
//*****************************************************************************

  part->UniqueID  = trkid[itrk];
  part->NHits     = _nhits_trk[itrk];
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_2);

  part->PositionStart[0]  = trkstartx[itrk]*units::cm;
  part->PositionStart[1]  = trkstarty[itrk]*units::cm;
  part->PositionStart[2]  = trkstartz[itrk]*units::cm;

  part->PositionEnd[0]  = trkendx[itrk]*units::cm;
  part->PositionEnd[1]  = trkendy[itrk]*units::cm;
  part->PositionEnd[2]  = trkendz[itrk]*units::cm;

  part->DirectionStart[0] = trkstartdcosx[itrk];
  part->DirectionStart[1] = trkstartdcosy[itrk];
  part->DirectionStart[2] = trkstartdcosz[itrk];

  part->DirectionEnd[0] = trkenddcosx[itrk];
  part->DirectionEnd[1] = trkenddcosy[itrk];
  part->DirectionEnd[2] = trkenddcosz[itrk];

  part->Length = trklen[itrk]*units::cm;

  if (_nhits_trk[itrk]>0)
    part->AveragedEdx = _dedx_trk[itrk]/(Float_t)_nhits_trk[itrk];

  part->TrueObject= FindTrueParticle(trkg4id[itrk],trueParticles);
}

//*****************************************************************************
void nueAnaTreeConverter::FillParticleInfoFromShower(std::vector<AnaTrueParticleB*>& trueParticles, Int_t ishw, AnaParticle* part){
//*****************************************************************************

/*
   fChain->SetBranchAddress("shwenergy", shwenergy, &b_shwenergy);
   fChain->SetBranchAddress("shwbestplane", shwbestplane, &b_shwbestplane);
*/

  part->UniqueID  = shwid[ishw];
  part->NHits     = _nhits_shw[ishw];
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);

  part->PositionStart[0]  = shwstartx[ishw]*units::cm;
  part->PositionStart[1]  = shwstarty[ishw]*units::cm;
  part->PositionStart[2]  = shwstartz[ishw]*units::cm;

  part->DirectionStart[0] = shwdcosx[ishw];
  part->DirectionStart[1] = shwdcosy[ishw];
  part->DirectionStart[2] = shwdcosz[ishw];

  part->Length = 0;
  //  for (Int_t i=0;i<3;i++)
  //  part->AveragedEdx += shwdedx[ishw][i];

  if (_nhits_shw[ishw]>0)
    part->AveragedEdx = _dedx_shw[ishw]/(Float_t)_nhits_shw[ishw];


  part->TrueObject= FindTrueParticle(shwg4id[ishw],trueParticles);
}

//*****************************************************************************
void nueAnaTreeConverter::FillVertexInfo(Int_t ivtx, AnaVertex* vertex){
//*****************************************************************************

  for (Int_t i=0;i<3;i++)
    vertex->Position[i] = vtx[ivtx][i]*units::cm;

}

//*****************************************************************************
AnaTrueObjectC* nueAnaTreeConverter::FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles){
//*****************************************************************************


  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == g4id) return trueParticles[i];
  }

  return NULL;
}

//*****************************************************************************
void nueAnaTreeConverter::FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, Int_t ipart, AnaTrueParticle* truePart){
//*****************************************************************************

/*
   fChain->SetBranchAddress("Eng", Eng, &b_Eng);
   fChain->SetBranchAddress("NumberDaughters", NumberDaughters, &b_NumberDaughters);
   fChain->SetBranchAddress("process_primary", process_primary, &b_process_primary);
   fChain->SetBranchAddress("G4Process", &G4Process, &b_G4Process);
   fChain->SetBranchAddress("G4FinalProcess", &G4FinalProcess, &b_G4FinalProcess);
*/

  truePart->ID = TrackId[ipart];

  //  truePart->Charge = ;

  truePart->Momentum = sqrt(Px[ipart]*Px[ipart]+Py[ipart]*Py[ipart]+Pz[ipart]*Pz[ipart])*units::GeV;

  truePart->Direction[0] = Startdcosx[ipart];
  truePart->Direction[1] = Startdcosy[ipart];
  truePart->Direction[2] = Startdcosz[ipart];
  
  truePart->Position[0] = StartPointx[ipart]*units::cm;
  truePart->Position[1] = StartPointy[ipart]*units::cm;
  truePart->Position[2] = StartPointz[ipart]*units::cm;
  truePart->Position[3] = 0;
  
  truePart->PositionEnd[0] = EndPointx[ipart]*units::cm;
  truePart->PositionEnd[1] = EndPointy[ipart]*units::cm;
  truePart->PositionEnd[2] = EndPointz[ipart]*units::cm;
  truePart->PositionEnd[3] = 0;
  
  truePart->PDG        = pdg[ipart];        
  truePart->ParentPDG  = 0;
  truePart->GParentPDG = 0;
  truePart->PrimaryID  = truePart->ID;
  
  if (Mother[ipart]>0){ 
    // Search for the parent
    Int_t GParentID=-1;
    for (Int_t i=0;i<geant_list_size;i++){
      if (TrackId[i] == Mother[ipart]){
        truePart->ParentPDG = pdg[i];      
        truePart->ParentID  = TrackId[i];
        GParentID           = Mother[i];
        truePart->PrimaryID = truePart->ParentID;
        break;
      }
    }
    
    // Search for the grand parent
    if (GParentID>0){
      for (Int_t i=0;i<geant_list_size;i++){
        if (TrackId[i] == GParentID){
          truePart->GParentPDG = pdg[i];      
          truePart->PrimaryID  = GParentID;
          break;
        }      
      }
    }
  }
  
  if (trueVertices.size()>0)
    truePart->TrueVertex = trueVertices[0];
}
