/*
   converter for DUNE AnaTree input format

   A. Cervera April 2016
*/

#ifndef anaTreeConverter_h
#define anaTreeConverter_h

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

class anaTreeConverter: public InputConverter{

 public:

  anaTreeConverter();
  virtual ~anaTreeConverter();

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
  virtual void FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, Int_t ipart, AnaTrueParticle* truePart);
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
   Float_t         efield[3];
   Int_t           t0;
   Int_t           ntracks_reco;
   Int_t           ntrkhits[7];   //[ntracks_reco]
   Int_t           trkid[7];   //[ntracks_reco]
   Float_t         trkstartx[7];   //[ntracks_reco]
   Float_t         trkstarty[7];   //[ntracks_reco]
   Float_t         trkstartz[7];   //[ntracks_reco]
   Float_t         trkendx[7];   //[ntracks_reco]
   Float_t         trkendy[7];   //[ntracks_reco]
   Float_t         trkendz[7];   //[ntracks_reco]
   Float_t         trkstartdcosx[7];   //[ntracks_reco]
   Float_t         trkstartdcosy[7];   //[ntracks_reco]
   Float_t         trkstartdcosz[7];   //[ntracks_reco]
   Float_t         trkenddcosx[7];   //[ntracks_reco]
   Float_t         trkenddcosy[7];   //[ntracks_reco]
   Float_t         trkenddcosz[7];   //[ntracks_reco]
   Float_t         trkx[7][1000];   //[ntracks_reco]
   Float_t         trky[7][1000];   //[ntracks_reco]
   Float_t         trkz[7][1000];   //[ntracks_reco]
   Float_t         trktheta_xz[7];   //[ntracks_reco]
   Float_t         trktheta_yz[7];   //[ntracks_reco]
   Float_t         trketa_xy[7];   //[ntracks_reco]
   Float_t         trketa_zy[7];   //[ntracks_reco]
   Float_t         trktheta[7];   //[ntracks_reco]
   Float_t         trkphi[7];   //[ntracks_reco]
   Float_t         trkd2[7];   //[ntracks_reco]
   Float_t         trkdedx2[7][3][1000];   //[ntracks_reco]
   Float_t         trkdqdx[7][3][1000];   //[ntracks_reco]
   Float_t         trkpitch[7][3];   //[ntracks_reco]
   Float_t         trkpitchHit[7][3][1000];   //[ntracks_reco]
   Float_t         trkkinE[7][3];   //[ntracks_reco]
   Float_t         trkrange[7][3];   //[ntracks_reco]
   Float_t         trkTPC[7][3][1000];   //[ntracks_reco]
   Float_t         trkplaneid[7][3][1000];   //[ntracks_reco]
   Float_t         trkresrg[7][3][1000];   //[ntracks_reco]
   Float_t         trkPosx[7][3][1000];   //[ntracks_reco]
   Float_t         trkPosy[7][3][1000];   //[ntracks_reco]
   Float_t         trkPosz[7][3][1000];   //[ntracks_reco]
   Float_t         trklen[7];   //[ntracks_reco]
   Float_t         trklen_L[7];   //[ntracks_reco]
   Float_t         trkdQdxSum[7];   //[ntracks_reco]
   Float_t         trkdQdxAverage[7];   //[ntracks_reco]
   Float_t         trkdEdxSum[7];   //[ntracks_reco]
   Float_t         trkdEdxAverage[7];   //[ntracks_reco]
   Float_t         trkMCTruthT0[7];   //[ntracks_reco]
   Int_t           trkMCTruthTrackID[7];   //[ntracks_reco]
   Float_t         trkPhotonCounterT0[7];   //[ntracks_reco]
   Int_t           trkPhotonCounterID[7];   //[ntracks_reco]
   Int_t           trkPhotonCounterConf[7];   //[ntracks_reco]
   Int_t           nMCParticles;
   Int_t           trkid_MC[52];   //[nMCParticles]
   Int_t           trkpdg_MC[52];   //[nMCParticles]
   Int_t           trkndaughters_MC[52];   //[nMCParticles]
   Int_t           nTPCHits_MC[52];   //[nMCParticles]
   Int_t           StartInTPC_MC[52];   //[nMCParticles]
   Int_t           EndInTPC_MC[52];   //[nMCParticles]
   Int_t           trkMother_MC[52];   //[nMCParticles]
   Int_t           trkNumDaughters_MC[52];   //[nMCParticles]
   Int_t           trkFirstDaughter_MC[52];   //[nMCParticles]
   Int_t           trkPrimary_MC[52];   //[nMCParticles]
   Float_t         StartTime_MC[52];   //[nMCParticles]
   Float_t         trkstartx_MC[52];   //[nMCParticles]
   Float_t         trkstarty_MC[52];   //[nMCParticles]
   Float_t         trkstartz_MC[52];   //[nMCParticles]
   Float_t         trkendx_MC[52];   //[nMCParticles]
   Float_t         trkendy_MC[52];   //[nMCParticles]
   Float_t         trkendz_MC[52];   //[nMCParticles]
   Float_t         trkenergy_MC[52];   //[nMCParticles]
   Float_t         EnergyDeposited_MC[52];   //[nMCParticles]
   Float_t         trkmom_MC[52];   //[nMCParticles]
   Float_t         trkmom_XMC[52];   //[nMCParticles]
   Float_t         trkmom_YMC[52];   //[nMCParticles]
   Float_t         trkmom_ZMC[52];   //[nMCParticles]
   Float_t         trkstartdoc_XMC[52];   //[nMCParticles]
   Float_t         trkstartdoc_YMC[52];   //[nMCParticles]
   Float_t         trkstartdoc_ZMC[52];   //[nMCParticles]
   Float_t         mcpos_x[52];   //[nMCParticles]
   Float_t         mcpos_y[52];   //[nMCParticles]
   Float_t         mcpos_z[52];   //[nMCParticles]
   Float_t         mcang_x[52];   //[nMCParticles]
   Float_t         mcang_y[52];   //[nMCParticles]
   Float_t         mcang_z[52];   //[nMCParticles]
   Float_t         trktheta_xz_MC[52];   //[nMCParticles]
   Float_t         trktheta_yz_MC[52];   //[nMCParticles]
   Float_t         trktheta_MC[52];   //[nMCParticles]
   Float_t         trkphi_MC[52];   //[nMCParticles]
   Float_t         trketa_xy_MC[52];   //[nMCParticles]
   Float_t         trketa_zy_MC[52];   //[nMCParticles]
   Float_t         trkTPCLen_MC[52];   //[nMCParticles]
   Int_t           nhits;
   Int_t           nhits2;
   Int_t           nclust;
   Int_t           hit_plane[1304];   //[nhits2]
   Int_t           hit_tpc[1304];   //[nhits2]
   Int_t           hit_wire[1304];   //[nhits2]
   Int_t           hit_channel[1304];   //[nhits2]
   Float_t         hit_peakT[1304];   //[nhits2]
   Float_t         hit_charge[1304];   //[nhits2]
   Float_t         hit_ph[1304];   //[nhits2]
   Int_t           hit_trkid[1304];   //[nhits2]
   Int_t           flash_total;
   Float_t         flash_time[4];   //[flash_total]
   Float_t         flash_width[4];   //[flash_total]
   Float_t         flash_abstime[4];   //[flash_total]
   Float_t         flash_YCentre[4];   //[flash_total]
   Float_t         flash_YWidth[4];   //[flash_total]
   Float_t         flash_ZCentre[4];   //[flash_total]
   Float_t         flash_ZWidth[4];   //[flash_total]
   Float_t         flash_TotalPE[4];   //[flash_total]
   Int_t           ntrigs;
   Int_t           trig_time[5];   //[ntrigs]
   Int_t           trig_id[5];   //[ntrigs]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!
   TBranch        *b_evttime;   //!
   TBranch        *b_efield;   //!
   TBranch        *b_t0;   //!
   TBranch        *b_ntracks_reco;   //!
   TBranch        *b_ntrkhits;   //!
   TBranch        *b_trkid;   //!
   TBranch        *b_trkstartx;   //!
   TBranch        *b_trkstarty;   //!
   TBranch        *b_trkstartz;   //!
   TBranch        *b_trkendx;   //!
   TBranch        *b_trkendy;   //!
   TBranch        *b_trkendz;   //!
   TBranch        *b_trkstartdcosx;   //!
   TBranch        *b_trkstartdcosy;   //!
   TBranch        *b_trkstartdcosz;   //!
   TBranch        *b_trkenddcosx;   //!
   TBranch        *b_trkenddcosy;   //!
   TBranch        *b_trkenddcosz;   //!
   TBranch        *b_trkx;   //!
   TBranch        *b_trky;   //!
   TBranch        *b_trkz;   //!
   TBranch        *b_trktheta_xz;   //!
   TBranch        *b_trktheta_yz;   //!
   TBranch        *b_trketa_xy;   //!
   TBranch        *b_trketa_zy;   //!
   TBranch        *b_trktheta;   //!
   TBranch        *b_trkphi;   //!
   TBranch        *b_trkd2;   //!
   TBranch        *b_trkdedx2;   //!
   TBranch        *b_trkdqdx;   //!
   TBranch        *b_trkpitch;   //!
   TBranch        *b_trkpitchHit;   //!
   TBranch        *b_trkkinE;   //!
   TBranch        *b_trkrange;   //!
   TBranch        *b_trkTPC;   //!
   TBranch        *b_trkplaneid;   //!
   TBranch        *b_trkresrg;   //!
   TBranch        *b_trkPosx;   //!
   TBranch        *b_trkPosy;   //!
   TBranch        *b_trkPosz;   //!
   TBranch        *b_trklen;   //!
   TBranch        *b_trklen_L;   //!
   TBranch        *b_trkdQdxSum;   //!
   TBranch        *b_trkdQdxAverage;   //!
   TBranch        *b_trkdEdxSum;   //!
   TBranch        *b_trkdEdxAverage;   //!
   TBranch        *b_trkMCTruthT0;   //!
   TBranch        *b_trkMCTruthTrackID;   //!
   TBranch        *b_trkPhotonCounterT0;   //!
   TBranch        *b_trkPhotonCounterID;   //!
   TBranch        *b_trkPhotonCounterConf;   //!
   TBranch        *b_nMCParticles;   //!
   TBranch        *b_trkid_MC;   //!
   TBranch        *b_trkpdg_MC;   //!
   TBranch        *b_trkndaughters_MC;   //!
   TBranch        *b_nTPCHits_MC;   //!
   TBranch        *b_StartInTPC_MC;   //!
   TBranch        *b_EndInTPC_MC;   //!
   TBranch        *b_trkMother_MC;   //!
   TBranch        *b_trkNumDaughters_MC;   //!
   TBranch        *b_trkFirstDaughter_MC;   //!
   TBranch        *b_trkPrimary_MC;   //!
   TBranch        *b_StartTime_MC;   //!
   TBranch        *b_trkstartx_MC;   //!
   TBranch        *b_trkstarty_MC;   //!
   TBranch        *b_trkstartz_MC;   //!
   TBranch        *b_trkendx_MC;   //!
   TBranch        *b_trkendy_MC;   //!
   TBranch        *b_trkendz_MC;   //!
   TBranch        *b_trkenergy_MC;   //!
   TBranch        *b_EnergyDeposited_MC;   //!
   TBranch        *b_trkmom_MC;   //!
   TBranch        *b_trkmom_XMC;   //!
   TBranch        *b_trkmom_YMC;   //!
   TBranch        *b_trkmom_ZMC;   //!
   TBranch        *b_trkstartdoc_XMC;   //!
   TBranch        *b_trkstartdoc_YMC;   //!
   TBranch        *b_trkstartdoc_ZMC;   //!
   TBranch        *b_mcpos_x;   //!
   TBranch        *b_mcpos_y;   //!
   TBranch        *b_mcpos_z;   //!
   TBranch        *b_mcang_x;   //!
   TBranch        *b_mcang_y;   //!
   TBranch        *b_mcang_z;   //!
   TBranch        *b_trktheta_xz_MC;   //!
   TBranch        *b_trktheta_yz_MC;   //!
   TBranch        *b_trktheta_MC;   //!
   TBranch        *b_trkphi_MC;   //!
   TBranch        *b_trketa_xy_MC;   //!
   TBranch        *b_trketa_zy_MC;   //!
   TBranch        *b_trkTPCLen_MC;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_nhits2;   //!
   TBranch        *b_nclust;   //!
   TBranch        *b_hit_plane;   //!
   TBranch        *b_hit_tpc;   //!
   TBranch        *b_hit_wire;   //!
   TBranch        *b_hit_channel;   //!
   TBranch        *b_hit_peakT;   //!
   TBranch        *b_hit_charge;   //!
   TBranch        *b_hit_ph;   //!
   TBranch        *b_hit_trkid;   //!
   TBranch        *b_flash_total;   //!
   TBranch        *b_flash_time;   //!
   TBranch        *b_flash_width;   //!
   TBranch        *b_flash_abstime;   //!
   TBranch        *b_flash_YCentre;   //!
   TBranch        *b_flash_YWidth;   //!
   TBranch        *b_flash_ZCentre;   //!
   TBranch        *b_flash_ZWidth;   //!
   TBranch        *b_flash_TotalPE;   //!
   TBranch        *b_ntrigs;   //!
   TBranch        *b_trig_time;   //!
   TBranch        *b_trig_id;   //!

  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};



#endif

