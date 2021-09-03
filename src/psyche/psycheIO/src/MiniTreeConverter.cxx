#include "MiniTreeConverter.hxx"
#include "BasicUtils.hxx"
//#include "VersioningUtils.hxx"
#include "Parameters.hxx"
//#include "GeometryManager.hxx"
#include <typeinfo>  

AnaSpillC* RawSpillC=NULL;

//********************************************************************
MiniTreeConverter::MiniTreeConverter(const std::string& tree_path, bool readRooTrackerVtx):InputConverter(tree_path){
//********************************************************************

  // Use corrected values or raw values
  //  _useCorrectedValues = ND::params().GetParameterI("highlandIO.ReadFlatTree.UseCorrectedValues");

  AddChain(_treeName);

  minitree = GetChain(_treeName);
  NRooTrackerVTX = NULL;
  GRooTrackerVTX = NULL;
  _spill = NULL;
  _entry_roo=0;

  fChain = minitree;

  // If true the RooTrackerVtx tree will be read
  _readRooTrackerVtx = readRooTrackerVtx;
}

//********************************************************************
bool MiniTreeConverter::Initialize(){
//********************************************************************

  // Set branch addresses and branch pointers
  if (!fChain) return false;
  
  // Check the existence of the "minitree" tree
  //  if (!gDirectory->FindObjectAny(_treeName.c_str())) return false;
  
  fCurrent = -1;
  fChain->SetMakeClass(1);
  
  return true;   
}

//********************************************************************
MiniTreeConverter::~MiniTreeConverter(){
//********************************************************************
  
  if (!fChain) return;

  if (minitree)       delete   minitree->GetCurrentFile();
  if (GRooTrackerVTX) delete   GRooTrackerVTX->GetCurrentFile();
  if (NRooTrackerVTX) delete   NRooTrackerVTX->GetCurrentFile();
  if (minitree)       delete   minitree;
  if (GRooTrackerVTX) delete   GRooTrackerVTX;
  if (NRooTrackerVTX) delete   NRooTrackerVTX;
}

//****************************************************************************
bool MiniTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "MiniTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

  // ------------- Check that the file has some entries. Otherwise ignore it -----------------
  TChain dummy(_treeName.c_str());
  dummy.AddFile(inputString.c_str());
  //  if (dummy.GetEntries("sEvt>=0") == 0){
  if (dummy.GetEntries() == 0){
    std::cout << "      ----> This file does not contain any entries. IGNORED !!!!" << std::endl;
    return true;
  }
  
  // Open the file to do few checks
  TFile *f = TFile::Open(inputString.c_str()); 
  f->cd(); 

  // ------------- Check that the header tree exists (needed for POT counting). If it doesn't ignore the file -----------------

  //  bool isMC = false;
  if (!gDirectory->FindObjectAny("header")){
    std::cout << "      ----> This file does not contain a header tree. IGNORED !!!!" << std::endl;
    return true;
  }
  else{
    TChain chain("header");
    chain.AddFile(inputString.c_str());
    Header *header=0;
    chain.SetBranchAddress("POTInfo", &header);    
    chain.GetEntry(0);      
    //    isMC = header->GetIsMC(); 
  }

  // ----------  Deal with RooTrackerVtx trees. Only for the first file

  static bool first = true; 
  if( first ) {
    first = false; 
    fGenie=fNeut=false;

    if( gDirectory->FindObjectAny("NRooTrackerVtx")) {
      fNeut = true;
      AddChain("NRooTrackerVtx");
      NRooTrackerVTX = GetChain("NRooTrackerVtx");
      NRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
      NRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
      NRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
      NVtx = new TClonesArray("ND::NRooTrackerVtx",100);
      NRooTrackerVTX->SetBranchAddress("NVtx",&NNVtx);
      NRooTrackerVTX->SetBranchAddress("Vtx",&NVtx);
      std::cout << "MiniTreeConverter::AddFileToTChain(). NEUT RooTrackerVtx tree found !!" << std::endl; 
    }
    else if(gDirectory->FindObjectAny("GRooTrackerVtx")) {
      fGenie = true;
      AddChain("GRooTrackerVtx");
      GRooTrackerVTX = GetChain("GRooTrackerVtx");
      GRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
      GRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
      GRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
      GVtx = new TClonesArray("ND::GRooTrackerVtx",100);
      GRooTrackerVTX->SetBranchAddress("NVtx",&NGVtx);
      GRooTrackerVTX->SetBranchAddress("Vtx",&GVtx);
      std::cout << "MiniTreeConverter::AddFileToTChain(). GENIE RooTrackerVtx tree found !!" << std::endl; 
    }
  }

  // Close the file
  f->Close(); 
  
  // ------------- Add the file to the RooTrackerVtx chain

  if( fGenie && GRooTrackerVTX) {
    GRooTrackerVTX->AddFile(inputString.c_str());
  }
  else if( fNeut  && NRooTrackerVTX) {
    NRooTrackerVTX->AddFile(inputString.c_str());
  }

  // ------------- Add the file to the Minitree chain
  minitree->AddFile(inputString.c_str());  
  // Read the header tree for POT counting (when running over a file list POT is incremented), IsMC and SoftwareVersion
  //  bool bySpillPOT = (versionUtils::prod6_POT || !isMC); // by Spill POT is not available for prod5 MC
  bool bySpillPOT = true; // TODOD DUNE
  return header().AddHeader(inputString,bySpillPOT); 
}

