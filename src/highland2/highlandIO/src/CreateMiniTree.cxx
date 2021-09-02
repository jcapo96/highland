#include "CreateMiniTree.hxx"
#include "InputManager.hxx"
//#include "FlatTreeConverter.hxx"
#include "HighlandMiniTreeConverter.hxx"
#include "Parameters.hxx"
//#include "Versioning.hxx"
//#include "GeometryManager.hxx"
#include "HighlandAnalysisUtils.hxx"

#include "CorrId.hxx"

// Corrections
//#include "MomRangeCorrection.hxx"

Int_t NVtx;
TClonesArray* Vtx;
Int_t RunID;    
Int_t SubrunID;    
Int_t EventID;    
bool  Preselected;

//********************************************************************
CreateMiniTree::CreateMiniTree(int argc, char *argv[]):SimpleLoopBase(argc, argv){
  //********************************************************************

  // Add the different productions
  //  ND::versioning().AddProduction(ProdId::MCC5,     "MCC5",     "v0r0",  "v1r0");

  // Add package versions to be saved in config tree
  /*
  ND::versioning().AddPackage("psycheCore",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHECOREROOT")));
  ND::versioning().AddPackage("psycheUtils",         anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEUTILSROOT")));
  ND::versioning().AddPackage("highlandTools",       anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDTOOLSROOT")));
  ND::versioning().AddPackage("highlandCorrections", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCORRECTIONSROOT")));
  ND::versioning().AddPackage("highlandIO",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDIOROOT")));
  */
  
  // add the different converters
  //  input().AddConverter("FlatTree",       new FlatTreeConverter(true));
  input().AddConverter("MiniTree",       new HighlandMiniTreeConverter("MiniTree",true));

}
//********************************************************************
bool CreateMiniTree::Initialize(){
  //********************************************************************
  // save RooTrackerVtx tree
  _saveGeometry = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveGeometry");

  _saveSubdet2Info     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveSubdet2Info");
  _saveSubdet1Info     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveSubdet1Info");


  _useSubdet2_1     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_1");
  _useSubdet2_2     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_2");
  _useSubdet2_3     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_3");
  _useSubdet1_1     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet1_1");
  _useSubdet1_2     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet1_2");

  _useSubdet2_1outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_1outOfBunch");
  _useSubdet2_2outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_2outOfBunch");
  _useSubdet2_3outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet2_3outOfBunch");
  _useSubdet1_1outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet1_1outOfBunch");
  _useSubdet1_2outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSubdet1_2outOfBunch");

  _saveTrueNuNC       = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NuNC");
  _saveTrueAntiNuNC   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNuNC");
  _saveTrueNumuCC     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NumuCC");
  _saveTrueAntiNumuCC = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNumuCC");
  _saveTrueNueCC      = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NueCC");
  _saveTrueAntiNueCC  = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNueCC");


  _trackStartPreselection     = (bool)ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.Preselection");

  _trackStartMinX     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MinX");
  _trackStartMaxX     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MaxX");
  _trackStartMinY     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MinY");
  _trackStartMaxY     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MaxY");
  _trackStartMinZ     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MinZ");
  _trackStartMaxZ     = ND::params().GetParameterD("highlandIO.MiniTree.TrackStart.MaxZ");

  _beamTOFPreselection     = (bool)ND::params().GetParameterD("highlandIO.MiniTree.Beam.TOFPreselection");
 
  _beamMinTOF     = ND::params().GetParameterD("highlandIO.MiniTree.Beam.MinTOF");
  _beamMaxTOF     = ND::params().GetParameterD("highlandIO.MiniTree.Beam.MaxTOF");


  _trueWithRecoPreselection          = (bool)ND::params().GetParameterD("highlandIO.MiniTree.Truth.WithRecoPreselection");
  _trueWithRecoDaughtersPreselection = (bool)ND::params().GetParameterD("highlandIO.MiniTree.Truth.IncludeTrueDaughters");

  
  
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.Subdet2_1"))    _saveTrueVertexInDet.push_back(SubDetId::kSubdet2_1);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.Subdet2_2"))    _saveTrueVertexInDet.push_back(SubDetId::kSubdet2_2);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.Subdet2_3"))    _saveTrueVertexInDet.push_back(SubDetId::kSubdet2_3);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.Subdet1_1"))    _saveTrueVertexInDet.push_back(SubDetId::kSubdet1_1);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.Subdet1_2"))    _saveTrueVertexInDet.push_back(SubDetId::kSubdet1_2);


  _currentGeomID=999;


  // Check software version compatibility between nd280AnalysisTools and current file
  /*
  if (_versionCheck){
    if(!ND::versioning().CheckVersionCompatibility(ND::versioning().GetProduction(input().GetSoftwareVersion()),
                                                   anaUtils::GetProductionIdFromReader())) return false;
  }
  */
  // Dump the production used for corrections, bunching, systematics, etc
  //  versionUtils::DumpProductions();


  // Enable any corrections that have to be done while the mini tree is being created.
  
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableMomRangeCorrection")){
    //    _corrections.AddCorrection(CorrId::kMomRangeCorr, "momrange_corr", new MomRangeCorrection());
  }

  //  _file->mkdir("geom");

  return true;
}

