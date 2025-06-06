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
#include <TMath.h>

#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const TString CutFileA = "TCutG/alpha_tcut_run79.root";
const TString CutFileC = "TCutG/c_tcut_run79.root";
const Int_t NRows = 5;
const Int_t NEvts = 1000000000;
const Double_t rate = 1600.;                                                // Rate in Run175 expressed in Hz

void C_plotTheta_vs_time(int run){

//################################################################################################################
// Required variables
   char key;
   Int_t npoint = 0;
   Double_t vartheta = 0;
   Double_t discr = 0.;
   Double_t mean_theta = 0.;
   Double_t max = 0.;
   Double_t theta1 = 0.;
   Double_t theta2 = 0.;
   Double_t delta_theta = 0.;
   Int_t time = 0;
   Double_t pulse = 0;
   TH1D *h_theta = new TH1D("h_theta","",500,55,75);
   TH1D *h_thetaSiC = new TH1D("h_thetaSic","",1000,50,80);
   h_theta->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->SetLineColor(kBlue);
   h_theta->GetXaxis()->SetRangeUser(63,66);
   TF1 *g = new TF1("g","gaus",50,80);
   g->SetParameters(1,1,1);
   TCanvas *c = new TCanvas("c","c");
   TCanvas *c1 = new TCanvas("c1","c1");
   TGraph *thetaTime = new TGraph();
   thetaTime->GetXaxis()->SetTitle("Evt (#)");
   thetaTime->GetYaxis()->SetTitle("Pulse (0 - 1)");
   thetaTime->SetMarkerColor(kBlue);
   thetaTime->GetXaxis()->SetRangeUser(0,1E4);
   thetaTime->SetLineWidth(1);
   thetaTime->SetMarkerSize(1.2);
   thetaTime->SetMarkerStyle(20);
   thetaTime->SetLineColor(kBlue);
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
//################################################################################################################
// Loop on the tree entries to get theta and filling the TH1D object

   for(Int_t i= 0; i<entries; i++){
      tree->GetEntry(i);
      vartheta = theta_deg;
      //cout << "theta: " << vartheta << endl;                             // control line: print vartheta(deg)
      h_theta->Fill(theta_deg);
      if(sic_charge>2000){
         h_thetaSiC->Fill(theta_deg);
      }
      h_theta->Fit("g","N","",55,70);
      mean_theta = g->GetParameter(1);
      discr = g->GetParameter(2);
      cout << "mean_theta: " << mean_theta << endl;
      c->cd();
      h_theta->Draw();
      h_thetaSiC->SetLineColor(kRed); 
      h_thetaSiC->Draw("same");
      /*cout << "Press a key to continue: " << endl;
      cin >> key;
      switch(key){
         case 'q': return;
         case 'u': c->Update();
         case 'e': break;
      }*/
      c->Update();
      if(i>2 && Abs(mean_theta-vartheta)>2.35*discr){
         cout << "Discrepancy reached "<< discr << " - Evt. " << i << endl;
         cout << "Press 'q' to quit, 'c' to continue: " << key;
         cin >> key;
         time = i/rate;
         pulse = 1;
         thetaTime->SetPoint(i,i,pulse);
         switch(key){
            case 'q': return;
            case 'c': continue;
         }
         npoint++;
         c1->cd();
         thetaTime->Draw();
      }else{
         time = i/rate;
         pulse = 0;
         thetaTime->SetPoint(i,i,pulse);
      }
      vartheta = mean_theta = 0.;
      npoint++;
      c1->cd();
      thetaTime->Draw();
   }
   // h_theta gaussian fit
   /*TF1 *g = new TF1("g","gaus",50,80);
   g->SetParameters(1,1,1);
   h_theta->Fit(g,"+","",50,80);
   mean_theta=g->GetParameter(1);
   max = g->Eval(mean_theta);
   theta1 = g->GetX(max/2,0,mean_theta);
   theta2 = g->GetX(max/2,mean_theta,80);
   delta_theta = theta2-theta1;
   cout << "mean_theta: " << mean_theta << "	max: " << max << "	theta1: " << theta1 << "	theta2: " << theta2 << "	delta_theta: " << delta_theta << endl;                             // control line: print max in theta distribution
   TF1 *fwhm = new TF1("fwhm","[0]",50,80);
   fwhm->FixParameter(0,max/2); 
   fwhm->SetLineColor(kGreen+2);
   
//###00#############################################################################################################
// Visualization block
   TCanvas *c = new TCanvas("c","c");
   h_theta->Draw();
   h_thetaSiC->SetLineColor(kRed);
   h_thetaSiC->Draw("same");*/
   //fwhm->Draw("same");
   
   c1->Update();
}
