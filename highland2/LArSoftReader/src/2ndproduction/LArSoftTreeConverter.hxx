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
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include <TProfile.h>

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


  struct cnnOutput2D{

    cnnOutput2D(){track=0; em=0; michel=0; none=0; nHits=0;}
    
    double track;
    double em;
    double michel;
    double none;
    size_t nHits;
  };

  enum beamPDG{
    kElectron = 11,
    kMuon = 13,
    kPion = 211,
    kKaon = 321,
    kProton = 2212,
    kDeuteron = 1000010020
  };


  struct PossibleParticleCands {
    public:
      bool electron = false;
      bool muon = false;
      bool pion = false;
      bool kaon = false;
      bool proton = false;
      bool deuteron = false;
    /*
    inline PossibleParticleCands operator&&(const PossibleParticleCands & b) const {
      return {electron && b.electron, muon && b.muon, pion && b.pion, kaon && b.kaon, proton && b.proton, deuteron && b.deuteron};
    }
    inline PossibleParticleCands operator||(const PossibleParticleCands & b) const {
        return {electron || b.electron, muon || b.muon, pion || b.pion, kaon || b.kaon, proton || b.proton, deuteron || b.deuteron};
    }
    */
    inline operator std::string () const { // overload cast to string
        std::string result = "PossibleParticleCands: [ ";
        if (electron) result += "e ";
        if (muon) result += "mu ";
        if (pion) result += "pi ";
        if (kaon) result += "k ";
        if (proton) result += "p ";
        if (deuteron) result += "e ";
        result += "]";
        return result;
    }
    inline std::vector<int> getPDGCodes() const {
        std::vector<int> result;
        if (electron) result.push_back(kElectron);
        if (muon) result.push_back(kMuon);
        if (pion) result.push_back(kPion);
        if (kaon) result.push_back(kKaon);
        if (proton) result.push_back(kProton);
        if (deuteron) result.push_back(kDeuteron);
        return result;
    }
  };

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


  Float_t ComputedEdxfromdQdx(Float_t prim_dqdx, Float_t EField=0.50);
  Float_t ComputeCalibratedDqDx(Float_t Cx, Float_t prim_dqdx, Float_t prim_hitx);

  Float_t ComputeKineticEnergy(const AnaParticle &part, Int_t plane);

  //  cnnOutput2D GetCNNOutputFromPFParticle( const recob::PFParticle & part, const art::Event & evt, const anab::MVAReader<recob::Hit,4> & CNN_results,  protoana::ProtoDUNEPFParticleUtils & pfpUtil, std::string fPFParticleTag );
  //  cnnOutput2D GetCNNOutputFromPFParticle( const recob::PFParticle & part, const anab::MVAReader<recob::Hit,4> & CNN_results,  std::string fPFParticleTag );
  //  const std::vector< art::Ptr< recob::Hit > > GetPFParticleHits_Ptrs(const recob::PFParticle &particle, art::Event const &evt, const std::string particleLabel) const;
  //  const std::vector<const recob::Cluster*> GetPFParticleClusters(const recob::PFParticle &particle, art::Event const &evt, const std::string particleLabel) const;


#ifndef ISMC  
  const std::vector< short > & GetActiveFibers(const std::vector<std::pair<std::string,beam::FBM> >& fiberMonitors,  const std::string& FBMName);

  std::vector< int > GetPID( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum );
  PossibleParticleCands GetPIDCandidates( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum );
  PossibleParticleCands GetPIDCandidates_CERNCalib( beam::ProtoDUNEBeamEvent const & beamevt, double nominal_momentum );
#endif

  std::vector<anab::Calorimetry*> GetRecoTrackCalorimetry(const recob::Track &track, const std::string trackModule, const std::string caloModule );
  std::pair< double, int > Chi2PID( const std::vector< double > & track_dedx, const std::vector< float > & range, TProfile * profile );
  std::pair< double, int > GetChi2PID( const recob::Track &track, const std::string caloModule, TProfile * profile );
  std::vector< double >  GetCalibratedCalorimetry(const recob::Track &track, UInt_t planeID, const std::string trackModule, const std::string caloModule );
  double tot_Ef( double x, double y, double z );


  const std::vector<int> GetPFParticleHitsIndices(const recob::PFParticle &part);
  cnnOutput2D GetCNNOutputFromPFParticle( const recob::PFParticle & part,std::string fPFParticleTag );
  int GetMVAIndex(const std::string& tag, const std::string& name);

  
      std::string X_correction_name;
      TFile * X_correction_file;

      std::string YZ_correction_name;
      TFile * YZ_correction_file;

      std::string E_field_correction_name;
      TFile * E_field_file;

      TH1F * X_correction_hist;
      TH2F * YZ_neg;
      TH2F * YZ_pos;

      TH3F * ex_neg;
      TH3F * ey_neg;
      TH3F * ez_neg;

      TH3F * ex_pos;
      TH3F * ey_pos;
      TH3F * ez_pos;

    ////New section -- mechanical class members
  std::map< int, TProfile* > templates;

  
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
  art::Wrapper<vector<anab::Calorimetry> >* CALOsSCE;
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
  art::Wrapper<art::Assns<recob::Track,recob::Hit       , void> >* Hits_Showers;
  art::Wrapper<art::Assns<recob::Track,anab::ParticleID , void> >* PIDs_Tracks;
  art::Wrapper<art::Assns<recob::Track,anab::Calorimetry, void> >* CALOs_Tracks;
  art::Wrapper<art::Assns<recob::Track,anab::Calorimetry, void> >* CALOsSCE_Tracks;

  art::Wrapper<art::Assns<recob::PFParticle,recob::Track, void> >* PFParticles_Tracks;
  art::Wrapper<art::Assns<recob::PFParticle,recob::Shower,void> >* PFParticles_Showers;
  art::Wrapper<art::Assns<recob::PFParticle,recob::Vertex,void> >* PFParticles_Vertices;


  art::Wrapper<art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata, void> >* PFParticles_MetaData;

  art::Wrapper<vector<anab::FeatureVector<4> > >* MVA;
  art::Wrapper<vector<anab::MVADescription<4> > >* MVADescription;
  
  
#ifdef ISMC
  map<unsigned short,vector<sim::IDE> >* fTDCIDEs;
#endif


  TFile* dEdX_template_file;
  
  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Char_t SoftwareVersion[50];
  Float_t POTPerSpill;
};



#endif

