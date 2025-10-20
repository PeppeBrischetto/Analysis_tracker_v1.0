//#################################################################################################################
//#   This macro allows to obtain the charge distribution from a tracks_ID.root file.
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
   Int_t counter = 0;
   Double_t x[NRows] = {0.};
   Double_t max[NRows] = {0.};
   Double_t fwhm[NRows] = {0.};
   Double_t rms[NRows] = {0.};
   Double_t theta = 0.;
   ofstream outfit;

   TH1D *h_charge[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_charge %i",i);
      h_charge[i]=new TH1D("","",60,-0.5,60.5);
      h_charge[i]->GetXaxis()->SetTitle("x (mm)");
      h_charge[i]->GetXaxis()->SetTitleSize(0.05);
      h_charge[i]->GetXaxis()->SetLabelSize(0.05);
      h_charge[i]->GetXaxis()->SetTitleOffset(1.);
      h_charge[i]->GetYaxis()->SetTitle("Counts");
      h_charge[i]->GetYaxis()->SetTitleSize(0.05);
      h_charge[i]->GetYaxis()->SetLabelSize(0.05);
      h_charge[i]->GetYaxis()->SetTitleOffset(1.);
      h_charge[i]->SetNdivisions(7);
      h_charge[i]->SetFillColor(kBlue-5);
   }
   
   TH1D *h_theta = new TH1D("h_theta","",500,0,70);
   h_theta->GetXaxis()->SetTitle("x (mm)");
   h_theta->GetXaxis()->SetTitleSize(0.05);
   h_theta->GetXaxis()->SetLabelSize(0.05);
   h_theta->GetXaxis()->SetTitleOffset(1.);
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetYaxis()->SetTitleSize(0.05);
   h_theta->GetYaxis()->SetLabelSize(0.05);
   h_theta->GetYaxis()->SetTitleOffset(1.);
   h_theta->SetNdivisions(7);
   h_theta->SetFillColor(kBlue-5);
   
   TF1 *norm = new TF1("norm","gaus",20,70);
   norm->SetParameters(0,0,0);
   
   TCanvas *c = new TCanvas("c","c",1600,200);
   c->Divide(5,1);

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
      if(theta_deg>=60 && theta_deg<70. && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      counter++ ;
      h_theta->Fill(theta_deg);
      for(Int_t row=0; row<NRows; row++){
         x[row] += cl_padMult[row];
         for(Int_t pad=0; pad<cl_padMult[row]; pad++) {
            h_charge[row]->SetBinContent(pads_fired[row][pad], pad_charge[row][pad]);
         }
         max[row] += h_charge[row]->GetMaximum();
         c->cd(row+1);
         h_charge[row]->Draw();
         }
         if(counter<100){
           char titolo[100];
           sprintf(titolo,"ChargeDistrib/Run%d/4He_60_70/chargeDistr_%d.png",run,i);
           c->SaveAs(titolo);                                                                     
         }
         
         for(Int_t i=0; i<NRows; i++){
            TF1 *f = new TF1(Form("f_%d", i), "gaus", 0, 300);
            f->SetParameters(0, 0);
            h_charge[i]->Fit(f,"N","N",0,300);
            fwhm[i] += f->GetParameter(2);
         }
      }else{                                                                                 // TCutG parenthesis
               cout << "I am out of the blob!" << endl;
           }
   }
   
   theta=h_theta->Fit("norm","R+");
   char titolo[100];
   sprintf(titolo,"ChargeDistrib/chargeDistrib_run%d4He_60_70.txt",run);
   outfit.open(titolo);
   outfit << "                  Charge Distribution width (FWHM) - run: " << run << endl << endl;
   outfit << "Theta: " << theta << endl << endl; 
   
   for(Int_t row=0; row<NRows; row++){
      fwhm[row] = fwhm[row]/counter;
      x[row] = x[row]/counter;
      max[row] = max[row]/counter;
      outfit << "sigma row" << row << ": " << fwhm[row] << "   FWHM(2.35*sigma): 2.35*" << fwhm[row] << " = " << 2.35*fwhm[row] << endl;
      outfit << "x row" << row << " (mm): " << 5*x[row] << "   max row" << row << " (ch): " << max[row] << endl << endl;
   }
   
   TCanvas *c1 = new TCanvas ("c1","c1",900,900);
   c1->cd();
   h_theta->Draw();
   


}
