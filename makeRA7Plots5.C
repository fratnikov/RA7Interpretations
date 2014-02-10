#include<iostream>
#include<math.h>

#include "TStyle.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TColor.h"

using namespace std;

#include "makeExclu.h"
#include "TBetterLegend2.h"

void repareHoles (TH2D* hist) {
  for (int iy = 1; iy <= hist->GetNbinsY(); ++iy) {
    for (int ix = 1; ix <= hist->GetNbinsX(); ++ix) {
      if (hist->GetBinContent (ix,iy) <=0) {
        if (ix > 1 && ix < hist->GetNbinsX() &&
            hist->GetBinContent (ix-1,iy) >1e-40 &&
            hist->GetBinContent (ix+1,iy) >1e-40) {
          hist->SetBinContent (ix,iy, 0.5*(hist->GetBinContent (ix-1,iy)+hist->GetBinContent (ix+1,iy)));
        }
      }
    }
  }

  for (int iy = 1; iy <= hist->GetNbinsY(); ++iy) {
    for (int ix = 1; ix <= hist->GetNbinsX(); ++ix) {
      if (hist->GetBinContent (ix,iy) <=0) {
        if (iy > 1 && iy < hist->GetNbinsY() &&
            hist->GetBinContent (ix,iy-1) >1e-40 &&
            hist->GetBinContent (ix,iy+1) >1e-40) {
          hist->SetBinContent (ix,iy, 0.5*(hist->GetBinContent (ix,iy-1)+hist->GetBinContent (ix,iy+1)));
        }
      }
    }
  }

  for (int iy = 1; iy <= hist->GetNbinsY(); ++iy) {
    for (int ix = 1; ix <= hist->GetNbinsX(); ++ix) {
      if (hist->GetBinContent (ix,iy) <=0) {
        if (ix > 1 && ix < hist->GetNbinsX() &&
	    iy > 1 && iy < hist->GetNbinsY() &&
            hist->GetBinContent (ix-1,iy-1) >1e-40 &&
            hist->GetBinContent (ix+1,iy+1) >1e-40) {
          hist->SetBinContent (ix,iy, 0.5*(hist->GetBinContent (ix-1,iy-1)+hist->GetBinContent (ix+1,iy+1)));
        }
      }
    }
  }
//   bool thisDone = true;
//   while (thisDone) {
//     thisDone = false;
//     for (int iy = 1; !thisDone && iy <= hist->GetNbinsY(); ++iy) {
//       for (int ix = 1; !thisDone && ix <= hist->GetNbinsX(); ++ix) {
//         if (hist->GetBinContent (ix,iy) <=0) {
//           if (ix > 1 && iy > 1) {
//             if (hist->GetBinContent (ix-1,iy-1) > 1e-40 &&
//                 hist->GetBinContent (ix,iy-1) > 1e-40 &&
//                 hist->GetBinContent (ix-1,iy) > 1e-40) {
//               hist->SetBinContent (ix,iy, 
//                                    hist->GetBinContent (ix,iy-1)*
//                                    hist->GetBinContent (ix-1,iy)/
//                                    hist->GetBinContent (ix-1,iy-1));
//               thisDone = true;
//               break;
//             }
//           }
//         }
//       }
//     }
//   }
}

void cleanDiagonal (TH2D& hist, double offset = 25.) {
  for (int ix = 1; ix <= hist.GetXaxis()->GetNbins(); ++ix) {
    double x = hist.GetXaxis()->GetBinCenter (ix);
    for (int iy = 1; iy <= hist.GetYaxis()->GetNbins(); ++iy) {
      double y = hist.GetYaxis()->GetBinCenter (iy);
      if (y > x-offset) hist.SetBinContent (ix, iy, 0);
    }
  }
}

const char* inFile = "graphs/allGraphs.root";
bool getGraphs = false;
int canvasX=800;
int canvasY=600;
double scaleXsection = 1.;
double plotZoom = 1;
double cleanOffset = 0;
int maxXbin = 99;
double xsecMin;
double xsecMax;
const char* file12 = 0;
const char* file1 = 0;
const char* file2 = 0;
const char* processText1 = 0;
const char* processText2 = 0;
const char* processText3 = 0;
const char* processText4 = 0;
const char* labelX = 0;
const char* labelY = 0;
const char* labelZ = "95% C.L. upper limit on cross section (fb)";
const char* label12 = 0;
const char* labelExpected = 0;
const char* labelExpected2 = 0;
const char* label1 = 0;
const char* label2 = 0;
const char* massLabel = 0;
double massAngle = 45;
const char* xslepLabel = 0;
double diagonalOffset = 0;
bool logSmoothBypass = false;

TH2D* xSecObserved = 0;
TH2F* hFrame = 0;
TGraph* observed = 0;
TGraph* observedp = 0;
TGraph* observedm = 0;
TGraph* expected = 0;
TGraph* expectedM1 = 0;
TGraph* expectedP1 = 0;
TGraph* expectedM2 = 0;
TGraph* expectedP2 = 0;
TGraph* observed1 = 0;
TGraph* observed2 = 0;

// bool fancy = false;
//bool fancy = true;
bool fancy = false;
bool draw2sigma = false;

void printGraphs (const char* outFile) {
  ofstream out (outFile);
  out << "{" << endl;
  out << "  double xx[1024];" << endl;
  out << "  double yy[1024];" << endl;
  if (observed) dumpGraph (observed, "observed", out);
  if (observedp) dumpGraph (observedp, "observedp", out);
  if (observedm) dumpGraph (observedm, "observedm", out);
  if (expected) dumpGraph (expected, "expected", out);
  if (expectedM1) dumpGraph (expectedM1, "expectedM1", out);
  if (expectedP1) dumpGraph (expectedP1, "expectedP1", out);
  if (expectedM2) dumpGraph (expectedM2, "expectedM2", out);
  if (expectedP2) dumpGraph (expectedP2, "expectedP2", out);
  if (observed1) dumpGraph (observed1, "observed1", out);
  if (observed2) dumpGraph (observed2, "observed2", out);
  out << "}" << endl;
}

