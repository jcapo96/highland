#include "LArSoftTreeConverter.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"
#include "TSpline.h"
#include "ClockConstants.h"

const bool debug = false;
const bool debugTrueReco = false;

float Range_grampercm[29] = {
  9.833E-1/1.396, 1.786E0/1.396, 3.321E0/1.396, 6.598E0/1.396, 1.058E1/1.396, 3.084E1/1.396, 4.250E1/1.396, 6.732E1/1.396,
  1.063E2/1.396,  1.725E2/1.396, 2.385E2/1.396, 4.934E2/1.396, 6.163E2/1.396, 8.552E2/1.396, 1.202E3/1.396, 1.758E3/1.396,
  2.297E3/1.396,  4.359E3/1.396, 5.354E3/1.396, 7.298E3/1.396, 1.013E4/1.396, 1.469E4/1.396, 1.910E4/1.396, 3.558E4/1.396,
  4.326E4/1.396,  5.768E4/1.396, 7.734E4/1.396, 1.060E5/1.396, 1.307E5/1.396};
//  for (float& value : Range_grampercm) {
//    value /= 1.396; // convert to cm
//  }
//  return Range_grampercm;
//}


float KE_MeV[29]= {
  10,    14,    20,    30,    40,     80,     100,    140,    200,   300,
  400,   800,   1000,  1400,  2000,   3000,   4000,   8000,   10000, 14000,
  20000, 30000, 40000, 80000, 100000, 140000, 200000, 300000, 400000};

TGraph const KEvsR(29, Range_grampercm, KE_MeV);
TSpline3 const KEvsR_spline3("KEvsRS", &KEvsR);


//********************************************************************
LArSoftTreeConverter::LArSoftTreeConverter():InputConverter("Events"){
//********************************************************************

  _spill = NULL;

  _isMC = false;

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;


  std::string X_correction_name = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/Xcalo_r5387.root";
  std::string YZ_correction_name = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/YZcalo_r5387.root";
  std::string E_field_correction_name = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/SCE_DataDriven_180kV_v3.root";

  std::string dEdX_template_name = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/dEdxrestemplates.root"; 

  X_correction_file  = new TFile( X_correction_name.c_str(), "OPEN" );
  YZ_correction_file = new TFile( YZ_correction_name.c_str(), "OPEN" );
  E_field_file       = new TFile( E_field_correction_name.c_str(), "OPEN" );

  UInt_t planeID=2;
  std::string hist_name = "dqdx_X_correction_hist_" + std::to_string(planeID);
  X_correction_hist = (TH1F*)X_correction_file->Get( hist_name.c_str() );

  YZ_neg = (TH2F*)YZ_correction_file->Get("correction_dqdx_ZvsY_negativeX_hist_2");
  YZ_pos = (TH2F*)YZ_correction_file->Get("correction_dqdx_ZvsY_positiveX_hist_2");

  ex_neg = (TH3F*)E_field_file->Get("Reco_ElecField_X_Neg");
  ey_neg = (TH3F*)E_field_file->Get("Reco_ElecField_Y_Neg");
  ez_neg = (TH3F*)E_field_file->Get("Reco_ElecField_Z_Neg");
  ex_pos = (TH3F*)E_field_file->Get("Reco_ElecField_X_Pos");
  ey_pos = (TH3F*)E_field_file->Get("Reco_ElecField_Y_Pos");
  ez_pos = (TH3F*)E_field_file->Get("Reco_ElecField_Z_Pos");


  dEdX_template_file = new TFile( dEdX_template_name.c_str(), "OPEN" );
  templates[ 211 ]  = (TProfile*)dEdX_template_file->Get( "dedx_range_pi"  );
  templates[ 321 ]  = (TProfile*)dEdX_template_file->Get( "dedx_range_ka"  );
  templates[ 13 ]   = (TProfile*)dEdX_template_file->Get( "dedx_range_mu"  );
  templates[ 2212 ] = (TProfile*)dEdX_template_file->Get( "dedx_range_pro" );


  templates[ 2212 ]->Print(); 
}

//********************************************************************
bool LArSoftTreeConverter::Initialize(){
//********************************************************************

  AddChain("Events");

  eventsTree          = GetChain("Events");

  fChain = eventsTree;

  // Set branch addresses and branch pointers

  if (!fChain) return false;
  fCurrent = -1;


#ifndef ISMC
  std::string trailer = "DecoderandReco.";
#else
  std::string trailer = "Reco.";
#endif
  
  eventsTree->SetBranchAddress("EventAuxiliary", &EventInfo);

  // beam info
#ifndef ISMC
    eventsTree->SetBranchAddress(("beam::ProtoDUNEBeamEvents_beamevent__"+trailer).c_str(), &BEAM);
#endif

  // Particles META Data
  eventsTree->SetBranchAddress(("larpandoraobj::PFParticleMetadatarecob::PFParticlevoidart::Assns_pandora__"+trailer).c_str(), &PFParticles_MetaData);
  
  eventsTree->SetBranchAddress(("larpandoraobj::PFParticleMetadatas_pandora__"+trailer).c_str(), &MetaData);

  
  // Calorimetry
  eventsTree->SetBranchAddress(("anab::Calorimetrys_pandoracalo__"+trailer).c_str(), &CALOs);
  eventsTree->SetBranchAddress(("anab::Calorimetryrecob::Trackvoidart::Assns_pandoracalo__"+trailer).c_str(), &CALOs_Tracks);
  eventsTree->SetBranchAddress(("anab::Calorimetrys_pandoracaloSCE__"+trailer).c_str(), &CALOsSCE);
  eventsTree->SetBranchAddress(("anab::Calorimetryrecob::Trackvoidart::Assns_pandoracaloSCE__"+trailer).c_str(), &CALOsSCE_Tracks);
                                
  // Particle id
  eventsTree->SetBranchAddress(("anab::ParticleIDs_pandorapid__"+trailer).c_str(), &PIDs);

  eventsTree->SetBranchAddress(("anab::ParticleIDrecob::Trackvoidart::Assns_pandorapid__"+trailer).c_str(), &PIDs_Tracks);

  // Reconstructed tracks
  //  eventsTree->SetBranchAddress(("recob::Tracks_pmtrackdc__"+trailer).c_str(), &Tracks);

  eventsTree->SetBranchAddress(("recob::PFParticles_pandora__"+trailer).c_str(), &PFParticles);

  eventsTree->SetBranchAddress(("recob::Tracks_pandoraTrack__"+trailer).c_str(), &Tracks);

  eventsTree->SetBranchAddress(("recob::Showers_pandoraShower__"+trailer).c_str(), &Showers);

  eventsTree->SetBranchAddress(("recob::Vertexs_pandora__"+trailer).c_str(), &Vertices);
  

  eventsTree->SetBranchAddress(("recob::PFParticlerecob::Trackvoidart::Assns_pandoraTrack__"+trailer).c_str(),   &PFParticles_Tracks);
  eventsTree->SetBranchAddress(("recob::PFParticlerecob::Showervoidart::Assns_pandoraShower__"+trailer).c_str(), &PFParticles_Showers);
  eventsTree->SetBranchAddress(("recob::PFParticlerecob::Vertexvoidart::Assns_pandora__"+trailer).c_str(),       &PFParticles_Vertices);


  // CRT
#ifndef ISMC
  eventsTree->SetBranchAddress(("raw::ctb::pdspctbs_ctbrawdecoder_daq_"+trailer).c_str(), &CTB);
#endif
  
  // ------------- Points --------------------
  //  eventsTree->SetBranchAddress(("recob::SpacePoints_pmtrackdc__"+trailer).c_str(), &SpacePoints);

  // MC particles
#ifdef ISMC
    eventsTree->SetBranchAddress("simb::MCParticles_largeant__G4.", &MCParticles);
#endif

  // MC neutrinos
  //  eventsTree->SetBranchAddress("simb::MCTruths_generator__GenieGen.", &MCNeutrinos);
#ifdef ISMC
    eventsTree->SetBranchAddress("simb::MCTruths_generator__SinglesGen.", &MCTruths);
#endif

  // Reconstructed hits
  //  eventsTree->SetBranchAddress(("recob::Hits_lineclusterdc__"+trailer).c_str(), &Hits);
  //  eventsTree->SetBranchAddress(("recob::Hits_linecluster__"+trailer).c_str(), &Hits);
    eventsTree->SetBranchAddress(("recob::Hits_gaushit__"+trailer).c_str(), &Hits);

    // Association between reconstructed hits and tracks/showers
    eventsTree->SetBranchAddress(("recob::Hitrecob::Trackvoidart::Assns_pandoraTrack__"+trailer).c_str(), &Hits_Tracks);
    eventsTree->SetBranchAddress(("recob::Hitrecob::Showervoidart::Assns_pandoraShower__"+trailer).c_str(), &Hits_Showers);

    
  // Channels
#ifdef ISMC
    eventsTree->SetBranchAddress("sim::SimChannels_largeant__G4.", &SimChannels);
#endif


    //MVA

  eventsTree->SetBranchAddress(("4anab::FeatureVectors_emtrkmichelid_emtrkmichelrecobHit_"+trailer).c_str(), &MVA);

  eventsTree->SetBranchAddress(("4anab::MVADescriptions_emtrkmichelid_emtrkmichel_"+trailer).c_str(), &MVADescription);

                                 
    
  //-------------------- Disable the unnecessary branches -------------------
  eventsTree->SetBranchStatus("art::*",0);
  //  eventsTree->SetBranchStatus("sim::Beam*",0);
#ifdef ISMC
    eventsTree->SetBranchStatus("sim::AuxDet*",0);
    eventsTree->SetBranchStatus("sim::SimPhoton*",0);
#endif
  
  //  eventsTree->SetBranchStatus("*shower*",0);

  eventsTree->SetBranchStatus("raw*",0);

  eventsTree->SetBranchStatus("anab::T0recob*",0);
  eventsTree->SetBranchStatus("anab::Cosmic*",0);
  //  eventsTree->SetBranchStatus("anab::Calo*",0);
        
  eventsTree->SetBranchStatus("recob::Wires*",0);
  //  eventsTree->SetBranchStatus("recob::Vertexs*",0);
  eventsTree->SetBranchStatus("recob::Space*",0);
  //  eventsTree->SetBranchStatus("recob::Trackrecob*",0);
  //  eventsTree->SetBranchStatus("recob::Tracks_emshower*",0);
  //  eventsTree->SetBranchStatus("recob::Tracks_pmtrack_*",0);
  //  eventsTree->SetBranchStatus("recob::Shower*",0);
  eventsTree->SetBranchStatus("recob::Op*",0);
  //  eventsTree->SetBranchStatus("recob::Hits_dcheat*",0);
  //  eventsTree->SetBranchStatus("recob::Hits_gaushit*",0);
  //  eventsTree->SetBranchStatus("recob::Hits_hitfd*",0);
  eventsTree->SetBranchStatus("recob::Hitrecob::Space*",0);
  eventsTree->SetBranchStatus("recob::Hitrecob::Wire*",0);
  //  eventsTree->SetBranchStatus("recob::End*",0);
  eventsTree->SetBranchStatus("recob::Cluster*",0);
  //  eventsTree->SetBranchStatus("simb::MCTruths*",0);
  //  eventsTree->SetBranchStatus("simb::MCFlux*",0);
  //  eventsTree->SetBranchStatus("simb::GTruth*",0);
#ifdef ISMC
    eventsTree->SetBranchStatus("simb::MCParticlesimb*",0);
#endif

  eventsTree->SetBranchStatus("EventAuxiliary", 1);
#ifdef ISMC
    eventsTree->SetBranchStatus("sim::SimChannels_largeant__G4.", 1);  
    eventsTree->SetBranchStatus("simb::MCParticles_largeant__G4.",1);
    //  eventsTree->SetBranchStatus("simb::MCTruths_generator__GenieGen.",1);
    eventsTree->SetBranchStatus("simb::MCTruths_generator__SinglesGen.",1);
#endif
  //  eventsTree->SetBranchStatus("recob::Tracks_pmtrackdc__"+trailer).c_str(),1);
  eventsTree->SetBranchStatus(("anab::ParticleIDs_pandorapid__"+trailer).c_str(),1);
  eventsTree->SetBranchStatus(("recob::Tracks_pandoraTrack__"+trailer).c_str(),1);
  //  eventsTree->SetBranchStatus(("recob::Hitrecob::Trackvoidart::Assns_pmtrack__"+trailer).c_str(), 1);  
  //  eventsTree->SetBranchStatus("recob::Hits_lineclusterdc__"+trailer).c_str());

  //eventsTree->SetBranchStatus(("recob::Hits_linecluster__"+trailer).c_str(),1);
  eventsTree->SetBranchStatus(("recob::Hits_gaushit__"+trailer).c_str(),1);

  eventsTree->SetBranchStatus(("anab::ParticleIDrecob::Trackvoidart::Assns_pandorapid__"+trailer).c_str(),1);

  eventsTree->SetBranchStatus(("recob::PFParticlerecob::Trackvoidart::Assns_pandoraTrack__"+trailer).c_str(),1);

#ifndef ISMC
  eventsTree->SetBranchStatus(("raw::ctb::pdspctbs_ctbrawdecoder_daq_"+trailer).c_str(),1);
#endif

  eventsTree->SetBranchStatus(("4anab::FeatureVectors_emtrkmichelid_emtrkmichelrecobHit_"+trailer).c_str(), 1);
  eventsTree->SetBranchStatus(("4anab::MVADescriptions_emtrkmichelid_emtrkmichel_"+trailer).c_str(),1);
  
  return true;
}

//********************************************************************
LArSoftTreeConverter::~LArSoftTreeConverter(){
//********************************************************************

  if (!fChain) return;

  if (eventsTree         ) delete   eventsTree          ->GetCurrentFile();

}

