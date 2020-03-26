/*
   converter for DUNE AnaTree input format

   A. Cervera February 2020
*/

#ifndef pionTreeConverter_h
#define pionTreeConverter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TRef.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <set>
#include "InputConverter.hxx"
#include "DataClasses.hxx"
#include "GeometryManager.hxx"

/*
const unsigned int NMAXHITS      = 3000;
const unsigned int NMAXCLUSTERS  = 200;
const unsigned int NMAXFLUSHES   = 10;
const unsigned int NMAXTRUEPARTS = 1000;
const unsigned int NMAXTRACKS    = 50;
const unsigned int NMAXVTXS      = 50; 
*/
class pionTreeConverter: public InputConverter{

 public:

  pionTreeConverter();
  virtual ~pionTreeConverter();

  virtual bool Initialize();
  virtual Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event){(void)entry;(void)event; return 0;}

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  virtual Int_t ReadEntries(Long64_t& entry);
  virtual bool AddFileToTChain(const std::string& inputString);

  //----------------
  virtual AnaSpillB*        MakeSpill()       { return new AnaSpill(); }
  virtual AnaBunch*         MakeBunch()       { return new AnaBunch(); }
  virtual AnaBeamB*         MakeBeam()        { return new AnaBeam(); }
  virtual AnaDataQualityB*  MakeDataQuality() { return new AnaDataQuality(); }
  virtual AnaEventInfoB*    MakeEventInfo()   { return new AnaEventInfo(); }
  virtual AnaTrigger*       MakeTrigger()     { return new AnaTrigger(); }

  virtual AnaTrueParticle*  MakeTrueParticle(){ return new AnaTrueParticle(); }
  virtual AnaTrueVertex*    MakeTrueVertex()  { return new AnaTrueVertex(); }
  virtual AnaParticle*      MakeParticle()    { return new AnaParticle(); }

  // ----------------------------

  virtual void FillInfo(AnaSpill* spill);
  virtual void FillBeamInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBeam* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);
  virtual void FillDQInfo(AnaDataQuality* dq);
  virtual void FillTrueInfo(AnaSpill* spill);
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch);
  virtual void FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex);

  virtual void FillBeamParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaParticle* part);
  virtual void FillDaughterParticleTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part);
  virtual void FillDaughterParticleShowerInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part);

  virtual void FillBeamTrueParticleInfo(AnaTrueParticle* truePart);
  virtual void FillDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart);

  virtual void FillTrueBeamTrueParticleInfo(AnaTrueParticle* truePart);
  virtual void FillTrueBeamDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart, AnaTrueParticle* parentPart);
  virtual void FillTrueBeamGrandDaughterTrueParticleInfo(Int_t ipart, AnaTrueParticle* truePart, AnaTrueParticle* parent);
  
  AnaTrueObjectC* FindTrueParticle(Int_t itrk, std::vector<AnaTrueParticleB*>& trueParticles);

