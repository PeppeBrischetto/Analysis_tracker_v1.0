//################################################################################################################
//#   plot of theta distribution for all the tracks acquired during measurements
//#   before use this macro, be sure you have got the TCut cl_x[1]:cl_x[0] (is necessary, for runs 7Li + alpha as well as for 12C, to delete uncorrelated events).
//#   have a look to the file "README_dir.txt" for detailed information
//#
//################################################################################################################
//#   creatd May 2025 by A. Pitronaci 
//################################################################################################################
//#   updated:
//################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>

#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NPads = 100;

void C_plotTheta_padExclusion(int run){

//################################################################################################################
// Required variables
   Double_t vartheta = 0;
   Double_t vartheta_padExc = 0.;
   Double_t mean_theta = 0;
   Double_t max = 0;
   Double_t theta1 = 0;
   Double_t theta2 = 0;
   Double_t delta_theta = 0;
   Double_t padExc = 0.;
   Int_t index0 = 0;
   Int_t index4 = 0;
   TH1D *h_theta = new TH1D("h_theta","",1000,50,80);
   TH1D *h_thetaSiC = new TH1D("h_thetaSic","",1000,50,80);
   h_thetaSiC->SetLineColor(kRed);
   h_theta->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->SetLineColor(kBlue);
   TH1D *h_theta_padExc = new TH1D("h_theta_padExc","",1000,50,80);
   h_theta_padExc->SetLineColor(kGreen+2);
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
//################################################################################################################
// Loop on the tree entries to get theta and filling the TH1D object

   for(Int_t i= 0; i<entries; i++){
      tree->GetEntry(i);
      vartheta=theta*180/Pi();
      //cout << "theta: " << vartheta << endl;                             // control line: print vartheta(deg)
      h_theta->Fill(theta_deg);
      if(sic_charge>2000){
        index0+=1;
        h_thetaSiC->Fill(theta_deg);
      }
      index0 = cl_padMult[0];
      index4=cl_padMult[4];
      if(sic_charge>2000 && pads_fired[0][index0]>2 && pads_fired[4][index4]<58){
        h_theta_padExc->Fill(theta_deg);
      }
   }
   // h_theta gaussian fit
   TF1 *g = new TF1("g","gaus",50,80);
   g->SetParameters(1,1,1);
   h_theta->Fit(g,"","",50,80);
   mean_theta=g->GetParameter(1);
   max = g->Eval(mean_theta);
   theta1 = g->GetX(max/2,0,mean_theta);
   theta2 = g->GetX(max/2,mean_theta,80);
   delta_theta = theta2-theta1;
   cout << "mean_theta: " << mean_theta << "	max: " << max << "	theta1: " << theta1 << "	theta2: " << theta2 << "	delta_theta: " << delta_theta << "	index: " << index0 << endl;                            
   TF1 *fwhm = new TF1("fwhm","[0]",50,80);
   fwhm->FixParameter(0,max/2); 
   fwhm->SetLineColor(kGreen+2);
   
//###00#############################################################################################################
// Visualization block
   TCanvas *c = new TCanvas("c","c");
   h_theta->Draw();
   h_thetaSiC->Draw("same");
   h_theta_padExc->Draw("same");
   //fwhm->Draw("same");
   
}
