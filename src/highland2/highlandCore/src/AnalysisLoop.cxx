#include "AnalysisLoop.hxx"
#include <TClonesArray.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "Parameters.hxx"
#include "Header.hxx"
#include "DocStringManager.hxx"
//#include "AnalysisUtils.hxx"
//#include "GeometryManager.hxx"
#include "MultiThread.hxx"
#include "VersionManager.hxx"

#include "ZeroToyMaker.hxx"

#include <sys/time.h>
#include <TError.h>

// save all trees every 5000 entries
const int NENTRIES_TREESAVE=5000;

//********************************************************************
AnalysisLoop::AnalysisLoop(AnalysisAlgorithm* ana, int argc, char *argv[]){
//********************************************************************

  _ana = ana;

  // Remove root warnings
  gErrorIgnoreLevel = kError;
  
  std::string programName = argv[0];
  std::string paramFile = "";

  _entry_nmax = 0;
  _entry_imin = 0;
  _entry_count = 0;
  _outputFileName = "";
  _inputFileName = "";
  _inputSkimFileName = "";
  _cosmicMode = false;
  _versionCheck = true;
  _fillTrees=true;
  bool logMemory = false;

  for (;;) {
    int c = getopt(argc, argv, "crvmn:e:o:p:s:");
    if (c < 0)
      break;
    switch (c) {
      case 'c': {
        _cosmicMode = true;
        break;
      }
      case 'v': {
        _versionCheck = false;
        break;
      }
      case 'r': {
        _fillTrees = false;
        break;
      }
      case 'm': {
        logMemory = true;
        break;
      }
      case 'n': {
        std::istringstream tmp(optarg);
        tmp >> _entry_nmax;
        break;
      }
      case 'o': {
        _outputFileName = optarg;
        break;
      }
      case 'e': {
        _inputSkimFileName = optarg;
        break;
      }
      case 'p': {
        paramFile = optarg;
        break;
      }
      case 's': {
        std::istringstream tmp(optarg);
        tmp >> _entry_imin;
        break;
      }
      default: {
        PrintUsage(programName);
      }
    }
  }

  // Check that an output file was specified
  if (_outputFileName == "") {
    std::cerr << "ERROR: No output file" << std::endl << std::endl;
    PrintUsage(programName);
  }

  // Check that there is an input file.
  if (argc <= optind) {
    std::cerr << "ERROR: No input file" << std::endl << std::endl;
    PrintUsage(programName);
  } else if (argc != optind + 1) {
    std::cerr << "ERROR: Only one input file may be specified. To process multiple ROOT files, " << std::endl
              << "       list them in a text file, and specify that text file as the input." << std::endl << std::endl;
    PrintUsage(programName);
  }

  _inputFileName = argv[optind++];

  // Set up the parameter override file if it was specified
  if (paramFile != "") {
    ND::params().ReadParamOverrideFile(paramFile);
  }

  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  _weightSyst = NULL;

  _enableTruthTree = true;
  _memory.Enable(logMemory);

#ifdef MULTITHREAD
  std::cout << "For runnning in highland2 you must comment out the line #define MULTITHREAD psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
  exit(0);
#endif

}

//********************************************************************
bool AnalysisLoop::Initialize(){
//********************************************************************

  // Define the InputConverters. This must be called before the InputManager is initialized
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineInputConverters);
  ana().DefineInputConverters();

  // Define Productions
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineProductions);
  ana().DefineProductions();

  // Set the skim file name
  anaUtils::skimFileName = _inputSkimFileName;
  
  // Initialize the InputManager by specifying the input type and the input file
  if (!ana().input().Initialize(_inputFileName,_inputFileType, _cosmicMode)) return false;

  // Tell the AnalysisAlgorithm to perform version checking to make sure the sofware version and the input file match each other
  ana().SetVersionCheck(_versionCheck);
  
  // Initialize the User Analysis Algorithm. It can overwride the production via parameters file in baseAnalysis 
  ana().SetAnalysisPoint(AnalysisAlgorithm::kInitialize);
  if (!ana().Initialize()) return false;

  // This must be called after the Initialize of all algorithms since it is there where the last used algorithms can be decleared
  if (!ana().InitializeBase()) return false;
  
  // This will take care of data/MC differences in detector volumes definitions 	 
  // Should be applied after the version has been defined
  // TODO. Moved temporarily to baseAnalysis.cxx to avoid dependency on psycheND280Utils
  //  ND::hgman().InitializeGeometry(ana().input().GetIsMC()); 

  // Initialise the Output Manager. 
  ana().output().Initialize();

  // Open the output file
  if (_fillTrees)
    if (!ana().output().OpenOutputFile(_outputFileName)) return false;

  //  if (ana().input().GetChain() == 0) return;

  // Define the Corrections
  DefineCorrections();

  // Define the selection (s)
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineSelections);
  ana().DefineSelections();

  // Initialize the Selections (create the ToyBox)
  ana().sel().CreateToyBoxArray(_entry_nmax);

  // Define the Systematics
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineSystematics);
  ana().DefineSystematics();

  // Define the configurations
  DefineConfigurations();
  
  // Define the micro tree variables
  if (_fillTrees)
    DefineMicroTrees();

  // Define the "truth tree" variables
  if (_enableTruthTree && _fillTrees)
    DefineTruthTree();

  // Create and fill the "config" tree with a single entry
  if (_fillTrees)
    FillConfigTree();

  // Fill allways the event summary even when the selection is not passed
  ana().sel().SetForceFillEventSummary(true);

  return true;
}

//********************************************************************
bool AnalysisLoop::InitializeSpill(){
//********************************************************************

  // Initialize the OutputManager (TODO)
  if (_fillTrees)
    ana().output().InitializeEntry();

  // Initialize the truth tree (which does not belong to any configuration)
  if (_fillTrees)
    ana().output().InitializeTree(OutputManager::truth);

  // To compute the entry increment
  Int_t entry0 = _entry;
  
  // Fill the spill structure for the current spill from the input tree
  bool spillOK = ana().input().LoadSpill(_entry);
 
  // To compute the entry increment
  Int_t entry1 = _entry;

  // Increment the number of entries run so far
  _entry_count += entry1-entry0;

  // Dump info about number of entries run
  if (_entry_count%1000==0 || _entry_count == _entry_nmax)
    std::cout << "entry: " <<  _entry_count  << " of " << _entry_nmax << " (" << (100*_entry_count/_entry_nmax) << "%) --> " << _entry << std::endl;

  // return if the read spill is not OK
  if (!spillOK) return false;
  
  // Apply Corrections. This will affect all configurations
  ana().corr().ApplyCorrections(ana().input().GetCorrectedSpill());

  // Create a clone of the Corrected Spill
  ana().input().MakeFinalSpill();

  // Record the POT for this spill
  ana().input().IncrementPOTBySpill();

  // initialize the spill in the User Analysis Algorithm
  ana().SetAnalysisPoint(AnalysisAlgorithm::kInitializeSpill);
  if (!ana().InitializeSpill()) return false;

  return true;
}