//********************************************************************
void CreateMiniTree::DefineOutputTree(){
  //********************************************************************

  // Add a new tree called "MiniTree"
  AddTreeWithName(minitree,"MiniTree");

  // Add a tree to save the truth info as a clone of the TruthDir/NRooTrackerVtx tree.
  // oaAnalysis tree converter has already dealt with the NRooTrackerVtx bit for us.

  std::string rooTreeName="";
  if(input().GetChain("NRooTrackerVtx"))
    rooTreeName = "NRooTrackerVtx";
  else if (input().GetChain("GRooTrackerVtx"))
    rooTreeName = "GRooTrackerVtx";

  if(_saveRoo && rooTreeName!=""){
    if (input().InputIsOriginalTree() && _filterRoo){
      AddTreeWithName(RooTrackerVtx, rooTreeName);
              
      Vtx = new TClonesArray(("ND::"+rooTreeName).c_str(),100);

      GetTree(RooTrackerVtx)->Branch("RunID",       &RunID,       "RunID/I",       32000);
      GetTree(RooTrackerVtx)->Branch("EventID",     &EventID,     "EventID/I",     32000);
      GetTree(RooTrackerVtx)->Branch("Preselected", &Preselected, "Preselected/B", 32000);
      GetTree(RooTrackerVtx)->Branch("SubrunID",    &SubrunID,    "SubrunID/I",    32000);

      GetTree(RooTrackerVtx)->Branch("NVtx", &NVtx, "NVtx/I", 32000);
      GetTree(RooTrackerVtx)->Branch("Vtx","TClonesArray", &Vtx   , 32000, 1);
    }
    else
      AddTreeWithName(RooTrackerVtx, rooTreeName,input().GetChain(rooTreeName));
  }

  SetCurrentTree(minitree);
  SetFillSingleTree(GetCurrentTree());

  // The full AnaSpill. _spill cannot be undefined here, otherwise it give problems in some platforms 
  _spill=NULL; 
  GetTree(minitree)->Branch("Spill", "AnaSpill", &_spill,64000,1);

  _POTSincePreviousSavedSpill   =0;
  _SpillsSincePreviousSavedSpill=0;
}


//********************************************************************
bool CreateMiniTree::Process(){
//********************************************************************

  // Set the tree to fill
  SetFillSingleTree(minitree);
  SetCurrentTree(minitree);
  
  // Get the corrected spill and set the branch address
  _spill = static_cast<AnaSpill*>(&input().GetCorrectedSpill());
  GetTree(minitree)->SetBranchAddress("Spill",&_spill );  

  // The number of POT and Spills since the last saved Spill
  _POTSincePreviousSavedSpill    += _spill->Beam->POTSincePreviousSavedSpill; 
  _SpillsSincePreviousSavedSpill += _spill->Beam->SpillsSincePreviousSavedSpill; 

  // Delete the uninteresting bunches
  //  DeleteUninterestingBunches();

  // Delete all particles we are not interested in
  DeleteUninterestingParticles();

  // Delete all true particles we are not interested in
  DeleteUninterestingTrueParticles();
  
  
  _lastSpillSaved = false;


  // Spill level preselection
  if (!SpillLevelPreselection()) return true;

  // The spill is saved when there are bunches or when there are true signal vertices on it (for eff calculation)
  if (_spill->Bunches.size()>0 || CheckTruthFillMiniTree(*_spill)){

    // Write the geometry when it changes
    if (_saveGeometry) WriteGeometry();

    // Fill the RooTrackerVtx tree 
    // Must be called before FillMiniTree since when the RooTrackerVtx filtering is applied the RooVtxIndex in true vertices changes
    FillRooTrackerVtxTree();
    
    // Fill the minitree
    FillMiniTree();

    // Mark this spill as saved
    _lastSpillSaved=true;
  }

  return true;
}

