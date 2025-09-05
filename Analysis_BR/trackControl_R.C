//################################################################################################################
//#   This macro allows to perform an additional control on the track events coming from the IRRAD4 experiment.
//#   in particular, for each track, this macro extrapolates the (pad,row)-pairs, thus makes a fit of them and 
//#   plots the R-coefficient (i.e. the Pearson correlation coefficient) in order to discern pathological events.
//################################################################################################################
//#   Created July 2025 by A. Pitronaci 
//################################################################################################################
//#   Updated:
//################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void trackControl_R(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   char titolofile[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *pearson = new TH1D("pearson","",1000,0.92,1.04);
   pearson->GetXaxis()->SetTitle("Pearson coefficient r");
   pearson->GetYaxis()->SetTitle("Counts");
   
   
   ofstream outputfile;
   TGraph *retta = new TGraph(5);
   retta->SetMarkerStyle(20);
   retta->SetLineWidth(0);
   retta->GetXaxis()->SetTitle("x (mm)");
   retta->GetYaxis()->SetTitle("z (mm)");
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d_4He.txt",run);
   outputfile.open(titolofile);
   outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,14.6,30.);
   cutGli->SetPoint(1,88,128.6);
   cutGli->SetPoint(2,58.6,140.6);
   cutGli->SetPoint(3,6.4,57.);
   cutGli->SetPoint(4,14.6,30.);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,11.7,13.7);
   cutGa->SetPoint(1,134.5,158.4);
   cutGa->SetPoint(2,116.6,168.3);
   cutGa->SetPoint(3,5.4,30.);
   cutGa->SetPoint(4,11.7,13.7);

//#################################################################################################
// Data loop
   for(Int_t i = 0; i < entries; i++){
      Double_t theta_fit = 0.;
      Double_t pad[NRows][100] = {0.};
      Double_t charge[NRows][100] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t y[NRows] = {0.};
      Double_t totalCharge[NRows] = {0.};
      Double_t r = 0.;

      tree->GetEntry(i);
      retta->Set(0);

      TF1* f = new TF1(Form("f_%d", i), "[0] + [1]*x", 0, 300);
      f->SetParameters(0, 0);

      if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            anode->Fill(pad[row][p], 2 * row + 1, charge[row][p]);
            x_mm[p] = 2.5 + (5 * pad[row][p]);
            x[row] += (x_mm[p] * charge[row][p]);
         }
         x[row] = x[row] / (totalCharge[row]);
         retta->AddPoint(x[row], z[row]);
      }
//      cout << "Pearson coefficient: " << anode->GetCorrelationFactor() << endl;
      r=anode->GetCorrelationFactor();
      pearson->Fill(r);
      
      outputfile << "Evt. " << i << "   P.coefficient (R): " << r << endl;
      }                                                                      // TCutg parenthesis
      
      
      anode->Reset("ICES");
   }
   
   outputfile << "*************************** entries: " << entries << " *****************************" << endl;
   
   Int_t hpos = 0.5*pearson->GetMaximum();
   char testo[100];
   sprintf(testo,"Run %d - ^{4}He tracks",run);
   TLatex *t = new TLatex(0.93,hpos,testo);
   TCanvas *c = new TCanvas();
   c->cd();
   pearson->Draw();
   t->Draw("SAME");
   sprintf(testo,"Pictures_Analysis/Perason_correlation_analysis/run%d_4He.png",run);
   c->SaveAs(testo);


}