//****************************************************************************
bool LArSoftTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "LArSoftTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

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

  // Read one entry from the BasicHeader tree such that RunID and SubrunID are available
  eventsTree->GetEntry(eventsTree->GetEntries() - 1);

  // Make sure the current file has not the same run and subrun number as the previous

  std::cout << _previousRefEventID << " " <<  (Int_t)EventInfo->id_.event_ << std::endl;

  if (_previousRunID      == (Int_t)EventInfo->id_.subRun_.run_.run_ &&  
      _previousSubrunID   == (Int_t)EventInfo->id_.subRun_.subRun_   && 
      _previousRefEventID >= (Int_t)EventInfo->id_.event_){
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "LArSoftTreeConverter::AddFileToTChain(). Current file has the same run and subrun as the previous" << std::endl;
    std::cout << "                                           and no higher event number !!!" << std::endl;
    std::cout << "   - this file:     " << inputString << std::endl;
    std::cout << "   - previous file: " << _previousFile << std::endl;
    std::cout << "Please verify the input file list !!!" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << _previousRunID      << " " << (Int_t)EventInfo->id_.subRun_.run_.run_ << std::endl;
    std::cout << _previousSubrunID   << " " << (Int_t)EventInfo->id_.subRun_.subRun_   << std::endl;
    std::cout << _previousRefEventID << " " << (Int_t)EventInfo->id_.event_ << std::endl;          
    //    exit(1);
  }

  // The previous attributes
  _previousFile         = inputString;
  _previousRunID        = (Int_t)EventInfo->id_.subRun_.run_.run_;
  _previousSubrunID     = (Int_t)EventInfo->id_.subRun_.subRun_;
  _previousRefEventID   = (Int_t)EventInfo->id_.event_;

  
  _isMC = (!EventInfo->isRealData_);

  std::cout << "Running on data or MC ?  MC = " << _isMC << std::endl;
  
  // Set the data/MC mode and return false when mixing data and MC files
  if (!header().SetIsMC(_isMC)) return false;

  // Sets the software version for this file
  return header().SetSoftwareVersion(SoftwareVersion);
}


//*****************************************************************************
Int_t LArSoftTreeConverter::ReadEntries(Long64_t& entry) {
//*****************************************************************************

  Int_t entry_temp = eventsTree->GetEntry(entry);

  //-------- Increase the cache size to speed up reading Branches ----------
  static bool first = false;
  if (first){
    if( eventsTree ) {
      eventsTree->SetCacheSize(800000000);
      eventsTree->AddBranchToCache("*",kTRUE);
    }
    first=false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t LArSoftTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
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
    //    _spill->Print();
  }
  else{
    std::cout << "Failed in reading entry " << entry << std::endl;
  }
  
  entry++;

  return entry_temp;
}

//********************************************************************
void LArSoftTreeConverter::IncrementPOTBySpill() {
//********************************************************************
  
//  bool bySpillInMC = false;
  
  //  if (!_isMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
  //    anaUtils::IncrementPOTBySpill(*_spill,header());
}

//*****************************************************************************
void LArSoftTreeConverter::FillDQInfo(AnaDataQuality* dq){
//*****************************************************************************

    dq->GoodDaq   = true;
}

//*****************************************************************************
void LArSoftTreeConverter::FillBeamInfo(AnaBeam* beam){
//*****************************************************************************

    
  beam->GoodSpill = true;

#ifdef ISMC


    // Get the G4 particle that corresponds to the beam particle
    const simb::MCParticle* geantGoodParticle = GetGeantGoodParticle((MCTruths->obj[0]));
    
    if(geantGoodParticle == NULL) return;
    
    
    //    std::cout << "Found GEANT particle corresponding to the good particle with pdg = " << geantGoodParticle->fpdgCode 
    //              << " , track id = " << geantGoodParticle->ftrackId
    //              << std::endl;
    
    // Create the BeamParticle object

    beam->BeamParticle = new AnaParticle();
    
    //---------------- Fill the truth info of the beam particle -----------------------
    
    // Create the TrueParticle inside the BeamParticle
    beam->BeamParticle->TrueObject = new AnaTrueParticle();
    
    // Downcast the AnaTRueObjectC inside BeamParticle to a AnaTrueParticle such that we can access all info
    AnaTrueParticle * truePart = static_cast<AnaTrueParticle*>(beam->BeamParticle->TrueObject);
   
    FillTrueParticleInfo(NULL, *geantGoodParticle, truePart);


#else
    
    std::vector<beam::ProtoDUNEBeamEvent> beaminfo = BEAM->obj;
    
    std::cout << "BEAM->size(): " << beaminfo.size() <<  std::endl; 
    
    for(unsigned int i = 0; i < beaminfo.size(); ++i){

      std::cout << "BEAM[0]: " << beaminfo[0].theTOF << " " << beaminfo[i].Tracks.size() << " " << beaminfo[0].RecoBeamMomenta.size() <<  std::endl; 

      //if(!beaminfo[i]->CheckIsMatched()) continue;
      beam->BeamTrigger   = beaminfo[i].TimingTrigger;
      beam->BeamTrackTime = (double)beaminfo[i].RDTimestamp;

      // If ToF is 0-3 there was a good match corresponding to the different pair-wise combinations of the upstream and downstream channels
      if(beaminfo[i].TOFChan >= 0)
        beam->TOF =  beaminfo[i].theTOF;

      // Get Cerenkov
      if(beaminfo[i].BITrigger == 1){
        beam->CerenkovStatus[0]   = beaminfo[i].CKov0.trigger;
        beam->CerenkovStatus[1]   = beaminfo[i].CKov1.trigger;
        beam->CerenkovTime[0]     = beaminfo[i].CKov0.timeStamp;
        beam->CerenkovTime[1]     = beaminfo[i].CKov1.timeStamp;
        beam->CerenkovPressure[0] = beaminfo[i].CKov0.pressure;
        beam->CerenkovPressure[1] = beaminfo[i].CKov1.pressure;
      }

      // Beam particle could have more than one tracks - for now take the first one, need to do this properly      
      auto & tracks = beaminfo[i].Tracks;

      if(!tracks.empty()){

        recob::Track track = tracks[0];

        // Create the BeamParticle object
        AnaParticleMomB* part = beam->BeamParticle = new AnaParticleMomB();

        // Fill the basic track info
        FillBasicTrackInfo(track, part);
      }

      // Beam momentum
      auto & beammom = beaminfo[i].RecoBeamMomenta;
      beam->nMomenta = beaminfo[i].RecoBeamMomenta.size();
      beam->nTracks  = beaminfo[i].Tracks.size();

      if(!beammom.empty()){
        beam->BeamMomentum = beammom[0];
        if (beam->BeamParticle)
          beam->BeamParticle->Momentum = beammom[0];
        std::cout << "HERE!!  " << beammom[0] << std::endl;
      }

      beam::ProtoDUNEBeamEvent beamEvent = BEAM->obj[0];
      std::vector< int > pdg_cands = GetPID( beamEvent, 1. );
      //      beam->PDGs.insert( beam->PDGs.end(), pdg_cands.begin(), pdg_cands.end() );
      
      // Beam particle could have more than one  ... - for now take the first one, need to do this properly      
      auto & fibers = beaminfo[i].fiberMonitors;

      if(!fibers.empty()){
        beam->nFibers[0] = GetActiveFibers(fibers, "XBPF022697" ).size();
        beam->nFibers[1] = GetActiveFibers(fibers, "XBPF022701" ).size();
        beam->nFibers[2] = GetActiveFibers(fibers, "XBPF022702" ).size();

      }
      
      // For now only take the first beam particle - need to add some criteria if more than one are found
      break;
      
    }

    std::cout << "beam info" << std::endl;
    beam->Print();

    
    std::cout << "beam particle: " <<     beam->BeamParticle << std::endl;
    if (beam->BeamParticle)
      beam->BeamParticle->Print();
    
#endif

}

//*****************************************************************************
void LArSoftTreeConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  (void)trigger;
}

//*****************************************************************************
void LArSoftTreeConverter::FillTrueInfo(AnaSpill* spill){
//*****************************************************************************

#ifdef ISMC
  
  // Fill the true vertices vector
  spill->TrueVertices.clear();

  int nVertices = std::min((int)NMAXTRUEVERTICES, (int)MCTruths->obj.size());

  std::cout << "nVertices: " << nVertices << std::endl;
  for (int i=0;i<nVertices;i++){
    AnaTrueVertex* trueVertex = MakeTrueVertex();

    FillTrueVertexInfo(i, trueVertex);
    spill->TrueVertices.push_back(trueVertex);    
  }

  // Fill the true particles vector
  spill->TrueParticles.clear();

  for (UInt_t j=0;j<spill->TrueVertices.size();j++){
    for (Int_t k=0;k<spill->TrueVertices[j]->nTrueParticles;k++){
      spill->TrueParticles.push_back(spill->TrueVertices[j]->TrueParticles[k]);
    }
  }


  //  int nTrueParts = std::min((int)NMAXTRUEPARTICLES, (int)MCParticles->obj.size());
  int nTrueParts = (int)MCParticles->obj.size();
  for (int i=0;i<nTrueParts;i++){

    // Check if already added to a vertex
    AnaTrueParticle* truePart = NULL;
    for (UInt_t j=0;j<spill->TrueVertices.size();j++){
      for (Int_t k=0;k<spill->TrueVertices[j]->nTrueParticles;k++){
        //        if (spill->TrueVertices[j]->TrueParticles[k]->ID == MCParticles->obj[i].ftrackId){
        if (fabs(spill->TrueVertices[j]->TrueParticles[k]->Momentum - MCParticles->obj[i].ftrajectory.ftrajectory[0].second.Vect().Mag())<0.001){
          truePart = static_cast<AnaTrueParticle*>(spill->TrueVertices[j]->TrueParticles[k]);
          truePart->ID = MCParticles->obj[i].ftrackId;
          break;
        }
      }
      if (truePart) break;
    }
    // If not added to a TrueVertex create it
    if (!truePart){
      truePart = MakeTrueParticle();
      FillTrueParticleInfo(NULL, MCParticles->obj[i], truePart);
      spill->TrueParticles.push_back(truePart);    
    }

  }

#endif
  
}

//*****************************************************************************
void LArSoftTreeConverter::FillInfo(AnaSpill* spill){
//*****************************************************************************

  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *static_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = EventInfo->id_.subRun_.run_.run_;
  info.SubRun = EventInfo->id_.subRun_.subRun_;
  info.Event  = EventInfo->id_.event_;
  info.IsMC   = _isMC;
  info.EventTime = EventTime;

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

  // All information about each bunch
  AnaBunch* bunch = MakeBunch();
  spill->Bunches.push_back(bunch);
  FillBunchInfo(spill->TrueParticles, bunch);  

}

//*****************************************************************************
void LArSoftTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
  bunch->Particles.clear();
  bunch->Vertices.clear();


  // Find the beam particle
  std::vector<const recob::PFParticle*> pfParticles = GetPFParticlesFromBeamSlice("");
  //  if (debug)
  //  std::cout << "0. particles in beam slice: " << pfParticles.size() << std::endl;
  //  if (pfParticles.size())
  //    std::cout << "0.a. particles in beam slice: " << pfParticles.size() << " " << pfParticles[0]->fSelf << std::endl;

  //  FillPFParticleInfo(trueParticles, pfParticles[0]->fSelf, bunch);

  // Create and fill all AnaParticle from tracks and showers in PFParticles
  for (UInt_t i=0;i<PFParticles->obj.size();i++){
    FillPFParticleInfo(trueParticles, i, bunch);

    if (pfParticles.size()){
      if (pfParticles[0]->fSelf == PFParticles->obj[i].fSelf)
        static_cast<AnaParticle*>(bunch->Particles.back())->Charge = -8888;
    }
  } 

  //  std::cout << "# particles: " << bunch->Particles.size() << " " << PFParticles->obj.size() << " " << Tracks->obj.size() + Showers->obj.size() << std::endl;
  // Fill the Daughters in all AnaParticles following the PFParticle hierarchy 
  for (UInt_t i=0;i<PFParticles->obj.size();i++){
    FillPFParticleDaughterInfo(i, bunch);
  }

  /*
  for (int i=0;i<NVertices;i++){
    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(i, vertex, bunch);
    bunch->Vertices.push_back(vertex);
  }
  */

  // Find the particle vertex. We need the tracker tag here because we need to do a bit of
  // additional work if the PFParticle is track-like to find the vertex. 
  //  const TVector3 vtx = GetPFParticleVertex(PFParticles->obj[2]);
  //  std::cout << vtx.X() << " " << vtx.Y() << " " <<  vtx.Z() << std::endl;
  //fprimaryVertex[0] = vtx.X(); fprimaryVertex[1] = vtx.Y(); fprimaryVertex[2] = vtx.Z();

  /*
  for (size_t i=0;i<bunch->Particles.size();i++){
    bunch->Particles[i]->Print();
    if (bunch->Particles[i]->TrueObject)
      static_cast<AnaTrueParticleB*>(bunch->Particles[i]->TrueObject)->Print();
  }
  */

}

//*****************************************************************************
void LArSoftTreeConverter::FillPFParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t ipart, AnaBunch* bunch){
//*****************************************************************************

  recob::PFParticle& PFPart = PFParticles->obj[ipart];


  // Pandora's BDT beam-cosmic score
  //  fprimaryBDTScore = (double)pfpUtil.GetBeamCosmicScore(*particle,evt,fPFParticleTag);
  
  // NHits associated with this pfParticle
  //  fprimaryNHits = (pfpUtil.GetPFParticleHits(*particle,evt,fPFParticleTag)).size();
  
  // Get the T0 for this pfParticle
  //  std::vector<anab::T0> pfT0vec = pfpUtil.GetPFParticleT0(*particle,evt,fPFParticleTag);
  //  if(!pfT0vec.empty())
  //    fprimaryT0 = pfT0vec[0].Time();
    
  // "particle" is the pointer to our beam particle. The recob::Track or recob::Shower object
  // of this particle might be more helpful. These return null pointers if not track-like / shower-like

  //  std::cout << "an0: " << ipart << " " << PFPart.fSelf << " " << track << " " << shower << " " << PFPart.fPdgCode << " " << PFPart.fDaughters.size() << std::endl; 



  AnaParticle* part = MakeParticle();
  
  const recob::Track*  track  = GetPFParticleTrack( PFPart);
  if (track){
    FillParticleTrackInfo(trueParticles, *track, part);
    bunch->Particles.push_back(part);
  }
  else{
    const recob::Shower*  shower  = GetPFParticleShower( PFPart);
    if (shower){
      FillParticleShowerInfo(trueParticles, *shower, part);
      bunch->Particles.push_back(part);
    }
    else{
      delete part;
      return;
    }
  }

  cnnOutput2D output = GetCNNOutputFromPFParticle(PFPart,"");
  if (output.nHits!=0){
    part->CNNscore[0] = output.track/(1.*output.nHits);
    part->CNNscore[1] = output.em/(1.*output.nHits);
    part->CNNscore[2] = output.michel/(1.*output.nHits);
  }

}