protected:


  AnaSpill* _spill;
  
  std::string _previousFile;
  Int_t _previousRunID;
  Int_t _previousSubrunID;
  Int_t _previousRefEventID;

 protected:

  // TChains   
  TChain *eventsTree;
  TChain *FileIndexTree;

  Int_t Entries; 
  Int_t Counter; 

  Bool_t _isMC;
  std::string _softwareVersion;

   // Declaration of leaf types
   Int_t           run;
   Int_t           subrun;
   Int_t           event;
   Int_t           MC;
   Int_t           reco_beam_type;
   Double_t        reco_beam_startX;
   Double_t        reco_beam_startY;
   Double_t        reco_beam_startZ;
   Double_t        reco_beam_endX;
   Double_t        reco_beam_endY;
   Double_t        reco_beam_endZ;
   Double_t        reco_beam_len;
   Double_t        reco_beam_trackDirX;
   Double_t        reco_beam_trackDirY;
   Double_t        reco_beam_trackDirZ;
   Double_t        reco_beam_trackEndDirX;
   Double_t        reco_beam_trackEndDirY;
   Double_t        reco_beam_trackEndDirZ;
   Double_t        reco_beam_vtxX;
   Double_t        reco_beam_vtxY;
   Double_t        reco_beam_vtxZ;
   Int_t           reco_beam_trackID;
   std::vector<double>  *reco_beam_dQdX;
   std::vector<double>  *reco_beam_dEdX;
   std::vector<double>  *reco_beam_calibrated_dEdX;
   std::vector<double>  *reco_beam_resRange;
   std::vector<double>  *reco_beam_TrkPitch;
   std::vector<double>  *reco_beam_calo_wire;
   std::vector<double>  *reco_beam_calo_tick;
   Int_t           reco_beam_nTrackDaughters;
   Int_t           reco_beam_nShowerDaughters;
   Bool_t          reco_beam_flipped;
   Bool_t          reco_beam_passes_beam_cuts;
   Int_t           reco_beam_PFP_ID;
   Int_t           reco_beam_PFP_nHits;
   Double_t        reco_beam_PFP_trackScore;
   Double_t        reco_beam_PFP_emScore;
   Double_t        reco_beam_PFP_michelScore;
   Double_t        reco_beam_PFP_trackScore_collection;
   Double_t        reco_beam_PFP_emScore_collection;
   Double_t        reco_beam_PFP_michelScore_collection;
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
   std::vector<int>     *reco_daughter_PFP_true_byHits_PDG;
   std::vector<int>     *reco_daughter_PFP_true_byHits_ID;
   std::vector<int>     *reco_daughter_PFP_true_byHits_origin;
   std::vector<int>     *reco_daughter_PFP_true_byHits_parID;
   std::vector<int>     *reco_daughter_PFP_true_byHits_parPDG;
   std::vector<std::string>  *reco_daughter_PFP_true_byHits_process;
   std::vector<unsigned long> *reco_daughter_PFP_true_byHits_sharedHits;
   std::vector<unsigned long> *reco_daughter_PFP_true_byHits_emHits;
   std::vector<double>  *reco_daughter_PFP_true_byHits_len;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startX;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startY;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startZ;
   std::vector<double>  *reco_daughter_PFP_true_byHits_endX;
   std::vector<double>  *reco_daughter_PFP_true_byHits_endY;
   std::vector<double>  *reco_daughter_PFP_true_byHits_endZ;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startPx;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startPy;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startPz;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startP;
   std::vector<double>  *reco_daughter_PFP_true_byHits_startE;
   std::vector<std::string>  *reco_daughter_PFP_true_byHits_endProcess;
   std::vector<double>  *reco_daughter_PFP_true_byHits_purity;
   std::vector<int>     *reco_daughter_allTrack_ID;
   std::vector<std::vector<double> > *reco_daughter_allTrack_dEdX;
   std::vector<std::vector<double> > *reco_daughter_allTrack_dQdX;
   std::vector<std::vector<double> > *reco_daughter_allTrack_resRange;
   std::vector<std::vector<double> > *reco_daughter_allTrack_dQdX_SCE;
   std::vector<std::vector<double> > *reco_daughter_allTrack_dEdX_SCE;
   std::vector<std::vector<double> > *reco_daughter_allTrack_resRange_SCE;
   std::vector<std::vector<double> > *reco_daughter_allTrack_calibrated_dEdX;
   std::vector<std::vector<double> > *reco_daughter_allTrack_calibrated_dEdX_SCE;
   std::vector<double>  *reco_daughter_allTrack_Chi2_proton;
   std::vector<int>     *reco_daughter_allTrack_Chi2_ndof;
   std::vector<double>  *reco_daughter_allTrack_Theta;
   std::vector<double>  *reco_daughter_allTrack_Phi;
   std::vector<double>  *reco_daughter_allTrack_len;
   std::vector<double>  *reco_daughter_allTrack_startX;
   std::vector<double>  *reco_daughter_allTrack_startY;
   std::vector<double>  *reco_daughter_allTrack_startZ;
   std::vector<double>  *reco_daughter_allTrack_endX;
   std::vector<double>  *reco_daughter_allTrack_endY;
   std::vector<double>  *reco_daughter_allTrack_endZ;
   std::vector<double>  *reco_daughter_allTrack_dR;
   std::vector<double>  *reco_daughter_allTrack_to_vertex;
   std::vector<int>     *reco_daughter_allShower_ID;
   std::vector<double>  *reco_daughter_allShower_len;
   std::vector<double>  *reco_daughter_allShower_startX;
   std::vector<double>  *reco_daughter_allShower_startY;
   std::vector<double>  *reco_daughter_allShower_startZ;
   std::vector<int>     *reco_daughter_PFP_ID;
   std::vector<int>     *reco_daughter_PFP_nHits;
   std::vector<double>  *reco_daughter_PFP_trackScore;
   std::vector<double>  *reco_daughter_PFP_emScore;
   std::vector<double>  *reco_daughter_PFP_michelScore;
   std::vector<double>  *reco_daughter_PFP_trackScore_collection;
   std::vector<double>  *reco_daughter_PFP_emScore_collection;
   std::vector<double>  *reco_daughter_PFP_michelScore_collection;
   Int_t           true_beam_PDG;
   Int_t           true_beam_ID;
   std::string          *true_beam_endProcess;
   Double_t        true_beam_endX;
   Double_t        true_beam_endY;
   Double_t        true_beam_endZ;
   Double_t        true_beam_startX;
   Double_t        true_beam_startY;
   Double_t        true_beam_startZ;
   Double_t        true_beam_startPx;
   Double_t        true_beam_startPy;
   Double_t        true_beam_startPz;
   Double_t        true_beam_startP;
   Double_t        true_beam_endPx;
   Double_t        true_beam_endPy;
   Double_t        true_beam_endPz;
   Double_t        true_beam_endP;
   Double_t        true_beam_startDirX;
   Double_t        true_beam_startDirY;
   Double_t        true_beam_startDirZ;
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
   Int_t           true_daughter_nPi0;
   Int_t           true_daughter_nPiPlus;
   Int_t           true_daughter_nProton;
   Int_t           true_daughter_nNeutron;
   Int_t           true_daughter_nPiMinus;
   Int_t           true_daughter_nNucleus;
   Int_t           reco_beam_vertex_slice;
   std::vector<std::vector<double> > *reco_beam_vertex_dRs;
   std::vector<int>     *reco_beam_vertex_hits_slices;
   std::vector<int>     *true_beam_daughter_PDG;
   std::vector<int>     *true_beam_daughter_ID;
   std::vector<double>  *true_beam_daughter_len;
   std::vector<double>  *true_beam_daughter_startX;
   std::vector<double>  *true_beam_daughter_startY;
   std::vector<double>  *true_beam_daughter_startZ;
   std::vector<double>  *true_beam_daughter_startPx;
   std::vector<double>  *true_beam_daughter_startPy;
   std::vector<double>  *true_beam_daughter_startPz;
   std::vector<double>  *true_beam_daughter_startP;
   std::vector<double>  *true_beam_daughter_endX;
   std::vector<double>  *true_beam_daughter_endY;
   std::vector<double>  *true_beam_daughter_endZ;
   std::vector<std::string>  *true_beam_daughter_Process;
   std::vector<std::string>  *true_beam_daughter_endProcess;
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
   std::vector<int>     *true_beam_grand_daughter_ID;
   std::vector<int>     *true_beam_grand_daughter_parID;
   std::vector<int>     *true_beam_grand_daughter_PDG;
   std::vector<int>     *true_beam_grand_daughter_nHits;
   std::vector<std::string>  *true_beam_grand_daughter_Process;
   std::vector<std::string>  *true_beam_grand_daughter_endProcess;
   std::string          *reco_beam_true_byE_endProcess;
   std::string          *reco_beam_true_byE_process;
   Int_t           reco_beam_true_byE_origin;
   Int_t           reco_beam_true_byE_PDG;
   Int_t           reco_beam_true_byE_ID;
   std::string          *reco_beam_true_byHits_endProcess;
   std::string          *reco_beam_true_byHits_process;
   Int_t           reco_beam_true_byHits_origin;
   Int_t           reco_beam_true_byHits_PDG;
   Int_t           reco_beam_true_byHits_ID;
   Bool_t          reco_beam_true_byE_matched;
   Bool_t          reco_beam_true_byHits_matched;
   Double_t        reco_beam_true_byHits_purity;
   std::vector<std::string>  *true_beam_processes;
   Double_t        data_BI_P;
   Double_t        data_BI_X;
   Double_t        data_BI_Y;
   Double_t        data_BI_Z;
   Double_t        data_BI_dirX;
   Double_t        data_BI_dirY;
   Double_t        data_BI_dirZ;
   Int_t           data_BI_nFibersP1;
   Int_t           data_BI_nFibersP2;
   Int_t           data_BI_nFibersP3;
   std::vector<int>     *data_BI_PDG_candidates;
   Int_t           data_BI_nTracks;
   Int_t           data_BI_nMomenta;
   Bool_t          quality_reco_view_0_hits_in_TPC5;
   Bool_t          quality_reco_view_1_hits_in_TPC5;
   Bool_t          quality_reco_view_2_hits_in_TPC5;
   Double_t        quality_reco_max_lateral;
   Double_t        quality_reco_max_segment;
   Double_t        quality_reco_view_0_max_segment;
   Double_t        quality_reco_view_1_max_segment;
   Double_t        quality_reco_view_2_max_segment;
   Double_t        quality_reco_view_0_wire_backtrack;
   Double_t        quality_reco_view_1_wire_backtrack;
   Double_t        quality_reco_view_2_wire_backtrack;
   std::vector<double>  *quality_reco_view_0_wire;
   std::vector<double>  *quality_reco_view_1_wire;
   std::vector<double>  *quality_reco_view_2_wire;
   std::vector<double>  *quality_reco_view_2_z;
   std::vector<double>  *quality_reco_view_0_tick;
   std::vector<double>  *quality_reco_view_1_tick;
   std::vector<double>  *quality_reco_view_2_tick;
   Double_t        reco_beam_Chi2_proton;
   Int_t           reco_beam_Chi2_ndof;
   std::vector<double>  *reco_beam_cosmic_candidate_lower_hits;
   std::vector<double>  *reco_beam_cosmic_candidate_upper_hits;
   std::vector<int>     *reco_beam_cosmic_candidate_ID;
   Bool_t          beam_has_cosmic_IDE;
   std::vector<int>     *cosmic_has_beam_IDE;
   Int_t           n_cosmics_with_beam_IDE;
   Double_t        reco_beam_true_byE_endPx;
   Double_t        reco_beam_true_byE_endPy;
   Double_t        reco_beam_true_byE_endPz;
   Double_t        reco_beam_true_byE_endE;
   Double_t        reco_beam_true_byE_endP;
   Double_t        reco_beam_true_byE_startPx;
   Double_t        reco_beam_true_byE_startPy;
   Double_t        reco_beam_true_byE_startPz;
   Double_t        reco_beam_true_byE_startE;
   Double_t        reco_beam_true_byE_startP;
   Double_t        reco_beam_true_byHits_endPx;
   Double_t        reco_beam_true_byHits_endPy;
   Double_t        reco_beam_true_byHits_endPz;
   Double_t        reco_beam_true_byHits_endE;
   Double_t        reco_beam_true_byHits_endP;
   Double_t        reco_beam_true_byHits_startPx;
   Double_t        reco_beam_true_byHits_startPy;
   Double_t        reco_beam_true_byHits_startPz;
   Double_t        reco_beam_true_byHits_startE;
   Double_t        reco_beam_true_byHits_startP;
   std::vector<double>  *reco_beam_incidentEnergies;
   Double_t        reco_beam_interactingEnergy;
   std::vector<double>  *true_beam_incidentEnergies;
   Double_t        true_beam_interactingEnergy;
   std::vector<double>  *reco_beam_spacePts_X;
   std::vector<double>  *reco_beam_spacePts_Y;
   std::vector<double>  *reco_beam_spacePts_Z;
   std::vector<std::vector<double> > *reco_daughter_spacePts_X;
   std::vector<std::vector<double> > *reco_daughter_spacePts_Y;
   std::vector<std::vector<double> > *reco_daughter_spacePts_Z;
   std::vector<std::vector<double> > *reco_daughter_shower_spacePts_X;
   std::vector<std::vector<double> > *reco_daughter_shower_spacePts_Y;
   std::vector<std::vector<double> > *reco_daughter_shower_spacePts_Z;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!
   TBranch        *b_MC;   //!
   TBranch        *b_reco_beam_type;   //!
   TBranch        *b_reco_beam_startX;   //!
   TBranch        *b_reco_beam_startY;   //!
   TBranch        *b_reco_beam_startZ;   //!
   TBranch        *b_reco_beam_endX;   //!
   TBranch        *b_reco_beam_endY;   //!
   TBranch        *b_reco_beam_endZ;   //!
   TBranch        *b_reco_beam_len;   //!
   TBranch        *b_reco_beam_trackDirX;   //!
   TBranch        *b_reco_beam_trackDirY;   //!
   TBranch        *b_reco_beam_trackDirZ;   //!
   TBranch        *b_reco_beam_trackEndDirX;   //!
   TBranch        *b_reco_beam_trackEndDirY;   //!
   TBranch        *b_reco_beam_trackEndDirZ;   //!
   TBranch        *b_reco_beam_vtxX;   //!
   TBranch        *b_reco_beam_vtxY;   //!
   TBranch        *b_reco_beam_vtxZ;   //!
   TBranch        *b_reco_beam_trackID;   //!
   TBranch        *b_reco_beam_dQdX;   //!
   TBranch        *b_reco_beam_dEdX;   //!
   TBranch        *b_reco_beam_calibrated_dEdX;   //!
   TBranch        *b_reco_beam_resRange;   //!
   TBranch        *b_reco_beam_TrkPitch;   //!
   TBranch        *b_reco_beam_calo_wire;   //!
   TBranch        *b_reco_beam_calo_tick;   //!
   TBranch        *b_reco_beam_nTrackDaughters;   //!
   TBranch        *b_reco_beam_nShowerDaughters;   //!
   TBranch        *b_reco_beam_flipped;   //!
   TBranch        *b_reco_beam_passes_beam_cuts;   //!
   TBranch        *b_reco_beam_PFP_ID;   //!
   TBranch        *b_reco_beam_PFP_nHits;   //!
   TBranch        *b_reco_beam_PFP_trackScore;   //!
   TBranch        *b_reco_beam_PFP_emScore;   //!
   TBranch        *b_reco_beam_PFP_michelScore;   //!
   TBranch        *b_reco_beam_PFP_trackScore_collection;   //!
   TBranch        *b_reco_beam_PFP_emScore_collection;   //!
   TBranch        *b_reco_beam_PFP_michelScore_collection;   //!
   TBranch        *b_reco_beam_allTrack_ID;   //!
   TBranch        *b_reco_beam_allTrack_beam_cuts;   //!
   TBranch        *b_reco_beam_allTrack_flipped;   //!
   TBranch        *b_reco_beam_allTrack_len;   //!
   TBranch        *b_reco_beam_allTrack_startX;   //!
   TBranch        *b_reco_beam_allTrack_startY;   //!
   TBranch        *b_reco_beam_allTrack_startZ;   //!
   TBranch        *b_reco_beam_allTrack_endX;   //!
   TBranch        *b_reco_beam_allTrack_endY;   //!
   TBranch        *b_reco_beam_allTrack_endZ;   //!
   TBranch        *b_reco_beam_allTrack_trackDirX;   //!
   TBranch        *b_reco_beam_allTrack_trackDirY;   //!
   TBranch        *b_reco_beam_allTrack_trackDirZ;   //!
   TBranch        *b_reco_beam_allTrack_trackEndDirX;   //!
   TBranch        *b_reco_beam_allTrack_trackEndDirY;   //!
   TBranch        *b_reco_beam_allTrack_trackEndDirZ;   //!
   TBranch        *b_reco_beam_allTrack_resRange;   //!
   TBranch        *b_reco_beam_allTrack_calibrated_dEdX;   //!
   TBranch        *b_reco_beam_allTrack_Chi2_proton;   //!
   TBranch        *b_reco_beam_allTrack_Chi2_ndof;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_PDG;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_ID;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_origin;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_parID;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_parPDG;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_process;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_sharedHits;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_emHits;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_len;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startX;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startY;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startZ;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_endX;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_endY;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_endZ;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startPx;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startPy;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startPz;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startP;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_startE;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_endProcess;   //!
   TBranch        *b_reco_daughter_PFP_true_byHits_purity;   //!
   TBranch        *b_reco_daughter_allTrack_ID;   //!
   TBranch        *b_reco_daughter_allTrack_dEdX;   //!
   TBranch        *b_reco_daughter_allTrack_dQdX;   //!
   TBranch        *b_reco_daughter_allTrack_resRange;   //!
   TBranch        *b_reco_daughter_allTrack_dQdX_SCE;   //!
   TBranch        *b_reco_daughter_allTrack_dEdX_SCE;   //!
   TBranch        *b_reco_daughter_allTrack_resRange_SCE;   //!
   TBranch        *b_reco_daughter_allTrack_calibrated_dEdX;   //!
   TBranch        *b_reco_daughter_allTrack_calibrated_dEdX_SCE;   //!
   TBranch        *b_reco_daughter_allTrack_Chi2_proton;   //!
   TBranch        *b_reco_daughter_allTrack_Chi2_ndof;   //!
   TBranch        *b_reco_daughter_allTrack_Theta;   //!
   TBranch        *b_reco_daughter_allTrack_Phi;   //!
   TBranch        *b_reco_daughter_allTrack_len;   //!
   TBranch        *b_reco_daughter_allTrack_startX;   //!
   TBranch        *b_reco_daughter_allTrack_startY;   //!
   TBranch        *b_reco_daughter_allTrack_startZ;   //!
   TBranch        *b_reco_daughter_allTrack_endX;   //!
   TBranch        *b_reco_daughter_allTrack_endY;   //!
   TBranch        *b_reco_daughter_allTrack_endZ;   //!
   TBranch        *b_reco_daughter_allTrack_dR;   //!
   TBranch        *b_reco_daughter_allTrack_to_vertex;   //!
   TBranch        *b_reco_daughter_allShower_ID;   //!
   TBranch        *b_reco_daughter_allShower_len;   //!
   TBranch        *b_reco_daughter_allShower_startX;   //!
   TBranch        *b_reco_daughter_allShower_startY;   //!
   TBranch        *b_reco_daughter_allShower_startZ;   //!
   TBranch        *b_reco_daughter_PFP_ID;   //!
   TBranch        *b_reco_daughter_PFP_nHits;   //!
   TBranch        *b_reco_daughter_PFP_trackScore;   //!
   TBranch        *b_reco_daughter_PFP_emScore;   //!
   TBranch        *b_reco_daughter_PFP_michelScore;   //!
   TBranch        *b_reco_daughter_PFP_trackScore_collection;   //!
   TBranch        *b_reco_daughter_PFP_emScore_collection;   //!
   TBranch        *b_reco_daughter_PFP_michelScore_collection;   //!
   TBranch        *b_true_beam_PDG;   //!
   TBranch        *b_true_beam_ID;   //!
   TBranch        *b_true_beam_endProcess;   //!
   TBranch        *b_true_beam_endX;   //!
   TBranch        *b_true_beam_endY;   //!
   TBranch        *b_true_beam_endZ;   //!
   TBranch        *b_true_beam_startX;   //!
   TBranch        *b_true_beam_startY;   //!
   TBranch        *b_true_beam_startZ;   //!
   TBranch        *b_true_beam_startPx;   //!
   TBranch        *b_true_beam_startPy;   //!
   TBranch        *b_true_beam_startPz;   //!
   TBranch        *b_true_beam_startP;   //!
   TBranch        *b_true_beam_endPx;   //!
   TBranch        *b_true_beam_endPy;   //!
   TBranch        *b_true_beam_endPz;   //!
   TBranch        *b_true_beam_endP;   //!
   TBranch        *b_true_beam_startDirX;   //!
   TBranch        *b_true_beam_startDirY;   //!
   TBranch        *b_true_beam_startDirZ;   //!
   TBranch        *b_true_beam_nElasticScatters;   //!
   TBranch        *b_true_beam_elastic_costheta;   //!
   TBranch        *b_true_beam_elastic_X;   //!
   TBranch        *b_true_beam_elastic_Y;   //!
   TBranch        *b_true_beam_elastic_Z;   //!
   TBranch        *b_true_beam_IDE_totalDep;   //!
   TBranch        *b_true_beam_IDE_found_in_recoVtx;   //!
   TBranch        *b_true_beam_nHits;   //!
   TBranch        *b_true_beam_reco_byHits_PFP_ID;   //!
   TBranch        *b_true_beam_reco_byHits_PFP_nHits;   //!
   TBranch        *b_true_beam_reco_byHits_allTrack_ID;   //!
   TBranch        *b_true_daughter_nPi0;   //!
   TBranch        *b_true_daughter_nPiPlus;   //!
   TBranch        *b_true_daughter_nProton;   //!
   TBranch        *b_true_daughter_nNeutron;   //!
   TBranch        *b_true_daughter_nPiMinus;   //!
   TBranch        *b_true_daughter_nNucleus;   //!
   TBranch        *b_reco_beam_vertex_slice;   //!
   TBranch        *b_reco_beam_vertex_dRs;   //!
   TBranch        *b_reco_beam_vertex_hits_slices;   //!
   TBranch        *b_true_beam_daughter_PDG;   //!
   TBranch        *b_true_beam_daughter_ID;   //!
   TBranch        *b_true_beam_daughter_len;   //!
   TBranch        *b_true_beam_daughter_startX;   //!
   TBranch        *b_true_beam_daughter_startY;   //!
   TBranch        *b_true_beam_daughter_startZ;   //!
   TBranch        *b_true_beam_daughter_startPx;   //!
   TBranch        *b_true_beam_daughter_startPy;   //!
   TBranch        *b_true_beam_daughter_startPz;   //!
   TBranch        *b_true_beam_daughter_startP;   //!
   TBranch        *b_true_beam_daughter_endX;   //!
   TBranch        *b_true_beam_daughter_endY;   //!
   TBranch        *b_true_beam_daughter_endZ;   //!
   TBranch        *b_true_beam_daughter_Process;   //!
   TBranch        *b_true_beam_daughter_endProcess;   //!
   TBranch        *b_true_beam_daughter_nHits;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_PFP_ID;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_PFP_nHits;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_PFP_trackScore;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_ID;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_startX;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_startY;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_startZ;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_endX;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_endY;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_endZ;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allTrack_len;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allShower_ID;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allShower_startX;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allShower_startY;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allShower_startZ;   //!
   TBranch        *b_true_beam_daughter_reco_byHits_allShower_len;   //!
   TBranch        *b_true_beam_Pi0_decay_ID;   //!
   TBranch        *b_true_beam_Pi0_decay_parID;   //!
   TBranch        *b_true_beam_Pi0_decay_PDG;   //!
   TBranch        *b_true_beam_Pi0_decay_startP;   //!
   TBranch        *b_true_beam_Pi0_decay_len;   //!
   TBranch        *b_true_beam_Pi0_decay_nHits;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_PFP_ID;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_PFP_nHits;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_PFP_trackScore;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_ID;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_startX;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_startY;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_startZ;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_endX;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_endY;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_endZ;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allTrack_len;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allShower_ID;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allShower_startX;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allShower_startY;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allShower_startZ;   //!
   TBranch        *b_true_beam_Pi0_decay_reco_byHits_allShower_len;   //!
   TBranch        *b_true_beam_grand_daughter_ID;   //!
   TBranch        *b_true_beam_grand_daughter_parID;   //!
   TBranch        *b_true_beam_grand_daughter_PDG;   //!
   TBranch        *b_true_beam_grand_daughter_nHits;   //!
   TBranch        *b_true_beam_grand_daughter_Process;   //!
   TBranch        *b_true_beam_grand_daughter_endProcess;   //!
   TBranch        *b_reco_beam_true_byE_endProcess;   //!
   TBranch        *b_reco_beam_true_byE_process;   //!
   TBranch        *b_reco_beam_true_byE_origin;   //!
   TBranch        *b_reco_beam_true_byE_PDG;   //!
   TBranch        *b_reco_beam_true_byE_ID;   //!
   TBranch        *b_reco_beam_true_byHits_endProcess;   //!
   TBranch        *b_reco_beam_true_byHits_process;   //!
   TBranch        *b_reco_beam_true_byHits_origin;   //!
   TBranch        *b_reco_beam_true_byHits_PDG;   //!
   TBranch        *b_reco_beam_true_byHits_ID;   //!
   TBranch        *b_reco_beam_true_byE_matched;   //!
   TBranch        *b_reco_beam_true_byHits_matched;   //!
   TBranch        *b_reco_beam_true_byHits_purity;   //!
   TBranch        *b_true_beam_processes;   //!
   TBranch        *b_data_BI_P;   //!
   TBranch        *b_data_BI_X;   //!
   TBranch        *b_data_BI_Y;   //!
   TBranch        *b_data_BI_Z;   //!
   TBranch        *b_data_BI_dirX;   //!
   TBranch        *b_data_BI_dirY;   //!
   TBranch        *b_data_BI_dirZ;   //!
   TBranch        *b_data_BI_nFibersP1;   //!
   TBranch        *b_data_BI_nFibersP2;   //!
   TBranch        *b_data_BI_nFibersP3;   //!
   TBranch        *b_data_BI_PDG_candidates;   //!
   TBranch        *b_data_BI_nTracks;   //!
   TBranch        *b_data_BI_nMomenta;   //!
   TBranch        *b_quality_reco_view_0_hits_in_TPC5;   //!
   TBranch        *b_quality_reco_view_1_hits_in_TPC5;   //!
   TBranch        *b_quality_reco_view_2_hits_in_TPC5;   //!
   TBranch        *b_quality_reco_max_lateral;   //!
   TBranch        *b_quality_reco_max_segment;   //!
   TBranch        *b_quality_reco_view_0_max_segment;   //!
   TBranch        *b_quality_reco_view_1_max_segment;   //!
   TBranch        *b_quality_reco_view_2_max_segment;   //!
   TBranch        *b_quality_reco_view_0_wire_backtrack;   //!
   TBranch        *b_quality_reco_view_1_wire_backtrack;   //!
   TBranch        *b_quality_reco_view_2_wire_backtrack;   //!
   TBranch        *b_quality_reco_view_0_wire;   //!
   TBranch        *b_quality_reco_view_1_wire;   //!
   TBranch        *b_quality_reco_view_2_wire;   //!
   TBranch        *b_quality_reco_view_2_z;   //!
   TBranch        *b_quality_reco_view_0_tick;   //!
   TBranch        *b_quality_reco_view_1_tick;   //!
   TBranch        *b_quality_reco_view_2_tick;   //!
   TBranch        *b_reco_beam_Chi2_proton;   //!
   TBranch        *b_reco_beam_Chi2_ndof;   //!
   TBranch        *b_reco_beam_cosmic_candidate_lower_hits;   //!
   TBranch        *b_reco_beam_cosmic_candidate_upper_hits;   //!
   TBranch        *b_reco_beam_cosmic_candidate_ID;   //!
   TBranch        *b_beam_has_cosmic_IDE;   //!
   TBranch        *b_cosmic_has_beam_IDE;   //!
   TBranch        *b_n_cosmics_with_beam_IDE;   //!
   TBranch        *b_reco_beam_true_byE_endPx;   //!
   TBranch        *b_reco_beam_true_byE_endPy;   //!
   TBranch        *b_reco_beam_true_byE_endPz;   //!
   TBranch        *b_reco_beam_true_byE_endE;   //!
   TBranch        *b_reco_beam_true_byE_endP;   //!
   TBranch        *b_reco_beam_true_byE_startPx;   //!
   TBranch        *b_reco_beam_true_byE_startPy;   //!
   TBranch        *b_reco_beam_true_byE_startPz;   //!
   TBranch        *b_reco_beam_true_byE_startE;   //!
   TBranch        *b_reco_beam_true_byE_startP;   //!
   TBranch        *b_reco_beam_true_byHits_endPx;   //!
   TBranch        *b_reco_beam_true_byHits_endPy;   //!
   TBranch        *b_reco_beam_true_byHits_endPz;   //!
   TBranch        *b_reco_beam_true_byHits_endE;   //!
   TBranch        *b_reco_beam_true_byHits_endP;   //!
   TBranch        *b_reco_beam_true_byHits_startPx;   //!
   TBranch        *b_reco_beam_true_byHits_startPy;   //!
   TBranch        *b_reco_beam_true_byHits_startPz;   //!
   TBranch        *b_reco_beam_true_byHits_startE;   //!
   TBranch        *b_reco_beam_true_byHits_startP;   //!
   TBranch        *b_reco_beam_incidentEnergies;   //!
   TBranch        *b_reco_beam_interactingEnergy;   //!
   TBranch        *b_true_beam_incidentEnergies;   //!
   TBranch        *b_true_beam_interactingEnergy;   //!
   TBranch        *b_reco_beam_spacePts_X;   //!
   TBranch        *b_reco_beam_spacePts_Y;   //!
   TBranch        *b_reco_beam_spacePts_Z;   //!
   TBranch        *b_reco_daughter_spacePts_X;   //!
   TBranch        *b_reco_daughter_spacePts_Y;   //!
   TBranch        *b_reco_daughter_spacePts_Z;   //!
   TBranch        *b_reco_daughter_shower_spacePts_X;   //!
   TBranch        *b_reco_daughter_shower_spacePts_Y;   //!
   TBranch        *b_reco_daughter_shower_spacePts_Z;   //!

  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};



#endif

