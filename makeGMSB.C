void makeGMSB ()
{
  bool benOnly = false;
  double scale = 1;

  TFile* f12 = new TFile (benOnly ? "hists/GMSB.2l2j.root" : "hists/GMSB.2l2jshruti.root", "read");
  //TFile* f12 = new TFile (benOnly ? "hists/GMSB.ru.root" : "hists/GMSB.2l2jru.root", "read");
  TH2D* processXSection = dynamic_cast<TH2D*> f12->FindObjectAny ("processXSection");
  TH2D* hObsLimitPlain = dynamic_cast<TH2D*> f12->FindObjectAny ("hObsLimitPlain");
  TH2D* hExpLimit = dynamic_cast<TH2D*> f12->FindObjectAny ("hExpLimit");
  TH2D* hObsLimitSmooth = dynamic_cast<TH2D*> f12->FindObjectAny ("hObsLimitSmooth");
  TH2D* hExpLimitSmooth = dynamic_cast<TH2D*> f12->FindObjectAny ("hExpLimitSmooth");
  TH2D* hExpM1LimitSmooth = dynamic_cast<TH2D*> f12->FindObjectAny ("hExpM1LimitSmooth");
  TH2D* hExpP1LimitSmooth = dynamic_cast<TH2D*> f12->FindObjectAny ("hExpP1LimitSmooth");


  TFile* f1 = new TFile ("hists/GMSB.2l2j.root", "read");
  TH2D* hObsLimitSmooth1 = dynamic_cast<TH2D*> f1->FindObjectAny ("hObsLimitSmooth");

  TFile* f2 = new TFile ("hists/GMSB.shruti.root", "read");
  //TFile* f2 = new TFile ("hists/GMSB.ru.corrected417.root", "read");
  TH2D* hObsLimitSmooth2 = dynamic_cast<TH2D*> f2->FindObjectAny ("hObsLimitSmooth");


  TH2D* xSecObserved = new TH2D(*processXSection);
  xSecObserved->SetName("xSecObserved");
  xSecObserved->Multiply (hObsLimitSmooth);

  TH2D* xSecExpected = new TH2D(*processXSection);
  xSecExpected->SetName("xSecExpected");
  xSecExpected->Multiply (hExpLimitSmooth);

  TH2D* xSecObserved1 = new TH2D(*processXSection);
  xSecObserved1->SetName("xSecObserved1");
  xSecObserved1->Multiply (hObsLimitSmooth1);

  TH2D* xSecObserved2 = new TH2D(*processXSection);
  xSecObserved2->SetName("xSecObserved2");
  xSecObserved2->Multiply (hObsLimitSmooth2);

  int nBins = hObsLimit->GetXaxis()->GetNbins();
  double mMin = hObsLimit->GetXaxis()->GetBinCenter (1);
  double mMax = hObsLimit->GetXaxis()->GetBinCenter(nBins);
  TH1F* hObsLimit1 = new TH1F ("hObsLimit1", "", nBins, mMin, mMax);
  hObsLimit1->UseCurrentStyle();
  hObsLimit1->SetStats(0);
  hObsLimit1->SetMinimum(0.02);
  hObsLimit1->SetMaximum(10);
  hObsLimit1->GetXaxis()->SetTitle ("#mu  [GeV]");
  hObsLimit1->GetYaxis()->SetTitle ("95% CL upper limit on #sigma  [pb]");

  TGraph* gObsLimit = new TGraph();
  gObsLimit->SetLineWidth(5);
  TGraph* gObsLimit1 = new TGraph();
  gObsLimit1->SetLineWidth(3);
  gObsLimit1->SetLineColor(kMagenta);
  gObsLimit1->SetLineStyle(4);
  TGraph* gObsLimit2 = new TGraph();
  gObsLimit2->SetLineWidth(3);
  gObsLimit2->SetLineColor(kGreen+3);
  gObsLimit2->SetLineStyle(8);

  TGraphAsymmErrors* gExpLimit = new TGraphAsymmErrors();
  gExpLimit->SetLineWidth(3);
  gExpLimit->SetLineColor(kBlue);
  gExpLimit->SetLineStyle(2);
  gExpLimit->SetFillColor(kCyan-7);
  TGraphAsymmErrors* gTheory = new TGraphAsymmErrors();
  gTheory->SetLineWidth(2);
  gTheory->SetLineColor(kRed);
  gTheory->SetFillColor(kYellow);

  for (int i = 1; i <= nBins; ++i) {
    double xBin = xSecObserved->GetXaxis()->GetBinCenter (i);
    gObsLimit->SetPoint (i-1, xBin, xSecObserved->GetBinContent (i, 1));
    gObsLimit1->SetPoint (i-1, xBin, xSecObserved1->GetBinContent (i, 1));
    gObsLimit2->SetPoint (i-1, xBin, xSecObserved2->GetBinContent (i, 1));
    //cout << "observed: " << xBin<<':'<<xSecObserved->GetBinContent (i, 1)<<'/'<<xSecObserved1->GetBinContent (i, 1)<<'/'<<xSecObserved2->GetBinContent (i, 1) << endl;
    double expected = xSecExpected->GetBinContent (i, 1);
    double expectedLE = expected * (1 - hExpP1LimitSmooth->GetBinContent (i, 1) / hExpLimitSmooth->GetBinContent (i, 1));
    double expectedHE = expected * (hExpM1LimitSmooth->GetBinContent (i, 1) / hExpLimitSmooth->GetBinContent (i, 1) - 1);
    //    cout << "mass/exp/le/he/obs1/obs2: " << xBin <<'/'<<expected<<'/'<<expectedLE<<'/'<<expectedHE<<<<'/'<<expected1<<'/'<<expected2<<endl;
    gExpLimit->SetPoint (i-1, xBin, expected);
    gExpLimit->SetPointError (i-1, 0, 0, expectedLE, expectedHE);

    double theoryXsec = processXSection->GetBinContent (i, 1);
    gTheory->SetPoint (i-1, xBin, theoryXsec);
    gTheory->SetPointError (i-1, 0, 0, theoryXsec*0.05, theoryXsec*0.05);
  }

  cout << "gObsLimit->Print();" << endl;
  gObsLimit->Print();

  TCanvas* ccc = new TCanvas("ccc", "ccc", 0, 0, 900, 750);
  ccc->SetLogy();
  hObsLimit1->Draw();
  gExpLimit->Draw("3");
  gExpLimit->Draw("lx");
  //  gExpLimit->Draw("cx");
  if (!benOnly) {
    gObsLimit1->Draw("l");
    gObsLimit2->Draw("l");
  }
  gTheory->Draw("3");
  gTheory->Draw("cx");
  gObsLimit->Draw("l");
  hObsLimit1->Draw("AXISsame");


  TLegend* myleg = new TLegend(0.40,0.60,0.80,0.88,"   95% C.L. CLs Limits","brNDC");
  myleg->SetTextFont(42);
  myleg->SetTextSize(0.04);
  ((TLegendEntry*)myleg->GetListOfPrimitives()->At(0))->SetTextFont(42);
  ((TLegendEntry*)myleg->GetListOfPrimitives()->At(0))->SetTextSize(0.035);
  myleg->SetFillColor(0); 
  myleg->SetShadowColor(0);
  myleg->SetTextSize(0.035);
  myleg->SetBorderSize(0);
  myleg->AddEntry(gObsLimit,benOnly ? "observed 2#font[12]{l}2#font[12]{j}" : "observed 2#font[12]{l}2#font[12]{j} + 4#font[12]{l} + 3#font[12]{l}","l");
  myleg->AddEntry(gExpLimit,"expected median #pm 1 #sigma","lf");
  if (!benOnly) {
    myleg->AddEntry(gObsLimit1,"observed 2#font[12]{l}2#font[12]{j}","l");
    myleg->AddEntry(gObsLimit2,"observed 4#font[12]{l} + 3#font[12]{l}","l");
  }
  myleg->AddEntry(gTheory,"#sigma^{NLO} #pm 1 #sigma","lf");
  myleg->Draw();

  TLatex *   tex = new TLatex(115, 12,
			      "CMS Preliminary   #sqrt{s} = 8 TeV, L_{int} = 19.5 fb^{-1}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();

  TPaveText *pt = new TPaveText(120,0.025,220,0.07,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetTextAlign(12);
  pt->SetTextFont(42);
  pt->SetTextSize(0.035);
  pt->AddText("GMSB  ZZ+E_{T}^{miss}");
  pt->AddText("  tan#beta = 2");
  pt->AddText("  M_{1} = M_{2} = 1 TeV");
  pt->Draw();

  
  gPad->SaveAs(benOnly ? "newfiles/exclusion_GMSB_2l2j.pdf" : "newfiles/exclusion_GMSB.pdf");
  gPad->SaveAs(benOnly ? "newfiles/exclusion_GMSB_2l2j.C" : "newfiles/exclusion_GMSB.root");

}
