// --- ProtoDUNE style ---

#include <TColor.h>
#include <TGaxis.h>

TStyle* SetProtoDUNEStyle(TString styleName = "ProtoDUNE") {
  TStyle *protoDUNEStyle= new TStyle(styleName, "ProtoDUNE approved plots style");
  
  // Colors
  //set the background color to white
  protoDUNEStyle->SetFillColor(10);
  protoDUNEStyle->SetFrameFillColor(10);
  protoDUNEStyle->SetCanvasColor(10);
  protoDUNEStyle->SetPadColor(10);
  protoDUNEStyle->SetTitleFillColor(10);
  protoDUNEStyle->SetStatColor(10); 

  //dont put a colored frame around the plots
  protoDUNEStyle->SetFrameBorderMode(0);
  protoDUNEStyle->SetCanvasBorderMode(0);
  protoDUNEStyle->SetPadBorderMode(0);

  //use the primary color palette
  protoDUNEStyle->SetPalette(1,0);

  //set the default line color for a histogram to be black
  protoDUNEStyle->SetHistLineColor(kBlack);
 
  //set the default line color for a fit function to be red
  protoDUNEStyle->SetFuncColor(kRed);

  //make the axis labels black
  protoDUNEStyle->SetLabelColor(kBlack,"xyz");

  //set the default title color to be black                                                                                                  
  protoDUNEStyle->SetTitleColor(kBlack);

  // Sizes

  //set the margins
  protoDUNEStyle->SetPadBottomMargin(0.125);
  protoDUNEStyle->SetPadTopMargin(0.075);
  protoDUNEStyle->SetPadLeftMargin(0.1);
  protoDUNEStyle->SetPadRightMargin(0.1);

  //set axis label and title text sizes
  protoDUNEStyle->SetLabelSize(0.045,"xy");
  protoDUNEStyle->SetLabelSize(0.035,"z");
  protoDUNEStyle->SetLabelOffset(0.005,"xy");
  //protoDUNEStyle->SetLabelOffset(0.005,"z");
  protoDUNEStyle->SetTitleSize(0.05,"xyz");
  protoDUNEStyle->SetTitleOffset(1,"x");
  protoDUNEStyle->SetTitleOffset(1,"yz");
  protoDUNEStyle->SetStatFontSize(0.05);
  protoDUNEStyle->SetTextSize(0.05);
  protoDUNEStyle->SetTitleBorderSize(0);
  protoDUNEStyle->SetStatBorderSize(0);

  //set line widths
  protoDUNEStyle->SetHistLineWidth(3);
  protoDUNEStyle->SetFrameLineWidth(2);
  protoDUNEStyle->SetFuncWidth(2);

  // Misc

  //align the titles to be centered
  protoDUNEStyle->SetTitleAlign(22); 

  //set the number of divisions to show
  protoDUNEStyle->SetNdivisions(506, "xy");

  //turn off xy grids
  protoDUNEStyle->SetPadGridX(0);
  protoDUNEStyle->SetPadGridY(0);

  //set the tick mark style
  protoDUNEStyle->SetPadTickX(1);
  protoDUNEStyle->SetPadTickY(1);

  //show the fit parameters in a box
  //protoDUNEStyle->SetOptFit(1111);

  //turn off all other stats
  protoDUNEStyle->SetOptStat(0);
  protoDUNEStyle->SetOptFit(0);
  protoDUNEStyle->SetOptTitle(0);

  //marker settings
  protoDUNEStyle->SetMarkerStyle(20);
  protoDUNEStyle->SetMarkerSize(0.9);

  // Fonts
  const int kProtoDUNEFont = 42;

  protoDUNEStyle->SetStatFont(kProtoDUNEFont);
  protoDUNEStyle->SetLabelFont(kProtoDUNEFont,"xyz");
  protoDUNEStyle->SetTitleFont(kProtoDUNEFont,"xyz");
  protoDUNEStyle->SetTitleFont(kProtoDUNEFont,"");
  protoDUNEStyle->SetTextFont(kProtoDUNEFont);

  // ---------------------------------------------------------
  // Additions from George 26/06/2019
  protoDUNEStyle->SetCanvasBorderSize(0);
  protoDUNEStyle->SetFrameBorderSize(0);
  protoDUNEStyle->SetDrawBorder(0);
  protoDUNEStyle->SetTitleBorderSize(0);

  // Set the size (in pixels) of the small lines drawn at the end of the error bars
  protoDUNEStyle->SetEndErrorSize(4);

  // Set option to strip decimals when drawing axis labels.
  protoDUNEStyle->SetStripDecimals(kFALSE);

  protoDUNEStyle->SetLegendBorderSize(0);
  protoDUNEStyle->SetLegendFont(kProtoDUNEFont);

  protoDUNEStyle->SetLabelOffset(0.015, "x");
  protoDUNEStyle->SetLabelOffset(0.015, "y");
  protoDUNEStyle->SetLabelOffset(0.01, "z");

  protoDUNEStyle->SetTitleStyle(0);
  protoDUNEStyle->SetTitleFont(kProtoDUNEFont, "pad");
  protoDUNEStyle->SetTitleX(0.1f);
  protoDUNEStyle->SetTitleY(.98f);
  protoDUNEStyle->SetTitleW(0.8f);
  protoDUNEStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

  // get rid of X error bars and y error bar caps
  protoDUNEStyle->SetErrorX(0.001);

  protoDUNEStyle->SetNumberContours(255);
  protoDUNEStyle->SetPalette(kBird);


  // ---------------------------------------------------------
  //done
  //protoDUNEStyle->cd();

  //gROOT->ForceStyle();
  //gStyle->ls();

  // Avoid too many decimal places in the axis labels
  TGaxis::SetMaxDigits(4);

  return protoDUNEStyle;
}


