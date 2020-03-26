#include "pionTreeConverter.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"

/*

  - true_beam_ID:                       true ID of the true beam particle
  - true_beam_reco_byHits_PFP_ID:       rec  ID of the PFP associated to the true beam particle
  - true_beam_reco_byHits_allTrack_ID   rec  ID of the track associated to the PFP associated to the true beam particle

  - reco_beam_PFP_ID:                   rec ID of the PFP tagged as beam by Pandora
  - reco_beam_allTrack_ID:              rec ID of the track associated to the PFP tagged as beam by Pandora

  - reco_beam_true_byHits_ID            true ID of the true particle matched to the reco beam particle
  - reco_beam_true_byE_ID               true ID of the true particle matched to the reco beam particle


 */


bool byHits = true;

//********************************************************************
pionTreeConverter::pionTreeConverter():InputConverter("pionana/beamana"){
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
bool pionTreeConverter::Initialize(){
//********************************************************************
  
  std::string folder= "pionana";

  AddChain(folder+"/beamana");
  eventsTree = GetChain(folder+"/beamana");

  fChain = eventsTree;


   // Set object pointer
   reco_beam_dQdX = 0;
   reco_beam_dEdX = 0;
   reco_beam_calibrated_dEdX = 0;
   reco_beam_resRange = 0;
   reco_beam_TrkPitch = 0;
   reco_beam_calo_wire = 0;
   reco_beam_calo_tick = 0;
   reco_beam_allTrack_resRange = 0;
   reco_beam_allTrack_calibrated_dEdX = 0;
   reco_daughter_PFP_true_byHits_PDG = 0;
   reco_daughter_PFP_true_byHits_ID = 0;
   reco_daughter_PFP_true_byHits_origin = 0;
   reco_daughter_PFP_true_byHits_parID = 0;
   reco_daughter_PFP_true_byHits_parPDG = 0;
   reco_daughter_PFP_true_byHits_process = 0;
   reco_daughter_PFP_true_byHits_sharedHits = 0;
   reco_daughter_PFP_true_byHits_emHits = 0;
   reco_daughter_PFP_true_byHits_len = 0;
   reco_daughter_PFP_true_byHits_startX = 0;
   reco_daughter_PFP_true_byHits_startY = 0;
   reco_daughter_PFP_true_byHits_startZ = 0;
   reco_daughter_PFP_true_byHits_endX = 0;
   reco_daughter_PFP_true_byHits_endY = 0;
   reco_daughter_PFP_true_byHits_endZ = 0;
   reco_daughter_PFP_true_byHits_startPx = 0;
   reco_daughter_PFP_true_byHits_startPy = 0;
   reco_daughter_PFP_true_byHits_startPz = 0;
   reco_daughter_PFP_true_byHits_startP = 0;
   reco_daughter_PFP_true_byHits_startE = 0;
   reco_daughter_PFP_true_byHits_endProcess = 0;
   reco_daughter_PFP_true_byHits_purity = 0;
   reco_daughter_allTrack_ID = 0;
   reco_daughter_allTrack_dEdX = 0;
   reco_daughter_allTrack_dQdX = 0;
   reco_daughter_allTrack_resRange = 0;
   reco_daughter_allTrack_dQdX_SCE = 0;
   reco_daughter_allTrack_dEdX_SCE = 0;
   reco_daughter_allTrack_resRange_SCE = 0;
   reco_daughter_allTrack_calibrated_dEdX = 0;
   reco_daughter_allTrack_calibrated_dEdX_SCE = 0;
   reco_daughter_allTrack_Chi2_proton = 0;
   reco_daughter_allTrack_Chi2_ndof = 0;
   reco_daughter_allTrack_Theta = 0;
   reco_daughter_allTrack_Phi = 0;
   reco_daughter_allTrack_len = 0;
   reco_daughter_allTrack_startX = 0;
   reco_daughter_allTrack_startY = 0;
   reco_daughter_allTrack_startZ = 0;
   reco_daughter_allTrack_endX = 0;
   reco_daughter_allTrack_endY = 0;
   reco_daughter_allTrack_endZ = 0;
   reco_daughter_allTrack_dR = 0;
   reco_daughter_allTrack_to_vertex = 0;
   reco_daughter_allShower_ID = 0;
   reco_daughter_allShower_len = 0;
   reco_daughter_allShower_startX = 0;
   reco_daughter_allShower_startY = 0;
   reco_daughter_allShower_startZ = 0;
   reco_daughter_PFP_ID = 0;
   reco_daughter_PFP_nHits = 0;
   reco_daughter_PFP_trackScore = 0;
   reco_daughter_PFP_emScore = 0;
   reco_daughter_PFP_michelScore = 0;
   reco_daughter_PFP_trackScore_collection = 0;
   reco_daughter_PFP_emScore_collection = 0;
   reco_daughter_PFP_michelScore_collection = 0;
   true_beam_endProcess = 0;
   true_beam_elastic_costheta = 0;
   true_beam_elastic_X = 0;
   true_beam_elastic_Y = 0;
   true_beam_elastic_Z = 0;
   true_beam_reco_byHits_PFP_ID = 0;
   true_beam_reco_byHits_PFP_nHits = 0;
   true_beam_reco_byHits_allTrack_ID = 0;
   reco_beam_vertex_dRs = 0;
   reco_beam_vertex_hits_slices = 0;
   true_beam_daughter_PDG = 0;
   true_beam_daughter_ID = 0;
   true_beam_daughter_len = 0;
   true_beam_daughter_startX = 0;
   true_beam_daughter_startY = 0;
   true_beam_daughter_startZ = 0;
   true_beam_daughter_startPx = 0;
   true_beam_daughter_startPy = 0;
   true_beam_daughter_startPz = 0;
   true_beam_daughter_startP = 0;
   true_beam_daughter_endX = 0;
   true_beam_daughter_endY = 0;
   true_beam_daughter_endZ = 0;
   true_beam_daughter_Process = 0;
   true_beam_daughter_endProcess = 0;
   true_beam_daughter_nHits = 0;
   true_beam_daughter_reco_byHits_PFP_ID = 0;
   true_beam_daughter_reco_byHits_PFP_nHits = 0;
   true_beam_daughter_reco_byHits_PFP_trackScore = 0;
   true_beam_daughter_reco_byHits_allTrack_ID = 0;
   true_beam_daughter_reco_byHits_allTrack_startX = 0;
   true_beam_daughter_reco_byHits_allTrack_startY = 0;
   true_beam_daughter_reco_byHits_allTrack_startZ = 0;
   true_beam_daughter_reco_byHits_allTrack_endX = 0;
   true_beam_daughter_reco_byHits_allTrack_endY = 0;
   true_beam_daughter_reco_byHits_allTrack_endZ = 0;
   true_beam_daughter_reco_byHits_allTrack_len = 0;
   true_beam_daughter_reco_byHits_allShower_ID = 0;
   true_beam_daughter_reco_byHits_allShower_startX = 0;
   true_beam_daughter_reco_byHits_allShower_startY = 0;
   true_beam_daughter_reco_byHits_allShower_startZ = 0;
   true_beam_daughter_reco_byHits_allShower_len = 0;
   true_beam_Pi0_decay_ID = 0;
   true_beam_Pi0_decay_parID = 0;
   true_beam_Pi0_decay_PDG = 0;
   true_beam_Pi0_decay_startP = 0;
   true_beam_Pi0_decay_len = 0;
   true_beam_Pi0_decay_nHits = 0;
   true_beam_Pi0_decay_reco_byHits_PFP_ID = 0;
   true_beam_Pi0_decay_reco_byHits_PFP_nHits = 0;
   true_beam_Pi0_decay_reco_byHits_PFP_trackScore = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_ID = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_startX = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_startY = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_startZ = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_endX = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_endY = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_endZ = 0;
   true_beam_Pi0_decay_reco_byHits_allTrack_len = 0;
   true_beam_Pi0_decay_reco_byHits_allShower_ID = 0;
   true_beam_Pi0_decay_reco_byHits_allShower_startX = 0;
   true_beam_Pi0_decay_reco_byHits_allShower_startY = 0;
   true_beam_Pi0_decay_reco_byHits_allShower_startZ = 0;
   true_beam_Pi0_decay_reco_byHits_allShower_len = 0;
   true_beam_grand_daughter_ID = 0;
   true_beam_grand_daughter_parID = 0;
   true_beam_grand_daughter_PDG = 0;
   true_beam_grand_daughter_nHits = 0;
   true_beam_grand_daughter_Process = 0;
   true_beam_grand_daughter_endProcess = 0;
   reco_beam_true_byE_endProcess = 0;
   reco_beam_true_byE_process = 0;
   reco_beam_true_byHits_endProcess = 0;
   reco_beam_true_byHits_process = 0;
   true_beam_processes = 0;
   data_BI_PDG_candidates = 0;
   quality_reco_view_0_wire = 0;
   quality_reco_view_1_wire = 0;
   quality_reco_view_2_wire = 0;
   quality_reco_view_2_z = 0;
   quality_reco_view_0_tick = 0;
   quality_reco_view_1_tick = 0;
   quality_reco_view_2_tick = 0;
   reco_beam_cosmic_candidate_lower_hits = 0;
   reco_beam_cosmic_candidate_upper_hits = 0;
   reco_beam_cosmic_candidate_ID = 0;
   cosmic_has_beam_IDE = 0;
   reco_beam_incidentEnergies = 0;
   true_beam_incidentEnergies = 0;
   reco_beam_spacePts_X = 0;
   reco_beam_spacePts_Y = 0;
   reco_beam_spacePts_Z = 0;
   reco_daughter_spacePts_X = 0;
   reco_daughter_spacePts_Y = 0;
   reco_daughter_spacePts_Z = 0;
   reco_daughter_shower_spacePts_X = 0;
   reco_daughter_shower_spacePts_Y = 0;
   reco_daughter_shower_spacePts_Z = 0;

  
  // Set branch addresses and branch pointers

  if (!fChain) return false;
  fCurrent = -1;

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("MC", &MC, &b_MC);
   fChain->SetBranchAddress("reco_beam_type", &reco_beam_type, &b_reco_beam_type);
   fChain->SetBranchAddress("reco_beam_startX", &reco_beam_startX, &b_reco_beam_startX);
   fChain->SetBranchAddress("reco_beam_startY", &reco_beam_startY, &b_reco_beam_startY);
   fChain->SetBranchAddress("reco_beam_startZ", &reco_beam_startZ, &b_reco_beam_startZ);
   fChain->SetBranchAddress("reco_beam_endX", &reco_beam_endX, &b_reco_beam_endX);
   fChain->SetBranchAddress("reco_beam_endY", &reco_beam_endY, &b_reco_beam_endY);
   fChain->SetBranchAddress("reco_beam_endZ", &reco_beam_endZ, &b_reco_beam_endZ);
   fChain->SetBranchAddress("reco_beam_len", &reco_beam_len, &b_reco_beam_len);
   fChain->SetBranchAddress("reco_beam_trackDirX", &reco_beam_trackDirX, &b_reco_beam_trackDirX);
   fChain->SetBranchAddress("reco_beam_trackDirY", &reco_beam_trackDirY, &b_reco_beam_trackDirY);
   fChain->SetBranchAddress("reco_beam_trackDirZ", &reco_beam_trackDirZ, &b_reco_beam_trackDirZ);
   fChain->SetBranchAddress("reco_beam_trackEndDirX", &reco_beam_trackEndDirX, &b_reco_beam_trackEndDirX);
   fChain->SetBranchAddress("reco_beam_trackEndDirY", &reco_beam_trackEndDirY, &b_reco_beam_trackEndDirY);
   fChain->SetBranchAddress("reco_beam_trackEndDirZ", &reco_beam_trackEndDirZ, &b_reco_beam_trackEndDirZ);
   fChain->SetBranchAddress("reco_beam_vtxX", &reco_beam_vtxX, &b_reco_beam_vtxX);
   fChain->SetBranchAddress("reco_beam_vtxY", &reco_beam_vtxY, &b_reco_beam_vtxY);
   fChain->SetBranchAddress("reco_beam_vtxZ", &reco_beam_vtxZ, &b_reco_beam_vtxZ);
   fChain->SetBranchAddress("reco_beam_trackID", &reco_beam_trackID, &b_reco_beam_trackID);
   fChain->SetBranchAddress("reco_beam_dQdX", &reco_beam_dQdX, &b_reco_beam_dQdX);
   fChain->SetBranchAddress("reco_beam_dEdX", &reco_beam_dEdX, &b_reco_beam_dEdX);
   fChain->SetBranchAddress("reco_beam_calibrated_dEdX", &reco_beam_calibrated_dEdX, &b_reco_beam_calibrated_dEdX);
   fChain->SetBranchAddress("reco_beam_resRange", &reco_beam_resRange, &b_reco_beam_resRange);
   fChain->SetBranchAddress("reco_beam_TrkPitch", &reco_beam_TrkPitch, &b_reco_beam_TrkPitch);
   fChain->SetBranchAddress("reco_beam_calo_wire", &reco_beam_calo_wire, &b_reco_beam_calo_wire);
   fChain->SetBranchAddress("reco_beam_calo_tick", &reco_beam_calo_tick, &b_reco_beam_calo_tick);
   fChain->SetBranchAddress("reco_beam_nTrackDaughters", &reco_beam_nTrackDaughters, &b_reco_beam_nTrackDaughters);
   fChain->SetBranchAddress("reco_beam_nShowerDaughters", &reco_beam_nShowerDaughters, &b_reco_beam_nShowerDaughters);
   fChain->SetBranchAddress("reco_beam_flipped", &reco_beam_flipped, &b_reco_beam_flipped);
   fChain->SetBranchAddress("reco_beam_passes_beam_cuts", &reco_beam_passes_beam_cuts, &b_reco_beam_passes_beam_cuts);
   fChain->SetBranchAddress("reco_beam_PFP_ID", &reco_beam_PFP_ID, &b_reco_beam_PFP_ID);
   fChain->SetBranchAddress("reco_beam_PFP_nHits", &reco_beam_PFP_nHits, &b_reco_beam_PFP_nHits);
   fChain->SetBranchAddress("reco_beam_PFP_trackScore", &reco_beam_PFP_trackScore, &b_reco_beam_PFP_trackScore);
   fChain->SetBranchAddress("reco_beam_PFP_emScore", &reco_beam_PFP_emScore, &b_reco_beam_PFP_emScore);
   fChain->SetBranchAddress("reco_beam_PFP_michelScore", &reco_beam_PFP_michelScore, &b_reco_beam_PFP_michelScore);
   fChain->SetBranchAddress("reco_beam_PFP_trackScore_collection", &reco_beam_PFP_trackScore_collection, &b_reco_beam_PFP_trackScore_collection);
   fChain->SetBranchAddress("reco_beam_PFP_emScore_collection", &reco_beam_PFP_emScore_collection, &b_reco_beam_PFP_emScore_collection);
   fChain->SetBranchAddress("reco_beam_PFP_michelScore_collection", &reco_beam_PFP_michelScore_collection, &b_reco_beam_PFP_michelScore_collection);
   fChain->SetBranchAddress("reco_beam_allTrack_ID", &reco_beam_allTrack_ID, &b_reco_beam_allTrack_ID);
   fChain->SetBranchAddress("reco_beam_allTrack_beam_cuts", &reco_beam_allTrack_beam_cuts, &b_reco_beam_allTrack_beam_cuts);
   fChain->SetBranchAddress("reco_beam_allTrack_flipped", &reco_beam_allTrack_flipped, &b_reco_beam_allTrack_flipped);
   fChain->SetBranchAddress("reco_beam_allTrack_len", &reco_beam_allTrack_len, &b_reco_beam_allTrack_len);
   fChain->SetBranchAddress("reco_beam_allTrack_startX", &reco_beam_allTrack_startX, &b_reco_beam_allTrack_startX);
   fChain->SetBranchAddress("reco_beam_allTrack_startY", &reco_beam_allTrack_startY, &b_reco_beam_allTrack_startY);
   fChain->SetBranchAddress("reco_beam_allTrack_startZ", &reco_beam_allTrack_startZ, &b_reco_beam_allTrack_startZ);
   fChain->SetBranchAddress("reco_beam_allTrack_endX", &reco_beam_allTrack_endX, &b_reco_beam_allTrack_endX);
   fChain->SetBranchAddress("reco_beam_allTrack_endY", &reco_beam_allTrack_endY, &b_reco_beam_allTrack_endY);
   fChain->SetBranchAddress("reco_beam_allTrack_endZ", &reco_beam_allTrack_endZ, &b_reco_beam_allTrack_endZ);
   fChain->SetBranchAddress("reco_beam_allTrack_trackDirX", &reco_beam_allTrack_trackDirX, &b_reco_beam_allTrack_trackDirX);
   fChain->SetBranchAddress("reco_beam_allTrack_trackDirY", &reco_beam_allTrack_trackDirY, &b_reco_beam_allTrack_trackDirY);
   fChain->SetBranchAddress("reco_beam_allTrack_trackDirZ", &reco_beam_allTrack_trackDirZ, &b_reco_beam_allTrack_trackDirZ);
   fChain->SetBranchAddress("reco_beam_allTrack_trackEndDirX", &reco_beam_allTrack_trackEndDirX, &b_reco_beam_allTrack_trackEndDirX);
   fChain->SetBranchAddress("reco_beam_allTrack_trackEndDirY", &reco_beam_allTrack_trackEndDirY, &b_reco_beam_allTrack_trackEndDirY);
   fChain->SetBranchAddress("reco_beam_allTrack_trackEndDirZ", &reco_beam_allTrack_trackEndDirZ, &b_reco_beam_allTrack_trackEndDirZ);
   fChain->SetBranchAddress("reco_beam_allTrack_resRange", &reco_beam_allTrack_resRange, &b_reco_beam_allTrack_resRange);
   fChain->SetBranchAddress("reco_beam_allTrack_calibrated_dEdX", &reco_beam_allTrack_calibrated_dEdX, &b_reco_beam_allTrack_calibrated_dEdX);
   fChain->SetBranchAddress("reco_beam_allTrack_Chi2_proton", &reco_beam_allTrack_Chi2_proton, &b_reco_beam_allTrack_Chi2_proton);
   fChain->SetBranchAddress("reco_beam_allTrack_Chi2_ndof", &reco_beam_allTrack_Chi2_ndof, &b_reco_beam_allTrack_Chi2_ndof);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_PDG", &reco_daughter_PFP_true_byHits_PDG, &b_reco_daughter_PFP_true_byHits_PDG);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_ID", &reco_daughter_PFP_true_byHits_ID, &b_reco_daughter_PFP_true_byHits_ID);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_origin", &reco_daughter_PFP_true_byHits_origin, &b_reco_daughter_PFP_true_byHits_origin);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_parID", &reco_daughter_PFP_true_byHits_parID, &b_reco_daughter_PFP_true_byHits_parID);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_parPDG", &reco_daughter_PFP_true_byHits_parPDG, &b_reco_daughter_PFP_true_byHits_parPDG);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_process", &reco_daughter_PFP_true_byHits_process, &b_reco_daughter_PFP_true_byHits_process);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_sharedHits", &reco_daughter_PFP_true_byHits_sharedHits, &b_reco_daughter_PFP_true_byHits_sharedHits);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_emHits", &reco_daughter_PFP_true_byHits_emHits, &b_reco_daughter_PFP_true_byHits_emHits);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_len", &reco_daughter_PFP_true_byHits_len, &b_reco_daughter_PFP_true_byHits_len);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startX", &reco_daughter_PFP_true_byHits_startX, &b_reco_daughter_PFP_true_byHits_startX);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startY", &reco_daughter_PFP_true_byHits_startY, &b_reco_daughter_PFP_true_byHits_startY);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startZ", &reco_daughter_PFP_true_byHits_startZ, &b_reco_daughter_PFP_true_byHits_startZ);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_endX", &reco_daughter_PFP_true_byHits_endX, &b_reco_daughter_PFP_true_byHits_endX);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_endY", &reco_daughter_PFP_true_byHits_endY, &b_reco_daughter_PFP_true_byHits_endY);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_endZ", &reco_daughter_PFP_true_byHits_endZ, &b_reco_daughter_PFP_true_byHits_endZ);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startPx", &reco_daughter_PFP_true_byHits_startPx, &b_reco_daughter_PFP_true_byHits_startPx);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startPy", &reco_daughter_PFP_true_byHits_startPy, &b_reco_daughter_PFP_true_byHits_startPy);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startPz", &reco_daughter_PFP_true_byHits_startPz, &b_reco_daughter_PFP_true_byHits_startPz);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startP", &reco_daughter_PFP_true_byHits_startP, &b_reco_daughter_PFP_true_byHits_startP);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_startE", &reco_daughter_PFP_true_byHits_startE, &b_reco_daughter_PFP_true_byHits_startE);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_endProcess", &reco_daughter_PFP_true_byHits_endProcess, &b_reco_daughter_PFP_true_byHits_endProcess);
   fChain->SetBranchAddress("reco_daughter_PFP_true_byHits_purity", &reco_daughter_PFP_true_byHits_purity, &b_reco_daughter_PFP_true_byHits_purity);
   fChain->SetBranchAddress("reco_daughter_allTrack_ID", &reco_daughter_allTrack_ID, &b_reco_daughter_allTrack_ID);   
   fChain->SetBranchAddress("reco_daughter_allTrack_dEdX", &reco_daughter_allTrack_dEdX, &b_reco_daughter_allTrack_dEdX);
   fChain->SetBranchAddress("reco_daughter_allTrack_dQdX", &reco_daughter_allTrack_dQdX, &b_reco_daughter_allTrack_dQdX);
   fChain->SetBranchAddress("reco_daughter_allTrack_resRange", &reco_daughter_allTrack_resRange, &b_reco_daughter_allTrack_resRange);
   fChain->SetBranchAddress("reco_daughter_allTrack_dQdX_SCE", &reco_daughter_allTrack_dQdX_SCE, &b_reco_daughter_allTrack_dQdX_SCE);
   fChain->SetBranchAddress("reco_daughter_allTrack_dEdX_SCE", &reco_daughter_allTrack_dEdX_SCE, &b_reco_daughter_allTrack_dEdX_SCE);
   fChain->SetBranchAddress("reco_daughter_allTrack_resRange_SCE", &reco_daughter_allTrack_resRange_SCE, &b_reco_daughter_allTrack_resRange_SCE);
   fChain->SetBranchAddress("reco_daughter_allTrack_calibrated_dEdX", &reco_daughter_allTrack_calibrated_dEdX, &b_reco_daughter_allTrack_calibrated_dEdX);
   fChain->SetBranchAddress("reco_daughter_allTrack_calibrated_dEdX_SCE", &reco_daughter_allTrack_calibrated_dEdX_SCE, &b_reco_daughter_allTrack_calibrated_dEdX_SCE);
   fChain->SetBranchAddress("reco_daughter_allTrack_Chi2_proton", &reco_daughter_allTrack_Chi2_proton, &b_reco_daughter_allTrack_Chi2_proton);
   fChain->SetBranchAddress("reco_daughter_allTrack_Chi2_ndof", &reco_daughter_allTrack_Chi2_ndof, &b_reco_daughter_allTrack_Chi2_ndof);
   fChain->SetBranchAddress("reco_daughter_allTrack_Theta", &reco_daughter_allTrack_Theta, &b_reco_daughter_allTrack_Theta);
   fChain->SetBranchAddress("reco_daughter_allTrack_Phi", &reco_daughter_allTrack_Phi, &b_reco_daughter_allTrack_Phi);
   fChain->SetBranchAddress("reco_daughter_allTrack_len", &reco_daughter_allTrack_len, &b_reco_daughter_allTrack_len);
   fChain->SetBranchAddress("reco_daughter_allTrack_startX", &reco_daughter_allTrack_startX, &b_reco_daughter_allTrack_startX);
   fChain->SetBranchAddress("reco_daughter_allTrack_startY", &reco_daughter_allTrack_startY, &b_reco_daughter_allTrack_startY);
   fChain->SetBranchAddress("reco_daughter_allTrack_startZ", &reco_daughter_allTrack_startZ, &b_reco_daughter_allTrack_startZ);
   fChain->SetBranchAddress("reco_daughter_allTrack_endX", &reco_daughter_allTrack_endX, &b_reco_daughter_allTrack_endX);
   fChain->SetBranchAddress("reco_daughter_allTrack_endY", &reco_daughter_allTrack_endY, &b_reco_daughter_allTrack_endY);
   fChain->SetBranchAddress("reco_daughter_allTrack_endZ", &reco_daughter_allTrack_endZ, &b_reco_daughter_allTrack_endZ);
   fChain->SetBranchAddress("reco_daughter_allTrack_dR", &reco_daughter_allTrack_dR, &b_reco_daughter_allTrack_dR);
   fChain->SetBranchAddress("reco_daughter_allTrack_to_vertex", &reco_daughter_allTrack_to_vertex, &b_reco_daughter_allTrack_to_vertex);
   fChain->SetBranchAddress("reco_daughter_allShower_ID", &reco_daughter_allShower_ID, &b_reco_daughter_allShower_ID);
   fChain->SetBranchAddress("reco_daughter_allShower_len", &reco_daughter_allShower_len, &b_reco_daughter_allShower_len);
   fChain->SetBranchAddress("reco_daughter_allShower_startX", &reco_daughter_allShower_startX, &b_reco_daughter_allShower_startX);
   fChain->SetBranchAddress("reco_daughter_allShower_startY", &reco_daughter_allShower_startY, &b_reco_daughter_allShower_startY);
   fChain->SetBranchAddress("reco_daughter_allShower_startZ", &reco_daughter_allShower_startZ, &b_reco_daughter_allShower_startZ);
   fChain->SetBranchAddress("reco_daughter_PFP_ID", &reco_daughter_PFP_ID, &b_reco_daughter_PFP_ID);
   fChain->SetBranchAddress("reco_daughter_PFP_nHits", &reco_daughter_PFP_nHits, &b_reco_daughter_PFP_nHits);
   fChain->SetBranchAddress("reco_daughter_PFP_trackScore", &reco_daughter_PFP_trackScore, &b_reco_daughter_PFP_trackScore);
   fChain->SetBranchAddress("reco_daughter_PFP_emScore", &reco_daughter_PFP_emScore, &b_reco_daughter_PFP_emScore);
   fChain->SetBranchAddress("reco_daughter_PFP_michelScore", &reco_daughter_PFP_michelScore, &b_reco_daughter_PFP_michelScore);
   fChain->SetBranchAddress("reco_daughter_PFP_trackScore_collection", &reco_daughter_PFP_trackScore_collection, &b_reco_daughter_PFP_trackScore_collection);
   fChain->SetBranchAddress("reco_daughter_PFP_emScore_collection", &reco_daughter_PFP_emScore_collection, &b_reco_daughter_PFP_emScore_collection);
   fChain->SetBranchAddress("reco_daughter_PFP_michelScore_collection", &reco_daughter_PFP_michelScore_collection, &b_reco_daughter_PFP_michelScore_collection);
   fChain->SetBranchAddress("true_beam_PDG", &true_beam_PDG, &b_true_beam_PDG);
   fChain->SetBranchAddress("true_beam_ID", &true_beam_ID, &b_true_beam_ID);
   fChain->SetBranchAddress("true_beam_endProcess", &true_beam_endProcess, &b_true_beam_endProcess);
   fChain->SetBranchAddress("true_beam_endX", &true_beam_endX, &b_true_beam_endX);
   fChain->SetBranchAddress("true_beam_endY", &true_beam_endY, &b_true_beam_endY);
   fChain->SetBranchAddress("true_beam_endZ", &true_beam_endZ, &b_true_beam_endZ);
   fChain->SetBranchAddress("true_beam_startX", &true_beam_startX, &b_true_beam_startX);
   fChain->SetBranchAddress("true_beam_startY", &true_beam_startY, &b_true_beam_startY);
   fChain->SetBranchAddress("true_beam_startZ", &true_beam_startZ, &b_true_beam_startZ);
   fChain->SetBranchAddress("true_beam_startPx", &true_beam_startPx, &b_true_beam_startPx);
   fChain->SetBranchAddress("true_beam_startPy", &true_beam_startPy, &b_true_beam_startPy);
   fChain->SetBranchAddress("true_beam_startPz", &true_beam_startPz, &b_true_beam_startPz);
   fChain->SetBranchAddress("true_beam_startP", &true_beam_startP, &b_true_beam_startP);
   fChain->SetBranchAddress("true_beam_endPx", &true_beam_endPx, &b_true_beam_endPx);
   fChain->SetBranchAddress("true_beam_endPy", &true_beam_endPy, &b_true_beam_endPy);
   fChain->SetBranchAddress("true_beam_endPz", &true_beam_endPz, &b_true_beam_endPz);
   fChain->SetBranchAddress("true_beam_endP", &true_beam_endP, &b_true_beam_endP);
   fChain->SetBranchAddress("true_beam_startDirX", &true_beam_startDirX, &b_true_beam_startDirX);
   fChain->SetBranchAddress("true_beam_startDirY", &true_beam_startDirY, &b_true_beam_startDirY);
   fChain->SetBranchAddress("true_beam_startDirZ", &true_beam_startDirZ, &b_true_beam_startDirZ);
   fChain->SetBranchAddress("true_beam_nElasticScatters", &true_beam_nElasticScatters, &b_true_beam_nElasticScatters);
   fChain->SetBranchAddress("true_beam_elastic_costheta", &true_beam_elastic_costheta, &b_true_beam_elastic_costheta);
   fChain->SetBranchAddress("true_beam_elastic_X", &true_beam_elastic_X, &b_true_beam_elastic_X);
   fChain->SetBranchAddress("true_beam_elastic_Y", &true_beam_elastic_Y, &b_true_beam_elastic_Y);
   fChain->SetBranchAddress("true_beam_elastic_Z", &true_beam_elastic_Z, &b_true_beam_elastic_Z);
   fChain->SetBranchAddress("true_beam_IDE_totalDep", &true_beam_IDE_totalDep, &b_true_beam_IDE_totalDep);
   fChain->SetBranchAddress("true_beam_IDE_found_in_recoVtx", &true_beam_IDE_found_in_recoVtx, &b_true_beam_IDE_found_in_recoVtx);
   fChain->SetBranchAddress("true_beam_nHits", &true_beam_nHits, &b_true_beam_nHits);
   fChain->SetBranchAddress("true_beam_reco_byHits_PFP_ID", &true_beam_reco_byHits_PFP_ID, &b_true_beam_reco_byHits_PFP_ID);
   fChain->SetBranchAddress("true_beam_reco_byHits_PFP_nHits", &true_beam_reco_byHits_PFP_nHits, &b_true_beam_reco_byHits_PFP_nHits);
   fChain->SetBranchAddress("true_beam_reco_byHits_allTrack_ID", &true_beam_reco_byHits_allTrack_ID, &b_true_beam_reco_byHits_allTrack_ID);
   fChain->SetBranchAddress("true_daughter_nPi0", &true_daughter_nPi0, &b_true_daughter_nPi0);
   fChain->SetBranchAddress("true_daughter_nPiPlus", &true_daughter_nPiPlus, &b_true_daughter_nPiPlus);
   fChain->SetBranchAddress("true_daughter_nProton", &true_daughter_nProton, &b_true_daughter_nProton);
   fChain->SetBranchAddress("true_daughter_nNeutron", &true_daughter_nNeutron, &b_true_daughter_nNeutron);
   fChain->SetBranchAddress("true_daughter_nPiMinus", &true_daughter_nPiMinus, &b_true_daughter_nPiMinus);
   fChain->SetBranchAddress("true_daughter_nNucleus", &true_daughter_nNucleus, &b_true_daughter_nNucleus);
   fChain->SetBranchAddress("reco_beam_vertex_slice", &reco_beam_vertex_slice, &b_reco_beam_vertex_slice);
   fChain->SetBranchAddress("reco_beam_vertex_dRs", &reco_beam_vertex_dRs, &b_reco_beam_vertex_dRs);
   fChain->SetBranchAddress("reco_beam_vertex_hits_slices", &reco_beam_vertex_hits_slices, &b_reco_beam_vertex_hits_slices);
   fChain->SetBranchAddress("true_beam_daughter_PDG", &true_beam_daughter_PDG, &b_true_beam_daughter_PDG);
   fChain->SetBranchAddress("true_beam_daughter_ID", &true_beam_daughter_ID, &b_true_beam_daughter_ID);
   fChain->SetBranchAddress("true_beam_daughter_len", &true_beam_daughter_len, &b_true_beam_daughter_len);
   fChain->SetBranchAddress("true_beam_daughter_startX", &true_beam_daughter_startX, &b_true_beam_daughter_startX);
   fChain->SetBranchAddress("true_beam_daughter_startY", &true_beam_daughter_startY, &b_true_beam_daughter_startY);
   fChain->SetBranchAddress("true_beam_daughter_startZ", &true_beam_daughter_startZ, &b_true_beam_daughter_startZ);
   fChain->SetBranchAddress("true_beam_daughter_startPx", &true_beam_daughter_startPx, &b_true_beam_daughter_startPx);
   fChain->SetBranchAddress("true_beam_daughter_startPy", &true_beam_daughter_startPy, &b_true_beam_daughter_startPy);
   fChain->SetBranchAddress("true_beam_daughter_startPz", &true_beam_daughter_startPz, &b_true_beam_daughter_startPz);
   fChain->SetBranchAddress("true_beam_daughter_startP", &true_beam_daughter_startP, &b_true_beam_daughter_startP);
   fChain->SetBranchAddress("true_beam_daughter_endX", &true_beam_daughter_endX, &b_true_beam_daughter_endX);
   fChain->SetBranchAddress("true_beam_daughter_endY", &true_beam_daughter_endY, &b_true_beam_daughter_endY);
   fChain->SetBranchAddress("true_beam_daughter_endZ", &true_beam_daughter_endZ, &b_true_beam_daughter_endZ);
   fChain->SetBranchAddress("true_beam_daughter_Process", &true_beam_daughter_Process, &b_true_beam_daughter_Process);
   fChain->SetBranchAddress("true_beam_daughter_endProcess", &true_beam_daughter_endProcess, &b_true_beam_daughter_endProcess);
   fChain->SetBranchAddress("true_beam_daughter_nHits", &true_beam_daughter_nHits, &b_true_beam_daughter_nHits);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_PFP_ID", &true_beam_daughter_reco_byHits_PFP_ID, &b_true_beam_daughter_reco_byHits_PFP_ID);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_PFP_nHits", &true_beam_daughter_reco_byHits_PFP_nHits, &b_true_beam_daughter_reco_byHits_PFP_nHits);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_PFP_trackScore", &true_beam_daughter_reco_byHits_PFP_trackScore, &b_true_beam_daughter_reco_byHits_PFP_trackScore);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_ID", &true_beam_daughter_reco_byHits_allTrack_ID, &b_true_beam_daughter_reco_byHits_allTrack_ID);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_startX", &true_beam_daughter_reco_byHits_allTrack_startX, &b_true_beam_daughter_reco_byHits_allTrack_startX);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_startY", &true_beam_daughter_reco_byHits_allTrack_startY, &b_true_beam_daughter_reco_byHits_allTrack_startY);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_startZ", &true_beam_daughter_reco_byHits_allTrack_startZ, &b_true_beam_daughter_reco_byHits_allTrack_startZ);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_endX", &true_beam_daughter_reco_byHits_allTrack_endX, &b_true_beam_daughter_reco_byHits_allTrack_endX);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_endY", &true_beam_daughter_reco_byHits_allTrack_endY, &b_true_beam_daughter_reco_byHits_allTrack_endY);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_endZ", &true_beam_daughter_reco_byHits_allTrack_endZ, &b_true_beam_daughter_reco_byHits_allTrack_endZ);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allTrack_len", &true_beam_daughter_reco_byHits_allTrack_len, &b_true_beam_daughter_reco_byHits_allTrack_len);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allShower_ID", &true_beam_daughter_reco_byHits_allShower_ID, &b_true_beam_daughter_reco_byHits_allShower_ID);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allShower_startX", &true_beam_daughter_reco_byHits_allShower_startX, &b_true_beam_daughter_reco_byHits_allShower_startX);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allShower_startY", &true_beam_daughter_reco_byHits_allShower_startY, &b_true_beam_daughter_reco_byHits_allShower_startY);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allShower_startZ", &true_beam_daughter_reco_byHits_allShower_startZ, &b_true_beam_daughter_reco_byHits_allShower_startZ);
   fChain->SetBranchAddress("true_beam_daughter_reco_byHits_allShower_len", &true_beam_daughter_reco_byHits_allShower_len, &b_true_beam_daughter_reco_byHits_allShower_len);
   fChain->SetBranchAddress("true_beam_Pi0_decay_ID", &true_beam_Pi0_decay_ID, &b_true_beam_Pi0_decay_ID);
   fChain->SetBranchAddress("true_beam_Pi0_decay_parID", &true_beam_Pi0_decay_parID, &b_true_beam_Pi0_decay_parID);
   fChain->SetBranchAddress("true_beam_Pi0_decay_PDG", &true_beam_Pi0_decay_PDG, &b_true_beam_Pi0_decay_PDG);
   fChain->SetBranchAddress("true_beam_Pi0_decay_startP", &true_beam_Pi0_decay_startP, &b_true_beam_Pi0_decay_startP);
   fChain->SetBranchAddress("true_beam_Pi0_decay_len", &true_beam_Pi0_decay_len, &b_true_beam_Pi0_decay_len);
   fChain->SetBranchAddress("true_beam_Pi0_decay_nHits", &true_beam_Pi0_decay_nHits, &b_true_beam_Pi0_decay_nHits);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_PFP_ID", &true_beam_Pi0_decay_reco_byHits_PFP_ID, &b_true_beam_Pi0_decay_reco_byHits_PFP_ID);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_PFP_nHits", &true_beam_Pi0_decay_reco_byHits_PFP_nHits, &b_true_beam_Pi0_decay_reco_byHits_PFP_nHits);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_PFP_trackScore", &true_beam_Pi0_decay_reco_byHits_PFP_trackScore, &b_true_beam_Pi0_decay_reco_byHits_PFP_trackScore);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_ID", &true_beam_Pi0_decay_reco_byHits_allTrack_ID, &b_true_beam_Pi0_decay_reco_byHits_allTrack_ID);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_startX", &true_beam_Pi0_decay_reco_byHits_allTrack_startX, &b_true_beam_Pi0_decay_reco_byHits_allTrack_startX);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_startY", &true_beam_Pi0_decay_reco_byHits_allTrack_startY, &b_true_beam_Pi0_decay_reco_byHits_allTrack_startY);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_startZ", &true_beam_Pi0_decay_reco_byHits_allTrack_startZ, &b_true_beam_Pi0_decay_reco_byHits_allTrack_startZ);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_endX", &true_beam_Pi0_decay_reco_byHits_allTrack_endX, &b_true_beam_Pi0_decay_reco_byHits_allTrack_endX);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_endY", &true_beam_Pi0_decay_reco_byHits_allTrack_endY, &b_true_beam_Pi0_decay_reco_byHits_allTrack_endY);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_endZ", &true_beam_Pi0_decay_reco_byHits_allTrack_endZ, &b_true_beam_Pi0_decay_reco_byHits_allTrack_endZ);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allTrack_len", &true_beam_Pi0_decay_reco_byHits_allTrack_len, &b_true_beam_Pi0_decay_reco_byHits_allTrack_len);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allShower_ID", &true_beam_Pi0_decay_reco_byHits_allShower_ID, &b_true_beam_Pi0_decay_reco_byHits_allShower_ID);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allShower_startX", &true_beam_Pi0_decay_reco_byHits_allShower_startX, &b_true_beam_Pi0_decay_reco_byHits_allShower_startX);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allShower_startY", &true_beam_Pi0_decay_reco_byHits_allShower_startY, &b_true_beam_Pi0_decay_reco_byHits_allShower_startY);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allShower_startZ", &true_beam_Pi0_decay_reco_byHits_allShower_startZ, &b_true_beam_Pi0_decay_reco_byHits_allShower_startZ);
   fChain->SetBranchAddress("true_beam_Pi0_decay_reco_byHits_allShower_len", &true_beam_Pi0_decay_reco_byHits_allShower_len, &b_true_beam_Pi0_decay_reco_byHits_allShower_len);
   fChain->SetBranchAddress("true_beam_grand_daughter_ID", &true_beam_grand_daughter_ID, &b_true_beam_grand_daughter_ID);
   fChain->SetBranchAddress("true_beam_grand_daughter_parID", &true_beam_grand_daughter_parID, &b_true_beam_grand_daughter_parID);
   fChain->SetBranchAddress("true_beam_grand_daughter_PDG", &true_beam_grand_daughter_PDG, &b_true_beam_grand_daughter_PDG);
   fChain->SetBranchAddress("true_beam_grand_daughter_nHits", &true_beam_grand_daughter_nHits, &b_true_beam_grand_daughter_nHits);
   fChain->SetBranchAddress("true_beam_grand_daughter_Process", &true_beam_grand_daughter_Process, &b_true_beam_grand_daughter_Process);
   fChain->SetBranchAddress("true_beam_grand_daughter_endProcess", &true_beam_grand_daughter_endProcess, &b_true_beam_grand_daughter_endProcess);
   fChain->SetBranchAddress("reco_beam_true_byE_endProcess", &reco_beam_true_byE_endProcess, &b_reco_beam_true_byE_endProcess);
   fChain->SetBranchAddress("reco_beam_true_byE_process", &reco_beam_true_byE_process, &b_reco_beam_true_byE_process);
   fChain->SetBranchAddress("reco_beam_true_byE_origin", &reco_beam_true_byE_origin, &b_reco_beam_true_byE_origin);
   fChain->SetBranchAddress("reco_beam_true_byE_PDG", &reco_beam_true_byE_PDG, &b_reco_beam_true_byE_PDG);
   fChain->SetBranchAddress("reco_beam_true_byE_ID", &reco_beam_true_byE_ID, &b_reco_beam_true_byE_ID);
   fChain->SetBranchAddress("reco_beam_true_byHits_endProcess", &reco_beam_true_byHits_endProcess, &b_reco_beam_true_byHits_endProcess);
   fChain->SetBranchAddress("reco_beam_true_byHits_process", &reco_beam_true_byHits_process, &b_reco_beam_true_byHits_process);
   fChain->SetBranchAddress("reco_beam_true_byHits_origin", &reco_beam_true_byHits_origin, &b_reco_beam_true_byHits_origin);
   fChain->SetBranchAddress("reco_beam_true_byHits_PDG", &reco_beam_true_byHits_PDG, &b_reco_beam_true_byHits_PDG);
   fChain->SetBranchAddress("reco_beam_true_byHits_ID", &reco_beam_true_byHits_ID, &b_reco_beam_true_byHits_ID);
   fChain->SetBranchAddress("reco_beam_true_byE_matched", &reco_beam_true_byE_matched, &b_reco_beam_true_byE_matched);
   fChain->SetBranchAddress("reco_beam_true_byHits_matched", &reco_beam_true_byHits_matched, &b_reco_beam_true_byHits_matched);
   fChain->SetBranchAddress("reco_beam_true_byHits_purity", &reco_beam_true_byHits_purity, &b_reco_beam_true_byHits_purity);
   fChain->SetBranchAddress("true_beam_processes", &true_beam_processes, &b_true_beam_processes);
   fChain->SetBranchAddress("data_BI_P", &data_BI_P, &b_data_BI_P);
   fChain->SetBranchAddress("data_BI_X", &data_BI_X, &b_data_BI_X);
   fChain->SetBranchAddress("data_BI_Y", &data_BI_Y, &b_data_BI_Y);
   fChain->SetBranchAddress("data_BI_Z", &data_BI_Z, &b_data_BI_Z);
   fChain->SetBranchAddress("data_BI_dirX", &data_BI_dirX, &b_data_BI_dirX);
   fChain->SetBranchAddress("data_BI_dirY", &data_BI_dirY, &b_data_BI_dirY);
   fChain->SetBranchAddress("data_BI_dirZ", &data_BI_dirZ, &b_data_BI_dirZ);
   fChain->SetBranchAddress("data_BI_nFibersP1", &data_BI_nFibersP1, &b_data_BI_nFibersP1);
   fChain->SetBranchAddress("data_BI_nFibersP2", &data_BI_nFibersP2, &b_data_BI_nFibersP2);
   fChain->SetBranchAddress("data_BI_nFibersP3", &data_BI_nFibersP3, &b_data_BI_nFibersP3);
   fChain->SetBranchAddress("data_BI_PDG_candidates", &data_BI_PDG_candidates, &b_data_BI_PDG_candidates);
   fChain->SetBranchAddress("data_BI_nTracks", &data_BI_nTracks, &b_data_BI_nTracks);
   fChain->SetBranchAddress("data_BI_nMomenta", &data_BI_nMomenta, &b_data_BI_nMomenta);
   fChain->SetBranchAddress("quality_reco_view_0_hits_in_TPC5", &quality_reco_view_0_hits_in_TPC5, &b_quality_reco_view_0_hits_in_TPC5);
   fChain->SetBranchAddress("quality_reco_view_1_hits_in_TPC5", &quality_reco_view_1_hits_in_TPC5, &b_quality_reco_view_1_hits_in_TPC5);
   fChain->SetBranchAddress("quality_reco_view_2_hits_in_TPC5", &quality_reco_view_2_hits_in_TPC5, &b_quality_reco_view_2_hits_in_TPC5);
   fChain->SetBranchAddress("quality_reco_max_lateral", &quality_reco_max_lateral, &b_quality_reco_max_lateral);
   fChain->SetBranchAddress("quality_reco_max_segment", &quality_reco_max_segment, &b_quality_reco_max_segment);
   fChain->SetBranchAddress("quality_reco_view_0_max_segment", &quality_reco_view_0_max_segment, &b_quality_reco_view_0_max_segment);
   fChain->SetBranchAddress("quality_reco_view_1_max_segment", &quality_reco_view_1_max_segment, &b_quality_reco_view_1_max_segment);
   fChain->SetBranchAddress("quality_reco_view_2_max_segment", &quality_reco_view_2_max_segment, &b_quality_reco_view_2_max_segment);
   fChain->SetBranchAddress("quality_reco_view_0_wire_backtrack", &quality_reco_view_0_wire_backtrack, &b_quality_reco_view_0_wire_backtrack);
   fChain->SetBranchAddress("quality_reco_view_1_wire_backtrack", &quality_reco_view_1_wire_backtrack, &b_quality_reco_view_1_wire_backtrack);
   fChain->SetBranchAddress("quality_reco_view_2_wire_backtrack", &quality_reco_view_2_wire_backtrack, &b_quality_reco_view_2_wire_backtrack);
   fChain->SetBranchAddress("quality_reco_view_0_wire", &quality_reco_view_0_wire, &b_quality_reco_view_0_wire);
   fChain->SetBranchAddress("quality_reco_view_1_wire", &quality_reco_view_1_wire, &b_quality_reco_view_1_wire);
   fChain->SetBranchAddress("quality_reco_view_2_wire", &quality_reco_view_2_wire, &b_quality_reco_view_2_wire);
   fChain->SetBranchAddress("quality_reco_view_2_z", &quality_reco_view_2_z, &b_quality_reco_view_2_z);
   fChain->SetBranchAddress("quality_reco_view_0_tick", &quality_reco_view_0_tick, &b_quality_reco_view_0_tick);
   fChain->SetBranchAddress("quality_reco_view_1_tick", &quality_reco_view_1_tick, &b_quality_reco_view_1_tick);
   fChain->SetBranchAddress("quality_reco_view_2_tick", &quality_reco_view_2_tick, &b_quality_reco_view_2_tick);
   fChain->SetBranchAddress("reco_beam_Chi2_proton", &reco_beam_Chi2_proton, &b_reco_beam_Chi2_proton);
   fChain->SetBranchAddress("reco_beam_Chi2_ndof", &reco_beam_Chi2_ndof, &b_reco_beam_Chi2_ndof);
   fChain->SetBranchAddress("reco_beam_cosmic_candidate_lower_hits", &reco_beam_cosmic_candidate_lower_hits, &b_reco_beam_cosmic_candidate_lower_hits);
   fChain->SetBranchAddress("reco_beam_cosmic_candidate_upper_hits", &reco_beam_cosmic_candidate_upper_hits, &b_reco_beam_cosmic_candidate_upper_hits);
   fChain->SetBranchAddress("reco_beam_cosmic_candidate_ID", &reco_beam_cosmic_candidate_ID, &b_reco_beam_cosmic_candidate_ID);
   fChain->SetBranchAddress("beam_has_cosmic_IDE", &beam_has_cosmic_IDE, &b_beam_has_cosmic_IDE);
   fChain->SetBranchAddress("cosmic_has_beam_IDE", &cosmic_has_beam_IDE, &b_cosmic_has_beam_IDE);
   fChain->SetBranchAddress("n_cosmics_with_beam_IDE", &n_cosmics_with_beam_IDE, &b_n_cosmics_with_beam_IDE);
   fChain->SetBranchAddress("reco_beam_true_byE_endPx", &reco_beam_true_byE_endPx, &b_reco_beam_true_byE_endPx);
   fChain->SetBranchAddress("reco_beam_true_byE_endPy", &reco_beam_true_byE_endPy, &b_reco_beam_true_byE_endPy);
   fChain->SetBranchAddress("reco_beam_true_byE_endPz", &reco_beam_true_byE_endPz, &b_reco_beam_true_byE_endPz);
   fChain->SetBranchAddress("reco_beam_true_byE_endE", &reco_beam_true_byE_endE, &b_reco_beam_true_byE_endE);
   fChain->SetBranchAddress("reco_beam_true_byE_endP", &reco_beam_true_byE_endP, &b_reco_beam_true_byE_endP);
   fChain->SetBranchAddress("reco_beam_true_byE_startPx", &reco_beam_true_byE_startPx, &b_reco_beam_true_byE_startPx);
   fChain->SetBranchAddress("reco_beam_true_byE_startPy", &reco_beam_true_byE_startPy, &b_reco_beam_true_byE_startPy);
   fChain->SetBranchAddress("reco_beam_true_byE_startPz", &reco_beam_true_byE_startPz, &b_reco_beam_true_byE_startPz);
   fChain->SetBranchAddress("reco_beam_true_byE_startE", &reco_beam_true_byE_startE, &b_reco_beam_true_byE_startE);
   fChain->SetBranchAddress("reco_beam_true_byE_startP", &reco_beam_true_byE_startP, &b_reco_beam_true_byE_startP);
   fChain->SetBranchAddress("reco_beam_true_byHits_endPx", &reco_beam_true_byHits_endPx, &b_reco_beam_true_byHits_endPx);
   fChain->SetBranchAddress("reco_beam_true_byHits_endPy", &reco_beam_true_byHits_endPy, &b_reco_beam_true_byHits_endPy);
   fChain->SetBranchAddress("reco_beam_true_byHits_endPz", &reco_beam_true_byHits_endPz, &b_reco_beam_true_byHits_endPz);
   fChain->SetBranchAddress("reco_beam_true_byHits_endE", &reco_beam_true_byHits_endE, &b_reco_beam_true_byHits_endE);
   fChain->SetBranchAddress("reco_beam_true_byHits_endP", &reco_beam_true_byHits_endP, &b_reco_beam_true_byHits_endP);
   fChain->SetBranchAddress("reco_beam_true_byHits_startPx", &reco_beam_true_byHits_startPx, &b_reco_beam_true_byHits_startPx);
   fChain->SetBranchAddress("reco_beam_true_byHits_startPy", &reco_beam_true_byHits_startPy, &b_reco_beam_true_byHits_startPy);
   fChain->SetBranchAddress("reco_beam_true_byHits_startPz", &reco_beam_true_byHits_startPz, &b_reco_beam_true_byHits_startPz);
   fChain->SetBranchAddress("reco_beam_true_byHits_startE", &reco_beam_true_byHits_startE, &b_reco_beam_true_byHits_startE);
   fChain->SetBranchAddress("reco_beam_true_byHits_startP", &reco_beam_true_byHits_startP, &b_reco_beam_true_byHits_startP);
   fChain->SetBranchAddress("reco_beam_incidentEnergies", &reco_beam_incidentEnergies, &b_reco_beam_incidentEnergies);
   fChain->SetBranchAddress("reco_beam_interactingEnergy", &reco_beam_interactingEnergy, &b_reco_beam_interactingEnergy);
   fChain->SetBranchAddress("true_beam_incidentEnergies", &true_beam_incidentEnergies, &b_true_beam_incidentEnergies);
   fChain->SetBranchAddress("true_beam_interactingEnergy", &true_beam_interactingEnergy, &b_true_beam_interactingEnergy);
   fChain->SetBranchAddress("reco_beam_spacePts_X", &reco_beam_spacePts_X, &b_reco_beam_spacePts_X);
   fChain->SetBranchAddress("reco_beam_spacePts_Y", &reco_beam_spacePts_Y, &b_reco_beam_spacePts_Y);
   fChain->SetBranchAddress("reco_beam_spacePts_Z", &reco_beam_spacePts_Z, &b_reco_beam_spacePts_Z);
   fChain->SetBranchAddress("reco_daughter_spacePts_X", &reco_daughter_spacePts_X, &b_reco_daughter_spacePts_X);
   fChain->SetBranchAddress("reco_daughter_spacePts_Y", &reco_daughter_spacePts_Y, &b_reco_daughter_spacePts_Y);
   fChain->SetBranchAddress("reco_daughter_spacePts_Z", &reco_daughter_spacePts_Z, &b_reco_daughter_spacePts_Z);
   fChain->SetBranchAddress("reco_daughter_shower_spacePts_X", &reco_daughter_shower_spacePts_X, &b_reco_daughter_shower_spacePts_X);
   fChain->SetBranchAddress("reco_daughter_shower_spacePts_Y", &reco_daughter_shower_spacePts_Y, &b_reco_daughter_shower_spacePts_Y);
   fChain->SetBranchAddress("reco_daughter_shower_spacePts_Z", &reco_daughter_shower_spacePts_Z, &b_reco_daughter_shower_spacePts_Z);


   //   fChain->SetBranchStatus("*", 0);


  return true;
}

