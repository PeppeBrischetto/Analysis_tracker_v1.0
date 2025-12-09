//#################################################################################################################
//#   This macro allows to obtain the charge distribution from a tracks_ID.root file.
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

void chargeDistr(int run){

   char histoname[100];
   char titolox[100];
   Int_t counter = 0;
   Double_t x[NRows] = {0.};
   Double_t max[NRows] = {0.};
   Double_t fwhm[NRows] = {0.};
   Double_t rms[NRows] = {0.};
   Double_t theta = 0.;
   ofstream outfit;
   Int_t noSic = 0;
   Double_t threshold[NRows] = {400,400,400,400,400};

   TH1D *h_charge[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_charge %i",i);
      sprintf(titolox,"pad_{row:%i}",i);
      h_charge[i]=new TH1D("","",60,-0.5,59.5);
      h_charge[i]->GetXaxis()->SetTitle(titolox);
      h_charge[i]->GetXaxis()->SetTitleSize(0.05);
      h_charge[i]->GetXaxis()->SetLabelSize(0.05);
      h_charge[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_charge[i]->GetYaxis()->SetTitle("Counts");
      h_charge[i]->GetYaxis()->SetTitleSize(0.05);
      h_charge[i]->GetYaxis()->SetLabelSize(0.05);
      h_charge[i]->GetYaxis()->SetTitleOffset(1.);
      h_charge[i]->SetNdivisions(7);
      h_charge[i]->SetFillColor(kBlue-5);
      h_charge[i]->SetFillStyle(3001);
   }
   
   TH1D *h_theta = new TH1D("h_theta","",500,0,70);
   h_theta->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta->GetXaxis()->SetTitleSize(0.05);
   h_theta->GetXaxis()->SetLabelSize(0.05);
   h_theta->GetXaxis()->SetTitleOffset(1.);
   //h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetYaxis()->SetTitleSize(0.05);
   h_theta->GetYaxis()->SetLabelSize(0.05);
   h_theta->GetYaxis()->SetTitleOffset(1.);
   h_theta->SetNdivisions(7);
   h_theta->SetFillColor(kBlue-5);
   
   TF1 *norm = new TF1("norm","gaus",20,70);
   norm->SetParameters(0,0,0);
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",5);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,29.66,13.13);
   cutG->SetPoint(1,192.49,209.12);
   cutG->SetPoint(2,176.50,214.1);
   cutG->SetPoint(3,14.15,20.40);
   cutG->SetPoint(4,29.66,13.13);
   
//################################################################################################################
// Data loop
   for(Int_t i=0; i<entries; i++){
      
      for(Int_t row=0; row<NRows; row++){
         h_charge[row]->Reset();
      }
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>4000){
        counter++ ;
        h_theta->Fill(theta_deg);
        for(Int_t row=0; row<NRows; row++){
           x[row] += cl_padMult[row];
           for(Int_t pad=0; pad<cl_padMult[row]; pad++) {
             if(pad_charge[row][pad]>400){
              h_charge[row]->SetBinContent(pads_fired[row][pad], pad_charge[row][pad]);
             }else{
                h_charge[row]->SetBinContent(pads_fired[row][pad],0.);
             }
           }
           max[row] += h_charge[row]->GetMaximum();
           c->cd(row+1);
           h_charge[row]->Draw();
           gPad->Update();
           TPaveStats *st = (TPaveStats*)h_charge[row]->GetListOfFunctions()->FindObject("stats");
           st->SetX1NDC(0.75);
           st->SetX2NDC(0.98);
           st->SetY1NDC(0.75);
           st->SetY2NDC(0.98);
           st->SetTextSize(0.041);
           gPad->Modified();
           gPad->Update();
           }
        if(counter<100){
          char titolo[100];
          sprintf(titolo,"ChargeDistrib/Run%d/charge_distr_artThr/chargeDistr_%d.png",run,i);
          c->SaveAs(titolo);                                                                     
        }
         
        for(Int_t i=0; i<NRows; i++){
           TF1 *f = new TF1(Form("f_%d", i), "gaus", 0, 300);
           f->SetParameters(0, 0);
           h_charge[i]->Fit(f,"N","N",0,300);
           fwhm[i] += f->GetParameter(2);
        }
      }else{
              noSic++;
       }
   }
   
   theta=h_theta->Fit("norm","R+");
   char titolo[100];
   sprintf(titolo,"ChargeDistrib/Run%d/chargeDistrib_run_%d.txt",run,run);
   outfit.open(titolo);
   outfit << "                  Charge Distribution width (FWHM) - run: " << run << endl << endl;
   outfit << "Theta: " << theta << endl << endl; 
   
   for(Int_t row=0; row<NRows; row++){
      fwhm[row] = fwhm[row]/counter;
      x[row] = x[row]/counter;
      max[row] = max[row]/counter;
      outfit << "sigma row" << row << ": " << fwhm[row] << "   FWHM(2.35*sigma): 2.35*" << fwhm[row] << " = " << 2.35*fwhm[row] << "   StDEV: " << h_charge[row]->GetStdDev() << "   FWHM: " << 2.35*h_charge[row]->GetStdDev() << endl;
      outfit << "x row" << row << " (mm): " << 5*x[row] << "   max row" << row << " (ch): " << max[row] << endl << endl;
   }
   
   outfit << "Evts. not reaching the SiC: " << noSic << endl;
   TCanvas *c1 = new TCanvas ("c1","c1",900,900);
   c1->cd();
   h_theta->Draw();
   


}
