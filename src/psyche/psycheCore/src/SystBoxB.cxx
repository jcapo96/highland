#include "SystBoxB.hxx"

//********************************************************************
SystBoxB::SystBoxB(){
//******************************************************************** 

  Reset();
  
}

//********************************************************************
void SystBoxB::Reset(){
//******************************************************************** 

  nRelevantRecObjects      = 0;
  nRelevantTrueObjects     = 0;
  RelevantRecObjects       = NULL;
  RelevantTrueObjects      = NULL;
  RelevantTrueObjectsReco  = NULL;

  RelevantRecObjectsSet.clear();
  
}

//********************************************************************
SystBoxB::~SystBoxB(){
//********************************************************************

  if (RelevantRecObjects)
    delete [] RelevantRecObjects;
  RelevantRecObjects = NULL;

  if (RelevantTrueObjects)
    delete [] RelevantTrueObjects;
  RelevantTrueObjects = NULL;
  
  if (RelevantTrueObjectsReco)
    delete [] RelevantTrueObjectsReco;
  RelevantTrueObjectsReco = NULL;

} 