//********************************************************************
pionTreeConverter::~pionTreeConverter(){
//********************************************************************

  if (!fChain) return;

  if (eventsTree         ) delete   eventsTree          ->GetCurrentFile();
}

//****************************************************************************
bool pionTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "pionTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

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
  //  eventsTree->GetEntry(eventsTree->GetEntries() - 1);
  eventsTree->GetEntry(1);

  
  // Make sure the current file has not the same run and subrun number as the previous
  if (_previousRunID==run &&  _previousSubrunID==subrun && _previousRefEventID>= event){
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "pionTreeConverter::AddFileToTChain(). Current file has the same run and subrun as the previous" << std::endl;
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
  _isMC = MC; 
  if (!header().SetIsMC(_isMC)) return false;

  _softwareVersion = "v0r0";

  // Sets the software version for this file
  return header().SetSoftwareVersion(_softwareVersion);
}


//*****************************************************************************
Int_t pionTreeConverter::ReadEntries(Long64_t& entry) {
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
Int_t pionTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
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
void pionTreeConverter::IncrementPOTBySpill() {
//********************************************************************
  
//  bool bySpillInMC = false;
  
  //  if (!_isMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
  //    anaUtils::IncrementPOTBySpill(*_spill,header());
}

//*****************************************************************************
void pionTreeConverter::FillInfo(AnaSpill* spill){
//*****************************************************************************
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *static_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = run;
  info.SubRun = subrun;
  info.Event  = event;
  info.IsMC   = MC;
  //  info.EventTime = evttime;

  spill->DataQuality = MakeDataQuality();
  spill->Beam = MakeBeam();

  spill->GeomID = (UInt_t)ND::hgman().GetCurrentGeomID();
  
  // data quality info
  FillDQInfo(static_cast<AnaDataQuality*>(spill->DataQuality));

  // Trigger info
  FillTriggerInfo(&(spill->Trigger));

  // True vertex information
  FillTrueInfo(spill);

  // beam related information (must be after true info)
  FillBeamInfo(spill->TrueParticles, static_cast<AnaBeam*>(spill->Beam));
  
  AnaBunch* bunch = MakeBunch();
  spill->Bunches.push_back(bunch);

  // All information about each bunch
  FillBunchInfo(spill->TrueParticles, bunch);

}

//*****************************************************************************
void pionTreeConverter::FillDQInfo(AnaDataQuality* dq){
//*****************************************************************************

    dq->GoodDaq   = true;
}

//*****************************************************************************
void pionTreeConverter::FillBeamInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBeam* beam){
//*****************************************************************************

    beam->GoodSpill = true;

    //Fill beam info
    beam->nMomenta = data_BI_nMomenta;
    beam->nTracks  = data_BI_nTracks;
    beam->nFibers[0] = data_BI_nFibersP1;
    beam->nFibers[1] = data_BI_nFibersP2;
    beam->nFibers[2] = data_BI_nFibersP3;
    for(int i = 0; i < (int)data_BI_PDG_candidates->size(); i++)
      beam->PDGs.push_back(data_BI_PDG_candidates->at(i));
    
    // Create the BeamParticle object
    beam->BeamParticle = MakeParticle();
        
    //Fill beam particle info
    beam->BeamParticle->PositionEnd[0] = data_BI_X;
    beam->BeamParticle->PositionEnd[1] = data_BI_Y;
    beam->BeamParticle->PositionEnd[2] = data_BI_Z;
    beam->BeamParticle->DirectionEnd[0] = data_BI_dirX;
    beam->BeamParticle->DirectionEnd[1] = data_BI_dirY;
    beam->BeamParticle->DirectionEnd[2] = data_BI_dirZ;

    beam->BeamParticle->Momentum = data_BI_P;

    // ------------- True info for MC ----------------
    if (_isMC){
      beam->BeamParticle->TrueObject = FindTrueParticle(true_beam_ID,trueParticles);
    }

}

