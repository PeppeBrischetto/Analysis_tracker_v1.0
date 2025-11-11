//#################################################################################################################
//#   This macro allows to evaluate the error on the x_{i} measured on the padPlane.
//#################################################################################################################
//#   Created September 2025 by A. Pitronaci 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TLatex.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void cut(int run){

   TH2D *h_cut = new TH2D("h_cut", "Graph. Cut 38", 2000,0,300,2000,0,300);
   h_cut->SetStats(kFALSE);
   h_cut->GetXaxis()->SetTitle("cl_x_mm[0]");
   h_cut->GetYaxis()->SetTitle("cl_x_mm[1]");

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   Double_t entries = tree->GetEntries();
   cout << entries << endl << endl;
//###########################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",14);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,84.5357,115.814);
   cutG->SetPoint(1,84.1774,115.997);
   cutG->SetPoint(2,83.6477,115.89);
   cutG->SetPoint(3,83.0556,115.388);
   cutG->SetPoint(4,82.6661,114.672);
   cutG->SetPoint(5,82.5648,114.154);
   cutG->SetPoint(6,82.5726,113.788);
   cutG->SetPoint(7,82.7284,113.392);
   cutG->SetPoint(8,83.2426,113.194);
   cutG->SetPoint(9,83.9281,113.53);
   cutG->SetPoint(10,84.4968,114.245);
   cutG->SetPoint(11,84.7461,115.053);
   cutG->SetPoint(12,84.6915,115.555);
   cutG->SetPoint(13,84.5357,115.814);
   cutG->SetLineColor(kRed);
   cutG->SetLineWidth(2);
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      h_cut->Fill(cl_x_mm[0],cl_x_mm[1]);
   }
   
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   h_cut->Draw("COLZ");
   cutG->Draw("SAME");
   
   TH2D *h_cut_cl = (TH2D*)h_cut->Clone("h_cut_cl");
   h_cut_cl->GetXaxis()->SetLabelSize(0.05);
   h_cut_cl->GetYaxis()->SetLabelSize(0.05);
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.5,0.1,0.9,0.5);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->Draw();
   zoomPad->cd();
   h_cut_cl->Draw("SAME");
   cutG->Draw("SAME");
   
   


      
   
}