void reset () {
  scaleXsection = 1.;
  plotZoom = 1;
  cleanOffset = 0;
  maxXbin = 99;
  xsecMin = 0;
  xsecMax = 0;
  file12 = 0;
  file1 = 0;
  file2 = 0;
  processText1 = 0;
  processText2 = 0;
  processText3 = 0;
  processText4 = 0;
  labelX = 0;
  labelY = 0;
  labelZ = "95% C.L. upper limit on cross section (fb)";
  label12 = 0;
  labelExpected = 0;
  labelExpected2 = 0;
  label1 = 0;
  label2 = 0;
  massLabel = 0;
  massAngle = 45;
  xslepLabel = 0;
  diagonalOffset = 0;
  logSmoothBypass = false;
  //logSmoothBypass = true;
  observed1 = 0;
  observed2 = 0;
  draw2sigma = false;
}

void makeObjects (const string option) {
  // color, style, width
  TAttLine attObs (kBlack, 1, 4);
  TAttLine attObsTheo (kBlack, 1, 2);
  TAttLine attExp (kRed, 7, 4);
  TAttLine attExpSig (kRed, 7, 2);
  TAttLine attExpSig2 (kRed, 3, 2);
  TAttLine attObs1 (kMagenta, 5, 4);
  TAttLine attObs2 (kBlue, 6, 4);

  if (fancy) {
      attObs = TAttLine (kRed, 1, 7);
      attObsTheo = TAttLine (kRed, 1, 3);
      attExp = TAttLine (kYellow, 1, 6);
      attExpSig = TAttLine (kYellow, 1, 3);
      attExpSig2 = TAttLine (kOrange, 3, 3);
      attObs1 = TAttLine (kMagenta, 1, 5);
      attObs2 = TAttLine (kBlue, 1, 5);
  }

  const Int_t NRGBs = 5;
  const Int_t NCont = 255; 
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.50, 0.50, 1.00, 1.00, 1.00 };
  Double_t green[NRGBs] = { 0.50, 1.00, 1.00, 0.60, 0.50 };
  Double_t blue[NRGBs]  = { 1.00, 1.00, 0.50, 0.40, 0.50 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  bool use2 = file1 && file2;
  bool PAS = true;
  TFile* f12 = new TFile (file12, "read");
  TH2D* processXSection = dynamic_cast<TH2D*> (f12->FindObjectAny ("processXSection"));
  cout << "point 5:1->" << processXSection->GetBinContent(5,1) << endl;
  TH2D* hObsLimitPlain = dynamic_cast<TH2D*> (f12->FindObjectAny ("hObsLimitPlain"));
  TH2D* hObsLimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hObsLimitSmooth"));
  if (PAS) cleanDiagonal (*hObsLimitSmooth, cleanOffset);
  TH2D* hExpLimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpLimitSmooth"));
  if (PAS) cleanDiagonal (*hExpLimitSmooth, cleanOffset);
  TH2D* hExpM1LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpM1LimitSmooth"));
  if (PAS) cleanDiagonal (*hExpM1LimitSmooth, cleanOffset);
  TH2D* hExpP1LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpP1LimitSmooth"));
  if (PAS) cleanDiagonal (*hExpP1LimitSmooth, cleanOffset);
  TH2D* hExpM2LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpM2LimitSmooth"));
  if (!hExpM2LimitSmooth)  hExpM2LimitSmooth = new TH2D (*hExpLimitSmooth);
  if (PAS) cleanDiagonal (*hExpM2LimitSmooth, cleanOffset);
  TH2D* hExpP2LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpP2LimitSmooth"));
  if (!hExpP2LimitSmooth) hExpP2LimitSmooth = new TH2D (*hExpLimitSmooth);
  if (PAS) cleanDiagonal (*hExpP2LimitSmooth, cleanOffset);
//   TH2D* hExpM3LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpM3LimitSmooth"));
//   cleanDiagonal (*hExpM3LimitSmooth, cleanOffset);
//   TH2D* hExpP3LimitSmooth = dynamic_cast<TH2D*> (f12->FindObjectAny ("hExpP3LimitSmooth"));
//   cleanDiagonal (*hExpP3LimitSmooth, cleanOffset);

  xSecObserved = new TH2D(*processXSection);
  cout << "xSecObserved 5:1->" << xSecObserved->GetBinContent(5,1) << endl;
  xSecObserved->UseCurrentStyle();
  xSecObserved->SetName("xSecObserved");
  xSecObserved->Scale (1000./scaleXsection);
  xSecObserved->Multiply (hObsLimitPlain);
  xSecObserved->SetStats(false);
  xSecObserved->SetTitle("");
  xSecObserved->GetXaxis()->SetTitle(labelX);
  xSecObserved->GetYaxis()->SetTitle(labelY);

  xSecObserved->GetZaxis()->SetTitleFont(42);
  xSecObserved->GetZaxis()->SetTitleSize(0.045);
  xSecObserved->GetZaxis()->SetTitleOffset(1.35);
  xSecObserved->GetZaxis()->SetTitle(labelZ);
  xSecObserved->SetMinimum (xsecMin);
  xSecObserved->SetMaximum (xsecMax);
  repareHoles (xSecObserved);
  cleanDiagonal (*xSecObserved, cleanOffset);
  cout << "xSecObserved 2 5:1->" <<xSecObserved->GetBinContent(5,1) << endl;

  // clean also diagonals for limit settings
//   cleanDiagonal (*hObsLimitSmooth, cleanOffset);
//   cleanDiagonal (*hExpLimitSmooth, cleanOffset);
//   cleanDiagonal (*hExpM1LimitSmooth, cleanOffset);
//   cleanDiagonal (*hExpP1LimitSmooth, cleanOffset);
//   cleanDiagonal (*hExpM2LimitSmooth, cleanOffset);
//   cleanDiagonal (*hExpP2LimitSmooth, cleanOffset);


  double xMin = xSecObserved->GetXaxis()->GetBinCenter (1);
  double xMax = xSecObserved->GetXaxis()->GetBinCenter (int(xSecObserved->GetXaxis()->GetNbins()/plotZoom));
  double yMin = xSecObserved->GetYaxis()->GetBinCenter (1);
  double yMax = xSecObserved->GetYaxis()->GetBinCenter (int(xSecObserved->GetYaxis()->GetNbins()/plotZoom))*canvasY/canvasX*1.1;
  //double yMax = xSecObserved->GetYaxis()->GetBinCenter (int(xSecObserved->GetYaxis()->GetNbins()/plotZoom));
  hFrame = new TH2F ("hFrame", "", 2, xMin, xMax, 1, yMin, yMax);


//   cout << "set : " << xMin<<':'<<xMax<<'/'<<yMin<<':'<<yMax<<endl;
//   cout << "actual" << hFrame->GetXaxis()->GetXmin()<<':'<<hFrame->GetXaxis()->GetXmax()
//        <<'/'<<hFrame->GetYaxis()->GetXmin()<<':'<<hFrame->GetYaxis()->GetXmax()<<endl;

  hFrame->UseCurrentStyle();
  hFrame->SetStats(false);
  hFrame->GetXaxis()->SetLabelFont(42);
  hFrame->GetXaxis()->SetLabelSize(0.04);
  hFrame->GetXaxis()->SetTitleFont(42);
  hFrame->GetXaxis()->SetTitleSize(0.05);
  hFrame->GetXaxis()->SetTitleOffset(1.2);
  hFrame->GetXaxis()->SetTitle(labelX);

  hFrame->GetYaxis()->SetLabelFont(42);
  hFrame->GetYaxis()->SetLabelSize(0.04);
  hFrame->GetYaxis()->SetTitleFont(42);
  hFrame->GetYaxis()->SetTitleSize(0.05);
  hFrame->GetYaxis()->SetTitleOffset(1.35);
  hFrame->GetYaxis()->SetTitle(labelY);

  hFrame->GetZaxis()->SetTitleFont(42);
  hFrame->GetZaxis()->SetTitleSize(0.04);
  hFrame->GetZaxis()->SetTitleOffset(1.35);
  hFrame->GetZaxis()->SetTitle(labelZ);
  hFrame->GetZaxis()->SetLimits(xsecMin, xsecMax);
  hFrame->SetBinContent (1,1,xsecMin);
  hFrame->SetBinContent (2,1,xsecMax);

  //double cleanOffsetPlots = cleanOffset;
  double cleanOffsetPlots = 0;
  
  observed = makeCurveUpDown (makeLogSmooth(hObsLimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
  
  observedp = makeCurveUpDown (makeLogSmooth(hObsLimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection*1.05);
  observedm = makeCurveUpDown (makeLogSmooth(hObsLimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection*0.95);
 
  expected = makeCurveUpDown (makeLogSmooth(hExpLimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);

  expectedM1 = makeCurveUpDown (makeLogSmooth(hExpM1LimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
  expectedP1 = makeCurveUpDown (makeLogSmooth(hExpP1LimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
  expectedM2 = makeCurveUpDown (makeLogSmooth(hExpM2LimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
  expectedP2 = makeCurveUpDown (makeLogSmooth(hExpP2LimitSmooth, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);


  TH2D* hObsLimitSmooth1 = 0;
  TH2D* hObsLimitSmooth2 = 0;
  if (use2) {
    TFile* f1 = new TFile (file1, "read");
    hObsLimitSmooth1 = dynamic_cast<TH2D*> (f1->FindObjectAny ("hObsLimitSmooth"));
    cleanDiagonal (*hObsLimitSmooth1, cleanOffset); // <--- must be removed
    observed1 = makeCurveUpDown (makeLogSmooth(hObsLimitSmooth1, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
    
    TFile* f2 = new TFile (file2, "read");
    hObsLimitSmooth2 = dynamic_cast<TH2D*> (f2->FindObjectAny ("hObsLimitSmooth"));
    cleanDiagonal (*hObsLimitSmooth2, cleanOffset); // <--- must be removed
    observed2 = makeCurveUpDown (makeLogSmooth(hObsLimitSmooth2, logSmoothBypass, cleanOffsetPlots), 1, maxXbin, 1, 99, scaleXsection);
  }
  if (getGraphs) { // get graphs from external file
    TFile* fext = new TFile (inFile, "read");
    observed = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_observed").c_str()));
    observedp = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_observedp").c_str()));
    observedm = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_observedm").c_str()));
    expected = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_expected").c_str()));
    expectedM1 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_expectedM1").c_str()));
    expectedP1 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_expectedP1").c_str()));
    expectedM2 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_expectedM2").c_str()));
    expectedP2 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_expectedP2").c_str()));
    observed1 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_observed1").c_str()));
    observed2 = dynamic_cast<TGraph*> (fext->FindObjectAny (string (option + "_observed2").c_str()));
				      
  }
  if (!observed) cout << "no 'observed' TGraph" << endl;
  if (!observedp) cout << "no 'observedp' TGraph" << endl;
  if (!observedm) cout << "no 'observedm' TGraph" << endl;
  if (!expected) cout << "no 'expected' TGraph" << endl;
  if (!expectedM1) cout << "no 'expectedM1' TGraph" << endl;
  if (!expectedP1) cout << "no 'expectedP1' TGraph" << endl;
  if (!expectedM2) cout << "no 'expectedM2' TGraph" << endl;
  if (!expectedP2) cout << "no 'expectedP2' TGraph" << endl;

  attObs.Copy(*observed);
  attObsTheo.Copy(*observedp);
  attObsTheo.Copy(*observedm);
  attExp.Copy(*expected);
  attExpSig.Copy(*expectedM1);
  attExpSig.Copy(*expectedP1);
  attExpSig2.Copy(*expectedM2);
  attExpSig2.Copy(*expectedP2);
  if (observed1) attObs1.Copy(*observed1);
  if (observed2) attObs2.Copy(*observed2);
  
}

TCanvas* drawRA7Plots () {
  //return 0;

  bool use2 = observed1 && observed2;
  double xMin = hFrame->GetXaxis()->GetXmin();
  double xMax = hFrame->GetXaxis()->GetXmax();
  double yMin = hFrame->GetYaxis()->GetXmin();
  double yMax = hFrame->GetYaxis()->GetXmax();

  TGraph* observedShadow = new TGraph (*observed);
  observedShadow->SetFillColor (kWhite);
  double graphXfirst,  graphYfirst;
  observed->GetPoint (0, graphXfirst, graphYfirst);
  double graphXlast,  graphYlast;
  observed->GetPoint (observed->GetN()-1, graphXlast, graphYlast);

  AppendGraph (observedShadow, graphXlast, yMin);
  AppendGraph (observedShadow, xMax, yMin);
  AppendGraph (observedShadow, xMax, yMax);
  AppendGraph (observedShadow, xMin, yMax);
  AppendGraph (observedShadow, xMin, yMin);
  AppendGraph (observedShadow, graphXfirst, yMin);

  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.20);

  TGraph* expectedPM = new TGraph (*expectedM1);
  AppendGraph (expectedPM, expectedP1, true);
  expectedPM->SetFillColor (expectedPM->GetLineColor());
  expectedPM->SetFillStyle (3002);
  TGraph* observedPM = new TGraph (*observedm);
  AppendGraph (observedPM, observedp, true);
  observedPM->SetFillColor (observedPM->GetLineColor());
  observedPM->SetFillStyle (3002);
  
  TGraph* expectedPM2 = new TGraph (*expectedM2);
  //TGraph* expectedPM2 = new TGraph (*expected);
  //AppendGraph (expectedPM2, expectedP2, true);
  AppendGraph (expectedPM2, expected, true);
  expectedPM2->SetFillColor (expectedPM2->GetLineColor());
  expectedPM2->SetFillStyle (3002);
  
  TBetterLegend* legend = new TBetterLegend(0.42,0.65,0.7,0.88,"  95% C.L. CLs NLO Exclusions","brNDC", fancy);
  legend->SetTextFont(42);
  legend->SetTextSize(0.035);
  ((TLegendEntry*)legend->GetListOfPrimitives()->At(0))->SetTextFont(42);
  ((TLegendEntry*)legend->GetListOfPrimitives()->At(0))->SetTextSize(0.035);
  legend->SetFillColor(0); 
  legend->SetShadowColor(0);
  legend->SetBorderSize(0);
  legend->AddBetterEntry(observed, observedp, label12);
  legend->AddBetterEntry(expected, expectedM1, labelExpected);
  if (draw2sigma) {
    legend->AddBetterEntry(expected, expectedM2, labelExpected2);
  }
  if (use2) {
    legend->AddBetterEntry(observed1, 0, label1);
    legend->AddBetterEntry(observed2, 0, label2);
  }

  

  TLatex *   cmsHeader = new TLatex(xMin + 0.05*(xMax-xMin), yMax + 0.04*(yMax-yMin),
       //	 "CMS Preliminary                    #sqrt{s} = 8 TeV, L_{int} = 9.2 fb^{-1}");
				    //  "CMS Preliminary                    #sqrt{s} = 8 TeV, L_{int} = 18.1 fb^{-1}");
  "CMS Preliminary                    L_{int} = 19.5 fb^{-1}, #sqrt{s} = 8 TeV");
  cmsHeader->SetTextFont(42);
  cmsHeader->SetTextSize(0.038);
  
  TPaveText *processText = new TPaveText(xMin+0.05*(xMax-xMin), yMin + 0.63*(yMax-yMin), 
					 xMin+0.25*(xMax-xMin), yMin + 0.99*(yMax-yMin), 
					 "br");
  processText->SetBorderSize(0);
  processText->SetFillColor(0);
  processText->SetTextAlign(12);
  processText->SetTextFont(42);
  processText->SetTextSize(0.04);
  if (processText1) processText->AddText(processText1);
  if (processText2) processText->AddText(processText2);
  if (processText3) processText->AddText(processText3);
  if (processText4) processText->AddText(processText4);

  TLatex* xslepText = 0;
  if (xslepLabel) {
    xslepText = new TLatex (xMin - 0.15*(xMax-xMin), yMin - 0.15*(yMax-yMin), xslepLabel);
    xslepText->SetTextFont(42);
    xslepText->SetTextSize(0.04);
  }


  double diagXmax = xMax;
  double diagYmax = xMax-diagonalOffset;
  if (diagYmax > yMax) {
    diagYmax = yMax;
    diagXmax = yMax+diagonalOffset;
  }
  TLine* diagonal = new TLine (xMin, xMin-diagonalOffset, diagXmax, diagYmax);
  diagonal->SetLineWidth(2);
  diagonal->SetLineColor(kBlack);
  diagonal->SetLineStyle(2);

  double xmh = xMin + 0.07*(xMax-xMin);
  double ymh = xmh - diagonalOffset + 0.08*(yMax-yMin);
  TLatex* mhierarchy = 0;
  if (massLabel) {
    mhierarchy = new TLatex(xmh, ymh, massLabel);
    mhierarchy->SetTextFont(42);
    mhierarchy->SetTextSize(0.04);
    mhierarchy->SetTextColor(kBlack);
    mhierarchy->SetTextAngle (massAngle);
  }
  
  TBox* fadeBox = new TBox (xMin, yMin+0.62*(yMax-yMin), xMax, yMax);
  fadeBox->SetFillColor(kWhite);
  fadeBox->SetLineColor(kBlack);
  fadeBox->SetLineWidth(3);


  // Draw everything
  TCanvas* c = new TCanvas("interpret", "interpret", 0, 0, canvasX, canvasY);
  c->SetLogz();
  hFrame->Draw ("axis");
  c->Draw();
  xSecObserved->Draw("samecolz");
  cout << "xSecObserved 3 5:1->" << xSecObserved->GetBinContent(5,1) << endl;
  if (fancy) {
    // all xSection by Ben's request 
    // observedShadow->Draw("F");  
  }
  if (mhierarchy) {
    diagonal->Draw();
    //mhierarchy->SetTextAngle(atan2(canvasY/(c->GetY2()-c->GetY1()), canvasX/(c->GetX2()-c->GetX1()))*180/3.1415);
//     mhierarchy->SetTextAngle(atan2((xSecObserved->GetYaxis()->GetXmax()-xSecObserved->GetYaxis()->GetXmin())*canvasY/(c->GetY2()-c->GetY1()), 
// 				   (xSecObserved->GetXaxis()->GetXmax()-xSecObserved->GetXaxis()->GetXmin())*canvasX/(c->GetX2()-c->GetX1()))*180/3.1415);
     double angle = atan2 (c->YtoPixel(diagonal->GetY1()) - c->YtoPixel(diagonal->GetY2()),
 			  c->XtoPixel(diagonal->GetX2()) - c->XtoPixel(diagonal->GetX1()))
       *180/3.1415;
     mhierarchy->SetTextAngle(angle);
     mhierarchy->Draw();
  }

  const char* graphStyle = "L";
  if (fancy) {
    if (draw2sigma) {
      //expectedPM2->Draw ("LF");
      (new TBetterGraph(*expectedM2))->Draw(graphStyle);
      //expectedP2->Draw(graphStyle);
    }
    //expectedPM->Draw ("LF");
    //expectedM1->Draw(graphStyle);
    //expectedP1->Draw(graphStyle);
    (new TBetterGraph(*expectedM1))->Draw(graphStyle);
    (new TBetterGraph(*expectedP1))->Draw(graphStyle);
    //observedPM->Draw ("LF");
    (new TBetterGraph(*observedp))->Draw(graphStyle);
    (new TBetterGraph(*observedm))->Draw(graphStyle);
    (new TBetterGraph(*expected))->Draw(graphStyle);
    if (use2) {
      (new TBetterGraph(*observed2))->Draw(graphStyle);
      (new TBetterGraph(*observed1))->Draw(graphStyle);
    }
    (new TBetterGraph(*observed))->Draw(graphStyle);
  }
  else {
    if (draw2sigma) {
      expectedM2->Draw(graphStyle);
      //expectedP2->Draw(graphStyle);
    }
    expectedM1->Draw(graphStyle);
    expectedP1->Draw(graphStyle);
    observedp->Draw(graphStyle);
    observedm->Draw(graphStyle);
    expected->Draw(graphStyle);
    if (use2) {
      observed2->Draw(graphStyle);
      observed1->Draw(graphStyle);
    }
    observed->Draw(graphStyle);
  }

  fadeBox->Draw("L");
  
  cmsHeader->Draw();
  legend->Draw();
  processText->Draw();
  if (xslepText) xslepText->Draw();

  hFrame->Draw ("sameaxis");
  c->Update();

  return c;
}
  
TCanvas* makeRA7Plot (const std::string& option, bool extGraphs = false) {
  getGraphs = extGraphs;
  TCanvas* result = 0; 
  reset ();
  if (option.find ("wz")  != string::npos) {
    draw2sigma = true;
    scaleXsection = 1;
    plotZoom = 450./400.;
    cleanOffset = 0;
    maxXbin = 27;
    xsecMin = 50;
    xsecMax = 4000;
    file12 = "hists/TChiWZ.2l2jucla.root";
    file1 = "hists/TChiWZ.ucla.root";
    file2 = "hists/TChiWZ.2l2j.root";
    processText1 = "#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}";
    processText2 = "#tilde{#chi}_{2}^{0} #rightarrow Z #tilde{#chi}_{1}^{0}";
    processText3 = "#tilde{#chi}_{1}^{#pm} #rightarrow W #tilde{#chi}_{1}^{0}";
    labelX = "m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}}=m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}}  (GeV)";
    labelY = "m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}  (GeV)";;
    label12 = "Observed 2#font[12]{l}2#font[12]{j}#oplus 3#font[12]{l} #pm1#sigma_{theory}";
    labelExpected = "Expected 2#font[12]{l}2#font[12]{j}#oplus 3#font[12]{l} #pm1#sigma_{experiment}";
    labelExpected2 = "Expected 2#font[12]{l}2#font[12]{j}#oplus 3#font[12]{l} -2#sigma_{experiment}";
    label1 = "Observed 3#font[12]{l} only";
    label2 = "Observed 2#font[12]{l}2#font[12]{j} only";
    massLabel = "m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}} - m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}} = m_{#scale[1.2]{Z}}";
    diagonalOffset = 91;
    if (option.find ("hig")  != string::npos) {
      scaleXsection = 0.05;
      processText1 = "#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm} (#tilde{h}-like)";
    }
    if (option.find ("3l")  != string::npos) {
      file12 = file1;
      file1 = file2 = 0;
      label12 = label1;
      label1 = label2 = 0;
      labelExpected = "Expected 3#font[12]{l} #pm1#sigma_{experiment}";
      labelExpected2 = "Expected 3#font[12]{l} -2#sigma_{experiment}";
    }
    else if (option.find ("2l2j")  != string::npos) {
      draw2sigma = false;
      file12 = file2;
      file1 = file2 = 0;
      label12 = label2;
      label1 = label2 = 0;
      labelExpected = "Expected 2#font[12]{l}2#font[12]{j} #pm1#sigma_{experiment}";
      labelExpected2 = 0;
    }

    makeObjects (option);
    //    AppendGraph (expectedP1, 400, 160);
    // AppendGraph (expectedP1, 400, 0);
//     PrependGraph (observed, 100, 5);
//     PrependGraph (observed2, 168, 0);
    result = drawRA7Plots ();
  }
  if (option.find ("wh")  != string::npos) {
    draw2sigma = false;
    scaleXsection = 1;
    plotZoom = 400./400.;
    cleanOffset = 0;
    maxXbin = 27;
    xsecMin = 50;
    xsecMax = 4000;
    file12 = "hists/TChiWH.root";
    processText1 = "#it{pp} #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}";
    processText2 = "#tilde{#chi}_{2}^{0} #rightarrow H #tilde{#chi}_{1}^{0}";
    processText3 = "#tilde{#chi}_{1}^{#pm} #rightarrow W #tilde{#chi}_{1}^{0}";
    labelX = "m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}}=m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}}  (GeV)";
    labelY = "m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}  (GeV)";;
    label12 = "Observed #pm1#sigma_{theory}";
    labelExpected = "Expected #pm1#sigma_{experiment}";
    massLabel = "m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}} - m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}} = m_{#scale[1.2]{H}}";
    diagonalOffset = 125;

    makeObjects (option);
    //    AppendGraph (expectedP1, 400, 160);
    // AppendGraph (expectedP1, 400, 0);
//     PrependGraph (observed, 100, 5);
//     PrependGraph (observed2, 168, 0);
    result = drawRA7Plots ();
  }
  else if (option.find ("2")  != string::npos) {
    bool b2i =  option.find ("2i") != string::npos;
    bool b2a =  option.find ("2a") != string::npos;
    bool b005 =  option.find ("0.05") != string::npos;
    bool b095 =  option.find ("0.95") != string::npos;
    bool b05 =  option.find ("0.5") != string::npos;
    bool bss = option.find ("ss") != string::npos;
    bool b3l = option.find ("3l") != string::npos;
    scaleXsection = b2i ? 0.5 : 1;
    plotZoom = bss ? 
      b2i || b2a ? 800./600. : 800./350. 
      //      : b2i || b2a ? 1000./800. : 1000./350.;
      : b2i || b2a ? 800./800. : 500./500.;
    cleanOffset = b2i && b05 ? 0 : 0;
    maxXbin = 99;
    xsecMin = b2i || b2a ? 0.5 : 5;
    xsecMax = b2i || b2a ? 1e3 : 1e4;
    file12 = 
      bss ? (
	     b2i ? ( 
		    b05 ? "hists/TChiSlepSnu_2i_0.5.SS.root" : b095 ? "hists/TChiSlepSnu_2i_0.95.SS.root": "hists/TChiSlepSnu_2i_0.05.SS.root")
	     : b2a ? (
		      b05 ? "hists/TChiSlepSnu_2a_0.5.SS.root" : b095 ? "hists/TChiSlepSnu_2a_0.95.SS.root": "hists/TChiSlepSnu_2a_0.05.SS.root")
	     : 0
	     )
      : 
      b3l ? (
	     b2i ? ( 
		    b05 ? "hists/TChiSlepSnu_2i_0.5.ucla.root" : b095 ? "hists/TChiSlepSnu_2i_0.95.ucla.root": "hists/TChiSlepSnu_2i_0.05.ucla.root")
	     : b2a ? (
		      b05 ? "hists/TChiSlepSnu_2a_0.5.ucla.root" : b095 ? "hists/TChiSlepSnu_2a_0.95.ucla.root": "hists/TChiSlepSnu_2a_0.05.ucla.root")
	     : 0
	     )
      : (
	 b2i ? ( 
		(b05 ? "hists/TChiSlepSnu_2i_0.5.ucla.root" : b095 ? "hists/TChiSlepSnu_2i_0.95.uclaSS.root": "hists/TChiSlepSnu_2i_0.05.uclaSS.root")
		)
	 : b2a ? (
		  b05 ? "hists/TChiSlepSnu_2a_0.5.ucla.root" : b095 ? "hists/TChiSlepSnu_2a_0.95.uclaSS.root": "hists/TChiSlepSnu_2a_0.05.uclaSS.root"
		  )
	 : "hists/TChiStauSnu_0.5.ucla.root"
	 );
    file1 = 
      bss || b3l ? 0 :
      b2i ? 
      (b05 ? 0 : b095 ? "hists/TChiSlepSnu_2i_0.95.ucla.root": "hists/TChiSlepSnu_2i_0.05.ucla.root")
      : b2a ?
      (b05 ? 0 : b095 ? "hists/TChiSlepSnu_2a_0.95.ucla.root": "hists/TChiSlepSnu_2a_0.05.ucla.root")
      : 0;
    file2 = 
      bss | b3l ? 0 :
      b2i ? 
      (b05 ? 0 : b095 ? "hists/TChiSlepSnu_2i_0.95.SS.root": "hists/TChiSlepSnu_2i_0.05.SS.root")
      : b2a ?
      (b05 ? 0 : b095 ? "hists/TChiSlepSnu_2a_0.95.SS.root": "hists/TChiSlepSnu_2a_0.05.SS.root")
      : 0;
    processText1 = "#it{pp} #rightarrow #tilde{#chi}_{1}^{#pm} #tilde{#chi}_{2}^{0}";
    processText2 = 
      b2i ? "#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#font[12]{e}}#nu_{#font[12]{e}}, #tilde{#mu}#nu_{#mu}, #tilde{#tau}#nu_{#tau}"
      : b2a ? "#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#tau}#nu_{#tau}"
      : "#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#tau}#nu_{#tau}"; 
    processText3 = 
      b2i || b2a ? "#tilde{#chi}_{2}^{0} #rightarrow #tilde{#font[12]{e}}#font[12]{e}, #tilde{#mu}#mu, #tilde{#tau}#tau"
      : "#tilde{#chi}_{2}^{0} #rightarrow #tilde{#tau}#tau";
    processText4 = b2i ? "#it{Br}(#tilde{#chi}_{2}^{0} #rightarrow #font[12]{#tilde{l}^{+}l^{-}}) = 0.5"
      : b2a ? "#it{Br}(#tilde{#chi}_{2}^{0} #rightarrow #font[12]{#tilde{l}^{+}l^{-}}) = 1"
      : 0;
    labelX = "m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}}=m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}}  (GeV)";
    labelY = "m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}  (GeV)";
    label12 = 
      bss || b3l ? "Observed #pm1#sigma_{theory}"
      : !b05 ? "Observed 3#font[12]{l}#oplusSS #pm1#sigma_{theory}"
      : "Observed 3#font[12]{l} #pm1#sigma_{theory}";
    labelExpected = 
      bss || b3l ? "Expected #pm 1#sigma_{experiment}"
      : !b05 ? "Expected 3#font[12]{l}#oplusSS #pm1#sigma_{experiment}"
      : "Expected 3#font[12]{l} #pm1#sigma_{experiment}";
    label1 = b05 || bss || b3l ? 0 : "Observed 3#font[12]{l} only";
    label2 = b05 || bss || b3l ? 0 :  "Observed SS only";
    massLabel = b2i || b2a ? "m_{#scale[1.2]{#tilde{#chi}_{2}^{0}}} = m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} > m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}" : 0;
    xslepLabel = b05 ? "m_{ #scale[1.2]{#tilde{#font[12]{l}}}} = 0.5m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} + 0.5m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}"
      : b005 ? "m_{ #scale[1.2]{#tilde{#font[12]{l}}}} = 0.05m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} + 0.95m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}"
      : "m_{ #scale[1.2]{#tilde{#font[12]{l}}}} = 0.95m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} + 0.05m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}";
    diagonalOffset = 0;
    makeObjects (option);
    result = drawRA7Plots ();
  }
  else if (option == "chipm") {
    //draw2sigma = true;
    cleanOffset = 101;
    xsecMin = 1;
    xsecMax = 100;
    file12 = "hists/TChipmSlepSnu.root";
//     processText1 = "#it{pp} #rightarrow #tilde{#chi}_{1}^{+} #tilde{#chi}_{1}^{-}";
//     processText2 = "#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#font[12]{e}}#font[12]{e}, #tilde{#mu}#mu, #tilde{#tau}#tau";
    processText1 = "#it{pp} #rightarrow #tilde{#chi}_{1}^{+} #tilde{#chi}_{1}^{-}";
    processText2 = "#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#font[12]{l}}^{#pm}#font[12]{#nu_{l}}, #tilde{#font[12]{#nu_{l}}}#font[12]{l^{#pm}}";
    processText3 = "#tilde{#font[12]{l}}^{#pm} #rightarrow #font[12]{l^{#pm}}#tilde{#chi}_{1}^{0},  #tilde{#font[12]{#nu}} #rightarrow #font[12]{#nu}#tilde{#chi}_{1}^{0}";
    labelX = "m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}}  (GeV)";
    labelY = "m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}  (GeV)";
    label12 = "Observed #pm1#sigma_{theory}";
    labelExpected = "Expected #pm1#sigma_{experiment}";
    massLabel = "m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} > m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}";
    xslepLabel = "m_{ #scale[1.2]{#tilde{#font[12]{l}}}} = m_{ #scale[1.2]{#tilde{#font[12]{#nu}}}} = 0.5m_{#scale[1.2]{#tilde{#chi}_{1}^{#pm}}} + 0.5m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}},  (#font[12]{l} = e, #mu, #tau)";
    diagonalOffset = 0;
    //logSmoothBypass = true;
    makeObjects (option);
    //PrependGraph (observed, 180, 0);
    //observed->SetPoint (observed->GetN(), 450, 0);
    //observedp->SetPoint (observedp->GetN(), 455, 0);
    //AppendGraph (expectedP1, 460, 0);
//     observed->RemovePoint(0);
//     observedp->RemovePoint(0);
//     observedm->RemovePoint(0);
//     expected->RemovePoint(0);
//     expectedM1->RemovePoint(0);
//     expectedP1->RemovePoint(0);
    result = drawRA7Plots ();
  }
   else if (option == "slep") {
    cleanOffset = 51;
    xsecMin = 1;
    xsecMax = 100;
    file12 = "hists/TSlepSlep.root";
    processText1 = "#it{pp} #rightarrow #tilde{#font[12]{e}}_{L} #tilde{#font[12]{e}}_{L},  #tilde{#mu}_{L} #tilde{#mu}_{L}";
    processText2 = "#it{Br}(#tilde{#font[12]{l}}_{L} #rightarrow #font[12]{l} #tilde{#chi}_{1}^{0}) = 1";
    labelX = "m_{ #scale[1.2]{#tilde{#font[12]{l}}}}  (GeV)";
    labelY = "m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}  (GeV)";
    label12 = "Observed #pm 1#sigma_{theory}";
    labelExpected = "Expected #pm1#sigma_{experiment}";
    //massLabel = "m_{ #scale[1.2]{#tilde{#font[12]{l}}}} > m_{#scale[1.2]{#tilde{#chi}_{1}^{0}}}";
    diagonalOffset = 0;
    //logSmoothBypass = true;
    makeObjects (option);
//     PrependGraph (observed, 112.5, 0);
//     PrependGraph (observedp, 112.5, 0);
//     PrependGraph (observedm, 112.5, 0);
//     PrependGraph (expected, 120, 0);
//     PrependGraph (expectedM1, 125, 0);
//     PrependGraph (expectedP1, 112.5, 0);
    result = drawRA7Plots ();
   }
  dumpGraph (observed, option);
  string scriptName = "scripts/graphs_" + option + ".C";
  if (!getGraphs) printGraphs (scriptName.c_str());
  return result;
 }

void dumpRA7Plots () {
  makeRA7Plot ("wz");
  //makeRA7Plot ("wz_hig");
  makeRA7Plot ("2i_0.5");
  makeRA7Plot ("2i_0.05");
  makeRA7Plot ("2i_0.95");
  makeRA7Plot ("2a_0.5");
  makeRA7Plot ("2a_0.05");
  makeRA7Plot ("2a_0.95");
  makeRA7Plot ("2b_0.5");
  makeRA7Plot ("chipm");
  makeRA7Plot ("slep");
}

void makeRA7EPS() {
  makeRA7Plot ("wz")->SaveAs ("newfiles/exclusion_TChiWZ.eps");
  //makeRA7Plot ("wz_hig")->SaveAs ("newfiles/exclusion_TChiWZ_hig.eps");
  makeRA7Plot ("2i_0.5")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_5.eps");
  makeRA7Plot ("2i_0.05")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_05.eps");
  makeRA7Plot ("2i_0.95")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_95.eps");
  makeRA7Plot ("2a_0.5")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_5.eps");
  makeRA7Plot ("2a_0.05")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_05.eps");
  makeRA7Plot ("2a_0.95")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_95.eps");
  makeRA7Plot ("2b_0.5")->SaveAs ("newfiles/exclusion_TChiStauSnu_0_5.eps");
  makeRA7Plot ("chipm")->SaveAs ("newfiles/exclusion_TChipmSlepSnu.eps");
  makeRA7Plot ("slep")->SaveAs ("newfiles/exclusion_TSlepSlep.eps");
}

void makeRA7PDF() {
  makeRA7Plot ("wz")->SaveAs ("newfiles/exclusion_TChiWZ.pdf");
  //makeRA7Plot ("wz_hig")->SaveAs ("newfiles/exclusion_TChiWZ_hig.pdf");
  makeRA7Plot ("2i_0.5")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_5.pdf");
  makeRA7Plot ("2i_0.05")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_05.pdf");
  makeRA7Plot ("2i_0.95")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_95.pdf");
  makeRA7Plot ("2a_0.5")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_5.pdf");
  makeRA7Plot ("2a_0.05")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_05.pdf");
  makeRA7Plot ("2a_0.95")->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_95.pdf");
  makeRA7Plot ("2b_0.5")->SaveAs ("newfiles/exclusion_TChiStauSnu_0_5.pdf");
  makeRA7Plot ("chipm")->SaveAs ("newfiles/exclusion_TChipmSlepSnu.pdf");
  makeRA7Plot ("slep")->SaveAs ("newfiles/exclusion_TSlepSlep.pdf");
}

void makeRA7Root(bool extGraphs = false) {
  getGraphs = extGraphs;
  makeRA7Plot ("wz", extGraphs)->SaveAs ("newfiles/exclusion_TChiWZ.root");
  makeRA7Plot ("wz_3l", extGraphs)->SaveAs ("newfiles/exclusion_TChiWZ_3l.root");
  makeRA7Plot ("wz_2l2j", extGraphs)->SaveAs ("newfiles/exclusion_TChiWZ_2l2j.root");
  //makeRA7Plot ("wz_hig")->SaveAs ("newfiles/exclusion_TChiWZ_hig.root");
  makeRA7Plot ("2i_0.5", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_5.root");
  makeRA7Plot ("2i_0.05", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_05.root");
  makeRA7Plot ("2i_0.95", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2i_0_95.root");
  makeRA7Plot ("2a_0.5", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_5.root");
  makeRA7Plot ("2a_0.05", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_05.root");
  makeRA7Plot ("2a_0.95", extGraphs)->SaveAs ("newfiles/exclusion_TChiSlepSnu_2a_0_95.root");
  makeRA7Plot ("2b_0.5", extGraphs)->SaveAs ("newfiles/exclusion_TChiStauSnu_0_5.root");
  makeRA7Plot ("chipm", extGraphs)->SaveAs ("newfiles/exclusion_TChipmSlepSnu.root");
  makeRA7Plot ("slep", extGraphs)->SaveAs ("newfiles/exclusion_TSlepSlep.root");
}

void makeRA7SSPlots() {
  makeRA7Plot ("ss_2i_0.5")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2i_0_5.pdf");
  makeRA7Plot ("ss_2i_0.05")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2i_0_05.pdf");
  makeRA7Plot ("ss_2i_0.95")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2i_0_95.pdf");
  makeRA7Plot ("ss_2a_0.5")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2a_0_5.pdf");
  makeRA7Plot ("ss_2a_0.05")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2a_0_05.pdf");
  makeRA7Plot ("ss_2a_0.95")->SaveAs ("newfiles/exclusion_SS_TChiSlepSnu_2a_0_95.pdf");
}

