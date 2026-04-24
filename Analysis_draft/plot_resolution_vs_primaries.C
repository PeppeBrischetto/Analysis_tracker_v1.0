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

void plot_resolution_vs_primaries(){

//####################################################################################################################
// Needed variables
   Int_t p = 0;
   Double_t np_c[2] = {95834.,203948.};                  // Number of primary electrons - obtained dividing energy loss (by LISE++) and the average ioization potential (23.4 eV)
   Double_t sp_c[2] = {2.98E+08,1.0E+9};                 // Number of secondary electrons - obtained converting the digitizer signal (in mV) in # e- -> digi * 61e-3/7.24e-5    
   Double_t np_o[1] = {180769.23};                                                                                                        
   Double_t sp_o[1] = {1.1111656E+09};                                                                                                    
   Double_t np_li[1] = {21965.812};
   Double_t sp_li[1] = {2.7197069E+08};
   
   
   Double_t resol_2c[8] = {1.01453,1.20145,1.37213,1.4087,1.00553,1.16879,1.28275,1.33634};                                                // resolution array, measured in mm
   Double_t resol_1c[8] = {0.78438,0.839794,0.955735,0.991787,0.74615,0.759098,0.823552,0.891991};                                         // resolution array, measured in mm
   Double_t resol_05c[8] = {0.680787,0.757098,0.780148,0.796688,0.550655,0.540796,0.612535,0.668728};                                      // resolution array, measured in mm
   Double_t resolDiff_2c[8] = {1.73435,1.79388,1.71778,1.55589,1.08613,1.12722,1.10526,1.20752};                                           // resolution array, measured in mm
   Double_t resolDiff_1c[8] = {1.683,1.74864,1.71677,1.49317,1.04758,1.08048,1.03241,1.10741};                                             // resolution array, measured in mm
   Double_t resolDiff_05c[8] = {1.42162,1.89264,1.68736,1.43778,1.10787,1.0605,0.996745,1.10243};                                          // resolution array, measured in mm
   
   Double_t ex[2] = {0.};                                                                                                                 // error on vthgem, we asssume 0 V
   Double_t resol_2c_err[8] = {0.0121375,0.0122631,0.00601778,0.00283003,0.0138078,0.0130585,0.00625376,0.00197921};                       // resolution array, measured in mm
   Double_t resol_1c_err[8] = {0.0152887,0.01646,0.00943,0.00455997,0.0187307,0.0193095,0.00861552,0.00304904};                            // resolution array, measured in mm
   Double_t resol_05c_err[8] = {0.0299145,0.0332214,0.0182243,0.00811091,0.0297345,0.0376067,0.0151905,0.00520609};                        // resolution array, measured in mm
   Double_t resolDiff_2c_err[8] = {0.0280179,0.0274246,0.011019,0.00461767,0.0216694,0.017685,0.00768981,0.00257592};                      // resolution array, measured in mm
   Double_t resolDiff_1c_err[8] = {0.0554518,0.0546712,0.0218924,0.00798279,0.0680556,0.0351187,0.0130178,0.00427};                        // resolution array, measured in mm
   Double_t resolDiff_05c_err[8] = {0.1251,0.100593,0.0436291,0.0147473,0.127081,0.0587356,0.0243829,0.00799995};                          // resolution array, measured in mm
   
   Double_t resol_c[2] = {0.};                                                                                                              
   Double_t resol_c_err[2] = {0.};
   Double_t resolDiff_c[2] = {0.};
   Double_t resolDiff_c_err[2] = {0.};
   
   Double_t resol_05_o[1] = {0.757098};
   Double_t resol_05_o_err[1] = {0.0332214};
   Double_t resolDiff_05_o[1] = {1.53101};
   Double_t resolDiff_05_o_err[1] = {0.0418414};
   
   Double_t resol_05_li[1] = {1.02632};
   Double_t resol_05_li_err[1] = {0.00957856};
   Double_t resolDiff_05_li[1] = {2.82262};
   Double_t resolDiff_05_li_err[1] = {0.0486812};
   
   resolDiff_05_o[0] = resolDiff_05_o[0]/sqrt(2);
   resolDiff_05_o_err[0] = resolDiff_05_o_err[0]/sqrt(2);
   
   TGraphErrors *gr_rc = new TGraphErrors();     
   gr_rc->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_rc->GetXaxis()->SetTitle("N_{secondaries}");
   gr_rc->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rc->SetMarkerColor(kGreen+2);
   gr_rc->SetMarkerStyle(24);
   gr_rc->SetMarkerSize(1.5);
   gr_rc->SetLineWidth(2);
   gr_rc->SetLineStyle(1);
   gr_rc->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_rcDiff = new TGraphErrors();     
   gr_rcDiff->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_rcDiff->GetXaxis()->SetTitle("N_{secondaries}");
   gr_rcDiff->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rcDiff->SetMarkerColor(kGreen+2);
   gr_rcDiff->SetMarkerStyle(26);
   gr_rcDiff->SetMarkerSize(1.5);
   gr_rcDiff->SetLineWidth(2);
   gr_rcDiff->SetLineStyle(1);
   gr_rcDiff->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_ro = new TGraphErrors(1,sp_o,resol_05_o,ex,resol_05_o_err);     
   gr_ro->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_ro->GetXaxis()->SetTitle("N_{secondaries}");
   gr_ro->GetYaxis()->SetTitle("#deltax (mm)");
   gr_ro->SetMarkerColor(kOrange+2);
   gr_ro->SetMarkerStyle(24);
   gr_ro->SetMarkerSize(1.5);
   gr_ro->SetLineWidth(2);
   gr_ro->SetLineStyle(1);
   gr_ro->SetLineColor(kOrange+2);
   
   TGraphErrors *gr_roDiff = new TGraphErrors(1,sp_o,resolDiff_05_o,ex,resolDiff_05_o_err);     
   gr_roDiff->SetTitle("#deltax vs N_{secondaries} - R = 3000 Hz");
   gr_roDiff->GetXaxis()->SetTitle("N_{secondaries}");
   gr_roDiff->GetYaxis()->SetTitle("#deltax (mm)");
   gr_roDiff->SetMarkerColor(kOrange+2);
   gr_roDiff->SetMarkerStyle(26);
   gr_roDiff->SetMarkerSize(1.5);
   gr_roDiff->SetLineWidth(2);
   gr_roDiff->SetLineStyle(1);
   gr_roDiff->SetLineColor(kOrange+2);
   
   TGraphErrors *gr_rli = new TGraphErrors(1,sp_li,resol_05_li,ex,resol_05_li_err);     
   gr_rli->SetTitle("#deltax vs N_{secondaries} - R = 2000 Hz");
   gr_rli->GetXaxis()->SetTitle("N_{secondaries}");
   gr_rli->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rli->SetMarkerColor(kMagenta+2);
   gr_rli->SetMarkerStyle(24);
   gr_rli->SetMarkerSize(1.5);
   gr_rli->SetLineWidth(2);
   gr_rli->SetLineStyle(1);
   gr_rli->SetLineColor(kMagenta+2);
   
   TGraphErrors *gr_rliDiff = new TGraphErrors(1,sp_li,resolDiff_05_li,ex,resolDiff_05_li_err);     
   gr_rliDiff->SetTitle("#deltax vs N_{secondaries} - R = 2000 Hz");
   gr_rliDiff->GetXaxis()->SetTitle("N_{secondaries}");
   gr_rliDiff->GetYaxis()->SetTitle("#deltax (mm)");
   gr_rliDiff->SetMarkerColor(kMagenta+2);
   gr_rliDiff->SetMarkerStyle(26);
   gr_rliDiff->SetMarkerSize(1.5);
   gr_rliDiff->SetLineWidth(2);
   gr_rliDiff->SetLineStyle(1);
   gr_rliDiff->SetLineColor(kMagenta+2);
   
   
   for(Int_t i=0; i<8; i++){
      resolDiff_2c[i] = resolDiff_2c[i]/sqrt(2);
      resolDiff_1c[i] = resolDiff_1c[i]/sqrt(2);
      resolDiff_05c[i] = resolDiff_05c[i]/sqrt(2);
      resolDiff_2c_err[i] = resolDiff_2c_err[i]/sqrt(2);
      resolDiff_1c_err[i] = resolDiff_1c_err[i]/sqrt(2);
      resolDiff_05c_err[i] = resolDiff_05c_err[i]/sqrt(2);
      if(i==3 || i==7){
         cout << "p: " << p << endl; 
         resol_c[p] = resol_05c[i];
         resol_c_err[p] = resol_05c_err[i];  
         resolDiff_c[p] = resolDiff_05c[i];
         resolDiff_c_err[p] = resolDiff_05c_err[i];
         gr_rc->SetPoint(p,sp_c[p],resol_c[p]);
         gr_rc->SetPointError(p,ex[p],resol_c_err[p]);
         gr_rcDiff->SetPoint(p,sp_c[p],resolDiff_c[p]);
         gr_rcDiff->SetPointError(p,ex[p],resolDiff_c_err[p]);
         p++;
      }
   }

//####################################################################################################################
// Visualization block

   TH2F *bga=new TH2F("bga","#deltax vs N_{secondaries} - R = 3000 Hz",100,3.4E+7,1.2E+9,1000,0.4,3);
   bga->SetStats(0);
   bga->GetXaxis()->SetTitle("N_{secondaries}");
   bga->GetXaxis()->SetTitleSize(0.04);
   bga->GetXaxis()->SetTitleOffset(1.06);
   bga->GetXaxis()->SetLabelSize(0.04);
   bga->GetXaxis()->SetLabelOffset(0.010);
   bga->GetYaxis()->SetTitle("#deltax (mm)");
   bga->GetYaxis()->SetTitleSize(0.04);
   bga->GetYaxis()->SetTitleOffset(1.38);
   bga->GetYaxis()->SetLabelSize(0.04);
   
   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.03);
   //l->SetLineWidth(0);
   l->AddEntry(gr_rc, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm - ^{12}C", "p");
   l->AddEntry(gr_rcDiff, "|x_{3}-x_{2}| - Virtual slit 0.5 mm - ^{12}C", "p");
   //l->AddEntry(gr_ro, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm - ^{16}O", "p");
   //l->AddEntry(gr_roDiff, "|x_{3}-x_{2}| - Virtual slit 0.5 mm - ^{16}O", "p");
   l->AddEntry(gr_rli, "x_{fit}|_{z=z_{0}} - Virtual slit 0.5 mm - ^{7}Li", "p");
   l->AddEntry(gr_rliDiff, "|x_{3}-x_{2}| - Virtual slit 0.5 mm - ^{7}Li", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   bga->Draw();
   
   gr_rc->Draw("PL");
   gr_rcDiff->Draw("PL");
   gr_rc->Draw("PL");
   //gr_ro->Draw("PL && SAME");
   //gr_roDiff->Draw("PL && SAME");
   gr_rli->Draw("PL");
   gr_rliDiff->Draw("PL");
   l->Draw();

}