//********************************************************************
void AnalysisLoop::InitializeBunch(){
//********************************************************************
  
  // Create the event from the spill and the current bunch (needed by psyche packages)
  _event = ana().MakeEvent();
  ana().SetEvent(_event);

  // Create and fill the EventBox for the current event
  ana().sel().InitializeEvent(*_event);

  // Initialize the Bunch in the User Analysis Algorithm
  ana().SetAnalysisPoint(AnalysisAlgorithm::kInitializeBunch);
  ana().InitializeBunch();  
}

//********************************************************************
void AnalysisLoop::InitializeConfiguration(){
//********************************************************************

  // Initialize the tree associated to the current configuration (put all variables to the default value)
  if (_fillTrees)
    ana().output().InitializeTree(ana().output().GetCurrentTree(), ana().GetInitializeTrees());

  if (ana().evar().HasEventVariations()){
    // Enable the appropriate EventVariations for this configuration
    ana().evar().DisableAllEventVariations();
    ana().evar().EnableEventVariations(ana().conf().GetEnabledEventVariations());
        
    // Create the SystBox array (only the first time it is called for each EventVariation)
    ana().evar().Initialize(_entry_nmax);
    
    // Initialize The SystBox for EventVariations
    ana().evar().InitializeEvent(ana().sel(),*_event);
  }    

  if (ana().eweight().HasEventWeights()){
    // Enable the appropriate EventWeights for this configuration
    ana().eweight().DisableAllEventWeights();
    ana().eweight().EnableEventWeights(ana().conf().GetEnabledEventWeights());
    
    // Create an array to store the systematics weights. (TODO. Is it faster to just resize the existing array ?)
    _nWeightSyst= ana().eweight().GetNEnabledEventWeights();
    anaUtils::CreateArray(_weightSyst, _nWeightSyst);
    
    // Create the SystBox array (only the first time it is called for each EventWeight)
    ana().eweight().Initialize(ana().sel(),_entry_nmax);
    
    // Initialize The SystBox for variation systematics
    ana().eweight().InitializeEvent(ana().sel(),*_event);
  }    

  // Initialize the Configuration in the User Analysis Algorithm
  ana().InitializeConfiguration();

  // This variables is reseted here and filled in FillToyVarsInMicroTrees
  // If true means that at least one toy fullfils the condition to be saved into the microtree
  _toy_filled=false;
}

//********************************************************************
void AnalysisLoop::InitializeToy(){
//********************************************************************

  // Apply variation systematics. Only for MC. TODO: now that this is not only a systematic it could be also used for data
  if (_event->GetIsMC() && ana().evar().GetNEnabledEventVariations()>0){

    // Get the current Toy Experiment
    ToyExperiment* toy = ana().conf().GetCurrentConfiguration()->GetToyMaker().GetToyExperiment(ana().conf().GetToyIndex());

    // Apply  systematic variations for systematic error propagation
    ana().evar().ApplyEventVariations(*toy, *_event);
  }

  // Initialize this toy experiment for the user Analysis Algorithm
  ana().SetAnalysisPoint(AnalysisAlgorithm::kInitializeToy);
  ana().InitializeToy();	
}

//********************************************************************
void AnalysisLoop::InitializeSelection(const SelectionBase& selec){
//********************************************************************

  // initialize selection for the user analysis. TODO: currently not used by any analysis
  ana().SetAnalysisPoint(AnalysisAlgorithm::kInitializeSelection);
  ana().InitializeSelection(selec);	
  
}

//********************************************************************
void AnalysisLoop::FinalizeSelection(const SelectionBase& selec){
//********************************************************************

  /// Compute the weight for this selection by applying the systematic weights. Only for MC when any of the weight systematics is enabled,
  // and when the selection was passed. TODO: now that this is not only a systematic it could be also used for data
  if (_toy_passed && _event->GetIsMC() && ana().eweight().GetNEnabledEventWeights()>0){      
    // Get the current Toy Experiment
    ToyExperiment* toy = ana().conf().GetCurrentConfiguration()->GetToyMaker().GetToyExperiment(ana().conf().GetToyIndex());
    // Apply the relevant weight systematics for the current selection. Internally loops over branches in the selection and compute the weights only 
    // for the succesfull branches
    ana().eweight().ComputeEventWeights(selec,*toy, *_event, selec.GetPreviousToyBox(*_event), _weightSyst);
  }

  // finalize selection for the user analysis. TODO: currently not used by any analysis.
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFinalizeSelection);
  ana().FinalizeSelection(selec);	
}

//********************************************************************
bool AnalysisLoop::FinalizeToy(){
//********************************************************************

  // Finalize the Toy Experiment for the AnalysisAlgorithm (mandatory)
  ana().FinalizeToyBase();

  // Finalize the Toy Experiment for the User AnalysisAlgorithm (optional). TODO: currently not used by any analysis
  //  ana().FinalizeToy();

  // Save the toy experiment weight such that it can be properly normalized after all toys have been run
  //  ana().output().AddToyWeight(1.);

  // The toy experiment dependent micro-tree variables are filled here
  // Conditions inside method 
  if (_fillTrees) FillToyVarsInMicroTrees();

  // break the toy loop when the the first toy does not reach a minimum accum level for any of the selections
  // TODO: when running several selections we should stop running a given selection when the minimum accum level for that selection 
  // is not reached
  if (!ana().sel().PreSelectionPassed(*_event)) return false;

  // Undo all variationSystematics variations at the end of this toy experiment. TODO: we should reset undo all systematics in the same method to 
  // avoid looping several times over the same objects
  if (_event->GetIsMC() && ana().evar().GetNEnabledEventVariations()>0){
    if  (ana().evar().UndoEventVariations(*_event))
      ana().input().ResetSpillToCorrected();
  }

  // When false is returned the loop over toys is broken
  return true;
}