//*****************************************************************************
void pionTreeConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  (void)trigger;
  
}

//*****************************************************************************
void pionTreeConverter::FillTrueInfo(AnaSpill* spill){
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
  /*
  int nParts = std::min((int)NMAXTRUEPARTICLES, geant_list_size);
  for (int i=0;i<nParts;i++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueParticleInfo(spill->TrueVertices, spill->TrueParticles, i, truePart);
    spill->TrueParticles.push_back(truePart);    
  }
  */


  // Add the true beam particle
  AnaTrueParticle* beamTruePart = MakeTrueParticle();
  FillTrueBeamTrueParticleInfo(beamTruePart);
  spill->TrueParticles.push_back(beamTruePart);    

  // Add the true daughters of the true beam particle
  int nParts = std::min((int)NMAXTRUEPARTICLES,   (int)true_beam_daughter_ID->size());
  for (int ipart=0;ipart<nParts;ipart++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueBeamDaughterTrueParticleInfo(ipart, truePart, beamTruePart);
    spill->TrueParticles.push_back(truePart);    
  }

  // Add the true granddaughters of the true beam particle
  nParts = std::min((int)NMAXTRUEPARTICLES,   (int)true_beam_grand_daughter_ID->size());
  for (int ipart=0;ipart<nParts;ipart++){
    AnaTrueParticle* truePart = MakeTrueParticle();
    FillTrueBeamGrandDaughterTrueParticleInfo(ipart, truePart, beamTruePart);
    spill->TrueParticles.push_back(truePart);    
  }
    
}

