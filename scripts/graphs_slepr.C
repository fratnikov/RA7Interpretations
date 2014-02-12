{
  double xx[1024];
  double yy[1024];
// Graph observed
  double xx[] = {150, 175, 181.025, 184.753};
  double yy[] = {16.6448, 32.0231, 25, 0};
  TGraph* observed = new TGraph(4, xx, yy);
// end observed

// Graph observedp
  double xx[] = {150, 175, 184.066, 188.247};
  double yy[] = {25.707, 35.5676, 25, 0};
  TGraph* observedp = new TGraph(4, xx, yy);
// end observedp

// Graph observedm
  double xx[] = {150, 175, 177.828, 181.08};
  double yy[] = {5.13947, 28.2968, 25, 0};
  TGraph* observedm = new TGraph(4, xx, yy);
// end observedm

// Graph expected
  double xx[] = {150, 200, 205.528};
  double yy[] = {27.7044, 8.24365, 0};
  TGraph* expected = new TGraph(3, xx, yy);
// end expected

// Graph expectedM1
  double xx[] = {125, 150, 175, 200, 211.984};
  double yy[] = {18.5499, 52.1732, 47.8497, 23.8494, 0};
  TGraph* expectedM1 = new TGraph(5, xx, yy);
// end expectedM1

// Graph expectedP1
  double xx[] = {};
  double yy[] = {};
  TGraph* expectedP1 = new TGraph(0, xx, yy);
// end expectedP1

// Graph expectedM2
  double xx[] = {150, 200, 205.528};
  double yy[] = {27.7044, 8.24365, 0};
  TGraph* expectedM2 = new TGraph(3, xx, yy);
// end expectedM2

// Graph expectedP2
  double xx[] = {150, 200, 205.528};
  double yy[] = {27.7044, 8.24365, 0};
  TGraph* expectedP2 = new TGraph(3, xx, yy);
// end expectedP2

}
