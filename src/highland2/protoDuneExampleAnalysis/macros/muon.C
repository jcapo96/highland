{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());

  bool save_plots=false;
  bool wait_canvas=true;
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce,false);
  draw.SetLineWidth(1);

  gStyle->SetTitleXOffset(0.8);
  gStyle->SetTitleYOffset(1);


  draw.Draw(d,mcs,"seltrk_length",100,0,500,"particle","/accum_level[0][2]>2 || ","","AREA");
  

  c1->WaitPrimitive();
  
  draw.SetTitleX("muon range momentum [GeV/c]");
  draw.Draw(d,mcs,"seltrk_mom_muon",100,0,2,"particle","accum_level[0][2]>3","","AREA");



  c1->WaitPrimitive();

  
}
