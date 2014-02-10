{
  double xx[1024];
  double yy[1024];
// Graph observed
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 353.279, 350.724};
  double yy[] = {37.5, 62.5, 87.5, 98.8292, 99.3547, 104.587, 117.715, 133.89, 118.845, 74.1571, 34.3176, 25, 0};
  TGraph* observed = new TGraph(13, xx, yy);
// end observed

// Graph observedp
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 357.64, 356.851};
  double yy[] = {37.5, 62.5, 87.5, 100, 101.478, 107.303, 121.364, 139.365, 127.16, 83.7982, 46.7106, 25, 0};
  TGraph* observedp = new TGraph(13, xx, yy);
// end observedp

// Graph observedm
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 338.395, 348.369, 346.686};
  double yy[] = {37.5, 62.5, 87.5, 96.372, 97.0801, 101.731, 113.879, 128.135, 110.183, 67.3403, 50, 25, 0};
  TGraph* observedm = new TGraph(13, xx, yy);
// end observedm

// Graph expected
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 330.714, 334.714};
  double yy[] = {37.5, 62.5, 87.5, 90.3292, 94.2256, 100.701, 102.786, 92.246, 69.2453, 35.1206, 25, 0};
  TGraph* expected = new TGraph(12, xx, yy);
// end expected

// Graph expectedM1
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 257.722, 268.123, 266.081};
  double yy[] = {37.5, 62.5, 66.779, 68.29, 75.387, 73.6849, 57.1073, 50, 25, 0};
  TGraph* expectedM1 = new TGraph(10, xx, yy);
// end expectedM1

// Graph expectedP1
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 362.254, 366.932, 368.627};
  double yy[] = {37.5, 62.5, 87.5, 100.0, 106.935, 115.531, 128.231, 132.225, 115.68, 89.9488, 69.7404, 50, 25, 0};
  TGraph* expectedP1 = new TGraph(14, xx, yy);
// end expectedP1

// Graph expectedM2
  double xx[] = {100, 125, 150, 175, 200, 205.829, 228.883, 224.603};
  double yy[] = {36.761, 38.597, 42.7574, 48.9223, 52.1136, 50, 25, 0};
  TGraph* expectedM2 = new TGraph(8, xx, yy);
// end expectedM2

// Graph expectedP2
  double xx[] = {100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 375.926, 380.919, 384.278};
  double yy[] = {62.5, 62.5, 87.5, 112.5, 116.092, 123.874, 137.303, 151.608, 150.922, 141.119, 133.913, 79.8029, 75, 25, 0};
  TGraph* expectedP2 = new TGraph(15, xx, yy);
// end expectedP2

  TCanvas* c1 = new TCanvas();
  c1->cd();
  gPad->SetGridx();
  gPad->SetGridy();

  observed->SetLineWidth(5);
  observed->Draw("APl");

  observedm->Draw("samePl");
  observedp->Draw("samePl");

  expected->SetLineColor(2);
  expected->SetLineWidth(3);
  expected->SetMarkerColor(2);
  expected->Draw("samePl");

  expectedM1->SetLineColor(2);
  expectedM1->SetMarkerColor(2);
  expectedM1->Draw("samePl");

  expectedP1->SetLineColor(2);
  expectedP1->SetMarkerColor(2);
  expectedP1->Draw("samePl");


}
