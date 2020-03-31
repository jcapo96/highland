#include "ProductionUtils.hxx"

//********************************************************************
void anaUtils::AddStandardProductions(){
//********************************************************************

  // Add the different productions
  
  //May change when production evolves
  ND::versioning().AddProduction(ProdId::PDSPProd2,  "PDSPProd2","v08_40",  "v08_40");
}

//********************************************************************
ProdId_h anaUtils::GetProductionIdFromReader(){
//********************************************************************

//  ProdId_h prodId=-1;
  ProdId_h prodId=UNKNOWN_PRODUCTION;

  prodId=ProdId::PDSPProd2;

  return prodId;

}
