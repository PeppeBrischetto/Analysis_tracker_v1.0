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

void C_plotTheta_vs_mult(int run){

//################################################################################################################
// Required variables
   char histoname[200];
   char title[100];
   Int_t index0 = 0;
   Int_t index4 = 0;
   Double_t vartheta = 0;
   Double_t mean_theta = 0;
   Double_t max = 0;
   Double_t theta1 = 0;
   Double_t theta2 = 0;
   Double_t delta_theta = 0;
   TH2D *h_theta[5];
   for(Int_t j=0; j<5; j++){
      sprintf(histoname,"Charge Distrib. - row%d",j);
      sprintf(title,"Pad mult.%d",j);
      h_theta[j]=new TH2D("h_theta","",1000,40,75,16,-0.5,15.5);
      h_theta[j]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta[j]->GetYaxis()->SetTitle(title);
      h_theta[j]->SetFillColor(kBlue);
   }
   TH2D *h_thetaSiC[5];
   for(Int_t j=0; j<5; j++){
      sprintf(histoname,"Charge Distrib. - row%d",j);
      sprintf(title,"Pad mult.%d",j);
      h_thetaSiC[j]=new TH2D("h_theta","",1000,40,75,16,-0.5,15.5);
      h_thetaSiC[j]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_thetaSiC[j]->GetYaxis()->SetTitle(title);
      h_thetaSiC[j]->SetFillColor(kRed);
   } 
   
   TH2D *h_theta_padExc[5];
   for(Int_t j=0; j<5; j++){
      sprintf(histoname,"Charge Distrib. - row%d",j);
      sprintf(title,"Pad mult.%d",j);
      h_theta_padExc[j]=new TH2D("h_theta","",1000,40,75,16,-0.5,15.5);
      h_theta_padExc[j]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta_padExc[j]->GetYaxis()->SetTitle(title);
      h_theta_padExc[j]->SetFillColor(kGreen+2);
   } 
   
   
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
      for(Int_t r=0; r<NRows; r++){
         h_theta[r]->Fill(theta_deg,cl_padMult[r]);
      }
      if(sic_charge>2000){
        for(Int_t r=0; r<NRows; r++){
           h_thetaSiC[r]->Fill(theta_deg,cl_padMult[r]);
        }
      }
      index0 = cl_padMult[0];
      index4=cl_padMult[4];
      if(sic_charge>2000 && pads_fired[0][index0]>6 && pads_fired[4][index4]<52){
        for(Int_t r=0; r<NRows; r++){
           h_theta_padExc[r]->Fill(theta_deg,cl_padMult[r]);
        }
      }
   }
   
//###00#############################################################################################################
// Visualization block
   
   TLegend *l = new TLegend(0.1,0.75,0.45,0.9);
   l->AddEntry(h_theta[0],"All evts","f");
   l->AddEntry(h_thetaSiC[0],"E_{SiC}>2000 a.u.","f");
   l->AddEntry(h_theta_padExc[0],"Pad #in [4;56]","f");
   TCanvas *c = new TCanvas("c","c");
   c->Divide(3,2);
   c->cd(1);
   h_theta[0]->Draw("box");
   h_thetaSiC[0]->Draw("box && same");
   h_theta_padExc[0]->Draw("box && same");
   l->Draw("same");
   c->cd(2);
   h_theta[1]->Draw("box");
   h_thetaSiC[1]->Draw("box && same");
   h_theta_padExc[1]->Draw("box && same");
   l->Draw("same");
   c->cd(3);
   h_theta[2]->Draw("box");
   h_thetaSiC[2]->Draw("box && same");
   h_theta_padExc[2]->Draw("box && same");
   l->Draw("same");
   c->cd(4);
   h_theta[3]->Draw("box");
   h_thetaSiC[3]->Draw("box && same");
   l->Draw("same");
   h_theta_padExc[3]->Draw("box && same");
   c->cd(5);
   h_theta[4]->Draw("box");
   h_thetaSiC[4]->Draw("box && same");
   h_theta_padExc[4]->Draw("box && same");
   l->Draw("same");
}
