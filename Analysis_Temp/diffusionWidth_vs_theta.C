//###################################################################################################
//#   plot of charge multiplicity as a function of theta
//#
//###################################################################################################
//#   creatd Feb 2025 by A. Pitronaci 
//#######################################
//#   updated: March 2025 by A. Pitronaci
//# 
//###################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>

using namespace std;
using namespace TMath;

const Double_t drift_time = 1.02E-6;                                                    // drift time in s
const Double_t mobility = 9.37E+6;                                                      // electron mobility (in mm^2V^-1s^-1) in I-C4H10 with u as drift velocity
const Double_t drift_velocity = 50.0;                                                   // electron drift velocity (mm/us) in I-C4H10 at saturation regime
const Double_t me = 9.11E-31;                                                           // electron mass expressed in Kg
const Double_t e = 1.6022E-19;                                                          // electron charge
const Double_t padWidth = 5.0;                                                          // pad width expressed in mm
const Double_t padLenght = 10.2;                                                        // pad lenght expressed in mm
const Double_t KB_T = 0.0259;                                                           // product of the Boltzmann constato and the temperature (K_B \cdot T) expressed in eV    
const Double_t l_drift = 99.0;                                                          // electron drift path expressed in mm     
    
void diffusionWidth_vs_theta() {
   
   /* Required variables */
   Double_t diffusion_nc = 0;
   Double_t diffusion_10mbar = 0;
   Double_t sigma_x = 0;
   Double_t fwhm_pads[10] = {0.};
   Double_t theta[10] = {0,10,20,30,40,50,60,65,70,80};
   Double_t delta_x[10] = {0.};
   Double_t mult[10] = {0};
   Double_t delta_x_tot[10] = {0.};
   
   Double_t thetaEX[1] = {62.5};
   Double_t errTheta[1] = {2.5};
   
   Double_t theta_he[1] = {40};
   Double_t errThetaHe[1] = {20};
 
   Double_t x_c[1] = {7.59054*5};
   Double_t errXc[1] = {1.99634*5};
   Double_t x_li[1] = {9.791*5};
   Double_t errXli[1] = {1.557*5};
   Double_t x_he[1] = {4.092*5};
   Double_t errXhe[1] = {1.495*5};
   Double_t x_o[1] = {8.73384*5};
   Double_t errXo[1] = {2.67765*5};
   /* Evaluation of the total diffusion width */
   
   diffusion_nc = mobility * KB_T;
   diffusion_10mbar = diffusion_nc *(1013/10);
   sigma_x = sqrt(2*diffusion_10mbar*drift_time);
   
           /* fwhm evaluation */
           for(Int_t i=0;i<10;i++){
              fwhm_pads[i] = sigma_x*2.35*Cos(theta[i]*Pi()/180);
              delta_x[i] = padLenght * Tan(theta[i]*Pi()/180);
              delta_x_tot[i] = fwhm_pads[i] + delta_x[i];    
              mult[i] = delta_x_tot[i]/padWidth;   
           }

   
   TGraph *gr1 = new TGraph(10,theta,delta_x_tot);
   gr1->SetTitle("Diffusion width - Row 4");
   gr1->SetLineWidth(2);
   gr1->SetMarkerStyle(1);
   //gr1->SetMarkerSize(1.2);
   gr1->SetMarkerColor(kBlue);
   gr1->GetXaxis()->SetTitle("#theta (#circ)");
   gr1->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   
   TGraphErrors *gr1li = new TGraphErrors(1,thetaEX,x_li,errTheta,errXli);
   gr1li->SetLineWidth(2);
   gr1li->SetMarkerSize(1.2);
   gr1li->SetMarkerStyle(20);
   gr1li->SetMarkerColor(kRed);
   gr1li->SetLineColor(kRed);
   gr1li->GetXaxis()->SetTitle("#theta (#circ)");
   gr1li->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraphErrors *gr1he = new TGraphErrors(1,theta_he,x_he,errThetaHe,errXhe);
   gr1he->SetLineWidth(2);
   gr1he->SetMarkerSize(1.2);
   gr1he->SetMarkerStyle(20);
   gr1he->SetMarkerColor(kGreen+2);
   gr1he->SetLineColor(kGreen+2);
   gr1he->GetXaxis()->SetTitle("#theta (#circ)");
   gr1he->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraphErrors *gr1o = new TGraphErrors(1,thetaEX,x_o,errTheta,errXo);
   gr1o->SetLineWidth(2);
   gr1o->SetMarkerSize(1.2);
   gr1o->SetMarkerStyle(20);
   gr1o->SetMarkerColor(kOrange+2);
   gr1o->SetLineColor(kOrange+2);
   gr1o->GetXaxis()->SetTitle("#theta (#circ)");
   gr1o->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraphErrors *gr1c = new TGraphErrors(1,thetaEX,x_c,errTheta,errXc);
   gr1c->SetLineWidth(2);
   gr1c->SetMarkerSize(1.2);
   gr1c->SetMarkerStyle(20);
   gr1c->SetMarkerColor(kMagenta+2);
   gr1c->SetLineColor(kMagenta+2);
   gr1c->GetXaxis()->SetTitle("#theta (#circ)");
   gr1c->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   /* Visualization block */

   TCanvas *c = new TCanvas("c");
   TLegend *l = new TLegend();
   l->AddEntry(gr1,"Theo. predict.","lp");
   l->AddEntry(gr1li,"Lithium","lp");
   l->AddEntry(gr1c,"Carbon","lp");
   l->AddEntry(gr1he,"Helium","lp");
   l->AddEntry(gr1o,"Oxigen","lp");
   
   c->cd();
   gr1->Draw("ALP same");
   gr1li->Draw("LP SAME");
   gr1he->Draw("LP SAME");
   gr1c->Draw("LP SAME");
   gr1o->Draw("LP SAME");
   
   l->Draw("same");
}
