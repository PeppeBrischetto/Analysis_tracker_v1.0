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

void plot_resolution_vs_vthgem(){

//####################################################################################################################
// Needed variables
   Double_t vthgem[6] = {160.,170.,180.,190.,200.,205.};                        // vthgem array, measured in V
   Double_t rate[4] = {20.,250.,1000.,3000.};                                       // rate array, measured in Hz
   
   Double_t resol_2[6] = {1.13476,1.20145,1.34075,1.39399,1.46165,1.54371};                                            // resolution array, measured in mm
   Double_t resol_1[6] = {0.825931,0.839794,0.876337,0.959115,1.0487,1.10781};                                                // resolution array, measured in mm
   Double_t resol_05[6] = {0.69531,0.757098,0.720781,0.849366,0.883151,0.925996};                                                // resolution array, measured in mm
   Double_t resolDiff_2[6] = {1.98156,1.79388,1.77302,1.91663,1.91166,2.35468};                                               // resolution array, measured in mm
   Double_t resolDiff_1[6] = {1.81806,1.74864,1.71765,1.80279,2.05804,2.62855};                                                // resolution array, measured in mm
   Double_t resolDiff_05[6] = {1.85713,1.89264,1.80438,1.75959,1.93756,2.1824};                                               // resolution array, measured in mm
   
   Double_t ex[6] = {0.};                                                       // error on vthgem, we asssume 0 V
   Double_t resol_2_err[6] = {0.00971125,0.0122631,0.0163161,0.0231475,0.0262914,0.0558077};                                            // resolution array, measured in mm
   Double_t resol_1_err[6] = {0.0142351,0.01646,0.0227181,0.0371282,0.0452136,0.166769};                                            // resolution array, measured in mm};
   Double_t resol_05_err[6] = {0.0268578,0.0332214,0.0371714,0.0550562,0.0658262,0.11575};                                             // resolution array, measured in mm
   Double_t resolDiff_2_err[6] = {0.0279707,0.0274246,0.0297931,0.0410062,0.037401,0.21295};                                          // resolution array, measured in mm
   Double_t resolDiff_1_err[6] = {0.0569329,0.0546712,0.0718138,0.0964524,0.123847,0.222153};                                          // resolution array, measured in mm
   Double_t resolDiff_05_err[6] = {0.095774,0.100593,0.108611,0.125685,0.142839,0.363734};                                           // resolution array, measured in mm
   
   for(Int_t i=0; i<6; i++){
      resolDiff_2[i] = resolDiff_2[i]/sqrt(2);
      resolDiff_1[i] = resolDiff_1[i]/sqrt(2);
      resolDiff_05[i] = resolDiff_05[i]/sqrt(2);
      resolDiff_2_err[i] = resolDiff_2_err[i]/sqrt(2);
      resolDiff_1_err[i] = resolDiff_1_err[i]/sqrt(2);
      resolDiff_05_err[i] = resolDiff_05_err[i]/sqrt(2);
   }
   
   TGraphErrors *gr_r2 = new TGraphErrors(6,vthgem,resol_2,ex,resol_2_err);   
   gr_r2->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");  
   gr_r2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r2->SetMarkerColor(kBlue);
   gr_r2->SetMarkerStyle(24);
   gr_r2->SetMarkerSize(1.5);
   gr_r2->SetLineWidth(2);
   gr_r2->SetLineStyle(1);
   gr_r2->SetLineColor(kBlue);
   
   TGraphErrors *gr_r1 = new TGraphErrors(6,vthgem,resol_1,ex,resol_1_err);     
   gr_r1->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");
   gr_r1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r1->SetMarkerColor(kRed);
   gr_r1->SetMarkerStyle(24);
   gr_r1->SetMarkerSize(1.5);
   gr_r1->SetLineWidth(2);
   gr_r1->SetLineStyle(1);
   gr_r1->SetLineColor(kRed);
   
   TGraphErrors *gr_r05 = new TGraphErrors(6,vthgem,resol_05,ex,resol_05_err);     
   gr_r05->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");
   gr_r05->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_r05->GetYaxis()->SetTitle("#deltax (mm)");
   gr_r05->SetMarkerColor(kGreen+2);
   gr_r05->SetMarkerStyle(24);
   gr_r05->SetMarkerSize(1.5);
   gr_r05->SetLineWidth(2);
   gr_r05->SetLineStyle(1);
   gr_r05->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_rDiff2 = new TGraphErrors(6,vthgem,resolDiff_2,ex,resolDiff_2_err);     
   gr_rDiff2->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");
   gr_rDiff2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_rDiff2->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff2->SetMarkerColor(kBlue);
   gr_rDiff2->SetMarkerStyle(26);
   gr_rDiff2->SetMarkerSize(1.5);
   gr_rDiff2->SetLineWidth(2);
   gr_rDiff2->SetLineStyle(1);
   gr_rDiff2->SetLineColor(kBlue);
   
   TGraphErrors *gr_rDiff1 = new TGraphErrors(6,vthgem,resolDiff_1,ex,resolDiff_1_err);     
   gr_rDiff1->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");
   gr_rDiff1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gr_rDiff1->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rDiff1->SetMarkerColor(kRed);
   gr_rDiff1->SetMarkerStyle(26);
   gr_rDiff1->SetMarkerSize(1.5);
   gr_rDiff1->SetLineWidth(2);
   gr_rDiff1->SetLineStyle(1);
   gr_rDiff1->SetLineColor(kRed);
   
   TGraphErrors *gr_rDiff05 = new TGraphErrors(6,vthgem,resolDiff_05,ex,resolDiff_05_err);     
   gr_rDiff05->SetTitle("#deltax vs V_{THGEM} - R = 200 Hz - P = 10.0 mbar");
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