//*****************************************************************************
Int_t MiniTreeConverter::GetEvent(Long64_t& entry, AnaEventC*& event){
//*****************************************************************************

//  static AnaSpillB* RawSpill = 0;
  static Int_t currentBunch = 0;
  Int_t entry_temp=1;

  AnaSpillB* RawSpill = static_cast<AnaSpillB*>(RawSpillC);

  if (RawSpill && currentBunch < (Int_t)(RawSpill->Bunches.size()-1)){
    currentBunch++;
  }
  else{
    if (RawSpill) delete RawSpill;
    entry_temp = GetSpill(entry,RawSpillC);
    if (entry_temp<=0 || !RawSpill || RawSpill->Bunches.size()==0) return 0;
    currentBunch=0;
  }

  // Create a new event from the RawSpill and the current bunch
  event = MakeEvent(*RawSpill,*static_cast<AnaBunchB*>((RawSpill->Bunches[currentBunch])));

  return entry_temp;
}

//*****************************************************************************
Int_t MiniTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
//*****************************************************************************

  // Read contents of entry.
  if (!fChain) return 0;
  
  // Create the appropriate instance of Spill
  spill = MakeSpill();
  
  // cast to AnaSpillB
  _spill = static_cast<AnaSpillB*>(spill);

  // Set the branch address
  fChain->SetBranchAddress("Spill", &spill);
  
  // Print the current file
  static std::string currentfilename=""; 
  std::string filename =minitree->GetFile()->GetName();   
  if( filename != currentfilename ) {
    std::cout << " Running on file: " << filename << std::endl; 
    currentfilename = filename;
  }

  // get a new entry from the flat tree. entry_temp >0 when succesfull
  Int_t entry_temp = minitree->GetEntry(entry);

  if (_spill->EventInfo){
    if (_readRooTrackerVtx){
      bool sIsMC    = _spill->EventInfo->IsMC;
      Int_t sEvt    = _spill->EventInfo->Event;
      Int_t sSubrun = _spill->EventInfo->SubRun;
      Int_t sRun    = _spill->EventInfo->Run;
      
      // sEvt should be positive since sEvt=-999 is used for the last flatree entry
      if (entry_temp>0 && sIsMC && (fGenie || fNeut) && sEvt>=0) {
        // Loop over RooTrackerVtx entries until we get the same run, subrun and event numbers
        // In general we will get the right entry in the first iteration, but just in case
        do{       
          if      (NRooTrackerVTX)  NRooTrackerVTX->GetEntry(_entry_roo);
          else if (GRooTrackerVTX)  GRooTrackerVTX->GetEntry(_entry_roo);
          if ((RunID> sRun                                      ) ||
              (RunID==sRun && SubrunID> sSubrun                 ) ||
              (RunID==sRun && SubrunID==sSubrun && EventID>sEvt ))
            _entry_roo--;  
          else
            _entry_roo++;  
        }while(EventID!=sEvt || RunID!=sRun || SubrunID!=sSubrun);
      }
    }
    
    // Copy vectors into arrays
    _spill->CopyVectorsIntoArrays();
    // Redo reco-reco and reco-truth links (many not pressent in MiniTree)
    _spill->RedoLinks();
  }
  else
    entry_temp=0;


  // Increment entry number
  entry++;
  if (entry%10000==0 || entry == _nentries || (entry%1000==0 && entry<10000) )
    std::cout << "entry: " << entry << " of " << _nentries << " (" << (100*entry/_nentries) << "%)" << std::endl;

  // Load the geometry for this spill(999 is the default value in BaseDataClasses). TODO DUNE
  //  if (_spill->GeomID!=999)
    //    ND::hgman().LoadGeometry(filename,(Int_t)_spill->GeomID,"geom");

  return entry_temp;
}

