
TGraph* makeCurveUp (TH2D* hist, 
		     int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale = 1.);

TGraph* makeCurveDown (TH2D* hist, 
		       int binXfirst, int binXlast, int binYfirst, int binYlast, 
		       double scale = 1.);

TGraph* makeCurveUpDown (TH2D* hist, 
		       int binXfirst, int binXlast, int binYfirst, int binYlast, 
		       double scale = 1.);

TGraph* makeCurveLeft (TH2D* hist, 
		       int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale = 1.);

TGraph* makeCurveRight (TH2D* hist, 
			int binXfirst, int binXlast, int binYfirst, int binYlast, 
			double scale = 1.);

void PrependGraph (TGraph* graph, double x, double y);
void PrependGraph (TGraph* graph, TGraph* anotherGraph, bool reverse = false);

void AppendGraph (TGraph* graph, double x, double y);
void AppendGraph (TGraph* graph, TGraph* anotherGraph, bool reverse = false);

void swap (double& x, double& y) {
  double buf = x;
  x = y;
  y = buf;
}

void orderInY (std::vector<double>& xx, std::vector<double>& yy) {
  while (1) {
    unsigned i = 0;
    for (; i < yy.size(); ++i) {
      if ((i > 0) && (yy[i] < yy[i-1])) {
	swap (yy[i], yy[i-1]);
	swap (xx[i], xx[i-1]);
	break;
      }
    }
    if (i == yy.size()) return;
  }
}

void orderInYInv (vector<double>& xx, vector<double>& yy) {
  while (1) {
    unsigned i = 0;
    for (; i < yy.size(); ++i) {
      if ((i > 0) && (yy[i] > yy[i-1])) {
	swap (yy[i], yy[i-1]);
	swap (xx[i], xx[i-1]);
	break;
      }
    }
    if (i == yy.size()) return;
  }
}

void orderInX (vector<double>& xx, vector<double>& yy) {
  while (1) {
    unsigned i = 0;
    for (; i < yy.size(); ++i) {
      if ((i > 0) && (xx[i] < xx[i-1])) {
	swap (yy[i], yy[i-1]);
	swap (xx[i], xx[i-1]);
	break;
      }
    }
    if (i == yy.size()) return;
  }
}

void orderInDir (vector<double>& xx, vector<double>& yy) {
  while (1) {
    unsigned i = 1;
    for (; i < yy.size(); ++i) {
      if (fabs(xx[i] - xx[i-1]) < fabs(yy[i] - yy[i-1])) { // order in y
	if (yy[i] > yy[i-1]) {
	  swap (yy[i], yy[i-1]);
	  swap (xx[i], xx[i-1]);
	  break;
	}
      }
      else if (fabs(xx[i] - xx[i-1]) >= fabs(yy[i] - yy[i-1])) { // order in x
	if (xx[i] < xx[i-1]) {
	  swap (yy[i], yy[i-1]);
	  swap (xx[i], xx[i-1]);
	  break;
	}
      }
    }
    if (i == yy.size()) return;
  }
}

