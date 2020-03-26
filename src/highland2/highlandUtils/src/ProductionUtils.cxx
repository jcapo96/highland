#include "ProductionUtils.hxx"

//********************************************************************
void anaUtils::AddStandardProductions(){
//********************************************************************

  // Add the different productions
  
  //May change when production evolves
  //  ND::versioning().AddProduction(ProdId::PROD7DEVEL, "PROD7DEVEL", "v12",        "v13");
}

//********************************************************************
ProdId_h anaUtils::GetProductionIdFromoaAnalysisReader(){
//********************************************************************

//  ProdId_h prodId=-1;
  ProdId_h prodId=UNKNOWN_PRODUCTION;

  prodId=ProdId::MCC5;

  return prodId;

}
