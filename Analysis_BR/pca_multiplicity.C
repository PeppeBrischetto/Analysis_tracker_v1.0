//################################################################################################################
//#   This macro allows to perform PCA
//#   the runID is required
//#
//################################################################################################################
//#   creatd July 2025 by A. Pitronaci 
//################################################################################################################
//#   updated:
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

void pca_multiplicity(int run){

//################################################################################################################
// Variables
   Double_t pad_mult[NRows] = {0.};
   Double_t charge_mult[NRows] = {0.};
   Double_t phi = 0.;
   Double_t theta = 0.;
   Double_t projx[5] = {0.};
   Double_t projy[5] = {0.};
   
   Double_t invx[5] = {0.};
   Double_t invy[5] = {0.};
   TPrincipal *obj = new TPrincipal(5,"ND");
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//################################################################################################################
// Data loop for PCA
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      phi = phi_deg;
      theta = theta_deg;
      
      for(Int_t row=0; row<NRows; row++){
         pad_mult[row] = cl_padMult[row];
         obj->AddRow(pad_mult);
         charge_mult[row] = cl_charge[row];
         obj->AddRow(charge_mult);
      }
      
   }
  
   obj->MakePrincipals();
   obj->Print();
   obj->Test();
   obj->MakeHistograms();

   const TVectorD* eigenvalues = obj->GetEigenValues();
   eigenvalues->Print();
   TH1D *h1 = new TH1D("h1","PCA Eigenvalues",eigenvalues->GetNrows(), 0.5, eigenvalues->GetNrows());
   h1->GetXaxis()->SetTitle("Principal component");
   h1->GetYaxis()->SetTitle("Eigenvalue");
   h1->GetXaxis()->SetNdivisions(5);
   
   for(Int_t e=0; e<eigenvalues->GetNrows(); e++){
      h1->SetBinContent(e+1,(*eigenvalues)[e]);
   }
   
   
   obj->X2P(projx,projy);
   obj->P2X(projy,projx,3);
   TGraph *g_pca = new TGraph(5,projx,projy);   
   g_pca->SetTitle("PCA projections - P1 & P2");
   g_pca->SetMarkerStyle(20);
   TGraph *g_pca_inv = new TGraph(5,projy,projx);   
   g_pca_inv->SetTitle("PCA inverse projections");
   g_pca_inv->SetMarkerStyle(20);
   
   TH2D *h2 = new TH2D("h2","multiplicity vs PC",5e1,-2.5e5,2.5e5,5e1,-2.5e5,2.5e5);
   h2->Fill(projx[0],projy[1]);
//################################################################################################################
// Visualization block
   
   TCanvas *c = new TCanvas("c","",1800,800);
   c->Divide(2,1);
   c->cd(1);
   g_pca->Draw("AP");
   c->cd(2);
   g_pca_inv->Draw("AP");
   
   TCanvas *c1 = new TCanvas("c1","",900,800);
   c1->cd();
   h1->Draw();
   
   TCanvas *c2 = new TCanvas("c2","",900,800);
   c2->cd();
   h2->Draw("COLZ");

}
