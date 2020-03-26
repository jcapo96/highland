#include "ToyBoxPD.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxPD::ToyBoxPD():ToyBoxB(){
//******************************************************************** 
  
  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
} 

//********************************************************************
void ToyBoxPD::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxPD)){
      std::cerr << "ERROR in ToyBoxPD::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxPD::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxPD::ResetBase(){
//******************************************************************** 

  if (Vertex) delete Vertex; 

  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
  SoftReset();
}  
