#define DataClasses_C

#include "DataClasses.hxx"
#include "AnalysisUtils.hxx"
#include "TMath.h"

// define a constant value for uninitialised parameters
const Float_t  kFloatUnassigned = -999.;
//const Double_t kDoubleUnassigned = -999.;
//const UInt_t   kUnassigned      = 0xDEADBEEF;
//const short    kShortUnassigned = 0xFF;
const Int_t    kIntUnassigned = -999;

//********************************************************************
AnaSubTrack::AnaSubTrack(){
//********************************************************************
//  NHits          = -999;
  Length         = kFloatUnassigned;
  //  anaUtils::ReserveArray(DirectionEnd, 3);
  //  UniqueID       = -999;
}

//********************************************************************
AnaSubTrack::AnaSubTrack(const AnaSubTrack& seg){
//********************************************************************
//  NHits          = seg.NHits;
  Length         = seg.Length;
  //  anaUtils::CopyArray(seg.DirectionEnd, DirectionEnd, 3);
  //  UniqueID       = seg.UniqueID;
}

//********************************************************************
void AnaSubTrack::Print() const{
//********************************************************************

  std::cout << "-------- AnaSubTrack --------- " << std::endl;

  //  std::cout << "NHits:        " << NHits << std::endl;
  std::cout << "Length:       " << Length << std::endl;
  //  std::cout << "UniqueID:     " << UniqueID << std::endl;
}

//********************************************************************
AnaParticleE::AnaParticleE(){
//********************************************************************

  TrueEff       = kFloatUnassigned;
  TruePur       = kFloatUnassigned;
  
  AveragedEdx   = kFloatUnassigned;
  AveragedQdx   = kFloatUnassigned;
  MomentumError = kFloatUnassigned;

  NDOF          = kIntUnassigned;
  Chi2          = kFloatUnassigned;
  Bunch         = kIntUnassigned;

  for (int i=0; i<2; i++) {
    UpstreamHits_Position[i]   = TVector3(kFloatUnassigned, kFloatUnassigned, kFloatUnassigned);
    DownstreamHits_Position[i] = TVector3(kFloatUnassigned, kFloatUnassigned, kFloatUnassigned);
    UpstreamHits_Charge[i]   = kFloatUnassigned;
    DownstreamHits_Charge[i] = kFloatUnassigned;
  }


  ReconVertex = NULL;
  ReconVertices.clear();
  anaUtils::ReserveArray(DirectionAtVertex, 3);
  MomentumAtVertex    = kFloatUnassigned;

  Daughters.clear();
  DaughtersIDs.clear();
  
}

//********************************************************************
AnaParticleE::~AnaParticleE(){
//********************************************************************

  // TODO. We must uncomment for MiniTree reading
/*
  for (UInt_t i=0;i<Daughters.size();i++){
    delete Daughters[i];
  }
  Daughters.clear();
*/
}

//********************************************************************
AnaParticleE::AnaParticleE(const AnaParticleE& part){
//********************************************************************

  TrueEff       = part.TrueEff;
  TruePur       = part.TruePur;

  
  AveragedQdx   = part.AveragedQdx;
  AveragedEdx   = part.AveragedEdx;
  MomentumError = part.MomentumError;

  NDOF          = part.NDOF;
  Chi2          = part.Chi2;
  Bunch         = part.Bunch;

  for (int i=0; i<2; i++) {
    UpstreamHits_Position[i] = part.UpstreamHits_Position[i];
    DownstreamHits_Position[i] = part.DownstreamHits_Position[i];
    UpstreamHits_Charge[i] = part.UpstreamHits_Charge[i];
    DownstreamHits_Charge[i] = part.DownstreamHits_Charge[i];
  }


  // The most lower PrimaryIndex associated global vertex (if it exists).
  ReconVertex = NULL;//part.ReconVertex;

  ReconVertices.clear();
  /*
  for (UInt_t i=0;i<part.ReconVertices.size();i++)
    ReconVertices.push_back(part.ReconVertices[i]);
  */
  anaUtils::CopyArray(part.DirectionAtVertex, DirectionAtVertex, 3);
  MomentumAtVertex = part.MomentumAtVertex;

  // Daughters should not be clone since the pointer leaves in the Particle vector
  // This vector is filled in AnaBunch, using te IDS
  Daughters.clear();
  
  // Instead a copy of the daughters IDs is made
  DaughtersIDs.clear();
  for (UInt_t j=0;j<part.DaughtersIDs.size();j++){
    DaughtersIDs.push_back(part.DaughtersIDs[j]);
  }
}