//********************************************************************
bool AnalysisLoop::FinalizeConfiguration(){
//********************************************************************

  // Delete the array for systematics weights. Will create a new one in InitializeConfiguration (TODO)
  if (_weightSyst) delete [] _weightSyst;
  _weightSyst = NULL;

  // Delete the SystBox for the current event
  if (_event->GetIsMC() && ana().evar().GetNEnabledEventVariations()>0)
    ana().evar().FinalizeEvent(*_event);

  // Delete the SystBox for the current event
  if (_event->GetIsMC() && ana().eweight().GetNEnabledEventWeights()>0)
    ana().eweight().FinalizeEvent(*_event);

  // Finalize the configuration for the User AnalysisAlgorithm
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFinalizeConfiguration);
  if (!ana().FinalizeConfiguration()) return false;

  // When requested fill the tree only for events that pass the selection at least for one of the toys
  // conf_passed is true when at least one of the toys passes the selection
  if (!_fillTrees ||  (!_conf_passed && ana().GetFillSuccessOnly())) return true;

  // Only fill the tree if any of the toys was saved (passed a given cut)
  //  if (!ana().CheckAccumLevelToSave()) return false;
  if (!_toy_filled) return false;

  // Reset the categories for the current track
  ana().cat().ResetCurrentCategories();

  // Fill categories for color drawing for the current track
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFillCategories);
  ana().FillCategories();

  // Fill all micro-trees (main trees for analysis)
  FillMicroTrees();

  return true;
}

//********************************************************************
void AnalysisLoop::FinalizeBunch(){
//********************************************************************

  // The PreviousToyBox must be Reset after processing one event (all toys)
  ana().sel().FinalizeEvent(*_event);

  // Derived classes
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFinalizeBunch);
  ana().FinalizeBunch();

  // Delete the event
  delete _event;
}

//********************************************************************
void AnalysisLoop::FinalizeSpill(){
//********************************************************************

  if (_fillTrees){

    // Fill only the truth tree
    ana().output().SetFillSingleTree(OutputManager::truth);
    ana().output().SetCurrentTree(OutputManager::truth);

    // call the derived class function
    ana().SetAnalysisPoint(AnalysisAlgorithm::kFillTruthTree);
    ana().FillTruthTree();

    // Fill all trees
    ana().output().SetFillAllTrees();

    // Finalize the derived class
    ana().SetAnalysisPoint(AnalysisAlgorithm::kFinalizeSpill);
    ana().FinalizeSpill();

    _memory.LogMemory();

    // Save all trees every NENTRIES_TREESAVE
    if (_entry%NENTRIES_TREESAVE == 0){
      std::vector< TTree* >::iterator it;
      for (it=ana().output().GetTrees().begin();it!=ana().output().GetTrees().end();it++){
        (*it)->AutoSave("SaveSelf");
      }
    }
  }
  // delete Spill, CorrectedSpill and RawSpill
  ana().input().DeleteSpill();

}

//********************************************************************
void AnalysisLoop::Finalize(){
//********************************************************************

  // Finalize the derived class
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFinalize);
  ana().Finalize();

  // Create and fill the header tree with a single entry
  if (_fillTrees) {
    FillHeaderTree();

    // Save all trees after looping over all events
    std::vector< TTree* >::iterator it;
    for (it=ana().output().GetTrees().begin();it!=ana().output().GetTrees().end();it++){
      (*it)->AutoSave("SaveSelf");
    }

    // Write out memory usage, if needed.
    _memory.Write();

    // Close the output file
    ana().output().CloseOutputFile();
  }
}

