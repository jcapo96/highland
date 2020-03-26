{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());

  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetTitleXOffset(0.8);
  gStyle->SetTitleYOffset(0.8);
  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce,false);

  // Track length distribution
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitleX("X [cm]");
  draw.SetTitleY("Y [cm]");


  draw.SetTitle("MC track start position for L/CSDA>0.8");
  draw.Draw(mcs,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");

  c1->WaitPrimitive();

  draw.SetTitle("MC track start position for L/CSDA<0.5");
  draw.Draw(mcs,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");

  c1->WaitPrimitive();


  draw.SetTitle("data track start position for L/CSDA>0.8");
  draw.Draw(d,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");

  c1->WaitPrimitive();

  draw.SetTitle("data track start position for L/CSDA<0.5");
  draw.Draw(d,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");

  c1->WaitPrimitive();

  
  draw.SetTitle("data beam track end position for L/CSDA>0.8");
  draw.Draw(d,"beam_endpos[1]:beam_endpos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(beam_endpos[0]+30,2)+pow(beam_endpos[1]-422,2))<30","colz","AREA");

  c1->WaitPrimitive();

  draw.SetTitle("data beam track end position for L/CSDA<0.5");
  draw.Draw(d,"beam_endpos[1]:beam_endpos[0]",50,-50,0,50,400,450,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(beam_endpos[0]+30,2)+pow(beam_endpos[1]-422,2))<30","colz","AREA");

  
  
  //  draw.SetTitle("data (color) and MC (box) track start position");
  //  draw.Draw(d,mcs,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2","colz","");

  //  draw.Draw(mcs,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2","box same","");

  
  c1->WaitPrimitive();

  c1->Clear();
  c1->Divide(2,2);

  c1->cd(1);
  draw.SetTitleX("X direction");
  draw.SetTitle("for dE/dx>10 MeV/c & L/CSDA>0.8");
  draw.Draw(d,mcs,"seltrk_dir[0]",100,-0.6,0.,"particle","accum_level[0][0]>2 && seltrk_dedx_binned[2][0]>10 && seltrk_length/seltrk_csdarange_prot>0.8","HIST","AREA");

  c1->cd(2);
  draw.SetTitle("for dE/dx>10 MeV/c & L/CSDA<0.5");
  draw.Draw(d,mcs,"seltrk_dir[0]",100,-0.6,0.,"particle","accum_level[0][0]>2 && seltrk_dedx_binned[2][0]>10 && seltrk_length/seltrk_csdarange_prot<0.5","HIST","AREA");


  draw.SetTitleX("Y direction");
  c1->cd(3);
  draw.SetTitle("for dE/dx>10 MeV/c & L/CSDA>0.8");
  draw.Draw(d,mcs,"seltrk_dir[1]",100,-0.6,0.,"particle","accum_level[0][0]>2 && seltrk_dedx_binned[2][0]>10 && seltrk_length/seltrk_csdarange_prot>0.8","HIST","AREA");

  c1->cd(4);
  draw.SetTitle("for dE/dx>10 MeV/c & L/CSDA<0.5");
  draw.Draw(d,mcs,"seltrk_dir[1]",100,-0.6,0.,"particle","accum_level[0][0]>2 && seltrk_dedx_binned[2][0]>10 && seltrk_length/seltrk_csdarange_prot<0.5","HIST","AREA");


  c1->WaitPrimitive();
  
  /*
  
  draw.SetTitle("data (color) and MC (box) track start position");
  draw.Draw(d,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.85 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");

  draw.Draw(mcs,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.85 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","box same","AREA");

  
  c1->WaitPrimitive();

  
  draw.SetTitle("data track start (box) and beam end (color) position");
  draw.Draw(d,"beam_endpos[1]:beam_endpos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.85 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","colz","AREA");
  
  draw.Draw(d,"seltrk_pos[1]:seltrk_pos[0]",50,-50,0,50,400,440,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.85 && seltrk_dedx_binned[2][0]>10 && sqrt(pow(seltrk_pos[0]+30,2)+pow(seltrk_pos[1]-422,2))<30","box same","AREA");

  c1->WaitPrimitive();  
  */
}
