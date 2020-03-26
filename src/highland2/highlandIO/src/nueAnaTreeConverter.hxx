/*
   converter for DUNE NueTree input format

   A. Cervera AprilMay 2016
*/

#ifndef nueAnaTreeConverter_h
#define nueAnaTreeConverter_h

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
//#include "LArSoftReaderHeaders.h"

class nueAnaTreeConverter: public InputConverter{

 public:

  nueAnaTreeConverter();
  virtual ~nueAnaTreeConverter();

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
  virtual AnaVertex*        MakeVertex()      { return new AnaVertex(); }

  // ----------------------------

  virtual void FillInfo(AnaSpill* spill);
  virtual void FillBeamInfo(AnaBeam* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);
  virtual void FillDQInfo(AnaDataQuality* dq);
  virtual void FillTrueInfo(AnaSpill* spill);
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch);
  virtual void FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles,           Int_t itrk, AnaParticle* part);
  virtual void FillParticleInfoFromShower(std::vector<AnaTrueParticleB*>& trueParticles, Int_t ishw, AnaParticle* part);
  virtual void FillTrueParticleInfo(std::vector<AnaTrueVertexB*>& trueVertices, Int_t ipart, AnaTrueParticle* truePart);
  virtual void FillVertexInfo(Int_t ivtx, AnaVertex* vertex);
  virtual void FillTrueVertexInfo(Int_t ivtx, AnaTrueVertex* vertex);

  AnaTrueObjectC* FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles);

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

  std::vector<Int_t> _nhits_trk;
  std::vector<Float_t> _dedx_trk;
  std::vector<Int_t> _nhits_shw;
  std::vector<Float_t> _dedx_shw;

   // Declaration of leaf types
   Int_t           run;
   Int_t           subrun;
   Int_t           event;
   Float_t         evttime;
   Float_t         taulife;
   Short_t         isdata;
   Int_t           ntracks_reco;
   Int_t           trkid[415];   //[ntracks_reco]
   Float_t         trkstartx[415];   //[ntracks_reco]
   Float_t         trkstarty[415];   //[ntracks_reco]
   Float_t         trkstartz[415];   //[ntracks_reco]
   Float_t         trkendx[415];   //[ntracks_reco]
   Float_t         trkendy[415];   //[ntracks_reco]
   Float_t         trkendz[415];   //[ntracks_reco]
   Float_t         trkstartdcosx[415];   //[ntracks_reco]
   Float_t         trkstartdcosy[415];   //[ntracks_reco]
   Float_t         trkstartdcosz[415];   //[ntracks_reco]
   Float_t         trkenddcosx[415];   //[ntracks_reco]
   Float_t         trkenddcosy[415];   //[ntracks_reco]
   Float_t         trkenddcosz[415];   //[ntracks_reco]
   Float_t         trklen[415];   //[ntracks_reco]
   Int_t           trkg4id[415];   //[ntracks_reco]
   Int_t           trkg4pdg[415];   //[ntracks_reco]
   Float_t         trkg4startx[415];   //[ntracks_reco]
   Float_t         trkg4starty[415];   //[ntracks_reco]
   Float_t         trkg4startz[415];   //[ntracks_reco]
   Float_t         trkg4initdedx[415];   //[ntracks_reco]
   Int_t           nshws;
   Int_t           shwid[42];   //[nshws]
   Float_t         shwdcosx[42];   //[nshws]
   Float_t         shwdcosy[42];   //[nshws]
   Float_t         shwdcosz[42];   //[nshws]
   Float_t         shwstartx[42];   //[nshws]
   Float_t         shwstarty[42];   //[nshws]
   Float_t         shwstartz[42];   //[nshws]
   Float_t         shwenergy[42][3];   //[nshws]
   Float_t         shwdedx[42][3];   //[nshws]
   Int_t           shwbestplane[42];   //[nshws]
   Int_t           shwg4id[42];   //[nshws]
   Int_t           flash_total;
   Float_t         flash_time[5];   //[flash_total]
   Float_t         flash_width[5];   //[flash_total]
   Float_t         flash_abstime[5];   //[flash_total]
   Float_t         flash_YCenter[5];   //[flash_total]
   Float_t         flash_YWidth[5];   //[flash_total]
   Float_t         flash_ZCenter[5];   //[flash_total]
   Float_t         flash_ZWidth[5];   //[flash_total]
   Float_t         flash_TotalPE[5];   //[flash_total]
   Int_t           nhits;
   Short_t         hit_plane[36059];   //[nhits]
   Short_t         hit_wire[36059];   //[nhits]
   Int_t           hit_channel[36059];   //[nhits]
   Float_t         hit_peakT[36059];   //[nhits]
   Float_t         hit_charge[36059];   //[nhits]
   Float_t         hit_summedADC[36059];   //[nhits]
   Float_t         hit_startT[36059];   //[nhits]
   Float_t         hit_endT[36059];   //[nhits]
   Int_t           hit_trkkey[36059];   //[nhits]
   Float_t         hit_dQds[36059];   //[nhits]
   Float_t         hit_dEds[36059];   //[nhits]
   Float_t         hit_resrange[36059];   //[nhits]
   Int_t           hit_shwkey[36059];   //[nhits]
   Int_t           infidvol;
   Short_t         nvtx;
   Float_t         vtx[9][3];   //[nvtx]
   Float_t         vtxrecomc;
   Float_t         vtxrecomcx;
   Float_t         vtxrecomcy;
   Float_t         vtxrecomcz;
   Int_t           mcevts_truth;
   Int_t           nuPDG_truth;
   Int_t           ccnc_truth;
   Int_t           mode_truth;
   Float_t         enu_truth;
   Float_t         Q2_truth;
   Float_t         W_truth;
   Float_t         X_truth;
   Float_t         Y_truth;
   Int_t           hitnuc_truth;
   Int_t           target_truth;
   Float_t         nuvtxx_truth;
   Float_t         nuvtxy_truth;
   Float_t         nuvtxz_truth;
   Float_t         nu_dcosx_truth;
   Float_t         nu_dcosy_truth;
   Float_t         nu_dcosz_truth;
   Float_t         lep_mom_truth;
   Float_t         lep_dcosx_truth;
   Float_t         lep_dcosy_truth;
   Float_t         lep_dcosz_truth;
   Float_t         t0_truth;
   Int_t           no_primaries;
   Int_t           geant_list_size;
   Int_t           pdg[8077];   //[geant_list_size]
   Float_t         Eng[8077];   //[geant_list_size]
   Float_t         Px[8077];   //[geant_list_size]
   Float_t         Py[8077];   //[geant_list_size]
   Float_t         Pz[8077];   //[geant_list_size]
   Float_t         StartPointx[8077];   //[geant_list_size]
   Float_t         StartPointy[8077];   //[geant_list_size]
   Float_t         StartPointz[8077];   //[geant_list_size]
   Float_t         EndPointx[8077];   //[geant_list_size]
   Float_t         EndPointy[8077];   //[geant_list_size]
   Float_t         EndPointz[8077];   //[geant_list_size]
   Float_t         Startdcosx[8077];   //[geant_list_size]
   Float_t         Startdcosy[8077];   //[geant_list_size]
   Float_t         Startdcosz[8077];   //[geant_list_size]
   Int_t           NumberDaughters[8077];   //[geant_list_size]
   Int_t           Mother[8077];   //[geant_list_size]
   Int_t           TrackId[8077];   //[geant_list_size]
   Int_t           process_primary[8077];   //[geant_list_size]
   std::vector<std::string>  *G4Process;
   std::vector<std::string>  *G4FinalProcess;
   Int_t           ptype_flux;
   Float_t         pdpx_flux;
   Float_t         pdpy_flux;
   Float_t         pdpz_flux;
   Int_t           pntype_flux;
   Float_t         vx_flux;
   Float_t         vy_flux;
   Float_t         vz_flux;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!
   TBranch        *b_evttime;   //!
   TBranch        *b_taulife;   //!
   TBranch        *b_isdata;   //!
   TBranch        *b_ntracks_reco;   //!
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
   TBranch        *b_trklen;   //!
   TBranch        *b_trkg4id;   //!
   TBranch        *b_trkg4pdg;   //!
   TBranch        *b_trkg4startx;   //!
   TBranch        *b_trkg4starty;   //!
   TBranch        *b_trkg4startz;   //!
   TBranch        *b_trkg4initdedx;   //!
   TBranch        *b_nshws;   //!
   TBranch        *b_shwid;   //!
   TBranch        *b_shwdcosx;   //!
   TBranch        *b_shwdcosy;   //!
   TBranch        *b_shwdcosz;   //!
   TBranch        *b_shwstartx;   //!
   TBranch        *b_shwstarty;   //!
   TBranch        *b_shwstartz;   //!
   TBranch        *b_shwenergy;   //!
   TBranch        *b_shwdedx;   //!
   TBranch        *b_shwbestplane;   //!
   TBranch        *b_shwg4id;   //!
   TBranch        *b_flash_total;   //!
   TBranch        *b_flash_time;   //!
   TBranch        *b_flash_width;   //!
   TBranch        *b_flash_abstime;   //!
   TBranch        *b_flash_YCenter;   //!
   TBranch        *b_flash_YWidth;   //!
   TBranch        *b_flash_ZCenter;   //!
   TBranch        *b_flash_ZWidth;   //!
   TBranch        *b_flash_TotalPE;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_hit_plane;   //!
   TBranch        *b_hit_wire;   //!
   TBranch        *b_hit_channel;   //!
   TBranch        *b_hit_peakT;   //!
   TBranch        *b_hit_charge;   //!
   TBranch        *b_hit_summedADC;   //!
   TBranch        *b_hit_startT;   //!
   TBranch        *b_hit_endT;   //!
   TBranch        *b_hit_trkkey;   //!
   TBranch        *b_hit_dQds;   //!
   TBranch        *b_hit_dEds;   //!
   TBranch        *b_hit_resrange;   //!
   TBranch        *b_hit_shwkey;   //!
   TBranch        *b_infidvol;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_vtx;   //!
   TBranch        *b_vtxrecomc;   //!
   TBranch        *b_vtxrecomcx;   //!
   TBranch        *b_vtxrecomcy;   //!
   TBranch        *b_vtxrecomcz;   //!
   TBranch        *b_mcevts_truth;   //!
   TBranch        *b_nuPDG_truth;   //!
   TBranch        *b_ccnc_truth;   //!
   TBranch        *b_mode_truth;   //!
   TBranch        *b_enu_truth;   //!
   TBranch        *b_Q2_truth;   //!
   TBranch        *b_W_truth;   //!
   TBranch        *b_X_truth;   //!
   TBranch        *b_Y_truth;   //!
   TBranch        *b_hitnuc_truth;   //!
   TBranch        *b_target_truth;   //!
   TBranch        *b_nuvtxx_truth;   //!
   TBranch        *b_nuvtxy_truth;   //!
   TBranch        *b_nuvtxz_truth;   //!
   TBranch        *b_nu_dcosx_truth;   //!
   TBranch        *b_nu_dcosy_truth;   //!
   TBranch        *b_nu_dcosz_truth;   //!
   TBranch        *b_lep_mom_truth;   //!
   TBranch        *b_lep_dcosx_truth;   //!
   TBranch        *b_lep_dcosy_truth;   //!
   TBranch        *b_lep_dcosz_truth;   //!
   TBranch        *b_t0_truth;   //!
   TBranch        *b_no_primaries;   //!
   TBranch        *b_geant_list_size;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_Eng;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_StartPointx;   //!
   TBranch        *b_StartPointy;   //!
   TBranch        *b_StartPointz;   //!
   TBranch        *b_EndPointx;   //!
   TBranch        *b_EndPointy;   //!
   TBranch        *b_EndPointz;   //!
   TBranch        *b_Startdcosx;   //!
   TBranch        *b_Startdcosy;   //!
   TBranch        *b_Startdcosz;   //!
   TBranch        *b_NumberDaughters;   //!
   TBranch        *b_Mother;   //!
   TBranch        *b_TrackId;   //!
   TBranch        *b_process_primary;   //!
   TBranch        *b_G4Process;   //!
   TBranch        *b_G4FinalProcess;   //!
   TBranch        *b_ptype_flux;   //!
   TBranch        *b_pdpx_flux;   //!
   TBranch        *b_pdpy_flux;   //!
   TBranch        *b_pdpz_flux;   //!
   TBranch        *b_pntype_flux;   //!
   TBranch        *b_vx_flux;   //!
   TBranch        *b_vy_flux;   //!
   TBranch        *b_vz_flux;   //!

  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};



#endif