//********************************************************************
void CreateMiniTree::Finalize(){
//********************************************************************
  // Save the last spill (if not save yet) such that we keep track of the total POT since the last saved entry.

  if (_lastSpillSaved) return;

  // Fill the RooTrackerVtx tree
    // Must be called before FillMiniTree since when the RooTrackerVtx filtering is applied the RooVtxIndex in true vertices changes
  FillRooTrackerVtxTree();

  // Fill the minitree
  FillMiniTree(); 

  // Delete Spills for the last entry
  input().DeleteSpill();  
}

//********************************************************************
bool CreateMiniTree::CheckTruthFillMiniTree(const AnaSpill& spill){
//********************************************************************

  bool ok_det = false;
  bool ok_reac = false;

  const std::vector<AnaTrueVertexB*>& vertices = spill.TrueVertices;
  for (std::vector<AnaTrueVertexB*>::const_iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *static_cast<AnaTrueVertex*>(*it);

    // Check the reaction code
    ok_reac = CheckTrueVertexReaction(vtx);

    // Check the detector only if the reaction is OK
    if (ok_reac)
      ok_det = CheckTrueVertexDetector(vtx.Detector);

    // return true when it finds a tru vertex fulfilling both conditions
    if (ok_reac && ok_det) return true;
  }

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckTrueVertexReaction(const AnaTrueVertex& vtx){
//********************************************************************

  // NuWro prod5 uses 70 for 2p2h code (Neut does not have 70 at all, and uses 2 instead)
  // NuWro prod6 uses 2 as NEUT
  if (_saveTrueNuNC       && vtx.ReacCode>=+30 && abs(vtx.ReacCode)!=70) return true;
  if (_saveTrueAntiNuNC   && vtx.ReacCode<=-30 && abs(vtx.ReacCode)!=70) return true;
  if (_saveTrueNumuCC     && vtx.NuPDG==+14 && ((vtx.ReacCode>0 && vtx.ReacCode<+30) || vtx.ReacCode==+70)) return true;
  if (_saveTrueAntiNumuCC && vtx.NuPDG==-14 && ((vtx.ReacCode<0 && vtx.ReacCode>-30) || vtx.ReacCode==-70)) return true;
  if (_saveTrueNueCC      && vtx.NuPDG==+12 && ((vtx.ReacCode>0 && vtx.ReacCode<+30) || vtx.ReacCode==+70)) return true;
  if (_saveTrueAntiNueCC  && vtx.NuPDG==-12 && ((vtx.ReacCode<0 && vtx.ReacCode>-30) || vtx.ReacCode==-70)) return true;
  return false;
}

//********************************************************************
bool CreateMiniTree::CheckTrueVertexDetector(unsigned long det){
//********************************************************************

  for (unsigned int i = 0;i<_saveTrueVertexInDet.size();i++)
    if (SubDetId::GetDetectorUsed(det,_saveTrueVertexInDet[i])) return true;

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch){
//********************************************************************

  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useSubdet2_1outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_1)) return true;
    if (_useSubdet2_2outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_2)) return true;
    if (_useSubdet2_3outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_3)) return true;
    if (_useSubdet1_1outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet1_1)) return true;
    if (_useSubdet1_2outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet1_2)) return true;
  }

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTree(const AnaBunchB& bunch){
//********************************************************************

  if (bunch.Bunch == -1) return CheckReconFillMiniTreeOutOfBunch(bunch);


  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useSubdet1_1)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet1_1)) return true;
    if (_useSubdet1_2)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet1_2)) return true;
    if (_useSubdet2_1)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_1)) return true;
    if (_useSubdet2_2)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_2)) return true;
    if (_useSubdet2_3)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kSubdet2_3)) return true;

  }

  return false;
}

//********************************************************************
void CreateMiniTree::WriteGeometry(){
//********************************************************************

  // Write the geometry when it changes
  if (_spill->GeomID != _currentGeomID){
    
    // Write the geometry in the geom directory
    if(_file) _file->cd("geom");
    //    ND::hgman().GeoManager()->Write();
    _file->cd();

    // Update the current GeomID
    _currentGeomID= _spill->GeomID;
  }

}