TH2D* makeLogSmooth (TH2D* hist, bool pass = false, double cleanOffset = 0) {
  TH2D* newHist = new TH2D (*hist);
  newHist->SetName("newHist");
  int nx = newHist->GetXaxis()->GetNbins();
  int ny = newHist->GetYaxis()->GetNbins();
  if (pass) { //only clean diagonal
      for (int ix = 1; ix <= newHist->GetXaxis()->GetNbins(); ++ix) {
	for (int iy = 1; iy <= newHist->GetYaxis()->GetNbins(); ++iy) {
	  if (newHist->GetYaxis()->GetBinCenter (iy) >= newHist->GetXaxis()->GetBinCenter (ix) - cleanOffset) {
	    newHist->SetBinContent(ix,iy, 0);
	  }
	}
      }
  }

  else {
    for (int ix = 1; ix <= nx; ++ix) {
      for (int iy = 1; iy <= ny; ++iy) {
	double bc = newHist->GetBinContent(ix, iy);
	if (bc > 0) newHist->SetBinContent(ix,iy, log(bc));
	else if (ix < nx && iy > 1) {
	  double bc1 = hist->GetBinContent(ix+1, iy);
	  double bc2 = hist->GetBinContent(ix, iy-1);
	  double bc3 = hist->GetBinContent(ix+1, iy-1);
	  if (bc1 > 0 && bc2 > 0 && bc3 > 0) {
	    newHist->SetBinContent(ix,iy, log(bc1)+log(bc2)-log(bc3));
	  }
	  if (ix < nx-1 && iy < ny) {
	    double bc4 = hist->GetBinContent(ix+2, iy-1);
	    if (bc4 > 0) {
	      newHist->SetBinContent(ix,iy+1, 2*log(bc1)-log(bc4));
	    }
	  }
	}
      }
    }
    for (int ix = 1; ix <= nx; ++ix) {
      for (int iy = ny; iy >= 1; --iy) {
	double bc = newHist->GetBinContent(ix, iy);
	if (bc != 0) continue;
	if (ix > 1 && iy < ny) {
	  double bc1 = newHist->GetBinContent(ix-1, iy);
	  double bc2 = newHist->GetBinContent(ix, iy+1);
	  double bc3 = newHist->GetBinContent(ix-1, iy+1);
	  if (bc1 != 0 && bc2 != 0 && bc3 != 0) {
	    newHist->SetBinContent(ix,iy, bc1+bc2-bc3);
	  }
	}
      }
    }
    newHist->Smooth();
    for (int ix = 1; ix <= newHist->GetXaxis()->GetNbins(); ++ix) {
      for (int iy = 1; iy <= newHist->GetYaxis()->GetNbins(); ++iy) {
	double bc = newHist->GetBinContent(ix, iy);
	if (bc != 0 && 
	    newHist->GetXaxis()->GetBinCenter (ix) > newHist->GetYaxis()->GetBinCenter (iy) + cleanOffset
	    ) newHist->SetBinContent(ix,iy, exp(bc));
	else newHist->SetBinContent(ix,iy, 0);
	//if (hist->GetBinContent(ix,iy) >0) newHist->SetBinContent(ix,iy, exp(bc));
	//else newHist->SetBinContent(ix,iy, 0);
      }
    }
  }
  return newHist;
}


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

TGraph* makeCurve (TH2D* hist, double scale = 0.19) {
  double logScale = log(scale);
  int nBinsX = hist->GetNbinsX();
  int nBinsY = hist->GetNbinsY();
  vector<double> xx;
  vector<double> yy;
  int minIx = 12;
  for (int ix = nBinsX; ix >= minIx; --ix) {
    double x = hist->GetXaxis()->GetBinCenter (ix);
    //    cout << "x bin: " << ix << ' ' << x << endl;
    double logyprev = 2*log (hist->GetBinContent(ix,2)) - log(hist->GetBinContent(ix,1));
    logyprev = logyprev - logScale;
    int iy = 1;
    for (; iy <= nBinsY; ++iy) {
      if (hist->GetBinContent(ix,iy) <= 0) continue;
      double logy = log (hist->GetBinContent(ix,iy)) - logScale;
      // cout << "next scan point " <<ix << ':'<<iy<<"  "<<hist->GetBinContent(ix,iy)<<'/'<<logy<<endl;
      if (logy > 0) {
	if (iy > 1) {
	  //cout << "y bin: " << iy << ' ' << hist->GetYaxis()->GetBinCenter (iy) << endl;
	  double y = hist->GetYaxis()->GetBinCenter (iy) - 
	    (hist->GetYaxis()->GetBinCenter (iy-1)-hist->GetYaxis()->GetBinCenter (iy))*
	    logy/(logyprev-logy);
	  xx.push_back(x);
	  yy.push_back(y);
  	  // cout << "new point 1 " << x << ':' << y 
  	       // << " (" << hist->GetYaxis()->GetBinCenter (iy) << ':' << logy << ","
  	       // << hist->GetYaxis()->GetBinCenter (iy+1) << ':' << logyprev << ")"
  	       // << endl;
	}
	break;
      }
      logyprev = logy;
    }
  }

  orderInX (xx, yy);

  TGraph* result = new TGraph (xx.size(), &(xx[0]), &(yy[0]));
  result->SetLineWidth(3);
  result->SetMarkerStyle(8);
  result->SetMarkerSize (1.0);
  return result;
}

