//#######################################################################################################################
//#   This macro allows to plot the gain vs the Rate. The gain is evaluated as the ratio between the number of electrons
//#   collected on the anode (obtained integrating the charge on the pad-plane, thus obtaining also the error) and the
//#   the number of primaries (obtained by LIDE++ dividing the energy loss in the gas tracker by the average ionization
//#   potential for i-C4H10, i.e. 23,4 eV).
//#######################################################################################################################
//#   Created March 2026 by A. Pitronaci 
//#######################################################################################################################
//#   Updated:
//#######################################################################################################################

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

void plot_gain_vs_rate(){

//####################################################################################################################
// Needed variables
   Int_t p = 0;                                                                // Integer counter for TGraphErrors points
   Double_t rate[4] = {20.,200.,1000.,3000};                                   // Array for the rate values.
   Double_t rate_err[4] = {0.};
   Double_t np[2] = {95690.,204358.9744};                                          // Number of primary electrons - obtained dividing energy loss (by LISE++) and the average ioization potential (23.4 eV)
   Double_t sp1[4] = {177570,240587,179803,176589};
   for(Int_t i=0; i<4; i++){
      sp1[i] = sp1[i]*2*0.061/(7.24E-05);
   }                            // Number of secondary electrons - obtained converting the digitizer signal (in mV) in # e- -> digi *2*61e-3/7.24e-5    
   Double_t sp1_error[4] = {0.};                                               // Array for the errors in the number of the collected collected electrons, obtained from the Poisson distribution 
   Double_t sp2[4] = {1.13859E+9,1.06097E+9,1.03979E+9,1.0E+9};
   Double_t sp2_error[4] = {0.};
   
   Double_t gain1[4] = {0.};                                                   // Array for the gain
   Double_t gain1_err[4] = {0.};                                               // Array for the gain-error
   Double_t gain2[4] = {0.};                                                   // Array for the gain
   Double_t gain2_err[4] = {0.};                                               // Array for the gain-error
   
   Double_t rate_o[1] = {250};
   
   Double_t rate_li[1] = {2000};

   Double_t spli[1] = {0};
   spli[0] = {20612102.};
   Double_t spli_err[1] = {0.};
   spli_err[0] = sqrt(spli[0]);
   Double_t npli[1] = {0.};
   npli[0] = {2.194E+4};
   Double_t gain_li[1] = {0.};
   gain_li[0] = spli[0]/npli[0];
   Double_t gain_li_err[1] = {0.};
   gain_li_err[0] = spli_err[0]/npli[0];
   
   Double_t spo[1] = {0.};
   spo[0] = {4.4946215e+08};
   Double_t spo_err[1] = {0.};
   spo_err[0] = sqrt(spo[0]);
   Double_t npo[1] = {0.};
   npo[0] = {1.635E5};
   Double_t gain_o[1] = {0.};
   gain_o[0] = spo[0]/npo[0];
   Double_t gain_o_err[1] = {0.};
   gain_o_err[0] = spo_err[0]/npo[0];
   
   Double_t rate_cR[1] = {260};
   Double_t s_c_R[1] = {1.0945742e+09};
   
   Double_t rate_li_R[5] = {20.,300.,1000.,2000.,3000};
   Double_t s_li_R[5] = {3.1271936e+08,2.6281058e+08,3.0191630e+08,2.7681867e+08,2.5104028e+08};
   Double_t gain_cR[1] = {0.};
   Double_t gain_cR_err[1] = {0.};
   gain_cR[0] = s_c_R[0]/np[0];
   gain_cR_err[0] = sqrt(s_c_R[0])/np[0];
   
   TGraphErrors *gr_g1 = new TGraphErrors();
   gr_g1->SetTitle("Gain vs Rate");
   gr_g1->GetXaxis()->SetTitle("Rate (Hz)");
   gr_g1->GetYaxis()->SetTitle("Gain");
   gr_g1->SetMarkerColor(kGreen+1);
   gr_g1->SetMarkerStyle(20);
   gr_g1->SetMarkerSize(1.2);
   gr_g1->SetLineWidth(2);
   gr_g1->SetLineStyle(1);
   gr_g1->SetLineColor(kGreen+1);
   
   TGraphErrors *gr_g2 = new TGraphErrors();     
   gr_g2->SetTitle("Gain vs Rate");
   gr_g2->GetXaxis()->SetTitle("Rate (Hz)");
   gr_g2->GetYaxis()->SetTitle("Gain");
   gr_g2->SetMarkerColor(kGreen+2);
   gr_g2->SetMarkerStyle(20);
   gr_g2->SetMarkerSize(1.2);
   gr_g2->SetLineWidth(2);
   gr_g2->SetLineStyle(1);
   gr_g2->SetLineColor(kGreen+2);
   
   TGraphErrors *gr_go = new TGraphErrors(1,rate_o,gain_o,rate_err,gain_o_err);     
   gr_go->SetTitle("Gain vs Rate");
   gr_go->GetXaxis()->SetTitle("Rate (Hz)");
   gr_go->GetYaxis()->SetTitle("Gain");
   gr_go->SetMarkerColor(kOrange+1);
   gr_go->SetMarkerStyle(20);
   gr_go->SetMarkerSize(1.2);
   gr_go->SetLineWidth(2);
   gr_go->SetLineStyle(1);
   gr_go->SetLineColor(kOrange+1);
   
   TGraphErrors *gr_gli = new TGraphErrors(1,rate_li,gain_li,rate_err,gain_li_err);     
   gr_gli->SetTitle("Gain vs Rate");
   gr_gli->GetXaxis()->SetTitle("Rate (Hz)");
   gr_gli->GetYaxis()->SetTitle("Gain");
   gr_gli->SetMarkerColor(kMagenta+1);
   gr_gli->SetMarkerStyle(20);
   gr_gli->SetMarkerSize(1.2);
   gr_gli->SetLineWidth(2);
   gr_gli->SetLineStyle(1);
   gr_gli->SetLineColor(kMagenta);
   
   TGraphErrors *gr_gliR = new TGraphErrors();     
   gr_gliR->SetTitle("Gain vs Rate");
   gr_gliR->GetXaxis()->SetTitle("Rate (Hz)");
   gr_gliR->GetYaxis()->SetTitle("Gain");
   gr_gliR->SetMarkerColor(kCyan+2);
   gr_gliR->SetMarkerStyle(20);
   gr_gliR->SetMarkerSize(1.2);
   gr_gliR->SetLineWidth(2);
   gr_gliR->SetLineStyle(1);
   gr_gliR->SetLineColor(kCyan+2);
   
   TGraphErrors *gr_gcR = new TGraphErrors(1,rate_cR,gain_cR,0,gain_cR_err);     
   gr_gcR->SetTitle("Gain vs Rate");
   gr_gcR->GetXaxis()->SetTitle("Rate (Hz)");
   gr_gcR->GetYaxis()->SetTitle("Gain");
   gr_gcR->SetMarkerColor(kRed);
   gr_gcR->SetMarkerStyle(20);
   gr_gcR->SetMarkerSize(1.2);
   gr_gcR->SetLineWidth(2);
   gr_gcR->SetLineStyle(1);
   gr_gcR->SetLineColor(kRed);
   
   for(Int_t i=0; i<4; i++){
      gain1[i] = sp1[i]/np[0];
      sp1_error[i] = sqrt(sp1[i]);
      gain1_err[i] = sp1_error[i]/np[0];
      gr_g1->SetPoint(p,rate[i],gain1[i]);
      gr_g1->SetPointError(p,rate_err[i],gain1_err[i]);
      cout << "Rate: " << rate[i] << "   Gain: " << gain1[i] << endl;
      
      gain2[i] = sp2[i]/np[1];
      sp2_error[i] = sqrt(sp2[i]);
      gain2_err[i] = sp2_error[i]/np[1];
      gr_g2->SetPoint(p,rate[i],gain2[i]);
      gr_g2->SetPointError(p,rate_err[i],gain2_err[i]);
      cout << "Rate: " << rate[i] << "   Gain: " << gain2[i] << endl;
      
      p++;
   }
   
   Int_t r = 0;
   Double_t gain_liR[5] = {0.};
   Double_t gain_liR_err[5] = {0.};
   Double_t rate_err_li[5] = {0.};
   for(Int_t i=0; i<5; i++){
      gain_liR[i] = s_li_R[i]/npli[0];
      sp1_error[i] = sqrt(s_li_R[i]);
      gain_liR_err[i] = sp1_error[i]/npli[0];
      gr_gliR->SetPoint(r,rate_li_R[i],gain_liR[i]);
      gr_gliR->SetPointError(r,rate_err_li[i],gain_liR_err[i]);
      cout << "Rate: " << rate_li_R[i] << "   Gain: " << gain_liR[i] << endl;
      
      r++;
   }

//####################################################################################################################
// Visualization block

   TLegend* l = new TLegend(0.1,0.7,0.58,0.9);
   l->SetTextSize(0.025);
   //l->SetLineWidth(0);
   l->AddEntry(gr_gli, "Gain(R) - V_{THGEM} = 170 V - P = 10.0 mbar - ^{7}Li - NO RIM", "p"); 
   l->AddEntry(gr_g1, "Gain(R) - V_{THGEM} = 170 V - P = 10.0 mbar - ^{12}C - NO RIM", "p");
   l->AddEntry(gr_g2, "Gain(R) - V_{THGEM} = 200 V - P = 20.0 mbar - ^{12}C - NO RIM", "p");
   l->AddEntry(gr_go, "Gain(R) - V_{THGEM} = 170 V - P = 10.0 mbar - ^{16}O - NO RIM", "p");
   l->AddEntry(gr_gliR, "Gain(R) - V_{THGEM} = 200 V - P = 10.0 mbar - ^{7}Li - RIM", "p");
   l->AddEntry(gr_gcR, "Gain(R) - V_{THGEM} = 200 V - P = 10.0 mbar - ^{12}C - RIM", "p");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   gr_g1->Draw("APL");
   gr_g2->Draw("PL SAME");
   gr_go->Draw("PL && SAME");
   gr_gli->Draw("PL && SAME");
   gr_gliR->Draw("PL & SAME");
   gr_gcR->Draw("PL & SAME");
   gr_g1->GetYaxis()->SetRangeUser(1.0E+3,2.0E+5);
   l->Draw("SAME");

}
