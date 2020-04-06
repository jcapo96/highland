#ifndef baseAnalysis_h
#define baseAnalysis_h

#include "HighlandAnalysisUtils.hxx"
#include "AnalysisAlgorithm.hxx"

#include "Parameters.hxx"
#include "CutUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystId.hxx"
#include "Versioning.hxx"
#include "CategoriesUtils.hxx"
#include "CategoryManager.hxx"

/// This class provides a base for user analyses. It handles many common
/// functions, and provides a structure so that users don't have to write too
/// much "boiler-plate" code.
///
/// User analyses should inherit from this class.
class baseAnalysis: public AnalysisAlgorithm{
 public:
  /// Constructor, which instantiates the necessary converters for converting
  /// input files to the AnaSpill format.
  baseAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~baseAnalysis(){}

  /// This method sould not be virtual
  bool InitializeBase();

  virtual bool Initialize();
  virtual bool FinalizeConfiguration();

  virtual void DefineProductions();
  virtual void DefineInputConverters();

  virtual void DefineCorrections();
  virtual void DefineSystematics();
  virtual void DefineConfigurations();
  virtual void DefineMicroTrees(bool addBase=true);
  virtual void DefineTruthTree();

  virtual bool CheckFillTruthTree(const AnaTrueVertex& vtx) = 0;
  virtual void FillTruthTree(const AnaTrueVertex& vtx) = 0;

  void FillMicroTreesBase(bool addBase=true);
  void FillToyVarsInMicroTreesBase(bool addBase=true);
  void FillTruthTree();
  void FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det=SubDetId::kSubdet1_1, bool IsAntinu = false);

  /// Get a casted AnaSpillC to AnaSpill from the InputManager
  AnaSpill& GetSpill(){return *static_cast<AnaSpill*>(&input().GetSpill());}
  
  /// Get a casted AnaBunchBB to AnaBunch from the InputManager
  AnaBunch& GetBunch(){return *static_cast<AnaBunch*>(&input().GetBunch());}

  /// Get a casted AnaEventC to AnaEvent 
  AnaEvent& GetEvent(){return *static_cast<AnaEvent*>(_event);}

  /// Create the appropriate event time from an Spill and a Bunch in that spill
  virtual AnaEventC* MakeEvent(){
    return new AnaEvent(GetSpill(),GetBunch());
  }

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB* GetVertex() const = 0;

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB* GetTrueVertex() const = 0;

  // protected:
  public:

  enum enumStandardMicroTrees_baseAnalysis{
    run = enumStandardMicroTreesLast_AnalysisAlgorithm+1,
    subrun,
    evt,
    bunch,
    selvtx_det,
    selvtx_pos,
    selvtx_truepos,

    enumStandardMicroTreesLast_baseAnalysis
  };

  enum enumConf_baseAnalysis{
    all_syst=ConfigurationManager::enumConfLast_ConfigurationManager+1,        
    enumConfLast_baseAnalysis
  };


  bool _enableSingleVariationSystConf;
  bool _enableSingleWeightSystConf;
  bool _enableAllSystConfig;
  
  Int_t _ntoys;
  Int_t _randomSeed;
  
};

#define AddConfiguration(conf, name, ntoys, randomSeed,toyMaker)    conf.AddConfiguration(name,#name,ntoys,randomSeed,toyMaker);
#define GetConfigurationIndex(name) conf().GetConfigurationIndex(#name)



#endif