TGraph* makeCurveUp (TH2D* hist, 
		     int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale) {
  bool verbose = true;
  double logScale = log(scale);
  int binsX = hist->GetNbinsX();
  int binsY = hist->GetNbinsY();

  if (binXlast > binsX) binXlast = binsX;
  if (binYlast > binsY) binYlast = binsY;
  vector<double> xx;
  vector<double> yy;
  for (int ix = binXfirst; ix <= binXlast; ++ix) {
    double x = hist->GetXaxis()->GetBinCenter (ix);
    if(verbose) cout << "x bin: " << ix << ' ' << x << endl;
    int iy = binYfirst;
    while (iy < binYlast && hist->GetBinContent(ix,iy) <= 0) ++iy;
    if (hist->GetBinContent(ix,iy) <= 0 || hist->GetBinContent(ix,iy+1) <= 0) {
      if(verbose) cout << "hist->GetBinContent(ix,iy) <= 0 || hist->GetBinContent(ix,iy+1) <= 0 "<<iy<<' '<<hist->GetBinContent(ix,iy)<<' '<<hist->GetBinContent(ix,iy+1)<<endl;
      continue;
    }
    double logyprev = (hist->GetBinContent(ix,iy+1) > hist->GetBinContent(ix,iy)) ? 
      2*log (hist->GetBinContent(ix,iy)) - log(hist->GetBinContent(ix,iy+1))
      : log (hist->GetBinContent(ix,iy));
    logyprev = logyprev - logScale;
    if(verbose)     cout << "makeCurveUp starting point " <<ix << ':'<<iy<<"  "<<hist->GetBinContent(ix,iy)<<'/'<<logyprev<<endl;
    if (logyprev > 0) {
      if(verbose) cout <<"logyprev > 0 "<<logyprev+logScale<<' '<<logyprev<<endl;
      continue;
    }
    for (; iy <= binYlast; ++iy) {
      if (hist->GetBinContent(ix,iy) <= 0) continue;
      double logy = log (hist->GetBinContent(ix,iy)) - logScale;
      if(verbose) cout << "makeCurveUp next scan point " <<ix << ':'<<iy<<"  "<<hist->GetBinContent(ix,iy)<<'/'<<logy<<endl;
      if (logy > 0 && (iy < binsY && (hist->GetBinContent(ix,iy+1) <= 0 || log (hist->GetBinContent(ix,iy+1)) - logScale) > 0)) {
	if (iy > binYfirst) {
	  if(verbose) cout << "y bin: " << iy << ' ' << hist->GetYaxis()->GetBinCenter (iy) << endl;
	  double y = hist->GetYaxis()->GetBinCenter (iy-1) + 
	    (hist->GetYaxis()->GetBinCenter (iy)-hist->GetYaxis()->GetBinCenter (iy-1))*
	    (1.-logy/(logy-logyprev));
 	  if(verbose) cout << "ytest "
	  << " " << hist->GetYaxis()->GetBinCenter (iy-1)
	     << " " << hist->GetYaxis()->GetBinCenter (iy)
	     << " " << hist->GetYaxis()->GetBinCenter (iy)-hist->GetYaxis()->GetBinCenter (iy-1)
	     << " " << logy
	     << " " << logyprev
	     << " " << logy-logyprev 
	     << " " << endl;
	  xx.push_back(x);
	  yy.push_back(y);
	  if(verbose) cout << "makeCurveUp new point 1 " << x << ':' << y 
	      << " (" << hist->GetYaxis()->GetBinCenter (iy-1) << ':' << logyprev << ","
	      << hist->GetYaxis()->GetBinCenter (iy) << ':' << logy << ")"
	      << endl;
	}
	break;
      }
      else if (logy <= 0 && hist->GetBinContent(ix,iy+1) <= 0) {
	double y = hist->GetYaxis()->GetBinUpEdge (iy);
	xx.push_back(x);
	yy.push_back(y);
	  if(verbose) cout << "makeCurveUp new point on the edge " << x << ':' << y 
	      << " (" << hist->GetYaxis()->GetBinCenter (iy) << ':' << logy << ","
	      << hist->GetYaxis()->GetBinUpEdge (iy) << ")"
	      << endl;
	break;
      }
      logyprev = logy;
    }
  }

  orderInX (xx, yy);

  TGraph* result = xx.size() > 0 ? new TGraph (xx.size(), &(xx[0]), &(yy[0])) : new TGraph();
  if(verbose) dumpGraph (result, "makeCurveUp");
  result->SetLineWidth(3);
  result->SetMarkerStyle(8);
  result->SetMarkerSize (1.0);
  return result;
}

