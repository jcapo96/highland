/*
   converter for DUNE AnaTree input format

   A. Cervera April 2016
*/

#ifndef anaTreeNewConverter_h
#define anaTreeNewConverter_h

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

const unsigned int NMAXHITS      = 3000;
const unsigned int NMAXCLUSTERS  = 200;
const unsigned int NMAXFLUSHES   = 10;
const unsigned int NMAXTRUEPARTS = 1000;
const unsigned int NMAXTRACKS    = 50;
const unsigned int NMAXVTXS      = 50; 

class anaTreeNewConverter: public InputConverter{

 public:

  anaTreeNewConverter();
  virtual ~anaTreeNewConverter();

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
  virtual void FillBeamInfo(AnaBeam* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);
  virtual void FillDQInfo(AnaDataQuality* dq);
  virtual void FillTrueInfo(AnaSpill* spill);
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch);
  virtual void FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part);
  virtual void FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, std::vector<AnaTrueParticleB*>& trueParticles, Int_t ipart, AnaTrueParticle* truePart);
  virtual void FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex);

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
   Double_t        evttime;
   Double_t        beamtime;
   Double_t        pot;
   Char_t          isdata;
   Double_t        taulife;
   UInt_t          triggernumber;
   Double_t        triggertime;
   Double_t        beamgatetime;
   UInt_t          triggerbits;
   Double_t        potbnb;
   Double_t        potnumitgt;
   Double_t        potnumi101;
   Int_t           no_hits;
   Int_t           no_hits_stored;
   Short_t         hit_tpc[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_plane[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_wire[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_channel[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_peakT[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_charge[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_ph[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_startT[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_endT[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_rms[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_trueX[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_goodnessOfFit[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_multiplicity[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_trkid[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_trkKey[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_clusterid[NMAXHITS];   //[no_hits_stored]
   Short_t         hit_clusterKey[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_nelec[NMAXHITS];   //[no_hits_stored]
   Float_t         hit_energy[NMAXHITS];   //[no_hits_stored]
   Short_t         nclusters;
   Short_t         clusterId[NMAXCLUSTERS];   //[nclusters]
   Short_t         clusterView[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_StartCharge[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_StartAngle[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_EndCharge[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_EndAngle[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_Integral[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_IntegralAverage[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_SummedADC[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_SummedADCaverage[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_MultipleHitDensity[NMAXCLUSTERS];   //[nclusters]
   Float_t         cluster_Width[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluster_NHits[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluster_StartWire[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluster_StartTick[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluster_EndWire[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluster_EndTick[NMAXCLUSTERS];   //[nclusters]
   Short_t         cluncosmictags_tagger[NMAXCLUSTERS];   //[nclusters]
   Float_t         clucosmicscore_tagger[NMAXCLUSTERS];   //[nclusters]
   Short_t         clucosmictype_tagger[NMAXCLUSTERS];   //[nclusters]
   Int_t           no_flashes;
   Float_t         flash_time[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_pe[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_ycenter[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_zcenter[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_ywidth[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_zwidth[NMAXFLUSHES];   //[no_flashes]
   Float_t         flash_timewidth[NMAXFLUSHES];   //[no_flashes]
   Int_t           no_ExternCounts;
   Float_t         externcounts_time[1];   //[no_ExternCounts]
   Float_t         externcounts_id[1];   //[no_ExternCounts]
   Char_t          kNTracker;
   Char_t          kNVertexAlgos;
   Int_t           mcevts_truthcry;
   Int_t           cry_no_primaries;
   Int_t           cry_primaries_pdg[1];   //[cry_no_primaries]
   Float_t         cry_Eng[1];   //[cry_no_primaries]
   Float_t         cry_Px[1];   //[cry_no_primaries]
   Float_t         cry_Py[1];   //[cry_no_primaries]
   Float_t         cry_Pz[1];   //[cry_no_primaries]
   Float_t         cry_P[1];   //[cry_no_primaries]
   Float_t         cry_StartPointx[1];   //[cry_no_primaries]
   Float_t         cry_StartPointy[1];   //[cry_no_primaries]
   Float_t         cry_StartPointz[1];   //[cry_no_primaries]
   Float_t         cry_StartPointt[1];   //[cry_no_primaries]
   Int_t           cry_status_code[1];   //[cry_no_primaries]
   Float_t         cry_mass[1];   //[cry_no_primaries]
   Int_t           cry_trackID[1];   //[cry_no_primaries]
   Int_t           cry_ND[1];   //[cry_no_primaries]
   Int_t           cry_mother[1];   //[cry_no_primaries]
   Int_t           no_primaries;
   Int_t           geant_list_size;
   Int_t           geant_list_size_in_tpcAV;
   Int_t           pdg[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           status[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         Mass[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         Eng[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndE[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         Px[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         Py[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         Pz[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         P[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointx[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointy[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointz[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartT[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointx[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointy[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointz[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndT[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         theta[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         phi[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         theta_xz[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         theta_yz[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         pathlen[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           inTPCActive[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointx_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointy_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointz_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartT_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartE_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartP_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPx_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPy_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPz_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointx_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointy_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointz_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndT_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndE_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndP_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPx_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPy_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPz_tpcAV[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         pathlen_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           inTPCDrifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointx_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointy_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPointz_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartT_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartE_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartP_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPx_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPy_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         StartPz_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointx_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointy_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPointz_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndT_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndE_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndP_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPx_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPy_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Float_t         EndPz_drifted[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           NumberDaughters[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           Mother[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           TrackId[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           MergedId[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           origin[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           MCTruthIndex[NMAXTRUEPARTS];   //[geant_list_size]
   Int_t           process_primary[NMAXTRUEPARTS];   //[geant_list_size]
   std::vector<std::string>  *processname;
   Short_t         ntracks_pmtrack;
   Short_t         trkId_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkncosmictags_tagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkcosmicscore_tagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkcosmictype_tagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkncosmictags_containmenttagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkcosmicscore_containmenttagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkcosmictype_containmenttagger_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkncosmictags_flashmatch_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkcosmicscore_flashmatch_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkcosmictype_flashmatch_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkke_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkrange_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Int_t           trkidtruth_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Short_t         trkorigin_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Int_t           trkpdgtruth_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkefftruth_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpurtruth_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpitchc_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Short_t         ntrkhits_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkdedx_pmtrack[NMAXTRACKS][3][2000];   //[ntracks_pmtrack]
   Float_t         trkdqdx_pmtrack[NMAXTRACKS][3][2000];   //[ntracks_pmtrack]
   Float_t         trkresrg_pmtrack[NMAXTRACKS][3][2000];   //[ntracks_pmtrack]
   Int_t           trktpc_pmtrack[NMAXTRACKS][3][2000];   //[ntracks_pmtrack]
   Float_t         trkxyz_pmtrack[NMAXTRACKS][3][2000][3];   //[ntracks_pmtrack]
   Float_t         trkstartx_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstarty_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstartz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstartd_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkendx_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkendy_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkendz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkendd_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkflashT0_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trktrueT0_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Int_t           trkg4id_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Int_t           trkorig_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpurity_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkcompleteness_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trktheta_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkphi_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstartdcosx_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstartdcosy_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkstartdcosz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkenddcosx_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkenddcosy_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkenddcosz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkthetaxz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkthetayz_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkmom_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkmomrange_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkmommschi2_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkmommsllhd_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trklen_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trksvtxid_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkevtxid_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpidmvamu_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpidmvae_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpidmvapich_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpidmvapi0_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Float_t         trkpidmvapr_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Int_t           trkpidpdg_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidchi_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidchipr_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidchika_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidchipi_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidchimu_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Float_t         trkpidpida_pmtrack[NMAXTRACKS][3];   //[ntracks_pmtrack]
   Short_t         trkpidbestplane_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkhasPFParticle_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         trkPFParticleID_pmtrack[NMAXTRACKS];   //[ntracks_pmtrack]
   Short_t         ntracks_pandora;
   Short_t         trkId_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkncosmictags_tagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkcosmicscore_tagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkcosmictype_tagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkncosmictags_containmenttagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkcosmicscore_containmenttagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkcosmictype_containmenttagger_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkncosmictags_flashmatch_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkcosmicscore_flashmatch_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkcosmictype_flashmatch_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkke_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkrange_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Int_t           trkidtruth_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Short_t         trkorigin_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Int_t           trkpdgtruth_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkefftruth_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpurtruth_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpitchc_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Short_t         ntrkhits_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkdedx_pandora[NMAXTRACKS][3][2000];   //[ntracks_pandora]
   Float_t         trkdqdx_pandora[NMAXTRACKS][3][2000];   //[ntracks_pandora]
   Float_t         trkresrg_pandora[NMAXTRACKS][3][2000];   //[ntracks_pandora]
   Int_t           trktpc_pandora[NMAXTRACKS][3][2000];   //[ntracks_pandora]
   Float_t         trkxyz_pandora[NMAXTRACKS][3][2000][3];   //[ntracks_pandora]
   Float_t         trkstartx_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstarty_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstartz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstartd_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkendx_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkendy_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkendz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkendd_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkflashT0_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trktrueT0_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Int_t           trkg4id_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Int_t           trkorig_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpurity_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkcompleteness_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trktheta_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkphi_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstartdcosx_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstartdcosy_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkstartdcosz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkenddcosx_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkenddcosy_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkenddcosz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkthetaxz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkthetayz_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkmom_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkmomrange_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkmommschi2_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkmommsllhd_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trklen_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trksvtxid_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkevtxid_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpidmvamu_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpidmvae_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpidmvapich_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpidmvapi0_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Float_t         trkpidmvapr_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Int_t           trkpidpdg_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidchi_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidchipr_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidchika_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidchipi_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidchimu_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Float_t         trkpidpida_pandora[NMAXTRACKS][3];   //[ntracks_pandora]
   Short_t         trkpidbestplane_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkhasPFParticle_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         trkPFParticleID_pandora[NMAXTRACKS];   //[ntracks_pandora]
   Short_t         ntracks_pmtrajfit;
   Short_t         trkId_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkncosmictags_tagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkcosmicscore_tagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkcosmictype_tagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkncosmictags_containmenttagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkcosmicscore_containmenttagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkcosmictype_containmenttagger_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkncosmictags_flashmatch_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkcosmicscore_flashmatch_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkcosmictype_flashmatch_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkke_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkrange_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Int_t           trkidtruth_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Short_t         trkorigin_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Int_t           trkpdgtruth_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkefftruth_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpurtruth_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpitchc_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Short_t         ntrkhits_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkdedx_pmtrajfit[NMAXTRACKS][3][2000];   //[ntracks_pmtrajfit]
   Float_t         trkdqdx_pmtrajfit[NMAXTRACKS][3][2000];   //[ntracks_pmtrajfit]
   Float_t         trkresrg_pmtrajfit[NMAXTRACKS][3][2000];   //[ntracks_pmtrajfit]
   Int_t           trktpc_pmtrajfit[NMAXTRACKS][3][2000];   //[ntracks_pmtrajfit]
   Float_t         trkxyz_pmtrajfit[NMAXTRACKS][3][2000][3];   //[ntracks_pmtrajfit]
   Float_t         trkstartx_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstarty_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstartz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstartd_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkendx_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkendy_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkendz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkendd_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkflashT0_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trktrueT0_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Int_t           trkg4id_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Int_t           trkorig_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpurity_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkcompleteness_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trktheta_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkphi_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstartdcosx_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstartdcosy_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkstartdcosz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkenddcosx_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkenddcosy_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkenddcosz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkthetaxz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkthetayz_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkmom_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkmomrange_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkmommschi2_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkmommsllhd_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trklen_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trksvtxid_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkevtxid_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpidmvamu_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpidmvae_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpidmvapich_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpidmvapi0_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Float_t         trkpidmvapr_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Int_t           trkpidpdg_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidchi_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidchipr_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidchika_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidchipi_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidchimu_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Float_t         trkpidpida_pmtrajfit[NMAXTRACKS][3];   //[ntracks_pmtrajfit]
   Short_t         trkpidbestplane_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkhasPFParticle_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         trkPFParticleID_pmtrajfit[NMAXTRACKS];   //[ntracks_pmtrajfit]
   Short_t         nvtx_linecluster;
   Short_t         vtxId_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Float_t         vtxx_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Float_t         vtxy_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Float_t         vtxz_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Short_t         vtxhasPFParticle_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Short_t         vtxPFParticleID_linecluster[NMAXVTXS];   //[nvtx_linecluster]
   Short_t         nvtx_lineclusterdc;
   Short_t         vtxId_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Float_t         vtxx_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Float_t         vtxy_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Float_t         vtxz_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Short_t         vtxhasPFParticle_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Short_t         vtxPFParticleID_lineclusterdc[NMAXVTXS];   //[nvtx_lineclusterdc]
   Short_t         nvtx_pmtrack;
   Short_t         vtxId_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Float_t         vtxx_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Float_t         vtxy_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Float_t         vtxz_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Short_t         vtxhasPFParticle_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Short_t         vtxPFParticleID_pmtrack[NMAXVTXS];   //[nvtx_pmtrack]
   Short_t         nvtx_pmtrackdc;
   Short_t         vtxId_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Float_t         vtxx_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Float_t         vtxy_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Float_t         vtxz_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Short_t         vtxhasPFParticle_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Short_t         vtxPFParticleID_pmtrackdc[NMAXVTXS];   //[nvtx_pmtrackdc]
   Short_t         nvtx_pandora;
   Short_t         vtxId_pandora[NMAXVTXS];   //[nvtx_pandora]
   Float_t         vtxx_pandora[NMAXVTXS];   //[nvtx_pandora]
   Float_t         vtxy_pandora[NMAXVTXS];   //[nvtx_pandora]
   Float_t         vtxz_pandora[NMAXVTXS];   //[nvtx_pandora]
   Short_t         vtxhasPFParticle_pandora[NMAXVTXS];   //[nvtx_pandora]
   Short_t         vtxPFParticleID_pandora[NMAXVTXS];   //[nvtx_pandora]
   Short_t         nvtx_pandoradc;
   Short_t         vtxId_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]
   Float_t         vtxx_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]
   Float_t         vtxy_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]
   Float_t         vtxz_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]
   Short_t         vtxhasPFParticle_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]
   Short_t         vtxPFParticleID_pandoradc[NMAXVTXS];   //[nvtx_pandoradc]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!
   TBranch        *b_evttime;   //!
   TBranch        *b_beamtime;   //!
   TBranch        *b_pot;   //!
   TBranch        *b_isdata;   //!
   TBranch        *b_taulife;   //!
   TBranch        *b_triggernumber;   //!
   TBranch        *b_triggertime;   //!
   TBranch        *b_beamgatetime;   //!
   TBranch        *b_triggerbits;   //!
   TBranch        *b_potbnb;   //!
   TBranch        *b_potnumitgt;   //!
   TBranch        *b_potnumi101;   //!
   TBranch        *b_no_hits;   //!
   TBranch        *b_no_hits_stored;   //!
   TBranch        *b_hit_tpc;   //!
   TBranch        *b_hit_plane;   //!
   TBranch        *b_hit_wire;   //!
   TBranch        *b_hit_channel;   //!
   TBranch        *b_hit_peakT;   //!
   TBranch        *b_hit_charge;   //!
   TBranch        *b_hit_ph;   //!
   TBranch        *b_hit_startT;   //!
   TBranch        *b_hit_endT;   //!
   TBranch        *b_hit_rms;   //!
   TBranch        *b_hit_trueX;   //!
   TBranch        *b_hit_goodnessOfFit;   //!
   TBranch        *b_hit_multiplicity;   //!
   TBranch        *b_hit_trkid;   //!
   TBranch        *b_hit_trkKey;   //!
   TBranch        *b_hit_clusterid;   //!
   TBranch        *b_hit_clusterKey;   //!
   TBranch        *b_hit_nelec;   //!
   TBranch        *b_hit_energy;   //!
   TBranch        *b_nclusters;   //!
   TBranch        *b_clusterId;   //!
   TBranch        *b_clusterView;   //!
   TBranch        *b_cluster_StartCharge;   //!
   TBranch        *b_cluster_StartAngle;   //!
   TBranch        *b_cluster_EndCharge;   //!
   TBranch        *b_cluster_EndAngle;   //!
   TBranch        *b_cluster_Integral;   //!
   TBranch        *b_cluster_IntegralAverage;   //!
   TBranch        *b_cluster_SummedADC;   //!
   TBranch        *b_cluster_SummedADCaverage;   //!
   TBranch        *b_cluster_MultipleHitDensity;   //!
   TBranch        *b_cluster_Width;   //!
   TBranch        *b_cluster_NHits;   //!
   TBranch        *b_cluster_StartWire;   //!
   TBranch        *b_cluster_StartTick;   //!
   TBranch        *b_cluster_EndWire;   //!
   TBranch        *b_cluster_EndTick;   //!
   TBranch        *b_cluncosmictags_tagger;   //!
   TBranch        *b_clucosmicscore_tagger;   //!
   TBranch        *b_clucosmictype_tagger;   //!
   TBranch        *b_no_flashes;   //!
   TBranch        *b_flash_time;   //!
   TBranch        *b_flash_pe;   //!
   TBranch        *b_flash_ycenter;   //!
   TBranch        *b_flash_zcenter;   //!
   TBranch        *b_flash_ywidth;   //!
   TBranch        *b_flash_zwidth;   //!
   TBranch        *b_flash_timewidth;   //!
   TBranch        *b_no_ExternCounts;   //!
   TBranch        *b_externcounts_time;   //!
   TBranch        *b_externcounts_id;   //!
   TBranch        *b_kNTracker;   //!
   TBranch        *b_kNVertexAlgos;   //!
   TBranch        *b_mcevts_truthcry;   //!
   TBranch        *b_cry_no_primaries;   //!
   TBranch        *b_cry_primaries_pdg;   //!
   TBranch        *b_cry_Eng;   //!
   TBranch        *b_cry_Px;   //!
   TBranch        *b_cry_Py;   //!
   TBranch        *b_cry_Pz;   //!
   TBranch        *b_cry_P;   //!
   TBranch        *b_cry_StartPointx;   //!
   TBranch        *b_cry_StartPointy;   //!
   TBranch        *b_cry_StartPointz;   //!
   TBranch        *b_cry_StartPointt;   //!
   TBranch        *b_cry_status_code;   //!
   TBranch        *b_cry_mass;   //!
   TBranch        *b_cry_trackID;   //!
   TBranch        *b_cry_ND;   //!
   TBranch        *b_cry_mother;   //!
   TBranch        *b_no_primaries;   //!
   TBranch        *b_geant_list_size;   //!
   TBranch        *b_geant_list_size_in_tpcAV;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_status;   //!
   TBranch        *b_Mass;   //!
   TBranch        *b_Eng;   //!
   TBranch        *b_EndE;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_P;   //!
   TBranch        *b_StartPointx;   //!
   TBranch        *b_StartPointy;   //!
   TBranch        *b_StartPointz;   //!
   TBranch        *b_StartT;   //!
   TBranch        *b_EndPointx;   //!
   TBranch        *b_EndPointy;   //!
   TBranch        *b_EndPointz;   //!
   TBranch        *b_EndT;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_theta_xz;   //!
   TBranch        *b_theta_yz;   //!
   TBranch        *b_pathlen;   //!
   TBranch        *b_inTPCActive;   //!
   TBranch        *b_StartPointx_tpcAV;   //!
   TBranch        *b_StartPointy_tpcAV;   //!
   TBranch        *b_StartPointz_tpcAV;   //!
   TBranch        *b_StartT_tpcAV;   //!
   TBranch        *b_StartE_tpcAV;   //!
   TBranch        *b_StartP_tpcAV;   //!
   TBranch        *b_StartPx_tpcAV;   //!
   TBranch        *b_StartPy_tpcAV;   //!
   TBranch        *b_StartPz_tpcAV;   //!
   TBranch        *b_EndPointx_tpcAV;   //!
   TBranch        *b_EndPointy_tpcAV;   //!
   TBranch        *b_EndPointz_tpcAV;   //!
   TBranch        *b_EndT_tpcAV;   //!
   TBranch        *b_EndE_tpcAV;   //!
   TBranch        *b_EndP_tpcAV;   //!
   TBranch        *b_EndPx_tpcAV;   //!
   TBranch        *b_EndPy_tpcAV;   //!
   TBranch        *b_EndPz_tpcAV;   //!
   TBranch        *b_pathlen_drifted;   //!
   TBranch        *b_inTPCDrifted;   //!
   TBranch        *b_StartPointx_drifted;   //!
   TBranch        *b_StartPointy_drifted;   //!
   TBranch        *b_StartPointz_drifted;   //!
   TBranch        *b_StartT_drifted;   //!
   TBranch        *b_StartE_drifted;   //!
   TBranch        *b_StartP_drifted;   //!
   TBranch        *b_StartPx_drifted;   //!
   TBranch        *b_StartPy_drifted;   //!
   TBranch        *b_StartPz_drifted;   //!
   TBranch        *b_EndPointx_drifted;   //!
   TBranch        *b_EndPointy_drifted;   //!
   TBranch        *b_EndPointz_drifted;   //!
   TBranch        *b_EndT_drifted;   //!
   TBranch        *b_EndE_drifted;   //!
   TBranch        *b_EndP_drifted;   //!
   TBranch        *b_EndPx_drifted;   //!
   TBranch        *b_EndPy_drifted;   //!
   TBranch        *b_EndPz_drifted;   //!
   TBranch        *b_NumberDaughters;   //!
   TBranch        *b_Mother;   //!
   TBranch        *b_TrackId;   //!
   TBranch        *b_MergedId;   //!
   TBranch        *b_origin;   //!
   TBranch        *b_MCTruthIndex;   //!
   TBranch        *b_process_primary;   //!
   TBranch        *b_processname;   //!
   TBranch        *b_ntracks_pmtrack;   //!
   TBranch        *b_trkId_pmtrack;   //!
   TBranch        *b_trkncosmictags_tagger_pmtrack;   //!
   TBranch        *b_trkcosmicscore_tagger_pmtrack;   //!
   TBranch        *b_trkcosmictype_tagger_pmtrack;   //!
   TBranch        *b_trkncosmictags_containmenttagger_pmtrack;   //!
   TBranch        *b_trkcosmicscore_containmenttagger_pmtrack;   //!
   TBranch        *b_trkcosmictype_containmenttagger_pmtrack;   //!
   TBranch        *b_trkncosmictags_flashmatch_pmtrack;   //!
   TBranch        *b_trkcosmicscore_flashmatch_pmtrack;   //!
   TBranch        *b_trkcosmictype_flashmatch_pmtrack;   //!
   TBranch        *b_trkke_pmtrack;   //!
   TBranch        *b_trkrange_pmtrack;   //!
   TBranch        *b_trkidtruth_pmtrack;   //!
   TBranch        *b_trkorigin_pmtrack;   //!
   TBranch        *b_trkpdgtruth_pmtrack;   //!
   TBranch        *b_trkefftruth_pmtrack;   //!
   TBranch        *b_trkpurtruth_pmtrack;   //!
   TBranch        *b_trkpitchc_pmtrack;   //!
   TBranch        *b_ntrkhits_pmtrack;   //!
   TBranch        *b_trkdedx_pmtrack;   //!
   TBranch        *b_trkdqdx_pmtrack;   //!
   TBranch        *b_trkresrg_pmtrack;   //!
   TBranch        *b_trktpc_pmtrack;   //!
   TBranch        *b_trkxyz_pmtrack;   //!
   TBranch        *b_trkstartx_pmtrack;   //!
   TBranch        *b_trkstarty_pmtrack;   //!
   TBranch        *b_trkstartz_pmtrack;   //!
   TBranch        *b_trkstartd_pmtrack;   //!
   TBranch        *b_trkendx_pmtrack;   //!
   TBranch        *b_trkendy_pmtrack;   //!
   TBranch        *b_trkendz_pmtrack;   //!
   TBranch        *b_trkendd_pmtrack;   //!
   TBranch        *b_trkflashT0_pmtrack;   //!
   TBranch        *b_trktrueT0_pmtrack;   //!
   TBranch        *b_trkg4id_pmtrack;   //!
   TBranch        *b_trkorig_pmtrack;   //!
   TBranch        *b_trkpurity_pmtrack;   //!
   TBranch        *b_trkcompleteness_pmtrack;   //!
   TBranch        *b_trktheta_pmtrack;   //!
   TBranch        *b_trkphi_pmtrack;   //!
   TBranch        *b_trkstartdcosx_pmtrack;   //!
   TBranch        *b_trkstartdcosy_pmtrack;   //!
   TBranch        *b_trkstartdcosz_pmtrack;   //!
   TBranch        *b_trkenddcosx_pmtrack;   //!
   TBranch        *b_trkenddcosy_pmtrack;   //!
   TBranch        *b_trkenddcosz_pmtrack;   //!
   TBranch        *b_trkthetaxz_pmtrack;   //!
   TBranch        *b_trkthetayz_pmtrack;   //!
   TBranch        *b_trkmom_pmtrack;   //!
   TBranch        *b_trkmomrange_pmtrack;   //!
   TBranch        *b_trkmommschi2_pmtrack;   //!
   TBranch        *b_trkmommsllhd_pmtrack;   //!
   TBranch        *b_trklen_pmtrack;   //!
   TBranch        *b_trksvtxid_pmtrack;   //!
   TBranch        *b_trkevtxid_pmtrack;   //!
   TBranch        *b_trkpidmvamu_pmtrack;   //!
   TBranch        *b_trkpidmvae_pmtrack;   //!
   TBranch        *b_trkpidmvapich_pmtrack;   //!
   TBranch        *b_trkpidmvapi0_pmtrack;   //!
   TBranch        *b_trkpidmvapr_pmtrack;   //!
   TBranch        *b_trkpidpdg_pmtrack;   //!
   TBranch        *b_trkpidchi_pmtrack;   //!
   TBranch        *b_trkpidchipr_pmtrack;   //!
   TBranch        *b_trkpidchika_pmtrack;   //!
   TBranch        *b_trkpidchipi_pmtrack;   //!
   TBranch        *b_trkpidchimu_pmtrack;   //!
   TBranch        *b_trkpidpida_pmtrack;   //!
   TBranch        *b_trkpidbestplane_pmtrack;   //!
   TBranch        *b_trkhasPFParticle_pmtrack;   //!
   TBranch        *b_trkPFParticleID_pmtrack;   //!
   TBranch        *b_ntracks_pandora;   //!
   TBranch        *b_trkId_pandora;   //!
   TBranch        *b_trkncosmictags_tagger_pandora;   //!
   TBranch        *b_trkcosmicscore_tagger_pandora;   //!
   TBranch        *b_trkcosmictype_tagger_pandora;   //!
   TBranch        *b_trkncosmictags_containmenttagger_pandora;   //!
   TBranch        *b_trkcosmicscore_containmenttagger_pandora;   //!
   TBranch        *b_trkcosmictype_containmenttagger_pandora;   //!
   TBranch        *b_trkncosmictags_flashmatch_pandora;   //!
   TBranch        *b_trkcosmicscore_flashmatch_pandora;   //!
   TBranch        *b_trkcosmictype_flashmatch_pandora;   //!
   TBranch        *b_trkke_pandora;   //!
   TBranch        *b_trkrange_pandora;   //!
   TBranch        *b_trkidtruth_pandora;   //!
   TBranch        *b_trkorigin_pandora;   //!
   TBranch        *b_trkpdgtruth_pandora;   //!
   TBranch        *b_trkefftruth_pandora;   //!
   TBranch        *b_trkpurtruth_pandora;   //!
   TBranch        *b_trkpitchc_pandora;   //!
   TBranch        *b_ntrkhits_pandora;   //!
   TBranch        *b_trkdedx_pandora;   //!
   TBranch        *b_trkdqdx_pandora;   //!
   TBranch        *b_trkresrg_pandora;   //!
   TBranch        *b_trktpc_pandora;   //!
   TBranch        *b_trkxyz_pandora;   //!
   TBranch        *b_trkstartx_pandora;   //!
   TBranch        *b_trkstarty_pandora;   //!
   TBranch        *b_trkstartz_pandora;   //!
   TBranch        *b_trkstartd_pandora;   //!
   TBranch        *b_trkendx_pandora;   //!
   TBranch        *b_trkendy_pandora;   //!
   TBranch        *b_trkendz_pandora;   //!
   TBranch        *b_trkendd_pandora;   //!
   TBranch        *b_trkflashT0_pandora;   //!
   TBranch        *b_trktrueT0_pandora;   //!
   TBranch        *b_trkg4id_pandora;   //!
   TBranch        *b_trkorig_pandora;   //!
   TBranch        *b_trkpurity_pandora;   //!
   TBranch        *b_trkcompleteness_pandora;   //!
   TBranch        *b_trktheta_pandora;   //!
   TBranch        *b_trkphi_pandora;   //!
   TBranch        *b_trkstartdcosx_pandora;   //!
   TBranch        *b_trkstartdcosy_pandora;   //!
   TBranch        *b_trkstartdcosz_pandora;   //!
   TBranch        *b_trkenddcosx_pandora;   //!
   TBranch        *b_trkenddcosy_pandora;   //!
   TBranch        *b_trkenddcosz_pandora;   //!
   TBranch        *b_trkthetaxz_pandora;   //!
   TBranch        *b_trkthetayz_pandora;   //!
   TBranch        *b_trkmom_pandora;   //!
   TBranch        *b_trkmomrange_pandora;   //!
   TBranch        *b_trkmommschi2_pandora;   //!
   TBranch        *b_trkmommsllhd_pandora;   //!
   TBranch        *b_trklen_pandora;   //!
   TBranch        *b_trksvtxid_pandora;   //!
   TBranch        *b_trkevtxid_pandora;   //!
   TBranch        *b_trkpidmvamu_pandora;   //!
   TBranch        *b_trkpidmvae_pandora;   //!
   TBranch        *b_trkpidmvapich_pandora;   //!
   TBranch        *b_trkpidmvapi0_pandora;   //!
   TBranch        *b_trkpidmvapr_pandora;   //!
   TBranch        *b_trkpidpdg_pandora;   //!
   TBranch        *b_trkpidchi_pandora;   //!
   TBranch        *b_trkpidchipr_pandora;   //!
   TBranch        *b_trkpidchika_pandora;   //!
   TBranch        *b_trkpidchipi_pandora;   //!
   TBranch        *b_trkpidchimu_pandora;   //!
   TBranch        *b_trkpidpida_pandora;   //!
   TBranch        *b_trkpidbestplane_pandora;   //!
   TBranch        *b_trkhasPFParticle_pandora;   //!
   TBranch        *b_trkPFParticleID_pandora;   //!
   TBranch        *b_ntracks_pmtrajfit;   //!
   TBranch        *b_trkId_pmtrajfit;   //!
   TBranch        *b_trkncosmictags_tagger_pmtrajfit;   //!
   TBranch        *b_trkcosmicscore_tagger_pmtrajfit;   //!
   TBranch        *b_trkcosmictype_tagger_pmtrajfit;   //!
   TBranch        *b_trkncosmictags_containmenttagger_pmtrajfit;   //!
   TBranch        *b_trkcosmicscore_containmenttagger_pmtrajfit;   //!
   TBranch        *b_trkcosmictype_containmenttagger_pmtrajfit;   //!
   TBranch        *b_trkncosmictags_flashmatch_pmtrajfit;   //!
   TBranch        *b_trkcosmicscore_flashmatch_pmtrajfit;   //!
   TBranch        *b_trkcosmictype_flashmatch_pmtrajfit;   //!
   TBranch        *b_trkke_pmtrajfit;   //!
   TBranch        *b_trkrange_pmtrajfit;   //!
   TBranch        *b_trkidtruth_pmtrajfit;   //!
   TBranch        *b_trkorigin_pmtrajfit;   //!
   TBranch        *b_trkpdgtruth_pmtrajfit;   //!
   TBranch        *b_trkefftruth_pmtrajfit;   //!
   TBranch        *b_trkpurtruth_pmtrajfit;   //!
   TBranch        *b_trkpitchc_pmtrajfit;   //!
   TBranch        *b_ntrkhits_pmtrajfit;   //!
   TBranch        *b_trkdedx_pmtrajfit;   //!
   TBranch        *b_trkdqdx_pmtrajfit;   //!
   TBranch        *b_trkresrg_pmtrajfit;   //!
   TBranch        *b_trktpc_pmtrajfit;   //!
   TBranch        *b_trkxyz_pmtrajfit;   //!
   TBranch        *b_trkstartx_pmtrajfit;   //!
   TBranch        *b_trkstarty_pmtrajfit;   //!
   TBranch        *b_trkstartz_pmtrajfit;   //!
   TBranch        *b_trkstartd_pmtrajfit;   //!
   TBranch        *b_trkendx_pmtrajfit;   //!
   TBranch        *b_trkendy_pmtrajfit;   //!
   TBranch        *b_trkendz_pmtrajfit;   //!
   TBranch        *b_trkendd_pmtrajfit;   //!
   TBranch        *b_trkflashT0_pmtrajfit;   //!
   TBranch        *b_trktrueT0_pmtrajfit;   //!
   TBranch        *b_trkg4id_pmtrajfit;   //!
   TBranch        *b_trkorig_pmtrajfit;   //!
   TBranch        *b_trkpurity_pmtrajfit;   //!
   TBranch        *b_trkcompleteness_pmtrajfit;   //!
   TBranch        *b_trktheta_pmtrajfit;   //!
   TBranch        *b_trkphi_pmtrajfit;   //!
   TBranch        *b_trkstartdcosx_pmtrajfit;   //!
   TBranch        *b_trkstartdcosy_pmtrajfit;   //!
   TBranch        *b_trkstartdcosz_pmtrajfit;   //!
   TBranch        *b_trkenddcosx_pmtrajfit;   //!
   TBranch        *b_trkenddcosy_pmtrajfit;   //!
   TBranch        *b_trkenddcosz_pmtrajfit;   //!
   TBranch        *b_trkthetaxz_pmtrajfit;   //!
   TBranch        *b_trkthetayz_pmtrajfit;   //!
   TBranch        *b_trkmom_pmtrajfit;   //!
   TBranch        *b_trkmomrange_pmtrajfit;   //!
   TBranch        *b_trkmommschi2_pmtrajfit;   //!
   TBranch        *b_trkmommsllhd_pmtrajfit;   //!
   TBranch        *b_trklen_pmtrajfit;   //!
   TBranch        *b_trksvtxid_pmtrajfit;   //!
   TBranch        *b_trkevtxid_pmtrajfit;   //!
   TBranch        *b_trkpidmvamu_pmtrajfit;   //!
   TBranch        *b_trkpidmvae_pmtrajfit;   //!
   TBranch        *b_trkpidmvapich_pmtrajfit;   //!
   TBranch        *b_trkpidmvapi0_pmtrajfit;   //!
   TBranch        *b_trkpidmvapr_pmtrajfit;   //!
   TBranch        *b_trkpidpdg_pmtrajfit;   //!
   TBranch        *b_trkpidchi_pmtrajfit;   //!
   TBranch        *b_trkpidchipr_pmtrajfit;   //!
   TBranch        *b_trkpidchika_pmtrajfit;   //!
   TBranch        *b_trkpidchipi_pmtrajfit;   //!
   TBranch        *b_trkpidchimu_pmtrajfit;   //!
   TBranch        *b_trkpidpida_pmtrajfit;   //!
   TBranch        *b_trkpidbestplane_pmtrajfit;   //!
   TBranch        *b_trkhasPFParticle_pmtrajfit;   //!
   TBranch        *b_trkPFParticleID_pmtrajfit;   //!
   TBranch        *b_nvtx_linecluster;   //!
   TBranch        *b_vtxId_linecluster;   //!
   TBranch        *b_vtxx_linecluster;   //!
   TBranch        *b_vtxy_linecluster;   //!
   TBranch        *b_vtxz_linecluster;   //!
   TBranch        *b_vtxhasPFParticle_linecluster;   //!
   TBranch        *b_vtxPFParticleID_linecluster;   //!
   TBranch        *b_nvtx_lineclusterdc;   //!
   TBranch        *b_vtxId_lineclusterdc;   //!
   TBranch        *b_vtxx_lineclusterdc;   //!
   TBranch        *b_vtxy_lineclusterdc;   //!
   TBranch        *b_vtxz_lineclusterdc;   //!
   TBranch        *b_vtxhasPFParticle_lineclusterdc;   //!
   TBranch        *b_vtxPFParticleID_lineclusterdc;   //!
   TBranch        *b_nvtx_pmtrack;   //!
   TBranch        *b_vtxId_pmtrack;   //!
   TBranch        *b_vtxx_pmtrack;   //!
   TBranch        *b_vtxy_pmtrack;   //!
   TBranch        *b_vtxz_pmtrack;   //!
   TBranch        *b_vtxhasPFParticle_pmtrack;   //!
   TBranch        *b_vtxPFParticleID_pmtrack;   //!
   TBranch        *b_nvtx_pmtrackdc;   //!
   TBranch        *b_vtxId_pmtrackdc;   //!
   TBranch        *b_vtxx_pmtrackdc;   //!
   TBranch        *b_vtxy_pmtrackdc;   //!
   TBranch        *b_vtxz_pmtrackdc;   //!
   TBranch        *b_vtxhasPFParticle_pmtrackdc;   //!
   TBranch        *b_vtxPFParticleID_pmtrackdc;   //!
   TBranch        *b_nvtx_pandora;   //!
   TBranch        *b_vtxId_pandora;   //!
   TBranch        *b_vtxx_pandora;   //!
   TBranch        *b_vtxy_pandora;   //!
   TBranch        *b_vtxz_pandora;   //!
   TBranch        *b_vtxhasPFParticle_pandora;   //!
   TBranch        *b_vtxPFParticleID_pandora;   //!
   TBranch        *b_nvtx_pandoradc;   //!
   TBranch        *b_vtxId_pandoradc;   //!
   TBranch        *b_vtxx_pandoradc;   //!
   TBranch        *b_vtxy_pandoradc;   //!
   TBranch        *b_vtxz_pandoradc;   //!
   TBranch        *b_vtxhasPFParticle_pandoradc;   //!
   TBranch        *b_vtxPFParticleID_pandoradc;   //!

  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};



#endif

