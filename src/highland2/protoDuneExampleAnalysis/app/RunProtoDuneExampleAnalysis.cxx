#include "protoDuneExampleAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  protoDuneExampleAnalysis* ana = new protoDuneExampleAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
