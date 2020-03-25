#include "pionAnalysis.hxx"
#include "Parameters.hxx"
#include "pionSelection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"

#include "dEdxVariation.hxx"
#include "dEdxCorrection.hxx"
#include "BeamMomCorrection.hxx"
#include "BeamMomSmearingCorrection.hxx"
#include "CryoWallBeamMomCorrection.hxx"
#include "dEdxDataCorrection.hxx"
#include "BrokenTrackDataCorrection.hxx"

#include "pionSelectionUtils.hxx"

#include "baseToyMaker.hxx"

/*
  A highland Analysis inherits in ultimate instance from AnalysisAlgorithm. 
  In this particular case an intermediate class baseAnalysis is used, 
  which does all the work that is common for DUNE analysis 
  Then pionAnalysis inherits from baseAnalysis, which inherits from AnalysisAlgorithm.
  
  The class that does the loops (over events, over toys, etc) is AnalysisLoop (under highlandTools). 
  There you can see the analysis flow, which is as follows

  LOOP over Spills{
    InitializeSpill
    LOOP over Bunches in the Spill{
      InitializeBunch
      LOOP over Configurations{
        InitializeConfiguration
        LOOP over Toy experiments for each configuration{
          InitializeToy
          LOOP over Enabled Selections{
            InitializeSelection
            Apply The Selection
            FinalizeSelection
          }
          FinalizeToy
          FillMicroTrees (Fill toy-dependent variables in micro-trees)
        }
        FillToyVarsInMicroTrees (Fill toy-independent variables)
        FillCategories (Fill categories for color code drawing)
        FinalizeConfiguration
      }
      FinalizeBunch
    }
    FillTruthTree (Fill the truth tree)
    FinalizeSpill
  }

  The Initialize.. and Finalize... methods can be implemented by the user to do more complicated analyses, but are not mandatory


  These is the list of mandatory methods to configure the analysis (call at initialization level):
  
  - Initialize:           This is all initializations that cannot be done in the constructor should be put 
                          (the ones that require access to a parameter in the parameters file)
  - DefineSelections:     Add to the SelectionManager the selections  we have defined in other files
  - DefineSystematics:    Add to the SystematicManager the systematics to be run (defined in other files)
  - DefineCorrections:    Add to the CorrectionManager the corrections to be run (defined in other files)
  - DefineConfigurations: Add to the ConfigurationManager the configurations to be considered
  - DefineMicroTrees:     Define the micro-tree variables
  - DefineTruthTree:      Define the variables of the truth tree

  These are the mandatory methods that are run for each Toy Experiment 

  - FillToyVarsInMicroTrees: Fill the micro-trees toy-dependent   variables (run for each toy experiment)

  These are the methods that are run for Event (Bunch)

  - FillMicroTrees:          Fill the micro-trees toy-independent variables. (MANDATORY) 
  - FillCategories:          Fill the micro-tree variables used for color code drawing (OPTIONAL)

  These are the mandatory methods that are run for each Spill

  - FillTruthTree:           Fill the truth tree variables
  - CheckFillTruthTree:      Check whether to include or not  a given true vertex in the truth tree

  These are the methods that are run at initialization level

  - FillConfigTree:           Fill the user defined variables in the single-entry config tree
  
*/


//********************************************************************
pionAnalysis::pionAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("pionAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("DUNEEXAMPLEANALYSISROOT")));
}

//********************************************************************
bool pionAnalysis::Initialize(){
//********************************************************************

  /* In this method we Initialize everything that requires access to parameters in the parameters file. 
     This is because in order to the overwride parameters file 
     option (-p param.dat) to work, parameters cannot be accessed in the constructors. 
  */

  
  // Initialize the baseAnalysis
  if(!baseAnalysis::Initialize()) return false;

  // Minimum accum cut level (how many cuts should be passed) to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("pionAnalysis.MinAccumLevelToSave"));

  // Define categories for color drawing
  anaUtils::AddStandardCategories();
  anaUtils::AddStandardCategories("beam");
  anaUtils::AddStandardCategories("dau");

  return true;
}

//********************************************************************
void pionAnalysis::DefineSelections(){
//********************************************************************

  /* In this method the user will add to the SelectionManager (accessed by  sel() ) the selections to be run, 
     defined in other files. In general an analysis has a single selection, which could have multiple branches. 
     Each of the branches is in fact a different selection (different cuts and actions), but defining them as branches 
     we ussualy gain in speed and simplicity since the steps that are common are applied only once. 
     Sometimes the user does not want to expend time merging two selections in a single one (as branches), but preffers to run 
     both independently. This is in general done for quick checks (are the selections mutually exclusive ?, etc). 
     This is possible in highland2. An example on how to do that is shown below. 
  */

  // Add selections to the SelectionManager provided:
  // - Name of the selection 
  // - Title, the one dumped by the DrawingTools::DumpSelections() method. It is an explaination of the selection
  // - Pointer to the selection. The argument in the constructor (false) indicates the 
  //   step sequence is not broken when a cut is not passed. In this way we can save intermediate information for events 
  //   not passing the entire selection


  sel().AddSelection("pionSelection",            "pion selection",     new pionSelection(false));     // true/false for forcing break  
  /*
  sel().AddSelection("stoppingProtonSelection",  "pion selection",     new stoppingProtonSelection(false));     // true/false for forcing break
  sel().AddSelection("stoppingKaonSelection",    "pion selection",     new stoppingKaonSelection(false));     // true/false for forcing break
  sel().AddSelection("stoppingMuonSelection",    "pion selection",     new stoppingMuonSelection(false));
  */
}

