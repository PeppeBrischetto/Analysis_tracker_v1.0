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

void plot_resolution_vs_rate(){

//####################################################################################################################
// Needed variables
   Double_t rate[5] = {20.,200.,1000.,2000.,3000.};                                                          // rate array, measured in Hz
   
   Double_t resol_2[5] = {1.00553,1.16879,1.28275,1.33634};                                            // resolution array, measured in mm
   Double_t resol_1[5] = {0.74615,0.759098,0.823552,0.891991};                                         // resolution array, measured in mm
   Double_t resol_05[5] = {1.06602,1.14485,1.21214,1.53078,1.41256};                                      // resolution array, measured in mm
   Double_t resolDiff_2[5] = {1.08613,1.12722,1.10526,1.19782};                                        // resolution array, measured in mm
   Double_t resolDiff_1[5] = {1.04758,1.08048,1.03241,1.10741};                                        // resolution array, measured in mm
   Double_t resolDiff_05[5] = {2.99258,3.45285,2.77538,2.91767,2.81724};                                        // resolution array, measured in mm
   
   Double_t ex[5] = {0.};                                                                              // error on vthgem, we asssume 0 V
   Double_t resol_2_err[5] = {0.0138078,0.0130585,0.00625376,0.00197921};                              // resolution array, measured in mm
   Double_t resol_1_err[5] = {0.0187307,0.0193095,0.00861552,0.00304904};                              // resolution array, measured in mm
   Double_t resol_05_err[5] = {0.0315976,0.049545,0.0284391,0.0219655,0.0167541};                               // resolution array, measured in mm
   Double_t resolDiff_2_err[5] = {0.0216694,0.017685,0.00768981,0.00263261};                           // resolution array, measured in mm
   Double_t resolDiff_1_err[5] = {0.0680556,0.0351187,0.0130178,0.00427};                              // resolution array, measured in mm
   Double_t resolDiff_05_err[5] = {0.165744,0.300532,0.107473,0.0641303,0.0488879};                           // resolution array, measured in mm
   
   for(Int_t i=0; i<4; i++){
      resolDiff_2[i] = resolDiff_2[i]/sqrt(2);
      resolDiff_1[i] = resolDiff_1[i]/sqrt(2);
      resolDiff_05[i] = resolDiff_05[i]/sqrt(2);
      resolDiff_2_err[i] = resolDiff_2_err[i]/sqrt(2);
      resolDiff_1_err[i] = resolDiff_1_err[i]/sqrt(2);
      resolDiff_05_err[i] = resolDiff_05_err[i]/sqrt(2);
   }
   
   TGraphErrors *gr_r2 = new TGraphErrors(4,rate,resol_2,ex,resol_2_err);   
   gr_r2->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 20.0 mbar");  
   gr_r2->GetXaxis()->SetTitle("R (Hz)");
   gr_r2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r2->SetMarkerColor(kBlue);
   gr_r2->SetMarkerStyle(24);
   gr_r2->SetMarkerSize(1.5);
   gr_r2->SetLineWidth(2);
   gr_r2->SetLineStyle(1);
   gr_r2->SetLineColor(kBlue);
   
   TGraphErrors *gr_r1 = new TGraphErrors(4,rate,resol_1,ex,resol_1_err);     
   gr_r1->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 20.0 mbar");
   gr_r1->GetXaxis()->SetTitle("R (Hz)");
   gr_r1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r1->SetMarkerColor(kRed);
   gr_r1->SetMarkerStyle(24);
   gr_r1->SetMarkerSize(1.5);
   gr_r1->SetLineWidth(2);
   gr_r1->SetLineStyle(1);
   gr_r1->SetLineColor(kRed);
   
   TGraphErrors *gr_r05 = new TGraphErrors(5,rate,resol_05,ex,resol_05_err);     
   gr_r05->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 10.0 mbar");
   gr_r05->GetXaxis()->SetTitle("R (Hz)");
   gr_r05->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r05->SetMarkerColor(kBlue);
   gr_r05->SetMarkerStyle(20);
   gr_r05->SetMarkerSize(1.5);
   gr_r05->SetLineWidth(2);
   gr_r05->SetLineStyle(1);
   gr_r05->SetLineColor(kBlue);
   
   TGraphErrors *gr_rDiff2 = new TGraphErrors(4,rate,resolDiff_2,ex,resolDiff_2_err);     
   gr_rDiff2->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 20.0 mbar");
   gr_rDiff2->GetXaxis()->SetTitle("R (Hz)");
   gr_rDiff2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff2->SetMarkerColor(kBlue);
   gr_rDiff2->SetMarkerStyle(24);
   gr_rDiff2->SetMarkerSize(1.5);
   gr_rDiff2->SetLineWidth(2);
   gr_rDiff2->SetLineStyle(1);
   gr_rDiff2->SetLineColor(kBlue);
   
   TGraphErrors *gr_rDiff1 = new TGraphErrors(4,rate,resolDiff_1,ex,resolDiff_1_err);     
   gr_rDiff1->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 20.0 mbar");
   gr_rDiff1->GetXaxis()->SetTitle("R (Hz)");
   gr_rDiff1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff1->SetMarkerColor(kRed);
   gr_rDiff1->SetMarkerStyle(26);
   gr_rDiff1->SetMarkerSize(1.5);
   gr_rDiff1->SetLineWidth(2);
   gr_rDiff1->SetLineStyle(1);
   gr_rDiff1->SetLineColor(kRed);
   
   TGraphErrors *gr_rDiff05 = new TGraphErrors(5,rate,resolDiff_05,ex,resolDiff_05_err);     
   gr_rDiff05->SetTitle("#deltax vs R - V_{THGEM} = 200 V - P = 10.0 mbar");
   gr_rDiff05->GetXaxis()->SetTitle("R (Hz)");
   gr_rDiff05->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff05->SetMarkerColor(kRed);
   gr_rDiff05->SetMarkerStyle(20);
   gr_rDiff05->SetMarkerSize(1.5);
   gr_rDiff05->SetLineWidth(2);
   gr_rDiff05->SetLineStyle(1);
   gr_rDiff05->SetLineColor(kRed);

//####################################################################################################################
// Visualization block

   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.03);
   //l->SetLineWidth(0);
   //l->AddEntry(gr_r2, "x_{fit}|_{z=z_{0}} - Virtual slit 2.0 mm", "p");
   //l->AddEntry(gr_r1, "x_{fit}|_{z=z_{0}} - Virtual slit 1.0 mm", "p");
   
   //l->AddEntry(gr_rDiff2, "|x_{3}-x_{2}| - Virtual slit 2.0 mm", "p");
   //l->AddEntry(gr_rDiff1, "|x_{3}-x_{2}| - Virtual slit 1.0 mm", "p");
   l->AddEntry(gr_rDiff05, "|x_{3}-x_{2}| vs R - V_{THGEM} = 200 V - P = 10.0 mbar", "p");
   l->AddEntry(gr_r05, "#deltax vs R - V_{THGEM} = 200 V - P = 10.0 mbar", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   //gr_r2->Draw("APL");
   //gr_r2->GetYaxis()->SetRangeUser(0.5,1.4);
   //gr_r1->Draw("PL && SAME");
   gr_r05->Draw("APL");
   //gr_rDiff2->Draw("PL && SAME");
   //gr_rDiff1->Draw("PL && SAME");
   gr_rDiff05->Draw("PL && SAME");
   l->Draw("SAME");

}
