void dumpGraph (const TGraph* graph, const string& title, ostream& out = cout) {
  out << "// Graph " << title << endl;
  int nPoints = graph->GetN();
  out << "  double xx[] = {";
  for (int i = 0; i < nPoints; ++i) {
    if (i != 0) out << ", ";
    out << graph->GetX()[i];
  }
  out << "};" << endl;

  out << "  double yy[] = {";
  for (int i = 0; i < nPoints; ++i) {
    if (i != 0) out << ", ";
    out << graph->GetY()[i];
  }
  out << "};" << endl;

  out << "  TGraph* " << title << " = new TGraph(" << nPoints << ", xx, yy);" << endl;

  out << "// end " << title << endl << endl;
}

void dumpData () {
  ofstream out ("HWcurves.C");
  dumpGraph (observed, "observed", out); 
  dumpGraph (observed1m, "observedm", out); 
  dumpGraph (observed1p, "observedp", out); 
  dumpGraph (expected, "expected", out); 
  dumpGraph (expected1m, "expectedM1", out); 
  dumpGraph (expected1p, "expectedP1", out); 
}

void dumpHists () {
  TH2D processXSection (*hOverlayObs25);
  processXSection.SetName ("processXSection");
  processXSection.Scale (1e-3);
  TH2D hObsLimitPlain (processXSection);
  hObsLimitPlain.SetName("hObsLimitPlain");
  for (int ix = 1; ix <= hObsLimitPlain.GetXaxis()->GetNbins(); ++ix) {
    for (int iy = 1; iy <= hObsLimitPlain.GetYaxis()->GetNbins(); ++iy) {
      hObsLimitPlain.SetBinContent (ix, iy, 1.);
    }
  }

  TH2D hObsLimitSmooth (hObsLimitPlain);
  hObsLimitSmooth.SetName("hObsLimitSmooth");

  TH2D hExpLimitSmooth (hObsLimitPlain);
  hExpLimitSmooth.SetName("hExpLimitSmooth");

  TH2D hExpM1LimitSmooth (hObsLimitPlain);
  hExpM1LimitSmooth.SetName("hExpM1LimitSmooth");

  TH2D hExpP1LimitSmooth (hObsLimitPlain);
  hExpP1LimitSmooth.SetName("hExpP1LimitSmooth");

  TH2D hExpM2LimitSmooth (hObsLimitPlain);
  hExpM2LimitSmooth.SetName("hExpM2LimitSmooth");

  TH2D hExpP2LimitSmooth (hObsLimitPlain);
  hExpP2LimitSmooth.SetName("hExpP2LimitSmooth");

  TFile fout ("WH.root", "RECREATE");
  processXSection.Write();
  hObsLimitPlain.Write();
  hObsLimitSmooth.Write();
  hExpLimitSmooth.Write();
  hExpM1LimitSmooth.Write();
  hExpP1LimitSmooth.Write();
  hExpM2LimitSmooth.Write();
  hExpP2LimitSmooth.Write();
}
