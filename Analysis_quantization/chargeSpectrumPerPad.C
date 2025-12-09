//#################################################################################################################
//#   This macro allows to obtain the charge spectrum for the pad of each rows from a tracks_ID.root file.
//#################################################################################################################
//#   Created November 2025 (review by September 2025 by A. Pitronaci) 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfilesclone.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void chargeSpectrumPerPad(int run){
   
   char histoname[100];
   char titolox[100];
   Int_t sensPad[NRows][NPads] = {0};
   
   TH1D *h_pad[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"pad_{row:%i}",i);
      h_pad[i]=new TH1D("","",60,-0.5,59.5);
      h_pad[i]->GetXaxis()->SetTitle(titolox);
      h_pad[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad[i]->GetYaxis()->SetTitle("Counts");
      h_pad[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad[i]->SetNdivisions(60);
      h_pad[i]->SetLineColor(kCyan+2);
      h_pad[i]->SetFillColor(kCyan-10);
      h_pad[i]->SetFillStyle(3001);
   }
   
   TH1D *h_charge[NRows][NPads];
   for(Int_t row=0; row<NRows; row++){
      for(int i=0; i<NPads; i++){
         sprintf(histoname,"h_pad %i",i);
         sprintf(titolox,"charge pad_{row:%i} (bit)",row);
         h_charge[row][i]=new TH1D("","",2000,0,1.5e4);
         h_charge[row][i]->GetXaxis()->SetTitle(titolox);
         h_charge[row][i]->GetXaxis()->SetTitleSize(0.05);
         h_charge[row][i]->GetXaxis()->SetLabelSize(0.05);
         h_charge[row][i]->GetXaxis()->SetTitleOffset(0.92);
         //h_charge[row][i]->GetYaxis()->SetTitle("Counts");
         h_charge[row][i]->GetYaxis()->SetTitleSize(0.05);
         h_charge[row][i]->GetYaxis()->SetLabelSize(0.05);
         h_charge[row][i]->GetYaxis()->SetTitleOffset(1.);
         //h_charge[row][i]->SetNdivisions(60);
         h_charge[row][i]->SetLineColor(kCyan+2);
         h_charge[row][i]->SetFillColor(kCyan-10);
         h_charge[row][i]->SetFillStyle(3001);
      }
   }
   
//################################################################################################################
// OpenFiles
   openTrackCloneFile(run);
   tree->Print();   

//################################################################################################################
// Data-loop
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>4000){
        for(Int_t row=0; row<NRows; row++){
           for(Int_t pad=0; pad<cl_padMult[row]; pad++){
              h_pad[row]->Fill(pads_fired[row][pad]);
              h_charge[row][pad]->Fill(pad_charge[row][pad]);
           }
        }
      }
   }
   
// Visualization block
   
   TLegend* l = new TLegend(0.6,0.1,0.86,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   for(Int_t pad=0; pad<NPads; pad++){
      char titLeg[100];
      sprintf(titLeg,"Charge pad %d",pad);
      l->AddEntry(h_charge[0][pad], titLeg, "f");
   }
   
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      h_pad[row]->Draw();
   }
   
   TCanvas *c1 = new TCanvas("c1","c1",2200,2200);
   c1->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c1->cd(row+1);
      //h_charge[row][0]->Draw("SAME");
      for(Int_t pad=0; pad<NPads; pad++){
         h_charge[row][pad]->SetLineColor(pad+1);
         h_charge[row][pad]->Draw("SAME");
         l->Draw("SAME");
      }
   }
   
}
