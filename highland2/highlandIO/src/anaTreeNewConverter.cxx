#include "anaTreeNewConverter.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"

//********************************************************************
anaTreeNewConverter::anaTreeNewConverter():InputConverter("analysistree/anatree"){
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
bool anaTreeNewConverter::Initialize(){
//********************************************************************
  
  std::string folder= "analysistree";

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
   fChain->SetBranchAddress("beamtime", &beamtime, &b_beamtime);
   fChain->SetBranchAddress("pot", &pot, &b_pot);
   fChain->SetBranchAddress("isdata", &isdata, &b_isdata);
   fChain->SetBranchAddress("taulife", &taulife, &b_taulife);
   fChain->SetBranchAddress("triggernumber", &triggernumber, &b_triggernumber);
   fChain->SetBranchAddress("triggertime", &triggertime, &b_triggertime);
   fChain->SetBranchAddress("beamgatetime", &beamgatetime, &b_beamgatetime);
   fChain->SetBranchAddress("triggerbits", &triggerbits, &b_triggerbits);
   /*
   fChain->SetBranchAddress("potbnb", &potbnb, &b_potbnb);
   fChain->SetBranchAddress("potnumitgt", &potnumitgt, &b_potnumitgt);
   fChain->SetBranchAddress("potnumi101", &potnumi101, &b_potnumi101);
   fChain->SetBranchAddress("no_hits", &no_hits, &b_no_hits);
   fChain->SetBranchAddress("no_hits_stored", &no_hits_stored, &b_no_hits_stored);
   fChain->SetBranchAddress("hit_tpc", hit_tpc, &b_hit_tpc);
   fChain->SetBranchAddress("hit_plane", hit_plane, &b_hit_plane);
   fChain->SetBranchAddress("hit_wire", hit_wire, &b_hit_wire);
   fChain->SetBranchAddress("hit_channel", hit_channel, &b_hit_channel);
   fChain->SetBranchAddress("hit_peakT", hit_peakT, &b_hit_peakT);
   fChain->SetBranchAddress("hit_charge", hit_charge, &b_hit_charge);
   fChain->SetBranchAddress("hit_ph", hit_ph, &b_hit_ph);
   fChain->SetBranchAddress("hit_startT", hit_startT, &b_hit_startT);
   fChain->SetBranchAddress("hit_endT", hit_endT, &b_hit_endT);
   fChain->SetBranchAddress("hit_rms", hit_rms, &b_hit_rms);
   fChain->SetBranchAddress("hit_trueX", hit_trueX, &b_hit_trueX);
   fChain->SetBranchAddress("hit_goodnessOfFit", hit_goodnessOfFit, &b_hit_goodnessOfFit);
   fChain->SetBranchAddress("hit_multiplicity", hit_multiplicity, &b_hit_multiplicity);
   fChain->SetBranchAddress("hit_trkid", hit_trkid, &b_hit_trkid);
   fChain->SetBranchAddress("hit_trkKey", hit_trkKey, &b_hit_trkKey);
   fChain->SetBranchAddress("hit_clusterid", hit_clusterid, &b_hit_clusterid);
   fChain->SetBranchAddress("hit_clusterKey", hit_clusterKey, &b_hit_clusterKey);
   fChain->SetBranchAddress("hit_nelec", hit_nelec, &b_hit_nelec);
   fChain->SetBranchAddress("hit_energy", hit_energy, &b_hit_energy);
   fChain->SetBranchAddress("nclusters", &nclusters, &b_nclusters);
   fChain->SetBranchAddress("clusterId", clusterId, &b_clusterId);
   fChain->SetBranchAddress("clusterView", clusterView, &b_clusterView);
   fChain->SetBranchAddress("cluster_StartCharge", cluster_StartCharge, &b_cluster_StartCharge);
   fChain->SetBranchAddress("cluster_StartAngle", cluster_StartAngle, &b_cluster_StartAngle);
   fChain->SetBranchAddress("cluster_EndCharge", cluster_EndCharge, &b_cluster_EndCharge);
   fChain->SetBranchAddress("cluster_EndAngle", cluster_EndAngle, &b_cluster_EndAngle);
   fChain->SetBranchAddress("cluster_Integral", cluster_Integral, &b_cluster_Integral);
   fChain->SetBranchAddress("cluster_IntegralAverage", cluster_IntegralAverage, &b_cluster_IntegralAverage);
   fChain->SetBranchAddress("cluster_SummedADC", cluster_SummedADC, &b_cluster_SummedADC);
   fChain->SetBranchAddress("cluster_SummedADCaverage", cluster_SummedADCaverage, &b_cluster_SummedADCaverage);
   fChain->SetBranchAddress("cluster_MultipleHitDensity", cluster_MultipleHitDensity, &b_cluster_MultipleHitDensity);
   fChain->SetBranchAddress("cluster_Width", cluster_Width, &b_cluster_Width);
   fChain->SetBranchAddress("cluster_NHits", cluster_NHits, &b_cluster_NHits);
   fChain->SetBranchAddress("cluster_StartWire", cluster_StartWire, &b_cluster_StartWire);
   fChain->SetBranchAddress("cluster_StartTick", cluster_StartTick, &b_cluster_StartTick);
   fChain->SetBranchAddress("cluster_EndWire", cluster_EndWire, &b_cluster_EndWire);
   fChain->SetBranchAddress("cluster_EndTick", cluster_EndTick, &b_cluster_EndTick);
   fChain->SetBranchAddress("cluncosmictags_tagger", cluncosmictags_tagger, &b_cluncosmictags_tagger);
   fChain->SetBranchAddress("clucosmicscore_tagger", clucosmicscore_tagger, &b_clucosmicscore_tagger);
   fChain->SetBranchAddress("clucosmictype_tagger", clucosmictype_tagger, &b_clucosmictype_tagger);
   fChain->SetBranchAddress("no_flashes", &no_flashes, &b_no_flashes);
   fChain->SetBranchAddress("flash_time", flash_time, &b_flash_time);
   fChain->SetBranchAddress("flash_pe", flash_pe, &b_flash_pe);
   fChain->SetBranchAddress("flash_ycenter", flash_ycenter, &b_flash_ycenter);
   fChain->SetBranchAddress("flash_zcenter", flash_zcenter, &b_flash_zcenter);
   fChain->SetBranchAddress("flash_ywidth", flash_ywidth, &b_flash_ywidth);
   fChain->SetBranchAddress("flash_zwidth", flash_zwidth, &b_flash_zwidth);
   fChain->SetBranchAddress("flash_timewidth", flash_timewidth, &b_flash_timewidth);
   fChain->SetBranchAddress("no_ExternCounts", &no_ExternCounts, &b_no_ExternCounts);
   fChain->SetBranchAddress("externcounts_time", &externcounts_time, &b_externcounts_time);
   fChain->SetBranchAddress("externcounts_id", &externcounts_id, &b_externcounts_id);
   fChain->SetBranchAddress("kNTracker", &kNTracker, &b_kNTracker);
   fChain->SetBranchAddress("kNVertexAlgos", &kNVertexAlgos, &b_kNVertexAlgos);
   fChain->SetBranchAddress("mcevts_truthcry", &mcevts_truthcry, &b_mcevts_truthcry);
   fChain->SetBranchAddress("cry_no_primaries", &cry_no_primaries, &b_cry_no_primaries);
   fChain->SetBranchAddress("cry_primaries_pdg", cry_primaries_pdg, &b_cry_primaries_pdg);
   fChain->SetBranchAddress("cry_Eng", cry_Eng, &b_cry_Eng);
   fChain->SetBranchAddress("cry_Px", cry_Px, &b_cry_Px);
   fChain->SetBranchAddress("cry_Py", cry_Py, &b_cry_Py);
   fChain->SetBranchAddress("cry_Pz", cry_Pz, &b_cry_Pz);
   fChain->SetBranchAddress("cry_P", cry_P, &b_cry_P);
   fChain->SetBranchAddress("cry_StartPointx", cry_StartPointx, &b_cry_StartPointx);
   fChain->SetBranchAddress("cry_StartPointy", cry_StartPointy, &b_cry_StartPointy);
   fChain->SetBranchAddress("cry_StartPointz", cry_StartPointz, &b_cry_StartPointz);
   fChain->SetBranchAddress("cry_StartPointt", cry_StartPointt, &b_cry_StartPointt);
   fChain->SetBranchAddress("cry_status_code", cry_status_code, &b_cry_status_code);
   fChain->SetBranchAddress("cry_mass", cry_mass, &b_cry_mass);
   fChain->SetBranchAddress("cry_trackID", cry_trackID, &b_cry_trackID);
   fChain->SetBranchAddress("cry_ND", cry_ND, &b_cry_ND);
   fChain->SetBranchAddress("cry_mother", cry_mother, &b_cry_mother);
   */
   fChain->SetBranchAddress("no_primaries", &no_primaries, &b_no_primaries);
   fChain->SetBranchAddress("geant_list_size", &geant_list_size, &b_geant_list_size);
   fChain->SetBranchAddress("geant_list_size_in_tpcAV", &geant_list_size_in_tpcAV, &b_geant_list_size_in_tpcAV);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("status", status, &b_status);
   fChain->SetBranchAddress("Mass", Mass, &b_Mass);
   /*
   fChain->SetBranchAddress("Eng", Eng, &b_Eng);
   fChain->SetBranchAddress("EndE", EndE, &b_EndE);
   fChain->SetBranchAddress("Px", Px, &b_Px);
   fChain->SetBranchAddress("Py", Py, &b_Py);
   fChain->SetBranchAddress("Pz", Pz, &b_Pz);
   fChain->SetBranchAddress("P", P, &b_P);
   fChain->SetBranchAddress("StartPointx", StartPointx, &b_StartPointx);
   fChain->SetBranchAddress("StartPointy", StartPointy, &b_StartPointy);
   fChain->SetBranchAddress("StartPointz", StartPointz, &b_StartPointz);
   fChain->SetBranchAddress("StartT", StartT, &b_StartT);
   fChain->SetBranchAddress("EndPointx", EndPointx, &b_EndPointx);
   fChain->SetBranchAddress("EndPointy", EndPointy, &b_EndPointy);
   fChain->SetBranchAddress("EndPointz", EndPointz, &b_EndPointz);
   fChain->SetBranchAddress("EndT", EndT, &b_EndT);
   fChain->SetBranchAddress("theta", theta, &b_theta);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("theta_xz", theta_xz, &b_theta_xz);
   fChain->SetBranchAddress("theta_yz", theta_yz, &b_theta_yz);
   fChain->SetBranchAddress("pathlen", pathlen, &b_pathlen);
   fChain->SetBranchAddress("inTPCActive", inTPCActive, &b_inTPCActive);
   fChain->SetBranchAddress("StartPointx_tpcAV", StartPointx_tpcAV, &b_StartPointx_tpcAV);
   fChain->SetBranchAddress("StartPointy_tpcAV", StartPointy_tpcAV, &b_StartPointy_tpcAV);
   fChain->SetBranchAddress("StartPointz_tpcAV", StartPointz_tpcAV, &b_StartPointz_tpcAV);
   fChain->SetBranchAddress("StartT_tpcAV", StartT_tpcAV, &b_StartT_tpcAV);
   fChain->SetBranchAddress("StartE_tpcAV", StartE_tpcAV, &b_StartE_tpcAV);
   fChain->SetBranchAddress("StartP_tpcAV", StartP_tpcAV, &b_StartP_tpcAV);
   fChain->SetBranchAddress("StartPx_tpcAV", StartPx_tpcAV, &b_StartPx_tpcAV);
   fChain->SetBranchAddress("StartPy_tpcAV", StartPy_tpcAV, &b_StartPy_tpcAV);
   fChain->SetBranchAddress("StartPz_tpcAV", StartPz_tpcAV, &b_StartPz_tpcAV);
   fChain->SetBranchAddress("EndPointx_tpcAV", EndPointx_tpcAV, &b_EndPointx_tpcAV);
   fChain->SetBranchAddress("EndPointy_tpcAV", EndPointy_tpcAV, &b_EndPointy_tpcAV);
   fChain->SetBranchAddress("EndPointz_tpcAV", EndPointz_tpcAV, &b_EndPointz_tpcAV);
   fChain->SetBranchAddress("EndT_tpcAV", EndT_tpcAV, &b_EndT_tpcAV);
   fChain->SetBranchAddress("EndE_tpcAV", EndE_tpcAV, &b_EndE_tpcAV);
   fChain->SetBranchAddress("EndP_tpcAV", EndP_tpcAV, &b_EndP_tpcAV);
   fChain->SetBranchAddress("EndPx_tpcAV", EndPx_tpcAV, &b_EndPx_tpcAV);
   fChain->SetBranchAddress("EndPy_tpcAV", EndPy_tpcAV, &b_EndPy_tpcAV);
   fChain->SetBranchAddress("EndPz_tpcAV", EndPz_tpcAV, &b_EndPz_tpcAV);
   fChain->SetBranchAddress("pathlen_drifted", pathlen_drifted, &b_pathlen_drifted);
   */
   fChain->SetBranchAddress("inTPCDrifted", inTPCDrifted, &b_inTPCDrifted);
   fChain->SetBranchAddress("StartPointx_drifted", StartPointx_drifted, &b_StartPointx_drifted);
   fChain->SetBranchAddress("StartPointy_drifted", StartPointy_drifted, &b_StartPointy_drifted);
   fChain->SetBranchAddress("StartPointz_drifted", StartPointz_drifted, &b_StartPointz_drifted);
   fChain->SetBranchAddress("StartT_drifted", StartT_drifted, &b_StartT_drifted);
   fChain->SetBranchAddress("StartE_drifted", StartE_drifted, &b_StartE_drifted);
   fChain->SetBranchAddress("StartP_drifted", StartP_drifted, &b_StartP_drifted);
   fChain->SetBranchAddress("StartPx_drifted", StartPx_drifted, &b_StartPx_drifted);
   fChain->SetBranchAddress("StartPy_drifted", StartPy_drifted, &b_StartPy_drifted);
   fChain->SetBranchAddress("StartPz_drifted", StartPz_drifted, &b_StartPz_drifted);
   fChain->SetBranchAddress("EndPointx_drifted", EndPointx_drifted, &b_EndPointx_drifted);
   fChain->SetBranchAddress("EndPointy_drifted", EndPointy_drifted, &b_EndPointy_drifted);
   fChain->SetBranchAddress("EndPointz_drifted", EndPointz_drifted, &b_EndPointz_drifted);
   fChain->SetBranchAddress("EndT_drifted", EndT_drifted, &b_EndT_drifted);
   fChain->SetBranchAddress("EndE_drifted", EndE_drifted, &b_EndE_drifted);
   fChain->SetBranchAddress("EndP_drifted", EndP_drifted, &b_EndP_drifted);
   fChain->SetBranchAddress("EndPx_drifted", EndPx_drifted, &b_EndPx_drifted);
   fChain->SetBranchAddress("EndPy_drifted", EndPy_drifted, &b_EndPy_drifted);
   fChain->SetBranchAddress("EndPz_drifted", EndPz_drifted, &b_EndPz_drifted);
   fChain->SetBranchAddress("NumberDaughters", NumberDaughters, &b_NumberDaughters);
   fChain->SetBranchAddress("Mother", Mother, &b_Mother);
   fChain->SetBranchAddress("TrackId", TrackId, &b_TrackId);
   fChain->SetBranchAddress("MergedId", MergedId, &b_MergedId);
   fChain->SetBranchAddress("origin", origin, &b_origin);
   fChain->SetBranchAddress("MCTruthIndex", MCTruthIndex, &b_MCTruthIndex);
   fChain->SetBranchAddress("process_primary", process_primary, &b_process_primary);
   fChain->SetBranchAddress("processname", &processname, &b_processname);
   /*
   fChain->SetBranchAddress("ntracks_pmtrack", &ntracks_pmtrack, &b_ntracks_pmtrack);
   fChain->SetBranchAddress("trkId_pmtrack", trkId_pmtrack, &b_trkId_pmtrack);
   fChain->SetBranchAddress("trkncosmictags_tagger_pmtrack", trkncosmictags_tagger_pmtrack, &b_trkncosmictags_tagger_pmtrack);
   fChain->SetBranchAddress("trkcosmicscore_tagger_pmtrack", trkcosmicscore_tagger_pmtrack, &b_trkcosmicscore_tagger_pmtrack);
   fChain->SetBranchAddress("trkcosmictype_tagger_pmtrack", trkcosmictype_tagger_pmtrack, &b_trkcosmictype_tagger_pmtrack);
   fChain->SetBranchAddress("trkncosmictags_containmenttagger_pmtrack", trkncosmictags_containmenttagger_pmtrack, &b_trkncosmictags_containmenttagger_pmtrack);
   fChain->SetBranchAddress("trkcosmicscore_containmenttagger_pmtrack", trkcosmicscore_containmenttagger_pmtrack, &b_trkcosmicscore_containmenttagger_pmtrack);
   fChain->SetBranchAddress("trkcosmictype_containmenttagger_pmtrack", trkcosmictype_containmenttagger_pmtrack, &b_trkcosmictype_containmenttagger_pmtrack);
   fChain->SetBranchAddress("trkncosmictags_flashmatch_pmtrack", trkncosmictags_flashmatch_pmtrack, &b_trkncosmictags_flashmatch_pmtrack);
   fChain->SetBranchAddress("trkcosmicscore_flashmatch_pmtrack", trkcosmicscore_flashmatch_pmtrack, &b_trkcosmicscore_flashmatch_pmtrack);
   fChain->SetBranchAddress("trkcosmictype_flashmatch_pmtrack", trkcosmictype_flashmatch_pmtrack, &b_trkcosmictype_flashmatch_pmtrack);
   fChain->SetBranchAddress("trkke_pmtrack", trkke_pmtrack, &b_trkke_pmtrack);
   fChain->SetBranchAddress("trkrange_pmtrack", trkrange_pmtrack, &b_trkrange_pmtrack);
   fChain->SetBranchAddress("trkidtruth_pmtrack", trkidtruth_pmtrack, &b_trkidtruth_pmtrack);
   fChain->SetBranchAddress("trkorigin_pmtrack", trkorigin_pmtrack, &b_trkorigin_pmtrack);
   fChain->SetBranchAddress("trkpdgtruth_pmtrack", trkpdgtruth_pmtrack, &b_trkpdgtruth_pmtrack);
   fChain->SetBranchAddress("trkefftruth_pmtrack", trkefftruth_pmtrack, &b_trkefftruth_pmtrack);
   fChain->SetBranchAddress("trkpurtruth_pmtrack", trkpurtruth_pmtrack, &b_trkpurtruth_pmtrack);
   fChain->SetBranchAddress("trkpitchc_pmtrack", trkpitchc_pmtrack, &b_trkpitchc_pmtrack);
   fChain->SetBranchAddress("ntrkhits_pmtrack", ntrkhits_pmtrack, &b_ntrkhits_pmtrack);
   fChain->SetBranchAddress("trkdedx_pmtrack", trkdedx_pmtrack, &b_trkdedx_pmtrack);
   fChain->SetBranchAddress("trkdqdx_pmtrack", trkdqdx_pmtrack, &b_trkdqdx_pmtrack);
   fChain->SetBranchAddress("trkresrg_pmtrack", trkresrg_pmtrack, &b_trkresrg_pmtrack);
   fChain->SetBranchAddress("trktpc_pmtrack", trktpc_pmtrack, &b_trktpc_pmtrack);
   fChain->SetBranchAddress("trkxyz_pmtrack", trkxyz_pmtrack, &b_trkxyz_pmtrack);
   fChain->SetBranchAddress("trkstartx_pmtrack", trkstartx_pmtrack, &b_trkstartx_pmtrack);
   fChain->SetBranchAddress("trkstarty_pmtrack", trkstarty_pmtrack, &b_trkstarty_pmtrack);
   fChain->SetBranchAddress("trkstartz_pmtrack", trkstartz_pmtrack, &b_trkstartz_pmtrack);
   fChain->SetBranchAddress("trkstartd_pmtrack", trkstartd_pmtrack, &b_trkstartd_pmtrack);
   fChain->SetBranchAddress("trkendx_pmtrack", trkendx_pmtrack, &b_trkendx_pmtrack);
   fChain->SetBranchAddress("trkendy_pmtrack", trkendy_pmtrack, &b_trkendy_pmtrack);
   fChain->SetBranchAddress("trkendz_pmtrack", trkendz_pmtrack, &b_trkendz_pmtrack);
   fChain->SetBranchAddress("trkendd_pmtrack", trkendd_pmtrack, &b_trkendd_pmtrack);
   fChain->SetBranchAddress("trkflashT0_pmtrack", trkflashT0_pmtrack, &b_trkflashT0_pmtrack);
   fChain->SetBranchAddress("trktrueT0_pmtrack", trktrueT0_pmtrack, &b_trktrueT0_pmtrack);
   fChain->SetBranchAddress("trkg4id_pmtrack", trkg4id_pmtrack, &b_trkg4id_pmtrack);
   fChain->SetBranchAddress("trkorig_pmtrack", trkorig_pmtrack, &b_trkorig_pmtrack);
   fChain->SetBranchAddress("trkpurity_pmtrack", trkpurity_pmtrack, &b_trkpurity_pmtrack);
   fChain->SetBranchAddress("trkcompleteness_pmtrack", trkcompleteness_pmtrack, &b_trkcompleteness_pmtrack);
   fChain->SetBranchAddress("trktheta_pmtrack", trktheta_pmtrack, &b_trktheta_pmtrack);
   fChain->SetBranchAddress("trkphi_pmtrack", trkphi_pmtrack, &b_trkphi_pmtrack);
   fChain->SetBranchAddress("trkstartdcosx_pmtrack", trkstartdcosx_pmtrack, &b_trkstartdcosx_pmtrack);
   fChain->SetBranchAddress("trkstartdcosy_pmtrack", trkstartdcosy_pmtrack, &b_trkstartdcosy_pmtrack);
   fChain->SetBranchAddress("trkstartdcosz_pmtrack", trkstartdcosz_pmtrack, &b_trkstartdcosz_pmtrack);
   fChain->SetBranchAddress("trkenddcosx_pmtrack", trkenddcosx_pmtrack, &b_trkenddcosx_pmtrack);
   fChain->SetBranchAddress("trkenddcosy_pmtrack", trkenddcosy_pmtrack, &b_trkenddcosy_pmtrack);
   fChain->SetBranchAddress("trkenddcosz_pmtrack", trkenddcosz_pmtrack, &b_trkenddcosz_pmtrack);
   fChain->SetBranchAddress("trkthetaxz_pmtrack", trkthetaxz_pmtrack, &b_trkthetaxz_pmtrack);
   fChain->SetBranchAddress("trkthetayz_pmtrack", trkthetayz_pmtrack, &b_trkthetayz_pmtrack);
   fChain->SetBranchAddress("trkmom_pmtrack", trkmom_pmtrack, &b_trkmom_pmtrack);
   fChain->SetBranchAddress("trkmomrange_pmtrack", trkmomrange_pmtrack, &b_trkmomrange_pmtrack);
   fChain->SetBranchAddress("trkmommschi2_pmtrack", trkmommschi2_pmtrack, &b_trkmommschi2_pmtrack);
   fChain->SetBranchAddress("trkmommsllhd_pmtrack", trkmommsllhd_pmtrack, &b_trkmommsllhd_pmtrack);
   fChain->SetBranchAddress("trklen_pmtrack", trklen_pmtrack, &b_trklen_pmtrack);
   fChain->SetBranchAddress("trksvtxid_pmtrack", trksvtxid_pmtrack, &b_trksvtxid_pmtrack);
   fChain->SetBranchAddress("trkevtxid_pmtrack", trkevtxid_pmtrack, &b_trkevtxid_pmtrack);
   fChain->SetBranchAddress("trkpidmvamu_pmtrack", trkpidmvamu_pmtrack, &b_trkpidmvamu_pmtrack);
   fChain->SetBranchAddress("trkpidmvae_pmtrack", trkpidmvae_pmtrack, &b_trkpidmvae_pmtrack);
   fChain->SetBranchAddress("trkpidmvapich_pmtrack", trkpidmvapich_pmtrack, &b_trkpidmvapich_pmtrack);
   fChain->SetBranchAddress("trkpidmvapi0_pmtrack", trkpidmvapi0_pmtrack, &b_trkpidmvapi0_pmtrack);
   fChain->SetBranchAddress("trkpidmvapr_pmtrack", trkpidmvapr_pmtrack, &b_trkpidmvapr_pmtrack);
   fChain->SetBranchAddress("trkpidpdg_pmtrack", trkpidpdg_pmtrack, &b_trkpidpdg_pmtrack);
   fChain->SetBranchAddress("trkpidchi_pmtrack", trkpidchi_pmtrack, &b_trkpidchi_pmtrack);
   fChain->SetBranchAddress("trkpidchipr_pmtrack", trkpidchipr_pmtrack, &b_trkpidchipr_pmtrack);
   fChain->SetBranchAddress("trkpidchika_pmtrack", trkpidchika_pmtrack, &b_trkpidchika_pmtrack);
   fChain->SetBranchAddress("trkpidchipi_pmtrack", trkpidchipi_pmtrack, &b_trkpidchipi_pmtrack);
   fChain->SetBranchAddress("trkpidchimu_pmtrack", trkpidchimu_pmtrack, &b_trkpidchimu_pmtrack);
   fChain->SetBranchAddress("trkpidpida_pmtrack", trkpidpida_pmtrack, &b_trkpidpida_pmtrack);
   fChain->SetBranchAddress("trkpidbestplane_pmtrack", trkpidbestplane_pmtrack, &b_trkpidbestplane_pmtrack);
   fChain->SetBranchAddress("trkhasPFParticle_pmtrack", trkhasPFParticle_pmtrack, &b_trkhasPFParticle_pmtrack);
   fChain->SetBranchAddress("trkPFParticleID_pmtrack", trkPFParticleID_pmtrack, &b_trkPFParticleID_pmtrack);
   fChain->SetBranchAddress("ntracks_pandora", &ntracks_pandora, &b_ntracks_pandora);
   fChain->SetBranchAddress("trkId_pandora", trkId_pandora, &b_trkId_pandora);
   fChain->SetBranchAddress("trkncosmictags_tagger_pandora", trkncosmictags_tagger_pandora, &b_trkncosmictags_tagger_pandora);
   fChain->SetBranchAddress("trkcosmicscore_tagger_pandora", trkcosmicscore_tagger_pandora, &b_trkcosmicscore_tagger_pandora);
   fChain->SetBranchAddress("trkcosmictype_tagger_pandora", trkcosmictype_tagger_pandora, &b_trkcosmictype_tagger_pandora);
   fChain->SetBranchAddress("trkncosmictags_containmenttagger_pandora", trkncosmictags_containmenttagger_pandora, &b_trkncosmictags_containmenttagger_pandora);
   fChain->SetBranchAddress("trkcosmicscore_containmenttagger_pandora", trkcosmicscore_containmenttagger_pandora, &b_trkcosmicscore_containmenttagger_pandora);
   fChain->SetBranchAddress("trkcosmictype_containmenttagger_pandora", trkcosmictype_containmenttagger_pandora, &b_trkcosmictype_containmenttagger_pandora);
   fChain->SetBranchAddress("trkncosmictags_flashmatch_pandora", trkncosmictags_flashmatch_pandora, &b_trkncosmictags_flashmatch_pandora);
   fChain->SetBranchAddress("trkcosmicscore_flashmatch_pandora", trkcosmicscore_flashmatch_pandora, &b_trkcosmicscore_flashmatch_pandora);
   fChain->SetBranchAddress("trkcosmictype_flashmatch_pandora", trkcosmictype_flashmatch_pandora, &b_trkcosmictype_flashmatch_pandora);
   fChain->SetBranchAddress("trkke_pandora", trkke_pandora, &b_trkke_pandora);
   fChain->SetBranchAddress("trkrange_pandora", trkrange_pandora, &b_trkrange_pandora);
   fChain->SetBranchAddress("trkidtruth_pandora", trkidtruth_pandora, &b_trkidtruth_pandora);
   fChain->SetBranchAddress("trkorigin_pandora", trkorigin_pandora, &b_trkorigin_pandora);
   fChain->SetBranchAddress("trkpdgtruth_pandora", trkpdgtruth_pandora, &b_trkpdgtruth_pandora);
   fChain->SetBranchAddress("trkefftruth_pandora", trkefftruth_pandora, &b_trkefftruth_pandora);
   fChain->SetBranchAddress("trkpurtruth_pandora", trkpurtruth_pandora, &b_trkpurtruth_pandora);
   fChain->SetBranchAddress("trkpitchc_pandora", trkpitchc_pandora, &b_trkpitchc_pandora);
   fChain->SetBranchAddress("ntrkhits_pandora", ntrkhits_pandora, &b_ntrkhits_pandora);
   fChain->SetBranchAddress("trkdedx_pandora", trkdedx_pandora, &b_trkdedx_pandora);
   fChain->SetBranchAddress("trkdqdx_pandora", trkdqdx_pandora, &b_trkdqdx_pandora);
   fChain->SetBranchAddress("trkresrg_pandora", trkresrg_pandora, &b_trkresrg_pandora);
   fChain->SetBranchAddress("trktpc_pandora", trktpc_pandora, &b_trktpc_pandora);
   fChain->SetBranchAddress("trkxyz_pandora", trkxyz_pandora, &b_trkxyz_pandora);
   fChain->SetBranchAddress("trkstartx_pandora", trkstartx_pandora, &b_trkstartx_pandora);
   fChain->SetBranchAddress("trkstarty_pandora", trkstarty_pandora, &b_trkstarty_pandora);
   fChain->SetBranchAddress("trkstartz_pandora", trkstartz_pandora, &b_trkstartz_pandora);
   fChain->SetBranchAddress("trkstartd_pandora", trkstartd_pandora, &b_trkstartd_pandora);
   fChain->SetBranchAddress("trkendx_pandora", trkendx_pandora, &b_trkendx_pandora);
   fChain->SetBranchAddress("trkendy_pandora", trkendy_pandora, &b_trkendy_pandora);
   fChain->SetBranchAddress("trkendz_pandora", trkendz_pandora, &b_trkendz_pandora);
   fChain->SetBranchAddress("trkendd_pandora", trkendd_pandora, &b_trkendd_pandora);
   fChain->SetBranchAddress("trkflashT0_pandora", trkflashT0_pandora, &b_trkflashT0_pandora);
   fChain->SetBranchAddress("trktrueT0_pandora", trktrueT0_pandora, &b_trktrueT0_pandora);
   fChain->SetBranchAddress("trkg4id_pandora", trkg4id_pandora, &b_trkg4id_pandora);
   fChain->SetBranchAddress("trkorig_pandora", trkorig_pandora, &b_trkorig_pandora);
   fChain->SetBranchAddress("trkpurity_pandora", trkpurity_pandora, &b_trkpurity_pandora);
   fChain->SetBranchAddress("trkcompleteness_pandora", trkcompleteness_pandora, &b_trkcompleteness_pandora);
   fChain->SetBranchAddress("trktheta_pandora", trktheta_pandora, &b_trktheta_pandora);
   fChain->SetBranchAddress("trkphi_pandora", trkphi_pandora, &b_trkphi_pandora);
   fChain->SetBranchAddress("trkstartdcosx_pandora", trkstartdcosx_pandora, &b_trkstartdcosx_pandora);
   fChain->SetBranchAddress("trkstartdcosy_pandora", trkstartdcosy_pandora, &b_trkstartdcosy_pandora);
   fChain->SetBranchAddress("trkstartdcosz_pandora", trkstartdcosz_pandora, &b_trkstartdcosz_pandora);
   fChain->SetBranchAddress("trkenddcosx_pandora", trkenddcosx_pandora, &b_trkenddcosx_pandora);
   fChain->SetBranchAddress("trkenddcosy_pandora", trkenddcosy_pandora, &b_trkenddcosy_pandora);
   fChain->SetBranchAddress("trkenddcosz_pandora", trkenddcosz_pandora, &b_trkenddcosz_pandora);
   fChain->SetBranchAddress("trkthetaxz_pandora", trkthetaxz_pandora, &b_trkthetaxz_pandora);
   fChain->SetBranchAddress("trkthetayz_pandora", trkthetayz_pandora, &b_trkthetayz_pandora);
   fChain->SetBranchAddress("trkmom_pandora", trkmom_pandora, &b_trkmom_pandora);
   fChain->SetBranchAddress("trkmomrange_pandora", trkmomrange_pandora, &b_trkmomrange_pandora);
   fChain->SetBranchAddress("trkmommschi2_pandora", trkmommschi2_pandora, &b_trkmommschi2_pandora);
   fChain->SetBranchAddress("trkmommsllhd_pandora", trkmommsllhd_pandora, &b_trkmommsllhd_pandora);
   fChain->SetBranchAddress("trklen_pandora", trklen_pandora, &b_trklen_pandora);
   fChain->SetBranchAddress("trksvtxid_pandora", trksvtxid_pandora, &b_trksvtxid_pandora);
   fChain->SetBranchAddress("trkevtxid_pandora", trkevtxid_pandora, &b_trkevtxid_pandora);
   fChain->SetBranchAddress("trkpidmvamu_pandora", trkpidmvamu_pandora, &b_trkpidmvamu_pandora);
   fChain->SetBranchAddress("trkpidmvae_pandora", trkpidmvae_pandora, &b_trkpidmvae_pandora);
   fChain->SetBranchAddress("trkpidmvapich_pandora", trkpidmvapich_pandora, &b_trkpidmvapich_pandora);
   fChain->SetBranchAddress("trkpidmvapi0_pandora", trkpidmvapi0_pandora, &b_trkpidmvapi0_pandora);
   fChain->SetBranchAddress("trkpidmvapr_pandora", trkpidmvapr_pandora, &b_trkpidmvapr_pandora);
   fChain->SetBranchAddress("trkpidpdg_pandora", trkpidpdg_pandora, &b_trkpidpdg_pandora);
   fChain->SetBranchAddress("trkpidchi_pandora", trkpidchi_pandora, &b_trkpidchi_pandora);
   fChain->SetBranchAddress("trkpidchipr_pandora", trkpidchipr_pandora, &b_trkpidchipr_pandora);
   fChain->SetBranchAddress("trkpidchika_pandora", trkpidchika_pandora, &b_trkpidchika_pandora);
   fChain->SetBranchAddress("trkpidchipi_pandora", trkpidchipi_pandora, &b_trkpidchipi_pandora);
   fChain->SetBranchAddress("trkpidchimu_pandora", trkpidchimu_pandora, &b_trkpidchimu_pandora);
   fChain->SetBranchAddress("trkpidpida_pandora", trkpidpida_pandora, &b_trkpidpida_pandora);
   fChain->SetBranchAddress("trkpidbestplane_pandora", trkpidbestplane_pandora, &b_trkpidbestplane_pandora);
   fChain->SetBranchAddress("trkhasPFParticle_pandora", trkhasPFParticle_pandora, &b_trkhasPFParticle_pandora);
   fChain->SetBranchAddress("trkPFParticleID_pandora", trkPFParticleID_pandora, &b_trkPFParticleID_pandora);
   */
   fChain->SetBranchAddress("ntracks_pmtrajfit", &ntracks_pmtrajfit, &b_ntracks_pmtrajfit);
   fChain->SetBranchAddress("trkId_pmtrajfit", trkId_pmtrajfit, &b_trkId_pmtrajfit);
   fChain->SetBranchAddress("trkncosmictags_tagger_pmtrajfit", trkncosmictags_tagger_pmtrajfit, &b_trkncosmictags_tagger_pmtrajfit);
   fChain->SetBranchAddress("trkcosmicscore_tagger_pmtrajfit", trkcosmicscore_tagger_pmtrajfit, &b_trkcosmicscore_tagger_pmtrajfit);
   fChain->SetBranchAddress("trkcosmictype_tagger_pmtrajfit", trkcosmictype_tagger_pmtrajfit, &b_trkcosmictype_tagger_pmtrajfit);
   fChain->SetBranchAddress("trkncosmictags_containmenttagger_pmtrajfit", trkncosmictags_containmenttagger_pmtrajfit, &b_trkncosmictags_containmenttagger_pmtrajfit);
   fChain->SetBranchAddress("trkcosmicscore_containmenttagger_pmtrajfit", trkcosmicscore_containmenttagger_pmtrajfit, &b_trkcosmicscore_containmenttagger_pmtrajfit);
   fChain->SetBranchAddress("trkcosmictype_containmenttagger_pmtrajfit", trkcosmictype_containmenttagger_pmtrajfit, &b_trkcosmictype_containmenttagger_pmtrajfit);
   fChain->SetBranchAddress("trkncosmictags_flashmatch_pmtrajfit", trkncosmictags_flashmatch_pmtrajfit, &b_trkncosmictags_flashmatch_pmtrajfit);
   fChain->SetBranchAddress("trkcosmicscore_flashmatch_pmtrajfit", trkcosmicscore_flashmatch_pmtrajfit, &b_trkcosmicscore_flashmatch_pmtrajfit);
   fChain->SetBranchAddress("trkcosmictype_flashmatch_pmtrajfit", trkcosmictype_flashmatch_pmtrajfit, &b_trkcosmictype_flashmatch_pmtrajfit);
   fChain->SetBranchAddress("trkke_pmtrajfit", trkke_pmtrajfit, &b_trkke_pmtrajfit);
   fChain->SetBranchAddress("trkrange_pmtrajfit", trkrange_pmtrajfit, &b_trkrange_pmtrajfit);
   fChain->SetBranchAddress("trkidtruth_pmtrajfit", trkidtruth_pmtrajfit, &b_trkidtruth_pmtrajfit);
   fChain->SetBranchAddress("trkorigin_pmtrajfit", trkorigin_pmtrajfit, &b_trkorigin_pmtrajfit);
   fChain->SetBranchAddress("trkpdgtruth_pmtrajfit", trkpdgtruth_pmtrajfit, &b_trkpdgtruth_pmtrajfit);
   fChain->SetBranchAddress("trkefftruth_pmtrajfit", trkefftruth_pmtrajfit, &b_trkefftruth_pmtrajfit);
   fChain->SetBranchAddress("trkpurtruth_pmtrajfit", trkpurtruth_pmtrajfit, &b_trkpurtruth_pmtrajfit);
   fChain->SetBranchAddress("trkpitchc_pmtrajfit", trkpitchc_pmtrajfit, &b_trkpitchc_pmtrajfit);
   fChain->SetBranchAddress("ntrkhits_pmtrajfit", ntrkhits_pmtrajfit, &b_ntrkhits_pmtrajfit);
   fChain->SetBranchAddress("trkdedx_pmtrajfit", trkdedx_pmtrajfit, &b_trkdedx_pmtrajfit);
   fChain->SetBranchAddress("trkdqdx_pmtrajfit", trkdqdx_pmtrajfit, &b_trkdqdx_pmtrajfit);
   fChain->SetBranchAddress("trkresrg_pmtrajfit", trkresrg_pmtrajfit, &b_trkresrg_pmtrajfit);
   fChain->SetBranchAddress("trktpc_pmtrajfit", trktpc_pmtrajfit, &b_trktpc_pmtrajfit);
   fChain->SetBranchAddress("trkxyz_pmtrajfit", trkxyz_pmtrajfit, &b_trkxyz_pmtrajfit);
   fChain->SetBranchAddress("trkstartx_pmtrajfit", trkstartx_pmtrajfit, &b_trkstartx_pmtrajfit);
   fChain->SetBranchAddress("trkstarty_pmtrajfit", trkstarty_pmtrajfit, &b_trkstarty_pmtrajfit);
   fChain->SetBranchAddress("trkstartz_pmtrajfit", trkstartz_pmtrajfit, &b_trkstartz_pmtrajfit);
   fChain->SetBranchAddress("trkstartd_pmtrajfit", trkstartd_pmtrajfit, &b_trkstartd_pmtrajfit);
   fChain->SetBranchAddress("trkendx_pmtrajfit", trkendx_pmtrajfit, &b_trkendx_pmtrajfit);
   fChain->SetBranchAddress("trkendy_pmtrajfit", trkendy_pmtrajfit, &b_trkendy_pmtrajfit);
   fChain->SetBranchAddress("trkendz_pmtrajfit", trkendz_pmtrajfit, &b_trkendz_pmtrajfit);
   fChain->SetBranchAddress("trkendd_pmtrajfit", trkendd_pmtrajfit, &b_trkendd_pmtrajfit);
   fChain->SetBranchAddress("trkflashT0_pmtrajfit", trkflashT0_pmtrajfit, &b_trkflashT0_pmtrajfit);
   fChain->SetBranchAddress("trktrueT0_pmtrajfit", trktrueT0_pmtrajfit, &b_trktrueT0_pmtrajfit);
   fChain->SetBranchAddress("trkg4id_pmtrajfit", trkg4id_pmtrajfit, &b_trkg4id_pmtrajfit);
   fChain->SetBranchAddress("trkorig_pmtrajfit", trkorig_pmtrajfit, &b_trkorig_pmtrajfit);
   fChain->SetBranchAddress("trkpurity_pmtrajfit", trkpurity_pmtrajfit, &b_trkpurity_pmtrajfit);
   fChain->SetBranchAddress("trkcompleteness_pmtrajfit", trkcompleteness_pmtrajfit, &b_trkcompleteness_pmtrajfit);
   fChain->SetBranchAddress("trktheta_pmtrajfit", trktheta_pmtrajfit, &b_trktheta_pmtrajfit);
   fChain->SetBranchAddress("trkphi_pmtrajfit", trkphi_pmtrajfit, &b_trkphi_pmtrajfit);
   fChain->SetBranchAddress("trkstartdcosx_pmtrajfit", trkstartdcosx_pmtrajfit, &b_trkstartdcosx_pmtrajfit);
   fChain->SetBranchAddress("trkstartdcosy_pmtrajfit", trkstartdcosy_pmtrajfit, &b_trkstartdcosy_pmtrajfit);
   fChain->SetBranchAddress("trkstartdcosz_pmtrajfit", trkstartdcosz_pmtrajfit, &b_trkstartdcosz_pmtrajfit);
   fChain->SetBranchAddress("trkenddcosx_pmtrajfit", trkenddcosx_pmtrajfit, &b_trkenddcosx_pmtrajfit);
   fChain->SetBranchAddress("trkenddcosy_pmtrajfit", trkenddcosy_pmtrajfit, &b_trkenddcosy_pmtrajfit);
   fChain->SetBranchAddress("trkenddcosz_pmtrajfit", trkenddcosz_pmtrajfit, &b_trkenddcosz_pmtrajfit);
   fChain->SetBranchAddress("trkthetaxz_pmtrajfit", trkthetaxz_pmtrajfit, &b_trkthetaxz_pmtrajfit);
   fChain->SetBranchAddress("trkthetayz_pmtrajfit", trkthetayz_pmtrajfit, &b_trkthetayz_pmtrajfit);
   fChain->SetBranchAddress("trkmom_pmtrajfit", trkmom_pmtrajfit, &b_trkmom_pmtrajfit);
   fChain->SetBranchAddress("trkmomrange_pmtrajfit", trkmomrange_pmtrajfit, &b_trkmomrange_pmtrajfit);
   fChain->SetBranchAddress("trkmommschi2_pmtrajfit", trkmommschi2_pmtrajfit, &b_trkmommschi2_pmtrajfit);
   fChain->SetBranchAddress("trkmommsllhd_pmtrajfit", trkmommsllhd_pmtrajfit, &b_trkmommsllhd_pmtrajfit);
   fChain->SetBranchAddress("trklen_pmtrajfit", trklen_pmtrajfit, &b_trklen_pmtrajfit);
   fChain->SetBranchAddress("trksvtxid_pmtrajfit", trksvtxid_pmtrajfit, &b_trksvtxid_pmtrajfit);
   fChain->SetBranchAddress("trkevtxid_pmtrajfit", trkevtxid_pmtrajfit, &b_trkevtxid_pmtrajfit);
   fChain->SetBranchAddress("trkpidmvamu_pmtrajfit", trkpidmvamu_pmtrajfit, &b_trkpidmvamu_pmtrajfit);
   fChain->SetBranchAddress("trkpidmvae_pmtrajfit", trkpidmvae_pmtrajfit, &b_trkpidmvae_pmtrajfit);
   fChain->SetBranchAddress("trkpidmvapich_pmtrajfit", trkpidmvapich_pmtrajfit, &b_trkpidmvapich_pmtrajfit);
   fChain->SetBranchAddress("trkpidmvapi0_pmtrajfit", trkpidmvapi0_pmtrajfit, &b_trkpidmvapi0_pmtrajfit);
   fChain->SetBranchAddress("trkpidmvapr_pmtrajfit", trkpidmvapr_pmtrajfit, &b_trkpidmvapr_pmtrajfit);
   fChain->SetBranchAddress("trkpidpdg_pmtrajfit", trkpidpdg_pmtrajfit, &b_trkpidpdg_pmtrajfit);
   fChain->SetBranchAddress("trkpidchi_pmtrajfit", trkpidchi_pmtrajfit, &b_trkpidchi_pmtrajfit);
   fChain->SetBranchAddress("trkpidchipr_pmtrajfit", trkpidchipr_pmtrajfit, &b_trkpidchipr_pmtrajfit);
   fChain->SetBranchAddress("trkpidchika_pmtrajfit", trkpidchika_pmtrajfit, &b_trkpidchika_pmtrajfit);
   fChain->SetBranchAddress("trkpidchipi_pmtrajfit", trkpidchipi_pmtrajfit, &b_trkpidchipi_pmtrajfit);
   fChain->SetBranchAddress("trkpidchimu_pmtrajfit", trkpidchimu_pmtrajfit, &b_trkpidchimu_pmtrajfit);
   fChain->SetBranchAddress("trkpidpida_pmtrajfit", trkpidpida_pmtrajfit, &b_trkpidpida_pmtrajfit);
   fChain->SetBranchAddress("trkpidbestplane_pmtrajfit", trkpidbestplane_pmtrajfit, &b_trkpidbestplane_pmtrajfit);
   fChain->SetBranchAddress("trkhasPFParticle_pmtrajfit", trkhasPFParticle_pmtrajfit, &b_trkhasPFParticle_pmtrajfit);
   fChain->SetBranchAddress("trkPFParticleID_pmtrajfit", trkPFParticleID_pmtrajfit, &b_trkPFParticleID_pmtrajfit);
   /*
   fChain->SetBranchAddress("nvtx_linecluster", &nvtx_linecluster, &b_nvtx_linecluster);
   fChain->SetBranchAddress("vtxId_linecluster", vtxId_linecluster, &b_vtxId_linecluster);
   fChain->SetBranchAddress("vtxx_linecluster", vtxx_linecluster, &b_vtxx_linecluster);
   fChain->SetBranchAddress("vtxy_linecluster", vtxy_linecluster, &b_vtxy_linecluster);
   fChain->SetBranchAddress("vtxz_linecluster", vtxz_linecluster, &b_vtxz_linecluster);
   fChain->SetBranchAddress("vtxhasPFParticle_linecluster", vtxhasPFParticle_linecluster, &b_vtxhasPFParticle_linecluster);
   fChain->SetBranchAddress("vtxPFParticleID_linecluster", vtxPFParticleID_linecluster, &b_vtxPFParticleID_linecluster);
   fChain->SetBranchAddress("nvtx_lineclusterdc", &nvtx_lineclusterdc, &b_nvtx_lineclusterdc);
   fChain->SetBranchAddress("vtxId_lineclusterdc", &vtxId_lineclusterdc, &b_vtxId_lineclusterdc);
   fChain->SetBranchAddress("vtxx_lineclusterdc", &vtxx_lineclusterdc, &b_vtxx_lineclusterdc);
   fChain->SetBranchAddress("vtxy_lineclusterdc", &vtxy_lineclusterdc, &b_vtxy_lineclusterdc);
   fChain->SetBranchAddress("vtxz_lineclusterdc", &vtxz_lineclusterdc, &b_vtxz_lineclusterdc);
   fChain->SetBranchAddress("vtxhasPFParticle_lineclusterdc", &vtxhasPFParticle_lineclusterdc, &b_vtxhasPFParticle_lineclusterdc);
   fChain->SetBranchAddress("vtxPFParticleID_lineclusterdc", &vtxPFParticleID_lineclusterdc, &b_vtxPFParticleID_lineclusterdc);
   fChain->SetBranchAddress("nvtx_pmtrack", &nvtx_pmtrack, &b_nvtx_pmtrack);
   fChain->SetBranchAddress("vtxId_pmtrack", vtxId_pmtrack, &b_vtxId_pmtrack);
   fChain->SetBranchAddress("vtxx_pmtrack", vtxx_pmtrack, &b_vtxx_pmtrack);
   fChain->SetBranchAddress("vtxy_pmtrack", vtxy_pmtrack, &b_vtxy_pmtrack);
   fChain->SetBranchAddress("vtxz_pmtrack", vtxz_pmtrack, &b_vtxz_pmtrack);
   fChain->SetBranchAddress("vtxhasPFParticle_pmtrack", vtxhasPFParticle_pmtrack, &b_vtxhasPFParticle_pmtrack);
   fChain->SetBranchAddress("vtxPFParticleID_pmtrack", vtxPFParticleID_pmtrack, &b_vtxPFParticleID_pmtrack);
   fChain->SetBranchAddress("nvtx_pmtrackdc", &nvtx_pmtrackdc, &b_nvtx_pmtrackdc);
   fChain->SetBranchAddress("vtxId_pmtrackdc", &vtxId_pmtrackdc, &b_vtxId_pmtrackdc);
   fChain->SetBranchAddress("vtxx_pmtrackdc", &vtxx_pmtrackdc, &b_vtxx_pmtrackdc);
   fChain->SetBranchAddress("vtxy_pmtrackdc", &vtxy_pmtrackdc, &b_vtxy_pmtrackdc);
   fChain->SetBranchAddress("vtxz_pmtrackdc", &vtxz_pmtrackdc, &b_vtxz_pmtrackdc);
   fChain->SetBranchAddress("vtxhasPFParticle_pmtrackdc", &vtxhasPFParticle_pmtrackdc, &b_vtxhasPFParticle_pmtrackdc);
   fChain->SetBranchAddress("vtxPFParticleID_pmtrackdc", &vtxPFParticleID_pmtrackdc, &b_vtxPFParticleID_pmtrackdc);
   fChain->SetBranchAddress("nvtx_pandora", &nvtx_pandora, &b_nvtx_pandora);
   fChain->SetBranchAddress("vtxId_pandora", vtxId_pandora, &b_vtxId_pandora);
   fChain->SetBranchAddress("vtxx_pandora", vtxx_pandora, &b_vtxx_pandora);
   fChain->SetBranchAddress("vtxy_pandora", vtxy_pandora, &b_vtxy_pandora);
   fChain->SetBranchAddress("vtxz_pandora", vtxz_pandora, &b_vtxz_pandora);
   fChain->SetBranchAddress("vtxhasPFParticle_pandora", vtxhasPFParticle_pandora, &b_vtxhasPFParticle_pandora);
   fChain->SetBranchAddress("vtxPFParticleID_pandora", vtxPFParticleID_pandora, &b_vtxPFParticleID_pandora);
   fChain->SetBranchAddress("nvtx_pandoradc", &nvtx_pandoradc, &b_nvtx_pandoradc);
   fChain->SetBranchAddress("vtxId_pandoradc", &vtxId_pandoradc, &b_vtxId_pandoradc);
   fChain->SetBranchAddress("vtxx_pandoradc", &vtxx_pandoradc, &b_vtxx_pandoradc);
   fChain->SetBranchAddress("vtxy_pandoradc", &vtxy_pandoradc, &b_vtxy_pandoradc);
   fChain->SetBranchAddress("vtxz_pandoradc", &vtxz_pandoradc, &b_vtxz_pandoradc);
   fChain->SetBranchAddress("vtxhasPFParticle_pandoradc", &vtxhasPFParticle_pandoradc, &b_vtxhasPFParticle_pandoradc);
   fChain->SetBranchAddress("vtxPFParticleID_pandoradc", &vtxPFParticleID_pandoradc, &b_vtxPFParticleID_pandoradc);
   */
  return true;
}

//********************************************************************
anaTreeNewConverter::~anaTreeNewConverter(){
//********************************************************************

  if (!fChain) return;

  if (eventsTree         ) delete   eventsTree          ->GetCurrentFile();
}

//****************************************************************************
bool anaTreeNewConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "anaTreeNewConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

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
    std::cout << "anaTreeNewConverter::AddFileToTChain(). Current file has the same run and subrun as the previous" << std::endl;
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
  _isMC = (geant_list_size!=0); 
  if (!header().SetIsMC(_isMC)) return false;

  _softwareVersion = "v0r0";

  // Sets the software version for this file
  return header().SetSoftwareVersion(_softwareVersion);
}