TGraph* makeCurveDown (TH2D* hist, 
		     int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale) {
  double logScale = log(scale);
  int binsX = hist->GetNbinsX();
  int binsY = hist->GetNbinsY();

  if (binXlast > binsX) binXlast = binsX;
  if (binYlast > binsY) binYlast = binsY;
  vector<double> xx;
  vector<double> yy;
  for (int ix = binXfirst; ix <= binXlast; ++ix) {
    double x = hist->GetXaxis()->GetBinCenter (ix);
    //    cout << "x bin: " << ix << ' ' << x << endl;
    bool initedY = false;
    double logyprev = 0;
    int iy = binYlast;
    for (; iy >= binYfirst; --iy) {
      if (hist->GetBinContent(ix,iy) <= 0) continue;
      if (!initedY) {
	if (hist->GetBinContent(ix,iy-1) <= 0) continue;
	logyprev = 2*log (hist->GetBinContent(ix,iy)) - log(hist->GetBinContent(ix,iy-1));
	logyprev = logyprev - logScale;
	if (logyprev > 0) break;
	initedY = true;
      }
      double logy = log (hist->GetBinContent(ix,iy)) - logScale;
      // cout << "makeCurveDown next scan point " <<ix << ':'<<iy<<"  "<<hist->GetBinContent(ix,iy)<<'/'<<logy<<endl;
      if (logy > 0) {
	cout << "y bin: " << iy << ' ' << hist->GetYaxis()->GetBinCenter (iy) << endl;
	double y = hist->GetYaxis()->GetBinCenter (iy) + 
	  (hist->GetYaxis()->GetBinCenter (iy)-hist->GetYaxis()->GetBinCenter (iy+1))*
	  logy/(logy-logyprev);
	// 	  cout << "ytest "
	// 	       << " " << hist->GetYaxis()->GetBinCenter (iy-1)
	// 	       << " " << hist->GetYaxis()->GetBinCenter (iy)
	// 	       << " " << hist->GetYaxis()->GetBinCenter (iy)-hist->GetYaxis()->GetBinCenter (iy-1)
	// 	       << " " << logy
	// 	       << " " << logyprev
	// 	       << " " << logy-logyprev 
	// 	       << " " << endl;
	xx.push_back(x);
	yy.push_back(y);
	// cout << "makeCurveDown new point 1 " << x << ':' << y 
	//      << " (" << hist->GetYaxis()->GetBinCenter (iy+1) << ':' << logyprev << ","
	//      << hist->GetYaxis()->GetBinCenter (iy) << ':' << logy << ")"
	//      << endl;
	break;
      }
      logyprev = logy;
    }
  }

  orderInX (xx, yy);

  TGraph* result = new TGraph (xx.size(), &(xx[0]), &(yy[0]));
  result->SetLineWidth(3);
  result->SetMarkerStyle(8);
  result->SetMarkerSize (1.0);
  return result;
}