//********************************************************************
void AnalysisLoop::DefineMicroTrees(){
//********************************************************************

  /* Here we add all default variables  */


  //------------ Add a tree for each configuration -----------------  
  Int_t tree_index = NMAXSPECIALTREES;
  for (std::vector<ConfigurationBase* >::iterator it3= ana().conf().GetConfigurations().begin();it3!=ana().conf().GetConfigurations().end();it3++, tree_index++){
    if (!(*it3)->IsEnabled()) continue;

    // Set the tree index to this configuration
    (*it3)->SetTreeIndex(tree_index);
    // Add a tree with a given index and name
    ana().output().AddTreeWithName(tree_index, (*it3)->Name());
    // Set the number of toys for the tree
    ana().output().SetNToys(tree_index, (*it3)->GetNToys());
  }

  //------------ Add a common variables to all trees (non special trees) -----------------

  // add entry number in original tree
  ana().output().AddVar(AnalysisAlgorithm::entry,   "entry",   "I", "entry number");

  // add reference toy index to the tree
  ana().output().AddVar(AnalysisAlgorithm::toy_ref, "toy_ref", "I", "Reference toy index");

  //---- Add variables to the tree for the different track categories ------
  Int_t categ_index = AnalysisAlgorithm::firstCategory;
  Int_t counter_index = AnalysisAlgorithm::firstCategoryCounter;
  for (std::map< std::string, TrackCategoryDefinition*>::iterator it=ana().cat().GetCategories().begin();it!=ana().cat().GetCategories().end();it++, categ_index++){
    std::string categ_name = it->first;
    TrackCategoryDefinition& categ = *(it->second);
    if (categ.IsMultiType()){
      ana().output().AddVectorVar(categ_index, categ_name, "I","This is a category variable that can be used to split up a MC histogram into a stack. Use the DumpCategory(\"...\") function for more.", counter_index, "N"+categ_name, categ.GetNTypes());
      counter_index++;
    }
    else if (categ.IsObject()){
      ana().output().AddVectorVar(categ_index, categ_name, "I",
                                  "This is a category variable that can be used to split up a MC histogram into a stack. Use the DumpCategory(\"...\") function for more.",
                                  categ._counterIndex, categ._counterName, categ._counterSize);
    }
    else
      ana().output().AddVar(categ_index, categ_name,"I","This is a category variable that can be used to split up a MC histogram into a stack. Use the DumpCategory(\"...\") function for more.");
  }

  ana().output().AddToyVar(AnalysisAlgorithm::redo, "redo", "I", "If running multiple toys (for evaluating systematics), whether the selection was re-performed for this toy.");

  //--- add cut flags to the output tree ----------------------

  // accumulated cut level
  if (ana().sel().GetNEnabledSelections()>1) {
    if (ana().sel().GetNMaxBranches()>1) {
      ana().output().AddToyMatrixVar(AnalysisAlgorithm::accum_level, "accum_level", "I",
				     "The number of the first cut that was failed.\n"
				     "highland supports running multiple toys (for evaluating systematics), so the first index in this matrix is the toy index. \n"
				     "the second index is the selection and the third index is the branch.\n"
				     "If you only have one toy (normal), then you can use accum_level[][0][0], etc., where 0 can be replaced with your selection/branch number.",
				     ana().sel().GetNEnabledSelections(), ana().sel().GetNMaxBranches());
    }
    else{
      ana().output().AddToyVectorVar(AnalysisAlgorithm::accum_level, "accum_level", "I",
				     "The number of the first cut that was failed.\n"
				     "highland supports running multiple toys (for evaluating systematics), so the first index in this matrix is the toy index. \n"
				     "The second index is the selection.\n"
				     "If you only have one toy (normal), then you can use accum_level[][0], etc., where 0 can be replaced with your selection number.",
				     ana().sel().GetNEnabledSelections());
    }
  }
  else{
    if (ana().sel().GetNMaxBranches()>1) {
      ana().output().AddToyVectorVar(AnalysisAlgorithm::accum_level, "accum_level", "I",
				     "The number of the first cut that was failed.\n"
				     "highland supports running multiple toys (for evaluating systematics), so the first index in this matrix is the toy index. The second index is the branch.\n"
				     "If you only have one toy (normal), then you can use accum_level[][0], etc., where 0 can be replaced with your branch number.",
				     ana().sel().GetNMaxBranches());
    } else {
      ana().output().AddToyVar(AnalysisAlgorithm::accum_level, "accum_level", "I",
			       "The number of the first cut that was failed.\n"
			       "highland supports running multiple toys (for evaluating systematics), so the index in this vector is the toy index.\n"
			       "If you only have one toy (normal), then you can use accum_level or accum_level[] to access the correct number.");
    }
  }
  char cut[50];
  Int_t indx=AnalysisAlgorithm::first_cut;
  for (unsigned int i=0;i<ana().sel().GetNMaxCuts() ;i++, indx++){  
    sprintf(cut, "cut%d", i);
    if (ana().sel().GetNEnabledSelections()>1) {
      if (ana().sel().GetNMaxBranches()>1) {
        ana().output().AddToyMatrixVar(indx, cut, "I","Whether this cut was passed or not.\n"
                                       "highland supports running multiple toys (for evaluating systematics), so the first index in this matrix is the toy index. \n"
                                       "The second index is the selection and third index is the branch in that selection. \n"
                                       "If you only have one toy (normal), then you can use cutN[][0][0], etc., where the 0 can be replaced with your selection/branch number \n"
                                       "and N is the cut number.",
                                       ana().sel().GetNEnabledSelections(), ana().sel().GetNMaxBranches());
      }      
      else{
        ana().output().AddToyVectorVar(indx, cut, "I","Whether this cut was passed or not.\n"
                                       "highland supports running multiple toys (for evaluating systematics), so the index in this vector is the toy index.\n"
                                       "The second index is the selection. \n"
                                       "If you only have one toy (normal), then you can use cutN or cutN[][0] to access the correct number, \n"
                                       "where the 0 can be replaced with your selection number and N is the cut number.",
                                       ana().sel().GetNEnabledSelections());	
      }
    }
    else{
      if (ana().sel().GetNMaxBranches()>1) {
        ana().output().AddToyVectorVar(indx, cut, "I","Whether this cut was passed or not.\n"
                                       "highland supports running multiple toys (for evaluating systematics), so the first index in this matrix is the toy index. The second index is the branch.\n"
                                       "If you only have one toy (normal), then you can use cutN[][0], etc., where 0 can be replaced with your branch number and N is the cut number.",
                                       ana().sel().GetNMaxBranches());
      } else {
        ana().output().AddToyVar(indx, cut, "I","Whether this cut was passed or not.\n"
                                 "highland supports running multiple toys (for evaluating systematics), so the index in this vector is the toy index.\n"
                                 "If you only have one toy (normal), then you can use cutN or cutN[] to access the correct number, where N is the cut number.");
      }
    }
  }
  
  //------------ Add systematics related variables -----------------

  /*
  //! [AnalysisLoop_syst]
  There are several variables in the microtree related with systematic error propagation. The correspondence with variables used to compute the covariance matrix 
  is indicated inside the paretheses:
  
  - \b NTOYS (\(N_{toys}\)):  this the the number of toy experiments. The same for all events.
  - \b toy_index (t): the toy experiment index (from 0 to NTOYS-1). It has 1 index: toy_index[NTOYS];
  - \b toy_weight (\(w^t\)): This is the toy experiment weight. It has 1 index: toy_weight[NTOYS]. 
  - \b NWEIGHSYST (\(N_s\)): the number of weight systematics. 
  - \b weight_syst (\((W^t)_e^s\)):    this is the event weights for each toy and each weight systematic enabled. 
                                         It has 2 indexes: weight_syst[NTOYS][NWEIGHTSYST].
  - \b weight_syst_total (\((W^t)_e\)):   the product of all weight systematics. It has one index weight_syst_total[NTOYS]

  //! [AnalysisLoop_syst]
  */  

  for (std::vector<ConfigurationBase* >::iterator it3= ana().conf().GetConfigurations().begin();it3!=ana().conf().GetConfigurations().end();it3++){
    if (!(*it3)->IsEnabled()) continue;

    // Enable the appropriate Variations and Weights for this configuration
    ana().evar().DisableAllEventVariations();
    ana().evar().EnableEventVariations((*it3)->GetEnabledEventVariations());

    ana().eweight().DisableAllEventWeights();
    ana().eweight().EnableEventWeights((*it3)->GetEnabledEventWeights());


    if (ana().eweight().GetNEnabledEventWeights() == 0) continue;

    Int_t tree_index = (*it3)->GetTreeIndex();

    // add the vector of systematic weights and the total systematic weight. TODO
    // ToyVars except for default_conf
    if ((*it3)->GetIndex() == ConfigurationManager::default_conf){
      ana().output().AddVectorVar(tree_index,AnalysisAlgorithm::weight_syst, "weight_syst",       "F", "Weight parameters (corr+syst)", ana().eweight().GetNEnabledEventWeights());
      ana().output().AddVar(tree_index,AnalysisAlgorithm::weight_syst_total, "weight_syst_total", "F", "Total weight (corr+syst)");

      ana().output().AddVectorVar(tree_index,AnalysisAlgorithm::weight_corr, "weight_corr",       "F", "Weight parameters (corr only)", ana().eweight().GetNEnabledEventWeights());
      ana().output().AddVar(tree_index,AnalysisAlgorithm::weight_corr_total, "weight_corr_total", "F", "Total weight (corr only)");

    }
    else{
      ana().output().AddToyVectorVar(tree_index,AnalysisAlgorithm::weight_syst, "weight_syst",       "F", "Weight parameters (corr+syst)", ana().eweight().GetNEnabledEventWeights());
      ana().output().AddToyVar(tree_index,AnalysisAlgorithm::weight_syst_total, "weight_syst_total", "F", "Total weight (corr+syst)");

      ana().output().AddToyVectorVar(tree_index,AnalysisAlgorithm::weight_corr, "weight_corr",       "F", "Weight parameters (corr only)", ana().eweight().GetNEnabledEventWeights());
      ana().output().AddToyVar(tree_index,AnalysisAlgorithm::weight_corr_total, "weight_corr_total", "F", "Total weight (corr only)");
    }
    
    // add the number of systematic weights
    ana().output().AddVar(tree_index,AnalysisAlgorithm::NWEIGHTSYST, "NWEIGHTSYST", "I", "Number of systematic weight parameters");
  }

  // Define analysis specific variables
  ana().DefineMicroTrees();
}