//*****************************************************************************
void LArSoftTreeConverter::FillPFParticleDaughterInfo(Int_t ipart, AnaBunch* bunch, int indent){
//*****************************************************************************

  recob::PFParticle& PFPart = PFParticles->obj[ipart];

  UInt_t NMAXDAUGHTERS = 100;
  
  //  for (int k=0;k<indent;k++) std::cout << " ";
  //  std::cout << "Particle " << PFPart.fSelf << " has " << PFPart.fDaughters.size() << " daughters. indent = " << indent << std::endl;
  
  if(PFPart.fDaughters.size() > NMAXDAUGHTERS)
    std::cout << "INFO::Number of daughters is " << PFPart.fDaughters.size() << ". Only the first NMAXDAUGHTERS are processed." << std::endl;
  
  indent +=2;
  for (UInt_t idau=0;idau<PFPart.fDaughters.size();idau++){
    
    UInt_t daughterID = PFPart.fDaughters[idau];
    
    AnaParticle*   part = static_cast<AnaParticle*>(bunch->Particles[ipart]);
    AnaParticleB* Dpart = bunch->Particles[daughterID];      
    if (Dpart && daughterID < bunch->Particles.size()) part->Daughters.push_back(Dpart);
    
    // Find daughters recursively
    FillPFParticleDaughterInfo(daughterID, bunch,indent);
  }


}

//*****************************************************************************
void LArSoftTreeConverter::FillParticleTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, const recob::Track& track, AnaParticle* part){
//*****************************************************************************

  Int_t itrk = track.fID;

  // Fill the basic track info
  FillBasicTrackInfo(track, part);

  // Track NDOF
  part->NDOF = track.fNdof;

  // Track Chi2
  part->Chi2 = track.fChi2;

  // Particle ID hypothesis used in the fit (if any)
  part->FitPDG = track.fPId;
  
  // Compute the range momentum
  part->RangeMomentum[0] = GetTrackMomentum(part->Length,  13);
  part->RangeMomentum[1] = GetTrackMomentum(part->Length,2212); 
  
  // Compute the average dQdx of the track
  /*
  Int_t nsamples=0;
  for (UInt_t i=0;i<track.fdQdx.size();i++){
    for (UInt_t j=0;j<track.fdQdx[i].size();j++){
      part->AveragedEdx += track.fdQdx[i][j];
      nsamples++;
    }
  }

  if (nsamples>0)
    part->AveragedEdx /= (Float_t)nsamples;
  */
  // ------------------- Fill PID variables -----------------------
  
  // Vector of pids for track itrk
  std::vector<anab::ParticleID> pids;

  // Loop over the map with association between tracks and PIDs
  for (UInt_t i=0;i<PIDs_Tracks->obj.ptr_data_2_.size();i++){
    Int_t ipid =-1;
    if (PIDs_Tracks->obj.ptr_data_2_[i].second == (UInt_t)itrk){
      ipid = PIDs_Tracks->obj.ptr_data_1_[i].second;
      // fill a vector with all pids associated to the track with index itrk
      pids.push_back(PIDs->obj[ipid]);
    }
  }
  
  if(pids.size() != 3)
    std::cerr << "WARNING::PID vector size for primary is = " << pids.size() << std::endl;
    
  for (UInt_t i=0;i<pids.size();i++){

    int plane = pids[i].fPlaneID.Plane;
    if(plane < 0) continue;
    if(plane > 2) continue;
    
    part->PIDA[plane]=pids[plane].fPIDA;
    part->ReconPDG[plane] = pids[plane].fPdg;
    
    part->PID[plane][0] = pids[plane].fNdf;        
    part->PID[plane][1] = pids[plane].fMinChi2;    
    part->PID[plane][2] = pids[plane].fDeltaChi2;  
    part->PID[plane][3] = pids[plane].fChi2Proton; 
    part->PID[plane][4] = pids[plane].fChi2Kaon;   
    part->PID[plane][5] = pids[plane].fChi2Pion;   
    part->PID[plane][6] = pids[plane].fChi2Muon;   
    part->PID[plane][7] = pids[plane].fMissingE;   
    part->PID[plane][8] = pids[plane].fMissingEavg;

  }

  // ------------------- Fill Calorimetry variables -----------------------

  // Vector of calos for track itrk
  std::vector<anab::Calorimetry*> calovector = GetRecoTrackCalorimetry(track,"pandora2Track", "pandora2calo");
  
  if(calovector.size() != 3)
    std::cerr << "WARNING::Calorimetry vector size for primary is = " << calovector.size() << std::endl;

  Int_t nsamples=0;
  part->AveragedEdx = 0;
  part->AveragedQdx = 0;


  std::string filename = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/run_5387_Xcalibration.root";
  TFile cali_file(filename.c_str());
  cali_factor=static_cast<TH1F*>(cali_file.Get("dqdx_X_correction_hist")->Clone());
  cali_factor->SetDirectory(NULL);

//  Double_t CaloKin;

  for(size_t k = 0; k < calovector.size() && k<3; k++){
    int plane = calovector[k]->fPlaneID.Plane;
    if(plane < 0) continue;
    if(plane > 2) continue;
    //    CaloKin = 0.0;
    part->CALO[plane][0] = calovector[k]->fKineticEnergy;
    part->CALO[plane][1] = calovector[k]->fRange;
    if (calovector[k]->fTrkPitch.size())
      part->CALO[plane][2] = calovector[k]->fTrkPitch[0];

    part->NHitsPerPlane[plane] = calovector[k]->fdEdx.size();

    //now loop over ALL hits for computing kinetic energy
    for(UInt_t l=0;l<calovector[k]->fdEdx.size();l++){

#ifndef ISMC
      double kinetic=0;
      double res=0;
      Int_t bin;
      Double_t Cx;
      // dE/dx correction
      bin = cali_factor->FindBin(calovector[k]->fXYZ[l].X());
      if (bin<1) bin = 1;
      if (bin>148) bin = 148;
      Cx=cali_factor->GetBinContent(bin);
      double dqdxi_corr = ComputeCalibratedDqDx(Cx, calovector[k]->fdQdx[l], calovector[k]->fXYZ[l].X());
      double dedxi_corr = ComputedEdxfromdQdx(dqdxi_corr);
      double residualrangei = calovector[k]->fResidualRange[l];

      //compute kinetic energy      
      if (residualrangei<0.01 || dedxi_corr<0.01 || dedxi_corr>100)continue;
      else {
        kinetic = kinetic + dedxi_corr * (residualrangei - res);
        res = residualrangei;
      }
#endif

      //but store only the first 300 hits
      if(l<(Int_t)NMAXHITSPERPLANE){
        part->ResidualRange[plane][l] = calovector[k]->fResidualRange[l];
        part->dEdx[plane][l]          = calovector[k]->fdEdx[l];
        part->dQdx[plane][l]          = calovector[k]->fdQdx[l];
        if (calovector[k]->fdEdx.size()     == calovector[k]->fXYZ.size()){
          part->HitX[plane][l]          = calovector[k]->fXYZ[l].X();
          part->HitY[plane][l]          = calovector[k]->fXYZ[l].Y();
          part->HitZ[plane][l]          = calovector[k]->fXYZ[l].Z();
        }
#ifndef ISMC
        part->dQdx_corr[plane][l]     = dqdxi_corr;
        part->dEdx_corr[plane][l]     = dedxi_corr;
#endif

        part->AveragedEdx += calovector[k]->fdEdx[l];
        part->AveragedQdx += calovector[k]->fdQdx[l];

        //      std::cout << CaloKin << std::endl;
        //      CaloKin = CaloKin + part->dEdx_corr[plane][l]*part->HitX[plane][l];

        nsamples++;
      }
#ifdef ISMC
      // No need to loop more since for MC we don't recompute kinetic energy
      else break;
#endif
    }
    //    part->CALO[plane][0] = CaloKin;
    //    std::cout << part->CALO[plane][0] << " " << calovector[k]->fKineticEnergy  << std::endl;

#ifdef ISMC
    // repeat Kinetic in 0 an 3 such that is the same as for data
    part->CALO[plane][3] = calovector[k]->fKineticEnergy/1000;
#else    
    part->CALO[plane][3]=kinetic/units::GeV;
#endif
  }

  // Compute the average dEdx of the track
  if (nsamples>0){
    part->AveragedEdx /= (Float_t)nsamples;
    part->AveragedQdx /= (Float_t)nsamples;
  }


  // Associate a TrueObject to this Particle
#ifdef ISMC
  part->TrueObject = FindTrueParticle(itrk, trueParticles, static_cast<AnaParticle*>(part)->TruePur);  
#endif


  std::pair< double, int >  this_chi2_ndof = GetChi2PID( track, "pandora2caloSCE", templates[2212]);
  part->Chi2Proton = this_chi2_ndof.first;
  part->Chi2ndf    = this_chi2_ndof.second;

  
  //  std::cout << "TrueObject:" << part->TrueObject << std::endl;
  
  //  part->Print();
  
  //  if (part->TrueObject) static_cast<AnaTrueParticle*>(part->TrueObject)->Print();

}

//********************************************************************
Float_t LArSoftTreeConverter::ComputedEdxfromdQdx(Float_t prim_dqdx, Float_t Efield) {
//********************************************************************

  double Rho = 1.383;//g/cm^3 (liquid argon density at a pressure 18.0 psia) 
  double betap = 0.212;//(kV/cm)(g/cm^2)/MeV
  double alpha = 0.93;//parameter from ArgoNeuT experiment at 0.481kV/cm 
  double Wion = 23.6e-6;//parameter from ArgoNeuT experiment at 0.481kV/cm. In MeV/e
  //  double Efield1=0.50;//kV/cm protoDUNE electric filed
  double beta = betap/(Rho*Efield); // cm/MeV
  //  double calib_factor =6.155e-3; //right cali constant for the run 5387. This converts from ADC to e
  double dqdx;

  //  dqdx = prim_dqdx/calib_factor;
  dqdx = prim_dqdx;
  dqdx = (exp(dqdx*beta*Wion)-alpha)/beta;

  return dqdx;

}

//********************************************************************
Float_t LArSoftTreeConverter::ComputeCalibratedDqDx(Float_t Cx, Float_t prim_dqdx, Float_t prim_hitx) {
//********************************************************************

  (void)prim_hitx;

  double normalisation_factor=0.983;//for plane 2
  double cali_dqdx=Cx*prim_dqdx*normalisation_factor;
  return cali_dqdx;
}

//*****************************************************************************
void LArSoftTreeConverter::FillBasicTrackInfo(const recob::Track& track, AnaParticleMomB* part){
//*****************************************************************************

    
  // Track Unique ID
  part->UniqueID = track.fID;
    
  UInt_t lastTrajPoint = track.fTraj.fPositions.size()-1;

  for (int i=track.fTraj.fPositions.size()-1;i>=0;i--){
    if (track.fTraj.fPositions[i].X() != -999){
      lastTrajPoint=i;
      break;
    }
  }
      
  //  std::cout << "last: " << lastTrajPoint << std::endl;
  
  if (track.fTraj.fPositions.size() == 0) return;
  // Start position
  part->PositionStart[0]= track.fTraj.fPositions[0].X();
  part->PositionStart[1]= track.fTraj.fPositions[0].Y();
  part->PositionStart[2]= track.fTraj.fPositions[0].Z();

  // Start direction
  ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::GlobalCoordinateSystemTag> dir = track.fTraj.fMomenta[0].Unit();
  part->DirectionStart[0]= dir.X();
  part->DirectionStart[1]= dir.Y();
  part->DirectionStart[2]= dir.Z();

  // Start momentum
  if (track.fTraj.fHasMomentum)
    part->Momentum = track.fTraj.fMomenta[0].R();
  
  // End position
  part->PositionEnd[0]= track.fTraj.fPositions[lastTrajPoint].X();
  part->PositionEnd[1]= track.fTraj.fPositions[lastTrajPoint].Y();
  part->PositionEnd[2]= track.fTraj.fPositions[lastTrajPoint].Z();

  // End direction
  ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::GlobalCoordinateSystemTag> endDir = track.fTraj.fMomenta[lastTrajPoint].Unit();
  part->DirectionEnd[0]= endDir.X();
  part->DirectionEnd[1]= endDir.Y();
  part->DirectionEnd[2]= endDir.Z();
 
  // End momentum
  if (track.fTraj.fHasMomentum)
    part->MomentumEnd = track.fTraj.fMomenta[lastTrajPoint].R();

  // Number of hits as number of Trajectory points
  part->NHits = track.fTraj.fPositions.size();

  // Compute the track length
  part->Length = ComputeTrackLength(track);

  // dummy for the moment
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);
  SubDetId::SetDetectorSystemFields(part->Detector);
}

//*****************************************************************************
void LArSoftTreeConverter::FillParticleShowerInfo(std::vector<AnaTrueParticleB*>& trueParticles, const recob::Shower& shower, AnaParticle* part){
//*****************************************************************************

  (void)trueParticles;
  
  // Unique ID. It is -999 in LArSoft. We should may be use the PFParticle.fSelf
  part->UniqueID = shower.fID;

  // Shower length
  part->Length = shower.fLength;


  // Start position
  part->PositionStart[0] = shower.fXYZstart.X();
  part->PositionStart[1] = shower.fXYZstart.Y();
  part->PositionStart[2] = shower.fXYZstart.Z();

  // Start direction
  part->DirectionStart[0] = shower.fDCosStart.X();
  part->DirectionStart[1] = shower.fDCosStart.Y();
  part->DirectionStart[2] = shower.fDCosStart.Z();


  part->CALO[0][0] = shower.fBestPlane;
  part->CALO[0][1] = shower.fOpenAngle;

  if(shower.fTotalEnergy.size())
    part->CALO[0][2] =   shower.fTotalEnergy[0];
  
  if(shower.fTotalMIPEnergy.size())
    part->CALO[0][3] =   shower.fTotalMIPEnergy[0];

  if(shower.fdEdx.size())
    part->CALO[0][4] =   shower.fdEdx[0];
    
  //  part->Print();
}

#ifdef ISMC

//*****************************************************************************
AnaTrueObjectC* LArSoftTreeConverter::FindTrueParticle(Int_t itrk, std::vector<AnaTrueParticleB*>& trueParticles, Float_t& purity){
//*****************************************************************************
  
  // Vector of hits for track itrk
  std::vector<recob::Hit> hits;

  // Loop over the map with association between hits and trackID
  for (UInt_t i=0;i<Hits_Tracks->obj.ptr_data_2_.size();i++){
    Int_t ihit =-1;
    if (Hits_Tracks->obj.ptr_data_2_[i].second == (UInt_t)itrk){
      ihit = Hits_Tracks->obj.ptr_data_1_[i].second;
      // fill a vector with all hits associated to the track with index itrk
      hits.push_back(Hits->obj[ihit]);
    }
  }

  Int_t trackid; 
  purity=0;
  double maxe;
  // Get the MC track id for the particle with more true ionization energy from the input hits
  HitsPurity(hits, trackid, purity, maxe);

  // get the true particle with the corresponding track ID
  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == trackid) return trueParticles[i];
  }

  
  return NULL;
}

