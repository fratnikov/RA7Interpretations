//#include<iostream>

//#include "TH2D.h"
//#include "TGraph.h"
//#include "TLegend.h"
//#include "TCanvas.h"

TGraph *getSleptonGraph(){

  float x[4];
  float y[4];

  //http://pdglive.lbl.gov/Rsummary.brl?nodein=S046&exp=Y
  float exc = 82.0;

  x[0] = 100.0; y[0] = 0;
  x[1] = 2*exc; y[1] = 0;
  x[2] = 100;   y[2] = 2*exc-100;
  x[3] = 100.0; y[3] = 0;

  TGraph *gr = new TGraph(4,x,y);
  gr->SetFillColor(kGray);
  gr->SetLineColor(0);

  return gr;
}

TGraph *getCharginoGraph(){

  float x[4];
  float y[4];

  //http://lepsusy.web.cern.ch/lepsusy/www/inos_moriond01/charginos_pub.html
  float exc = 103;

  x[0] = 100.0; y[0] =     0;
  x[1] =   exc; y[1] =     0;
  x[2] =   exc; y[2] =   exc;
  x[3] = 100.0; y[3] = 100.0;
  x[4] = 100.0; y[4] =     0;

  TGraph *gr = new TGraph(5,x,y);
  gr->SetFillColor(kRed);
  gr->SetLineColor(0);

  return gr;
}



void makeCombined3 () {
  TFile* fext = new TFile ("graphs/allGraphs.root", "read");
  TGraph* wz = dynamic_cast<TGraph*> (fext->FindObjectAny ("wz_observed")));
  wz->SetLineWidth (5);
  wz->SetLineColor (kBlue);
  wz->SetLineStyle (3);
  
  TGraph* obs_2i_05 = dynamic_cast<TGraph*> (fext->FindObjectAny ("2i_0.5_observed")));
  obs_2i_05->SetLineWidth (5);
  obs_2i_05->SetLineColor (kGreen+3);
  obs_2i_05->SetLineStyle (1);
  
  TGraph* obs_2a_05 = dynamic_cast<TGraph*> (fext->FindObjectAny ("2a_0.5_observed")));
  obs_2a_05->SetLineWidth (5);
  obs_2a_05->SetLineColor (kRed);
  obs_2a_05->SetLineStyle (2);

  TGraph* chipm = dynamic_cast<TGraph*> (fext->FindObjectAny ("chipm_observed")));
  chipm->SetLineWidth (5);
  chipm->SetLineColor (kMagenta);
  chipm->SetLineStyle (4);

  TGraph* lepChargino = getCharginoGraph();
  TGraph* lepSlepton = getSleptonGraph();


  TH2F* hCombined = new TH2F ("hCombined", "", 1, 100, 800, 1, 1, 800);
  hCombined->SetStats(0);
  hCombined->GetXaxis()->SetTitle ("m_{#tilde{#chi}_{1}^{#pm}} = m_{#tilde{#chi}_{2}^{0}}  [GeV]");
  hCombined->GetYaxis()->SetTitle ("m_{#tilde{#chi}_{1}^{0}}  [GeV]");
  hCombined->GetZaxis()->SetTitle ("95% CL upper limit on #sigma  [fb]");

  TLine* diagonal = new TLine (100, 100, 799, 750);
  diagonal->SetLineWidth(3);
  diagonal->SetLineColor(kBlack);
  diagonal->SetLineStyle(2);

  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.14);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.1);
  TCanvas* c = new TCanvas("interpret", "interpret", 0, 0, 800, 650);
  c->SetLogz();

  hCombined->Draw("axis");
//   lepSlepton->Draw("f");
//   lepChargino->Draw("f");

  diagonal->Draw("L");
  const char* graphStyle = "C";


  obs_2i_05->Draw (graphStyle);
  obs_2a_05->Draw (graphStyle);
  wz->Draw (graphStyle);
  chipm->Draw (graphStyle);
  
  TLegend* myleg = new TLegend(0.19,0.55,0.46,0.88,"","brNDC");
  myleg->SetTextFont(42);
  myleg->SetTextSize(0.04);
  myleg->SetFillColor(0); 
  myleg->SetShadowColor(0);
  myleg->SetTextSize(0.035);
  myleg->SetBorderSize(0);
//   myleg->AddEntry(lepSlepton, "LEP2 slepton limit", "f");
//   myleg->AddEntry(lepChargino, "LEP2 chargino limit", "f");
  myleg->AddEntry(obs_2i_05,"#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}, ( #tilde{#font[12]{l}}_{L}, BF(#font[12]{l}^{+}#font[12]{l}^{-})=0.5)","l");
  myleg->AddEntry(obs_2a_05,"#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}, ( #tilde{#font[12]{l}}_{R}, BF(#font[12]{l}^{+}#font[12]{l}^{-})=1)","l");
  myleg->AddEntry(wz,"#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}, ( no #tilde{#font[12]{l}}, BF(WZ)=1)","l");
  myleg->AddEntry(chipm,"#it{pp} #rightarrow #tilde{#chi}_{1}^{+} #tilde{#chi}_{1}^{-}, ( #tilde{#font[12]{l}}_{L}, BF(#font[12]{l}^{+}#font[12]{l}^{-})=1)","l");
  myleg->Draw();

  TLatex *   tex = new TLatex(140,820,
			      "CMS Preliminary         #sqrt{s} = 8 TeV, L_{int} = 19.5 fb^{-1}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();
  
  
  TLatex* xslep = new TLatex(0,-100., "m_{ #tilde{#font[12]{l}}} = 0.5m_{#tilde{#chi}_{1}^{#pm}} + 0.5m_{#tilde{#chi}_{1}^{0}}");
  xslep->SetTextFont(42);
  xslep->SetTextSize(0.04);
  xslep->Draw();
  
  TLatex* mhierarchy = new TLatex(150,200., "m_{ #tilde{#chi}_{2}^{0}} = m_{#tilde{#chi}_{1}^{#pm}} > m_{#tilde{#chi}_{1}^{0}}");
  mhierarchy->SetTextFont(42);
  mhierarchy->SetTextSize(0.04);
  mhierarchy->SetTextAngle(33);
  mhierarchy->SetTextColor(kBlack);
  mhierarchy->Draw();
  hCombined->Draw("sameaxis");
  
  
  gPad->SaveAs("exclusion_Combined.pdf");
  gPad->SaveAs("exclusion_Combined.C");
}