//********************************************************************
void AnalysisLoop::DefineTruthTree(){
//********************************************************************

  // Add a tree
  ana().output().AddTreeWithName(OutputManager::truth, "truth");

  if (ana().sel().GetNEnabledSelections()>1) {
    if (ana().sel().GetNMaxBranches()>1) {
      ana().output().AddMatrixVar(OutputManager::truth, AnalysisAlgorithm::accum_level, "accum_level", "I",
        "The number of the first cut that was failed for each branch. Unlike the variable with the same name in the standard micro-trees, \n"
        "this one has a single toy running multiple toys \n"
        "The first index is the selection and the second index is the branch.\n"
        "Then you can use accum_level[0][0], etc., where 0 can be replaced with your selection/branch number.",
        ana().sel().GetNEnabledSelections(), ana().sel().GetNMaxBranches());
    }
    else{
      ana().output().AddVectorVar(OutputManager::truth, AnalysisAlgorithm::accum_level, "accum_level", "I",
        "The number of the first cut that was failed for each branch. Unlike the variable with the same name in the standard micro-trees, \n"
        "this one has a single toy running multiple toys \n"
        "The first index is the selection and the second index is the branch.\n"
        "Then you can use accum_level[0], etc., where 0 can be replaced with your selection number.",
        ana().sel().GetNEnabledSelections());
    }
  }
  else{
    if (ana().sel().GetNMaxBranches()>1) {
      ana().output().AddVectorVar(OutputManager::truth, AnalysisAlgorithm::accum_level, "accum_level", "I",
        "The number of the first cut that was failed for each branch. Unlike the variable with the same name in the standard micro-trees, \n"
        "this one has a single toy running multiple toys \n"
        "The first index is the branch.\n"
        "Then you can use accum_level[0], etc., where 0 can be replaced with your branch number.",
        ana().sel().GetNMaxBranches());
    }
    else {
      ana().output().AddVar(OutputManager::truth, AnalysisAlgorithm::accum_level, "accum_level", "I",
        "The number of the first cut that was failed for each branch. Unlike the variable with the same name in the standard micro-trees, \n"
        "this one has a single toy running multiple toys \n"
        "Then you can use accum_level to access the correct number.");
    }
  }

  // Add variables only to the truth tree
  ana().output().SetFillSingleTree(OutputManager::truth);

  //---- Add variables to the tree for the different track categories ------
  Int_t categ_index = AnalysisAlgorithm::firstCategory;
  Int_t counter_index = AnalysisAlgorithm::firstCategoryCounter;
  for (std::map< std::string, TrackCategoryDefinition*>::iterator it=ana().cat().GetCategories().begin();it!=ana().cat().GetCategories().end();it++, categ_index++){
    std::string categ_name = it->first;
    TrackCategoryDefinition& categ = *(it->second);
    if (categ.IsMultiType()){
      ana().output().AddVectorVar(categ_index, categ_name, "I","This is a category variable that can be used to split up a MC histogram into a stack. Use the DumpCategory(\"...\") function for more.", counter_index, "N"+categ_name, categ.GetNTypes());
      counter_index++;
    }
    else if (!categ.IsObject()) 
      ana().output().AddVar(categ_index, categ_name,"I","This is a category variable that can be used to split up a MC histogram into a stack. Use the DumpCategory(\"...\") function for more.");
  }

  // Define analysis specific variables
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineTruthTree);
  ana().DefineTruthTree();

  // Unset this field
  ana().output().SetFillAllTrees();
}

//********************************************************************
void AnalysisLoop::FillConfigTree(){
//********************************************************************

  // Add a tree
  ana().output().AddTreeWithName(OutputManager::config, "config");

  ana().output().SetFillSingleTree(OutputManager::config);

  // The $HIGHLANDPATH
  ana().output().AddVar(OutputManager::config, AnalysisAlgorithm::HIGHLANDPATH,   "HIGHLANDPATH",   "C", "the HIGHLAND used to produce the output file");  
  ana().output().FillVar(AnalysisAlgorithm::HIGHLANDPATH, (std::string)getenv("HIGHLANDPATH") );  

  // The Machine name
  ana().output().AddVar(OutputManager::config, AnalysisAlgorithm::HOSTNAME,   "HOSTNAME",   "C", "the machine used to produce the output file");  
  if (getenv("HOSTNAME"))
    ana().output().FillVar(AnalysisAlgorithm::HOSTNAME, (std::string)getenv("HOSTNAME") );  
  else 
    ana().output().FillVar(AnalysisAlgorithm::HOSTNAME, "unknown" );

  // The input file name
  ana().output().AddVar(OutputManager::config, AnalysisAlgorithm::INPUTFILE,   "InputFile",   "C", "the input file used to produce the output file");  
  ana().output().FillVar(AnalysisAlgorithm::INPUTFILE, _inputFileName );  

  // The original file (i.e. the recon output file)
  ana().output().AddVar(OutputManager::config, AnalysisAlgorithm::OriginalFile,   "OriginalFile",   "C", "the original file used to produce the input file");  


  if (!ana().input().InputIsOriginalTree()){
    TChain* chain = new TChain("config");
    chain->AddFile(_inputFileName.c_str());    
    char OriginalFile[200]="unknown";
    if (chain->FindLeaf("OriginalFile")){
      chain->SetBranchAddress("OriginalFile", OriginalFile);
      Long64_t centry = chain->LoadTree(0);
      Int_t nb = chain->GetEntry(0);
      (void) centry;
      (void) nb;
    }
    ana().output().FillVar(AnalysisAlgorithm::OriginalFile, OriginalFile );  
  }
  else
    ana().output().FillVar(AnalysisAlgorithm::OriginalFile, _inputFileName );  


  ana().output().AddVar(OutputManager::config, AnalysisAlgorithm::MinAccumLevelToSave,   "MinAccumLevelToSave",   "I", "the minimum accut level to save the event");  
  ana().output().FillVar(AnalysisAlgorithm::MinAccumLevelToSave,   ana().GetMinAccumCutLevelToSave());  


  // The software versions
  ND::versioning().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // TrackCategories
  ana().cat().WriteTreeMap(*ana().output().GetTree(OutputManager::config));

  // selections
  ana().sel().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // docstrings
  ana().doc().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // Configurations
  ana().conf().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // Systematics
  ana().syst().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // Corrections
  ana().corr().WriteTreeVector(*ana().output().GetTree(OutputManager::config));

  // Fill user defined variables
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFillConfigTree);
  ana().FillConfigTree();
  
  // Put back set fill all trees
  ana().output().SetFillAllTrees();

  // Fill the tree
  ana().output().GetTree(OutputManager::config)->Fill();
}