/*
// Function to find the best matched true particle to a reconstructed particle. In case of problems, returns a null pointer
const simb::MCParticle* LArSoftTreeConverter::GetMCParticleFromRecoTrack(const recob::Track &track, std::string trackModule) const{

  const simb::MCParticle* mcParticle = 0x0;

  // We must have MC for this module to make sense
  if(!_isMC) return mcParticle;

  // Get the reconstructed tracks
  //  auto allRecoTracks = evt.getValidHandle<std::vector<recob::Track> >(trackModule);

  // We need the association between the tracks and the hits
  //  const art::FindManyP<recob::Hit> findTrackHits(allRecoTracks, evt, trackModule);
  std::vector<recob::Hit> trackHits;

  
  //  unsigned int trackIndex = track.ID();
  unsigned int trackIndex = track.fID;

  art::ServiceHandle<cheat::BackTrackerService> bt_serv;
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  std::unordered_map<int, double> trkIDE; // map of track ID and Sum of Energy of the associated hits

  // Loop over hits in the input track
  for (auto const & h : findTrackHits.at(trackIndex)){
    // Loop over all ionization points associated to that hit
    for (auto const & ide : bt_serv->HitToTrackIDEs(h)) // loop over std::vector<sim::TrackIDE>
    {
        trkIDE[ide.trackID] += ide.energy; // sum energy contribution by each track ID
    }
  }

  int best_id = 0;
  double tot_e = 0, max_e = 0;
  // Loop over elements in the map to find the one with highest enery sum
  for (auto const & contrib : trkIDE)
  {
    tot_e += contrib.second;     // sum total energy in these hits
    if (contrib.second > max_e)  // find track ID corresponding to max energy
    {
        max_e = contrib.second;
        best_id = contrib.first;
    }
  }

  if ((max_e > 0) && (tot_e > 0)) // ok, found something reasonable
  {
    if (best_id < 0)            // NOTE: negative ID means this is EM activity
    {                           // caused by track with the same but positive ID
//        best_id = -best_id;     // --> we'll find mother MCParticle of these hits
      return mcParticle;
    }
    mcParticle = pi_serv->TrackIdToParticle_P(best_id); // MCParticle corresponding to track ID
  }

  return mcParticle;
}
*/

//*****************************************************************************
void LArSoftTreeConverter::FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  simb::MCNeutrino& nu = MCTruths->obj[ivertex].fMCNeutrino;

  if (nu.fNu.fstatus!=1) return;
  

  trueVertex->ID       = nu.fNu.ftrackId;
  trueVertex->NuPDG    = nu.fNu.fpdgCode;

  if (nu.fNu.ftrajectory.ftrajectory.size() == 0) return;
  trueVertex->NuEnergy = nu.fNu.ftrajectory.ftrajectory[0].second.E();

  anaUtils::VectorToArray(nu.fNu.ftrajectory.ftrajectory[0].second.Vect().Unit(),trueVertex->NuDir);
  anaUtils::VectorToArray(nu.fLepton.ftrajectory.ftrajectory[0].first,trueVertex->Position);


  vector<simb::MCParticle>& parts =  MCTruths->obj[ivertex].fPartList;  

  trueVertex->nTrueParticles = 0;
  anaUtils::CreateArray(trueVertex->TrueParticles, parts.size());

  for (UInt_t i=0;i<parts.size();i++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueParticleInfo(trueVertex, parts[i], truePart);
    trueVertex->TrueParticles[trueVertex->nTrueParticles++] = truePart;
  }
  

  trueVertex->ReacCode  = nu.fInteractionType;
  trueVertex->TargetPDG = nu.fTarget;
  trueVertex->LeptonPDG = nu.fLepton.fpdgCode;
  trueVertex->Q2        = nu.fQSqr;
  trueVertex->LeptonMom = nu.fLepton.ftrajectory.ftrajectory[0].second.Vect().Mag();

  anaUtils::VectorToArray(nu.fLepton.ftrajectory.ftrajectory[0].second.Vect().Unit(),trueVertex->LeptonDir);

}
#endif

// Length of reconstructed track, trajectory by trajectory.
//*****************************************************************************
double LArSoftTreeConverter::ComputeTrackLength(const recob::Track& track){
//*****************************************************************************

/*
  // sanity check
     if (startAt >= LastPoint()) return 0.;
     
     // just sum the distance between all locations in the trajectory
     Point_t const* curr = &(LocationAtPoint(startAt));
     Point_t const* next = curr;
     Point_t const* last = &(End());
     Coord_t length = 0.0;
     while (next++ != last) {
       length += (*next - *curr).R();
       curr = next;
     } // while
     return length;
   } // recob::Trajectory::Length()
*/
  
  double result = 0.;

  TVector3 disp(track.fTraj.fPositions[0].X(),track.fTraj.fPositions[0].Y(),track.fTraj.fPositions[0].Z());

  for(UInt_t i = 1; i < track.fTraj.fPositions.size(); ++i) {
    if (track.fTraj.fPositions[i].X() == -999) break;
    TVector3 pos(track.fTraj.fPositions[i].X(),track.fTraj.fPositions[i].Y(),track.fTraj.fPositions[i].Z());
    disp -= pos;
    result += disp.Mag();
    disp = pos;
  }

  return result;
}


#ifdef ISMC

//*****************************************************************************
void LArSoftTreeConverter::FillTrueParticleInfo(AnaTrueVertexB* trueVertex, const simb::MCParticle& artTruePart, AnaTrueParticle* truePart){
//*****************************************************************************
  
  truePart->ID       = artTruePart.ftrackId;
  truePart->ParentID = artTruePart.fmother;      
  truePart->PDG      = artTruePart.fpdgCode;

  truePart->ParentPDG  = 0;
  truePart->GParentPDG = 0;

  // Parent and GParent PDG codes
  simb::MCParticle* mother = GetMotherMCParticle(artTruePart);
  if (mother){
    truePart->ParentPDG = mother->fpdgCode;
    simb::MCParticle* gmother = GetMotherMCParticle(*mother);
    if (gmother) truePart->GParentPDG = gmother->fpdgCode;
  }
  
  // Set the daughter particles
  for (std::set<int>::iterator it = artTruePart.fdaughters.begin();it!=artTruePart.fdaughters.end();it++)
    truePart->Daughters.push_back(*it);
  
  truePart->TrueVertex = trueVertex;

  UInt_t lastTrajPoint = artTruePart.ftrajectory.ftrajectory.size()-1;
 
  TLorentzVector mom      = artTruePart.ftrajectory.ftrajectory[0].second;
  TLorentzVector endmom   = artTruePart.ftrajectory.ftrajectory[lastTrajPoint].second;
  TLorentzVector startpos = artTruePart.ftrajectory.ftrajectory[0].first;
  TLorentzVector endpos   = artTruePart.ftrajectory.ftrajectory[lastTrajPoint].first;

  truePart->Momentum    = mom.Vect().Mag();
  truePart->MomentumEnd = endmom.Vect().Mag();


  // Set the true momentum when entering in the TPC
  truePart->MomentumInTPC = GetTrueMomentumInTPC(artTruePart);

  
  anaUtils::VectorToArray(mom.Vect().Unit(),   truePart->Direction);
  anaUtils::VectorToArray(endmom.Vect().Unit(),truePart->DirectionEnd);
  anaUtils::VectorToArray(startpos,            truePart->Position);
  anaUtils::VectorToArray(endpos,              truePart->PositionEnd);
      

  // The length of the true particle
  truePart->Length = ComputeMCTrajectoryLength(artTruePart.ftrajectory, truePart->LengthInTPC);

  // The processes
  truePart->ProcessStart = truePart->ConvertProcess(artTruePart.fprocess);
  truePart->ProcessEnd   = truePart->ConvertProcess(artTruePart.fendprocess);

  //  std::cout << "anselmo 5" << std::endl;
  //  truePart->Print();
  
}

//*****************************************************************************
simb::MCParticle* LArSoftTreeConverter::GetMotherMCParticle(const simb::MCParticle& part) const{
//*****************************************************************************

  simb::MCParticle* mother=NULL;

  if (part.fmother<=0) return mother;
  
  if (part.fmother<= (Int_t)MCParticles->obj.size())
    mother = &MCParticles->obj[part.fmother-1];
  else{
    for (UInt_t i=0;i<MCParticles->obj.size();i++){
      if (MCParticles->obj[i].ftrackId == part.fmother){
        mother = &MCParticles->obj[i];
        break;
      }
    }
  }

  return mother;
}


//*****************************************************************************
double LArSoftTreeConverter::GetTrueMomentumInTPC(const simb::MCParticle& part) const{
//*****************************************************************************

  int i = 0;
  while (part.ftrajectory.ftrajectory[i].first.Z()<0.0)    i=i+1; //TPC starts at Z=0
  double mom = part.ftrajectory.ftrajectory[i].second.P();

  //  std::cout << "Mom in TPC: "<< part.ftrajectory.ftrajectory[0].second.P() << " " << i << " " << mom << std::endl;
  
  return mom;

}

//*****************************************************************************
double LArSoftTreeConverter::ComputeMCTrajectoryLength(const simb::MCTrajectory& traj, Float_t& lengthInTPC){
//*****************************************************************************

 
  const int N = traj.ftrajectory.size();
  if(N < 2) return 0;
  
  // We take the sum of the straight lines between the trajectory points
  double dist = 0;
  lengthInTPC=0;
  for(int n = 0; n < N-1; ++n){
    double l_inc = (traj.ftrajectory[n+1].first-traj.ftrajectory[n].first).Vect().Mag();
    dist += l_inc;
    if (traj.ftrajectory[n].first.Z()>0) lengthInTPC += l_inc; 
  }
  
  return dist;
}

//*****************************************************************************
void LArSoftTreeConverter::HitsPurity(std::vector<recob::Hit> const& hits, Int_t& trackid, Float_t& purity, double& maxe){
//*****************************************************************************

  // Given a vector of hits it returns the 
  
  trackid = -1;
  purity = -1;

  //  art::ServiceHandle<cheat::BackTracker> bt;

  std::map<int,double> trkide;

  if (debugTrueReco)
    std::cout << "HitsPurity: " << std::endl;

  // Loop over the input hits (the ones associated to a track)
  for(size_t h = 0; h < hits.size(); ++h){

    recob::Hit hit = hits[h];
    //    std::vector<sim::IDE> ides;
    //bt->HitToSimIDEs(hit,ides);
    //    std::vector<sim::TrackIDE> eveIDs;// = bt->HitToEveID(hit);
    //    std::cout << " hit " << h << " --> ch = " << hit.fChannel << std::endl;

    // Get the vector of ionization points for that hit
    //    std::vector<sim::IDE> eveIDs = HitToEveID(hit);

    /*
    // Loop over all IDEs
    for(size_t e = 0; e < eveIDEs.size(); ++e){
      std::cout<< h <<" "<<e<<" "<<eveIDEs[e].trackID<<" "<<eveIDEs[e].energy<<" "<<eveIDEs[e].z<<std::endl;

      // Sum the energy for the points generated by the same particle
      //      trkide[abs(eveIDs[e].trackID)] += eveIDs[e].energy;
      trkide[eveIDEs[e].trackID] += eveIDEs[e].energy;
    }
    */

    std::vector<sim::TrackIDE> trackIDEs = HitToTrackIDEs(hit);
    
    // Loop over all trackIDEs
    for(size_t e = 0; e < trackIDEs.size(); ++e){
      //      std::cout<< h <<" "<<e<<" "<<trackIDEs[e].trackID<<" "<<trackIDEs[e].energy<<std::endl;

      // Sum the energy for the points generated by the same particle
      //      trkide[abs(trackIDs[e].trackID)] += trackIDs[e].energy;
      trkide[trackIDEs[e].trackID] += trackIDEs[e].energy;
    }


  }

  maxe = -1;
  double tote = 0;
  // Find the particle with maximum energy from the input hits
  for (std::map<int,double>::iterator ii = trkide.begin(); ii!=trkide.end(); ++ii){
    tote += ii->second;
    if ((ii->second)>maxe){
      maxe = ii->second;
      //      trackid = abs(ii->first);
      trackid = ii->first;
    }
  }

  //std::cout << "the total energy of this reco track is: " << tote << std::endl;

  if (tote>0){
    purity = maxe/tote;
  }

  if (debugTrueReco)
    std::cout << "HitsPurity2: " << trackid << " " << purity << " " << maxe << " " << tote << std::endl;
}

//*****************************************************************************
std::vector<sim::TrackIDE> LArSoftTreeConverter::HitToTrackIDEs(const recob::Hit& hit){
//*****************************************************************************


    /*
    const std::vector< sim::TrackIDE > cheat::BackTracker::HitToTrackIDEs	(	recob::Hit const & 	hit	)	const
   {
    std::vector<  sim::TrackIDE > trackIDEs;
    const double start = hit.PeakTimeMinusRMS();
    const double end   = hit.PeakTimePlusRMS();
    trackIDEs = this->ChannelToTrackIDEs(hit.Channel(), start, end);
    return trackIDEs;
  }

 */



  const double start = hit.fPeakTime - hit.fRMS;
  const double end   = hit.fPeakTime + hit.fRMS;

  if (debugTrueReco)  std::cout << "HitToTrackIDEs: " << start << " " << end << std::endl;
  std::vector<  sim::TrackIDE > trackIDEs = ChannelToTrackIDEs(hit.fChannel, start, end);
  return trackIDEs;
}


