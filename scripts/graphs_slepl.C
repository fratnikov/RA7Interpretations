{
  double xx[1024];
  double yy[1024];
// Graph observed
  double xx[] = {125, 150, 175, 200, 225, 250, 259.696, 269.915, 269.819};
  double yy[] = {28.7402, 57.9756, 72.1303, 89.4854, 101.071, 60.1683, 50, 25, 0};
  TGraph* observed = new TGraph(9, xx, yy);
// end observed

// Graph observedp
  double xx[] = {125, 150, 175, 200, 225, 250, 265.474, 280.518, 274.837};
  double yy[] = {29.2577, 59.168, 74.2106, 93.0483, 102.68, 66.2274, 50, 25, 0};
  TGraph* observedp = new TGraph(9, xx, yy);
// end observedp

// Graph observedm
  double xx[] = {125, 150, 175, 200, 225, 250, 253.622, 255.412, 264.544};
  double yy[] = {28.1962, 56.722, 69.9433, 85.7397, 72.7904, 53.7984, 50, 25, 0};
  TGraph* observedm = new TGraph(9, xx, yy);
// end observedm

// Graph expected
  double xx[] = {125, 150, 175, 200, 225, 250, 275, 282.008, 283.252};
  double yy[] = {29.8071, 61.6941, 78.2102, 94.2853, 100.372, 80.9906, 26.6329, 25, 0};
  TGraph* expected = new TGraph(9, xx, yy);
// end expected

// Graph expectedM1
  double xx[] = {125, 150, 175, 200, 225, 250, 275, 300, 305.241, 303.878};
  double yy[] = {32.5044, 69.0914, 83.9893, 106.481, 111.772, 97.4787, 81.7272, 32.0525, 25, 0};
  TGraph* expectedM1 = new TGraph(10, xx, yy);
// end expectedM1

// Graph expectedP1
  double xx[] = {125, 150, 175, 200, 225, 227.635, 235.894, 242.863};
  double yy[] = {26.8549, 54.1167, 52.6461, 44.374, 52.626, 50, 25, 0};
  TGraph* expectedP1 = new TGraph(8, xx, yy);
// end expectedP1

// Graph expectedM2
  double xx[] = {125, 150, 175, 200, 225, 250, 275, 282.008, 283.252};
  double yy[] = {29.8071, 61.6941, 78.2102, 94.2853, 100.372, 80.9906, 26.6329, 25, 0};
  TGraph* expectedM2 = new TGraph(9, xx, yy);
// end expectedM2

// Graph expectedP2
  double xx[] = {125, 150, 175, 200, 225, 250, 275, 282.008, 283.252};
  double yy[] = {29.8071, 61.6941, 78.2102, 94.2853, 100.372, 80.9906, 26.6329, 25, 0};
  TGraph* expectedP2 = new TGraph(9, xx, yy);
// end expectedP2

}
