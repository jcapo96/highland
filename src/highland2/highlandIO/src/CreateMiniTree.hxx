#ifndef CreateMiniTree_h
#define CreateMiniTree_h

#include "SimpleLoopBase.hxx"
#include "DataClasses.hxx"
#include "InputManager.hxx"
#include <set>

class CreateMiniTree: public SimpleLoopBase {
 public:

  CreateMiniTree(int argc, char *argv[]);
  virtual ~CreateMiniTree(){}

protected:

  //---- These are mandatory functions
  virtual bool Initialize     ();
  virtual bool InitializeSpill(){return true;}

  virtual void DefineOutputTree();

  virtual void FinalizeSpill(){}
  virtual void Finalize     ();

  virtual bool Process();
  //--------------------

  virtual bool CheckTruthFillMiniTree(const AnaSpill& spill);
  virtual bool CheckReconFillMiniTree(const AnaBunchB& bunch);
  virtual bool CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch);

  virtual bool CheckTrueVertexReaction(const AnaTrueVertex& vtx);
  virtual bool CheckTrueVertexDetector(unsigned long det);

  virtual void FillMiniTree();
  virtual void FillRooTrackerVtxTree();
  virtual void FilterRooTrackerVtxTree();

  virtual void WriteGeometry();

  void DeleteUninterestingBunches();
  void DeleteUninterestingParticles();
  void DeleteUninterestingTrueParticles();

  bool SpillLevelPreselection();

  AnaTrueParticle* FindBeamTrueParticle(const AnaSpillB& spill);
  
  virtual AnaSpillB* MakeSpill(){return new AnaSpill();}
  virtual AnaBeamB*  MakeBeam() {return new AnaBeam();}

  enum miniTreeIndex{
    minitree = OutputManager::enumSpecialTreesLast+1
  };


protected:

  bool _saveGeometry;
  bool _saveRoo;
  bool _filterRoo;

  bool _saveSubdet2Info;
  bool _saveSubdet1Info;
  bool _saveTrackerInfo;

  bool _useSubdet2_1;
  bool _useSubdet2_2;
  bool _useSubdet2_3;
  bool _useSubdet1_1;
  bool _useSubdet1_2;

  bool _useSubdet2_1outOfBunch;
  bool _useSubdet2_2outOfBunch;
  bool _useSubdet2_3outOfBunch;
  bool _useSubdet1_1outOfBunch;
  bool _useSubdet1_2outOfBunch;

  bool _saveTrueNuNC;
  bool _saveTrueAntiNuNC;
  bool _saveTrueNumuCC;
  bool _saveTrueAntiNumuCC;
  bool _saveTrueNueCC;
  bool _saveTrueAntiNueCC;

  bool _trackStartPreselection;
  bool _beamTOFPreselection;
  bool _trueWithRecoPreselection;          
  bool _trueWithRecoDaughtersPreselection; 

  
  Float_t _trackStartMinX;
  Float_t _trackStartMaxX;
  Float_t _trackStartMinY;
  Float_t _trackStartMaxY;
  Float_t _trackStartMinZ;  
  Float_t _trackStartMaxZ;

  Float_t _beamMinTOF;  
  Float_t _beamMaxTOF;

  
  Double_t _POTSincePreviousSavedSpill;
  Int_t _SpillsSincePreviousSavedSpill;

  bool _lastSpillSaved;

  std::vector<SubDetId::SubDetEnum> _saveTrueVertexInDet;

  AnaSpill* _spill;

  UInt_t _currentGeomID;
};

#endif
