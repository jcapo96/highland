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
  
  _saveGeometry = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveGeometry");
  _trueWithRecoPreselection          = (bool)ND::params().GetParameterD("highlandIO.MiniTree.Truth.WithRecoPreselection");
  _trueWithRecoDaughtersPreselection = (bool)ND::params().GetParameterD("highlandIO.MiniTree.Truth.IncludeTrueDaughters");

  _currentGeomID=999;

  //  _file->mkdir("geom");

  // initialize filtering counters
  _totalParticles = 0;
  _savedParticles = 0;
  _totalTrueParticles = 0;
  _savedTrueParticles = 0;
  
  
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
  DeleteUninterestingBunches();

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

  // Dump info about true and reco filtering
  std::cout << std::setprecision(2);
  std::cout << "Reco particle filtering factor " << 100*double(_savedParticles)/_totalParticles << "%" << std::endl; 
  std::cout << "True particle filtering factor " << 100*double(_savedTrueParticles)/_totalTrueParticles << "%" << std::endl; 

  //---- Save the last spill (if not save yet) such that we keep track of the total POT since the last saved entry.

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

  const std::vector<AnaTrueVertexB*>& vertices = spill.TrueVertices;
  for (std::vector<AnaTrueVertexB*>::const_iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *static_cast<AnaTrueVertex*>(*it);
    if (CheckTrueVertex(vtx)) return true;
  }

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch){
//********************************************************************

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTree(const AnaBunchB& bunch){
//********************************************************************

  if (bunch.Bunch == -1) return CheckReconFillMiniTreeOutOfBunch(bunch);
  return true;
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


//  if (!_trackStartPreselection) return;
  
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    

    std::vector<AnaParticleB*> goodParticles;
    std::vector<AnaParticleB*> badParticles;     
    for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){      
      AnaParticleB* part = *it2;           

      // increment counter      
      _totalParticles++;
      
      if (CheckSaveParticle(*part)) {        

        // Filter information inside good particles
        FilterParticleInfo(*part);

        // increment counter
        _savedParticles++;
        
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
  
  // Loop over all reconstructed particles in the spill      
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);    

    for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){      
      AnaParticleB* part = *it2;

      // If the particle has an associated true object add it
      if (part->TrueObject){
        AnaTrueParticle* truePart0 = static_cast<AnaTrueParticle*>(part->TrueObject);
        goodTrueParticles.insert(truePart0);

        if (_trueWithRecoDaughtersPreselection){
          // Get all dauthers of this true particle recursively (all descendants) and add them 
          std::vector<AnaTrueParticle*> goodTrueDaughters = anaUtils::GetTrueDaughters(_spill->TrueParticles, truePart0, true);
          for (std::vector<AnaTrueParticle*>::iterator it3=goodTrueDaughters.begin();it3!=goodTrueDaughters.end();it3++){      
            goodTrueParticles.insert(*it3);
          }
        }
      }
    }
  }

  // Transfer from the set to the vector
  _spill->TrueParticles.assign( goodTrueParticles.begin(), goodTrueParticles.end());
}

