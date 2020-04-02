#ifndef DataClasses_hxx
#define DataClasses_hxx

#include "BaseDataClasses.hxx"
#include "ParticleId.hxx"


/// Representation of a true Monte Carlo vertex.
class AnaTrueVertex: public AnaTrueVertexB{
public :

  AnaTrueVertex();
  virtual ~AnaTrueVertex(){}

  /// Clone this object.
  virtual AnaTrueVertex* Clone() {
    return new AnaTrueVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueVertex(const AnaTrueVertex& vertex);

public:

  /// The neutrino event generator (Genie, Nuance, ...) reaction code
  Int_t ReacCode;

  /// The true (unit) direction of the incoming neutrino.
  Float_t NuDir[3];

  /// The PDG code of the target nucleus.
  Int_t TargetPDG;

  /// Neutrino parent PDG code
  Int_t NuParentPDG;

  /// Decay point of the neutrino parent.
  Float_t NuParentDecPoint[4];

  /// Accumulated cut level for all selections and cut branches. Tell us if a true vertex has been selected or not
  std::vector< std::vector<Int_t> > AccumLevel; //!

  /// The Q2 of the true interaction
  Float_t Q2;

  /// The PDG code of the primary outgoing electron/muon.
  Int_t LeptonPDG;

  /// The momentum of the primary outgoing electron/muon.
  Float_t LeptonMom;

  /// The direction of the primary outgoing electron/muon.
  Float_t LeptonDir[3];

  /// The momentum of the primary outgoing protons listed first (likely the interacted one).
  Float_t ProtonMom;

  /// The direction of the primary outgoing protons listed first (likely the interacted one).
  Float_t ProtonDir[3];

  /// The momentum of the primary outgoing pions listed first (likely the interacted one).
  Float_t PionMom;

  /// The direction of the primary outgoing pions listed first (likely the interacted one).
  Float_t PionDir[3];

  /// Array to count the outgoing primary particles of each type (
  Int_t NPrimaryParticles[Int_t(ParticleId::kLast)+1];

  /// The total number of primary baryons that were ejected.
  // OBSOLETE, NOT FILLED, TO BE DELETED, use NPrimaryParticles instead
  Int_t NBaryons;

  /// The momentum of the target nucleus.
  Float_t TargetMom;

  /// The direction of the target nucleus.
  Float_t TargetDir[3];

  /// Vector of pointers to AnaParticleB associated with this true vertex
  /// Do we really need this? It makes stuff more complicated
  std::vector<AnaParticleB*> ReconParticles; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this true vertex
  std::vector<AnaVertexB*> ReconVertices; //!
};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticle: public AnaTrueParticleB{
public :

  AnaTrueParticle();
  virtual ~AnaTrueParticle(){}

  /// Clone this object.
  virtual AnaTrueParticle* Clone() {
    return new AnaTrueParticle(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueParticle(const AnaTrueParticle& track);

public:

  /// The ID of the primary particle that created this particle. You can use the
  /// anaUtils::GetTrueTrackByID(Int_t ID) or anaUtils::GetPrimaryTrueTrack(AnaTrueParticleB* track)
  /// functions to find the AnaTrueParticleB representing the primary particle.
  Int_t PrimaryID;

  /// The purity with which this particle was matched to a reconstructed object.
  Float_t Purity;


  /// The particle length
  Float_t Length;

  /// The particle length inside the TPC
  Float_t LengthInTPC;
  
  /// The true momentum at the TPC entrance
  Float_t MomentumInTPC;
   
  /// The bunch in which this true interaction occurred, based on the time of
  /// this true particle Start Position and the bunching used for the reconstructed objects.
  Int_t Bunch;

  /// The index of the AnaTrueVertexB of the interaction that created this AnaTrueParticleB. Doesn't
  /// match to any variable in the AnaTrueVertexB class, but is needed to set up
  /// the Vertex pointer, which should be used to access the vertex that
  /// created this particle.
  Int_t VertexIndex;

  /// Vector of pointers to AnaParticle's associated with this true particle
  std::vector<AnaParticleB*> ReconParticles; //!


  /// Vector of Daughters TrueParticles
  std::vector<Int_t> Daughters; 
};

const UInt_t NMAXHITSPERPLANE = 300;

/// Extension to AnaParticleB to be used by aggregation
class AnaParticleE{
public :

  AnaParticleE();
  virtual ~AnaParticleE();

  /// Clone this object.
  virtual AnaParticleE* Clone() {
    return new AnaParticleE(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleE(const AnaParticleE& track);

public:

  /// True-reco matching efficiency
  Float_t TrueEff;

  /// True-reco matching purity
  Float_t TruePur;

  /// Number of hits in each wire plane
  Int_t NHitsPerPlane[3];

  /// Residual range for each wire in each plane
  Float_t ResidualRange[3][NMAXHITSPERPLANE];

  /// dEdx for each wire in each plane
  Float_t dEdx[3][NMAXHITSPERPLANE];
  Float_t dEdx_corr[3][NMAXHITSPERPLANE];

  /// dQdx for each wire in each plane
  Float_t dQdx[3][NMAXHITSPERPLANE];  
  Float_t dQdx_corr[3][NMAXHITSPERPLANE];  

  /// dQdx for each wire in each plane
  Float_t HitX[3][NMAXHITSPERPLANE]; 
  Float_t HitY[3][NMAXHITSPERPLANE]; 
  Float_t HitZ[3][NMAXHITSPERPLANE]; 
 
  /// Average energy deposited in the detector
  Float_t AveragedEdx;

  /// Average chage deposited in the detector
  Float_t AveragedQdx;

  /// The error on the reconstructed momentum.
  Float_t MomentumError;

  /// The number of degrees of freedom when the track was fitted with a Kalman filter.
  Int_t NDOF;

  /// The chi2 value when the track was fitted using a Kalman filter.
  Float_t Chi2;

  /// Particle ID hypothesis used in the fit (if any)
  Int_t FitPDG;
  
  /// PDG of the most probable particle hypothesis used at reconstruction level
  Int_t ReconPDG[3]; 
   
  /// The bunch of the track, based on the PositionStart.T()
  Int_t Bunch;

  /// Position and charge of 1 or 2 hits at the edges: if the more
  /// upstream/downstream hit does not contain both X and Y info,
  /// a second hit (containing the missing info) is also saved.
  /// (Time and PositionError are also available in oaAnalysis files)
  /// (variables not in FlatTreeConverter yet)
  TVector3 UpstreamHits_Position[2];
  TVector3 DownstreamHits_Position[2];
  Float_t UpstreamHits_Charge[2];
  Float_t DownstreamHits_Charge[2];

  /// PID variables
  Float_t PID[3][10];

  Float_t PIDA[3];

  Float_t Chi2Proton;
  Float_t Chi2ndf;

  Float_t CNNscore[3];
  
  /// CALO variables
  Float_t CALO[3][10];
  
  /// Momentum by range for muon and proton hypotheses
  Float_t RangeMomentum[2];

  /// Vector of daugthers particles
  std::vector<AnaRecObjectC*> Daughters;
  
  /// The pointer to the most primary AnaVertexB (global vertex) associated with this track
  AnaVertexB* ReconVertex; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this track
  std::vector<AnaVertexB*> ReconVertices; //!

  /// The reconstructed momentum of the track, at the most primary global vertex (if exists).
  Float_t MomentumAtVertex;
  //  std::vector<Float_t> MomentumAtVertices;

  /// The reconstructed direction of the track at the most primary global vertex (if exists).
  Float_t DirectionAtVertex[3];
  //  std::vector<TVector3> DirectionAtVertices;

};

/// AnaParticle
class AnaParticle: public AnaParticleMomB, public AnaParticleE{
public :

  AnaParticle():AnaParticleMomB(),AnaParticleE(){}
  virtual ~AnaParticle(){}

  /// Clone this object.
  virtual AnaParticle* Clone() {
    return new AnaParticle(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticle(const AnaParticle& part):AnaParticleMomB(part),AnaParticleE(part){}
};

class AnaSubTrack{
public :

  AnaSubTrack();
  virtual ~AnaSubTrack(){}

  /// Clone this object.
  virtual AnaSubTrack* Clone() {
    return new AnaSubTrack(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  //  ClassDef(AnaSubTrack, 1);

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSubTrack(const AnaSubTrack& seg);

public :

  /// The number of hits in the reconstructed object.
  //  Int_t NHits;

  /// The length of the reconstructed object, accounting for curvature etc.
  Float_t Length;

  /// The direction at the end of the reconstructed track.
  //  Float_t DirectionEnd[3];

  /// The unique ID of the reconstructed object for matchign between global and local reconstruction
  //  Int_t UniqueID;

};

/// Representation of a TPC segment of a global track.
class AnaSubdet2Particle:public AnaSubdet2ParticleB, public AnaSubTrack{
public :

  AnaSubdet2Particle();
  virtual ~AnaSubdet2Particle();

  using AnaSubdet2ParticleB::Print;

  /// Clone this object.
  virtual AnaSubdet2Particle* Clone() {
    return new AnaSubdet2Particle(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSubdet2Particle(const AnaSubdet2Particle& seg);

public:


  /// Muon pull of the segment: (dEdxMeas-dEdxexpMuon)/dEdxSigmaMuon.
  Float_t Pullmu;

  /// Electron pull of the segment: (dEdxMeas-dEdxexpEle)/dEdxSigmaEle.
  Float_t Pullele;

  /// Proton pull of the segment: (dEdxMeas-dEdxexpProton)/dEdxSigmaProton.
  Float_t Pullp;

  /// Pion pull of the segment: (dEdxMeas-dEdxexpPion)/dEdxSigmaPion.
  Float_t Pullpi;

  /// Kaon pull of the segment: (dEdxMeas-dEdxexpPion)/dEdxSigmaKaon.
  Float_t Pullk;

  /// Expected dE/dx for a proton, based on the reconstructed momentum.
  Float_t dEdxexpKaon;

  /// Expected error on the dE/dx measurement, for the proton hypothesis.
  Float_t dEdxSigmaKaon;

  /// The purity of the associated true track with respect to this Subdet2 segment.
  /// TrueTrack->Purity gives the purity of the true track with respect to the
  /// global object.
  Float_t Purity;

  //  ClassDef(AnaSubdet2Particle, 1);
};

/// Representation of a global track.
class AnaTrack: public AnaTrackB, public AnaParticleE{
public :

  AnaTrack();
  virtual ~AnaTrack();

  /// Clone this object.
  virtual AnaTrack* Clone() {
    return new AnaTrack(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrack(const AnaTrack& track);

public:

  /// A different representation of the detectors used by this track.
  Int_t Detectors;
};

class AnaVertex:public AnaVertexB{
public :

  AnaVertex();
  virtual ~AnaVertex(){}

  /// Clone this object.
  virtual AnaVertex* Clone() {
    return new AnaVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaVertex(const AnaVertex& vertex);

public:

  /// The variance values of the fit using a Kalman filter.
  Float_t Variance[4];

  /// The chi2 value of the fit using a Kalman filter.
  Float_t Chi2;

  /// The number of degrees of freedom of the fit using a Kalman filter.
  Int_t NDOF;
};

/// Representation of the beam information, including POT and quality.
class AnaBeam: public AnaBeamB{
public :

  AnaBeam();
  virtual ~AnaBeam();

  /// Clone this object.
  virtual AnaBeam* Clone() {
    return new AnaBeam(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;
  
protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBeam(const AnaBeam& beam);

public:

  /// The POT for this spill. For data, this comes from the Beam Summary Data.
  Float_t POT;

  /// Spill number 
  Int_t Spill;

  /// The beam particle
  AnaParticleMomB* BeamParticle;

  /// Other relevant beam info
  int BeamTrigger;
  double TOF;
  int    CerenkovStatus[2];
  double CerenkovTime[2];
  double CerenkovPressure[2];
  double BeamTrackTime;
  double BeamMomentum;
  double BeamMomentumInTPC;

  int nFibers[3];
  size_t nMomenta;  
  size_t nTracks;  
  
  std::vector< int > PDGs;

};

/// Representation of the data quality flags.
class AnaDataQuality: public AnaDataQualityB {
public:

  AnaDataQuality();
  virtual ~AnaDataQuality() {}

  /// Clone this object.
  virtual AnaDataQuality* Clone() {
    return new AnaDataQuality(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDataQuality(const AnaDataQuality& dq);

public:

  /// Flag for the data quality as a whole. The values the flag can take aren't
  /// documented in the oaAnalysis code, but the important thing is that
  /// 0 means good quality.
  Int_t GlobalFlag;

  /// Flags for each sub-detector. The values each flag can take aren't
  /// documented in the oaAnalysis code, but the important thing is that
  /// 0 means good quality.
  ///
  /// Indexing is:
  Int_t DetFlag[7];
};


class AnaEventInfo:public AnaEventInfoB{
public :

  AnaEventInfo();
  virtual ~AnaEventInfo(){}

  /// Clone this object.
  virtual AnaEventInfo* Clone() {
    return new AnaEventInfo(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventInfo(const AnaEventInfo& event);

public:

  /// The subrun number.
  //  Int_t SubRun;

  /// UNIX timestamp of the event, from the MCM (master clock module).
  Int_t EventTime;
};



/// Representation of the trigger bits
class AnaTrigger {
public:

  AnaTrigger();
  virtual ~AnaTrigger() {}

  /// Clone this object.
  virtual AnaTrigger* Clone() {
    return new AnaTrigger(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrigger(const AnaTrigger& tr);

public:

  // The numvber of triggers
  Int_t nTriggers;

  /// The trigger times
  Int_t* Time;

  /// The trigger IDs
  Int_t* ID;
};

/// An AnaBunch contains all the reconstructed objects in a given time window.
/// The normal bunches are indexed from 0-7.
///
/// The bunch windows for each run period are defined in the text file
/// $HIGHLEVELANALYSISIOROOT/data/BunchPosition.dat, where the run period of -1
/// means Monte Carlo.
///
/// Each bunch has a weighting associated with it, to allow for flux
/// re-weighting.
class AnaBunch: public AnaBunchB{
public :

  AnaBunch();
  virtual ~AnaBunch();

  /// Clone this object.
  virtual AnaBunch* Clone() {
    return new AnaBunch(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunch(const AnaBunch& bunch);

public:
};

/// The top-level object for representing information from the input files.
/// A single AnaSpill contains information from an entire event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpill: public AnaSpillB{
public :

  AnaSpill();
  virtual ~AnaSpill();

  /// Clone this object.
  virtual AnaSpill* Clone() {
    AnaSpill* spill = new AnaSpill(*this);
    spill->RedoLinks();
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks();

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpill(const AnaSpill& spill);

  /// Add to a particle a given associated vertex (not in the base class)
  virtual void associateVertexToParticle(AnaParticleB*, AnaVertexB*) const;

  /// Add to a true vertex a given associated reconstructed vertex (not in the base class)
  virtual void associateVertexToTrueVertex(AnaVertexB*) const;

  /// Add to a true particle a given associated reconstructed particle (not in the base class)
  virtual void associateParticleToTrueParticle(AnaParticleB*) const;

public:
  /// The trigger flags for this spill.
  AnaTrigger Trigger;
};


class AnaEvent: public AnaEventB{
public :

  AnaEvent();
  AnaEvent(const AnaSpill& spill, const AnaBunch& bunch);
  virtual ~AnaEvent();

  /// Clone this object.
  virtual AnaEvent* Clone() {
    AnaEvent* spill = new AnaEvent(*this);
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEvent(const AnaEvent& event);

public:
  /// The trigger flags for this event.
  AnaTrigger Trigger;
};


class AnaRecTrueMatch: public AnaRecTrueMatchB{
public :

  AnaRecTrueMatch():AnaRecTrueMatchB(){}
  virtual ~AnaRecTrueMatch(){}

};



#endif
