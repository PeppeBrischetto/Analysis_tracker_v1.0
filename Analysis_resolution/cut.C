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
   
   TH2D *h_cut1 = new TH2D("h_cut1", "Graph. Cut 38", 2000,0,300,2000,0,300);
   h_cut1->GetXaxis()->SetTitle("cl_x_mm[0]");
   h_cut1->GetYaxis()->SetTitle("cl_x_mm[1]");

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   Double_t entries = tree->GetEntries();
   cout << entries << endl << endl;
//###########################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",7);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,62.2771,106.697);
   cutG->SetPoint(1,59.6055,104.969);
   cutG->SetPoint(2,59.5694,103.386);
   cutG->SetPoint(3,62.3313,103.407);
   cutG->SetPoint(4,64.317,105.566);
   cutG->SetPoint(5,63.7754,106.758);
   cutG->SetPoint(6,62.2771,106.697);
   cutG->SetLineColor(kRed);
   cutG->SetLineWidth(2);
   
   TCutG *cutGe = new TCutG("cutGe",8);
   cutGe->SetVarX("sic_charge");
   cutGe->SetVarY("cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10]");
   cutGe->SetPoint(0,5188.44,81021.6);
   cutGe->SetPoint(1,5170.92,78217.9);
   cutGe->SetPoint(2,5189.79,73362.8);
   cutGe->SetPoint(3,5261.89,69670.1);
   cutGe->SetPoint(4,5305.68,71721.6);
   cutGe->SetPoint(5,5307.03,78628.2);
   cutGe->SetPoint(6,5224.83,82115.7);
   cutGe->SetPoint(7,5188.44,81021.6);
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      h_cut->Fill(cl_x_mm[0],cl_x_mm[1]);
      if(cutGe->IsInside(sic_charge,cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10])){
        h_cut1->Fill(cl_x_mm[0],cl_x_mm[1]);
      }
   }
   
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   h_cut->Draw("COLZ");
   h_cut1->Draw("SCAT SAME");
   cutG->Draw("SAME");
   
   TH2D *h_cut_cl = (TH2D*)h_cut->Clone("h_cut_cl");
   h_cut_cl->GetXaxis()->SetLabelSize(0.05);
   h_cut_cl->GetYaxis()->SetLabelSize(0.05);
   TH2D *h_cut_cl1 = (TH2D*)h_cut1->Clone("h_cut_cl1");
   h_cut_cl1->GetXaxis()->SetLabelSize(0.05);
   h_cut_cl1->GetYaxis()->SetLabelSize(0.05);
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.5,0.1,0.9,0.5);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->Draw();
   zoomPad->cd();
   h_cut_cl->Draw("SAME");
   h_cut_cl1->Draw("SCAT SAME");
   cutG->Draw("SAME");
   
   TCanvas *c1 = new TCanvas("c1","c1",900,900);
   c1->cd();
   tree->Draw("cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10]:sic_sharge","","COLZ");


      
   
}
