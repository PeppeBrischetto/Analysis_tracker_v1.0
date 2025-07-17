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
const Int_t NPads = 60;

void trackControl(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6,39.8,61.0,82.2,103.4};
   /* 2D-histogram to map the anode */
   TH2D *anode=new TH2D("anode","anode",60,-0.5,59.5,11,-0.5,10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);
   
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
//################################################################################################################
// Data loop
   for(Int_t i=6; i<7; i++){
      Double_t theta_fit = 0.;
      Double_t pad[NRows][100] = {0.};
      Double_t charge[NRows][100] = {0.};
      Double_t x[NRows] = {0.};
      Double_t y[NRows] = {0.};
      Double_t totalCharge[NRows];
      
      TCanvas *c = new TCanvas("c");
      TCanvas *c1 = new TCanvas("c1");
    
      tree->GetEntry(i);
      cout << "Evt: " << i << "   sic_fired: " << sic_fired << "   sic_charge: " << sic_charge << endl;
      for(Int_t row=0; row<NRows; row++){
         cout << "   Row: " << row << endl;
         for(Int_t p=0; p<cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            cout << "      Pads: " << pad[row][p] << "    Charge: " << charge[row][p] << endl;
            anode->Fill(pad[row][p],2*(row)+1,charge[row][p]);
            x[row] += (5*pad[row][p]*charge[row][p]);
         }
         x[row] = x[row]/(totalCharge[row]);
         cout << "      x[" << row << "]: " << x[row] << "   y[" << row << "]: " << cl_y_mm[row] << endl;
         
      }
      
      TF1 *f = new TF1("f","[0] + [1]*x",0,300);
      f->SetParameters(0,0);
      TGraph *retta = new TGraph(5,x,z);
      retta->SetMarkerStyle(20);
      retta->SetLineWidth(0);
      retta->GetXaxis()->SetTitle("x (mm)");
      retta->GetYaxis()->SetTitle("z (mm)");
      retta->Fit(f,"","+",0,300);
      theta_fit = 90-ATan(f->GetParameter(1))*180/Pi();
      
      c->cd();
      anode->Draw("COLZ");
      c1->cd();
      retta->Draw();
      thetaDeg = ATan((x[4]-x[0])/84.8)*180/Pi();
      cout << "   theta: " << thetaDeg << "°   theta_fit: " << theta_fit << "   thetaTrack_Br: " << theta_deg << endl;
      cout << "***************************************************************\n";
      
      
   }
   
   
}
