#include "duneExampleAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "duneExampleSelection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "EventBoxDUNE.hxx"

#include "DetectorMassWeight.hxx"
#include "dEdxVariation.hxx"

#include "baseToyMaker.hxx"

//********************************************************************
duneExampleAnalysis::duneExampleAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("duneExampleAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("DUNEEXAMPLEANALYSISROOT")));
}

//********************************************************************
bool duneExampleAnalysis::Initialize(){
//********************************************************************

  // Initialize the baseAnalysis
  if(!baseAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("duneExampleAnalysis.MinAccumLevelToSave"));

  // Define categories
  anaUtils::AddStandardCategories();
  anaUtils::AddStandardCategories("s");

  _detector = SubDetId::kSubdet1_2;

  return true;
}

//********************************************************************
void duneExampleAnalysis::DefineSelections(){
//********************************************************************

  sel().AddSelection("duneExample",    "inclusive duneExample selection",     new duneExampleSelection(false));     // true/false for forcing break

}

//********************************************************************
void duneExampleAnalysis::DefineCorrections(){
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();

}

//********************************************************************
void duneExampleAnalysis::DefineSystematics(){
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();


  eweight().AddEventWeight(SystId::kDetectorMass,           "DetectorMass",           new DetectorMassWeight());

  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void duneExampleAnalysis::DefineConfigurations(){
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

  if (_enableSingleVariationSystConf){
    if (ND::params().GetParameterI("duneExampleAnalysis.Variations.EnabledEdx")){
      AddConfiguration(conf(), dedx_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kdEdx,dedx_syst);
    }
  }


  if (_enableSingleWeightSystConf){
    if (ND::params().GetParameterI("duneExampleAnalysis.Weights.EnableDetectorMass")){
      AddConfiguration(conf(), detmass_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kDetectorMass,detmass_syst);
    }
  }
  
  // Enable all variation systematics in the all_syst configuration (created in baseAnalysis)
  if (_enableAllSystConfig){
    if (ND::params().GetParameterI("duneExampleAnalysis.Variations.EnabledEdx"))        conf().EnableEventVariation(SystId::kdEdx,         all_syst);
  }

  // Enable all Event Weights in the default and all_syst configurations
  for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
    Int_t index = (*it)->GetIndex();
    if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;
    if (ND::params().GetParameterI("duneExampleAnalysis.Weights.EnableDetectorMass"))   conf().EnableEventWeight(SystId::kDetectorMass, index);
  }
}

//********************************************************************
void duneExampleAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);

  AddVarI(  output(), ntracks,          "number of reconstructed tracks");

  // --- muon candidate truth variables ---
  AddVarF(  output(), selmu_truemom,    "muon candidate true momentum");
  AddVar4VF(output(), selmu_truepos,    "muon candidate true position");         
  AddVar4VF(output(), selmu_trueendpos, "muon candidate true end position");      
  AddVar3VF(output(), selmu_truedir,    "muon candidate true direction");         
  AddVarF(  output(), selmu_truebeta,   "muon candidate true beta");

  // --- true muon truth variables ---
  AddVarF(output(),   truemu_mom,       "true muon true momentum");
  AddVarF(output(),   truemu_costheta,  "true muon true cos(theta)");

  // --- muon candidate recon variables ---
  AddVar3VF(output(), selmu_dir,        "muon candidate reconstructed direction");
  AddVar4VF(output(), selmu_pos,        "muon candidate reconstructed position");
  AddVar4VF(output(), selmu_endpos,     "muon candidate reconstructed end position");
  AddVarF(  output(), selmu_costheta,   "muon candidate reconstructed cos(theta)");
  AddVarI(  output(), selmu_nhits,      "muon candidate number of hits");
  AddVarF(  output(), selmu_length,     "muon candidate length");
  AddVarF(  output(), selmu_dedx_raw,   "muon candidate average dEdx");
  AddVarFixVF( output(), selmu_dedx_exp,   "muon candidate expected dEdx for different hypothesis",4);

  AddToyVarF(output(), selmu_mom,      "muon candidate reconstructed momentum");
  AddToyVarF(output(), selmu_theta,    "muon candidate reconstructed polar angle");

  AddToyVarF(output(), selmu_dedx,     "muon candidate average dEdx");


  // --- neutrino truth variables ----
  AddVarI(  output(), nu_pdg,       "neutrino PDG code");
  AddVarF(  output(), nu_trueE,     "true neutrino energy");
  AddVarI(  output(), nu_truereac,  "true netrino reaction code");
  AddVar3VF(output(), nu_truedir,   "true neutrino direction");

  
}

//********************************************************************
void duneExampleAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from baseAnalysis (run, event, ...)
  baseAnalysis::DefineTruthTree();

  // ---- neutrino variables
  AddVarI(output(),   nu_pdg,            "neutrino pdg code");
  AddVarF(output(),   nu_trueE,          "true neutrino energy");  // true neutrino energy
  AddVarI(output(),   nu_truereac,       "true neutrino reaction type");  // true neutrino reaction code
  AddVar3VF(output(), nu_truedir,        "true neutrino direction");  // true neutrino direction
  AddVar4VF(output(), truevtx_pos,       "true neutrino interaction position");

  
  //--- muon variables -------
  AddVarF(  output(), truemu_mom,      "true muon true momentum");
  AddVarF(  output(), truemu_costheta, "true muon true cos(theta)");
  AddVar4VF(output(), truemu_pos,      "true muon true position");
  AddVar3VF(output(), truemu_dir,      "true muon true direction");

}

