{
  std::string hostname = gSystem->HostName();

  std::string dir = "/hep/DUNE/DataDir/MicroTrees/";
  if (hostname.find("neutrinos")!=std::string::npos) dir = "/data4/DUNE/DataDir/MicroTrees/";


  std::string fdata    = dir+"data_run5387_dau.root";
  std::string fmcsce   = dir+"mc11_sce_1GeV_dau.root";
  std::string fmc3ms   = dir+"mc11_3ms_1GeV_dau.root";
  std::string fmcflf   = dir+"mc11_flf_1GeV_dau.root";

  TFile *_file0 = TFile::Open(fdata.c_str());
  TFile *_file1 = TFile::Open(fmcsce.c_str());
  TFile *_file2 = TFile::Open(fmc3ms.c_str());
  TFile *_file3 = TFile::Open(fmcflf.c_str());

  TTree* d   = (TTree*)_file0->Get("default");
  TTree* mcs = (TTree*)_file1->Get("default");
  TTree* mc3 = (TTree*)_file2->Get("default");
  TTree* mcf = (TTree*)_file3->Get("default");

}