//********************************************************************
void AnaParticleE::Print() const{
//********************************************************************

  std::cout << "-------- AnaParticleE --------- " << std::endl;

  std::cout << "Chi2:                    " << Chi2 << std::endl;

  std::cout << "Bunch:                   " << Bunch << std::endl;
  std::cout << "NReconVertices:          " << (int)ReconVertices.size() << std::endl;
  std::cout << "MomentumAtVertex:        " << MomentumAtVertex << std::endl;
  std::cout << "DirectionAtVertex:       " << DirectionAtVertex[0] << " " << DirectionAtVertex[1] << " " << DirectionAtVertex[2] << std::endl;    
  std::cout << "AveragedEdx:             " << AveragedEdx << std::endl;
  std::cout << "AveragedQdx:             " << AveragedQdx << std::endl;
  std::cout << "#Daughters               " << Daughters.size() << std::endl;


  
}

//********************************************************************
void AnaParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaParticle --------- " << std::endl;

  AnaParticleMomB::Print();
  AnaParticleE::Print();
}

//********************************************************************
AnaTrueVertex::AnaTrueVertex(): AnaTrueVertexB(){
//********************************************************************

  ReacCode      = kIntUnassigned;
  TargetPDG     = kIntUnassigned;
  anaUtils::ReserveArray(NuDir, 3);
  NuParentPDG   = kIntUnassigned;
  anaUtils::ReserveArray(NuParentDecPoint, 4);

  NBaryons      = kIntUnassigned;
  Q2            = kFloatUnassigned;
  LeptonPDG     = kIntUnassigned;
  anaUtils::ReserveArray(TargetDir, 3);
  anaUtils::ReserveArray(LeptonDir, 3);
  anaUtils::ReserveArray(ProtonDir, 3);
  anaUtils::ReserveArray(PionDir, 3);
  TargetMom     = kFloatUnassigned;
  LeptonMom     = kFloatUnassigned;
  ProtonMom     = kFloatUnassigned;
  PionMom       = kFloatUnassigned;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=0;

  AccumLevel.clear();

  ReconParticles.clear();
  ReconVertices.clear();
}

//********************************************************************
AnaTrueVertex::AnaTrueVertex(const AnaTrueVertex& vertex):AnaTrueVertexB(vertex){
//********************************************************************

  ReacCode      = vertex.ReacCode;
  TargetPDG     = vertex.TargetPDG;
  anaUtils::CopyArray(vertex.NuDir, NuDir, 3);
  NuParentPDG   = vertex.NuParentPDG;
  anaUtils::CopyArray(vertex.NuParentDecPoint, NuParentDecPoint, 3);

  NBaryons      = vertex.NBaryons;
  Q2            = vertex.Q2;
  LeptonPDG     = vertex.LeptonPDG;
  anaUtils::CopyArray(vertex.TargetDir, TargetDir, 3);
  anaUtils::CopyArray(vertex.LeptonDir, LeptonDir, 3);
  anaUtils::CopyArray(vertex.ProtonDir, ProtonDir, 3);
  anaUtils::CopyArray(vertex.PionDir,   PionDir,   3);
  TargetMom     = vertex.TargetMom;
  LeptonMom     = vertex.LeptonMom;
  ProtonMom     = vertex.ProtonMom;
  PionMom       = vertex.PionMom;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=vertex.NPrimaryParticles[i];
/*
  PreFSIParticles.clear();
  for (UInt_t i=0;i<vertex.PreFSIParticles.size();i++)
    PreFSIParticles.push_back(vertex.PreFSIParticles[i]);
*/
  AccumLevel.resize(vertex.AccumLevel.size());
  for (UInt_t i=0;i<vertex.AccumLevel.size();i++){
    AccumLevel[i].resize(vertex.AccumLevel[i].size());
    for (UInt_t j=0;j<vertex.AccumLevel[i].size();j++){
      AccumLevel[i][j]=vertex.AccumLevel[i][j];
    }
  }

  ReconParticles.clear();
  /*
  for (UInt_t i=0;i<vertex.ReconParticles.size();i++)
    ReconParticles.push_back(vertex.ReconParticles[i]);
  */
  ReconVertices.clear();
  /*
  for (UInt_t i=0;i<vertex.ReconVertices.size();i++)
    ReconVertices.push_back(vertex.ReconVertices[i]);
  */
}

