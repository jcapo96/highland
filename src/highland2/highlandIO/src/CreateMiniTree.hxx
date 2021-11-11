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
  virtual bool Initialize();
  virtual bool InitializeSpill(){return true;}

  virtual void DefineOutputTree();

  virtual void FinalizeSpill(){}
  virtual void Finalize     ();

  virtual bool Process();
  //--------------------

  virtual bool CheckTruthFillMiniTree(const AnaSpill& spill);
  virtual bool CheckReconFillMiniTree(const AnaBunchB& bunch);
  virtual bool CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch);

  virtual bool CheckTrueVertex(const AnaTrueVertex& vtx) {return false;}

  virtual bool CheckSaveParticle(const AnaParticleB& part) {return true;}
  virtual bool CheckSaveTrueParticle(const AnaTrueParticleB& part) {return true;}

  virtual void FilterParticleInfo(AnaParticleB& part){}
  virtual void FilterTrueParticleInfo(AnaTrueParticleB& part){}
  
  virtual void FillMiniTree();
  virtual void FillRooTrackerVtxTree();
  virtual void FilterRooTrackerVtxTree();

  virtual void WriteGeometry();

  virtual void DeleteUninterestingBunches();
  virtual void DeleteUninterestingParticles();
  virtual void DeleteUninterestingTrueParticles();

  virtual bool SpillLevelPreselection() {return true;} 

  virtual AnaSpillB* MakeSpill(){return new AnaSpill();}
  virtual AnaBeamB*  MakeBeam() {return new AnaBeam();}

  enum miniTreeIndex{
    minitree = OutputManager::enumSpecialTreesLast+1
  };

protected:

  bool _saveGeometry;
  bool _saveRoo;
  bool _filterRoo;
  bool _trueWithRecoPreselection;          
  bool _trueWithRecoDaughtersPreselection; 
  
  Double_t _POTSincePreviousSavedSpill;
  Int_t _SpillsSincePreviousSavedSpill;

  bool _lastSpillSaved;

  AnaSpill* _spill;

  UInt_t _currentGeomID;

  Int_t _totalParticles;
  Int_t _savedParticles;
  Int_t _totalTrueParticles;
  Int_t _savedTrueParticles;


};

#endif