//*****************************************************************************
std::vector< sim::TrackIDE > LArSoftTreeConverter::ChannelToTrackIDEs(Int_t channel, double hit_start_time, double hit_end_time) const{
//*****************************************************************************


  /*

    const std::vector< sim::TrackIDE > cheat::BackTracker::ChannelToTrackIDEs	(	raw::ChannelID_t 	channel,
    const double 	hit_start_time,
    const double 	hit_end_time 
    )		const

    std::vector< sim::TrackIDE > trackIDEs;
     double totalE=0.;
     try{
       art::Ptr<sim::SimChannel> schannel = this->FindSimChannel(channel);
 
       // loop over the electrons in the channel and grab those that are in time
       // with the identified hit start and stop times
       int start_tdc = fDetClocks->TPCTick2TDC( hit_start_time );
       int end_tdc   = fDetClocks->TPCTick2TDC( hit_end_time   );
       if(start_tdc<0) start_tdc = 0;
       if(end_tdc<0) end_tdc = 0;
       std::vector<sim::IDE> simides = schannel->TrackIDsAndEnergies(start_tdc, end_tdc);
 
       // first get the total energy represented by all track ids for
       // this channel and range of tdc values
       for(size_t e = 0; e < simides.size(); ++e)
         totalE += simides[e].energy;
 
 
       // protect against a divide by zero below
       if(totalE < 1.e-5) totalE = 1.;
 
       // loop over the entries in the map and fill the input vectors
 
       for(size_t e = 0; e < simides.size(); ++e){
 
         if(simides[e].trackID == sim::NoParticleId) continue;
 
         sim::TrackIDE info;
         info.trackID    = simides[e].trackID;
         info.energyFrac = simides[e].energy/totalE;
         info.energy     = simides[e].energy;
         info.numElectrons = simides[e].numElectrons;
 
         trackIDEs.push_back(info);
 
       }
     }// end try
     catch(cet::exception e){
       mf::LogWarning("BackTracker") << "caught exception \n"
         << e;
     }
 
     return trackIDEs;
 
   }

   */

  Float_t totalE=0.;
  std::vector< sim::TrackIDE > trackIDEs;

  sim::SimChannel* schannel = FindSimChannel(channel);

  if (!schannel) return trackIDEs;
  if (debugTrueReco)  std::cout << "ChannelToTrackIDEs(0): " << schannel->fChannel << std::endl;
  
  // loop over the electrons in the channel and grab those that are in time
  // with the identified hit start and stop times
  int start_tdc = TPCTick2TDC( hit_start_time ); 
  int end_tdc   = TPCTick2TDC( hit_end_time   ); 
  if(start_tdc<0) start_tdc = 0;
  if(end_tdc<0) end_tdc = 0;
  std::vector<sim::IDE> simides = TrackIDsAndEnergies(*schannel, start_tdc, end_tdc);


  if (debugTrueReco)  std::cout << "ChannelToTrackIDEs(1): " << start_tdc << " " << end_tdc << " " << simides.size() << std::endl;
  
  // first get the total energy represented by all track ids for
  // this channel and range of tdc values
  for(size_t e = 0; e < simides.size(); ++e)
    totalE += simides[e].energy;
  
  
  // protect against a divide by zero below
  if(totalE < 1.e-5) totalE = 1.;

  // loop over the entries in the map and fill the input vectors
  
  for(size_t e = 0; e < simides.size(); ++e){
    
    //    if(simides[e].trackID == sim::NoParticleId) continue;  //TODO
    if(simides[e].trackID == std::numeric_limits<int>::min()) continue;  //TODO
    
    sim::TrackIDE info;
    info.trackID    = simides[e].trackID;
    info.energyFrac = simides[e].energy/totalE;
    info.energy     = simides[e].energy;
    info.numElectrons = simides[e].numElectrons;
 
    trackIDEs.push_back(info);
    
  }

  if (debugTrueReco)  std::cout << "ChannelToTrackIDEs(2): " << totalE << std::endl;
  
  return trackIDEs;

}

//*****************************************************************************
Double_t LArSoftTreeConverter::TPCTick2TDC(Double_t tick) const{
//*****************************************************************************


  // Definition at line 287 of file DetectorClocksStandard.h.

  Double_t TriggerOffsetTPC_0 = detinfo::kDEFAULT_TRIG_OFFSET_TPC; 
  Double_t TriggerTime        = detinfo::kDEFAULT_TRIG_TIME;
  Double_t Frequency          = detinfo::kDEFAULT_FREQUENCY;
  Double_t TriggerOffsetTPC;
  
  if (TriggerOffsetTPC_0<0)
    TriggerOffsetTPC = TriggerOffsetTPC_0; 
  else
    TriggerOffsetTPC = -TriggerOffsetTPC_0/Frequency; //convert ticks to us


  double TickPeriod =  1./Frequency;
  
  double doTPCTime =  TriggerTime + TriggerOffsetTPC;
  
  return ( doTPCTime / TickPeriod + tick ); 

}

//*****************************************************************************
sim::SimChannel* LArSoftTreeConverter::FindSimChannel(Int_t channel)	const{
//*****************************************************************************


  /*

    art::Ptr< sim::SimChannel > cheat::BackTracker::FindSimChannel	(	raw::ChannelID_t 	channel	)	const
                                                                                {
     art::Ptr<sim::SimChannel> chan;
     auto ilb = std::lower_bound(fSimChannels.begin(),fSimChannels.end(),channel,[](art::Ptr<sim::SimChannel> a, raw::    ChannelID_t channel) {return(a->Channel()<channel);});
     if (ilb != fSimChannels.end())
       if ( (*ilb)->Channel() == channel) {chan = *ilb;}
     if(!chan)
       throw cet::exception("BackTracker") << "No sim::SimChannel corresponding "
         << "to channel: " << channel << "\n";
     return chan;
   }

   */


  for (UInt_t j=0;j<SimChannels->obj.size();j++){
    if ((UInt_t)channel == SimChannels->obj[j].fChannel){
      return  &SimChannels->obj[j];
    }
  }

  return NULL;
}

//*****************************************************************************
struct LArSoftTreeConverter::CompareByTDC {
//*****************************************************************************
  
  bool operator()
  (TDCIDE const& a, TDCIDE const& b) const
  { return a.first < b.first; }

  bool operator()
  (StoredTDC_t a_tdc, TDCIDE const& b) const
  { return a_tdc < b.first; }
  
  bool operator()
  (TDCIDE const& a, StoredTDC_t b_tdc) const
  { return a.first < b_tdc; }

}; // struct CompareByTDC

//*****************************************************************************
TDCIDEs_t::const_iterator  LArSoftTreeConverter::findClosestTDCIDE(const sim::SimChannel& chan, StoredTDC_t tdc) const{	
//*****************************************************************************

  if (debugTrueReco)  std::cout << "findClosestTDCIDE(0): " << chan.fChannel << " " << chan.fTDCIDEs.size() << " " << tdc << std::endl;


  return std::lower_bound(chan.fTDCIDEs.begin(), chan.fTDCIDEs.end(), tdc, CompareByTDC());
}

//*****************************************************************************
std::vector<sim::IDE> LArSoftTreeConverter::TrackIDsAndEnergies(const sim::SimChannel& chan, Int_t startTDC, Int_t endTDC) const {
//*****************************************************************************


  /*

    std::vector< sim::IDE > sim::SimChannel::TrackIDsAndEnergies	(	TDC_t 	startTDC,
    TDC_t 	endTDC 
    )		const

    Return all the recorded energy deposition within a time interval.
    
    Parameters
    startTDC	TDC tick opening the time window
    endTDC	TDC tick closing the time window (included in the interval)
    Returns
    a collection of energy deposit information from all tracks
    This method returns the energy deposited on this channel by each track ID active in the specified TDC time interval.
    
    Each entry pertains a single track ID. For each entry, all energy deposit information is merged into a single record. It includes:
    
    energy and number of electrons, as the integral in the time interval
    position, as average weighted by the number of electrons
    the ID of the track depositing this energy
    Entries are sorted by track ID number.
    
    Definition at line 178 of file SimChannel.cxx.

   {
     // make a map of track ID values to sim::IDE objects
 
     if(startTDC > endTDC ){
       mf::LogWarning("SimChannel") << "requested tdc range is bogus: "
                                    << startTDC << " " << endTDC
                                    << " return empty vector";
       return {}; // returns an empty vector
     }
 
     std::map<TrackID_t, sim::IDE> idToIDE;
     
       //find the lower bound for this tdc and then iterate from there
     auto itr = findClosestTDCIDE(startTDC);
     
     while(itr != fTDCIDEs.end()){
       
       // check the tdc value for the iterator, break the loop if we
       // are outside the range
       if(itr->first > endTDC) break;
       
       // grab the vector of IDEs for this tdc
       auto const& idelist = itr->second;
       // now loop over them and add their content to the map
       for(auto const& ide : idelist){
         auto itTrkIDE = idToIDE.find(ide.trackID);
         if( itTrkIDE != idToIDE.end() ){
           // the IDE we are going to update:
           sim::IDE& trackIDE = itTrkIDE->second;
           
           double const nel1   = trackIDE.numElectrons;
           double const nel2   = ide.numElectrons;
           double const en1    = trackIDE.energy;
           double const en2    = ide.energy;
           double const energy = en1  + en2;
           double const weight = nel1 + nel2;
           
             // make a weighted average for the location information
           trackIDE.x            = (ide.x*nel2 + trackIDE.x*nel1)/weight;
           trackIDE.y            = (ide.y*nel2 + trackIDE.y*nel1)/weight;
           trackIDE.z            = (ide.z*nel2 + trackIDE.z*nel1)/weight;
           trackIDE.numElectrons = weight;
           trackIDE.energy = energy;
         } // end if the track id for this one is found
         else{
           idToIDE[ide.trackID] = sim::IDE(ide);
         }
       } // end loop over vector
       
       ++itr;
     } // end loop over tdc values
     
       // now fill the vector with the ides from the map
     std::vector<sim::IDE> ides;
     ides.reserve(idToIDE.size());
     for(auto const& itr : idToIDE){
       ides.push_back(itr.second);
     }
     
     return ides;
   }

   */

  //  std::map<TrackID_t, sim::IDE> idToIDE;
  std::map<Int_t, sim::IDE> idToIDE;  //TODO
  //find the lower bound for this tdc and then iterate from there
  
  auto itr = findClosestTDCIDE(chan,startTDC); 

  if (debugTrueReco)  std::cout << "TrackIDsAndEnergies(0): " << startTDC << " " << std::endl;

  
  while(itr != chan.fTDCIDEs.end()){
    
    // check the tdc value for the iterator, break the loop if we
    // are outside the range
    if(itr->first > endTDC) break;

    // grab the vector of IDEs for this tdc
    auto const& idelist = itr->second;
    // now loop over them and add their content to the map
    for(auto const& ide : idelist){
      auto itTrkIDE = idToIDE.find(ide.trackID);
      if( itTrkIDE != idToIDE.end() ){
        // the IDE we are going to update:
        sim::IDE& trackIDE = itTrkIDE->second;
        
        double const nel1   = trackIDE.numElectrons;
        double const nel2   = ide.numElectrons;
        double const en1    = trackIDE.energy;
        double const en2    = ide.energy;
        double const energy = en1  + en2;
        double const weight = nel1 + nel2;
        
        // make a weighted average for the location information
        trackIDE.x            = (ide.x*nel2 + trackIDE.x*nel1)/weight;
        trackIDE.y            = (ide.y*nel2 + trackIDE.y*nel1)/weight;
        trackIDE.z            = (ide.z*nel2 + trackIDE.z*nel1)/weight;
        trackIDE.numElectrons = weight;
        trackIDE.energy = energy;
      } // end if the track id for this one is found
      else{
        idToIDE[ide.trackID] = sim::IDE(ide);
      }
    } // end loop over vector
    
    ++itr;
  } // end loop over tdc values
  
  // now fill the vector with the ides from the map
  std::vector<sim::IDE> ides;
  ides.reserve(idToIDE.size());
  for(auto const& itr : idToIDE){
    ides.push_back(itr.second);
  }
  
  return ides;
  
}

//*****************************************************************************
std::vector<sim::IDE> LArSoftTreeConverter::HitToEveID(const recob::Hit& hit){
//*****************************************************************************

  // Given a hit it gives the vector of ionization points


  /* IDE
    Ionization at a point of the TPC sensitive volume.

    This class stores information about the ionization from the simulation of a small step of a track through the TPC active volume.
    
    Ionization information consists of both energy and number of electrons. It is of paramount importance to understand what each field stores:
    
    position: where the ionization occurred (from Geant4 simulation)
    track ID: Geant4 track ID of the ionizing particle
    energy: amount of energy released by ionization (from Geant4 simulation)
    electrons: amount of electrons reaching the readout channel
    Note the different definition of the electrons respect to the rest: it describes the electrons at the anode after the drifting occurred, while all the other quantities can be related to the moment the ionization happened.
    
    The number of electrons typically includes inefficiencies and physics effects that reduce and spread the electrons. In the simulation, this yields a fractional number of electrons.
    
    Each IDE is also typically associated with a time (TDC) count, that is the time at which the ionized electrons reached the readout channel, in electronic ticks, as opposed as the time when ionization occurred. The latter is not stored.
    
    At the time of writing this documentation (LArSoft 6.4.0), IDEs are computed in larg4::LArVoxelReadout. The energy and track ID come directly from Geant4 simulation. The position is the mid point of the Geant4 step that produced ionization. The electrons are
    
    converted from that same energy (using a fundamental conversion factor stored in larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h)
    applied recombination effect by larg4::IonizationAndScintillation::Reset()
    applied attenuation and diffusion in larg4::LArVoxelReadout::DriftIonizationElectrons()
    The latter also assembles the sim::IDE objects to be stored into sim::SimChannel.
    
    Definition at line 87 of file SimChannel.h.

    // Data Members.
    int         trackID;     //Geant4 supplied track ID
    float       numElectrons;    //number of electrons at the readout for this track ID and time
    float       energy;          //energy deposited by ionization by this track ID and time [MeV]
    float       x;               //x position of ionization [cm]
    float       y;               //y position of ionization [cm]
    float       z;               //z position of ionization [cm]

        
  */

  
  for (UInt_t j=0;j<SimChannels->obj.size();j++){
    if (hit.fChannel == SimChannels->obj[j].fChannel && SimChannels->obj[j].fTDCIDEs.size()>0){ 
      return (*SimChannels->obj[j].fTDCIDEs.begin()).second;
    }
  }
  return std::vector<sim::IDE>();
}

//*****************************************************************************
const simb::MCParticle* LArSoftTreeConverter::GetGeantGoodParticle(const simb::MCTruth &genTruth) const{
//*****************************************************************************
  
  // Get the good particle from the MCTruth
  simb::MCParticle goodPart;
  bool found = false;
  for(UInt_t t = 0; t < genTruth.fPartList.size(); ++t){
    simb::MCParticle part = genTruth.fPartList[t];
    if(part.fprocess == "primary"){
      goodPart = part;
      found = true;
      break;
    }
  }

  if(!found){
    std::cerr << "No good particle found, returning null pointer" << std::endl;
    return NULL;
  }

  // Now loop over geant particles to find the one that matches
  // Get list of the g4 particles. plist should be a std::map< int, simb::MCParticle* >

  for (UInt_t i=0;i<MCParticles->obj.size();i++){
    if((goodPart.fpdgCode == MCParticles->obj[i].fpdgCode) && fabs(goodPart.ftrajectory.ftrajectory[0].second.E() - MCParticles->obj[i].ftrajectory.ftrajectory[0].second.E()) < 1e-5){
      return &(MCParticles->obj[i]);
    }
  } 
  
  // If we get here something has gone wrong
  std::cerr << "No G4 version of the good particle was found, returning null pointer" << std::endl;
  return NULL;
}

#endif


