#include "pionAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  pionAnalysis* ana = new pionAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
