#include "baseAnalysis.hxx"

#include "HighlandMiniTreeConverter.hxx"
#include "anaTreeConverter.hxx"
#include "anaTreeNewConverter.hxx"
#include "nueAnaTreeConverter.hxx"
#include "pionTreeConverter.hxx"
#include "LArSoftTreeConverter.hxx"

#include "baseToyMaker.hxx"

//********************************************************************
baseAnalysis::baseAnalysis(AnalysisAlgorithm* ana): AnalysisAlgorithm(ana){
  //********************************************************************

  // Add the package version
  // Add package versions
  ND::versioning().AddPackage("psychePolicy",        anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEPOLICYROOT")));
  ND::versioning().AddPackage("psycheEventModel",    anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEEVENTMODELROOT")));
  ND::versioning().AddPackage("psycheCore",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHECOREROOT")));
  ND::versioning().AddPackage("psycheUtils",         anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEUTILSROOT")));
  ND::versioning().AddPackage("psycheDUNEUtils",     anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEDUNEUTILSROOT")));
  ND::versioning().AddPackage("psycheIO",            anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEIOROOT")));
  ND::versioning().AddPackage("psycheSelections",    anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHESELECTIONSROOT")));
  //  ND::versioning().AddPackage("psycheSystematics",   anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHESYSTEMATICSROOT")));
  ND::versioning().AddPackage("highlandEventModel",  anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDEVENTMODELROOT")));
  ND::versioning().AddPackage("highlandTools",       anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDTOOLSROOT")));
  ND::versioning().AddPackage("highlandCore",        anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCOREROOT")));
  ND::versioning().AddPackage("highlandCorrections", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCORRECTIONSROOT")));
  ND::versioning().AddPackage("highlandIO",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDIOROOT")));

  ND::versioning().AddPackage("baseAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("BASEANALYSISROOT")));
}

//********************************************************************
void baseAnalysis::DefineProductions(){
//********************************************************************

  // Add the different productions
  ND::versioning().AddProduction(ProdId::MCC5,     "MCC5",     "v0r0",  "v1r0");
  ND::versioning().AddProduction(ProdId::MCC6,     "MCC6",     "v0r0",  "v1r0");
  ND::versioning().AddProduction(ProdId::MCC7,     "MCC5",     "v0r0",  "v1r0");
}

//********************************************************************
void baseAnalysis::DefineInputConverters(){
//********************************************************************

  // add the different converters
  input().AddConverter("LArSoftTree",    new LArSoftTreeConverter());
  input().AddConverter("MiniTree",       new HighlandMiniTreeConverter());
  input().AddConverter("anatree",        new anaTreeConverter());
  input().AddConverter("anatree_new",    new anaTreeNewConverter());
  input().AddConverter("nueana",         new nueAnaTreeConverter());
  input().AddConverter("pionana",        new pionTreeConverter());
}

//********************************************************************
bool baseAnalysis::Initialize(){
//********************************************************************

  return true;
}

//********************************************************************
bool baseAnalysis::InitializeBase(){
  //********************************************************************

  // Initialize trees or not at the beginnng of each configuration
  SetInitializeTrees(ND::params().GetParameterI("baseAnalysis.InitializeTrees"));
  
  if (_versionCheck){
    if(!ND::versioning().CheckVersionCompatibility(ND::versioning().GetProduction(input().GetSoftwareVersion()),
                                                   anaUtils::GetProductionIdFromoaAnalysisReader())) return false;
  }

  // Dump the production used for corrections, bunching, systematics, etc
  versionUtils::DumpProductions();


  // This will take care of data/MC differences in detector volumes definitions 	 
  // Should be applied after the version has been defined
  // TODO. Moved temporarily from AnalysisLoop to avoid dependency of highlandCore on psycheND280Utils
  //  ND::hgman().InitializeGeometry(input().GetIsMC()); TODO DUNE
  
  return true;
}


//********************************************************************
void baseAnalysis::DefineSystematics(){
//********************************************************************

  // We add here EventWeights and EventVariations that are common to all analyses
  /*
  eweight().AddEventWeight(SystId::kSIPion,           "SIPion",           new SIPionSystematics());
  eweight().AddEventWeight(SystId::kSIProton,         "SIProton",         new SIProtonSystematics());

  eweight().AddEventWeight(SystId::kFluxWeightNu,     "FluxWeightNu",     new FluxWeightSystematicsNeutrino());
  eweight().AddEventWeight(SystId::kFluxWeightAntiNu, "FluxWeightAntiNu", new FluxWeightSystematicsAntiNeutrino());
  */
}

//********************************************************************
void baseAnalysis::DefineCorrections(){
  //********************************************************************

  /*
  //----------- Define all corrections ----------------
  if (ND::params().GetParameterI("baseAnalysis.Corrections.DisableAllCorrections")){
    // Should not be needed, but just in case !!!
    corr().DisableAllCorrections();
  }
  else{
    // Add corrections only when they are enabled. In that way the CorrectionManager does not have to loop over unused corrections

    // Ignore right ECal for runs 3 and 4 as part of it is broken.
    if (ND::params().GetParameterI("baseAnalysis.Corrections.EnableIgnoreRightECal"))  corr().AddCorrection("ignorerightecal_corr", new IgnoreRightECalRuns3and4Correction());     
    // Correct the data quality in periods when a FGD FEB wasn't working.
    if (ND::params().GetParameterI("baseAnalysis.Corrections.EnableDQ"))               corr().AddCorrection("dq_corr",              new DataQualityCorrection());   
  }
  // If set to true corrections applied in the input file are applied again
  corr().SetForceApplyCorrections((bool)ND::params().GetParameterI("baseAnalysis.Corrections.ForceApplyCorrections"));
  */
}

//********************************************************************
void baseAnalysis::DefineConfigurations(){
  //********************************************************************

  //-------  Add and define individual configurations with one systematic only ------------------

  _enableSingleVariationSystConf = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleVariationSystConfigurations");
  _enableSingleWeightSystConf    = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
  _enableAllSystConfig           = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableAllSystematics");

  _ntoys = (Int_t)ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");   // The number of Toy Experiments 
  _randomSeed = (Int_t)ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");   // The random seed to generate the ToyExperiments

  /*
  if (_enableSingleWeightSystConf){
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIPion")){
      AddConfiguration(conf(), sipion_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kSIPion,sipion_syst);
    }
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIProton")){
      AddConfiguration(conf(), siproton_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kSIProton,siproton_syst);
    }
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFluxNeutrino")) {
      AddConfiguration(conf(), nuflux_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFluxWeightNu    , nuflux_syst);
    }    
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFluxAntiNeutrino")) {
      AddConfiguration(conf(), antinuflux_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFluxWeightAntiNu , antinuflux_syst);
    }  
  }
  */  

  // A configuration with all systematics
  if (_enableAllSystConfig){ 
    AddConfiguration(conf(), all_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));  
  }

  /*
  // Enable all Event Weights in the default and all_syst configurations
  for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
    Int_t index = (*it)->GetIndex();
    if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIPion"))            conf().EnableEventWeight(SystId::kSIPion            , index);
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIProton"))          conf().EnableEventWeight(SystId::kSIProton          , index);
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFluxNeutrino"))      conf().EnableEventWeight(SystId::kFluxWeightNu      , index);
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFluxAntiNeutrino"))  conf().EnableEventWeight(SystId::kFluxWeightAntiNu  , index);	
  }
  */
}


//********************************************************************
void baseAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  (void)addBase;

  // -------- Add variables to the analysis tree ----------------------

  //--- event variables -------
  AddVarI(output(), run,    "run number");
  AddVarI(output(), subrun, "subrun number ");
  AddVarI(output(), evt,    "event number ");
  AddVarI(output(), bunch,  "bunch number ");

  // --- Vertex info
  AddVarI(  output(), selvtx_det,       "detector in which the reconstructed vertex is");
  AddVar4VF(output(), selvtx_pos,       "reconstructed vertex position");
  AddVar4VF(output(), selvtx_truepos,   "position of the true vertex associated to the reconstructed vertex");
}

//********************************************************************
void baseAnalysis::DefineTruthTree(){
  //********************************************************************

  //--- event variables -------
  AddVarI(output(),   evt,    "event number");
  AddVarI(output(),   run,    "run number");
  AddVarI(output(),   subrun, "subrun number");
}

//********************************************************************
bool baseAnalysis::FinalizeConfiguration(){
  //********************************************************************

  // This is called before FillMicroTrees()

  // Save the accum level for the true vertex associated to the recon one such that efficiencies can be computed from the truth tree
  if (conf().GetCurrentConfigurationIndex() != ConfigurationManager::default_conf)
    return true;


  std::vector<AnaTrueVertex*> trueVertices;
  trueVertices.reserve(NMAXTRUEVERTICES);
  std::vector<AnaTrueVertex*>::const_iterator iter;


  AnaTrueVertex* trueVertex = NULL;
  if (GetVertex()) trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
  else             trueVertex = static_cast<AnaTrueVertex*>(GetTrueVertex());

  if (trueVertex) trueVertices.push_back(trueVertex);


  // If true vertex does not exist (e.g. can happen that reco vertex is not yet available at this step of the selection) then 
  // store the accum_level to all true vertices of the bunch -> i.e. this basically corresponds to the fact that event as a whole 
  // passed some cuts
  if (trueVertices.size() == 0){
    // Loop over all true vertices in the event and found those that belong to the bunch being processed
    std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
    for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
      if (!(*it)) continue;
      AnaTrueVertex* vtx = static_cast<AnaTrueVertex*>(*it);

      // Check the bunch
      if (GetBunch().Bunch != vtx->Bunch) continue;

      trueVertices.push_back(vtx); 
    }
  }

  // Loop over all true vertices of interest
  for (iter = trueVertices.begin(); iter != trueVertices.end(); iter++){ 
    AnaTrueVertex* vtx = *iter;
    if (!vtx) continue;

    // When the AccumLevel has not been already saved for this vertex 
    if (vtx->AccumLevel.size() == 0)
      vtx->AccumLevel.resize(sel().GetNEnabledSelections());
    //else{
    // Sometimes the same true vertex is asigned to the candidate in another bunch, most likely because of a delayed track. In this case 
    // save the higher accum level
    // std::cout << "baseAnalysis::FinalizeConfiguration(). This true vertex was used in another bunch (likely a delayed track). Save higher accum_level" << std::endl;
    //}
    for (std::vector<SelectionBase*>::iterator it = sel().GetSelections().begin(); it != sel().GetSelections().end(); it++){

      if (!(*it)->IsEnabled()) continue;

      Int_t isel = (*it)->GetEnabledIndex();

      if (vtx->AccumLevel[isel].size() == 0)
        vtx->AccumLevel[isel].resize((*it)->GetNBranches());

      for (UInt_t ibranch=0;ibranch<(*it)->GetNBranches();ibranch++)
        vtx->AccumLevel[isel][ibranch]=(*it)->GetAccumCutLevel(ibranch);
    }
  }
  return true;
}

//********************************************************************
void baseAnalysis::FillMicroTreesBase(bool addBase){
  //********************************************************************

  (void)addBase;

  // This is called after FinalizeConfiguration

  // Fill all tree variables that are common to all toys in a given configuration

  // Event variables
  output().FillVar(run,    GetSpill().EventInfo->Run);
  output().FillVar(subrun, GetSpill().EventInfo->SubRun);
  output().FillVar(evt,    GetSpill().EventInfo->Event);
  output().FillVar(bunch,  GetBunch().Bunch);

  // Vertex info
  if (GetVertex()){
    output().FillVar(selvtx_det,anaUtils::GetDetector(GetVertex()->Position));
    output().FillVectorVarFromArray(selvtx_pos,GetVertex()->Position, 4);
    if (GetVertex()->TrueVertex)
      output().FillVectorVarFromArray(selvtx_truepos, GetVertex()->TrueVertex->Position, 4);
  }

}

//********************************************************************
void baseAnalysis::FillToyVarsInMicroTreesBase(bool addBase){
  //********************************************************************

  (void)addBase;

  if (!GetVertex()) return;
  if (!GetVertex()->TrueVertex) return;

  // Nothing here for the moment
  
  //  AnaTrueVertex* trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);

}

//********************************************************************
void baseAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det, bool IsAntinu){
  //********************************************************************

  // Fill track categories for color drawing
  anaUtils::FillCategories(&vtx, det, IsAntinu, GetSpill().GetIsSandMC());

  // Event variables
  output().FillVar(run,    GetSpill().EventInfo->Run);
  output().FillVar(subrun, GetSpill().EventInfo->SubRun);
  output().FillVar(evt,    GetSpill().EventInfo->Event);
}

//********************************************************************
void baseAnalysis::FillTruthTree(){
//********************************************************************

  // Fill the "truth" tree

  if (!output().HasTree(OutputManager::truth)) return;

  output().SetCurrentTree(OutputManager::truth);

  // Loop over all true vertices
  std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
  for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *static_cast<AnaTrueVertex*>(*it);

    // Check if this vertex needs to be saved in the truth tree
    if (!CheckFillTruthTree(vtx)) continue;

    // Initialize the truth tree. We must do that for each saved vertex since several entries may correspond to the save spill
    output().InitializeTree(OutputManager::truth);


    // accumulated cut levels to compute efficiencies. This is taken directly from the AnaTrueVertex
    Int_t accumLevel=1;  // intialize to 1 because EventQuality is passed by definition in MC (TODO: what about analysis in which the first cut is not EventQuality)
    for (std::vector<SelectionBase*>::iterator itf=sel().GetSelections().begin();itf!=sel().GetSelections().end();itf++){
      if (!(*itf)->IsEnabled()) continue;

      Int_t isel = (*itf)->GetEnabledIndex();

      for (UInt_t ibranch=0;ibranch<(*itf)->GetNBranches();ibranch++){
        if (vtx.AccumLevel.size()>0) accumLevel = vtx.AccumLevel[isel][ibranch];
        if (sel().GetNEnabledSelections()>1){
          if (sel().GetNMaxBranches()>1)
            output().FillMatrixVar(accum_level, accumLevel, isel, ibranch);
          else
            output().FillVectorVar(accum_level, accumLevel, isel);
        }
        else{
          if (sel().GetNMaxBranches()>1)
            output().FillVectorVar(accum_level, accumLevel, ibranch);
          else
            output().FillVar(accum_level, accumLevel);
        }
      }
    }

    // Reset the categories for the current track
    cat().ResetCurrentCategories();

    // Call the derive classes functions, that also fills the categories
    FillTruthTree(vtx);

    // Fill the truth tree provided the true codes for color drawing
    std::map< std::string, TrackCategoryDefinition* >::iterator its;
    Int_t categ_index = AnalysisAlgorithm::firstCategory;
    for (its=cat().GetCategories().begin();its!=cat().GetCategories().end();its++, categ_index++ ){
      std::string categ_name = its->first;
      TrackCategoryDefinition& categ = *(its->second);
      if (categ.IsMultiType()){
        for (unsigned int i=0;i<categ.GetNTypes();i++)
           output().FillVectorVar(categ_index, (int)cat().CheckCategoryType(categ_name,i),i);
      }
      else output().FillVar(categ_index, cat().GetCode(categ_name));
    }

    // Fill the tree
    output().GetTree(OutputManager::truth)->Fill();
  }
}