//********************************************************************
void AnalysisLoop::FillHeaderTree(){
//********************************************************************

  // Add a tree
  ana().output().AddTreeWithName(OutputManager::header,"header");

  Header* headerp = &ana().input().header();
  ana().output().GetTree(OutputManager::header)->Bronch("POTInfo","Header",&headerp,32000,0);

  ana().output().GetTree(OutputManager::header)->Fill();
}

//********************************************************************
void AnalysisLoop::DefineCorrections(){
//********************************************************************

  // Read corrections from the input file

  // When running over a FlatTree corrections may already exist in it.
  // It that case corrections are read from the input file (config tree) and added to the CorrectionManager. 
  // In this case each correction is stored as 
  // "appliedInInput" and "disabled", such that it is not applied twice. 

  if (!ana().input().InputIsOriginalTree()){
    bool isROOTFile = ana().input().IsROOTFile(_inputFileName);
    std::string firstFile = ana().input().FindFirstFile(_inputFileName, isROOTFile);
    if (!isROOTFile)
      std::cout << "ReadCorrections from first file in list: " << firstFile << std::endl;
    ana().corr().ReadCorrections(firstFile, true);
  }
  // Add other corrections
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineCorrections);
  ana().DefineCorrections();

  // Print on the screen the corrections
  ana().corr().DumpCorrections();
}

//********************************************************************
void AnalysisLoop::DefineConfigurations(){
//********************************************************************

  // Add a default configuration with a single toy experiment
  ana().conf().AddConfiguration(ConfigurationManager::default_conf, ConfigurationManager::default_conf_name,1,1,new ZeroToyMaker());

  // Set "default" as the initial configuration, so that corrections can be applied.
  ana().conf().SetCurrentConfigurationIndex(ConfigurationManager::default_conf);

  // Add other configurations
  ana().SetAnalysisPoint(AnalysisAlgorithm::kDefineConfigurations);
  ana().DefineConfigurations();

  // Create the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
  // This must be done after defining the configurations
  // The number of toys created is the one corresponding to the configuration with more toys

  for (UInt_t i=0;i<ana().eweight().GetNEventWeights();i++){
    EventWeightBase* it = ana().eweight().GetEventWeights()[i];
    ana().syst().AddWeightSystematic(it->GetIndex(), it->GetName(), it);
    //    ana().syst().AddSystematic(it->GetIndex(), it);
  }
  for (UInt_t i=0;i<ana().evar().GetNEventVariations();i++){
    EventVariationBase* it = ana().evar().GetEventVariations()[i];
    ana().syst().AddVariationSystematic(it->GetIndex(), it->GetName(), it);
    //    ana().syst().AddSystematic(it->GetIndex(), it);
  }


  for (std::vector<ConfigurationBase*>::iterator it= ana().conf().GetConfigurations().begin();it!=ana().conf().GetConfigurations().end();it++){
    ConfigurationBase* conf = *it;
    conf->EnableSystematics(conf->GetEnabledEventWeights());
    conf->EnableSystematics(conf->GetEnabledEventVariations());
  }

  ana().conf().CreateToyExperiments(ana().syst());

  // Dump all configurations
  ana().conf().DumpConfigurations(&ana().syst());
}
  