//********************************************************************
void duneExampleAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // WARNING: IF YOU RUN FGD1 AND FGD2 AT ONCE ( "whichFGD" parameter = 3), only for events with accum_level > 5
  // the vars in the output microtree will surely refer to the muon candidate in that FGD

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);


  // Neutrino truth variables
  if (GetVertex()){
    if (GetVertex()->TrueVertex){
      AnaTrueVertex* trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
      output().FillVar(nu_pdg,        trueVertex->NuPDG);
      output().FillVar(nu_trueE,      trueVertex->NuEnergy);
      output().FillVar(nu_truereac,   trueVertex->ReacCode);

      output().FillVectorVarFromArray(nu_truedir,        trueVertex->NuDir,    3);
    }
  }

  output().FillVar(ntracks,                        GetEvent().EventBoxes[EventBoxId::kEventBoxDUNE]->nRecObjectsInGroup[EventBoxDUNE::kLongTracks]);

  // Muon candidate variables
  if (box().MainTrack){

    // Properties of the true muon
    if(box().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);
      if(vtx) {
        output().FillVar(truemu_mom,     vtx->LeptonMom);
        output().FillVar(truemu_costheta,(Float_t)cos(anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir))));
      }
    }

    output().FillVar(selmu_dedx_raw,              static_cast<const AnaParticle*>(box().MainTrack->Original)->AveragedEdx);
    output().FillVar(selmu_nhits,                 box().MainTrack->NHits);
    output().FillVar(selmu_length,                box().MainTrack->Length);
    output().FillVar(selmu_costheta,              box().MainTrack->DirectionStart[2]);
    output().FillVectorVarFromArray(selmu_pos,    box().MainTrack->PositionStart, 4);
    output().FillVectorVarFromArray(selmu_endpos, box().MainTrack->PositionEnd, 4);
    output().FillVectorVarFromArray(selmu_dir,    box().MainTrack->DirectionStart, 3);


    // Properties of the true particle associated to the muon candidate
    if(  box().MainTrack->TrueObject ) {
      output().FillVar(selmu_truemom,          box().MainTrack->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selmu_truepos,   box().MainTrack->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selmu_trueendpos,box().MainTrack->GetTrueParticle()->PositionEnd, 4);
      output().FillVectorVarFromArray(selmu_truedir,   box().MainTrack->GetTrueParticle()->Direction, 3);

      output().FillVectorVar(selmu_dedx_exp, anaUtils::ExpecteddEdx(box().MainTrack->GetTrueParticle()->Momentum,ParticleId::kMuon),    0);
      output().FillVectorVar(selmu_dedx_exp, anaUtils::ExpecteddEdx(box().MainTrack->GetTrueParticle()->Momentum,ParticleId::kElectron),1);
      output().FillVectorVar(selmu_dedx_exp, anaUtils::ExpecteddEdx(box().MainTrack->GetTrueParticle()->Momentum,ParticleId::kProton),  2);
      output().FillVectorVar(selmu_dedx_exp, anaUtils::ExpecteddEdx(box().MainTrack->GetTrueParticle()->Momentum,ParticleId::kKaonPlus),3);

      if (anaUtils::GetParticleMass(ParticleId::GetParticle(box().MainTrack->GetTrueParticle()->PDG))>0)
        output().FillVar(selmu_truebeta,          anaUtils::ComputeBetaGamma(box().MainTrack->GetTrueParticle()->Momentum, ParticleId::GetParticle(box().MainTrack->GetTrueParticle()->PDG)));
    }
  }
}

//********************************************************************
void duneExampleAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Fill all tree variables that vary for each toy experiment

  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

  if (box().MainTrack){
    output().FillToyVar(selmu_dedx,              static_cast<AnaParticle*>(box().MainTrack)->AveragedEdx);
  }
}

//********************************************************************
bool duneExampleAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  (void)vtx;  // To avoid warning
  
  // Fill everything for the moment    
  return true;
}


//********************************************************************
void duneExampleAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // this method is also called from the antiNumu package with IsAntinu = true

  // Fill the common variables
  bool IsAntinu = false;
  baseAnalysis::FillTruthTreeBase(vtx, _detector, IsAntinu);

  // neutrino true variables
  output().FillVar(nu_pdg,        vtx.NuPDG);
  output().FillVar(nu_trueE,      vtx.NuEnergy);
  output().FillVar(nu_truereac,   vtx.ReacCode);
  output().FillVectorVarFromArray(nu_truedir,     vtx.NuDir, 3);
  output().FillVectorVarFromArray(truevtx_pos,    vtx.Position, 4);

  
  // Muon true variables
  output().FillVar(truemu_costheta,(Float_t)cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
  output().FillVar(truemu_mom,     vtx.LeptonMom);
  output().FillVectorVarFromArray(truemu_dir, vtx.LeptonDir,3);
  output().FillVectorVarFromArray(truemu_pos, vtx.Position, 4);
}

//********************************************************************
void duneExampleAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), box().MainTrack,"",  _detector);
  anaUtils::FillCategories(&GetEvent(), box().SLtrack,  "s", _detector);
}