//********************************************************************
void pionAnalysis::DefineCorrections(){
//********************************************************************

  /* Corrections modify some aspect of the input data (real data or MC). 
     The entire analysis will proceed on the modified data
  */
  
  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();

  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledEdx"))             corr().AddCorrection("dEdx",              new dEdxCorrection());
  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledEdxData"))         corr().AddCorrection("dEdx data",         new dEdxDataCorrection());   
  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledBeamMom"))         corr().AddCorrection("BeamMom",           new BeamMomCorrection());
  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledBeamMomSmearing")) corr().AddCorrection("BeamMomSmearing",   new BeamMomSmearingCorrection());
  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledCryoWallBeamMom")) corr().AddCorrection("CryoWallBeamMom",   new CryoWallBeamMomCorrection());
  if (ND::params().GetParameterI("pionAnalysis.Corrections.EnabledBrokenTrackData")) corr().AddCorrection("broken track data", new BrokenTrackDataCorrection());   

}

//********************************************************************
void pionAnalysis::DefineSystematics(){
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();

  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void pionAnalysis::DefineConfigurations(){
//********************************************************************

  /*  A "configuration" is defined by the systematics that are enabled, the number of toys being run and the random seed 
      used to generate the toys. Each configuration has a micro-tree associated in the output file (with the same name)
  */

  
  // Some configurations are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

  // Enable all variation systematics in the all_syst configuration (created in baseAnalysis)
  if (_enableAllSystConfig){
    if (ND::params().GetParameterI("pionAnalysis.Variations.EnabledEdx"))        conf().EnableEventVariation(SystId::kdEdx,         all_syst);
  }

}

//********************************************************************
void pionAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  /*  We call Micro-trees to the standard analysis trees appearing in the output file. 
      There is always a Micro-Tree call "default" which should contain the basic info to understand our selection. 
      The user can add extra Micro-Trees by adding configurations to the analysis (see DefineConfigurations method above).

      Here we give an example of different variables that can be added. Have a look at highlandTools/vXrY/src/OutputManager.hxx
      to see all available methods.
  */

  
  // -------- Add variables to the analysis tree ----------------------

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);

  // is this a signal event ? 
  AddVarI(  output(), true_signal,       "is true signal ?");

  
  AddVar4VF(output(), beam_truepos,      "beam track true start position");
  AddVar4VF(output(), beam_trueendpos,   "beam track true end position");
  AddVar3VF(output(), beam_truedir,      "beam track true direction");
  AddVarF(  output(), beam_truemom,      "beam track true momentum");
  AddVarF(  output(), beam_truemom_tpc,  "beam track true momentum in TPC");
  AddVarI(  output(), beam_truepdg,      "beam track true pdg");
  AddVarI(  output(), beam_trueendproc,  "beam track true end process");

  // beam reco info
  
  AddVar3VF(output(), beam_endpos,           "beam track end position");
  AddVar3VF(output(), beam_enddir,           "beam track end direction");
  AddVarF(  output(), beam_mom,              "beam track momentum");
  AddVarF(  output(), beam_mom_tpc,          "beam track momentum after crossing cryowall");
  AddVarF(  output(), beam_mom_raw,          "beam track momentum without BI corrections");
  //AddVarF(  output(), beam_mom_tpc_raw,      "beam track momentum after crossing cryowall without BI corrections");

  AddVarI(  output(), beam_trigger,          "beam trigger");
  AddVarD(  output(), beam_tof,              "beam TOF");
  AddVarD(  output(), beam_time,             "beam track time");

  AddVarFixVI(output(),  beam_ckov_status,      "beam ckov status",  2);
  AddVarFixVD(output(),  beam_ckov_time,        "beam ckov time",    2);
  AddVarFixVD(output(),  beam_ckov_pressure,    "beam ckov pressure",2);
  
  // all reconstructed track variables
  AddVarMaxSizeVI(  output(), trk_truepdg,      "pdg code",               ntracks,700);
  AddVarMaxSizeVF(  output(), trk_truemom,      "true momentum",          ntracks,700);
  AddVarMaxSize4MF( output(), trk_truepos,      "true start position",    ntracks,700);
  AddVarMaxSizeVI(  output(), trk_trueendproc,  "final process",          ntracks,700);
  AddVarMaxSizeVI(  output(), trk_trueproc,     "initial process",        ntracks,700);
  AddVarMaxSize3MF( output(), trk_truedir,      "true start direction",   ntracks,700);

  AddVarMaxSizeVF(  output(), trk_length,       "length",                 ntracks,700);
  AddVarMaxSizeVF(  output(), trk_dedx,         "dedx",                   ntracks,700);
  AddVarMaxSizeVF(  output(), trk_mom_muon,     "range momentum (muon)",  ntracks,700);
  AddVarMaxSizeVF(  output(), trk_mom_prot,     "range momentum (proton)",ntracks,700);
  AddVarMaxSize4MF( output(), trk_pos,          "start position",         ntracks,700);
  AddVarMaxSize3MF( output(), trk_dir,          "start direction",        ntracks,700);
  AddVarMaxSizeVI(  output(), trk_ndau,         "#daughters",             ntracks,700);

  // --- candidate truth variables ---
  AddVarI(  output(), seltrk_truepdg,     "candidate true pdg code");
  AddVarF(  output(), seltrk_trueeff,     "candidate true-reco matching eff");
  AddVarF(  output(), seltrk_truepur,     "candidate true-reco matching pur");
  AddVarI(  output(), seltrk_trueproc,    "candidate true initial process");
  AddVarI(  output(), seltrk_trueendproc, "candidate true final process");
  AddVarF(  output(), seltrk_truemom,     "candidate true momentum");
  AddVarF(  output(), seltrk_truemom_tpc, "candidate true momentum at TPC entrance");
  AddVarF(  output(), seltrk_trueendmom,  "candidate true end momentum");
  AddVar4VF(output(), seltrk_truepos,     "candidate true position");         
  AddVar4VF(output(), seltrk_trueendpos,  "candidate true end position");      
  AddVar3VF(output(), seltrk_truedir,     "candidate true direction");         
  AddVarF(  output(), seltrk_truebeta,    "candidate true beta");
  AddVarMaxSizeVI(output(),  seltrk_dau_truepdg,     "daguhters length",           seltrk_ndau,100);
  AddVarMaxSize4MF(output(), seltrk_dau_truepos,     "daugthers true position",    seltrk_ndau,100);
  AddVarMaxSize4MF(output(), seltrk_dau_trueendpos,  "daugthers true position",    seltrk_ndau,100);
  AddVarMaxSizeVI(output(),  seltrk_dau_trueproc,    "daugthers true process",     seltrk_ndau,100);
  AddVarMaxSizeVI(output(),  seltrk_dau_trueendproc, "daughters true end process", seltrk_ndau,100);
  AddVarMaxSizeVF(output(),  seltrk_dau_truemom,     "daughters true momentum",    seltrk_ndau,100);
  AddVarMaxSizeVF(output(),  seltrk_dau_trueendmom,  "daughters true end momentum",seltrk_ndau,100);

  //  AddVarI(  output(), seltrk_ndau,        "#daughters");
  AddVarMaxSizeVF(output(),  seltrk_dau_mom,        "daughters momentum",     seltrk_ndau,100);
  AddVarMaxSize4MF(output(), seltrk_dau_pos,        "daughters position",     seltrk_ndau,100); 
  AddVarMaxSize3MF(output(), seltrk_dau_dir,        "daughters direction",    seltrk_ndau,100);
  AddVarMaxSize4MF(output(), seltrk_dau_endpos,     "daughters position",     seltrk_ndau,100); 
  AddVarMaxSize3MF(output(), seltrk_dau_enddir,     "daughters direction",    seltrk_ndau,100);
  AddVarMaxSizeVF( output(), seltrk_dau_length,     "daguhters length",       seltrk_ndau,100);
  AddVarMaxSizeVI( output(), seltrk_dau_nhits,      "daguhters #hits",        seltrk_ndau,100);
  AddVarMaxSizeVI( output(), seltrk_dau_nhits2,     "daguhters hits in plane 2",seltrk_ndau,100);

  AddVarMF(output(), seltrk_dau_hit_dedx,     "daughters dEdx per hit",       seltrk_ndau,-100,NMAXHITSPERPLANE);
  AddVarMF(output(), seltrk_dau_hit_dqdx_raw, "daughters raw dQdx per hit",   seltrk_ndau,-100,NMAXHITSPERPLANE);
  AddVarMF(output(), seltrk_dau_hit_resrange, "daughters residual range",     seltrk_ndau,-100,NMAXHITSPERPLANE);
  AddVarMF(output(), seltrk_dau_dedx_binned,  "daughters dEdx binned",        seltrk_ndau,-100,4);

  // --- candidate recon variables ---
  AddVarI(  output(), seltrk_broken,     "candidate was broken");
  AddVar3VF(output(), seltrk_dir,        "candidate reconstructed direction");
  AddVar4VF(output(), seltrk_pos,        "candidate reconstructed position");
  AddVar4VF(output(), seltrk_endpos,     "candidate reconstructed end position");
  AddVar3VF(output(), seltrk_enddir,     "candidate reconstructed end direction");
  AddVarF(  output(), seltrk_costheta,   "candidate reconstructed cos(theta)");
  AddVarI(  output(), seltrk_nhits,      "candidate number of hits");
  AddVarF(  output(), seltrk_length,     "candidate length");
  AddVarF(  output(), seltrk_dedx_raw,   "candidate average dEdx");
  AddVarF(  output(), seltrk_kinetic,    "kinetic energy");
  AddVarFixMF(output(), seltrk_hit_x,          "x per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_y,          "y per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_z,          "z per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dedx_raw,   "dEdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dedx,       "calibrated dEdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dedx_cor,   "LAr calibrated dEdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dqdx_raw,   "dQdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dqdx,       "calibrated dQdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_dqdx_cor,   "LAr calibrated dQdx per hit",3,NMAXHITSPERPLANE);
  AddVarFixMF(output(), seltrk_hit_resrange,   "Residual Range of the candidate",3,NMAXHITSPERPLANE);
  AddVarFixVI(output(), seltrk_nhitsperplane,  "candidate number of hits per plane",3);



  //  AddVarFixVF( output(), seltrk_dedx_exp,"candidate expected dEdx for different hypothesis",4);

  AddVarF(output(), seltrk_mom_muon,      "candidate reconstructed momentum (muon)");
  AddVarF(output(), seltrk_mom_prot,      "candidate reconstructed momentum (proton)");

  AddVar3VF(output(),seltrk_pos_z0,        "candidate start position extrapolated at z=0");
  AddVarF(output(), seltrk_length_z0,     "candidate length assuming the particle starts at z=0");
  AddVarF(output(), seltrk_mom_muon_z0,   "candidate reconstructed momentum at z=0 (muon)");
  AddVarF(output(), seltrk_mom_prot_z0,   "candidate reconstructed momentum at z=0 (proton)");
  
  AddVarF(output(), seltrk_csdarange_muon,          "candidate reconstructed csdarange (muon) from beam mom");
  AddVarF(output(), seltrk_csdarange_prot,          "candidate reconstructed csdarange (proton) from beam mom");
  AddVarF(output(), seltrk_csdarange_muon_raw,      "candidate reconstructed csdarange (muon) from beam mom with no BI correction");
  AddVarF(output(), seltrk_csdarange_prot_raw,      "candidate reconstructed csdarange (proton) from beam mom with no BI correction");
  AddVarF(output(), seltrk_csdarange_tpc_muon,      "candidate reconstructed csdarange (muon) from cryo mom");
  AddVarF(output(), seltrk_csdarange_tpc_prot,      "candidate reconstructed csdarange (proton) from cryo mom");
  
  AddVarF(output(), seltrk_theta,    "candidate reconstructed polar angle");

  AddVarF(output(), seltrk_dqdx,     "candidate average dQdx");
  AddVarF(output(), seltrk_dedx,     "candidate average dEdx");

  AddVarFixMF(output(), seltrk_dedx_binned,     "candidate average dEdx for several resrange bins",3,4);
  
  AddVarFixMF(output(), seltrk_pid,  "candidate PID variables", 3,8);
  AddVarFixMF(output(), seltrk_calo, "candidate CALO variables",3,5); 

  AddVarF(   output(), seltrk_pida_raw,  "candidate PIDA (raw)");
  AddVarF(   output(), seltrk_pida_corr, "candidate PIDA (corrected)");
  AddToyVarF(output(), seltrk_pida,      "candidate PIDA (variated)");

  AddVarFixVF(  output(), seltrk_pida2,     "pida2", 3);
  AddVarFixVI(  output(), seltrk_pdg,       "recon pdg", 3);

  //new variables added for reproducing Jake/Fraceska plots
  AddVar3VF(       output(), seltrk_CNNscore,        "candidate reconstructed CNN score");
  AddVarMaxSize3MF(output(), seltrk_dau_CNNscore,    "candidate daughters reconstructed CNN score",seltrk_ndau,100);
  AddVarMaxSizeVF( output(), seltrk_dau_chi2_prot,   "candidate daughters chi2 proton",            seltrk_ndau,100);
  AddVarMaxSizeVF( output(), seltrk_dau_chi2_ndf,    "candidate daughters chi2 ndf",               seltrk_ndau,100);
  AddVarMaxSizeVF( output(), seltrk_dau_vtxdistance, "candidate daguhters distance to vtx",        seltrk_ndau,100);
}