//*****************************************************************************
void pionTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
  bunch->Particles.clear();
  bunch->Vertices.clear();

  // The beam particle
  AnaParticle* part = MakeParticle();
  FillBeamParticleInfo(trueParticles, part);  
  bunch->Particles.push_back(part);

  // IMPORTANT !!!!! All daughters are reconstructed as both tracks and showers and added to the particle vector.
  //                So the actual number of particles is half of the size of the vector

  // The daughter tracks
  for (UInt_t i=0;i<reco_daughter_allTrack_ID->size();i++){
    AnaParticle* dautrk = MakeParticle();
    FillDaughterParticleTrackInfo(trueParticles, i, dautrk);
    bunch->Particles.push_back(dautrk);
    part->Daughters.push_back(dautrk);
  }


  // The daughter showers
  for (UInt_t i=0;i<reco_daughter_allShower_ID->size();i++){
    AnaParticle* dausho = MakeParticle();
    FillDaughterParticleShowerInfo(trueParticles, i, dausho);
    bunch->Particles.push_back(dausho);
    part->Daughters.push_back(dausho);
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
void pionTreeConverter::FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  (void)ivertex;
  (void)trueVertex;

  // TODO
  /*
  for (Int_t i=0;i<geant_list_size;i++){
    if (process_primary[i]){
      trueVertex->Position[0] = StartPointx_drifted[i];
      trueVertex->Position[1] = StartPointy_drifted[i];
      trueVertex->Position[2] = StartPointz_drifted[i];
      trueVertex->LeptonPDG = pdg[i];
      trueVertex->LeptonMom = StartP_drifted[i];

      return;
    }
  }
  */
}


