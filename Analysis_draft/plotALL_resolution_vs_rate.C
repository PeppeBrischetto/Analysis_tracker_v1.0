//####################################################################################################################
//#   This macro allows to plot the resolution vs the Rate on the x_{i} measured on the padPlane opening virtual slits
//#   on the sensing rows of the gas tracker prorotype (0 & 4, resp_cectively). Tis is the last macro of the suite for 
//#   the resolutions studies and takes as inp_cuts the results obtained before and properly collected in the resolution
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
const Int_t np_cads = 60;

void plotALL_resolution_vs_rate(){

//####################################################################################################################
// Needed variables
   Int_t p = 0;
   Double_t rate20[1] = {20};
   Double_t rate200[1] = {200};
   Double_t rate250[1] = {250};
   Double_t rate260[1] = {260};
   Double_t rate300[1] = {300};
   Double_t rate1000[1] = {1000};
   Double_t rate2000[1] = {2000};
   Double_t rate3000[1] = {3000};
   
   Double_t resol10_12c_20[1] = {0.680787};
   Double_t resol10_12c_200[1] = {0.757098};
   Double_t resol10_12c_1000[1] = {0.780148};
   Double_t resol10_12c_3000[1] = {0.796688};
   Double_t resol20_12c_20[1] = {0.550655};
   Double_t resol20_12c_200[1] = {0.540796};
   Double_t resol20_12c_1000[1] = {0.612535};
   Double_t resol20_12c_3000[1] = {0.668728};
   Double_t resol10_7li_2000[1] = {1.02632};
   Double_t resol10_16o_200[1] = {0.717419};
   
   Double_t err_resol10_12c_20[1] = {0.0299145};
   Double_t err_resol10_12c_200[1] = {0.0332214};
   Double_t err_resol10_12c_1000[1] = {0.0182243};
   Double_t err_resol10_12c_3000[1] = {0.00811091};
   Double_t err_resol20_12c_20[1] = {0.0297345};
   Double_t err_resol20_12c_200[1] = {0.0376067};
   Double_t err_resol20_12c_1000[1] = {0.0151905};
   Double_t err_resol20_12c_3000[1] = {0.00520609};
   Double_t err_resol10_7li_2000[1] = {0.00957856};
   Double_t err_resol10_16o_200[1] = {0.0136944};
   
   TGraphErrors *gr_10r12c_20 = new TGraphErrors(1,rate20,resol10_12c_20,0,err_resol10_12c_20);     
   gr_10r12c_20->SetTitle("#deltax vs Rate (Hz) - R = 20 Hz");
   gr_10r12c_20->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r12c_20->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r12c_20->SetMarkerColor(kGreen);
   gr_10r12c_20->SetMarkerStyle(20);
   gr_10r12c_20->SetMarkerSize(1.5);
   gr_10r12c_20->SetLineWidth(2);
   gr_10r12c_20->SetLineStyle(1);
   gr_10r12c_20->SetLineColor(kGreen);
   
   TGraphErrors *gr_10r12c_200 = new TGraphErrors(1,rate200,resol10_12c_200,0,err_resol10_12c_200);     
   gr_10r12c_200->SetTitle("#deltax vs Rate (Hz) - R = 200 Hz");
   gr_10r12c_200->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r12c_200->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r12c_200->SetMarkerColor(kGreen+1);
   gr_10r12c_200->SetMarkerStyle(20);
   gr_10r12c_200->SetMarkerSize(1.5);
   gr_10r12c_200->SetLineWidth(2);
   gr_10r12c_200->SetLineStyle(1);
   gr_10r12c_200->SetLineColor(kGreen+1);
   
   TGraphErrors *gr_10r12c_1000 = new TGraphErrors(1,rate1000,resol10_12c_1000,0,err_resol10_12c_1000);     
   gr_10r12c_1000->SetTitle("#deltax vs Rate (Hz) - R = 1000 Hz");
   gr_10r12c_1000->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r12c_1000->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r12c_1000->SetMarkerColor(kGreen+2);
   gr_10r12c_1000->SetMarkerStyle(20);
   gr_10r12c_1000->SetMarkerSize(1.5);
   gr_10r12c_1000->SetLineWidth(2);
   gr_10r12c_1000->SetLineStyle(1);
   gr_10r12c_1000->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_10r12c_3000 = new TGraphErrors(1,rate3000,resol10_12c_3000,0,err_resol10_12c_3000);     
   gr_10r12c_3000->SetTitle("#deltax vs Rate (Hz) - R = 3000 Hz");
   gr_10r12c_3000->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r12c_3000->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r12c_3000->SetMarkerColor(kGreen+3);
   gr_10r12c_3000->SetMarkerStyle(20);
   gr_10r12c_3000->SetMarkerSize(1.5);
   gr_10r12c_3000->SetLineWidth(2);
   gr_10r12c_3000->SetLineStyle(1);
   gr_10r12c_3000->SetLineColor(kGreen+3);
   
   TGraphErrors *gr_10r7li_2000 = new TGraphErrors(1,rate2000,resol10_7li_2000,0,err_resol10_7li_2000);     
   gr_10r7li_2000->SetTitle("#deltax vs Rate (Hz) - R = 2000 Hz");
   gr_10r7li_2000->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7li_2000->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7li_2000->SetMarkerColor(kMagenta+2);
   gr_10r7li_2000->SetMarkerStyle(20);
   gr_10r7li_2000->SetMarkerSize(1.5);
   gr_10r7li_2000->SetLineWidth(2);
   gr_10r7li_2000->SetLineStyle(1);
   gr_10r7li_2000->SetLineColor(kMagenta+2);
   
   TGraphErrors *gr_10r16o_200 = new TGraphErrors(1,rate200,resol10_16o_200,err_resol10_16o_200);     
   gr_10r16o_200->SetTitle("#deltax vs Rate (Hz) - R = 2000 Hz");
   gr_10r16o_200->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r16o_200->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r16o_200->SetMarkerColor(kOrange+2);
   gr_10r16o_200->SetMarkerStyle(20);
   gr_10r16o_200->SetMarkerSize(1.5);
   gr_10r16o_200->SetLineWidth(2);
   gr_10r16o_200->SetLineStyle(1);
   gr_10r16o_200->SetLineColor(kOrange+2);
   
   TGraphErrors *gr_20r12c_20 = new TGraphErrors(1,rate20,resol20_12c_20,0,err_resol10_12c_20);     
   gr_20r12c_20->SetTitle("#deltax vs Rate (Hz) - R = 20 Hz");
   gr_20r12c_20->GetXaxis()->SetTitle("Rate (Hz)");
   gr_20r12c_20->GetYaxis()->SetTitle("#deltax (mm)");
   gr_20r12c_20->SetMarkerColor(kGreen);
   gr_20r12c_20->SetMarkerStyle(20);
   gr_20r12c_20->SetMarkerSize(1.5);
   gr_20r12c_20->SetLineWidth(2);
   gr_20r12c_20->SetLineStyle(1);
   gr_20r12c_20->SetLineColor(kGreen);
   
   TGraphErrors *gr_20r12c_200 = new TGraphErrors(1,rate200,resol20_12c_200,0,err_resol10_12c_200);     
   gr_20r12c_200->SetTitle("#deltax vs Rate (Hz) - R = 200 Hz");
   gr_20r12c_200->GetXaxis()->SetTitle("Rate (Hz)");
   gr_20r12c_200->GetYaxis()->SetTitle("#deltax (mm)");
   gr_20r12c_200->SetMarkerColor(kGreen+1);
   gr_20r12c_200->SetMarkerStyle(20);
   gr_20r12c_200->SetMarkerSize(1.5);
   gr_20r12c_200->SetLineWidth(2);
   gr_20r12c_200->SetLineStyle(1);
   gr_20r12c_200->SetLineColor(kGreen+1);
   
   TGraphErrors *gr_20r12c_1000 = new TGraphErrors(1,rate1000,resol20_12c_1000,0,err_resol10_12c_1000);     
   gr_20r12c_1000->SetTitle("#deltax vs Rate (Hz) - R = 1000 Hz");
   gr_20r12c_1000->GetXaxis()->SetTitle("Rate (Hz)");
   gr_20r12c_1000->GetYaxis()->SetTitle("#deltax (mm)");
   gr_20r12c_1000->SetMarkerColor(kGreen+2);
   gr_20r12c_1000->SetMarkerStyle(20);
   gr_20r12c_1000->SetMarkerSize(1.5);
   gr_20r12c_1000->SetLineWidth(2);
   gr_20r12c_1000->SetLineStyle(1);
   gr_20r12c_1000->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_20r12c_3000 = new TGraphErrors(1,rate3000,resol20_12c_3000,0,err_resol10_12c_3000);     
   gr_20r12c_3000->SetTitle("#deltax vs Rate (Hz) - R = 3000 Hz");
   gr_20r12c_3000->GetXaxis()->SetTitle("Rate (Hz)");
   gr_20r12c_3000->GetYaxis()->SetTitle("#deltax (mm)");
   gr_20r12c_3000->SetMarkerColor(kGreen+3);
   gr_20r12c_3000->SetMarkerStyle(20);
   gr_20r12c_3000->SetMarkerSize(1.5);
   gr_20r12c_3000->SetLineWidth(2);
   gr_20r12c_3000->SetLineStyle(1);
   gr_20r12c_3000->SetLineColor(kGreen+3);
   
//####################################################################################################################   
// THGEM + RIM
   
   Double_t resol10_7li_20R[1] = {1.06602};
   Double_t err_resol10_7li_20R[1] = {0.0315976};
   
   Double_t resol10_7li_300R[1] = {1.14485};
   Double_t err_resol10_7li_300R[1] = {0.049545};
   
   Double_t resol10_7li_1000R[1] = {1.21214};
   Double_t err_resol10_7li_1000R[1] = {0.0284391};
   
   Double_t resol10_7li_2000R[1] = {1.53078};
   Double_t err_resol10_7li_2000R[1] = {0.0219655};
   
   Double_t resol10_7li_3000R[1] = {1.41256};
   Double_t err_resol10_7li_3000R[1] = {0.0167541};
   
   Double_t resol10_12c_260R[1] = {0.795798};
   Double_t err_resol10_12c_260R[1] = {0.0350747};
   
   TGraphErrors *gr_10r7Li_20R = new TGraphErrors(1,rate20,resol10_7li_20R,0,err_resol10_7li_20R);     
   gr_10r7Li_20R->SetTitle("#deltax vs Rate (Hz) - R = 20 Hz");
   gr_10r7Li_20R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7Li_20R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7Li_20R->SetMarkerColor(kCyan-9);
   gr_10r7Li_20R->SetMarkerStyle(20);
   gr_10r7Li_20R->SetMarkerSize(1.5);
   gr_10r7Li_20R->SetLineWidth(2);
   gr_10r7Li_20R->SetLineStyle(1);
   gr_10r7Li_20R->SetLineColor(kCyan-9);
   
   TGraphErrors *gr_10r7Li_300R = new TGraphErrors(1,rate300,resol10_7li_300R,0,err_resol10_7li_300R);     
   gr_10r7Li_300R->SetTitle("#deltax vs Rate (Hz) - R = 300 Hz");
   gr_10r7Li_300R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7Li_300R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7Li_300R->SetMarkerColor(kCyan);
   gr_10r7Li_300R->SetMarkerStyle(20);
   gr_10r7Li_300R->SetMarkerSize(1.5);
   gr_10r7Li_300R->SetLineWidth(2);
   gr_10r7Li_300R->SetLineStyle(1);
   gr_10r7Li_300R->SetLineColor(kCyan);
   
   TGraphErrors *gr_10r7Li_1000R = new TGraphErrors(1,rate1000,resol10_7li_1000R,0,err_resol10_7li_1000R);     
   gr_10r7Li_1000R->SetTitle("#deltax vs Rate (Hz) - R = 1000 Hz");
   gr_10r7Li_1000R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7Li_1000R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7Li_1000R->SetMarkerColor(kCyan+1);
   gr_10r7Li_1000R->SetMarkerStyle(20);
   gr_10r7Li_1000R->SetMarkerSize(1.5);
   gr_10r7Li_1000R->SetLineWidth(2);
   gr_10r7Li_1000R->SetLineStyle(1);
   gr_10r7Li_1000R->SetLineColor(kCyan+1);
   
   TGraphErrors *gr_10r7Li_2000R = new TGraphErrors(1,rate2000,resol10_7li_2000R,0,err_resol10_7li_2000R);     
   gr_10r7Li_2000R->SetTitle("#deltax vs Rate (Hz) - R = 2000 Hz");
   gr_10r7Li_2000R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7Li_2000R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7Li_2000R->SetMarkerColor(kCyan+2);
   gr_10r7Li_2000R->SetMarkerStyle(20);
   gr_10r7Li_2000R->SetMarkerSize(1.5);
   gr_10r7Li_2000R->SetLineWidth(2);
   gr_10r7Li_2000R->SetLineStyle(1);
   gr_10r7Li_2000R->SetLineColor(kCyan+2); 
   
   TGraphErrors *gr_10r7Li_3000R = new TGraphErrors(1,rate3000,resol10_7li_3000R,0,err_resol10_7li_3000R);     
   gr_10r7Li_3000R->SetTitle("#deltax vs Rate (Hz) - R = 3000 Hz");
   gr_10r7Li_3000R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r7Li_3000R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r7Li_3000R->SetMarkerColor(kCyan+3);
   gr_10r7Li_3000R->SetMarkerStyle(20);
   gr_10r7Li_3000R->SetMarkerSize(1.5);
   gr_10r7Li_3000R->SetLineWidth(2);
   gr_10r7Li_3000R->SetLineStyle(1);
   gr_10r7Li_3000R->SetLineColor(kCyan+3); 
   
   TGraphErrors *gr_10r12c_260R = new TGraphErrors(1,rate260,resol10_12c_260R,0,err_resol10_12c_260R);     
   gr_10r12c_260R->SetTitle("#deltax vs Rate (Hz) - R = 260 Hz");
   gr_10r12c_260R->GetXaxis()->SetTitle("Rate (Hz)");
   gr_10r12c_260R->GetYaxis()->SetTitle("#deltax (mm)");
   gr_10r12c_260R->SetMarkerColor(kRed);
   gr_10r12c_260R->SetMarkerStyle(20);
   gr_10r12c_260R->SetMarkerSize(1.5);
   gr_10r12c_260R->SetLineWidth(2);
   gr_10r12c_260R->SetLineStyle(1);
   gr_10r12c_260R->SetLineColor(kRed);  

//####################################################################################################################
// Visualization block

   TH2F *bga=new TH2F("bga","#deltax vs Rate (Hz)",100,0,4000,1000,0.4,4);
   bga->SetStats(0);
   bga->GetXaxis()->SetTitle("Rate (Hz)");
   bga->GetXaxis()->SetTitleSize(0.04);
   bga->GetXaxis()->SetTitleOffset(1.06);
   bga->GetXaxis()->SetLabelSize(0.04);
   bga->GetXaxis()->SetLabelOffset(0.010);
   bga->GetYaxis()->SetTitle("#deltax (mm)");
   bga->GetYaxis()->SetTitleSize(0.04);
   bga->GetYaxis()->SetTitleOffset(1.38);
   bga->GetYaxis()->SetLabelSize(0.04);
   
   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.02);
   l->SetNColumns(2);
   //l->SetLineWidth(0);
   l->AddEntry(gr_10r7li_2000, "x_{fit}|_{z=z_{0}} - ^{7}Li - 2000 Hz - NO RIM", "p");
   l->AddEntry(gr_10r7Li_20R, "x_{fit}|_{z=z_{0}} - ^{7}Li - 20 Hz - RIM", "p");
   l->AddEntry(gr_10r12c_20, "x_{fit}|_{z=z_{0}} - ^{12}C - 20 Hz - NO RIM", "p");
   l->AddEntry(gr_10r7Li_300R, "x_{fit}|_{z=z_{0}} - ^{7}Li - 300 Hz - RIM", "p");
   l->AddEntry(gr_10r12c_200, "x_{fit}|_{z=z_{0}} -  ^{12}C - 200 Hz - NO RIM", "p");
   l->AddEntry(gr_10r7Li_1000R, "x_{fit}|_{z=z_{0}} - ^{7}Li - 1000 Hz - RIM", "p");
   l->AddEntry(gr_10r12c_1000, "x_{fit}|_{z=z_{0}} - ^{12}C - 1000 Hz - NO RIM", "p");
   l->AddEntry(gr_10r7Li_2000R, "x_{fit}|_{z=z_{0}} - ^{7}Li - 2000 Hz - RIM", "p");
   l->AddEntry(gr_10r12c_3000, "x_{fit}|_{z=z_{0}} - ^{12}C - 3000 Hz - NO RIM", "p");
   l->AddEntry(gr_10r7Li_3000R, "x_{fit}|_{z=z_{0}} - ^{7}Li - 3000 Hz - RIM", "p");
   l->AddEntry(gr_10r16o_200, "x_{fit}|_{z=z_{0}} - ^{16}O - 200 Hz - NO RIM", "p");
   l->AddEntry(gr_10r12c_260R, "x_{fit}|_{z=z_{0}} - ^{12}C - 260 Hz - RIM", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   bga->Draw();
   
   gr_10r12c_20->Draw("PL");
   gr_10r12c_200->Draw("PL");
   gr_10r12c_1000->Draw("PL");
   gr_10r12c_3000->Draw("PL");
   gr_20r12c_20->Draw("PL");
   gr_20r12c_200->Draw("PL");
   gr_20r12c_1000->Draw("PL");
   gr_20r12c_3000->Draw("PL");
   gr_10r7li_2000->Draw("PL");
   gr_10r16o_200->Draw("PL");
   gr_10r7Li_20R->Draw("PL");
   gr_10r7Li_300R->Draw("PL");
   gr_10r7Li_1000R->Draw("PL");
   gr_10r7Li_2000R->Draw("PL");
   gr_10r7Li_3000R->Draw("PL");
   gr_10r12c_260R->Draw("PL");
   l->Draw("SAME");

}