//*****************************************************************************
Int_t anaTreeNewConverter::ReadEntries(Long64_t& entry) {
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
Int_t anaTreeNewConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
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
void anaTreeNewConverter::IncrementPOTBySpill() {
//********************************************************************
  
//  bool bySpillInMC = false;
  
  //  if (!_isMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
  //    anaUtils::IncrementPOTBySpill(*_spill,header());
}

//*****************************************************************************
void anaTreeNewConverter::FillInfo(AnaSpill* spill){
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
void anaTreeNewConverter::FillDQInfo(AnaDataQuality* dq){
//*****************************************************************************

    dq->GoodDaq   = true;
}

//*****************************************************************************
void anaTreeNewConverter::FillBeamInfo(AnaBeam* beam){
//*****************************************************************************

    beam->GoodSpill = true;
}

//*****************************************************************************
void anaTreeNewConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  trigger->nTriggers = triggernumber;
  anaUtils::CreateArray(trigger->Time, triggernumber);
  anaUtils::CreateArray(trigger->ID,   triggernumber);


  //  for (UInt_t i=0;i<triggernumber;i++){
    //    trigger->Time[i] = triggertime[i];
    //    trigger->ID[i]   = triggerbits[i];
  //  }
}

//*****************************************************************************
void anaTreeNewConverter::FillTrueInfo(AnaSpill* spill){
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
    FillTrueParticleInfo(spill->TrueVertices, spill->TrueParticles, i, truePart);
    spill->TrueParticles.push_back(truePart);    
  }


}