//*****************************************************************************
void pionTreeConverter::FillBeamParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaParticle* part){
//*****************************************************************************



  // Not yet implemented
/*
   Int_t           reco_beam_type;
   Double_t        reco_beam_vtxX;
   Double_t        reco_beam_vtxY;
   Double_t        reco_beam_vtxZ;
   std::vector<double>  *reco_beam_TrkPitch;
   std::vector<double>  *reco_beam_calo_wire;
   std::vector<double>  *reco_beam_calo_tick;

   Int_t           reco_beam_nTrackDaughters;
   Int_t           reco_beam_nShowerDaughters;
   Bool_t          reco_beam_flipped;
   Bool_t          reco_beam_passes_beam_cuts;

   Int_t           reco_beam_allTrack_ID;
   Bool_t          reco_beam_allTrack_beam_cuts;
   Bool_t          reco_beam_allTrack_flipped;
   Double_t        reco_beam_allTrack_len;
   Double_t        reco_beam_allTrack_startX;
   Double_t        reco_beam_allTrack_startY;
   Double_t        reco_beam_allTrack_startZ;
   Double_t        reco_beam_allTrack_endX;
   Double_t        reco_beam_allTrack_endY;
   Double_t        reco_beam_allTrack_endZ;
   Double_t        reco_beam_allTrack_trackDirX;
   Double_t        reco_beam_allTrack_trackDirY;
   Double_t        reco_beam_allTrack_trackDirZ;
   Double_t        reco_beam_allTrack_trackEndDirX;
   Double_t        reco_beam_allTrack_trackEndDirY;
   Double_t        reco_beam_allTrack_trackEndDirZ;
   std::vector<double>  *reco_beam_allTrack_resRange;
   std::vector<double>  *reco_beam_allTrack_calibrated_dEdX;
   Double_t        reco_beam_allTrack_Chi2_proton;
   Int_t           reco_beam_allTrack_Chi2_ndof;


      Int_t           reco_beam_vertex_slice;
   std::vector<std::vector<double> > *reco_beam_vertex_dRs;
   std::vector<int>     *reco_beam_vertex_hits_slices;

   Double_t        reco_beam_Chi2_proton;
   Int_t           reco_beam_Chi2_ndof;
   std::vector<double>  *reco_beam_cosmic_candidate_lower_hits;
   std::vector<double>  *reco_beam_cosmic_candidate_upper_hits;
   std::vector<int>     *reco_beam_cosmic_candidate_ID;


   std::vector<double>  *reco_beam_incidentEnergies;
   Double_t        reco_beam_interactingEnergy;
   std::vector<double>  *true_beam_incidentEnergies;
   Double_t        true_beam_interactingEnergy;

*/   
  
  part->UniqueID  = reco_beam_trackID;

  // This is the beam particle (TODO)
  part->Charge = -8888;


  part->ReconPDG[0] = reco_beam_type;   // TODO

      
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);
  
  part->PositionEnd[0]  = reco_beam_endX;
  part->PositionEnd[1]  = reco_beam_endY;
  part->PositionEnd[2]  = reco_beam_endZ;

  part->PositionStart[0]  = reco_beam_startX;
  part->PositionStart[1]  = reco_beam_startY;
  part->PositionStart[2]  = reco_beam_startZ;

  part->DirectionEnd[0] = reco_beam_trackDirX;
  part->DirectionEnd[1] = reco_beam_trackDirY;
  part->DirectionEnd[2] = reco_beam_trackDirZ;

  part->DirectionStart[0] = reco_beam_trackDirX; 
  part->DirectionStart[1] = reco_beam_trackDirY;
  part->DirectionStart[2] = reco_beam_trackDirZ;

  part->Length = reco_beam_len;

  // Not available in input tree
  //  part->Momentum = ;

  // Not available in input tree
  //  part->RangeMomentum[0] = reco_daughter_allTrack_momByRange_proton;
  //  part->RangeMomentum[1] = reco_daughter_allTrack_momByRange_muon;

  part->AveragedEdx=0;
  part->AveragedQdx=0;
  Int_t ncontrib=0;
  for (UInt_t i=0;i<1;i++){   // only the last slice 
    for (UInt_t j=0;j<reco_beam_dQdX->size();j++){
      part->AveragedEdx += (*reco_beam_dEdX)[j];
      part->AveragedQdx += (*reco_beam_dQdX)[j];
      part->dEdx[i][j] = (*reco_beam_dEdX)[j];
      part->dQdx[i][j] = (*reco_beam_dQdX)[j];

      if (j < reco_beam_calibrated_dEdX->size())
        part->dEdx_corr[i][j] = (*reco_beam_calibrated_dEdX)[j];

      part->ResidualRange[i][j]= (*reco_beam_resRange)[j];
      ncontrib++;
    }
  }

  if (ncontrib!=0){
    part->AveragedEdx /= 1.*ncontrib;
    part->AveragedQdx /= 1.*ncontrib;
  }

  // TODO: Associating space points to hits in a plane for the moment
  UInt_t nHits = std::min((int)NMAXHITSPERPLANE,   (int)reco_beam_spacePts_X->size());
  for (UInt_t j=0;j<nHits;j++){
    part->HitX[0][j] = (*reco_beam_spacePts_X)[j];
    part->HitY[0][j] = (*reco_beam_spacePts_Y)[j];
    part->HitZ[0][j] = (*reco_beam_spacePts_Z)[j];
  }
  
  // --------- reco_beam_PFP ------------------------

  part->CNNscore[0] = reco_beam_PFP_trackScore;
  part->CNNscore[1] = reco_beam_PFP_emScore;
  part->CNNscore[2] = reco_beam_PFP_michelScore;

  /*
  part->NHits = 0;
  for (Int_t i=0;i<3;i++){
    part->NHitsPerPlane[i] = ntrkhits_pmtrajfit[itrk][i];
    part->NHits += part->NHitsPerPlane[i];
  }
  */
  part->NHits = reco_beam_PFP_nHits;


  // Not yet implemented
  /*
    Int_t           reco_beam_PFP_ID;
    Double_t        reco_beam_PFP_trackScore_collection;
    Double_t        reco_beam_PFP_emScore_collection;
    Double_t        reco_beam_PFP_michelScore_collection;
  */


  //------ Truth association ------- //TODO
  
  if (_isMC){

    // Search for the true-reco association within the vector of TrueParticles
    if (byHits)
      part->TrueObject = FindTrueParticle(reco_beam_true_byHits_ID,trueParticles);
    else
      part->TrueObject = FindTrueParticle(reco_beam_true_byE_ID,   trueParticles);

    
    // If not found create a new TrueParticle, fill it, and add it to the vector of TrueParticles
    if (!part->TrueObject){
      AnaTrueParticle* truePart = MakeTrueParticle();
      FillBeamTrueParticleInfo(truePart);
      trueParticles.push_back(truePart);    

      part->TrueObject = truePart;

      part->TruePur = reco_beam_true_byHits_purity;

      // not available
      part->TrueEff=0;
    }
  }
  
}