//********************************************************************
void CreateMiniTree::FillMiniTree(){
//********************************************************************

  // Copy arrays into std::vectors
  _spill->CopyArraysIntoVectors();

  // Update the POT and spills
  _spill->Beam->POTSincePreviousSavedSpill    = _POTSincePreviousSavedSpill;
  _spill->Beam->SpillsSincePreviousSavedSpill = _SpillsSincePreviousSavedSpill;
  
  // Fill the tree
  FillTree(minitree);

  // Reset to 0
  _POTSincePreviousSavedSpill   =0;
  _SpillsSincePreviousSavedSpill=0;

}

//********************************************************************
void CreateMiniTree::FillRooTrackerVtxTree(){
//********************************************************************
    
  if (HasTree(RooTrackerVtx)){
    // Select the interesting vertices
    if (_filterRoo) FilterRooTrackerVtxTree();
    
    // Fill the RooTrackerVtx tree
    OutputManager::FillTree(RooTrackerVtx);    
  }
}

//********************************************************************
void CreateMiniTree::FilterRooTrackerVtxTree(){
//********************************************************************
  
}

//********************************************************************
void CreateMiniTree::DeleteUninterestingBunches(){
//********************************************************************

  std::vector<AnaParticleB*> particlesToConsider;

  // Loop over bunches and schedule for deletion the ones not satisfying the conditions about reconstructed objects
  std::vector<Int_t> bunchesToDelete;
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    
    // Schedule bunches not fulfilling the requirements for deletion
    if (!CheckReconFillMiniTree(*bunch))
      bunchesToDelete.push_back(bunch->Bunch);
    else{
      // Save in a vector all belonging to vertices in that bunch
      for (std::vector<AnaVertexB*>::iterator it2=bunch->Vertices.begin();it2!=bunch->Vertices.end();it2++){      
        AnaVertexB* vertex = *it2;
        for (Int_t i = 0;i<vertex->nParticles;i++){
          AnaParticleB* particle = vertex->Particles[i];
          particlesToConsider.push_back(particle);
        }
      }
    }
  }

  // Delete the uninteresting bunches
  for (UInt_t i=0;i<bunchesToDelete.size();i++){
    for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){      
      AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    
      if (bunch->Bunch == bunchesToDelete[i]){
        bool found=false;
        // Don't delete if there is a particle in this bunch which belongs to a vertex in another bunch that is not deleted
        for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){      
          AnaParticleB* particle2 = *it2;
          for (std::vector<AnaParticleB*>::iterator it3=particlesToConsider.begin();it3!=particlesToConsider.end();it3++){      
            AnaParticleB* particle3 = *it3;
            if (particle2==particle3){
              found=true;
              break;
            }
          }
          if (found) break;
        }
        if (!found){        
          delete bunch;
          _spill->Bunches.erase(it);
          break;
        }
      }
    }
  }

  // Delete OutOfBunch
  if (_spill->OutOfBunch && !CheckReconFillMiniTreeOutOfBunch(*_spill->OutOfBunch)){

    bool found=false;
    for (std::vector<AnaParticleB*>::iterator it2=_spill->OutOfBunch->Particles.begin();it2!=_spill->OutOfBunch->Particles.end();it2++){      
      AnaParticleB* particle2 = *it2;
      for (std::vector<AnaParticleB*>::iterator it3=particlesToConsider.begin();it3!=particlesToConsider.end();it3++){      
        AnaParticleB* particle3 = *it3;
        if (particle2==particle3){
          found=true;
          break;
        }
      }
      if (found) break;
    }
    if (!found){

      delete _spill->OutOfBunch;
      _spill->OutOfBunch=NULL;
    }
  }

}



//********************************************************************
void CreateMiniTree::DeleteUninterestingParticles(){
//********************************************************************


  if (!_trackStartPreselection) return;
  
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    

    std::vector<AnaParticleB*> goodParticles;
    std::vector<AnaParticleB*> badParticles;     
    for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){      
      AnaParticleB* part = *it2;

      if (part->PositionStart[0]>= _trackStartMinX && part->PositionStart[0] <= _trackStartMaxX &&
          part->PositionStart[1]>= _trackStartMinY && part->PositionStart[1] <= _trackStartMaxY &&
          part->PositionStart[2]>= _trackStartMinZ && part->PositionStart[2] <= _trackStartMaxZ) {
        
        goodParticles.push_back(part);
      }
      else{
        badParticles.push_back(part);
      }
    }
    for (std::vector<AnaParticleB*>::iterator it2=badParticles.begin();it2!=badParticles.end();it2++){      
      delete *it2;
    }
    bunch->Particles = goodParticles;
  }
}


