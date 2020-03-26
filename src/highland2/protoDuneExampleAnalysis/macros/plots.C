{

  std::string filename= "kaon_3gev.root";
  
  TFile *_file0 = TFile::Open(filename.c_str());
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  
  // Create a DrawingTools instance initialized with a micro-tree file
  DrawingTools draw(filename);
  
  // Number of tracks
  draw.SetTitleX("number of tracks");
  draw.Draw(default,"ntracks",15,0,15,"particle","accum_level>-1","HIST","DRAWALLMC PUR");


  c1.Print("ntracks.png");
    
  c1.WaitPrimitive();

  // z starting position of selected track
  draw.SetTitleX("kaon candidate z position (cm)");
  draw.Draw(default,"seltrk_pos[2]",50,-10,500,"particle","accum_level>0","","PUR");
  c1.Print("zpos.png");
  
  c1.WaitPrimitive();

  // z ending position of selected track
  draw.SetTitleX("kaon candidate z end position (cm)");
  draw.Draw(default,"seltrk_endpos[2]",50,-10,500,"particle","accum_level>0");
  c1.Print("zendpos.png");
  
  c1.WaitPrimitive();

  // length of selected track
  draw.SetTitleX("kaon candidate range (cm)");
  draw.Draw(default,"seltrk_length",60,0,600,"particle","accum_level>0");
  c1.Print("range.png");
  
  c1.WaitPrimitive();

  // length of selected track
  draw.SetTitle("Length of selected track for particles decaying at rest");
  draw.SetTitleX("kaon candidate range (cm)");
  draw.Draw(default,"seltrk_length",200,0,500,"particle","accum_level>0 && seltrk_trueendproc==2 && seltrk_trueendmom<0.01");
  draw.DrawCutLineVertical(190,false,"r");
  draw.DrawCutLineVertical(210,false,"l");
  c1.Print("range_decayrest.png");
  
  c1.WaitPrimitive();

  // Number of tracks
  draw.SetTitle("number of tracks after range cut");
  draw.SetTitleX("number of tracks");
  draw.Draw(default,"ntracks",15,0,15,"particle","accum_level>1","","PUR");
  draw.DrawCutLineVertical(2,true,"r");
  c1.Print("ntracks_after_rangecut.png");
    
  c1.WaitPrimitive();


  
  // PIDA
  draw.SetTitle("PIDA before cuts (for most upstream track)");
  draw.SetTitleX("kaon candidate PIDA");
  draw.Draw(default,"seltrk_pid[4]",50,0,30,"particle","accum_level>0","","PUR");
  c1.Print("pida.png");
  
  c1.WaitPrimitive();
  
  // PIDA
  draw.SetTitle("PIDA after range cut");
  draw.SetTitleX("kaon candidate PIDA");
  draw.Draw(default,"seltrk_pid[4]",50,0,30,"particle","accum_level>1","","PUR");
  c1.Print("pida_after_rangecut.png");
  
  c1.WaitPrimitive();

  // PIDA
  draw.SetTitle("PIDA after range and >1 track cuts");
  draw.SetTitleX("kaon candidate PIDA");
  draw.Draw(default,"seltrk_pid[4]",50,0,30,"particle","accum_level>2","","PUR");
  draw.Draw(default,"seltrk_pida_raw",50,0,30,"all","accum_level>2","HIST same","");
  c1.Print("pida_aftercuts.png");
  
  c1.WaitPrimitive();


  // PIDA 
  draw.SetTitle("recomputed PIDA after range and >1 track cuts");
  draw.SetTitleX("kaon candidate PIDA");
  draw.Draw(default,"seltrk_pida_raw",50,0,30,"particle","accum_level>2","","PUR");
  draw.DrawCutLineVertical(15.-4,true,"r");
  draw.DrawCutLineVertical(15.+4,true,"l");
  c1.Print("pida_recomputed__aftercuts.png");

  c1.WaitPrimitive();


  
  
  // average dE/dx of selected track after multiplicity cut
  draw.SetTitle("average dE/dx after range and >1 track cuts");
  draw.SetTitleX("kaon candidate average dE/dx (MeV/mm)");
  draw.Draw(default,"seltrk_dedx",50,1,6,"particle","accum_level>2", "", "PUR");
  c1.Print("average_dedx_aftercuts.png");

  c1.WaitPrimitive();

  draw.SetTitle("");

  // Number of events after each cut
  gStyle->SetLabelSize(0.06);
  draw.DrawEventsVSCut(default);
  c1.Print("evts_vs_cut.png");
  
  c1.WaitPrimitive();
  
  // Create a data sample instance with the micro-tree file.
  // Needed to plot efficiency (from truth tree) and purity (from default tree) simultaneously
  DataSample mc(filename);
  
  // kaon selection Efficiency and purity after each cut
  draw.SetTitleY("kaon selection eff & purity");
  draw.DrawEffPurVSCut(mc,"true_signal==1");
  c1.Print("eff_pur_vs_cut.png");

  /*
  c1.WaitPrimitive();

  gStyle->SetLabelSize(0.035);
  
  // True muon momentum after all cuts
  draw.SetLegendPos("l");
  draw.SetTitle("true momentum after range and >1 track cuts");
  draw.SetTitleX("kaon candidate true momentum (MeV/c)");
  draw.SetTitleY("");
  draw.Draw(mc,"seltrk_truemom",100,0,1.1,"particle","accum_level>2", "", "PUR");
  c1.Print("truemom_aftercuts.png");
  */
}