//*****************************************************************************
void pionTreeConverter::FillDaughterParticleTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part){
//*****************************************************************************

  /* There are two different vectors here: reco_daughter_allTrack and reco_daughter_PFP. 
     reco_daughter_PFP is filled first and if there is a track associated to the PFP,  reco_daughter_allTrack is also filled


     for( size_t daughterID : particle->Daughters() ){
      const recob::PFParticle * daughterPFP = &(pfpVec->at( daughterID ));
      reco_daughter_PFP_ID.push_back( daughterID );

      ....

      try{
        const recob::Track* pandora2Track = pfpUtil.GetPFParticleTrack( *daughterPFP, evt, fPFParticleTag, "pandora2Track" );
        std::cout << "pandora2 track: " << pandora2Track << std::endl;

        if( pandora2Track ){
          reco_daughter_allTrack_ID.push_back( pandora2Track->ID() );

          ...
  */

  part->UniqueID  = (*reco_daughter_allTrack_ID)[itrk];
  //  part->UniqueID  = (*reco_daughter_PFP_ID)[itrk];
  
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);

  part->PositionEnd[0]  = (*reco_daughter_allTrack_endX)[itrk];
  part->PositionEnd[1]  = (*reco_daughter_allTrack_endY)[itrk];
  part->PositionEnd[2]  = (*reco_daughter_allTrack_endZ)[itrk];

  part->PositionStart[0]  = (*reco_daughter_allTrack_startX)[itrk];
  part->PositionStart[1]  = (*reco_daughter_allTrack_startY)[itrk];
  part->PositionStart[2]  = (*reco_daughter_allTrack_startZ)[itrk];

  /* Not available in input tree
  part->DirectionEnd[0] = (*reco_daughter_allTrack_trackDirX)[itrk];
  part->DirectionEnd[1] = (*reco_daughter_allTrack_trackDirY)[itrk];
  part->DirectionEnd[2] = (*reco_daughter_allTrack_trackDirZ)[itrk];

  part->DirectionStart[0] = (*reco_daughter_allTrack_trackDirX)[itrk]; 
  part->DirectionStart[1] = (*reco_daughter_allTrack_trackDirY)[itrk];
  part->DirectionStart[2] = (*reco_daughter_allTrack_trackDirZ)[itrk];
  */

  part->Length = (*reco_daughter_allTrack_len)[itrk];

  //  Not available in input tree
  //  part->Momentum = trkmomrange_pmtrajfit[itrk];

  //  Not available in input tree
  //  part->RangeMomentum[0] = (*reco_daughter_allTrack_momByRange_proton)[itrk];
  //  part->RangeMomentum[1] = (*reco_daughter_allTrack_momByRange_muon)[itrk];

  part->AveragedEdx=0;
  part->AveragedQdx=0;
  Int_t ncontrib=0;
  for (UInt_t i=0;i<1;i++){
    UInt_t nHits = std::min((int)NMAXHITSPERPLANE,   (int)(*reco_daughter_allTrack_dQdX)[itrk].size());
    for (UInt_t j=0;j<nHits;j++){
      part->AveragedEdx += (*reco_daughter_allTrack_dEdX)[itrk][j];
      part->AveragedQdx += (*reco_daughter_allTrack_dQdX)[itrk][j];
      part->dEdx[i][j] = (*reco_daughter_allTrack_dEdX)[itrk][j];
      part->dQdx[i][j] = (*reco_daughter_allTrack_dQdX)[itrk][j];

      if (j < (*reco_daughter_allTrack_calibrated_dEdX)[itrk].size())
        part->dEdx_corr[i][j] = (*reco_daughter_allTrack_calibrated_dEdX)[itrk][j];

      part->ResidualRange[i][j]= (*reco_daughter_allTrack_resRange)[itrk][j];
      ncontrib++;
    }
  }

  if (ncontrib!=0){
    part->AveragedEdx /= 1.*ncontrib;
    part->AveragedQdx /= 1.*ncontrib;
  }
  

  // TODO: Associating space points to hits in a plane for the moment
  UInt_t nHits = std::min((int)NMAXHITSPERPLANE,   (int)reco_daughter_spacePts_X->size());
  for (UInt_t j=0;j<nHits;j++){
    part->HitX[0][j] = (*reco_daughter_spacePts_X)[itrk][j];
    part->HitY[0][j] = (*reco_daughter_spacePts_Y)[itrk][j];
    part->HitZ[0][j] = (*reco_daughter_spacePts_Z)[itrk][j];
  }
   
  part->Chi2Proton = (*reco_daughter_allTrack_Chi2_proton)[itrk];
  part->Chi2ndf    = (*reco_daughter_allTrack_Chi2_ndof)[itrk];

  // -------- Not yet implemented --------
  /*
    std::vector<std::vector<double> > *reco_daughter_allTrack_dQdX_SCE;
    std::vector<std::vector<double> > *reco_daughter_allTrack_dEdX_SCE;
    std::vector<std::vector<double> > *reco_daughter_allTrack_resRange_SCE;
    std::vector<std::vector<double> > *reco_daughter_allTrack_calibrated_dEdX_SCE;
    std::vector<double>  *reco_daughter_allTrack_Theta;
    std::vector<double>  *reco_daughter_allTrack_Phi;
    std::vector<double>  *reco_daughter_allTrack_dR;
    std::vector<double>  *reco_daughter_allTrack_to_vertex;
  */
  
  // --------- reco_daughter_PFP ------------------------
  part->CNNscore[0] = (*reco_daughter_PFP_trackScore)[itrk];
  part->CNNscore[1] = (*reco_daughter_PFP_emScore)[itrk];
  part->CNNscore[2] = (*reco_daughter_PFP_michelScore)[itrk];

  /*
  part->NHits = 0;
  for (Int_t i=0;i<3;i++){
    part->NHitsPerPlane[i] = ntrkhits_pmtrajfit[itrk][i];
    part->NHits += part->NHitsPerPlane[i];
  }
  */
  part->NHits = (*reco_daughter_PFP_nHits)[itrk];

  
  // -------- Not yet implemented --------
  /*
    std::vector<int>     *reco_daughter_PFP_ID;
    std::vector<int>     *reco_daughter_PFP_nHits;
    std::vector<double>  *reco_daughter_PFP_trackScore_collection;
    std::vector<double>  *reco_daughter_PFP_emScore_collection;
    std::vector<double>  *reco_daughter_PFP_michelScore_collection;
  */
  

  //  FillDaughterTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, std::vector<AnaTrueParticleB*>& trueParticles, Int_t ipart, AnaTrueParticle* truePart){
  

  //------ Truth association ------- //TODO

  if (_isMC){
    // Search for the true-reco association within the vector of TrueParticles
    part->TrueObject = FindTrueParticle((*reco_daughter_PFP_true_byHits_ID)[itrk],trueParticles);
    
    // If not found create a new TrueParticle, fill it, and add it to the vector of TrueParticles
    if (!part->TrueObject){
      AnaTrueParticle* truePart = MakeTrueParticle();
      FillDaughterTrueParticleInfo(itrk, truePart);
      trueParticles.push_back(truePart);    
      
      part->TrueObject = truePart;

      part->TruePur = (*reco_daughter_PFP_true_byHits_purity)[itrk];

      // Not available
      part->TrueEff=0;
    }
  }

}

