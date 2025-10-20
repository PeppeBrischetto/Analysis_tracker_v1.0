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

   TCutG *cutG = new TCutG("cutG",10);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,83.8692,115.759);
   cutG->SetPoint(1,83.1424,115.641);
   cutG->SetPoint(2,82.6127,115.041);
   cutG->SetPoint(3,82.5141,114.296);
   cutG->SetPoint(4,82.9946,113.578);
   cutG->SetPoint(5,83.8692,113.291);
   cutG->SetPoint(6,84.7315,113.826);
   cutG->SetPoint(7,84.9779,114.714);
   cutG->SetPoint(8,84.6699,115.341);
   cutG->SetPoint(9,83.8692,115.759);
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
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.5,0.1,0.9,0.5);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->Draw();
   zoomPad->cd();
   h_cut_cl->Draw("SAME");
   cutG->Draw("SAME");
   
   


      
   
}