//********************************************************************
void AnalysisLoop::Loop(int nmax,int imin){
//********************************************************************
  
  if (!Initialize()) return;
  
  // Get the number of entries in the tree
  Long64_t nentries = ana().input().GetEntries();  

  if (imin>nentries){
    std::cout << "AnalysisLoop::Loop(). input tree has " << nentries << " entries. You cannot start from entry " << imin << std::endl;    
    return;
  }

  // Compute the number of entries to be run
  if (nmax==0 || imin+nmax>nentries) _entry_nmax = nentries-imin;
  else                               _entry_nmax = nmax;

  // Compute the number of the last entry to be run  
  _entry_imax = _entry_imin+_entry_nmax;

  if (ana().input().InputIsFlatTree())
    std::cout << "AnalysisLoop::Loop(). input tree has " << nentries << " entries for " << ana().input().header().GetPOT() << " good POT" << std::endl;
  else
    std::cout << "AnalysisLoop::Loop(). input tree has " << nentries << " entries (POT counted on the fly)" << std::endl;
  std::cout << "AnalysisLoop::Loop(). loop over " << _entry_nmax << " entries from entry number "<< _entry_imin << std::endl;

  double delta_t[20]={0};

  //--------- Loop over entries in the tree ----------------------------------
  _entry=_entry_imin;
  while (_entry<_entry_imax) {

    // Initialize clock
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);

    // Fills the full Spill structure (it could imply reading several entries in input tree)
    bool spillOK = InitializeSpill();

    // Break the loop (at the moment only for skimmed events, when the last interesting event is processed)
    if (anaUtils::breakLoop) break;

    // go to the next spill if this one is not OK
    if (!spillOK) continue;
    
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    delta_t[0] += t1-t0;

    //---- For each Spill loop over Bunches ----
    for (unsigned int ibunch=0;ibunch<ana().input().GetSpill().Bunches.size();ibunch++){
      gettimeofday(&tim, NULL);
      double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

      ana().input().SetCurrentBunch(ibunch);
      InitializeBunch();

      gettimeofday(&tim, NULL);
      double t3=tim.tv_sec+(tim.tv_usec/1000000.0);


      delta_t[1] += t3-t2;


      //---- For each bunch loop over configurations ----     
      for (std::vector<ConfigurationBase*>::iterator it= ana().conf().GetConfigurations().begin();it!=ana().conf().GetConfigurations().end();it++){
      	ConfigurationBase* conf = *it;

        gettimeofday(&tim, NULL);
        double t4=tim.tv_sec+(tim.tv_usec/1000000.0);
        
        if (!conf->IsEnabled()) continue;
        ana().conf().SetCurrentConfigurationIndex(conf->GetIndex());
        ana().output().SetCurrentTree(conf->GetTreeIndex());
        InitializeConfiguration();      

        gettimeofday(&tim, NULL);
        double t5=tim.tv_sec+(tim.tv_usec/1000000.0);
        
        delta_t[2] += t5-t4;
        
        _conf_passed=false;
        
        //---- For each configuration loop over toys --------
        for (int n=0;n<conf->GetNToys();n++){
          
          gettimeofday(&tim, NULL);
          double t6=tim.tv_sec+(tim.tv_usec/1000000.0);

          ana().conf().SetToyIndex(n);
          ana().output().SetToyIndex(n);
          InitializeToy();

          gettimeofday(&tim, NULL);
          double t7=tim.tv_sec+(tim.tv_usec/1000000.0);
          
          delta_t[3] += t7-t6;

          //---- For each toy loop over selections --------
          for (std::vector<SelectionBase*>::iterator sit= ana().sel().GetSelections().begin();sit!=ana().sel().GetSelections().end();sit++){
            SelectionBase& selec = **sit;
            if (!selec.IsEnabled()) continue;                            
            gettimeofday(&tim, NULL);
            double t8=tim.tv_sec+(tim.tv_usec/1000000.0);

            InitializeSelection(selec);

            gettimeofday(&tim, NULL);
            double t9=tim.tv_sec+(tim.tv_usec/1000000.0);

            delta_t[4] += t9-t8;

            _toy_passed = Process(selec);      

            gettimeofday(&tim, NULL);
            double t10=tim.tv_sec+(tim.tv_usec/1000000.0);

            delta_t[5] += t10-t9;

            FinalizeSelection(selec);

            gettimeofday(&tim, NULL);
            double t11=tim.tv_sec+(tim.tv_usec/1000000.0);


            delta_t[6] += t11-t10;
            if (_toy_passed) _conf_passed=true;            
          }

          gettimeofday(&tim, NULL);
          double t12=tim.tv_sec+(tim.tv_usec/1000000.0);

          if (!FinalizeToy()) break; 
          
          gettimeofday(&tim, NULL);
          double t13=tim.tv_sec+(tim.tv_usec/1000000.0);
          
          delta_t[7] += t13-t12;
          
        }
        
        gettimeofday(&tim, NULL);
        double t14=tim.tv_sec+(tim.tv_usec/1000000.0);
        
        FinalizeConfiguration();
        
        gettimeofday(&tim, NULL);
        double t15=tim.tv_sec+(tim.tv_usec/1000000.0);
        
        delta_t[8] += t15-t14;
      }      
      gettimeofday(&tim, NULL);
      double t16=tim.tv_sec+(tim.tv_usec/1000000.0);

      FinalizeBunch();

      gettimeofday(&tim, NULL);
      double t17=tim.tv_sec+(tim.tv_usec/1000000.0);

      delta_t[9] += t17-t16;
    }
    gettimeofday(&tim, NULL);
    double t18=tim.tv_sec+(tim.tv_usec/1000000.0);
    FinalizeSpill();
    gettimeofday(&tim, NULL);
    double t19=tim.tv_sec+(tim.tv_usec/1000000.0);

    delta_t[10] += t19-t18;

    delta_t[11] += t19-t0;
  }

  for (Int_t i=0;i<11;i++)
    delta_t[12] += delta_t[i];


  std::cout << "time profile --------------" << std::endl;
  std::cout << "Ini Spill:        " << delta_t[0] << std::endl;    
  std::cout << "Ini Bunch:        " << delta_t[1] << std::endl;    
  std::cout << "Ini Conf:         " << delta_t[2] << std::endl;    
  std::cout << "Ini Toy (v syst): " << delta_t[3] << std::endl;    
  std::cout << "Ini Sel:          " << delta_t[4] << std::endl;    
  std::cout << "Selection:        " << delta_t[5] << std::endl;    
  std::cout << "End Sel (w syst): " << delta_t[6] << std::endl;    
  std::cout << "End Toy:          " << delta_t[7] << std::endl;    
  std::cout << "End Conf:         " << delta_t[8] << std::endl;    
  std::cout << "End Bunch:        " << delta_t[9] << std::endl;    
  std::cout << "End Spill:        " << delta_t[10] << std::endl;   
  std::cout << "Total:            " << delta_t[11] << std::endl;   
  std::cout << "Total wo t:       " << delta_t[12] << std::endl;       



  Finalize();
}

//********************************************************************
bool AnalysisLoop::Process(SelectionBase& selec){
//********************************************************************

  // Returns true when any of the branches is passed
  // Internaly the CheckRedoSelection is called. The returned boolean is returned as argument 
  // by the Apply method of the selection such that we can save the redo value in the micro-tree
  bool redo = false;
  bool ToyPassed = selec.Apply(*_event, redo);

  // fill a variable saying if selection was  redone for this toy experiment
  if (_fillTrees)
    ana().output().FillToyVar(AnalysisAlgorithm::redo, (int)redo);

  return ToyPassed;
}

//********************************************************************
void AnalysisLoop::FillMicroTrees(){
//********************************************************************

  // Fill the entry number in the input tree
  ana().output().FillVar(AnalysisAlgorithm::entry, (int) _entry);

  // Fill the tree for the current configuration provided the true codes for color drawing
  std::map< std::string, TrackCategoryDefinition* >::iterator it;
  Int_t categ_index = AnalysisAlgorithm::firstCategory;
  for (it=ana().cat().GetCategories().begin();it!=ana().cat().GetCategories().end();it++, categ_index++ ){
    std::string categ_name = it->first;
    TrackCategoryDefinition& categ = *(it->second);
    if (categ.IsMultiType()){
      for (unsigned int i=0;i<categ.GetNTypes();i++)
         ana().output().FillVectorVar(categ_index, (int)ana().cat().CheckCategoryType(categ_name,i),i);
    }
    else if (categ.IsObject()){
      for (UInt_t i=0;i<categ._codes.size();i++)
        ana().output().FillVectorVarForceIndex(categ_index, categ.GetObjectCode(i),i);
    }
    else ana().output().FillVar(categ_index, ana().cat().GetCode(categ_name));
  }

  // reference toy
  ana().output().FillVar(AnalysisAlgorithm::toy_ref, ana().conf().GetCurrentConfiguration()->GetRefToyIndex());

  // Call the derive classes functions
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFillMicroTrees);
  ana().FillMicroTrees();

  // Call the base class function and actually fills the tree
  // (fill the toy configuration variables: toy_index, toy_weight)
  ana().output().FillMicroTrees();
}