//********************************************************************
void AnaTrueVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueVertex --------- " << std::endl;

  AnaTrueVertexB::Print();

  std::cout << "ReacCode:         " << ReacCode << std::endl;
  std::cout << "TargetPDG:        " << TargetPDG << std::endl;


  std::cout << "NuDir:            " << NuDir[0]  << " " << NuDir[1]  << " " << NuDir[2] << std::endl;

  std::cout << "NuParentPDG:      " << NuParentPDG << std::endl;
  std::cout << "NuParentDecPoint: " << NuParentDecPoint[0] << " " << NuParentDecPoint[1]  << " " << NuParentDecPoint[2] << " " << NuParentDecPoint[3] << std::endl;


  std::cout << "NBaryons:         " << NBaryons << std::endl;
  std::cout << "LeptonPDG:        " << LeptonPDG << std::endl;
  std::cout << "Q2:               " << Q2 << std::endl;

  std::cout << "TargetDir:        " << TargetDir[0]  << " " << TargetDir[1]  << " " << TargetDir[2] << std::endl;
  std::cout << "LeptonDir:        " << LeptonDir[0]  << " " << LeptonDir[1]  << " " << LeptonDir[2] << std::endl;
  std::cout << "ProtonDir:        " << ProtonDir[0]  << " " << ProtonDir[1]  << " " << ProtonDir[2] << std::endl;
  std::cout << "PionDir:          " << PionDir[0]    << " " << PionDir[1]    << " " << PionDir[2]   << std::endl;

  std::cout << "TargetMom:        " << TargetMom << std::endl;
  std::cout << "LeptonMom:        " << LeptonMom << std::endl;
  std::cout << "ProtonMom:        " << ProtonMom << std::endl;
  std::cout << "PionMom:          " << PionMom << std::endl;

  std::cout << "NReconParticles:  " << (int)ReconParticles.size() << std::endl;
  std::cout << "NReconVertices:   " << (int)ReconVertices.size() << std::endl;
}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(): AnaTrueParticleB(){
//********************************************************************

  PrimaryID     = kIntUnassigned;
  Purity        = kFloatUnassigned;
  Bunch         = kIntUnassigned;
  VertexIndex   = kIntUnassigned;
  Length        = kFloatUnassigned;

  ReconParticles.clear();
  Daughters.clear();
}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(const AnaTrueParticle& truePart):AnaTrueParticleB(truePart){
//********************************************************************

  PrimaryID   = truePart.PrimaryID;
  Purity      = truePart.Purity;
  Bunch       = truePart.Bunch;
  VertexIndex = truePart.VertexIndex;
  Length      = truePart.Length;
  
  ReconParticles.clear();

  for (UInt_t i=0;i<truePart.Daughters.size();i++)
    Daughters.push_back(truePart.Daughters[i]);

  
  /*
  for (UInt_t i=0;i<truePart.ReconParticles.size();i++)
    ReconParticles.push_back(truePart.ReconParticles[i]);
  */
}

//********************************************************************
void AnaTrueParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueParticle --------- " << std::endl;

  AnaTrueParticleB::Print();

  std::cout << "NReconParticles: " << (int)ReconParticles.size() << std::endl;
  std::cout << "VertexIndex:     " << VertexIndex << std::endl;
  std::cout << "Length:          " << Length << std::endl;
  std::cout << "NDaughters:      " << (int)Daughters.size() << std::endl;
  if (Daughters.size())
    std::cout << "First dau ID:    " << Daughters[0] << std::endl;
}


