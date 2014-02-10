#include <stdio.h>

#include "TStyle.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TMarker.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TList.h"
#include "TVirtualPad.h"
#include "TMath.h"
#include "TROOT.h"
#include "TLegendEntry.h"
#include "Riostream.h"
#include "TMultiGraph.h"
#include "THStack.h"

int coreWidth (int lineWidth) {
  return lineWidth; // no fancy patterns 
  return lineWidth <= 1 ? lineWidth : lineWidth <= 5 ? lineWidth-1 : lineWidth-2;
}

class TBetterGraph : public TGraph 
{
 public:
  TBetterGraph () {}
  TBetterGraph (const TGraph& graph) : TGraph (graph), bkg(graph), fg(graph) {
    bkg.SetLineColor (kBlack);
    fg.SetLineWidth (coreWidth(GetLineWidth()));
  }
  virtual ~TBetterGraph() {};
  virtual void Draw (Option_t* chopt = "") {
    bkg.Draw(chopt);
    fg.Draw(chopt);
  }
  private:
    TGraph bkg; 
    TGraph fg; 
};



class TBetterLegend : public TLegend
{
 public:
  TBetterLegend( Double_t x1, Double_t y1, Double_t x2, Double_t y2,
		 const char* header = "", Option_t* option="brNDC", bool fancy = false)
    : TLegend (x1, y1, x2, y2, header, option),
    mFancy (fancy)
      {}
    
    virtual ~TBetterLegend() {}
    
    virtual TLegendEntry* AddBetterEntry (const TGraph* obj1, const TGraph* obj2, const char* label);
    virtual void PaintPrimitives();
 private:
    std::vector<const TAttLine*> fancyLine2;
    bool mFancy;
};


TLegendEntry* TBetterLegend::AddBetterEntry (const TGraph* obj1, const TGraph* obj2, const char* label) 
{
  TLegendEntry* result = AddEntry (obj1, label);
  int counter = GetListOfPrimitives()->GetSize();
  for (int i = fancyLine2.size(); i < counter - 1; ++i) fancyLine2.push_back(0);
  const TAttLine* att2 = dynamic_cast<const TAttLine*> (obj2);
  if (att2) fancyLine2.push_back (att2);
  else
    {
      TAttLine* att3 = new TAttLine();
      att3->SetLineWidth(0);
      fancyLine2.push_back (att3);
    }
  return result;
}


