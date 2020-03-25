#include "duneExampleAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  duneExampleAnalysis* ana = new duneExampleAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