//*****************************************************************************
double LArSoftTreeConverter::GetTrackMomentum(double trkrange, int pdg) const {
//*****************************************************************************
  
  /* Muon range-momentum tables from CSDA (Argon density = 1.4 g/cm^3)
     website:
     http://pdg.lbl.gov/2012/AtomicNuclearProperties/MUON_ELOSS_TABLES/muonloss_289.pdf
     
     CSDA table values:
     float Range_grampercm[30] = {9.833E-1, 1.786E0, 3.321E0,
     6.598E0, 1.058E1, 3.084E1, 4.250E1, 6.732E1, 1.063E2, 1.725E2,
     2.385E2, 4.934E2, 6.163E2, 8.552E2, 1.202E3, 1.758E3, 2.297E3,
     4.359E3, 5.354E3, 7.298E3, 1.013E4, 1.469E4, 1.910E4, 3.558E4,
     4.326E4, 5.768E4, 7.734E4, 1.060E5, 1.307E5}; float KE_MeV[30] = {10, 14,
     20, 30, 40, 80, 100, 140, 200, 300, 400, 800, 1000, 1400, 2000, 3000,
     4000, 8000, 10000, 14000, 20000, 30000, 40000, 80000, 100000, 140000,
     200000, 300000, 400000};
     
     Functions below are obtained by fitting polynomial fits to KE_MeV vs
     Range (cm) graph. A better fit was obtained by splitting the graph into
     two: Below range<=200cm,a polynomial of power 4 was a good fit; above
     200cm, a polynomial of power 6 was a good fit
     
     Fit errors for future purposes:
     Below 200cm, Forpoly4 fit: p0 err=1.38533;p1 err=0.209626; p2
     err=0.00650077; p3 err=6.42207E-5; p4 err=1.94893E-7; Above 200cm,
     Forpoly6 fit: p0 err=5.24743;p1 err=0.0176229; p2 err=1.6263E-5; p3
     err=5.9155E-9; p4 err=9.71709E-13; p5 err=7.22381E-17;p6
     err=1.9709E-21;*/
  
  //*********For muon, the calculations are valid up to 1.91E4 cm range
  //corresponding to a Muon KE of 40 GeV**********//
  
  /*Proton range-momentum tables from CSDA (Argon density = 1.4 g/cm^3):
    website: https://physics.nist.gov/PhysRefData/Star/Text/PSTAR.html
    
    CSDA values:
    double KE_MeV_P_Nist[31]={10, 15, 20, 30, 40, 80, 100, 150, 200, 250, 300,
    350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000,
    1500, 2000, 2500, 3000, 4000, 5000};
    
    double Range_gpercm_P_Nist[31]={1.887E-1,3.823E-1, 6.335E-1, 1.296,
    2.159, 7.375, 1.092E1, 2.215E1, 3.627E1, 5.282E1, 7.144E1,
    9.184E1, 1.138E2, 1.370E2, 1.614E2, 1.869E2, 2.132E2, 2.403E2,
    2.681E2, 2.965E2, 3.254E2, 3.548E2, 3.846E2, 4.148E2, 4.454E2,
    7.626E2, 1.090E3, 1.418E3, 1.745E3, 2.391E3, 3.022E3};
    
    Functions below are obtained by fitting power and polynomial fits to
    KE_MeV vs Range (cm) graph. A better fit was obtained by splitting the
    graph into two: Below range<=80cm,a a*(x^b) was a good fit; above 80cm, a
    polynomial of power 6 was a good fit
    
    Fit errors for future purposes:
    For power function fit: a=0.388873; and b=0.00347075
    Forpoly6 fit: p0 err=3.49729;p1 err=0.0487859; p2 err=0.000225834; p3
    err=4.45542E-7; p4 err=4.16428E-10; p5 err=1.81679E-13;p6
    err=2.96958E-17;*/
  
  //*********For proton, the calculations are valid up to 3.022E3 cm range
  //corresponding to a Muon KE of 5 GeV**********//
  
  if (trkrange < 0 || std::isnan(trkrange)) {
    std::cout << "TrackMomentumCalculator   " 
              << "Invalid track range " << trkrange << " return -1" << std::endl;
    return -1.;
  }

    
  double KE, Momentum, M;
  constexpr double Muon_M = 105.7, Proton_M = 938.272;
  
  if (abs(pdg) == 13) {
    M = Muon_M;
    KE = KEvsR_spline3.Eval(trkrange);
  } else if (abs(pdg) == 2212) {
    M = Proton_M;
    if (trkrange > 0 && trkrange <= 80)
      KE = 29.9317 * std::pow(trkrange, 0.586304);
    else if (trkrange > 80 && trkrange <= 3.022E3)
      KE =
        149.904 + (3.34146 * trkrange) + (-0.00318856 * trkrange * trkrange) +
        (4.34587E-6 * trkrange * trkrange * trkrange) +
        (-3.18146E-9 * trkrange * trkrange * trkrange * trkrange) +
        (1.17854E-12 * trkrange * trkrange * trkrange * trkrange * trkrange) +
        (-1.71763E-16 * trkrange * trkrange * trkrange * trkrange * trkrange *
         trkrange);
    else
      KE = -999;
  } else
    KE = -999;
  
  if (KE < 0)
    Momentum = -999;
  else
    Momentum = std::sqrt((KE * KE) + (2 * M * KE));
  
  Momentum = Momentum / 1000;
  
  return Momentum;
}


// Get the track associated to this particle. Returns a null pointer if not found.
//*****************************************************************************
const recob::Track* LArSoftTreeConverter::GetPFParticleTrack(const recob::PFParticle &particle) const{
//*****************************************************************************

  // Vector of pids for track itrk
  std::vector<recob::Track*> pfpTracks;
  
  // Loop over the map with association between PFParticles and Tracks
  for (UInt_t j=0;j<PFParticles_Tracks->obj.ptr_data_1_.size();j++){
    Int_t itrack =-1;
    if (PFParticles_Tracks->obj.ptr_data_1_[j].second == (UInt_t)particle.fSelf){
      itrack = PFParticles_Tracks->obj.ptr_data_2_[j].second;
      //      std::cout << "anselmo " << j << " " << itrack << " " << particle.fSelf << std::endl;
      // fill a vector with all tracks associated to the track with index itrk
      pfpTracks.push_back(&Tracks->obj[itrack]);
    }
  }
  
  //  std::cout << "an0: " << pfpTracks.size() << std::endl; 

  // Check that the track exists
  if(pfpTracks.size() != 0){
    return (pfpTracks[0]);
  }
  else{
    return NULL;
  }
  
}

// Get the shower associated to this particle. Returns a null pointer if not found.
//*****************************************************************************
const recob::Shower* LArSoftTreeConverter::GetPFParticleShower(const recob::PFParticle &particle) const{
//*****************************************************************************

  // Vector of pids for shower itrk
  std::vector<recob::Shower*> pfpShowers;
  
  // Loop over the map with association between PFParticles and Showers
  for (UInt_t j=0;j<PFParticles_Showers->obj.ptr_data_1_.size();j++){
    Int_t ishower =-1;
    if (PFParticles_Showers->obj.ptr_data_1_[j].second == (UInt_t)particle.fSelf){
      ishower = PFParticles_Showers->obj.ptr_data_2_[j].second;
      //      std::cout << "anselmo " << j << " " << ishower << " " << particle.fSelf << std::endl;
      // fill a vector with all showers associated to the shower with index itrk
      pfpShowers.push_back(&Showers->obj[ishower]);
    }
  }
  
  //  std::cout << "an0: " << pfpShowers.size() << std::endl; 

  // Check that the shower exists
  if(pfpShowers.size() != 0){
    return (pfpShowers[0]);
  }
  else{
    return NULL;
  }
  
}



// Function to find the interaction vertex of a primary PFParticle
//*****************************************************************************
const TVector3 LArSoftTreeConverter::GetPFParticleVertex(const recob::PFParticle &particle) const{
//*****************************************************************************

  // Vector of vertices
  std::vector<recob::Vertex*> vertices;
  
  // Loop over the map with association between PFParticles and Vertices
  for (UInt_t j=0;j<PFParticles_Vertices->obj.ptr_data_1_.size();j++){
    Int_t ivertex =-1;
    if (PFParticles_Vertices->obj.ptr_data_1_[j].second == (UInt_t)particle.fSelf){
      ivertex = PFParticles_Vertices->obj.ptr_data_2_[j].second;
      //      std::cout << "anselmo " << j << " " << ivertex << " " << particle.fSelf << std::endl;
      // fill a vector with all vertex associated to the vertex with index itrk
      vertices.push_back(&Vertices->obj[ivertex]);
    }
  }

  // What happens next depends on the type of event.
  // Not primary    -> just use the pfparticle vertex
  // Shower objects -> just use the pfparticle vertex
  // Cosmics        -> use track start point
  // Beam           -> use track start point

//  std::cout << "PFParticle daughters " << particle.NumDaughters() << std::endl;

  // Non-primary particle or shower-like primary particle
  //  if(!particle.IsPrimary() || !IsPFParticleTracklike(particle)){


  if(particle.fParent != std::numeric_limits<size_t>::max() || abs(particle.fPdgCode) == 11){
    if(vertices.size() != 0){
      const recob::Vertex* vtx = vertices[0];
      return TVector3(vtx->pos_.X(),vtx->pos_.Y(),vtx->pos_.Z());
    }
    else{
      std::cerr << "Non track-like PFParticle has no vertex?! Return default vector" << std::endl;
      return TVector3();
    }
  }
  else{
    // Cosmic or track-like beam primary particle
        
    const recob::Track* track = GetPFParticleTrack(PFParticles->obj[0]);

    if(track){

      UInt_t last = track->fTraj.fPositions.size()-1;
      const TVector3 start(track->fTraj.fPositions[0].X(),track->fTraj.fPositions[0].Y(),track->fTraj.fPositions[0].Z());
      const TVector3 end  (track->fTraj.fPositions[last].X(),track->fTraj.fPositions[last].Y(),track->fTraj.fPositions[last].Z());
      // Return the most upstream point as some cases where the track is reversed...
      //      if(IsBeamParticle(particle,evt,particleLabel)){
      if(false){ 
        if(start.Z() < end.Z()) return start;
        else return end;
      }
      // Return the highest point for cosmics
      else{
        if(start.Y() > end.Y()) return start;
        else return end;
      }
    }
    else{
      std::cerr << "No track found for track-like PFParticle?! Return default vector" << std::endl;
      return TVector3();
    }

  }

}


// Return the pointers for the PFParticles in the beam slice
//*****************************************************************************
const std::vector<const recob::PFParticle*> LArSoftTreeConverter::GetPFParticlesFromBeamSlice(const std::string particleLabel) const{
//*****************************************************************************

  (void)particleLabel;  

  
  if (debug)
    std::cout << "1. GetPFParticlesFromBeamSlice" << std::endl;
  
  unsigned short beamSlice = GetBeamSlice("-beam");
  if (debug)
    std::cout << "1.a. beam slice 1: " << beamSlice << std::endl;

  return GetPFParticlesFromSlice(beamSlice,"-particles");
}



// Try to get the slice tagged as beam
//*****************************************************************************
unsigned short LArSoftTreeConverter::GetBeamSlice(const std::string particleLabel) const{
//*****************************************************************************

  if (debug)
    std::cout << "2. GetBeamSlice " << std::endl;
  
  const std::map<unsigned int, std::vector<const recob::PFParticle*> > sliceMap = GetPFParticleSliceMap(particleLabel);

  if (debug)
    std::cout << "2.a. GetBeamSlice: " << sliceMap.size() << std::endl;
  
  for(auto slice : sliceMap){
    for(auto particle : slice.second){
      if(IsBeamParticle(*particle,particleLabel)){
        if (debug)
          std::cout << "2.b. GetBeamSlice: IsBeamParticle: " << slice.first << std::endl;
        return slice.first;
      }
    }
  }

  return 9999;

}


// Use the pandora metadata to tell us if this is a beam particle or not
//*****************************************************************************
bool LArSoftTreeConverter::IsBeamParticle(const recob::PFParticle &particle, const std::string particleLabel) const{
//*****************************************************************************

  if (debug)
    std::cout << "7. IsBeamParticle " << std::endl;
  return FindBoolInMetaData(particle,particleLabel,"IsTestBeam");

}


//*****************************************************************************
bool LArSoftTreeConverter::FindBoolInMetaData(const recob::PFParticle &particle, const std::string particleLabel, const std::string entry) const{
//*****************************************************************************


  if (debug)
    std::cout << "8. FindBoolInMetaData " << std::endl;
  
  std::map<std::string,float> mdMap = GetPFParticleMetaData(particle,particleLabel);

  if (debug)
    std::cout << "8.a. FindBoolInMetaData: " << mdMap.size() << std::endl;



  std::map<std::string,float>::iterator it;
  for (it = mdMap.begin(); it != mdMap.end(); it++)    
    if (debug)
      std::cout << "  - " << it->first << " " << it->second << std::endl;
  
  if(mdMap.find(entry) != mdMap.end()){
    return true;
  }
  else{
    return false;
  }

}

//*****************************************************************************
const std::map<std::string,float> LArSoftTreeConverter::GetPFParticleMetaData(const recob::PFParticle &particle, const std::string particleLabel) const {
//*****************************************************************************
  
  (void)particleLabel;
  
  // Vector of MetaDatas
  std::vector<larpandoraobj::PFParticleMetadata*> metadatas;

  if (debug)
    std::cout << "    6. GetPFParticleMetaData: " << PFParticles_MetaData->obj.ptr_data_1_.size() << std::endl;
    
  // Loop over the map with association between PFParticles and MetaData
  for (UInt_t j=0;j<PFParticles_MetaData->obj.ptr_data_1_.size();j++){
    Int_t imd =-1;
    if (PFParticles_MetaData->obj.ptr_data_1_[j].second == (UInt_t)particle.fSelf){
      imd = PFParticles_MetaData->obj.ptr_data_2_[j].second;
      if (debug)
        std::cout << "    6.a GetPFParticleMetaData: imd = " << imd << std::endl;
      // fill a vector with all vertex associated to the metadata with index 
      metadatas.push_back(&MetaData->obj[imd]);
    }
  }

  if (debug)
    std::cout << "    6.b. GetPFParticleMetaData: " <<       metadatas.size() << std::endl;

  if (metadatas.size()){
    if (debug)
      std::cout << "    6.c. GetPFParticleMetaData: " <<       metadatas[0]->m_propertiesMap.size() << std::endl;
  
    return metadatas[0]->m_propertiesMap;
  }

  return std::map<std::string,float>();
}

