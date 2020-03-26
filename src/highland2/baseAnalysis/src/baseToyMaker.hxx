#ifndef baseToyMaker_h
#define baseToyMaker_h

#include "ToyMaker.hxx"
#include "BinnedPDF.hxx"
#include "TRandom3.h"

/// Creates ToyExperiments

class baseToyMaker: public ToyMaker{
  
public:
  
  /// Create the Toy experiment 
  baseToyMaker(UInt_t seed, bool zero_var=false);
  
  /// Everyone should have a destructor.
  virtual ~baseToyMaker() {}

  /// Fills the Toy Experiment with a given index
  void FillToyExperiment(ToyExperiment& toy);

  /// returns the random seed
  UInt_t GetSeed() const {return _seed;}

  // Use a random generator for each systematic
  void SetIndividualRandomGenerator(bool ok){ _individualRandomGenerator=ok;}
  


protected:

  // A binned PDF
  BinnedPDF* _binnedPDF;       

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  TRandom3 _RandomGenerators[NMAXSYSTEMATICS];

  /// The random seed used
  UInt_t _seed;

  UInt_t _seeds[NMAXSYSTEMATICS];

  // Use a random generator for each systematic
  bool _individualRandomGenerator;


  /// Apply 0 variation
  bool _zero_var;
};

#endif