//*****************************************************************************
void pionTreeConverter::FillDaughterParticleShowerInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part){
//*****************************************************************************

  part->UniqueID  = (*reco_daughter_allShower_ID)[itrk];

  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);


  part->PositionStart[0]  = (*reco_daughter_allShower_startX)[itrk];
  part->PositionStart[1]  = (*reco_daughter_allShower_startY)[itrk];
  part->PositionStart[2]  = (*reco_daughter_allShower_startZ)[itrk];

  part->Length = (*reco_daughter_allShower_len)[itrk];


  // TODO: Associating space points to hits in a plane for the moment
  UInt_t nHits = std::min((int)NMAXHITSPERPLANE,   (int)reco_daughter_shower_spacePts_X->size());
  for (UInt_t j=0;j<nHits;j++){
    part->HitX[0][j] = (*reco_daughter_shower_spacePts_X)[itrk][j];
    part->HitY[0][j] = (*reco_daughter_shower_spacePts_Y)[itrk][j];
    part->HitZ[0][j] = (*reco_daughter_shower_spacePts_Z)[itrk][j];
  }

  //------ Truth association ------- //TODO
  
  if (_isMC){

    // Search for the true-reco association within the vector of TrueParticles
    part->TrueObject = FindTrueParticle((*reco_daughter_PFP_true_byHits_ID)[itrk],trueParticles);
    
    // If not found create a new TrueParticle, fill it, and add it to the vector of TrueParticles
    // NEVER ENTERS HERE !!!!!
    if (!part->TrueObject){
      AnaTrueParticle* truePart = MakeTrueParticle();
      FillDaughterTrueParticleInfo(itrk, truePart);
      trueParticles.push_back(truePart);    
        
      part->TrueObject = truePart;

      part->TruePur = (*reco_daughter_PFP_true_byHits_purity)[itrk];

      // Not available
      part->TrueEff=0;
    }
  }
  
}

//*****************************************************************************
AnaTrueObjectC* pionTreeConverter::FindTrueParticle(Int_t ID, std::vector<AnaTrueParticleB*>& trueParticles){
//*****************************************************************************


  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == ID){
      return trueParticles[i];
    }
  }

  return NULL;
}

//*****************************************************************************
void pionTreeConverter::FillBeamTrueParticleInfo(AnaTrueParticle* truePart){
//*****************************************************************************


  if (byHits){
    truePart->ID  = reco_beam_true_byHits_ID;
    truePart->PDG = reco_beam_true_byHits_PDG;
    
    truePart->ProcessStart = truePart->ConvertProcess(*reco_beam_true_byHits_process);
    truePart->ProcessEnd   = truePart->ConvertProcess(*reco_beam_true_byHits_endProcess);      
    
    truePart->Momentum    = reco_beam_true_byHits_startP;
    truePart->MomentumEnd = reco_beam_true_byHits_endP;
    
    if (reco_beam_true_byHits_startP){
      truePart->Direction[0] = reco_beam_true_byHits_startPx/reco_beam_true_byHits_startP;
      truePart->Direction[1] = reco_beam_true_byHits_startPy/reco_beam_true_byHits_startP;
      truePart->Direction[2] = reco_beam_true_byHits_startPz/reco_beam_true_byHits_startP;
    }
    if (reco_beam_true_byHits_endP){
      truePart->DirectionEnd[0] = reco_beam_true_byHits_endPx/reco_beam_true_byHits_endP;
      truePart->DirectionEnd[1] = reco_beam_true_byHits_endPy/reco_beam_true_byHits_endP;
      truePart->DirectionEnd[2] = reco_beam_true_byHits_endPz/reco_beam_true_byHits_endP;
    }
    
  }
  else{
    truePart->ID  = reco_beam_true_byE_ID;
    truePart->PDG = reco_beam_true_byE_PDG;
    
    truePart->ProcessStart = truePart->ConvertProcess(*reco_beam_true_byE_process);
    truePart->ProcessEnd   = truePart->ConvertProcess(*reco_beam_true_byE_endProcess);      
    
    truePart->Momentum    = reco_beam_true_byE_startP;
    truePart->MomentumEnd = reco_beam_true_byE_endP;
    
    if (reco_beam_true_byE_startP){
      truePart->Direction[0] = reco_beam_true_byE_startPx/reco_beam_true_byE_startP;
      truePart->Direction[1] = reco_beam_true_byE_startPy/reco_beam_true_byE_startP;
      truePart->Direction[2] = reco_beam_true_byE_startPz/reco_beam_true_byE_startP;
    }
    if (reco_beam_true_byE_endP){
      truePart->DirectionEnd[0] = reco_beam_true_byE_endPx/reco_beam_true_byE_endP;
      truePart->DirectionEnd[1] = reco_beam_true_byE_endPy/reco_beam_true_byE_endP;
      truePart->DirectionEnd[2] = reco_beam_true_byE_endPz/reco_beam_true_byE_endP;
    }
  }

  // ----- Not yet implemented ----------
  /*
    Int_t           reco_beam_true_byE_origin;     // has value 2 or 4
    Bool_t          reco_beam_true_byE_matched;
    Double_t        reco_beam_true_byE_endE;
    Double_t        reco_beam_true_byE_startE;
    
    Int_t           reco_beam_true_byHits_origin;   // has value 2 or 4
    Bool_t          reco_beam_true_byHits_matched;
    Double_t        reco_beam_true_byHits_endE;
    Double_t        reco_beam_true_byHits_startE;    
  */

}


