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

const TString CutFileA = "TCutG/alpha_tcut_run79.root";
const TString CutFileC = "TCutG/c_tcut_run79.root";
const Int_t NRows = 5;

void C_plotTheta_rangeTracks(int run){

//################################################################################################################
// Required variables
   Double_t vartheta = 0;
   Double_t mean_theta = 0;
   Double_t max = 0;
   Double_t theta1 = 0;
   Double_t theta2 = 0;
   Double_t delta_theta = 0;
   TH1D *h_theta = new TH1D("h_theta","",1000,50,80);
   h_theta->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->SetLineColor(kBlue);
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
//################################################################################################################
// Loop on the tree entries to get theta and filling the TH1D object

   Double_t inf,sup = 0;
   inf = 2*entries/3;
   sup = entries;
   
   for(Int_t i= inf; i<sup; i++){
      tree->GetEntry(i);
      vartheta=theta*180/Pi();
      //cout << "theta: " << vartheta << endl;                             // control line: print vartheta(deg)
      h_theta->Fill(vartheta);
   }
   
   // h_theta gaussian fit
   TF1 *g = new TF1("g","gaus",50,80);
   g->SetParameters(1,1,1);
   /*h_theta->Fit(g,"+","",50,80);
   mean_theta=g->GetParameter(1);
   max = g->Eval(mean_theta);
   theta1 = g->GetX(max/2,0,mean_theta);
   theta2 = g->GetX(max/2,mean_theta,80);
   delta_theta = theta2-theta1;
   cout << "mean_theta: " << mean_theta << "	max: " << max << "	theta1: " << theta1 << "	theta2: " << theta2 << "	delta_theta: " << delta_theta << endl;                             // control line: print max in theta distribution
   TF1 *fwhm = new TF1("fwhm","[0]",50,80);
   fwhm->FixParameter(0,max/2); 
   fwhm->SetLineColor(kGreen+2);*/
   
//###00#############################################################################################################
// Visualization block
   TCanvas *c = new TCanvas();
   h_theta->Draw();
   //fwhm->Draw("same");
}
