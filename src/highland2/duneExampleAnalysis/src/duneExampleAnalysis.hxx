#ifndef duneExampleAnalysis_h
#define duneExampleAnalysis_h

#include "baseAnalysis.hxx"
#include "ToyBoxDUNE.hxx"

class duneExampleAnalysis: public baseAnalysis {
 public:
  duneExampleAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~duneExampleAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  //--------------------

  bool Initialize();
  void FillCategories();

  /// Returns the ToyBoxDUNE
  virtual const ToyBoxDUNE& box(Int_t isel=-1) const {return *static_cast<const ToyBoxDUNE*>(&boxB(isel));}

  /// Returns the vertex for the ToyBoxDUNE
  virtual AnaVertexB* GetVertex() const{return box().Vertex;}

  /// Returns the true vertex for the ToyBoxDUNE
  virtual AnaTrueVertexB* GetTrueVertex() const {return box().TrueVertex;}


private:

  SubDetId::SubDetEnum _detector;

public:

  enum enumStandardMicroTrees_duneExampleAnalysis{
    selmu_truemom = enumStandardMicroTreesLast_baseAnalysis+1,
    selmu_truebeta,
    truemu_mom,
    truemu_costheta,
    truemu_pos,
    truemu_dir,
    selmu_mom,
    selmu_theta,
    selmu_costheta,
    selmu_nhits,
    selmu_length,
    selmu_dedx,
    selmu_dedx_raw,
    selmu_dedx_exp,
    selmu_closest_tpc,
    selmu_detectors,
    selmu_truepos,
    selmu_trueendpos,
    selmu_truedir,
    selmu_dir,
    selmu_enddir, 
    selmu_pos,
    selmu_endpos,
    ntracks,   
    nu_pdg,
    nu_trueE,
    nu_truereac,
    nu_truedir,
    truevtx_pos,

    
    enumStandardMicroTreesLast_duneExampleAnalysis
  };

  enum enumConf_duneExampleAnalysis{
    detmass_syst=baseAnalysis::enumConfLast_baseAnalysis+1,    
    dedx_syst,
    enumConfLast_duneExampleAnalysis
  };



};

#endif