//********************************************************************
void pionAnalysis::DefineTruthTree(){
//********************************************************************

  /*  The "truth" tree also appears in the output file. It contains all interactions in which we are interested in regardless on whether 
      the selection was passed or not. This is the tree that should be used to compute signal efficiencies
  */

  
  // Variables from baseAnalysis (run, event, ...)
  baseAnalysis::DefineTruthTree();

  AddVarI(  output(), true_signal,      "is true signal ?");
  AddVarI(  output(), ntracks,          "number of reconstructed tracks");
  
  //--- initial beam particle true variables -------
  AddVarI(  output(), truetrk_pdg,      "beam particle pdg code");
  AddVarI(  output(), truetrk_endproc,  "beam particle end process");
  AddVarF(  output(), truetrk_mom,      "beam particle true momentum");
  AddVarF(  output(), truetrk_costheta, "beam particle true cos(theta)");
  AddVar4VF(output(), truetrk_pos,      "beam particle true position");
  AddVar4VF(output(), truetrk_endpos,   "beam particle true end position");
  AddVar3VF(output(), truetrk_dir,      "beam particle true direction");
}

//********************************************************************
void pionAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-independent variables (all except the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called once all toys has been run, what means that the value of all variables for the last toy will be saved. This is not a problem 
      for variables that are not expected to change from a toy to another.
  */

  
  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);

  // The true signal for protoDUNE requires the initial beam particle to reach the active volume
  if (GetEvent().nTrueParticles>0){
    if (GetEvent().TrueParticles[0]->PositionEnd[2]>0)     output().FillVar(true_signal, 1);
    else output().FillVar(true_signal, 0);
  }

  // Get all reconstructed tracks in the event
  AnaParticleB** tracks = GetEvent().Particles;
  Int_t nTracks         = GetEvent().nParticles;


  AnaBeam* beam         = static_cast<AnaBeam*>(GetSpill().Beam);
  AnaParticleMomB* beamPart = beam->BeamParticle;
  
  if (beamPart){
    if (beamPart->TrueObject){

      // TODO, since the beam particle was not completly filled in LArSoftTreeConverter we get the info from the
      // equivalent true particle in the TrueParticle vector
      //      AnaTrueParticle* beamTruePart= pionSelUtils::FindBeamTrueParticle(GetSpill());
      AnaTrueParticle* beamTruePart= static_cast<AnaTrueParticle*>(beamPart->TrueObject);

      //      output().FillVar(beam_truelengthInTPC,                 beamTruePart->Length);
      output().FillVar(beam_truemom,                    beamTruePart->Momentum);
      output().FillVar(beam_truemom_tpc,                beamTruePart->MomentumInTPC);
      output().FillVar(beam_truepdg,                    beamTruePart->PDG);
      output().FillVar(beam_trueendproc,                beamTruePart->ProcessEnd);
      output().FillVectorVarFromArray(beam_truedir,     beamTruePart->Direction,   3);
      output().FillVectorVarFromArray(beam_truepos,     beamTruePart->Position, 4);
      output().FillVectorVarFromArray(beam_trueendpos,  beamTruePart->PositionEnd, 4);
    }

    output().FillVectorVarFromArray(beam_endpos,         beamPart->PositionEnd,3);
    output().FillVectorVarFromArray(beam_enddir,         beamPart->DirectionEnd,3);
    output().FillVar(               beam_mom,            beamPart->Momentum);   
    output().FillVar(               beam_mom_tpc,        (Float_t)beam->BeamMomentumInTPC);

    if (beamPart->TrueObject)output().FillVar(               beam_mom_raw,        beamPart->Momentum);   
    else                     output().FillVar(               beam_mom_raw,        static_cast<const AnaParticle*>(beamPart->Original->Original)->Momentum);   
    
    //if (beamPart->TrueObject)output().FillVar(               beam_mom_tpc_raw,    beam_mom_tpc);   
    //else;
  }

  output().FillVar(beam_tof,     beam->TOF);
  output().FillVar(beam_trigger, beam->BeamTrigger);
  output().FillVar(beam_time,    beam->BeamTrackTime);

  output().FillVectorVarFromArray(beam_ckov_pressure,       beam->CerenkovPressure,2);
  output().FillVectorVarFromArray(beam_ckov_status,         beam->CerenkovStatus,  2);
  output().FillVectorVarFromArray(beam_ckov_time,           beam->CerenkovTime,    2);
  
  
  // ---------- Save information about all (max 700) recon tracks in the event --------------
  for (Int_t i=0;i<std::min((Int_t)700,nTracks); ++i){    
    AnaParticle* track = static_cast<AnaParticle*>(tracks[i]);
    output().FillVectorVar(trk_mom_muon,      track->RangeMomentum[0]);
    output().FillVectorVar(trk_mom_prot,      track->RangeMomentum[1]);
    output().FillVectorVar(trk_dedx,          track->AveragedEdx);
    output().FillVectorVar(trk_length,        track->Length);
    output().FillMatrixVarFromArray(trk_pos,  track->PositionStart,4);
    output().FillMatrixVarFromArray(trk_dir,  track->DirectionStart,3);
    output().FillVectorVar(trk_ndau,          (Int_t)track->Daughters.size());
    if (track->TrueObject){
      output().FillMatrixVarFromArray(trk_truepos,    track->GetTrueParticle()->Position,4);
      output().FillMatrixVarFromArray(trk_truedir,    track->GetTrueParticle()->Direction,3);
      output().FillVectorVar(trk_truemom,             track->GetTrueParticle()->Momentum);
      output().FillVectorVar(trk_truepdg,             track->GetTrueParticle()->PDG);
      output().FillVectorVar(trk_trueproc,     (Int_t)track->GetTrueParticle()->ProcessStart);
      output().FillVectorVar(trk_trueendproc,  (Int_t)track->GetTrueParticle()->ProcessEnd);

    }
    output().IncrementCounter(ntracks);
  }
  
  //---------------- candidate variables ------------------------------
  if (box().MainTrack){

    // This is the PIDA computed from the hits, and not the one precomputed in the Analysis Tree
    if (box().MainTrack->Original){
      if (box().MainTrack->Original->Original){
        output().FillVar(seltrk_pida_corr,  pionSelUtils::ComputePIDA(*static_cast<const AnaParticle*>(box().MainTrack->Original->Original)));      
      }
      if (box().MainTrack->Original->Original->Original){
        output().FillVar(seltrk_pida_raw,  pionSelUtils::ComputePIDA(*static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)));
      }
    }
    
    Int_t ndau = (Int_t)box().MainTrack->Daughters.size();
    std::vector<AnaRecObjectC*>& daughters = box().MainTrack->Daughters;
    
    std::vector<double> distance = pionSelUtils::ComputeDistanceToVertex(box().MainTrack);

    for (Int_t i=0;i<std::min((Int_t)100,ndau); ++i){
      
      AnaParticle* dau = static_cast<AnaParticle*>(daughters[i]);

      output().FillVectorVar(seltrk_dau_mom,             dau->RangeMomentum[0]);
      output().FillMatrixVarFromArray(seltrk_dau_pos,    dau->PositionStart,4);
      output().FillMatrixVarFromArray(seltrk_dau_dir,    dau->DirectionStart,3); 
      output().FillMatrixVarFromArray(seltrk_dau_endpos, dau->PositionEnd,4);
      output().FillMatrixVarFromArray(seltrk_dau_enddir, dau->DirectionEnd,3); 
      output().FillVectorVar(seltrk_dau_length,          dau->Length);
      output().FillVectorVar(seltrk_dau_nhits2,          dau->NHitsPerPlane[2] );
      output().FillVectorVar(seltrk_dau_nhits,           dau->NHits);

      
      output().FillMatrixVarFromArray(seltrk_dau_hit_dedx,      dau->dEdx[2],          NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_dau_hit_dqdx_raw,  static_cast<const AnaParticle*>(dau->Original->Original->Original)->dQdx[i],   NMAXHITSPERPLANE);  
      output().FillMatrixVarFromArray(seltrk_dau_hit_resrange,  dau->ResidualRange[2], NMAXHITSPERPLANE);

      output().FillMatrixVarFromArray(seltrk_dau_CNNscore,      dau->CNNscore,         3); 
      output().FillVectorVar(seltrk_dau_chi2_prot,          dau->Chi2Proton);
      output().FillVectorVar(seltrk_dau_chi2_ndf,           dau->Chi2ndf);
      output().FillVectorVar(seltrk_dau_vtxdistance,(Float_t)distance[i]);

      Float_t *dau_binned_dedx[3];
      for(int i = 0; i < 3; i++)
        dau_binned_dedx[i] = new Float_t[4];
      
      pionSelUtils::ComputeBinnedDeDx(dau,20,4,dau_binned_dedx);
      output().FillMatrixVarFromArray(seltrk_dau_dedx_binned,  dau_binned_dedx[2], 4);

      for(int i = 0; i < 3; i++)
        delete dau_binned_dedx[i];

      
      if(  daughters[i]->TrueObject ) {
       	output().FillVectorVar(seltrk_dau_truepdg,             dau->GetTrueParticle()->PDG);
        output().FillVectorVar(seltrk_dau_trueproc,     (Int_t)dau->GetTrueParticle()->ProcessStart);
        output().FillMatrixVarFromArray(seltrk_dau_truepos,    dau->GetTrueParticle()->Position,4);
        output().FillVectorVar(seltrk_dau_trueendproc,  (Int_t)dau->GetTrueParticle()->ProcessEnd);
        output().FillMatrixVarFromArray(seltrk_dau_trueendpos, dau->GetTrueParticle()->PositionEnd,4);
        output().FillVectorVar(seltrk_dau_truemom,             dau->GetTrueParticle()->Momentum);
        output().FillVectorVar(seltrk_dau_trueendmom,          dau->GetTrueParticle()->MomentumEnd);
      }
      output().IncrementCounter(seltrk_ndau);
    }

    output().FillVectorVarFromArray(seltrk_pida2,  box().MainTrack->PIDA,3);
    output().FillVectorVarFromArray(seltrk_pdg,    box().MainTrack->ReconPDG,3);

    output().FillVar(seltrk_mom_muon,          box().MainTrack->RangeMomentum[0]);
    output().FillVar(seltrk_mom_prot,          box().MainTrack->RangeMomentum[1]);

    if(beamPart){
      //Float_t mom = beam->BeamMomentumInTPC;
      //if (beamPart->TrueObject) mom =  static_cast<AnaTrueParticle*>(beamPart->TrueObject)->Momentum;
      output().FillVar(seltrk_csdarange_muon,         pionSelUtils::ComputeCSDARange(1000*(beamPart->Momentum), 13));
      output().FillVar(seltrk_csdarange_prot,         pionSelUtils::ComputeCSDARange(1000*(beamPart->Momentum), 2212));
      if (beamPart->TrueObject){
        output().FillVar(seltrk_csdarange_muon_raw,     pionSelUtils::ComputeCSDARange(1000*(static_cast<AnaTrueParticle*>(beamPart->TrueObject)->Momentum), 13));
        output().FillVar(seltrk_csdarange_prot_raw,     pionSelUtils::ComputeCSDARange(1000*(static_cast<AnaTrueParticle*>(beamPart->TrueObject)->Momentum), 2212));
      }
      else{
        output().FillVar(seltrk_csdarange_muon_raw,     pionSelUtils::ComputeCSDARange(1000*(static_cast<const AnaParticle*>(beamPart->Original->Original)->Momentum), 13));
        output().FillVar(seltrk_csdarange_prot_raw,     pionSelUtils::ComputeCSDARange(1000*(static_cast<const AnaParticle*>(beamPart->Original->Original)->Momentum), 2212));
      }
      output().FillVar(seltrk_csdarange_tpc_muon,     pionSelUtils::ComputeCSDARange(1000*((Float_t)beam->BeamMomentumInTPC), 13));
      output().FillVar(seltrk_csdarange_tpc_prot,     pionSelUtils::ComputeCSDARange(1000*((Float_t)beam->BeamMomentumInTPC), 2212));
    }


    //    output().FillVar(seltrk_mom_muon2,         pionSelUtils::ComputeRangeMomentum(box().MainTrack->Length/10., 13));
    //    output().FillVar(seltrk_mom_prot2,         pionSelUtils::ComputeRangeMomentum(box().MainTrack->Length/10., 2212));
    
    output().FillVar(seltrk_dedx,              box().MainTrack->AveragedEdx);
    output().FillVar(seltrk_dqdx,              box().MainTrack->AveragedQdx);

    output().FillVar(seltrk_broken,                (Int_t)(box().MainTrack->NDOF==8888));
    
    output().FillVar(seltrk_dedx_raw,              static_cast<const AnaParticle*>(box().MainTrack->Original)->AveragedEdx);
    output().FillVar(seltrk_nhits,                 box().MainTrack->NHits);
    output().FillVar(seltrk_length,                box().MainTrack->Length);
    output().FillVar(seltrk_costheta,              box().MainTrack->DirectionStart[2]);
    output().FillVectorVarFromArray(seltrk_pos,    box().MainTrack->PositionStart, 4);
    output().FillVectorVarFromArray(seltrk_endpos, box().MainTrack->PositionEnd, 4);
    output().FillVectorVarFromArray(seltrk_dir,    box().MainTrack->DirectionStart, 3);
    output().FillVectorVarFromArray(seltrk_enddir, box().MainTrack->DirectionEnd, 3);

    Float_t posz[3]={0};
    output().FillVectorVarFromArray(seltrk_pos_z0, pionSelUtils::ExtrapolateToZ(box().MainTrack,0,posz), 3); //default extrapolates to 0

    Float_t diflength = sqrt(pow(posz[0]-box().MainTrack->PositionStart[0],2) + pow(posz[1]-box().MainTrack->PositionStart[1],2) + pow(posz[2]-box().MainTrack->PositionStart[2],2));
    output().FillVar(seltrk_length_z0,             box().MainTrack->Length + diflength);

    //    output().FillVar(seltrk_mom_muon_z0,           pionSelUtils::ComputeRangeMomentum(box().MainTrack->Length + diflength, 13));
    //    output().FillVar(seltrk_mom_prot_z0,           pionSelUtils::ComputeRangeMomentum(box().MainTrack->Length + diflength, 2212));

    output().FillVar(seltrk_kinetic, pionSelUtils::ComputeKineticEnergy(*box().MainTrack));              

    for (int i=0;i<3;i++){
      output().FillMatrixVarFromArray(seltrk_hit_x,         static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)->HitX[i], i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_y,         static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)->HitY[i], i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_z,         static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)->HitZ[i], i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dedx_raw,  static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)->dEdx[i], i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dedx,      static_cast<const AnaParticle*>(box().MainTrack)->dEdx[i],                               i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dedx_cor,  static_cast<const AnaParticle*>(box().MainTrack)->dEdx_corr[i],                               i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dqdx_raw,  static_cast<const AnaParticle*>(box().MainTrack->Original->Original->Original)->dQdx[i], i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dqdx,      static_cast<const AnaParticle*>(box().MainTrack)->dQdx[i],                               i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_dqdx_cor,  static_cast<const AnaParticle*>(box().MainTrack)->dQdx_corr[i],                               i, NMAXHITSPERPLANE);
      output().FillMatrixVarFromArray(seltrk_hit_resrange,  static_cast<const AnaParticle*>(box().MainTrack)->ResidualRange[i],                      i, NMAXHITSPERPLANE);
      output().FillVectorVarFromArray(seltrk_nhitsperplane, static_cast<const AnaParticle*>(box().MainTrack)->NHitsPerPlane,3);
    }


    Float_t *binned_dedx[3];
    for(int i = 0; i < 3; i++)
      binned_dedx[i] = new Float_t[4];

    pionSelUtils::ComputeBinnedDeDx(static_cast<const AnaParticle*>(box().MainTrack),20,4,binned_dedx);
    for (int i=0;i<3;i++){
      output().FillMatrixVarFromArray(seltrk_pid,    static_cast<const AnaParticle*>(box().MainTrack)->PID[i],  i, 8);
      output().FillMatrixVarFromArray(seltrk_calo,   static_cast<const AnaParticle*>(box().MainTrack)->CALO[i], i, 5);      
      output().FillMatrixVarFromArray(seltrk_dedx_binned,  binned_dedx[i] , i, 4);
    }
    
    for(int i = 0; i < 3; i++)
      delete binned_dedx[i];

    //Jake/Franceska variables
    output().FillVectorVarFromArray(seltrk_CNNscore,       box().MainTrack->CNNscore,3);
           
    // Properties of the true particle associated to the candidate
    if (box().MainTrack->GetTrueParticle()){

      output().FillVar(seltrk_truepdg,                  box().MainTrack->GetTrueParticle()->PDG);
      output().FillVar(seltrk_truemom,                  box().MainTrack->GetTrueParticle()->Momentum);
      output().FillVar(seltrk_truemom_tpc,              static_cast<const AnaTrueParticle*>(box().MainTrack->GetTrueParticle())->MomentumInTPC);
      output().FillVar(seltrk_trueendmom,               box().MainTrack->GetTrueParticle()->MomentumEnd);

      output().FillVectorVarFromArray(seltrk_truepos,   box().MainTrack->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(seltrk_trueendpos,box().MainTrack->GetTrueParticle()->PositionEnd, 4);
      output().FillVectorVarFromArray(seltrk_truedir,   box().MainTrack->GetTrueParticle()->Direction, 3);

      output().FillVar(seltrk_trueproc,          (Int_t)box().MainTrack->GetTrueParticle()->ProcessStart);
      output().FillVar(seltrk_trueendproc,       (Int_t)box().MainTrack->GetTrueParticle()->ProcessEnd);

      output().FillVar(seltrk_truepur,           box().MainTrack->TruePur);
      output().FillVar(seltrk_trueeff,           box().MainTrack->TrueEff);
      
      if (anaUtils::GetParticleMass(ParticleId::GetParticle(box().MainTrack->GetTrueParticle()->PDG))>0)
        output().FillVar(seltrk_truebeta,          anaUtils::ComputeBetaGamma(box().MainTrack->GetTrueParticle()->Momentum, ParticleId::GetParticle(box().MainTrack->GetTrueParticle()->PDG)));
    }
  }

}

