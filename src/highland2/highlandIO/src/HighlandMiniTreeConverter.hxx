#ifndef HighlandMiniTreeConverter_h
#define HighlandMiniTreeConverter_h

#include "DataClasses.hxx"
#include "MiniTreeConverter.hxx"

/// Creates the appropriate AnaSpillB type. The rest of the work is done by the base converter

class HighlandMiniTreeConverter: public MiniTreeConverter{

 public:
  HighlandMiniTreeConverter(const std::string& tree_path="MiniTree", bool readRooTrackerVtx=false):MiniTreeConverter(tree_path, readRooTrackerVtx){}
  virtual ~HighlandMiniTreeConverter(){}

  // Create the appropriate spill instance
  virtual AnaSpillB*         MakeSpill() { return new AnaSpill(); }
};

#endif