//********************************************************************
AnaSubdet2Particle::AnaSubdet2Particle():AnaSubdet2ParticleB(),AnaSubTrack() {
//********************************************************************

  Pullmu  = kFloatUnassigned;
  Pullele = kFloatUnassigned;
  Pullp   = kFloatUnassigned;
  Pullpi  = kFloatUnassigned;
  Pullk   = kFloatUnassigned;
  dEdxexpKaon     = kFloatUnassigned;
  dEdxSigmaKaon   = kFloatUnassigned;
}

//********************************************************************
AnaSubdet2Particle::AnaSubdet2Particle(const AnaSubdet2Particle& seg):AnaSubdet2ParticleB(seg),AnaSubTrack(seg){
//********************************************************************

  Pullmu  = seg.Pullmu;
  Pullele = seg.Pullele;
  Pullp   = seg.Pullp;
  Pullpi  = seg.Pullpi;
  Pullk   = seg.Pullk;
  dEdxexpKaon   = seg.dEdxexpKaon;
  dEdxSigmaKaon   = seg.dEdxSigmaKaon;
}

//********************************************************************
void AnaSubdet2Particle::Print() const{
//********************************************************************

  std::cout << "-------- AnaSubdet2Particle --------- " << std::endl;

  AnaSubdet2ParticleB::Print();
  std::cout << "Pullmu:        " << Pullmu << std::endl;
  std::cout << "Pullele:       " << Pullele << std::endl;
  std::cout << "Pullp:         " << Pullp << std::endl;
  std::cout << "Pullpi:        " << Pullpi << std::endl;
  std::cout << "Pullk:         " << Pullk << std::endl;
}

//********************************************************************
AnaSubdet2Particle::~AnaSubdet2Particle(){
//********************************************************************
}



//********************************************************************
AnaTrack::AnaTrack():AnaTrackB(){
//********************************************************************

//  Length    = -999;
  Detectors = kIntUnassigned;

}

//********************************************************************
AnaTrack::~AnaTrack(){
//********************************************************************

}

//********************************************************************
AnaTrack::AnaTrack(const AnaTrack& track):AnaTrackB(track),AnaParticleE(track){
//********************************************************************

//  Length        = track.Length;
  Detectors     = track.Detectors;
  
}

//********************************************************************
void AnaTrack::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrack --------- " << std::endl;

  AnaParticleE::Print();
  AnaTrackB::Print();

  //  std::cout << "Length:               " << Length                      << std::endl;
}


//********************************************************************
AnaBunch::AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::~AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::AnaBunch(const AnaBunch& bunch):AnaBunchB(bunch){
  //AnaBunch::AnaBunch(const AnaBunch& bunch){
//********************************************************************


  // Associate to each particle all daughters (pointers) using the DaughterIDs
  for (UInt_t i=0;i<Particles.size();i++){
    AnaParticle* part = static_cast<AnaParticle*>(Particles[i]);
    for (UInt_t j=0;j<part->DaughtersIDs.size();j++){
      AnaParticleB* dau = anaUtils::GetParticleByID(*this,part->DaughtersIDs[j]);
      if (dau) part->Daughters.push_back(dau);      
    }
  }

  
/*
    Bunch  = bunch.Bunch;
    Weight = bunch.Weight;

    Tracks.clear();
    for (UInt_t i=0;i<bunch.Tracks.size();i++){
        AnaTrackB* clonedTrack = bunch.Tracks[i]->Clone();
        (static_cast<AnaTrack*>(clonedTrack))->ReconVertices.clear();
        Tracks.push_back(clonedTrack);
    }

    Vertices.clear();
    for (UInt_t i=0;i<bunch.Vertices.size();i++){
      AnaVertexB* clonedVertex = bunch.Vertices[i]->Clone();
      Vertices.push_back(clonedVertex);
      for (Int_t j=0;j<clonedVertex->nTracks;j++){
        for (UInt_t k=0;k<Tracks.size();k++){
          if (clonedVertex->Tracks[j] == bunch.Tracks[k]){
            clonedVertex->Tracks[j] = Tracks[k];
            (static_cast<AnaTrack*>(Tracks[k]))->ReconVertices.push_back(clonedVertex);
          }
        }
      }
    }
*/
}

