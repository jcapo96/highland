/*
   converter for DUNE LArSoftTree input format

   A. Cervera April 2016
*/

#ifndef LArSoftTreeConverter_h
#define LArSoftTreeConverter_h

#include "IsMC.h"

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
#include "LArSoftReaderProjectHeaders.h"

//#include "CorrectionBase.hxx"
//#include "BaseDataClasses.hxx"
#include "TH1F.h"
#include "TFile.h"

#ifdef ISMC
typedef std::pair<unsigned short, std::vector<sim::IDE> > TDCIDE;
typedef std::vector<TDCIDE> TDCIDEs_t;
typedef TDCIDE::first_type StoredTDC_t;

namespace sim {
   
  /// Ionization energy from a Geant4 track
  struct TrackIDE{
    int trackID;      ///< Geant4 supplied trackID
    float energyFrac; ///< fraction of hit energy from the particle with this trackID
    float energy;     ///< energy from the particle with this trackID [MeV]
    float numElectrons; ///< number of electrons from the particle detected on the wires
    
    TrackIDE() {}
    
    
    TrackIDE(int id, float ef, float e, float ne ) : trackID(id), energyFrac(ef), energy (e), numElectrons (ne) {}
    
    
  };
}
#endif

class LArSoftTreeConverter: public InputConverter{

 public:

  LArSoftTreeConverter();
  virtual ~LArSoftTreeConverter();

  virtual bool Initialize();
  virtual Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event){(void)entry;(void)event; return 0;}

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  virtual Int_t ReadEntries(Long64_t& entry);
  virtual bool AddFileToTChain(const std::string& inputString);

  //----------------
  virtual AnaSpillB* MakeSpill() { return new AnaSpill(); }
  virtual AnaBunch* MakeBunch() { return new AnaBunch(); }
  virtual AnaBeamB* MakeBeam() { return new AnaBeam(); }
  virtual AnaDataQualityB* MakeDataQuality() { return new AnaDataQuality(); }
  virtual AnaEventInfoB* MakeEventInfo() { return new AnaEventInfo(); }
  virtual AnaTrigger* MakeTrigger() { return new AnaTrigger(); }

  virtual AnaTrueParticle* MakeTrueParticle() { return new AnaTrueParticle(); }
  virtual AnaTrueVertex*   MakeTrueVertex() { return new AnaTrueVertex(); }
  virtual AnaParticle*     MakeParticle() { return new AnaParticle(); }

  // ----------------------------

  virtual void FillInfo(AnaSpill* spill);
  //  virtual void FillBunchInfo(AnaSpill* spill){}
  virtual void FillBeamInfo(AnaBeam* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);
  virtual void FillDQInfo(AnaDataQuality* dq);
  virtual void FillTrueInfo(AnaSpill* spill);
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunch* bunch);
  //  virtual void FillParticleTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaParticle* part);
  virtual void FillParticleTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, const recob::Track& track, AnaParticle* part);
  virtual void FillBasicTrackInfo(const recob::Track& track, AnaParticleMomB* part);
  virtual void FillParticleShowerInfo(std::vector<AnaTrueParticleB*>& trueParticles, const recob::Shower& shower, AnaParticle* part);
#ifdef ISMC
  virtual void FillTrueParticleInfo(AnaTrueVertexB* trueVertex, const simb::MCParticle& part, AnaTrueParticle* truePart);
  virtual void FillTrueVertexInfo(Int_t ivertex, AnaTrueVertex* trueVertex);
#endif
  
  virtual void FillPFParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles, Int_t itrk, AnaBunch* bunch);
  virtual void FillPFParticleDaughterInfo(Int_t itrk, AnaBunch* bunch, int indent=0);
  
  AnaTrueObjectC* FindTrueParticle(Int_t itrk, std::vector<AnaTrueParticleB*>& trueParticles, Float_t& purity);


  double ComputeTrackLength(const recob::Track& track);

  //  const simb::MCParticle* GetMCParticleFromRecoTrack(const recob::Track &track, std::string trackModule) const;

#ifdef ISMC
  const simb::MCParticle* GetGeantGoodParticle(const simb::MCTruth &genTruth) const;
  void HitsPurity(std::vector< recob::Hit > const& hits, Int_t& trackid, Float_t& purity, double& maxe);
  std::vector<sim::IDE> HitToEveID(const recob::Hit& hit);

  double ComputeMCTrajectoryLength(const simb::MCTrajectory& traj, Float_t& lengthInTPC);

  //  const recob::Slice* GetPFParticleSlice(const recob::PFParticle &particle, const std::string particleLabel) const;

  simb::MCParticle* GetMotherMCParticle(const simb::MCParticle& part) const;

  std::vector<sim::TrackIDE> HitToTrackIDEs(const recob::Hit& hit);
  std::vector<sim::TrackIDE> ChannelToTrackIDEs(Int_t channel, double hit_start_time, double hit_end_time) const;
  std::vector<sim::IDE> TrackIDsAndEnergies(const sim::SimChannel& chan, Int_t startTDC, Int_t endTDC) const;
  sim::SimChannel* FindSimChannel(Int_t channel)	const;
  Double_t TPCTick2TDC(Double_t tick) const;
  struct CompareByTDC;
  TDCIDEs_t::const_iterator findClosestTDCIDE(const sim::SimChannel& chan, StoredTDC_t tdc) const;
  double GetTrueMomentumInTPC(const simb::MCParticle& part) const;
