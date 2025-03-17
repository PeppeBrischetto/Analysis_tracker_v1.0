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

const TString InputFileC = "../../OutputFiles/Run65.txt";
const TString InputFileLi = "../../OutputFiles/Run332Li.txt";
const TString InputFileHe = "../../OutputFiles/Run332Alpha.txt";
const TString InputFileO = "../../OutputFiles/Run171.txt";

void diffusionWidth_vs_theta_beta() {
   
   /* Required variables */
   Double_t diffusion_nc = 0;
   Double_t diffusion_10mbar = 0;
   Double_t diffusion_11mbar = 0;
   Double_t diffusion_9mbar = 0;
   Double_t sigma_x = 0;
   Double_t sigma_x_9mbar = 0;
   Double_t sigma_x_11mbar = 0;
   Double_t fwhm_pads[10] = {0.};
   Double_t fwhm_pads_9mbar[10] = {0.};
   Double_t fwhm_pads_11mbar[10] = {0.};
   Double_t theta[10] = {0,10,20,30,40,50,60,65,70,80};
   Double_t delta_x[10] = {0.};
   Double_t delta_x_9mbar[10] = {0.};
   Double_t delta_x_11mbar[10] = {0.};
   Double_t mult[10] = {0};
   Double_t delta_x_tot[10] = {0.};
   Double_t delta_x_tot_9mbar[10] = {0.};
   Double_t delta_x_tot_11mbar[10] = {0.};
   
   Double_t thetaEX= 62.5;
   Double_t errTheta = 2.5;
   
   char histoname[100];
   Double_t theta_he = 0;
   Double_t errThetaHe = 0;
   Double_t x_he[5] = {0.};
   Double_t errXhe[5] = {0.};
   Int_t n_point_tot = 0;
   Double_t average_clMult1[5] = {0.};
   Int_t counter1[5] = {0};
   Double_t average_clMult2[5] = {0.};
   Int_t counter2[5] = {0};
   Double_t average_clMult3[5] = {0.};
   Int_t counter3[5] = {0};
   Double_t average_clMult4[5] = {0.};
   Int_t counter4[5] = {0};
   Double_t average_clMult5[5] = {0.};
   Int_t counter5[5] = {0};
   Double_t average_clMult6[5] = {0.};
   Int_t counter6[5] = {0}; 
   Double_t mean_thetaHe1[5] = {0.};
   Double_t mean_thetaHe2[5] = {0.};
   Double_t mean_thetaHe3[5] = {0.};
   Double_t mean_thetaHe4[5] = {0.};
   Double_t mean_thetaHe5[5] = {0.};
   Double_t mean_thetaHe6[5] = {0.};
 
   Double_t x_c[5] = {0};
   Double_t errXc[5] = {0.};
   Double_t x_li[5] = {0.};
   Double_t errXli[5] = {0.};
   Double_t x_o[5] = {0.};
   Double_t errXo[5] = {0.};
   
   ifstream f_li,f_o,f_he,f_c;
   string badLines;
   char dummy[100];
   /* Evaluation of the total diffusion width */
   
   diffusion_nc = mobility * KB_T;
   diffusion_10mbar = diffusion_nc *(1013/10);
   diffusion_9mbar = diffusion_nc *(1013/9);
   diffusion_11mbar = diffusion_nc *(1013/11);
   sigma_x = sqrt(2*diffusion_10mbar*drift_time);
   sigma_x_9mbar = sqrt(2*diffusion_9mbar*drift_time);
   sigma_x_11mbar = sqrt(2*diffusion_11mbar*drift_time);
   
           /* fwhm evaluation */
           for(Int_t i=0;i<10;i++){
              fwhm_pads[i] = sigma_x*2.35*Cos(theta[i]*Pi()/180);
              fwhm_pads_9mbar[i] = sigma_x_9mbar*2.35*Cos(theta[i]*Pi()/180);
              fwhm_pads_11mbar[i] = sigma_x_11mbar*2.35*Cos(theta[i]*Pi()/180);
              delta_x[i] = padLenght * Tan(theta[i]*Pi()/180);
              delta_x_9mbar[i] = padLenght * Tan(theta[i]*Pi()/180);
              delta_x_11mbar[i] = padLenght * Tan(theta[i]*Pi()/180);
              delta_x_tot[i] = fwhm_pads[i] + delta_x[i];
              delta_x_tot_9mbar[i] = fwhm_pads_9mbar[i] + delta_x_9mbar[i];
              delta_x_tot_11mbar[i] = fwhm_pads_11mbar[i] + delta_x_11mbar[i];    
              mult[i] = delta_x_tot[i]/padWidth;   
           }

   
   TGraph *gr1 = new TGraph(10,theta,delta_x_tot);
   gr1->SetTitle("Diffusion width - Row0");
   gr1->SetLineWidth(1);
   gr1->SetMarkerStyle(1);
   //gr1->SetMarkerSize(1.2);
   gr1->SetMarkerColor(kBlue);
   gr1->GetXaxis()->SetTitle("#theta (#circ)");
   gr1->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_9mbar = new TGraph(10,theta,delta_x_tot_9mbar);
   gr1_9mbar->SetTitle("Diffusion width - Row0");
   gr1_9mbar->SetLineWidth(1);
   gr1_9mbar->SetLineColor(kBlue);
   gr1_9mbar->SetMarkerStyle(1);
   //gr1_9mbar->SetMarkerSize(1.2);
   gr1_9mbar->SetMarkerColor(kBlue);
   gr1_9mbar->GetXaxis()->SetTitle("#theta (#circ)");
   gr1_9mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_11mbar = new TGraph(10,theta,delta_x_tot_11mbar);
   gr1_11mbar->SetTitle("Diffusion width - Row0");
   gr1_11mbar->SetLineWidth(1);
   gr1_11mbar->SetLineColor(kRed);
   gr1_11mbar->SetMarkerStyle(1);
   //gr1_11mbar->SetMarkerSize(1.2);
   gr1_11mbar->SetMarkerColor(kRed);
   gr1_11mbar->GetXaxis()->SetTitle("#theta (#circ)");
   gr1_11mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//   Reading outputFiles to evaluate experimental diffusion width                            //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

   f_li.open(InputFileLi);
   f_c.open(InputFileC);
   f_o.open(InputFileO);
   f_he.open(InputFileHe);
   
   TGraphErrors *gr1he1[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he1[i]=new TGraphErrors();
      gr1he1[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he1[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he1[i]->SetMarkerSize(1.2);
      gr1he1[i]->SetMarkerStyle(20);
      gr1he1[i]->SetMarkerColor(kGreen+2);
      gr1he1[i]->SetLineColor(kGreen+2);
      gr1he1[i]->SetLineWidth(2);
   }
   TGraphErrors *gr1he2[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he2[i]=new TGraphErrors();
      gr1he2[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he2[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he2[i]->SetMarkerSize(1.2);
      gr1he2[i]->SetMarkerStyle(20);
      gr1he2[i]->SetMarkerColor(kGreen+2);
      gr1he2[i]->SetLineColor(kGreen+2);
      gr1he2[i]->SetLineWidth(2);
   }
   TGraphErrors *gr1he3[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he3[i]=new TGraphErrors();
      gr1he3[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he3[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he3[i]->SetMarkerSize(1.2);
      gr1he3[i]->SetMarkerStyle(20);
      gr1he3[i]->SetMarkerColor(kGreen+2);
      gr1he3[i]->SetLineColor(kGreen+2);
      gr1he3[i]->SetLineWidth(2);
   }
   TGraphErrors *gr1he4[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he4[i]=new TGraphErrors();
      gr1he4[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he4[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he4[i]->SetMarkerSize(1.2);
      gr1he4[i]->SetMarkerStyle(20);
      gr1he4[i]->SetMarkerColor(kGreen+2);
      gr1he4[i]->SetLineColor(kGreen+2);
      gr1he4[i]->SetLineWidth(2);
   }
   TGraphErrors *gr1he5[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he5[i]=new TGraphErrors();
      gr1he5[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he5[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he5[i]->SetMarkerSize(1.2);
      gr1he5[i]->SetMarkerStyle(20);
      gr1he5[i]->SetMarkerColor(kGreen+2);
      gr1he5[i]->SetLineColor(kGreen+2);
      gr1he5[i]->SetLineWidth(2);
   }
   TGraphErrors *gr1he6[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he6[i]=new TGraphErrors();
      gr1he6[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1he6[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he6[i]->SetMarkerSize(1.2);
      gr1he6[i]->SetMarkerStyle(20);
      gr1he6[i]->SetMarkerColor(kGreen+2);
      gr1he6[i]->SetLineColor(kGreen+2);
      gr1he6[i]->SetLineWidth(2);
   }
   
   
   TGraphErrors *gr1li[5];
   for(Int_t i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1li[i]=new TGraphErrors();
      gr1li[i]->SetLineWidth(2);
      gr1li[i]->SetMarkerSize(1.2);
      gr1li[i]->SetMarkerStyle(20);
      gr1li[i]->SetMarkerColor(kRed);
      gr1li[i]->SetLineColor(kRed);
      gr1li[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1li[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   }
   
   TGraphErrors *gr1o[5];
   for(Int_t i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1o[i]=new TGraphErrors();
      gr1o[i]->SetLineWidth(2);
      gr1o[i]->SetMarkerSize(1.2);
      gr1o[i]->SetMarkerStyle(20);
      gr1o[i]->SetMarkerColor(kOrange+2);
      gr1o[i]->SetLineColor(kOrange+2);
      gr1o[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1o[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   }
   
   TGraphErrors *gr1c[5];
   for(Int_t i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1c[i]=new TGraphErrors();
      gr1c[i]->SetLineWidth(2);
      gr1c[i]->SetMarkerSize(1.2);
      gr1c[i]->SetMarkerStyle(20);
      gr1c[i]->SetMarkerColor(kMagenta+2);
      gr1c[i]->SetLineColor(kMagenta+2);
      gr1c[i]->GetXaxis()->SetTitle("#theta (#circ)");
      gr1c[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   }
   
   for(Int_t i=0; i<13; i++){
      getline(f_li,badLines);
      //cout << badLines << endl;
      getline(f_o,badLines);
      //cout << badLines << endl;
      getline(f_he,badLines);
      //cout << badLines << endl;
      getline(f_c,badLines);
      //cout << badLines << endl;
   }
   for(Int_t i=13; i<14; i++){
      f_li >> x_li[0] >> x_li[1] >> x_li[2] >> x_li[3] >> x_li[4] ;
      f_o >> x_o[0] >> x_o[1] >> x_o[2] >> x_o[3] >> x_o[4];
      f_c >> x_c[0] >> x_c[1] >> x_c[2] >> x_c[3] >> x_c[4];
      for(Int_t j=0; j<5; j++){
         x_li[j] = 5*x_li[j];
         gr1li[j]->SetPoint(n_point_tot,thetaEX,x_li[j]);
         x_o[j] = 5*x_o[j];
         gr1o[j]->SetPoint(n_point_tot,thetaEX,x_o[j]);
         x_c[j] = 5*x_c[j];
         gr1c[j]->SetPoint(n_point_tot,thetaEX,x_c[j]);
      }
   }
   for(Int_t i=14; i<16; i++){
      getline(f_li,badLines);
      //cout << badLines << endl;
      getline(f_o,badLines);
      //cout << badLines << endl;
      //cout << badLines << endl;
      getline(f_c,badLines);
      //cout << badLines << endl;
   }
   for(Int_t i=16; i<17; i++){
      f_li >> errXli[0] >> errXli[1] >> errXli[2] >> errXli[3] >> errXli[4] ;
      f_o  >> errXo[0] >> errXo[1] >> errXo[2] >> errXo[3] >> errXo[4];
      f_c  >> errXc[0] >> errXc[1] >> errXc[2] >> errXc[3] >> errXc[4];
      for(Int_t j=0; j<5; j++){
         errXli[j] = errXli[j]*5; 
         gr1li[j]->SetPointError(n_point_tot,errTheta,errXli[j]);
         errXo[j] = errXo[j]*5; 
         gr1o[j]->SetPointError(n_point_tot,errTheta,errXo[j]);
         errXc[j] = errXc[j]*5; 
         gr1c[j]->SetPointError(n_point_tot,errTheta,errXc[j]);
      }
   }
   
   for(Int_t i=13; i<1693; i++){
      f_he >> x_he[0] >>  x_he[1]  >> x_he[2] >>  x_he[3] >>  x_he[4] >> theta_he ;
      for(Int_t j=0; j<5; j++){
         x_he[j] = 5*x_he[j];
         //gr1he[j]->SetPoint(n_point_tot,theta_he,x_he[j]);
         //gr1he[j]->SetPointError(n_point_tot,0,0);
         
         if(theta_he>20 && theta_he<30){
           average_clMult1[j] += x_he[j];
           mean_thetaHe1[j] += theta_he;
           counter1[j] ++;
         }else
         if(theta_he>30 && theta_he<40){
           average_clMult2[j] += x_he[j];
           mean_thetaHe2[j] += theta_he;
           counter2[j] ++;
         }else
         if(theta_he>40 && theta_he<50){
           average_clMult3[j] += x_he[j];
           mean_thetaHe3[j] += theta_he;
           counter3[j] ++;
         }else
         if(theta_he>50 && theta_he<60){
           average_clMult4[j] += x_he[j];
           mean_thetaHe4[j] += theta_he;
           counter4[j] ++;
         }else
         if(theta_he>60 && theta_he<70){
           average_clMult5[j] += x_he[j];
           mean_thetaHe5[j] += theta_he;
           counter5[j] ++;
         }else
         if(theta_he>10 && theta_he<20){
           average_clMult6[j] += x_he[j];
           mean_thetaHe6[j] += theta_he;
           counter6[j] ++;
         }
      }
      n_point_tot ++;
   }
   
   for(Int_t j=0; j<5; j++){
      mean_thetaHe1[j] = mean_thetaHe1[j]/counter1[j];
      mean_thetaHe2[j] = mean_thetaHe2[j]/counter2[j];
      mean_thetaHe3[j] = mean_thetaHe3[j]/counter3[j];
      mean_thetaHe4[j] = mean_thetaHe4[j]/counter4[j];
      mean_thetaHe5[j] = mean_thetaHe5[j]/counter5[j];
      mean_thetaHe6[j] = mean_thetaHe6[j]/counter6[j];
      average_clMult1[j] = average_clMult1[j]/counter1[j];
      average_clMult2[j] = average_clMult2[j]/counter2[j];
      average_clMult3[j] = average_clMult3[j]/counter3[j];
      average_clMult4[j] = average_clMult4[j]/counter4[j];
      average_clMult5[j] = average_clMult5[j]/counter5[j];
      average_clMult6[j] = average_clMult6[j]/counter6[j];
      
      gr1he1[j]->SetPoint(0,mean_thetaHe1[j],average_clMult1[j]);
      gr1he2[j]->SetPoint(0,mean_thetaHe2[j],average_clMult2[j]);
      gr1he3[j]->SetPoint(0,mean_thetaHe3[j],average_clMult3[j]);
      gr1he4[j]->SetPoint(0,mean_thetaHe4[j],average_clMult4[j]);
      gr1he5[j]->SetPoint(0,mean_thetaHe5[j],average_clMult5[j]);
      gr1he6[j]->SetPoint(0,mean_thetaHe6[j],average_clMult6[j]);
   }
   
   cout << average_clMult1[0] << "   " << average_clMult2[0] << "   " << average_clMult3[0] << "   " << average_clMult4[0] << "   " << average_clMult5[0] << "   " << average_clMult6[0] << endl;
   cout << counter1[0] << "   " << counter2[0] << "   " << counter3[0] << "   " << counter4[0] << "   " << counter5[0] << "   " << counter6[0] <<  "   " << n_point_tot << endl; 
   
   
   
   /* Visualization block */

   TCanvas *c = new TCanvas("c");
   TLegend *l = new TLegend();
   l->AddEntry(gr1,"Theo. predict.","lp");
   l->AddEntry(gr1_9mbar,"Theo. predict. - 9mbar","lp");
   l->AddEntry(gr1_11mbar,"Theo. predict. - 11mbar","lp");
   l->AddEntry(gr1li[0],"Lithium","lp");
   l->AddEntry(gr1c[0],"Carbon","lp");
   l->AddEntry(gr1he6[0],"Helium 10#circ-20#circ","lp");
   l->AddEntry(gr1he1[0],"Helium 20#circ-30#circ","lp");
   l->AddEntry(gr1he2[0],"Helium 30#circ-40#circ","lp");
   l->AddEntry(gr1he3[0],"Helium 40#circ-50#circ","lp");
   l->AddEntry(gr1he4[0],"Helium 50#circ-60#circ","lp");
   l->AddEntry(gr1he5[0],"Helium 60#circ-70#circ","lp");
   l->AddEntry(gr1o[0],"Oxigen","lp");
   
   c->cd();
   gr1->Draw("APC same");
   gr1->GetXaxis()->SetRangeUser(0,85);
   gr1->GetYaxis()->SetRangeUser(0,90);
   gr1_9mbar->Draw("PC SAME");
   gr1_11mbar->Draw("PC SAME");
   gr1li[0]->Draw("P SAME");
   gr1he1[0]->Draw("P SAME");
   gr1he2[0]->Draw("P SAME");
   gr1he3[0]->Draw("P SAME");
   gr1he4[0]->Draw("P SAME");
   gr1he5[0]->Draw("P SAME");
   gr1he6[0]->Draw("P SAME");
   gr1c[0]->Draw("P SAME");
   gr1o[0]->Draw("P SAME");
   
   l->Draw("same");
}