//*****************************************************************************
void anaTreeNewConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
  bunch->Particles.clear();
  bunch->Vertices.clear();

  for (Int_t i=0;i<ntracks_pmtrajfit;i++){
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
void anaTreeNewConverter::FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  (void)ivertex;

  // TODO

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
}


//*****************************************************************************
void anaTreeNewConverter::FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part){
//*****************************************************************************

  part->UniqueID  = trkId_pmtrajfit[itrk];

  part->NHits = 0;
  for (Int_t i=0;i<3;i++){
    part->NHitsPerPlane[i] = ntrkhits_pmtrajfit[itrk][i];
    part->NHits += part->NHitsPerPlane[i];
  }
  
  SubDetId::SetDetectorUsed(part->Detector , SubDetId::kSubdet1_1);

  part->PositionEnd[0]  = trkstartx_pmtrajfit[itrk];
  part->PositionEnd[1]  = trkstarty_pmtrajfit[itrk];
  part->PositionEnd[2]  = trkstartz_pmtrajfit[itrk];

  part->PositionStart[0]  = trkendx_pmtrajfit[itrk];
  part->PositionStart[1]  = trkendy_pmtrajfit[itrk];
  part->PositionStart[2]  = trkendz_pmtrajfit[itrk];

  part->DirectionEnd[0] = trkstartdcosx_pmtrajfit[itrk];
  part->DirectionEnd[1] = trkstartdcosy_pmtrajfit[itrk];
  part->DirectionEnd[2] = trkstartdcosz_pmtrajfit[itrk];

  part->DirectionStart[0] = trkenddcosx_pmtrajfit[itrk];
  part->DirectionStart[1] = trkenddcosy_pmtrajfit[itrk];
  part->DirectionStart[2] = trkenddcosz_pmtrajfit[itrk];

  part->Length = trklen_pmtrajfit[itrk];

  part->Momentum = trkmomrange_pmtrajfit[itrk];

  part->AveragedEdx=0;
  part->AveragedQdx=0;
  Int_t ncontrib=0;
  for (Int_t i=0;i<3;i++){
    for (Int_t j=0;j<part->NHitsPerPlane[i];j++){
      if (trkdedx_pmtrajfit[itrk][i][j]>0){
        part->AveragedEdx += trkdedx_pmtrajfit[itrk][i][j];
        part->AveragedQdx += trkdqdx_pmtrajfit[itrk][i][j];
        part->dEdx[i][j]=trkdedx_pmtrajfit[itrk][i][j];
        part->ResidualRange[i][j]=trkresrg_pmtrajfit[itrk][i][j];
        ncontrib++;
      }
    }
  }

  part->AveragedEdx /= 1.*ncontrib;
  part->AveragedQdx /= 1.*ncontrib;


  /*  
  part->PID[0] = trkpidchimu_pmtrajfit[itrk][0];
  part->PID[1] = trkpidchika_pmtrajfit[itrk][0];
  part->PID[2] = trkpidchipr_pmtrajfit[itrk][0];
  part->PID[3] = trkpidchipi_pmtrajfit[itrk][0];
  part->PID[4] = trkpidpida_pmtrajfit[itrk][0];
  part->PID[5] = trkpidpdg_pmtrajfit[itrk][0];
  */

  part->TrueObject= FindTrueParticle(itrk,trueParticles);

  part->TrueEff=0;
  part->TruePur=0;
  for (Int_t i=0;i<3;i++){
    part->TrueEff += trkefftruth_pmtrajfit[itrk][i]/3.;
    part->TruePur += trkpurtruth_pmtrajfit[itrk][i]/3.;
  }
}

