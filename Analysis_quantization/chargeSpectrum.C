//#################################################################################################################
//#   This macro allows to obtain the charge spectrum for the rows from a tracks_ID.root file.
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
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void chargeSpectrum(int run){

   char histoname[100];
   char titolox[100];
   char titoloFile[100];
   char tit[100];
   Double_t minbin[NRows] = {0.};
   ofstream outfit;
   
   TH1D *h_spectrum[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_spectrum %i",i);
      sprintf(titolox,"charge_{row:%i} (bit)",i);
      h_spectrum[i]=new TH1D("","",2000,0,1.5e4);
      h_spectrum[i]->GetXaxis()->SetTitle(titolox);
      h_spectrum[i]->GetXaxis()->SetTitleSize(0.05);
      h_spectrum[i]->GetXaxis()->SetLabelSize(0.05);
      h_spectrum[i]->GetXaxis()->SetTitleOffset(0.92);
      //h_spectrum[i]->GetYaxis()->SetTitle("Counts");
      h_spectrum[i]->GetYaxis()->SetTitleSize(0.05);
      h_spectrum[i]->GetYaxis()->SetLabelSize(0.05);
      h_spectrum[i]->GetYaxis()->SetTitleOffset(1.);
      h_spectrum[i]->SetNdivisions(7);
      h_spectrum[i]->SetLineColor(kCyan+2);
      h_spectrum[i]->SetFillColor(kCyan-10);
      h_spectrum[i]->SetFillStyle(3001);
   }
   
   TH1D *h1_1 = new TH1D("h1_1","",2000,0,1.5e4);
   h1_1->SetLineColor(kGreen+2);
   h1_1->SetFillStyle(3001);
   h1_1->SetFillColor(kGreen-5);
   TH1D *h1_2 = new TH1D("h1_2","",2000,0,1.5e4);
   h1_2->SetLineColor(kOrange+2);
   h1_2->SetFillStyle(3001);
   h1_2->SetFillColor(kOrange-5);
   TH1D *h2_1 = new TH1D("h2_1","",2000,0,1.5e4);
   h2_1->SetLineColor(kBlue+2);
   h2_1->SetFillStyle(3001);
   h2_1->SetFillColor(kBlue-5);
   TH1D *h2_2 = new TH1D("h2_2","",2000,0,1.5e4);
   h2_2->SetLineColor(kGreen+2);
   h2_2->SetFillStyle(3001);
   h2_2->SetFillColor(kGreen-5);
   
   TH1D *histo_x[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x[i]=new TH1D("","",9000.,0.,300);
      histo_x[i]->GetXaxis()->SetTitle(tit);
      histo_x[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x[i]->SetNdivisions(12);
      histo_x[i]->SetLineColor(kCyan+1);
      histo_x[i]->SetFillColor(kCyan-10);
      //histo_x[i]->SetFillStyle(3001);
      histo_x[i]->SetLineWidth(2);
      histo_x[i]->SetLineStyle(2);
   }
   
   TH1D *histo_anti_x[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_anti_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_anti_x[i]=new TH1D("","",9000.,0.,300);
      histo_anti_x[i]->GetXaxis()->SetTitle(tit);
      histo_anti_x[i]->GetXaxis()->SetTitleSize(0.05);
      histo_anti_x[i]->GetXaxis()->SetLabelSize(0.05);
      histo_anti_x[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_anti_x[i]->GetYaxis()->SetTitleSize(0.05);
      histo_anti_x[i]->GetYaxis()->SetLabelSize(0.05);
      histo_anti_x[i]->GetYaxis()->SetTitleOffset(1.);
      histo_anti_x[i]->SetNdivisions(12);
      histo_anti_x[i]->SetLineColor(kOrange+1);
      histo_anti_x[i]->SetFillColor(kOrange-10);
      //histo_anti_x[i]->SetFillStyle(3001);
      histo_anti_x[i]->SetLineWidth(2);
      histo_anti_x[i]->SetLineStyle(2);
   }
   
   TH1D *histo_xTot[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_xTot %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_xTot[i]=new TH1D("","",9000.,0.,300);
      histo_xTot[i]->GetXaxis()->SetTitle(tit);
      histo_xTot[i]->GetXaxis()->SetTitleSize(0.05);
      histo_xTot[i]->GetXaxis()->SetLabelSize(0.05);
      histo_xTot[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_xTot[i]->GetYaxis()->SetTitleSize(0.05);
      histo_xTot[i]->GetYaxis()->SetLabelSize(0.05);
      histo_xTot[i]->GetYaxis()->SetTitleOffset(1.);
      histo_xTot[i]->SetNdivisions(12);
      histo_xTot[i]->SetLineColor(kBlue+1);
      histo_xTot[i]->SetFillColor(kBlue-9);
      //histo_xTot[i]->SetFillStyle(3001);
      histo_xTot[i]->SetLineWidth(2);
   }
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();   

//################################################################################################################
// Data-loop
   for(Int_t i=0; i<entries; i++){
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>4000){
        for(Int_t row=0; row<NRows; row++){
           histo_xTot[row]->Fill(cl_x_mm[row]);
           for(Int_t pad=0; pad<cl_padMult[row]; pad++){
              if(pad_charge[row][pad]!=0){
                h_spectrum[row]->Fill(pad_charge[row][pad]);
                if(row==1){
                  if(cl_padMult[1]==6){
                    h1_1->Fill(pad_charge[1][pad]);
                  }else
                       if(cl_padMult[1]==7){
                         h1_2->Fill(pad_charge[1][pad]);
                       }
                }else
                 if(row==2){
                   if(cl_padMult[2]==5){
                     h2_1->Fill(pad_charge[2][pad]);
                   }else
                    if(cl_padMult[2]==6){
                      h2_2->Fill(pad_charge[2][pad]);
                    }
                 }
              }
              if(pad_charge[2][pad]>1600){
                histo_x[row]->Fill(cl_x_mm[row]);
              }else{
                histo_anti_x[row]->Fill(cl_x_mm[row]);
              }
              break;
           }
        }
      }
      
   }
   
// Writing main info (mean and errors) on a .txt file for each threshold
   
   sprintf(titoloFile,"Run%d/CoG_run_%d_th1600.txt",run,run);
   outfit.open(titoloFile);
   outfit << "                  Centre of Gravity reconstruction - run: " << run << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      outfit << endl;
      outfit << "Centre of Gravity (tot) row " << row << ": " << histo_xTot[row]->GetMean() << "   Error of Centre of Gravity Tot): " << histo_xTot[row]->GetMeanError() << endl;
      outfit << "Centre of Gravity (>thr) row " << row << ": " << histo_x[row]->GetMean() << "   Error of Centre of Gravity Tot): " << histo_x[row]->GetMeanError() << endl;
      outfit << "Centre of Gravity (<thr) row " << row << ": " << histo_anti_x[row]->GetMean() << "   Error of Centre of Gravity Tot): " << histo_anti_x[row]->GetMeanError() << endl;
   }

//################################################################################################################
// Visualization block
   
   TH1D *h_spectrumClone[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_spectrumClone_ %i",i);
      h_spectrumClone[i] = (TH1D*)h_spectrum[i]->Clone(histoname);
      h_spectrumClone[i]->GetXaxis()->SetRangeUser(0,600);
      h_spectrumClone[i]->GetXaxis()->SetTitle("");
      h_spectrumClone[i]->GetXaxis()->SetTitleSize(0.08);
      h_spectrumClone[i]->GetXaxis()->SetLabelSize(0.09);
      h_spectrumClone[i]->GetYaxis()->SetLabelSize(0.09);
   }
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      h_spectrum[row]->Draw();
      if(row==1){
        h1_1->Draw("SAME");
        h1_2->Draw("SAME");
      }else
       if(row==2){
         h2_1->Draw("SAME");
         h2_2->Draw("SAME");
       }
      TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.4,0.2,0.9,0.7);
      zoomPad->SetFillColor(0);
      zoomPad->SetFrameFillStyle(0);
      //zoomPad->SetLogx();
      //zoomPad->SetLogy();
      zoomPad->Draw();
      zoomPad->cd();
      h_spectrumClone[row]->Draw();
   }
   
   TLegend* l = new TLegend(0.1,0.7,0.26,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(histo_x[0], "Events > th(1600)", "f");
   l->AddEntry(histo_anti_x[0], "Events < th(1600)", "f");
   l->AddEntry(histo_xTot[0], "All events", "f");
   
   TCanvas *c1 = new TCanvas("c1","c1",2200,2200);
   c1->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c1->cd(row+1);
      histo_xTot[row]->Draw();
      histo_anti_x[row]->Draw("SAME");
       histo_x[row]->Draw("SAME");
      l->Draw("SAME");
   }   

}
