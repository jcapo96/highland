#include "ToyBoxDUNE.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxDUNE::ToyBoxDUNE():ToyBoxB(){
//******************************************************************** 
  
  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
  Ltrack=SLtrack=TLtrack=NULL;
} 

//********************************************************************
void ToyBoxDUNE::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxDUNE)){
      std::cerr << "ERROR in ToyBoxDUNE::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxDUNE::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxDUNE::ResetBase(){
//******************************************************************** 

  if (Vertex) delete Vertex; 

  TrueVertex=NULL;
  Vertex=NULL;
  MainTrack=NULL;
  Ltrack=SLtrack=TLtrack=NULL;
  SoftReset();
}  