// Returns pointers for the primary PFParticles in a slice
//*****************************************************************************
const std::vector<const recob::PFParticle*> LArSoftTreeConverter::GetPFParticlesFromSlice(const unsigned short slice, const std::string particleLabel) const{
//*****************************************************************************

  if (debug)
    std::cout << "9. GetPFParticlesFromSlice" << std::endl;
  
  const std::map<unsigned int, std::vector<const recob::PFParticle*> > sliceMap = GetPFParticleSliceMap(particleLabel);

  if (debug)
    std::cout << "9.a. GetPFParticlesFromSlice: " << sliceMap.size() << std::endl;
  std::map<unsigned int, std::vector<const recob::PFParticle*> >::const_iterator it;
  for (it = sliceMap.begin();it!=sliceMap.end(); it++)
    if (debug)
      std::cout << "  - " << it->first << " " << it->second.size() << std::endl;
  
  if(sliceMap.find(slice) != sliceMap.end()){
    if (debug)
      std::cout << "9.b. GetPFParticlesFromSlice: " << sliceMap.at(slice).size() << std::endl;
    return sliceMap.at(slice);
  }
  else{
    return std::vector<const recob::PFParticle*>();
  }

}

// Return a map of primary particles grouped by their reconstructed slice. Useful for finding slices with multiple particles
//*****************************************************************************
const std::map<unsigned int,std::vector<const recob::PFParticle*> > LArSoftTreeConverter::GetPFParticleSliceMap(const std::string particleLabel) const{
//*****************************************************************************

  if (debug)
    std::cout << "3. GetPFParticleSliceMap"  << std::endl;

  return SliceMapHelper(particleLabel,true);
}

// Helper to get slice maps and avoid duplicate code
//*****************************************************************************
const std::map<unsigned int,std::vector<const recob::PFParticle*> > LArSoftTreeConverter::SliceMapHelper(const std::string particleLabel, bool primaryOnly) const{
//*****************************************************************************

  if (debug)
    std::cout << "4. SliceMapHelper " << std::endl;
  
  // Get the particles
  //  auto pfParticles = evt.getValidHandle<std::vector<recob::PFParticle>>(particleLabel);

  std::map<unsigned int, std::vector<const recob::PFParticle*> > sliceMap;

  for (UInt_t i=0;i<PFParticles->obj.size();i++){
    const recob::PFParticle* particle = &(PFParticles->obj[i]);

    //  Only the primary particles have the slice association
    if(primaryOnly && particle->fParent != std::numeric_limits<size_t>::max()) continue;
    
    unsigned int thisSlice = GetPFParticleSliceIndex(*particle,particleLabel);
    if (debug)
      std::cout << "  4.a. SliceMapHelper: " << thisSlice << std::endl;
    if(thisSlice != 9999){
      sliceMap[thisSlice].push_back(particle);
    }

  }
  if (debug)
    std::cout << "4.b. SliceMapHelper: " << sliceMap.size() << std::endl;
  std::map<unsigned int, std::vector<const recob::PFParticle*> >::const_iterator it;
  if (debug)
    for (it = sliceMap.begin();it!=sliceMap.end(); it++)
      std::cout << "  - " <<  it->first << " " << it->second.size() << std::endl;

  

  return sliceMap;
}


// Get the reconstructed slice associated with a particle
//*****************************************************************************
unsigned short LArSoftTreeConverter::GetPFParticleSliceIndex(const recob::PFParticle &particle, const std::string particleLabel) const{
//*****************************************************************************
  
  // Try to use slices if we can
  /*
  try{
    const recob::Slice* slice = GetPFParticleSlice(particle,particleLabel);
    //    return slice->ID();
    return 0;
  }
  // Otherwise fall back on metadata
  catch(...){
  */

  if (debug)
    std::cout << "  5. GetPFParticleSliceIndex " << std::endl;
  
  std::map<std::string,float> mdMap = GetPFParticleMetaData(particle,particleLabel);

  if (debug)
    std::cout << "  5.a. GetPFParticleSliceIndex: " << particle.fSelf << " " << particleLabel << std::endl;
  std::map<std::string,float>::iterator it;
  if (debug)
    for (it = mdMap.begin(); it != mdMap.end(); it++)    
      std::cout << "    - " << it->first << " " << it->second << std::endl;
  
  
  std::string search = "SliceIndex";
  if(mdMap.find(search) != mdMap.end()){
    return static_cast<unsigned short>(mdMap.at(search));
  }
  else{
    //    std::cerr << "Object has no slice index... returning 9999" << std::endl;
    return 9999;
  }
  
  //  }

}


// Get the reconstructed slice associated with a particle

//*****************************************************************************
//const recob::Slice* LArSoftTreeConverter::GetPFParticleSlice(const recob::PFParticle &particle, const std::string particleLabel) const{
//*****************************************************************************

/*  
  // Perhaps we should use the associations to do this? 
  auto pfParticles = evt.getValidHandle<std::vector<recob::PFParticle>>(particleLabel);
  const art::FindOneP<recob::Slice> findSlice(pfParticles,evt,particleLabel);

  const recob::Slice* slice = findSlice.at(particle.Self()).get();

  return slice;
}

*/

//********************************************************************
//Float_t LArSoftTreeConverter::ComputeKineticEnergy(const std::vector<anab::Calorimetry> &calovector, Int_t plane) {
  //********************************************************************

/*

  int nhits=calovector[k].fdEdx.size();
  double kinetic=0;
  double res=0;
  for (int i=0;i<nhits;i++){
    double dedxi = part.dEdx_corr[plane][i];
    //    double dedxi = part.dEdx[0][i];
    double Residualrangei = part.ResidualRange[plane][i];
    if (Residualrangei<0.01 || dedxi<0.01 || dedxi>100)continue;
    else {
      kinetic = kinetic + dedxi * (Residualrangei - res);
      res = Residualrangei;
    }
  }
  // convert to GeV
  return kinetic/units::GeV;
}
*/


//*****************************************************************************
/*
cnnOutput2D LArSoftTreeConverter::GetCNNOutputFromPFParticle( const recob::PFParticle & part,
                                                              //                                                              const art::Event & evt, 
                                                              const anab::MVAReader<recob::Hit,4> & CNN_results,
                                                              //                                                              protoana::ProtoDUNEPFParticleUtils & pfpUtil,
                                                              std::string fPFParticleTag ){
*/
//*****************************************************************************
/*  
    cnnOutput2D output;
    const std::vector< art::Ptr< recob::Hit > > daughterPFP_hits; //= pfpUtil.GetPFParticleHits_Ptrs( part, evt, fPFParticleTag );    

    for( size_t h = 0; h < daughterPFP_hits.size(); ++h ){
      std::array<float,4> cnn_out = CNN_results.getOutput( daughterPFP_hits[h] );
      output.track  += cnn_out[ CNN_results.getIndex("track") ];
      output.em     += cnn_out[ CNN_results.getIndex("em") ];
      output.michel += cnn_out[ CNN_results.getIndex("michel") ];
      output.none   += cnn_out[ CNN_results.getIndex("none") ];
    }

    output.nHits = daughterPFP_hits.size();

    return output;
  }
}
*/
//*****************************************************************************
//const std::vector< art::Ptr< recob::Hit > > LArSoftTreeConverter::GetPFParticleHits_Ptrs(const recob::PFParticle &particle, art::Event const &evt, const std::string particleLabel) const{
//*****************************************************************************
/*
  // This method gets the vector of hits associated to a PFParticle

  // Get all clsuters associated to the PFPArticle
  const std::vector<const recob::Cluster*> pfpClusters = GetPFParticleClusters(particle,evt,particleLabel);

  // Get all clusters in the event
  auto allClusters = evt.getValidHandle<std::vector<recob::Cluster>>(particleLabel);

  // Association between hits and clusters
  const art::FindManyP<recob::Hit> findHits(allClusters,evt,particleLabel);

  // Store all of the hits in a single vector 
  std::vector< art::Ptr< recob::Hit > > pfpHits;
  for(auto cluster : pfpClusters){
    const std::vector<art::Ptr<recob::Hit>> clusterHits = findHits.at(cluster->ID());
    for(auto hit : clusterHits){
      pfpHits.push_back(hit);
    }
  }

  return pfpHits;
}
*/
//*****************************************************************************
//const std::vector<const recob::Cluster*> LArSoftTreeConverter::GetPFParticleClusters(const recob::PFParticle &particle, art::Event const &evt, const std::string particleLabel) const{
//*****************************************************************************
/*  
  // Get the particles and their associations
  //  auto particles = evt.getValidHandle<std::vector<recob::PFParticle>>(particleLabel);

  for (UInt_t i=0;i<PFParticles->obj.size();i++){
    const recob::PFParticle* particle = &(PFParticles->obj[i]);



  const art::FindManyP<recob::Cluster> findClusters(particles,evt,particleLabel);
  const std::vector<art::Ptr<recob::Cluster>> pfpClusters = findClusters.at(particle.Self());

  // We don't want the art::Ptr so we need to get rid of it
  std::vector<const recob::Cluster*> clusters;
  for(auto pointer : pfpClusters){
    clusters.push_back(pointer.get());
  }  

  return clusters;
}
*/
#ifndef ISMC
//*****************************************************************************
const std::vector< short > & LArSoftTreeConverter::GetActiveFibers(const std::vector<std::pair<std::string,beam::FBM> >& fiberMonitors,  const std::string& FBMName) 	{
//*****************************************************************************
  
  for (int i=0;i<fiberMonitors.size();i++){
    if (fiberMonitors[0].first == FBMName) return fiberMonitors[0].second.active; 
  }

  std::cout << "FBM " << FBMName << " not found in list" << std::endl;
  return std::vector<short>();
}
#endif

//*****************************************************************************
std::pair< double, int > LArSoftTreeConverter::GetChi2PID( const recob::Track &track, const std::string caloModule, TProfile * profile ){
//*****************************************************************************

  UInt_t planeID = 0;
  

  std::vector< double > cali_dEdX_SCE = GetCalibratedCalorimetry(track, planeID, "pandora2Track", caloModule );
  
  std::vector< anab::Calorimetry* > dummy_calo = GetRecoTrackCalorimetry(track, "pandora2Track", "pandora2calo");  
  auto dummy_Range = dummy_calo[0]->fResidualRange;

  std::vector<anab::Calorimetry*> calovector = GetRecoTrackCalorimetry(track,"pandora2Track", "pandora2calo");
  /*
  for(size_t k = 0; k < calovector.size() && k<3; k++){
    int plane = calovector[k]->fPlaneID.Plane;
    if(plane < 0) continue;
    if(plane > 2) continue;
    //    CaloKin = 0.0;
    //now loop over ALL hits for computing kinetic energy
    for(UInt_t l=0;l<calovector[k]->fdEdx.size();l++){
      double residualrangei = calovector[k]->fResidualRange[l];
    }
  }
  */
  std::pair< double, int > this_chi2_ndof = Chi2PID( cali_dEdX_SCE, dummy_Range, profile );
  return this_chi2_ndof;
}
    
//*****************************************************************************
std::pair< double, int > LArSoftTreeConverter::Chi2PID( const std::vector< double > & track_dedx, const std::vector< float > & range, TProfile * profile ){
//*****************************************************************************	

  double pid_chi2 = 0.; 
  int npt = 0;

  
  if( track_dedx.size() < 1 || range.size() < 1 )
    return std::make_pair(9999., -1);
  
  //Ignore first and last point
  for( size_t i = 1; i < track_dedx.size()-1; ++i ){
    //Skip large pulse heights
    if( track_dedx[i] > 1000. )
      continue;

    int bin = profile->FindBin( range[i] );

    if( bin >= 1 && bin <= profile->GetNbinsX() ){
      
      double template_dedx = profile->GetBinContent( bin );
      if( template_dedx < 1.e-6 ){
        template_dedx = ( profile->GetBinContent( bin - 1 ) + profile->GetBinContent( bin + 1 ) ) / 2.;        
      }
      
      
      double template_dedx_err = profile->GetBinError( bin );
      if( template_dedx_err < 1.e-6 ){
        template_dedx_err = ( profile->GetBinError( bin - 1 ) + profile->GetBinError( bin + 1 ) ) / 2.;        
      }

      double dedx_res = 0.04231 + 0.0001783 * track_dedx[i] * track_dedx[i];      
      dedx_res *= track_dedx[i]; 
      
      
      //Chi2 += ( track_dedx - template_dedx )^2  / ( (template_dedx_err)^2 + (dedx_res)^2 )      
      pid_chi2 += ( pow( (track_dedx[i] - template_dedx), 2 ) / ( pow(template_dedx_err, 2) + pow(dedx_res, 2) ) ); 
            
      ++npt;      
    }	
  }
		
  if( npt == 0 )	
    return std::make_pair(9999., -1);
	  		
  return std::make_pair(pid_chi2, npt); 	
}

  
//*pandora2Track, "pandora2Track", "pandora2calo"){
//*****************************************************************************
std::vector<anab::Calorimetry*> LArSoftTreeConverter::GetRecoTrackCalorimetry(const recob::Track &track,
                                                                              const std::string trackModule,
                                                                              const std::string caloModule ) {
//*****************************************************************************

  // Vector of calos for track itrk
  std::vector<anab::Calorimetry*> calovector;

  calovector.clear();

  Int_t itrk = track.fID;
  
  if (caloModule == "pandora2calo"){
    // Loop over the map with association between tracks and CALOs
    for (UInt_t i=0;i<CALOs_Tracks->obj.ptr_data_2_.size();i++){
      Int_t icalo =-1;
      if (CALOs_Tracks->obj.ptr_data_2_[i].second == (UInt_t)itrk){
        icalo = CALOs_Tracks->obj.ptr_data_1_[i].second;
        // fill a vector with all calos associated to the track with index itrk
        calovector.push_back(&CALOs->obj[icalo]);
      }
    }
  }
  else if (caloModule == "pandora2caloSCE"){
    // Loop over the map with association between tracks and CALOs
    for (UInt_t i=0;i<CALOsSCE_Tracks->obj.ptr_data_2_.size();i++){
      Int_t icalo =-1;
      if (CALOsSCE_Tracks->obj.ptr_data_2_[i].second == (UInt_t)itrk){
        icalo = CALOsSCE_Tracks->obj.ptr_data_1_[i].second;
        // fill a vector with all calos associated to the track with index itrk
        calovector.push_back(&CALOsSCE->obj[icalo]);
      }
    }
  }
  
  return calovector;
}