//*****************************************************************************
AnaTrueObjectC* anaTreeNewConverter::FindTrueParticle(Int_t itrk, std::vector<AnaTrueParticleB*>& trueParticles){
//*****************************************************************************

  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == trkg4id_pmtrajfit[itrk]){
      return trueParticles[i];
    }
  }

  return NULL;
}

//*****************************************************************************
void anaTreeNewConverter::FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, std::vector<AnaTrueParticleB*>& trueParticles, Int_t ipart, AnaTrueParticle* truePart){
//*****************************************************************************

    truePart->ID = TrackId[ipart];

    //    truePart->Charge = ;

    truePart->Momentum    = StartP_drifted[ipart];
    truePart->MomentumEnd = EndP_drifted[ipart];
    if (StartP_drifted[ipart]>0){
      truePart->Direction[0] = StartPx_drifted[ipart]/StartP_drifted[ipart];
      truePart->Direction[1] = StartPy_drifted[ipart]/StartP_drifted[ipart];
      truePart->Direction[2] = StartPz_drifted[ipart]/StartP_drifted[ipart];
    }
    if (EndP_drifted[ipart]>0){
      truePart->DirectionEnd[0] = EndPx_drifted[ipart]/EndP_drifted[ipart];
      truePart->DirectionEnd[1] = EndPy_drifted[ipart]/EndP_drifted[ipart];
      truePart->DirectionEnd[2] = EndPz_drifted[ipart]/EndP_drifted[ipart];
    }

    truePart->Position[0] = StartPointx_drifted[ipart];
    truePart->Position[1] = StartPointy_drifted[ipart];
    truePart->Position[2] = StartPointz_drifted[ipart];
    truePart->Position[3] = StartT_drifted[ipart];

    truePart->PositionEnd[0] = EndPointx_drifted[ipart];
    truePart->PositionEnd[1] = EndPointy_drifted[ipart];
    truePart->PositionEnd[2] = EndPointz_drifted[ipart];
    truePart->PositionEnd[3] = EndT_drifted[ipart];

    truePart->PDG = pdg[ipart];

    truePart->ProcessStart = truePart->ConvertProcess((*processname)[ipart]);      

    truePart->ParentPDG = 0;
    Int_t GParentID=-1;
    for (Int_t i=0;i<geant_list_size;i++){
      if (TrackId[i] == Mother[ipart]){
        truePart->ParentPDG = pdg[i];      
        truePart->ParentID = TrackId[i];
        GParentID = Mother[i];

        for (UInt_t j=0;j<trueParticles.size();j++){
          if (trueParticles[j]->ID == Mother[ipart]){
            trueParticles[j]->ProcessEnd = truePart->ConvertProcess((*processname)[ipart]);      
            break;
          }
        }


        break;
      }
    }

    truePart->GParentPDG = 0;
    for (Int_t i=0;i<geant_list_size;i++){
      if (TrackId[i] == GParentID){
        truePart->GParentPDG = pdg[i];      
        break;
      }
    }

    if (process_primary[ipart]){
      truePart->TrueVertex = trueVertices[0];

      anaUtils::CreateArray(truePart->TrueVertex->TrueParticles, 1);
      truePart->TrueVertex->nTrueParticles = 1;	
      truePart->TrueVertex->TrueParticles[0]= truePart;
    }
}
