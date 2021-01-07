/*
   converter for MiniTree input format (direct dump of analysis data classes)

   A. Cervera May 2015
*/


#ifndef MiniTreeConverter_h
#define MiniTreeConverter_h

#include "BaseDataClasses.hxx"
#include "InputConverter.hxx"
#include <TClonesArray.h>

class MiniTreeConverter: public InputConverter{

 public:
  MiniTreeConverter(const std::string& tree_path="MiniTree", bool readRooTrackerVtx=false);
  virtual ~MiniTreeConverter();

  virtual bool Initialize();
  Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event);

  // Create the appropriate spill instance
  virtual AnaSpillB*         MakeSpill() { return new AnaSpillB(); }


  // Create an AnaEVentB from a Spill and a Bunch
  AnaEventB* MakeEvent(AnaSpillB& spill, AnaBunchB& bunch);

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  bool AddFileToTChain(const std::string& inputString);

  // TODO. Assume for the moment 20% more events that entries in the MiniTree. 
  Int_t GetNEvents(Int_t entries=-1);

protected:

  bool _useCorrectedValues;

  bool _readRooTrackerVtx;

  AnaSpillB* _spill;

  /// The accumulated POT
  Double_t _POTSincePreviousSavedSpill;


  TChain *minitree;
  TChain *GRooTrackerVTX;
  TChain *NRooTrackerVTX;

  bool fGenie;
  bool fNeut;

  // the current RooTrackerVtx entry
  Long64_t _entry_roo;

 protected:

   // Genie and Neut Tracker Vertex
   Int_t RunID;
   Int_t SubrunID;
   Int_t EventID;
   Int_t NNVtx;
   TClonesArray *NVtx;
   Int_t NGVtx;
   TClonesArray *GVtx;
};

#endif