//********************************************************************
void MiniTreeConverter::IncrementPOTBySpill() {
//********************************************************************

// TODO DUNE.  anaUtils::IncrementPOTBySpill(*_spill,header());
    //  header().IncrementPOTBySpill(*_spill);
}

//*****************************************************************************
AnaEventB* MiniTreeConverter::MakeEvent(AnaSpillB& spill, AnaBunchB& bunch) {
//*****************************************************************************

#ifndef MULTITHREAD
  return new AnaEventB(spill, bunch);
#endif

  // For MULTITHREAD we must recreate the links

  AnaEventB* event = new AnaEventB();

  event->UniqueID = 0;
  event->Particles = NULL;
  event->Vertices = NULL;
  event->TrueParticles = NULL;
  event->TrueVertices = NULL;
  event->nParticles = 0;
  event->nVertices = 0;
  event->nTrueParticles = 0;
  event->nTrueVertices = 0;
  event->Beam = NULL;
  event->DataQuality = NULL;
  event->isClone = false;

  event->nEventBoxes=0;
  for (UInt_t i=0;i<NMAXEVENTBOXES;i++)
    event->EventBoxes[i]=NULL;
  
  // The initial weight of the Event is 1;
  event->Weight=1;
  
  // Must create a summary object when we create an event
  // This is initialised to NULL and SampleId::kUnassigned, so you know it has not passed a selection
  event->Summary = new AnaEventSummaryB();

  std::map<AnaRecObjectC*,  AnaRecObjectC*>  clonedRecObject;
  std::map<AnaTrueObjectC*, AnaTrueObjectC*> clonedTrueObject;
  std::map<AnaTrueVertexB*, AnaTrueVertexB*> clonedTrueVertex;

  //------ Copy from Spill and Bunch ----------------

  event->Weight      = bunch.Weight;
  event->Bunch       = bunch.Bunch;
  event->EventInfo   = *spill.EventInfo;
  event->Beam        = spill.Beam->Clone();
  event->DataQuality = spill.DataQuality->Clone();

  // Fill the recon tracks vector
  event->nParticles = 0;
  anaUtils::CreateArray(event->Particles, bunch.Particles.size());
  for (UInt_t i=0;i<bunch.Particles.size();i++){
    clonedRecObject[bunch.Particles[i]] = event->Particles[event->nParticles] = bunch.Particles[i]->Clone();

    if (bunch.Particles[i]->TrueObject){
      if (bunch.Particles[i]->TrueObject->ID>=0){
        clonedTrueObject[bunch.Particles[i]->TrueObject] = event->Particles[event->nParticles]->TrueObject = bunch.Particles[i]->TrueObject->Clone();
        bunch.Particles[i]->TrueObject->ID *=-1;
      }
      else{
        event->Particles[event->nParticles]->TrueObject = clonedTrueObject[bunch.Particles[i]->TrueObject];
      }
      if (bunch.Particles[i]->GetTrueParticle()->TrueVertex){
        if(bunch.Particles[i]->GetTrueParticle()->TrueVertex->ID>=0){
          clonedTrueVertex[bunch.Particles[i]->GetTrueParticle()->TrueVertex] = event->Particles[event->nParticles]->GetTrueParticle()->TrueVertex = bunch.Particles[i]->GetTrueParticle()->TrueVertex->Clone();
          bunch.Particles[i]->GetTrueParticle()->TrueVertex->ID*=-1;
        }
        else
          event->Particles[event->nParticles]->GetTrueParticle()->TrueVertex = clonedTrueVertex[bunch.Particles[i]->GetTrueParticle()->TrueVertex];
      }
    }
    event->nParticles++;
  }

  // Fill the true tracks vector
  event->nTrueParticles = 0;
  anaUtils::CreateArray(event->TrueParticles, spill.TrueParticles.size());
  for (UInt_t i=0; i<spill.TrueParticles.size();i++){
    // Recreate the links
    if (spill.TrueParticles[i]->ID>=0){
      clonedTrueObject[spill.TrueParticles[i]] = event->TrueParticles[event->nTrueParticles] = spill.TrueParticles[i]->Clone();    
      spill.TrueParticles[i]->ID*=-1;

      if (spill.TrueParticles[i]->TrueVertex){
        if(spill.TrueParticles[i]->TrueVertex->ID>=0){
          clonedTrueVertex[spill.TrueParticles[i]->TrueVertex] = event->TrueParticles[event->nTrueParticles]->TrueVertex = spill.TrueParticles[i]->TrueVertex->Clone();
          spill.TrueParticles[i]->TrueVertex->ID*=-1;
        }
        else
          event->TrueParticles[event->nTrueParticles]->TrueVertex = clonedTrueVertex[spill.TrueParticles[i]->TrueVertex];
      }
    }
    else
      event->TrueParticles[event->nTrueParticles] = static_cast<AnaTrueParticleB*>(clonedTrueObject[spill.TrueParticles[i]]);    

    event->nTrueParticles++;
  }

  // Fill the true vertices vector
  event->nTrueVertices = 0;
  anaUtils::CreateArray(event->TrueVertices, spill.TrueVertices.size());
  for (UInt_t i=0;i<spill.TrueVertices.size();i++){
    if (spill.TrueVertices[i]->ID>=0){
      clonedTrueVertex[spill.TrueVertices[i]] = event->TrueVertices[event->nTrueVertices] = spill.TrueVertices[i]->Clone();
      spill.TrueVertices[i]->ID*=-1;
    }
    else
      event->TrueVertices[event->nTrueVertices] = clonedTrueVertex[spill.TrueVertices[i]];
    
    for (Int_t j=0; j<event->TrueVertices[event->nTrueVertices]->nTrueParticles;j++){
      event->TrueVertices[event->nTrueVertices]->TrueParticles[j] = static_cast<AnaTrueParticleB*>(clonedTrueObject[spill.TrueVertices[i]->TrueParticles[j]]);    
    }
    event->nTrueVertices++;
  }


  // Fill the recon vertices vector
  event->nVertices = 0;
  anaUtils::CreateArray(event->Vertices, bunch.Vertices.size());
  for (UInt_t i=0;i<bunch.Vertices.size();i++){
    // Recreate the links
    if (bunch.Vertices[i]->TrueVertex)
      event->Vertices[event->nVertices]->TrueVertex = clonedTrueVertex[bunch.Vertices[i]->TrueVertex];

    for (Int_t j=0;j<bunch.Vertices[i]->nParticles;j++){
      event->Vertices[i]->Particles[j] = static_cast<AnaParticleB*>(clonedRecObject[ bunch.Vertices[i]->Particles[j] ]);
    }
    event->nVertices++;
  }


  for (UInt_t i=0; i<spill.TrueParticles.size();i++){
    if (spill.TrueParticles[i]->ID<0){
      spill.TrueParticles[i]->ID*=-1;
    }
  }

  for (UInt_t i=0;i<spill.TrueVertices.size();i++){
    if (spill.TrueVertices[i]->ID<=0){
      spill.TrueVertices[i]->ID*=-1;
    }
  }

}

//*****************************************************************************
Int_t MiniTreeConverter::GetNEvents(Int_t entries0){
//*****************************************************************************

  Int_t entries = entries0;
  if (entries0==-1) entries=GetEntries();

  // TODO. Assume for the moment 20% more events that entries in the MiniTree. 
  return 1.2*entries;
}