//********************************************************************
void pionAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-dependent variables (the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called at the end of each toy.

      There could be many variables that are toy-dependent. We don't need to save all of them as toy variables, but only the ones we are interested in plotting 
      for different toys. 

      TOY VARIABLES ARE VERY SPACE CONSUMING SO WE SHOULD MINIMISE ITS NUMBER !!!!
  */


  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

  // PIDA has to be a toy variable, since there is a systematic variation associated to it
  if (box().MainTrack){
    output().FillToyVar(seltrk_pida,  pionSelUtils::ComputePIDA(*box().MainTrack));
  }
  
}
//********************************************************************
bool pionAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  /* To avoid unecessary events in the "truth" tree in this method we define the condition to include or not a given 
     true vertex in the tree. 
  */

  (void) vtx; // to avoid warning for unused vtx variable
  
  // fill it allways
  return true;
}

//********************************************************************
void pionAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx);


  AnaTrueParticle* beampart= pionSelUtils::FindBeamTrueParticle(GetSpill());

  // The true signal for protoDUNE requires the initial beam particle to reach the active volume
  output().FillVar(true_signal, 0);
  
  if (beampart){
    if (beampart->PositionEnd[2]>0) output().FillVar(true_signal, 1);

    output().FillVar(               truetrk_pdg,      beampart->PDG);
    output().FillVar(               truetrk_endproc,  beampart->ProcessEnd);
    output().FillVar(               truetrk_costheta, beampart->Direction[2]);
    output().FillVar(               truetrk_mom,      beampart->Momentum);
    output().FillVectorVarFromArray(truetrk_dir,      beampart->Direction,3);
    output().FillVectorVarFromArray(truetrk_pos,      beampart->Position, 4);
    output().FillVectorVarFromArray(truetrk_endpos,   beampart->PositionEnd, 4);

  }
  
  // Initial beam particle true variables. Take the only true particle in the true vertex
  /*
  if (vtx.nTrueParticles>0){
    AnaTrueParticle* truePart = static_cast<AnaTrueParticle*>(vtx.TrueParticles[0]);
    
    output().FillVar(               truetrk_costheta, truePart->Direction[2]);
    output().FillVar(               truetrk_mom,      truePart->Momentum);
    output().FillVectorVarFromArray(truetrk_dir,      truePart->Direction,3);
    output().FillVectorVarFromArray(truetrk_pos,      truePart->Position, 4);
    output().FillVectorVarFromArray(truetrk_endpos,   truePart->PositionEnd, 4);
  }
  */
  // Number of reconstructed tracks
  output().FillVar(ntracks,  (Int_t)static_cast<AnaBunchB*>(GetSpill().Bunches[0])->Particles.size());  
}

//********************************************************************
void pionAnalysis::FillCategories(){
//********************************************************************

  /* This method fills the micro-tree variables related with track categories for color drawing. 
     Those variables are added automatically (no need to explicitely add them in DefineMicroTrees) to the 
     micro-trees, but must be filled by the analyser, provided the event and the relevant track 

     If this method is not implemented, the one from the base class (baseAnalysis::FillCategories()) will be called.      
  */

  
  // For the candidate
  if (box().MainTrack){
    anaUtils::FillCategories(&GetEvent(), box().MainTrack,"");

    // Categories for first daugther of the candidate
    if (box().MainTrack->Daughters.size()>0) anaUtils::FillCategories(&GetEvent(), static_cast<AnaParticleB*>(box().MainTrack->Daughters[0]),"dau");
  }
  
  // For the beam track
  AnaParticleB* beam = static_cast<AnaBeam*>(GetSpill().Beam)->BeamParticle;
  if (beam) anaUtils::FillCategories(&GetEvent(), beam,"beam");
}
