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

void chargeFWHM(int run){

   char histoname[100];
   Double_t fwhm[NRows] = {0.};
   Double_t rms[NRows] = {0.};
   ofstream outfit;

   TH1D *h_charge[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_charge %i",i);
      h_charge[i]=new TH1D("","",500,0.,300.);
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
   
   TCanvas *c = new TCanvas("c","c",1600,200);
   c->Divide(5,1);

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,33,41);
   cutGli->SetPoint(1,135,165);
   cutGli->SetPoint(2,115,176);
   cutGli->SetPoint(3,20,53);
   cutGli->SetPoint(4,33,41);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,25,16);
   cutGa->SetPoint(1,195,207);
   cutGa->SetPoint(2,173,216);
   cutGa->SetPoint(3,13,22);
   cutGa->SetPoint(4,25,16);
   
//################################################################################################################
// Data loop
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      
      if(/*theta_deg>=50 && theta_deg<60. &&*/ cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row=0; row<NRows; row++){
         h_charge[row]->Fill(cl_x_mm[row]);
         c->cd(row+1);
         h_charge[row]->Draw();
      }
      }                                                                       // TCutG parenthesis
   }
   
   char titolo[100];
   sprintf(titolo,"chargeDistrib_run%d.txt",run);
   outfit.open(titolo);
   outfit << "                  Charge Distribution width (FWHM) - run: " << run << endl << endl; 
   
   for(Int_t i=0; i<NRows; i++){
      rms[i] = h_charge[i]->GetRMS();
      TF1 *f = new TF1(Form("f_%d", i), "gaus", 0, 300);
      f->SetParameters(0, 0);
      h_charge[i]->Fit(f,"N","N",0,300);
      fwhm[i] =f->GetParameter(2);
      outfit << "FWHM row" << i << ": " << fwhm[i] << "   " << "   FWHM(2.35*RMS): 2.35*" << fwhm[i] << " = " << 2.35*fwhm[i] << endl;
   }
   
   


}
