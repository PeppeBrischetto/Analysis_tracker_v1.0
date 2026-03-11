//####################################################################################################################
//#   This macro allows to plot the resolution vs the Rate on the x_{i} measured on the padPlane opening virtual slits
//#   on the sensing rows of the gas tracker prorotype (0 & 4, respectively). Tis is the last macro of the suite for 
//#   the resolutions studies and takes as inputs the results obtained before and properly collected in the resolution
//#   output files.
//####################################################################################################################
//#   Created March 2026 by A. Pitronaci 
//####################################################################################################################
//#   Updated:
//####################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TLatex.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfilesthresh.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void plot_resolution_vs_primaries(){

//####################################################################################################################
// Needed variables
   Int_t p = 0;
   Double_t np[2] = {95834.,203948.};                                       // Number of primary electrons - obtained dividing energy loss (by LISE++) and the average ioization potential (23.4 eV)
   Double_t sp[2] = {2.98E+08,1.0E+9};                                                     // Number of secondary electrons - obtained converting the digitizer signal (in mV) in # e- -> digi * 61e-3/7.24e-5    
   Double_t resol_2[8] = {1.01453,1.20145,1.37213,1.4087,1.00553,1.16879,1.28275,1.33634};                                            // resolution array, measured in mm
   Double_t resol_1[8] = {0.78438,0.839794,0.955735,0.991787,0.74615,0.759098,0.823552,0.891991};                                                // resolution array, measured in mm
   Double_t resol_05[8] = {0.680787,0.757098,0.780148,0.796688,0.550655,0.540796,0.612535,0.668728};                                               // resolution array, measured in mm
   Double_t resolDiff_2[8] = {1.73435,1.79388,1.71778,1.55589,1.08613,1.12722,1.10526,1.20752};                                              // resolution array, measured in mm
   Double_t resolDiff_1[8] = {1.683,1.74864,1.71677,1.49317,1.04758,1.08048,1.03241,1.10741};                                               // resolution array, measured in mm
   Double_t resolDiff_05[8] = {1.42162,1.89264,1.68736,1.43778,1.10787,1.0605,0.996745,1.10243};                                               // resolution array, measured in mm
   
   Double_t ex[2] = {0.};                                                       // error on vthgem, we asssume 0 V
   Double_t resol_2_err[8] = {0.0121375,0.0122631,0.00601778,0.00283003,0.0138078,0.0130585,0.00625376,0.00197921};                                             // resolution array, measured in mm
   Double_t resol_1_err[8] = {0.0152887,0.01646,0.00943,0.00455997,0.0187307,0.0193095,0.00861552,0.00304904};                                            // resolution array, measured in mm};
   Double_t resol_05_err[8] = {0.0299145,0.0332214,0.0182243,0.00811091,0.0297345,0.0376067,0.0151905,0.00520609};                                              // resolution array, measured in mm
   Double_t resolDiff_2_err[8] = {0.0280179,0.0274246,0.011019,0.00461767,0.0216694,0.017685,0.00768981,0.00257592};                                           // resolution array, measured in mm
   Double_t resolDiff_1_err[8] = {0.0554518,0.0546712,0.0218924,0.00798279,0.0680556,0.0351187,0.0130178,0.00427};                                           // resolution array, measured in mm
   Double_t resolDiff_05_err[8] = {0.1251,0.100593,0.0436291,0.0147473,0.127081,0.0587356,0.0243829,0.00799995};                                          // resolution array, measured in mm
   
   Double_t resol[2] = {0.};
   Double_t resol_err[2] = {0.};
   Double_t resolDiff[2] = {0.};
   Double_t resolDiff_err[2] = {0.};
   
   TGraphErrors *gr_r = new TGraphErrors();     
   gr_r->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_r->GetXaxis()->SetTitle("N_{secondaries}");
   gr_r->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r->SetMarkerColor(kGreen+2);
   gr_r->SetMarkerStyle(24);
   gr_r->SetMarkerSize(1.5);
   gr_r->SetLineWidth(2);
   gr_r->SetLineStyle(1);
   gr_r->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_rDiff = new TGraphErrors();     
   gr_rDiff->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_rDiff->GetXaxis()->SetTitle("N_{secondaries}");
   gr_rDiff->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff->SetMarkerColor(kGreen+2);
   gr_rDiff->SetMarkerStyle(26);
   gr_rDiff->SetMarkerSize(1.5);
   gr_rDiff->SetLineWidth(2);
   gr_rDiff->SetLineStyle(1);
   gr_rDiff->SetLineColor(kGreen+2);
   
   for(Int_t i=0; i<8; i++){
      resolDiff_2[i] = resolDiff_2[i]/sqrt(2);
      resolDiff_1[i] = resolDiff_1[i]/sqrt(2);
      resolDiff_05[i] = resolDiff_05[i]/sqrt(2);
      resolDiff_2_err[i] = resolDiff_2_err[i]/sqrt(2);
      resolDiff_1_err[i] = resolDiff_1_err[i]/sqrt(2);
      resolDiff_05_err[i] = resolDiff_05_err[i]/sqrt(2);
      if(i==3 || i==7){
         cout << "p: " << p << endl; 
         resol[p] = resol_05[i];
         resol_err[p] = resol_05_err[i];  
         resolDiff[p] = resolDiff_05[i];
         resolDiff_err[p] = resolDiff_05_err[i];
         gr_r->SetPoint(p,sp[p],resol[p]);
         gr_r->SetPointError(p,ex[p],resol_err[p]);
         gr_rDiff->SetPoint(p,sp[p],resolDiff[p]);
         gr_rDiff->SetPointError(p,ex[p],resolDiff_err[p]);
         p++;
      }
   }

//####################################################################################################################
// Visualization block

   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.03);
   //l->SetLineWidth(0);
   l->AddEntry(gr_r, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm", "p");
   //l->AddEntry(gr_r1, "x_{fit}|_{z=z_{0}} - Virtual slit 1.0 mm", "p");
   //l->AddEntry(gr_r05, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm", "p");
   l->AddEntry(gr_rDiff, "|x_{3}-x_{2}| - Virtual slit 0.5 mm", "p");
   //l->AddEntry(gr_rDiff1, "|x_{3}-x_{2}| - Virtual slit 1.0 mm", "p");
   //l->AddEntry(gr_rDiff05, "|x_{3}-x_{2}| - Virtual slit 0.5 mm (*)", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   gr_r->Draw("APL");
   gr_r->GetYaxis()->SetRangeUser(0.5,1.4);
   gr_rDiff->Draw("PL && SAME");
   l->Draw("SAME");

}