//********************************************************************
void CreateMiniTree::DeleteUninterestingTrueParticles(){
//********************************************************************

  // Keep the true particles associated to reconstructed particles and all its descendants
  // TODO: in principle no need to delete the true particles that are not kept since they will be deleted at the end of the event
  //       Since this is the final spill and not the raw spill we cannot delete particles !!!!

  if (!_trueWithRecoPreselection) return;

  std::set<AnaTrueParticleB*> goodTrueParticles;
  std::vector<AnaTrueParticleB*> badTrueParticles;
  //  std::vector<Int_t> goodTrueDaughters;

  //  std::cout << "1: True Particles:    " << _spill->TrueParticles.size() << std::endl;

  // add the first true particle, since it is ussually the beam one
  //  goodTrueParticles.insert(_spill->TrueParticles[0]);

  AnaTrueParticle* beampart = FindBeamTrueParticle(*_spill);
  if (beampart)   goodTrueParticles.insert(beampart);
  
  // Loop over all reconstructed particles in the spill      
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    

    //    std::cout << "1.5: Bunch Particles: " << bunch->Particles.size() << std::endl;
    for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){      
      AnaParticleB* part = *it2;

      //      std::cout <<  " - " << part->TrueObject << std::endl; 
      // If the particle has an associated true object add it
      if (part->TrueObject){
        AnaTrueParticle* truePart0 = static_cast<AnaTrueParticle*>(part->TrueObject);
        goodTrueParticles.insert(truePart0);

        if (_trueWithRecoDaughtersPreselection){
          // Get all dauthers of this true particle recursively (all descendants) and add them 
          std::vector<AnaTrueParticle*> goodTrueDaughters = anaUtils::GetTrueDaughters(_spill->TrueParticles, truePart0, true);
          for (std::vector<AnaTrueParticle*>::iterator it3=goodTrueDaughters.begin();it3!=goodTrueDaughters.end();it3++){      
            //          std::cout << "  - good daughter: " << (*it3)->ID << std::endl;
            goodTrueParticles.insert(*it3);
          }
        }
      }
    }
  }

  // Transfer from the set to the vector
  _spill->TrueParticles.assign( goodTrueParticles.begin(), goodTrueParticles.end());

  //  std::cout << "sizes: " << goodTrueParticles.size() << " " << _spill->TrueParticles.size() << std::endl;
  //  std::cout << "4: Final True Part:   " << _spill->TrueParticles.size() << std::endl;

}

//********************************************************************
bool CreateMiniTree::SpillLevelPreselection(){
//********************************************************************

  if (!_beamTOFPreselection) return true;

  if (!_spill->Beam) return true;

  // Don't apply tof cut for MC
  if (!static_cast<AnaBeam*>(_spill->Beam)->BeamParticle) return true;
  if (static_cast<AnaBeam*>(_spill->Beam)->BeamParticle->TrueObject) return true;
  
  //  double tof =  static_cast<AnaBeam*>(_spill->Beam)->TOF;
  
  //  if (tof > _beamMinTOF && tof < _beamMaxTOF) return true;
  //  else return false;

  return true;
}


//********************************************************************
AnaTrueParticle* CreateMiniTree::FindBeamTrueParticle(const AnaSpillB& spill){  
//********************************************************************


  AnaTrueParticle* beampart=NULL;
  
  AnaBeam* beam         = static_cast<AnaBeam*>(spill.Beam);
  AnaParticleMomB* beamPart = beam->BeamParticle;

  Float_t beammom=0;
  if (beamPart){
    if (beamPart->TrueObject){
      beammom = static_cast<AnaTrueParticleB*>(beamPart->TrueObject)->Momentum;
    }
  }
  
  if (spill.TrueParticles.size() > 0){
    for (UInt_t i =0; i< spill.TrueParticles.size();i++){
      if (beammom == spill.TrueParticles[i]->Momentum){
        beampart = static_cast<AnaTrueParticle*> (spill.TrueParticles[i]);
        break;
      }
    }
  }

  return beampart;

}
