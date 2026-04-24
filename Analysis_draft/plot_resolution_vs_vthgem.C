//####################################################################################################################
//#   This macro allows to plot the resolution vs the Rate on the x_{i} measured on the padPlane opening virtual slits
//#   on the sensing rows of the gas tracker prorotype (0 & 4, respectively). Tis is the last macro of the suite for 
//#   the resolutions studies and takes as inputs the results obtained before and properly collected in the resolution
//#   output files.
//####################################################################################################################
//#   Created March 25026 by A. Pitronaci 
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

void plot_resolution_vs_vthgem(){

//####################################################################################################################
// Needed variables
   Double_t vthgem[3] = {180.,190.,200.};                                                                  // vthgem array, measured in V
   Double_t rate[4] = {250.,250.,1000.,3000.};                                                                             // rate array, measured in Hz
   
   Double_t resol_2[3] = {1.65366,1.64961,1.92137};                                         // resolution array, measured in mm
   Double_t resol_1[3] = {1.46443,1.40104,1.64888};                                        // resolution array, measured in mm
   Double_t resol_05[3] = {1.37555,1.31257,1.53078};                                       // resolution array, measured in mm
   Double_t resolDiff_2[3] = {5.23228,3.25367,3.17729};                                       // resolution array, measured in mm
   Double_t resolDiff_1[3] = {5.12646,3.17272,3.06004};                                      // resolution array, measured in mm
   Double_t resolDiff_05[3] = {5.19556,3.17651,2.91767};                                       // resolution array, measured in mm
   
   Double_t ex[3] = {0.};                                                                                                 // error on vthgem, we asssume 0 V
   Double_t resol_2_err[3] = {0.00683122,0.00537185,0.00650801};                        // resolution array, measured in mm
   Double_t resol_1_err[3] = {0.0128351,0.00834968,0.0112605};                                 // resolution array, measured in mm
   Double_t resol_05_err[3] = {0.0250575,0.0149366,0.0219655};                              // resolution array, measured in mm
   Double_t resolDiff_2_err[3] = {0.0267376,0.0157314,0.0163145};                            // resolution array, measured in mm
   Double_t resolDiff_1_err[3] = {0.0508774,0.0323898,0.0315933};                          // resolution array, measured in mm
   Double_t resolDiff_05_err[3] = {0.124439,0.0723433,0.0641303};                           // resolution array, measured in mm
   
   for(Int_t i=0; i<3; i++){
      resolDiff_2[i] = resolDiff_2[i]/sqrt(2);
      resolDiff_1[i] = resolDiff_1[i]/sqrt(2);
      resolDiff_05[i] = resolDiff_05[i]/sqrt(2);
      resolDiff_2_err[i] = resolDiff_2_err[i]/sqrt(2);
      resolDiff_1_err[i] = resolDiff_1_err[i]/sqrt(2);
      resolDiff_05_err[i] = resolDiff_05_err[i]/sqrt(2);
   }
   
   TGraphErrors *gr_r2 = new TGraphErrors(3,vthgem,resol_2,ex,resol_2_err);   
   gr_r2->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");  
   gr_r2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r2->SetMarkerColor(kBlue);
   gr_r2->SetMarkerStyle(24);
   gr_r2->SetMarkerSize(1.5);
   gr_r2->SetLineWidth(2);
   gr_r2->SetLineStyle(1);
   gr_r2->SetLineColor(kBlue);
   
   TGraphErrors *gr_r1 = new TGraphErrors(3,vthgem,resol_1,ex,resol_1_err);     
   gr_r1->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");
   gr_r1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r1->SetMarkerColor(kRed);
   gr_r1->SetMarkerStyle(24);
   gr_r1->SetMarkerSize(1.5);
   gr_r1->SetLineWidth(2);
   gr_r1->SetLineStyle(1);
   gr_r1->SetLineColor(kRed);
   
   TGraphErrors *gr_r05 = new TGraphErrors(3,vthgem,resol_05,ex,resol_05_err);     
   gr_r05->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");
   gr_r05->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r05->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r05->SetMarkerColor(kGreen+2);
   gr_r05->SetMarkerStyle(24);
   gr_r05->SetMarkerSize(1.5);
   gr_r05->SetLineWidth(2);
   gr_r05->SetLineStyle(1);
   gr_r05->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_rDiff2 = new TGraphErrors(3,vthgem,resolDiff_2,ex,resolDiff_2_err);     
   gr_rDiff2->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");
   gr_rDiff2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_rDiff2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff2->SetMarkerColor(kBlue);
   gr_rDiff2->SetMarkerStyle(26);
   gr_rDiff2->SetMarkerSize(1.5);
   gr_rDiff2->SetLineWidth(2);
   gr_rDiff2->SetLineStyle(1);
   gr_rDiff2->SetLineColor(kBlue);
   
   TGraphErrors *gr_rDiff1 = new TGraphErrors(3,vthgem,resolDiff_1,ex,resolDiff_1_err);     
   gr_rDiff1->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");
   gr_rDiff1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_rDiff1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff1->SetMarkerColor(kRed);
   gr_rDiff1->SetMarkerStyle(26);
   gr_rDiff1->SetMarkerSize(1.5);
   gr_rDiff1->SetLineWidth(2);
   gr_rDiff1->SetLineStyle(1);
   gr_rDiff1->SetLineColor(kRed);
   
   TGraphErrors *gr_rDiff05 = new TGraphErrors(3,vthgem,resolDiff_05,ex,resolDiff_05_err);     
   gr_rDiff05->SetTitle("#deltax vs V_{THGEM} - R = 2000 Hz - P = 10.0 mbar");
   gr_rDiff05->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_rDiff05->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff05->SetMarkerColor(kGreen+2);
   gr_rDiff05->SetMarkerStyle(26);
   gr_rDiff05->SetMarkerSize(1.5);
   gr_rDiff05->SetLineWidth(2);
   gr_rDiff05->SetLineStyle(1);
   gr_rDiff05->SetLineColor(kGreen+2);

//####################################################################################################################
// Visualization block

   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.03);
   //l->SetLineWidth(0);
   l->AddEntry(gr_r2, "x_{fit}|_{z=z_{0}} - Virtual slit 2.0 mm", "p");
   l->AddEntry(gr_r1, "x_{fit}|_{z=z_{0}} - Virtual slit 1.0 mm", "p");
   l->AddEntry(gr_r05, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm", "p");
   l->AddEntry(gr_rDiff2, "|x_{3}-x_{2}| - Virtual slit 2.0 mm", "p");
   l->AddEntry(gr_rDiff1, "|x_{3}-x_{2}| - Virtual slit 1.0 mm", "p");
   l->AddEntry(gr_rDiff05, "|x_{3}-x_{2}| - Virtual slit 0.5 mm (*)", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   gr_r2->Draw("APL");
   gr_r2->GetYaxis()->SetRangeUser(0.3,1.8);
   gr_r1->Draw("PL && SAME");
   gr_r05->Draw("PL && SAME");
   gr_rDiff2->Draw("PL && SAME");
   gr_rDiff1->Draw("PL && SAME");
   gr_rDiff05->Draw("PL && SAME");
   l->Draw("SAME");

}