//*****************************************************************************
void pionTreeConverter::FillTrueBeamTrueParticleInfo(AnaTrueParticle* truePart){
//*****************************************************************************

  truePart->ID = true_beam_ID;
  truePart->PDG = true_beam_PDG;

  //    truePart->Charge = ;
  
  truePart->Momentum    = true_beam_startP;
  truePart->MomentumEnd = true_beam_endP;

  if (true_beam_startP){
    truePart->Direction[0] = true_beam_startPx/true_beam_startP;
    truePart->Direction[1] = true_beam_startPy/true_beam_startP;
    truePart->Direction[2] = true_beam_startPz/true_beam_startP;
  }
  if (true_beam_endP){
    truePart->DirectionEnd[0] = true_beam_endPx/true_beam_endP;
    truePart->DirectionEnd[1] = true_beam_endPy/true_beam_endP;
    truePart->DirectionEnd[2] = true_beam_endPz/true_beam_endP;
  }
  
  truePart->Position[0] = true_beam_startX;
  truePart->Position[1] = true_beam_startY;
  truePart->Position[2] = true_beam_startZ;
  //    truePart->Position[3] = 
  
  truePart->PositionEnd[0] = true_beam_endX;
  truePart->PositionEnd[1] = true_beam_endY;
  truePart->PositionEnd[2] = true_beam_endZ;
  //    truePart->PositionEnd[3] = 
    
  //    truePart->ProcessStart = truePart->ConvertProcess(*true_beam_process);
  truePart->ProcessEnd   = truePart->ConvertProcess(*true_beam_endProcess);      

  // ------- Not implemented yet ------------------
  /*
    Int_t           true_beam_nElasticScatters;
    std::vector<double>  *true_beam_elastic_costheta;
    std::vector<double>  *true_beam_elastic_X;
    std::vector<double>  *true_beam_elastic_Y;
    std::vector<double>  *true_beam_elastic_Z;
    Double_t        true_beam_IDE_totalDep;
    Bool_t          true_beam_IDE_found_in_recoVtx;
    Int_t           true_beam_nHits;
    std::vector<std::vector<int> > *true_beam_reco_byHits_PFP_ID;
    std::vector<std::vector<int> > *true_beam_reco_byHits_PFP_nHits;
    std::vector<std::vector<int> > *true_beam_reco_byHits_allTrack_ID;

    std::vector<std::string>  *true_beam_processes;
    
    std::vector<double>  *true_beam_incidentEnergies;
    Double_t        true_beam_interactingEnergy;

  */

  /*
    std::vector<int>     *true_beam_Pi0_decay_ID;
    std::vector<int>     *true_beam_Pi0_decay_parID;
    std::vector<int>     *true_beam_Pi0_decay_PDG;
    std::vector<double>  *true_beam_Pi0_decay_startP;
    std::vector<double>  *true_beam_Pi0_decay_len;
    std::vector<int>     *true_beam_Pi0_decay_nHits;
    std::vector<std::vector<int> > *true_beam_Pi0_decay_reco_byHits_PFP_ID;
    std::vector<std::vector<int> > *true_beam_Pi0_decay_reco_byHits_PFP_nHits;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_PFP_trackScore;
    std::vector<std::vector<int> > *true_beam_Pi0_decay_reco_byHits_allTrack_ID;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_startX;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_startY;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_startZ;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_endX;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_endY;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_endZ;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allTrack_len;
    std::vector<std::vector<int> > *true_beam_Pi0_decay_reco_byHits_allShower_ID;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allShower_startX;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allShower_startY;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allShower_startZ;
    std::vector<std::vector<double> > *true_beam_Pi0_decay_reco_byHits_allShower_len;
  */
  
}

//*****************************************************************************
void pionTreeConverter::FillTrueBeamDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart, AnaTrueParticle* parent){
//*****************************************************************************


  truePart->ID  = (*true_beam_daughter_ID)[ipart];
  truePart->PDG = (*true_beam_daughter_PDG)[ipart];
  
  truePart->ParentID  = parent->ID;
  truePart->ParentPDG = parent->PDG;

  //  truePart->GParentPDG = 0;

  truePart->ProcessStart = truePart->ConvertProcess((*true_beam_daughter_Process)[ipart]);
  truePart->ProcessEnd   = truePart->ConvertProcess((*true_beam_daughter_endProcess)[ipart]);      
  
  truePart->Momentum    = (*true_beam_daughter_startP)[ipart];

  // Not available in input tree
  //  truePart->MomentumEnd = (*true_beam_daughter_endP)[ipart];

  if ((*true_beam_daughter_startP)[ipart]){
    truePart->Direction[0] = (*true_beam_daughter_startPx)[ipart]/(*true_beam_daughter_startP)[ipart];
    truePart->Direction[1] = (*true_beam_daughter_startPy)[ipart]/(*true_beam_daughter_startP)[ipart];
    truePart->Direction[2] = (*true_beam_daughter_startPz)[ipart]/(*true_beam_daughter_startP)[ipart];
  }

  /*  Not available in input tree
  if ((*true_beam_daughter_endP)[ipart]){
    truePart->DirectionEnd[0] = (*true_beam_daughter_endPx)[ipart]/(*true_beam_daughter_endP)[ipart];
    truePart->DirectionEnd[1] = (*true_beam_daughter_endPy)[ipart]/(*true_beam_daughter_endP)[ipart];
    truePart->DirectionEnd[2] = (*true_beam_daughter_endPz)[ipart]/(*true_beam_daughter_endP)[ipart];
  }
  */

  truePart->Position[0] = (*true_beam_daughter_startX)[ipart];
  truePart->Position[1] = (*true_beam_daughter_startY)[ipart];
  truePart->Position[2] = (*true_beam_daughter_startZ)[ipart];
  //  truePart->Position[3] = 
  
  truePart->PositionEnd[0] = (*true_beam_daughter_endX)[ipart];
  truePart->PositionEnd[1] = (*true_beam_daughter_endY)[ipart];
  truePart->PositionEnd[2] = (*true_beam_daughter_endZ)[ipart];
  //  truePart->PositionEnd[3] = 

  truePart->Length = (*true_beam_daughter_len)[ipart];


  // --------- Not implemented yet --------------
  /*
    std::vector<int>     *true_beam_daughter_nHits;
    std::vector<std::vector<int> > *true_beam_daughter_reco_byHits_PFP_ID;
    std::vector<std::vector<int> > *true_beam_daughter_reco_byHits_PFP_nHits;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_PFP_trackScore;
    std::vector<std::vector<int> > *true_beam_daughter_reco_byHits_allTrack_ID;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_startX;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_startY;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_startZ;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_endX;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_endY;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_endZ;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allTrack_len;
    std::vector<std::vector<int> > *true_beam_daughter_reco_byHits_allShower_ID;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allShower_startX;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allShower_startY;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allShower_startZ;
    std::vector<std::vector<double> > *true_beam_daughter_reco_byHits_allShower_len;
  */
  
}

//*****************************************************************************
void pionTreeConverter::FillTrueBeamGrandDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart, AnaTrueParticle* parent){
//*****************************************************************************


  truePart->ID = (*true_beam_grand_daughter_ID)[ipart];
  truePart->PDG = (*true_beam_grand_daughter_PDG)[ipart];

  truePart->ParentID = (*true_beam_grand_daughter_parID)[ipart];
  
  truePart->ProcessStart = truePart->ConvertProcess((*true_beam_grand_daughter_Process)[ipart]);
  truePart->ProcessEnd   = truePart->ConvertProcess((*true_beam_grand_daughter_endProcess)[ipart]);      

  // ----- Not implemented yet ---------
  //  std::vector<int>     *true_beam_grand_daughter_nHits;

}

//*****************************************************************************
void pionTreeConverter::FillDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart){
//*****************************************************************************
  
  truePart->ID =   (*reco_daughter_PFP_true_byHits_ID)[ipart];
  truePart->PDG = (*reco_daughter_PFP_true_byHits_PDG)[ipart];

  truePart->ParentID  =   (*reco_daughter_PFP_true_byHits_parID)[ipart];
  truePart->ParentPDG =   (*reco_daughter_PFP_true_byHits_parPDG)[ipart];
  //  truePart->GParentPDG = 0;
  
  truePart->ProcessStart = truePart->ConvertProcess((*reco_daughter_PFP_true_byHits_process)[ipart]);
  truePart->ProcessEnd   = truePart->ConvertProcess((*reco_daughter_PFP_true_byHits_endProcess)[ipart]);      
  

  //    truePart->Charge = ;

  truePart->Momentum    = (*reco_daughter_PFP_true_byHits_startP)[ipart];
  //  truePart->MomentumEnd = (*reco_daughter_PFP_true_byHits_endP)[ipart];
  if ((*reco_daughter_PFP_true_byHits_startP)[ipart]){
    truePart->Direction[0] = (*reco_daughter_PFP_true_byHits_startPx)[ipart]/(*reco_daughter_PFP_true_byHits_startP)[ipart];;
    truePart->Direction[1] = (*reco_daughter_PFP_true_byHits_startPy)[ipart]/(*reco_daughter_PFP_true_byHits_startP)[ipart];;
    truePart->Direction[2] = (*reco_daughter_PFP_true_byHits_startPz)[ipart]/(*reco_daughter_PFP_true_byHits_startP)[ipart];;
  }
  /*
    if ((*reco_daughter_PFP_true_byHits_endP)[ipart]){
    truePart->DirectionEnd[0] = (*reco_daughter_PFP_true_byHits_endPx)[ipart]/(*reco_daughter_PFP_true_byHits_endP)[ipart];;
    truePart->DirectionEnd[1] = (*reco_daughter_PFP_true_byHits_endPy)[ipart]/(*reco_daughter_PFP_true_byHits_endP)[ipart];;
    truePart->DirectionEnd[2] = (*reco_daughter_PFP_true_byHits_endPz)[ipart]/(*reco_daughter_PFP_true_byHits_endP)[ipart];;
    }
  */
  truePart->Position[0] = (*reco_daughter_PFP_true_byHits_startX)[ipart];
  truePart->Position[1] = (*reco_daughter_PFP_true_byHits_startY)[ipart];
  truePart->Position[2] = (*reco_daughter_PFP_true_byHits_startZ)[ipart];
  //    truePart->Position[3] = StartT_drifted[ipart];
  
  truePart->PositionEnd[0] = (*reco_daughter_PFP_true_byHits_endX)[ipart];
  truePart->PositionEnd[1] = (*reco_daughter_PFP_true_byHits_endY)[ipart];
  truePart->PositionEnd[2] = (*reco_daughter_PFP_true_byHits_endZ)[ipart];
  //    truePart->PositionEnd[3] = EndT_drifted[ipart];
  
  
  truePart->Length =   (*reco_daughter_PFP_true_byHits_len)[ipart];
  
  
  // --------- Not yet implemented -----------------
  /*
    std::vector<int>     *reco_daughter_PFP_true_byHits_origin;    // has value 2 or 4
    std::vector<unsigned long> *reco_daughter_PFP_true_byHits_sharedHits;
    std::vector<unsigned long> *reco_daughter_PFP_true_byHits_emHits;
    std::vector<double>  *reco_daughter_PFP_true_byHits_startE;
  */

}