//********************************************************************
void AnaBunch::Print() const{
//********************************************************************

  AnaBunchB::Print();

}


//********************************************************************
AnaSpill::AnaSpill():AnaSpillB(){
//********************************************************************

}

//********************************************************************
AnaSpill::~AnaSpill(){
//********************************************************************
}

//********************************************************************
AnaSpill::AnaSpill(const AnaSpill& spill):AnaSpillB(spill){
//********************************************************************

  Trigger = spill.Trigger;
}

//********************************************************************
void AnaSpill::RedoLinks(){
//********************************************************************

  // Reset the true->Reco links since truth is not cloned and the vectors of ReconParticles and ReconVertices are already filled
  for (UInt_t i=0;i<TrueVertices.size();i++){
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconParticles.clear();
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconVertices.clear();    
  }
  for (UInt_t i=0;i<TrueParticles.size();i++)
    (static_cast<AnaTrueParticle*>(TrueParticles[i]))->ReconParticles.clear();


  // Fill the vector of pointers for the Daugthers of each particle using the IDs

  std::vector<AnaBunchC*> allBunches = Bunches;
  if (OutOfBunch) allBunches.push_back(OutOfBunch);
  
  for (UInt_t i=0;i<allBunches.size();i++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(allBunches[i]);
    
    for (UInt_t j=0;j<bunch->Particles.size();j++){
      AnaParticle* part = static_cast<AnaParticle*>(bunch->Particles[j]);

      part->Daughters.clear();
      for (UInt_t k=0;k<part->DaughtersIDs.size();k++){
        AnaParticleB* dau = anaUtils::GetParticleByID(*bunch, part->DaughtersIDs[k]);
        if (dau)  part->Daughters.push_back(dau);
      }      
    }
  }
  
  // Redo the links
  AnaSpillB::RedoLinks();
}

//********************************************************************
void AnaSpill::associateVertexToParticle(AnaParticleB* particle, AnaVertexB* vertex) const{
//********************************************************************  
  (static_cast<AnaTrack*>(particle))->ReconVertices.push_back(vertex);
}

//********************************************************************
void AnaSpill::associateVertexToTrueVertex(AnaVertexB* vertex) const{
//********************************************************************
  
  if (vertex->TrueVertex)
    (static_cast<AnaTrueVertex*>(vertex->TrueVertex))->ReconVertices.push_back(vertex);
}

//********************************************************************
void AnaSpill::associateParticleToTrueParticle(AnaParticleB* particle) const{
//********************************************************************

  if (particle->TrueObject){
    (static_cast<AnaTrueParticle*>(particle->TrueObject))->ReconParticles.push_back(particle);

    if (particle->GetTrueParticle()->TrueVertex)
      (static_cast<AnaTrueVertex*>(particle->GetTrueParticle()->TrueVertex))->ReconParticles.push_back(particle);
  }
}

//********************************************************************
void AnaSpill::Print() const{
//********************************************************************

  std::cout << "-------- AnaSpill --------- " << std::endl;

  AnaSpillB::Print();
}


//********************************************************************
AnaEvent::AnaEvent():AnaEventB(){
//********************************************************************

}

//********************************************************************
AnaEvent::~AnaEvent(){
//********************************************************************

}

//********************************************************************
AnaEvent::AnaEvent(const AnaEvent& event):AnaEventB(event){
//********************************************************************

  Trigger = event.Trigger;

}

//*****************************************************************************
AnaEvent::AnaEvent(const AnaSpill& spill, const AnaBunch& bunch):AnaEventB(spill,bunch) {
//*****************************************************************************

  Trigger = spill.Trigger;

  // Fill de Daugthers vector in eac particle using IDs
  for (Int_t i=0;i<nParticles;i++){
    AnaParticle* part = static_cast<AnaParticle*>(Particles[i]);
    for (UInt_t j=0;j<part->DaughtersIDs.size();j++){
      AnaParticleB* dau = anaUtils::GetParticleByID(*this,part->DaughtersIDs[j]);
      if (dau) part->Daughters.push_back(dau);      
    }
  }  
}