TGraph* makeCurveRight (TH2D* hist, 
		     int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale) {
  bool verbose = false;
  double logScale = log(scale);
  int binsX = hist->GetNbinsX();
  int binsY = hist->GetNbinsY();

  if (binXlast > binsX) binXlast = binsX;
  if (binYlast > binsY) binYlast = binsY;
  vector<double> xx;
  vector<double> yy;
  for (int iy = binYfirst; iy <= binYlast; ++iy) {
    double y = hist->GetYaxis()->GetBinCenter (iy);
    if (verbose)   cout << "y bin: " << iy << ' ' << y << endl;
    bool initedX = false;
    double logyprev = 0;
    int ix = binXfirst;
    for (; ix <= binXlast; ++ix) {
      if (hist->GetBinContent(ix,iy) <= 0) {
	continue;
      }
      if (!initedX) {
	if (ix > 1 && hist->GetBinContent(ix-1,iy) > 0) {
	  logyprev = log (hist->GetBinContent(ix-1,iy));
	}
	else if (hist->GetBinContent(ix+1,iy) > 0){
	  logyprev = 2*log (hist->GetBinContent(ix,iy)) - log(hist->GetBinContent(ix+1,iy));
	}
	else {
	  continue;
	}
	logyprev = logyprev - logScale;
	if (logyprev > 0) continue;
	initedX = true;
      }
      double logy = log (hist->GetBinContent(ix,iy)) - logScale;
      if (verbose) cout << "makeCurveRight next scan point " 
			<<ix<< '/' << binsX << '/' << binXlast << ':'
			<<iy<< '/' << binsY <<"  "
			<<hist->GetBinContent(ix,iy)<<'/'<<logy<<endl;
      if (ix == binsX || ((logy > 0) && (log (hist->GetBinContent(ix+1,iy)) - logScale) > 0)) {
	if (verbose) cout << "x bin: " << ix << ' ' << hist->GetXaxis()->GetBinCenter (ix) << endl;
	double x = 0;
	if (ix == binsX && logy <= 0) {
	  x = 2 * hist->GetXaxis()->GetBinCenter (binsX) - hist->GetXaxis()->GetBinCenter (binsX-1);
	}
	else {
	  x = hist->GetXaxis()->GetBinCenter (ix-1) + 
	    (hist->GetXaxis()->GetBinCenter (ix)-hist->GetXaxis()->GetBinCenter (ix-1))*
	    (1.-logy/(logy-logyprev));
	}
	xx.push_back(x);
	yy.push_back(y);
	if (verbose) cout << "makeCurveRight new point 1 " << x << ':' << y 
			  << " (" << hist->GetXaxis()->GetBinCenter (ix-1) << ':' << logyprev << ","
			  << hist->GetXaxis()->GetBinCenter (ix) << ':' << logy << ")"
			  << endl;
	break;
      }
      logyprev = logy;
    }
  }

  orderInYInv (xx, yy);

  TGraph* result = xx.size() > 0 ? new TGraph (xx.size(), &(xx[0]), &(yy[0])) : new TGraph ();
  result->SetLineWidth(3);
  result->SetMarkerStyle(8);
  result->SetMarkerSize (1.0);
  return result;
}