//********************************************************************
void AnalysisLoop::FillToyVarsInMicroTrees(){
//********************************************************************

  //Fill the Toy Experiment Index. Should be filled always !!!!
  ana().output().FillToyVar(OutputManager::toy_index, ana().conf().GetToyIndex());

  if (!_toy_passed && ana().GetFillSuccessOnly()) return;
  if (!ana().CheckAccumLevelToSave()) return;

  // This variables is reseted in InitializeConfiguration.
  // If true means that at least one toy fullfils the condition to be saved into the microtree
  _toy_filled = true;
  
  // ---------------- Fill cut level variables ------------------------

  Int_t isel=0;
  for (std::vector<SelectionBase*>::iterator it=ana().sel().GetSelections().begin();it!=ana().sel().GetSelections().end();it++){
    if (!(*it)->IsEnabled()) continue;
    for (UInt_t ibranch=0;ibranch<(*it)->GetNBranches();ibranch++){
      Int_t indx=AnalysisAlgorithm::first_cut;
      if (ana().sel().GetNEnabledSelections()>1){
        if (ana().sel().GetNMaxBranches()>1){
          ana().output().FillToyMatrixVar(AnalysisAlgorithm::accum_level, (*it)->GetAccumCutLevel(ibranch), isel, ibranch);
          for (unsigned int i=0;i<(*it)->GetNCuts(ibranch);i++, indx++){  
            ana().output().FillToyMatrixVar(indx, (*it)->GetCutPassed(i, ibranch), isel, ibranch);
          }	
        }
        else{
          ana().output().FillToyVectorVar(AnalysisAlgorithm::accum_level, (*it)->GetAccumCutLevel(ibranch), isel);
          for (unsigned int i=0;i<(*it)->GetNCuts(ibranch);i++, indx++){  
            ana().output().FillToyVectorVar(indx, (*it)->GetCutPassed(i, ibranch), isel);
          }	
        }
      }
      else{
        if (ana().sel().GetNMaxBranches()>1){
          ana().output().FillToyVectorVar(AnalysisAlgorithm::accum_level, (*it)->GetAccumCutLevel(ibranch), ibranch);
          for (unsigned int i=0;i<(*it)->GetNCuts(ibranch);i++, indx++){  
            ana().output().FillToyVectorVar(indx, (*it)->GetCutPassed(i, ibranch), ibranch);
          }
        }
        else{
          ana().output().FillToyVar(AnalysisAlgorithm::accum_level, (*it)->GetAccumCutLevel(ibranch));	
          for (unsigned int i=0;i<(*it)->GetNCuts(ibranch);i++, indx++){  
            ana().output().FillToyVar(indx, (*it)->GetCutPassed(i, ibranch));
          }
        }
      }
    }
    isel++;
  }

  //------ Fill the event weights for each toy experiment ----
  Int_t nWeightSyst = (Int_t)ana().eweight().GetNEnabledEventWeights();

  if (nWeightSyst>0){

    Int_t w = 0;
    Float_t weight_syst_total = 1.;
    Float_t weight_corr_total = 1.;
    
    for (int j = 0; j < nWeightSyst; ++j){
      Float_t weight_syst = 1.;
      Float_t weight_corr = 1.;
      if (w < nWeightSyst){
        weight_syst = _weightSyst[w].Systematic;
        weight_corr = _weightSyst[w].Correction;
      }
      if (ana().conf().GetCurrentConfigurationIndex() == ConfigurationManager::default_conf){
        ana().output().FillVectorVar(AnalysisAlgorithm::weight_syst, weight_syst, w);
        ana().output().FillVectorVar(AnalysisAlgorithm::weight_corr, weight_corr, w);
        w++;
      }
      else{
        ana().output().FillToyVectorVar(AnalysisAlgorithm::weight_syst, weight_syst, w);
        ana().output().FillToyVectorVar(AnalysisAlgorithm::weight_corr, weight_corr, w);
        w++;
      }
    //  if(weight_syst < 0.) weight_syst = 0.;
    //  if(weight_syst > 100.) weight_syst = 100.;
      if(weight_syst != weight_syst) {
        std::cerr << "Warning: systematic weight_syst is NaN, setting to 0!" << std::endl;
        weight_syst = 0.;
      }
    //  if(weight_corr < 0.) weight_corr = 0.;
    //  if(weight_corr > 100.) weight_corr = 100.;
      if(weight_corr != weight_corr) {
        std::cerr << "Warning: correction weight_corr is NaN, setting to 0!" << std::endl;
        weight_corr = 0.;
      }
      
      weight_syst_total *= weight_syst;
      weight_corr_total *= weight_corr;
    }
    
    // -------- Fill the total weight systematic -------------------
    if (ana().conf().GetCurrentConfigurationIndex() == ConfigurationManager::default_conf){
      ana().output().FillVar(AnalysisAlgorithm::weight_syst_total, weight_syst_total);
      ana().output().FillVar(AnalysisAlgorithm::weight_corr_total, weight_corr_total);
    }
    else{
      ana().output().FillToyVar(AnalysisAlgorithm::weight_syst_total, weight_syst_total);
      ana().output().FillToyVar(AnalysisAlgorithm::weight_corr_total, weight_corr_total);
    }
    ana().output().FillVar(AnalysisAlgorithm::NWEIGHTSYST, nWeightSyst);
  }

  // Fill the user toy experiment variables
  ana().SetAnalysisPoint(AnalysisAlgorithm::kFillToyVarsInMicroTrees);
  ana().FillToyVarsInMicroTrees();
}

//********************************************************************
void AnalysisLoop::PrintUsage(const std::string& programName){
//********************************************************************
  std::cout << "usage: " << programName << " [options] [input-file]" << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    -c                Run in cosmics mode, putting all tracks in one 'bunch'" << std::endl;
  std::cout << "    -o <file>         Set the name of an output file (mandatory)" << std::endl;
  std::cout << "    -n <cnt>          Only read <cnt> events" << std::endl;
  std::cout << "    -s <cnt>          Skip <cnt> events" << std::endl;
  std::cout << "    -e <file>         Set the name of file containing events to skim (format: run subrun event)" << std::endl;
  std::cout << "    -p <file>         Set the name of a parameter override file" << std::endl;
  std::cout << "    -v                Don't Check version compatibility between oaAnalysisReader and oaAnalysis file" << std::endl;
  std::cout << "    -r                Don't fill any trees and don't create an output file (mainly for speed tests)" << std::endl;
  std::cout << "    -m                Dumps on the screen the memory usage" << std::endl;
  std::exit(1);
}

//********************************************************************
void AnalysisLoop::Execute(){
//********************************************************************
  Loop(_entry_nmax, _entry_imin);
}

 