//********************************************************************
void AnaEvent::Print() const{
//********************************************************************

  std::cout << "-------- AnaEvent --------- " << std::endl;

  AnaEventB::Print();
  Trigger.Print();
}

//********************************************************************
AnaVertex::AnaVertex():AnaVertexB(){
//********************************************************************

  Bunch        = kIntUnassigned;
  anaUtils::ReserveArray(Variance, 4);
  NDOF         = kIntUnassigned;
  Chi2         = kFloatUnassigned;
}

//********************************************************************
AnaVertex::AnaVertex(const AnaVertex& vertex):AnaVertexB(vertex){
//********************************************************************

  Bunch        = vertex.Bunch;
  anaUtils::CopyArray(vertex.Variance, Variance, 4);
  NDOF         = vertex.NDOF;
  Chi2         = vertex.Chi2;
}

//********************************************************************
void AnaVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaVertex --------- " << std::endl;

  AnaVertexB::Print();

  std::cout << "Bunch:               " << Bunch << std::endl;
  std::cout << "Variance:            " << Variance[0] << " " << Variance[1] << " " << Variance[2] << " " << Variance[3] << std::endl;
  std::cout << "NDOF:                " << NDOF << std::endl;
  std::cout << "Chi2:                " << Chi2 << std::endl;
}

//********************************************************************
AnaEventInfo::AnaEventInfo():AnaEventInfoB(){
//********************************************************************

  EventTime = kFloatUnassigned;
}

//********************************************************************
AnaEventInfo::AnaEventInfo(const AnaEventInfo& spill):AnaEventInfoB(spill){
//********************************************************************

  EventTime = spill.EventTime;
}

//********************************************************************
void AnaEventInfo::Print() const{
//********************************************************************

  std::cout << "-------- AnaEventInfo --------- " << std::endl;

  AnaEventInfoB::Print();

  std::cout << "EventTime:          " << EventTime << std::endl;
}

//********************************************************************
AnaDataQuality::AnaDataQuality(){
//********************************************************************

  GlobalFlag   = kIntUnassigned;
  for (UInt_t i=0;i<7;i++){
    DetFlag[i] = kIntUnassigned;
  }
}

//********************************************************************
AnaDataQuality::AnaDataQuality(const AnaDataQuality& dq):AnaDataQualityB(dq){
//********************************************************************

  GlobalFlag   = dq.GlobalFlag;
  for (UInt_t i=0;i<7;i++)
    DetFlag[i] = dq.DetFlag[i];
}

//********************************************************************
AnaBeam::AnaBeam(){
//********************************************************************

  POT           = kIntUnassigned;
  Spill         = kIntUnassigned;
  BeamParticle  = NULL;
}

//********************************************************************
AnaBeam::~AnaBeam(){
//********************************************************************

  if (BeamParticle)
    delete BeamParticle;  
}

//********************************************************************
AnaBeam::AnaBeam(const AnaBeam& beam):AnaBeamB(beam){
//********************************************************************

  POT           = beam.POT;
  Spill         = beam.Spill;  

  BeamParticle  = NULL;
  if (beam.BeamParticle)
    BeamParticle  = beam.BeamParticle->Clone();
}

//********************************************************************
void AnaBeam::Print() const{
//********************************************************************

  std::cout << "-------- AnaBeam --------- " << std::endl;

  std::cout << "POT:              " << POT << std::endl;
}

//********************************************************************
AnaTrigger::AnaTrigger() {
//********************************************************************

  nTriggers=0;
  Time=NULL;
  ID=NULL;
}

//********************************************************************
AnaTrigger::AnaTrigger(const AnaTrigger& tr) {
//********************************************************************

  nTriggers= tr.nTriggers;
  
  anaUtils::CreateArray(Time, nTriggers);
  anaUtils::CreateArray(ID,   nTriggers);

  anaUtils::CopyArray(tr.Time, Time, nTriggers);
  anaUtils::CopyArray(tr.ID,   ID,   nTriggers);
}

//********************************************************************
void AnaTrigger::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrigger --------- " << std::endl;

  for (Int_t i=0;i<nTriggers;i++){
    std::cout << i << ":  id = " << ID[i] << "\t t = " << Time[i] << std::endl;  
  }

}

