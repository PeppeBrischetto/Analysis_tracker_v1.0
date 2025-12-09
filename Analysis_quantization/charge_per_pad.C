//#################################################################################################################
//#   This macro allows to obtain the charge spectrum for the pad of each rows from a tracks_ID.root file.
//#################################################################################################################
//#   Created November 2025 by A. Pitronaci
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>

#include "../Include/openfilesmerged.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void charge_per_pad(int run){

//################################################################################################################
// Needed variables
   
   char histoname[100];
   char titolox[100];
   Double_t charge = 0.;
   
   TH1I *h_pad0[NPads];
   for(int i=0; i<NPads; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"charge_{pad:%i} (bit)",i);
      h_pad0[i]=new TH1I("","",200,0,8000);
      h_pad0[i]->GetXaxis()->SetTitle(titolox);
      h_pad0[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad0[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad0[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad0[i]->GetYaxis()->SetTitle("Counts");
      h_pad0[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad0[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad0[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad0[i]->SetNdivisions(60);
      h_pad0[i]->SetLineColor(kCyan+2);
      h_pad0[i]->SetFillColor(kCyan-10);
      h_pad0[i]->SetFillStyle(3001);
   }
   
   TH1I *h_pad1[NPads];
   for(int i=0; i<NPads; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"charge_{pad:%i} (bit)",i);
      h_pad1[i]=new TH1I("","",200,0,8000);
      h_pad1[i]->GetXaxis()->SetTitle(titolox);
      h_pad1[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad1[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad1[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad1[i]->GetYaxis()->SetTitle("Counts");
      h_pad1[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad1[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad1[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad1[i]->SetNdivisions(60);
      h_pad1[i]->SetLineColor(kCyan+2);
      h_pad1[i]->SetFillColor(kCyan-10);
      h_pad1[i]->SetFillStyle(3001);
   }
   
   TH1I *h_pad2[NPads];
   for(int i=0; i<NPads; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"charge_{pad:%i} (bit)",i);
      h_pad2[i]=new TH1I("","",200,0,8000);
      h_pad2[i]->GetXaxis()->SetTitle(titolox);
      h_pad2[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad2[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad2[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad2[i]->GetYaxis()->SetTitle("Counts");
      h_pad2[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad2[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad2[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad2[i]->SetNdivisions(60);
      h_pad2[i]->SetLineColor(kCyan+2);
      h_pad2[i]->SetFillColor(kCyan-10);
      h_pad2[i]->SetFillStyle(3001);
   }
   
   TH1I *h_pad3[NPads];
   for(int i=0; i<NPads; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"charge_{pad:%i} (bit)",i);
      h_pad3[i]=new TH1I("","",200,0,8000);
      h_pad3[i]->GetXaxis()->SetTitle(titolox);
      h_pad3[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad3[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad3[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad3[i]->GetYaxis()->SetTitle("Counts");
      h_pad3[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad3[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad3[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad3[i]->SetNdivisions(60);
      h_pad3[i]->SetLineColor(kCyan+2);
      h_pad3[i]->SetFillColor(kCyan-10);
      h_pad3[i]->SetFillStyle(3001);
   }
   
   TH1I *h_pad4[NPads];
   for(int i=0; i<NPads; i++){
      sprintf(histoname,"h_pad %i",i);
      sprintf(titolox,"charge_{pad:%i} (bit)",i);
      h_pad4[i]=new TH1I("","",200,0,8000);
      h_pad4[i]->GetXaxis()->SetTitle(titolox);
      h_pad4[i]->GetXaxis()->SetTitleSize(0.05);
      h_pad4[i]->GetXaxis()->SetLabelSize(0.05);
      h_pad4[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_pad4[i]->GetYaxis()->SetTitle("Counts");
      h_pad4[i]->GetYaxis()->SetTitleSize(0.05);
      h_pad4[i]->GetYaxis()->SetLabelSize(0.05);
      h_pad4[i]->GetYaxis()->SetTitleOffset(1.);
      //h_pad4[i]->SetNdivisions(60);
      h_pad4[i]->SetLineColor(kCyan+2);
      h_pad4[i]->SetFillColor(kCyan-10);
      h_pad4[i]->SetFillStyle(3001);
   }
//################################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",5);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,22.0711,27.5907);
   cutG->SetPoint(1,176.674,208.212);
   cutG->SetPoint(2,185.042,194.43);
   cutG->SetPoint(3,27.5105,16.3471);
   cutG->SetPoint(4,22.0711,27.5907);
      
//################################################################################################################
// OpenFiles
   openMergedFile(run);
   tree->Print();   

//################################################################################################################
// Data-loop
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      //if(cutG->IsInside(cl_x_mm[0], cl_x_mm[1]){
      if(Row==0){
         //charge=Charge;
         h_pad0[Pads]->Fill(Charge);
      }else
       if(Row==1){
         //charge=Charge;
         h_pad1[Pads]->Fill(Charge);
       }else
        if(Row==2){
          //charge=Charge;
          h_pad2[Pads]->Fill(Charge);
        }else
         if(Row==3){
          //charge=Charge;
           h_pad3[Pads]->Fill(Charge);
         }else
          if(Row==4){
            //charge=Charge;
            h_pad4[Pads]->Fill(Charge);
          }
   }
   //}                                                                // TCutG parenthesis
   
// Visualization block
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   c->cd(1);
   h_pad0[15]->Draw();
   /*for(Int_t pad=0; pad<NPads; pad++){
      h_pad0[pad]->SetLineColor(pad);
      h_pad0[pad]->Draw("SAME");
   }*/
   c->cd(2);
   h_pad1[22]->Draw();
   /*for(Int_t pad=0; pad<NPads; pad++){
      h_pad1[pad]->SetLineColor(pad);
      h_pad1[pad]->Draw("SAME");
   }*/
   c->cd(3);
   h_pad2[29]->Draw();
   /*for(Int_t pad=0; pad<NPads; pad++){
      h_pad2[pad]->SetLineColor(pad);
      h_pad2[pad]->Draw("SAME");
   }*/
   c->cd(4);
   h_pad3[36]->Draw();
   /*for(Int_t pad=0; pad<NPads; pad++){
      h_pad3[pad]->SetLineColor(pad);
      h_pad3[pad]->Draw("SAME");
   }*/
   c->cd(5);
   h_pad4[44]->Draw();
   /*for(Int_t pad=0; pad<NPads; pad++){
      h_pad4[pad]->SetLineColor(pad);
      h_pad4[pad]->Draw("SAME");
   }*/

}
