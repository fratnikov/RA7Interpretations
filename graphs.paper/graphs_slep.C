{
  double xx[1024];
  double yy[1024];
// Graph observed
  double xx[] = {135,150, 175, 200, 225, 250, 275, 276.541, 283.199};
  double yy[] = {0,36.4235, 71.9345, 78.5651, 71.6116, 59.9835, 27.7184, 25, 0};
  TGraph* observed = new TGraph(9, xx, yy);
// end observed

// Graph observedp
  double xx[] = {132,150, 175, 200, 225, 250, 275, 284.704, 290.946};
  double yy[] = {0,41.816, 79.0, 83.1925, 77.1395, 68.394, 42.1219, 25, 0};
  TGraph* observedp = new TGraph(9, xx, yy);
// end observedp

// Graph observedm
  double xx[] = {138,150, 175, 200, 225, 250, 275, 275.053};
  double yy[] = {0,30.7544, 66.9882, 73.3403, 65.8183, 51.1415, 0.197237, 0};
  TGraph* observedm = new TGraph(8, xx, yy);
// end observedm

// Graph expected
  double xx[] = {130,150, 175, 200, 225, 250, 275, 278.792, 282.672};
  double yy[] = {0,49.4136, 80.0, 84.7569, 80.6062, 73.6377, 35.3293, 25, 0};
  TGraph* expected = new TGraph(9, xx, yy);
// end expected

// Graph expectedM1
  double xx[] = {120,125, 150, 175, 200, 225, 250, 275, 300, 302.874};
  double yy[] = {0,18.3904, 87.5, 112.5, 120, 104.757, 93.0228, 76.2628, 22.9183, 0};
  TGraph* expectedM1 = new TGraph(10, xx, yy);
// end expectedM1

// Graph expectedP1
  double xx[] = {155,175, 200, 225, 236.57, 244.375};
  double yy[] = {0,33.8148, 43.1474, 40.1031, 25, 0};
  TGraph* expectedP1 = new TGraph(6, xx, yy);
// end expectedP1

// Graph expectedM2
  double xx[] = {150, 175, 200, 225, 250, 275, 278.792, 282.672};
  double yy[] = {49.4136, 123.851, 84.7569, 80.6062, 73.6377, 35.3293, 25, 0};
  TGraph* expectedM2 = new TGraph(8, xx, yy);
// end expectedM2

// Graph expectedP2
  double xx[] = {150, 175, 200, 225, 250, 275, 278.792, 282.672};
  double yy[] = {49.4136, 123.851, 84.7569, 80.6062, 73.6377, 35.3293, 25, 0};
  TGraph* expectedP2 = new TGraph(8, xx, yy);
// end expectedP2

}