#endif

  
  double GetTrackMomentum(double trkrange, int pdg) const;

  const recob::Track*  GetPFParticleTrack(const recob::PFParticle &particle) const;
  const recob::Shower* GetPFParticleShower(const recob::PFParticle &particle) const;

  const TVector3 GetPFParticleVertex(const recob::PFParticle &particle) const;

  const std::vector<const recob::PFParticle*> GetPFParticlesFromBeamSlice(const std::string particleLabel) const;
  const std::vector<const recob::PFParticle*> GetPFParticlesFromSlice(const unsigned short slice, const std::string particleLabel) const;
  const std::map<unsigned int,std::vector<const recob::PFParticle*> > GetPFParticleSliceMap(const std::string particleLabel) const;
  const std::map<unsigned int,std::vector<const recob::PFParticle*> > SliceMapHelper(const std::string particleLabel, bool primaryOnly) const;
  unsigned short GetPFParticleSliceIndex(const recob::PFParticle &particle, const std::string particleLabel) const;
  unsigned short GetBeamSlice(const std::string particleLabel) const;
  bool IsBeamParticle(const recob::PFParticle &particle, const std::string particleLabel) const;
  bool FindBoolInMetaData(const recob::PFParticle &particle, const std::string particleLabel, const std::string entry) const;
  const std::map<std::string,float> GetPFParticleMetaData(const recob::PFParticle &particle, const std::string particleLabel) const;


  Float_t ComputedEdxfromdQdx(Float_t prim_dqdx);
  Float_t ComputeCalibratedDqDx(Float_t Cx, Float_t prim_dqdx, Float_t prim_hitx);

  Float_t ComputeKineticEnergy(const AnaParticle &part, Int_t plane);

protected:


  AnaSpill* _spill;
  
  std::string _previousFile;
  Int_t _previousRunID;
  Int_t _previousSubrunID;
  Int_t _previousRefEventID;

  TH1F *cali_factor;


 protected:

  // TChains   
  TChain *eventsTree;

  Int_t Entries; 
  Int_t Counter; 

  Bool_t _isMC;

  art::EventAuxiliary* EventInfo;

#ifndef ISMC
  art::Wrapper<vector<beam::ProtoDUNEBeamEvent> >* BEAM;
#endif
  art::Wrapper<vector<anab::Calorimetry> >* CALOs;
  art::Wrapper<vector<anab::ParticleID> >*  PIDs;
  art::Wrapper<vector<recob::PFParticle> >* PFParticles;
  art::Wrapper<vector<recob::Track> >*      Tracks;
  art::Wrapper<vector<recob::Shower> >*     Showers;
  art::Wrapper<vector<recob::Vertex> >*     Vertices;
  art::Wrapper<vector<recob::SpacePoint> >* SpacePoints;
#ifdef ISMC
  art::Wrapper<vector<simb::MCParticle> >*  MCParticles;
  art::Wrapper<vector<simb::MCTruth> >*     MCTruths;
  art::Wrapper<vector<sim::SimChannel> >*   SimChannels;
#endif
  art::Wrapper<vector<recob::Hit> >*        Hits;

  
  art::Wrapper<vector<larpandoraobj::PFParticleMetadata> >* MetaData;

#ifndef ISMC
  art::Wrapper<vector<raw::ctb::pdspctb> >*      CTB;  
#endif

  art::Wrapper<art::Assns<recob::Track,recob::Hit       , void> >* Hits_Tracks;
  art::Wrapper<art::Assns<recob::Track,anab::ParticleID , void> >* PIDs_Tracks;
  art::Wrapper<art::Assns<recob::Track,anab::Calorimetry, void> >* CALOs_Tracks;

  art::Wrapper<art::Assns<recob::PFParticle,recob::Track, void> >* PFParticles_Tracks;
  art::Wrapper<art::Assns<recob::PFParticle,recob::Shower,void> >* PFParticles_Showers;
  art::Wrapper<art::Assns<recob::PFParticle,recob::Vertex,void> >* PFParticles_Vertices;


  art::Wrapper<art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata, void> >* PFParticles_MetaData;

  
#ifdef ISMC
  map<unsigned short,vector<sim::IDE> >* fTDCIDEs;
#endif
  
  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Char_t SoftwareVersion[50];
  Float_t POTPerSpill;
};



#endif