TGraph* makeCurveLeft (TH2D* hist, 
		     int binXfirst, int binXlast, int binYfirst, int binYlast, 
		     double scale) {
  double logScale = log(scale);
  int binsX = hist->GetNbinsX();
  int binsY = hist->GetNbinsY();

  if (binXlast > binsX) binXlast = binsX;
  if (binYlast > binsY) binYlast = binsY;
  vector<double> xx;
  vector<double> yy;
  for (int iy = binYfirst; iy <= binYlast; ++iy) {
    double y = hist->GetYaxis()->GetBinCenter (iy);
        // cout << "y bin: " << iy << ' ' << y << endl;
    bool initedX = false;
    double logyprev = 0;
    int ix = binXlast;
    for (; ix >= binXfirst; --ix) {
      if (hist->GetBinContent(ix,iy) <= 0) continue;
      if (!initedX) {
	if (ix < binsX && hist->GetBinContent(ix+1,iy) > 0) {
	  logyprev = log (hist->GetBinContent(ix,iy));
	}
	else {
	  logyprev = 2*log (hist->GetBinContent(ix,iy)) - log(hist->GetBinContent(ix-1,iy));
	}
	logyprev = logyprev - logScale;
	if (logyprev > 0) break;
	initedX = true;
      }
      double logy = log (hist->GetBinContent(ix,iy)) - logScale;
            // cout << "makeCurveLeft next scan point " <<ix << ':'<<iy<<"  "<<hist->GetBinContent(ix,iy)<<'/'<<logy<<endl;
      if (logy > 0) {
	// cout << "x bin: " << ix << ' ' << hist->GetXaxis()->GetBinCenter (ix) << endl;
	double x = hist->GetXaxis()->GetBinCenter (ix) + 
	  (hist->GetXaxis()->GetBinCenter (ix)-hist->GetXaxis()->GetBinCenter (ix+1))*
	  logy/(logy-logyprev);
	xx.push_back(x);
	yy.push_back(y);
	// cout << "makeCurveLeft new point 1 " << x << ':' << y 
	//     << " (" << hist->GetXaxis()->GetBinCenter (ix+1) << ':' << logyprev << ","
	//     << hist->GetXaxis()->GetBinCenter (ix) << ':' << logy << ")"
	//     << endl;
	break;
      }
      logyprev = logy;
    }
  }

  //  orderInX (xx, yy);

  TGraph* result = new TGraph (xx.size(), &(xx[0]), &(yy[0]));
  result->SetLineWidth(3);
  result->SetMarkerStyle(8);
  result->SetMarkerSize (1.0);
  return result;
}

void ExtendGraphDown (TGraph* g, const TGraph* g2) {
  double xMax = -1e99;
  for (int i = 0; i < g->GetN(); ++i) {
    double x = g->GetX()[i];
    if (x > xMax) xMax = x;
  }
  for (int i = 0; i < g2->GetN(); ++i) {
    if (g2->GetX()[i] > xMax) {
      g->SetPoint (g->GetN(), g2->GetX()[i], g2->GetY()[i]);
    }
  }
}

TGraph* makeCurveUpDown (TH2D* hist, 
			 int binXfirst, int binXlast, int binYfirst, int binYlast, 
			 double scale) {
  TGraph* result = makeCurveUp (hist,binXfirst,binXlast,1, 99,scale);
  TGraph* result2 = makeCurveRight (hist, 1, 99, binYfirst, binYlast,scale);
  ExtendGraphDown (result, result2);
  return result;
}

void PrependGraph (TGraph* graph, double x, double y) {
  for (int i = graph->GetN()-1; i >= 0; --i) {
    graph->SetPoint (i+1, graph->GetX()[i], graph->GetY()[i]); 
  }
  graph->SetPoint (0, x, y);
}

void PrependGraph (TGraph* graph, TGraph* anotherGraph, bool reverse) {
  int newPoints = anotherGraph->GetN();
  if (newPoints > 0) {
    for (int i = graph->GetN()-1; i >= 0; --i) {
      graph->SetPoint (newPoints+i, graph->GetX()[i], graph->GetY()[i]);
    }
    for (int i = 0; i < newPoints; ++i) {
      int index = reverse ? newPoints-1-i : i;
      graph->SetPoint (i, anotherGraph->GetX()[index], anotherGraph->GetY()[index]);
    }
  }
}
 
 void AppendGraph (TGraph* graph, double x, double y) {
   graph->SetPoint (graph->GetN(), x, y);
 }
 
 void AppendGraph (TGraph* graph, TGraph* anotherGraph, bool reverse) {
  int oldPoints = graph->GetN();
  int newPoints = anotherGraph->GetN();
  for (int i = newPoints-1; i >= 0; --i) {
    int index = reverse ? newPoints-1-i : i;
    graph->SetPoint (oldPoints+i, anotherGraph->GetX()[index], anotherGraph->GetY()[index]);
  }
}

