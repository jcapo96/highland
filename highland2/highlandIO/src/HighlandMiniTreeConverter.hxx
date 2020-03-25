#ifndef HighlandMiniTreeConverter_h
#define HighlandMiniTreeConverter_h

#include "DataClasses.hxx"
#include "MiniTreeConverter.hxx"

/// Creates the appropriate AnaSpillB type. The rest of the work is done by the base converter

class HighlandMiniTreeConverter: public MiniTreeConverter{

 public:
  HighlandMiniTreeConverter(bool readRooTrackerVtx=false):MiniTreeConverter(readRooTrackerVtx){}
  virtual ~HighlandMiniTreeConverter(){}

  // Create the appropriate spill instance
  virtual AnaSpillB*         MakeSpill() { return new AnaSpill(); }
};

#endif