//______________________________________________________________________________
void TBetterLegend::PaintPrimitives()
{
   /* Begin_Html
   Paint the entries (list of primitives) for this legend.
   End_Html */

   Int_t nRows = GetNRows();
   if ( nRows == 0 ) return;

   // Evaluate text size as a function of the number of entries
   //  taking into account their real size after drawing latex
   // Note: in pixel coords y1 > y2=0, but x2 > x1=0
   //       in NDC          y2 > y1,   and x2 > x1

   Double_t x1 = fX1NDC;
   Double_t y1 = fY1NDC;
   Double_t x2 = fX2NDC;
   Double_t y2 = fY2NDC;
   Double_t margin = fMargin*( x2-x1 )/fNColumns;
   Double_t boxwidth = margin;
   Double_t boxw = boxwidth*0.35;
   Double_t yspace = (y2-y1)/nRows;
   Double_t textsize = GetTextSize();
   Double_t save_textsize = textsize;
   Double_t* columnWidths = new Double_t[fNColumns];
   memset(columnWidths, 0, fNColumns*sizeof(Double_t));

   if ( textsize == 0 ) {
      textsize = ( 1. - fEntrySeparation ) * yspace;

      // find the max width and height (in pad coords) of one latex entry label
      Double_t maxentrywidth = 0, maxentryheight = 0;
      TIter nextsize(fPrimitives);
      TLegendEntry *entrysize;
      Int_t iColumn = 0;
      while (( entrysize = (TLegendEntry *)nextsize() )) {
         TLatex entrytex( 0, 0, entrysize->GetLabel() );
         entrytex.SetNDC();
         Style_t tfont = entrysize->GetTextFont();
         if (tfont == 0) tfont = GetTextFont();
         entrytex.SetTextFont(tfont);
         entrytex.SetTextSize(textsize);
         if ( entrytex.GetYsize() > maxentryheight ) {
            maxentryheight = entrytex.GetYsize();
         }
         TString opt = entrysize->GetOption();
         opt.ToLower();
         if ( opt.Contains("h") ) {
            if ( entrytex.GetXsize() > maxentrywidth ) {
               maxentrywidth = entrytex.GetXsize();
            }
         } else {
            if ( entrytex.GetXsize() > columnWidths[iColumn] ) {
               columnWidths[iColumn] = entrytex.GetXsize();
            }
            iColumn++;
            iColumn %= fNColumns;
         }
         Double_t tmpMaxWidth = 0.0;
         for(int i=0; i<fNColumns; i++) tmpMaxWidth += columnWidths[i];
         if ( tmpMaxWidth > maxentrywidth) maxentrywidth = tmpMaxWidth;
      }
      // make sure all labels fit in the allotted space
      Double_t tmpsize_h = maxentryheight /(gPad->GetY2() - gPad->GetY1());
      textsize = TMath::Min( textsize, tmpsize_h );
      Double_t tmpsize_w = textsize*(fX2-fX1)*(1.0-fMargin)/maxentrywidth;
      if(fNColumns > 1) tmpsize_w = textsize*(fX2-fX1)*(1.0-fMargin-fColumnSeparation)/maxentrywidth;
      textsize = TMath::Min( textsize, tmpsize_w );
      SetTextSize( textsize );
   }

   // Update column widths, put into NDC units
   // block off this section of code to make sure all variables are local:
   // don't want to ruin initialization of these variables later on
   {
      TIter next(fPrimitives);
      TLegendEntry *entry;
      Int_t iColumn = 0;
      memset(columnWidths, 0, fNColumns*sizeof(Double_t));
      while (( entry = (TLegendEntry *)next() )) {
         TLatex entrytex( 0, 0, entry->GetLabel() );
         entrytex.SetNDC();
         Style_t tfont = entry->GetTextFont();
         if (tfont == 0) tfont = GetTextFont();
         entrytex.SetTextFont(tfont);
         if(entry->GetTextSize() == 0) entrytex.SetTextSize(textsize);
         TString opt = entry->GetOption();
         opt.ToLower();
         if (!opt.Contains("h")) {
            if ( entrytex.GetXsize() > columnWidths[iColumn] ) {
               columnWidths[iColumn] = entrytex.GetXsize();
            }
            iColumn++;
            iColumn %= fNColumns;
         }
      }
      double totalWidth = 0.0;
      for(int i=0; i<fNColumns; i++) totalWidth += columnWidths[i];
      if(fNColumns > 1) totalWidth /= (1.0-fMargin-fColumnSeparation);
      else totalWidth /= (1.0 - fMargin);
      for(int i=0; i<fNColumns; i++) {
         columnWidths[i] = columnWidths[i]/totalWidth*(x2-x1) + margin;
      }
   }

   Double_t ytext = y2 + 0.5*yspace;  // y-location of 0th entry

   // iterate over and paint all the TLegendEntries
   int counter = 0;
   TIter next(fPrimitives);
   TLegendEntry *entry;
   Int_t iColumn = 0;
   while (( entry = (TLegendEntry *)next() )) {
     ++counter;
      if(iColumn == 0) ytext -= yspace;

      // Draw Label in Latexmargin

      Short_t talign = entry->GetTextAlign();
      Float_t tangle = entry->GetTextAngle();
      Color_t tcolor = entry->GetTextColor();
      Style_t tfont  = entry->GetTextFont();
      Size_t  tsize  = entry->GetTextSize();
      // if the user hasn't set a parameter, then set it to the TLegend value
      if (talign == 0) entry->SetTextAlign(GetTextAlign());
      if (tangle == 0) entry->SetTextAngle(GetTextAngle());
      if (tcolor == 0) entry->SetTextColor(GetTextColor());
      if (tfont  == 0) entry->SetTextFont(GetTextFont());
      if (tsize  == 0) entry->SetTextSize(GetTextSize());
      // set x,y according to the requested alignment
      Double_t x=0,y=0;
      Int_t halign = entry->GetTextAlign()/10;
      Double_t entrymargin = margin;
      // for the header the margin is near zero
      TString opt = entry->GetOption();
      opt.ToLower();
      x1 = fX1NDC;
      x2 = fX2NDC;
      if ( opt.Contains("h") ) entrymargin = margin/10.;
      else if (fNColumns > 1) {
         for(int i=0; i<iColumn; i++) x1 += columnWidths[i] + fColumnSeparation*(fX2NDC-fX1NDC)/(fNColumns-1);
         x2 = x1 + columnWidths[iColumn];
         iColumn++;
         iColumn %= fNColumns;
      }
      if (halign == 1) x = x1 + entrymargin;
      if (halign == 2) x = 0.5*( (x1+entrymargin) + x2 );
      if (halign == 3) x = x2 - entrymargin/10.;
      Int_t valign = entry->GetTextAlign()%10;
      if (valign == 1) y = ytext - (1. - fEntrySeparation)* yspace/2.;
      if (valign == 2) y = ytext;
      if (valign == 3) y = ytext + (1. - fEntrySeparation)* yspace/2.;
      //
      TLatex entrytex( x, y, entry->GetLabel() );
      entrytex.SetNDC();
      entry->TAttText::Copy(entrytex);
      entrytex.Paint();
      // reset attributes back to their original values
      entry->SetTextAlign(talign);
      entry->SetTextAngle(tangle);
      entry->SetTextColor(tcolor);
      entry->SetTextFont(tfont);
      entry->SetTextSize(tsize);

      // define x,y as the center of the symbol for this entry
      Double_t xsym = x1 + margin/2.;
      Double_t ysym = ytext;

      TObject *eobj = entry->GetObject();

      const TAttLine* line2Att = fancyLine2[counter-1];
      if (line2Att) // Draw BetterLine
	{
	  TAttLine* atl = dynamic_cast<TAttLine*>(eobj);
	  if (atl) atl->Copy(*entry);
	  else cout << "Can not convert " << eobj << " to ATtLine" << endl;
	  TLine line1( xsym - boxw, ysym, xsym + boxw, ysym );
	  line1.SetBit(TLine::kLineNDC);
	  entry->TAttLine::Copy(line1);
	  if (mFancy) {
/* 	    if (line2Att->GetLineWidth() > 0) { */
/* 	      TBox box1; */
/* 	      //	      box1.SetFillColor (line1.GetLineColor()); */
/* 	      box1.SetFillColor (line2Att->GetLineColor()); */
/* 	      box1.SetFillStyle (3002); */
/* 	      box1.TAttFill::Modify(); */
/* 	      Double_t xf[4],yf[4]; */
/* 	      xf[0] = xsym - boxw; */
/* 	      yf[0] = ysym - yspace*0.3; */
/* 	      xf[1] = xsym + boxw; */
/* 	      yf[1] = yf[0]; */
/* 	      xf[2] = xf[1]; */
/* 	      yf[2] = ysym + yspace*0.3; */
/* 	      xf[3] = xf[0]; */
/* 	      yf[3] = yf[2]; */
/* 	      for (Int_t i=0;i<4;i++) { */
/* 		xf[i] = gPad->GetX1() + xf[i]*(gPad->GetX2()-gPad->GetX1()); */
/* 		yf[i] = gPad->GetY1() + yf[i]*(gPad->GetY2()-gPad->GetY1()); */
/* 	      } */
/* 	      gPad->PaintFillArea(4,xf,yf); */
/* 	    } */

	    TLine line11 (line1);
	    line11.SetLineColor (kBlack);
	    line11.Paint();
	    line1.SetLineWidth (coreWidth(line1.GetLineWidth ()));
	  }
	  line1.Paint();

	  if (line2Att->GetLineWidth() > 0) {
	    TLine line2( xsym - boxw, ysym, xsym + boxw, ysym );
	    line2.SetBit(TLine::kLineNDC);
	    line2Att->TAttLine::Copy(line2);
	    TLine line21 (line2);
	    line21.SetLineColor(kBlack);
	    line2.SetLineWidth (coreWidth(line2.GetLineWidth ()));
	    line21.PaintLineNDC(xsym - boxw, ysym+yspace*0.3, xsym + boxw, ysym+yspace*0.3);
	    line2.PaintLineNDC(xsym - boxw, ysym+yspace*0.3, xsym + boxw, ysym+yspace*0.3);
	    line21.PaintLineNDC(xsym - boxw, ysym-yspace*0.3, xsym + boxw, ysym-yspace*0.3);
	    line2.PaintLineNDC(xsym - boxw, ysym-yspace*0.3, xsym + boxw, ysym-yspace*0.3);
	  }
	}
      else
	{
	  // Draw fill pattern (in a box)
	  
	  if ( opt.Contains("f")) {
	    if (eobj && eobj->InheritsFrom(TAttFill::Class())) {
	      dynamic_cast<TAttFill*>(eobj)->Copy(*entry);
	    }
	    
	    // box total height is yspace*0.7
	    entry->TAttFill::Modify();
	    Double_t xf[4],yf[4];
	    xf[0] = xsym - boxw;
	    yf[0] = ysym - yspace*0.35;
	    xf[1] = xsym + boxw;
	    yf[1] = yf[0];
	    xf[2] = xf[1];
	    yf[2] = ysym + yspace*0.35;
	    xf[3] = xf[0];
	    yf[3] = yf[2];
	    for (Int_t i=0;i<4;i++) {
	      xf[i] = gPad->GetX1() + xf[i]*(gPad->GetX2()-gPad->GetX1());
	      yf[i] = gPad->GetY1() + yf[i]*(gPad->GetY2()-gPad->GetY1());
	    }
	    gPad->PaintFillArea(4,xf,yf);
	  }
	  
	  // Draw line
	  
	  if ( opt.Contains("l") || opt.Contains("f")) {
	    
	    if (eobj && eobj->InheritsFrom(TAttLine::Class())) {
	      dynamic_cast<TAttLine*>(eobj)->Copy(*entry);
	    }
	    // line total length (in x) is margin*0.8
	    TLine entryline( xsym - boxw, ysym, xsym + boxw, ysym );
	    entryline.SetBit(TLine::kLineNDC);
	    entry->TAttLine::Copy(entryline);
	    // if the entry is filled, then surround the box with the line instead
	    if ( opt.Contains("f") && !opt.Contains("l")) {
	      // box total height is yspace*0.7
	      boxwidth = yspace*
		(gPad->GetX2()-gPad->GetX1())/(gPad->GetY2()-gPad->GetY1());
            if ( boxwidth > margin ) boxwidth = margin;
	    
            entryline.PaintLineNDC( xsym - boxw, ysym + yspace*0.35,
				    xsym + boxw, ysym + yspace*0.35);
            entryline.PaintLineNDC( xsym - boxw, ysym - yspace*0.35,
				    xsym + boxw, ysym - yspace*0.35);
            entryline.PaintLineNDC( xsym + boxw, ysym - yspace*0.35,
				    xsym + boxw, ysym + yspace*0.35);
            entryline.PaintLineNDC( xsym - boxw, ysym - yspace*0.35,
				    xsym - boxw, ysym + yspace*0.35);
	    } else {
	      entryline.Paint();
	      if (opt.Contains("e")) {
		entryline.PaintLineNDC( xsym, ysym - yspace*0.30,
					xsym, ysym + yspace*0.30);
	      }
	    }
	  }
	  
	  // Draw Polymarker
	  
	  if ( opt.Contains("p")) {
	    
	    if (eobj && eobj->InheritsFrom(TAttMarker::Class())) {
	      dynamic_cast<TAttMarker*>(eobj)->Copy(*entry);
	    }
	    TMarker entrymarker( xsym, ysym, 0 );
	    entrymarker.SetNDC();
	    entry->TAttMarker::Copy(entrymarker);
	    entrymarker.Paint();
	  }
	}
   }

   SetTextSize(save_textsize);
   delete [] columnWidths;
}
