#ifndef ToyBoxDUNE_h
#define ToyBoxDUNE_h

#include "ToyBoxB.hxx"
#include "DataClasses.hxx"

class ToyBoxDUNE:public ToyBoxB{
public :
  
  ToyBoxDUNE();
  virtual ~ToyBoxDUNE(){}

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:

  /// For storing the true vertex, for analyses with no reconstructed primary vertex
  AnaTrueVertexB* TrueVertex;

  /// The reconstructed EventVertex
  AnaVertexB* Vertex;

  /// The MainTrack, defining the event vertex
  AnaParticleB* MainTrack;

  /// Longest particle in the event
  AnaParticleB* Ltrack;

  /// Second Longest particle in the event
  AnaParticleB* SLtrack;

  /// Third Longest particle in the event
  AnaParticleB* TLtrack;


};

#endif