/*
const std::vector< art::Ptr< recob::Hit > > daughterPFP_hits = pfpUtil.GetPFParticleHits_Ptrs( part, evt, fPFParticleTag );    

for( size_t h = 0; h < daughterPFP_hits.size(); ++h ){
  std::array<float,4> cnn_out = CNN_results.getOutput( daughterPFP_hits[h] );
}

std::array<float, N> anab::MVAReader< T, N >::getOutput 	( 	art::Ptr< T > const &  	item	) 	const
{ return FVectorReader<T, N>::getVector(item.key()); }

std::array<float, N> anab::FVectorReader< T, N >::getVector 	( 	size_t  	key	) 	const
{
    std::array<float, N> vout;
    for (size_t i = 0; i < N; ++i) vout[i] = (*fVectors)[key][i];
    return vout;
}

*/


//***************************************************************
std::vector< double >  LArSoftTreeConverter::GetCalibratedCalorimetry(const recob::Track &track,
                                                                     UInt_t planeID, 
                                                                     //                                                                     art::Event const &evt,
                                                                     const std::string trackModule,
                                                                     const std::string caloModule ) {
//***************************************************************	
	


  
  std::vector< double > calibrated_dEdx;
  
  //Get the Calorimetry vector from the track
  
  std::vector< anab::Calorimetry* > caloVector = GetRecoTrackCalorimetry( track, trackModule, caloModule );       
  size_t calo_position;  
  bool found_plane = false;
  
  for( size_t i = 0; i < caloVector.size(); ++i ){	
    unsigned int thePlane = caloVector[i]->fPlaneID.Plane;	
    if( thePlane == planeID ){      
      calo_position = i;      
      found_plane = true;      
      break;      
    }	
  }
  
  
  if( !found_plane ){	
    std::cout << "Could not find the correct plane in the calorimetry vector" << std::endl;	
    return calibrated_dEdx;	
  }
  

  std::vector< float > dQdX = caloVector[calo_position]->fdQdx;  
  auto theXYZPoints = caloVector[calo_position]->fXYZ; 
  std::vector< float > resRange = caloVector[calo_position]->fResidualRange;
    
  //Get the hits from the track from a specific plane

  /*
  const std::vector< const recob::Hit* > hits;// = trackUtil.GetRecoTrackHitsFromPlane( track, trackModule, planeID ); 
  
  if( hits.size() == 0 ){
	
    std::cout << "Got empty hits vector" << std::endl;
	
    return calibrated_dEdx;
	
  }
  */

  //Do Ajib's correction 
  
  for( size_t i = 0; i < dQdX.size(); ++i ){ 
    float hit_x = theXYZPoints[i].X();
    float hit_y = theXYZPoints[i].Y();
    float hit_z = theXYZPoints[i].Z();
	
    if( hit_y < 0. || hit_y > 600. ) continue;
    if( hit_z < 0. || hit_z > 695. ) continue;
	
    int X_bin = X_correction_hist->FindBin( hit_x );
    float X_correction = X_correction_hist->GetBinContent(X_bin);
	
    double YZ_correction = (
                            ( hit_x < 0 )
                            ? YZ_neg->GetBinContent( YZ_neg->FindBin( hit_z, hit_y ) ) 
                            : YZ_pos->GetBinContent( YZ_pos->FindBin( hit_z, hit_y ) )  
                            );

    float norm_factor = 0.983; // for plane 2
    double calib_factor =6.155e-3; //right cali constant for the run 5387. This converts from ADC to e
    float corrected_dq_dx = dQdX[i] * X_correction * YZ_correction * norm_factor;	
    float scaled_corrected_dq_dx = corrected_dq_dx / calib_factor;		
    double Efield = tot_Ef( hit_x, hit_y, hit_z );
    
    //    float cal_de_dx = calc_dEdX( scaled_corrected_dq_dx,  betap,  Rho,  Efield,  Wion,  alpha );
    float cal_de_dx = ComputedEdxfromdQdx(scaled_corrected_dq_dx, Efield);

    calibrated_dEdx.push_back( cal_de_dx );	
  }
	
	
	
 return calibrated_dEdx;

}




//***************************************************************
double LArSoftTreeConverter::tot_Ef( double x, double y, double z ){
//***************************************************************
  
  if( x >= 0 ){
    double ex = 0.5 + 0.5 * ex_pos->GetBinContent( ex_pos->FindBin( x, y, z ) );
    double ey = 0.5 * ey_pos->GetBinContent( ey_pos->FindBin( x, y, z ) );
    double ez = 0.5 * ez_pos->GetBinContent( ez_pos->FindBin( x, y, z ) );
    return sqrt( (ex*ex) + (ey*ey) + (ez*ez) );
  }
  else if( x < 0 ){
    double ex= 0.5 + 0.5 * ex_neg->GetBinContent( ex_neg->FindBin( x, y, z ) );
    double ey= 0.5 * ey_neg->GetBinContent( ey_neg->FindBin( x, y, z ) );
    double ez= 0.5 * ez_neg->GetBinContent( ez_neg->FindBin( x, y, z ) );	
    return sqrt( (ex*ex) + (ey*ey) + (ez*ez) );	
  }
  
  else return 0.5;
	
}


//***************************************************************
//const std::vector<const recob::Hit*> LArSoftTreeConverter::GetRecoTrackHitsFromPlane(const recob::Track &track,  const std::string trackModule, unsigned int planeID ) const{
//***************************************************************
/*  
  std::vector<const recob::Hit*> trackHits;
  if( planeID > 2 ){
    std::cout << "Please input plane 0, 1, or 2" << std::endl;
    return trackHits;
  }

  auto recoTracks = evt.getValidHandle<std::vector<recob::Track> >(trackModule);
  art::FindManyP<recob::Hit> findHits(recoTracks,evt,trackModule);
  std::vector<art::Ptr<recob::Hit>> inputHits = findHits.at(track.ID());

  for(const art::Ptr<recob::Hit> hit : inputHits){
    unsigned int thePlane = hit.get()->WireID().asPlaneID().Plane;
    if( thePlane != planeID ) continue;
       
    trackHits.push_back(hit.get());

  }


  // Vector of hits for track itrk
  std::vector<recob::Hit> hits;

  // Loop over the map with association between hits and trackID
  for (UInt_t i=0;i<Hits_Tracks->obj.ptr_data_2_.size();i++){
    Int_t ihit =-1;
    if (Hits_Tracks->obj.ptr_data_2_[i].second == (UInt_t)itrk){
      ihit = Hits_Tracks->obj.ptr_data_1_[i].second;
      // fill a vector with all hits associated to the track with index itrk
      hits.push_back(Hits->obj[ihit]);
    }
  }


  
  return trackHits;  

}

*/

#ifndef ISMC
//***************************************************************
std::vector< int > LArSoftTreeConverter::GetPID( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum ){
//***************************************************************
  const auto& thePIDCands = GetPIDCandidates(beamevt, nominal_momentum);
  std::vector< int > thePIDs = thePIDCands.getPDGCodes();
  return thePIDs;        
}

//***************************************************************
PossibleParticleCands LArSoftTreeConverter::GetPIDCandidates( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum ){
//***************************************************************
  return GetPIDCandidates_CERNCalib(beamevt,nominal_momentum);
}
//***************************************************************
PossibleParticleCands LArSoftTreeConverter::GetPIDCandidates_CERNCalib( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum ){
//***************************************************************
  PossibleParticleCands candidates;


  bool fUseCERNCalibSelection=true;
  
  //Check if momentum is in valid set
  std::vector< double > valid_momenta = {1., 2., 3., 6., 7.};
  if( std::find(valid_momenta.begin(), valid_momenta.end(), nominal_momentum) == valid_momenta.end() ){
    std::cout << "Reference momentum " << nominal_momentum << " not valid" << std::endl;
    return candidates;
  }
  //Get the high/low pressure Cerenkov info
  //int high_pressure_status, low_pressure_status; 
  int high_pressure_status = beamevt.CKov0.trigger;
  int low_pressure_status  = beamevt.CKov1.trigger;
  //std::cout << "Pressures: " << beamevt.GetCKov0Pressure() << " " << beamevt.GetCKov1Pressure() << std::endl;

  //if( beamevt.GetCKov0Pressure() < beamevt.GetCKov1Pressure() ){
  //  high_pressure_status = beamevt.GetCKov1Status();
  //  low_pressure_status = beamevt.GetCKov0Status();
  //}
  //else{
  //  high_pressure_status = beamevt.GetCKov0Status();
  //  low_pressure_status = beamevt.GetCKov1Status();
  //}

  if( nominal_momentum == 1. ){
    if( beamevt.TOFChan == -1 ){
      std::cout << "TOF invalid" << std::endl;
      return candidates;
    }
    if( low_pressure_status == -1 ){
      std::cout << "High pressure status invalid" << std::endl;
      return candidates;
    }
    const double & tof = beamevt.theTOF;
    if ( 
        ((fUseCERNCalibSelection && tof < 105.) 
            || (!fUseCERNCalibSelection && tof < 170.))
        && low_pressure_status == 1 
       ) {
      candidates.electron = true;
    }
    else if ( 
        ((fUseCERNCalibSelection && tof < 110.) 
            || (!fUseCERNCalibSelection && tof < 170.))
        && low_pressure_status == 0 ){
      candidates.muon = true;
      candidates.pion = true;
    }
    else if ( 
        ((fUseCERNCalibSelection && tof > 110. && tof < 160.) 
            || (!fUseCERNCalibSelection && tof > 170.))
        && low_pressure_status == 0 ) {
      candidates.proton = true;
    }
  }
  else if( nominal_momentum == 2. ){
    if( beamevt.TOFChan == -1 ){
      std::cout << "TOF invalid" << std::endl;
      return candidates;
    }
    if( low_pressure_status == -1 ){
      std::cout << "High pressure Cerenkov status invalid" << std::endl;
      return candidates;
    }
    const double & tof = beamevt.theTOF;
    if ( 
        ((fUseCERNCalibSelection && tof < 105.) 
            || (!fUseCERNCalibSelection && tof < 160.))
        && low_pressure_status == 1 
       ) {
      candidates.electron = true;
    }
    else if ( 
        ((fUseCERNCalibSelection && tof < 103.) 
            || (!fUseCERNCalibSelection && tof < 160.))
        && low_pressure_status == 0 ){
      candidates.muon = true;
      candidates.pion = true;
    }
    else if ( 
        ((fUseCERNCalibSelection && tof > 103. && tof < 160.) 
            || (!fUseCERNCalibSelection && tof > 160.))
        && low_pressure_status == 0 ) {
      candidates.proton = true;
    }
  }
  else if( nominal_momentum == 3. ){
    if( high_pressure_status == -1 || low_pressure_status == -1 ){
      std::cout << "At least one Cerenkov status invalid " << std::endl;
      std::cout << "High: " << high_pressure_status << " Low: " << low_pressure_status << std::endl;
      return candidates;
    }
    else if ( low_pressure_status == 1 && high_pressure_status == 1 ) 
      candidates.electron = true;
    else if ( low_pressure_status == 0 && high_pressure_status == 1 ){
      candidates.muon = true;
      candidates.pion = true;
    }
    else{ // low, high = 0, 0
      candidates.proton = true;
      candidates.kaon = true; 
    }
  }
  else if( nominal_momentum == 6. || nominal_momentum == 7. ){
    if( high_pressure_status == -1 || low_pressure_status == -1 ){
      std::cout << "At least one Cerenkov status invalid " << std::endl;
      std::cout << "High: " << high_pressure_status << " Low: " << low_pressure_status << std::endl;
      return candidates;
    }
    else if ( low_pressure_status == 1 && high_pressure_status == 1 ){
      candidates.electron = true;
      candidates.muon = true;
      candidates.pion = true;
    }
    else if ( low_pressure_status == 0 && high_pressure_status == 1 ) 
      candidates.kaon = true; 
    else  // low, high = 0, 0
      candidates.proton = true;
  }
  return candidates;

}
#endif



//*****************************************************************************
cnnOutput2D LArSoftTreeConverter::GetCNNOutputFromPFParticle( const recob::PFParticle & part,
                                                              //                                                              const art::Event & evt, 
                                                              //                                                              const anab::MVAReader<recob::Hit,4> & CNN_results,
                                                              //                                                              protoana::ProtoDUNEPFParticleUtils & pfpUtil,
                                                              std::string fPFParticleTag ){
//*****************************************************************************
  
  cnnOutput2D output;
  
  int itrack  = GetMVAIndex("emtrkmichelrecobHit","track");
  int iem     = GetMVAIndex("emtrkmichelrecobHit","em");
  int imichel = GetMVAIndex("emtrkmichelrecobHit","michel");
  int inone   = GetMVAIndex("emtrkmichelrecobHit","none");
  
  const std::vector<int> PFParticle_hits_indices = GetPFParticleHitsIndices(part);
  
  for( size_t h = 0; h < PFParticle_hits_indices.size(); ++h ){      
    output.track  += MVA->obj[h].fData[itrack];
    output.em     += MVA->obj[h].fData[iem];
    output.none   += MVA->obj[h].fData[imichel];
    output.michel += MVA->obj[h].fData[inone];      
  }

  output.nHits = PFParticle_hits_indices.size();
  
  return output;
}

//*****************************************************************************
const std::vector<int> LArSoftTreeConverter::GetPFParticleHitsIndices(const recob::PFParticle &part){
//*****************************************************************************

  // Vector of hits for track itrk
  std::vector<int> indices;


  const recob::Track* track = GetPFParticleTrack(part);
  if (track){
    // Loop over the map with association between hits and trackID
    for (UInt_t i=0;i<Hits_Tracks->obj.ptr_data_2_.size();i++){
      Int_t ihit =-1;
      if (Hits_Tracks->obj.ptr_data_2_[i].second == (UInt_t)track->fID){
        ihit = Hits_Tracks->obj.ptr_data_1_[i].second;
        // fill a vector with all hits associated to the track with index itrk
        indices.push_back(ihit);
      }
    }
  }else{
    const recob::Shower* shower = GetPFParticleShower(part);
    if (shower){
      // Loop over the map with association between hits and showerID
      for (UInt_t i=0;i<Hits_Showers->obj.ptr_data_2_.size();i++){
        Int_t ihit =-1;
        if (Hits_Showers->obj.ptr_data_2_[i].second == (UInt_t)shower->fID){
          ihit = Hits_Showers->obj.ptr_data_1_[i].second;
          // fill a vector with all hits associated to the shower with index itrk
          indices.push_back(ihit);
        }
      }
    }
  }

  return indices;
}


//*****************************************************************************
int LArSoftTreeConverter::GetMVAIndex(const std::string& tag, const std::string& name){
//*****************************************************************************

  for (UInt_t i =0;i<MVADescription->obj.size();i++){
    if (MVADescription->obj[i].fOutputInstance != tag) continue;
    for (Int_t j =0;j<4;j++){
      if (MVADescription->obj[i].fOutputNames[j] == name) return j;
    }
  }

  return -1;
}
