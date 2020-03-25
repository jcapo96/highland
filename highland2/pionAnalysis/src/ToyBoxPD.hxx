#ifndef ToyBoxPD_h
#define ToyBoxPD_h

#include "ToyBoxB.hxx"
#include "DataClasses.hxx"

class ToyBoxPD:public ToyBoxB{
public :
  
  ToyBoxPD();
  virtual ~ToyBoxPD(){}

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
  AnaParticle* MainTrack;
};

#endif
