//################################################################################################################
//#   plot of charge multiplicity as a function of theta
//#
//################################################################################################################
//#   creatd Feb 2025 by A. Pitronaci 
//################################################################################################################
//#   updated: May 2025 by A. Pitronaci: Assran & Ferbel data;
//#   updated: April 2025 by A. Pitronaci: squared sum of the two contribution to the charge distribution width;
//#   updated: March 2025: study on the two contributions (diffusion and geometrical one) separately;
//################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>

#include "../Include/openfiles.h"

const TString CutFileA = "TCutG/alpha_tcut_run443.root";
const TString CutFileC = "TCutG/li_tcut_run443.root";
const Int_t NRows = 5;

using namespace std;
using namespace TMath;

const Double_t drift_time = 1.02E-6;                                                    // drift time in s
const Double_t mobility = 9.37E+6;                                                      // electron mobility (in mm^2V^-1s^-1) in I-C4H10 with u as drift velocity
const Double_t drift_velocity = 5.0E6;                                                  // electron drift velocity (mm/s) in I-C4H10 at saturation regime
const Double_t me = 9.11E-31;                                                           // electron mass expressed in Kg
const Double_t e = 1.6022E-19;                                                          // electron charge
const Double_t padWidth = 5.0;                                                          // pad width expressed in mm
const Double_t padLenght = 10.2;                                                        // pad lenght expressed in mm
const Double_t KB_T = 0.0259;                                                           // product of the Boltzmann constato and the temperature (K_B \cdot T) expressed in eV    
const Double_t l_drift = 99.0;                                                          // electron drift path expressed in mm     

void C_ChargeDistrWidth_v1(int run){
   
//################################################################################################################
// Theoretical calculations
//################################################################################################################
   /* Required variables */
   Double_t diffusion_nc = 0;
   Double_t diffusion_10mbar = 0;
   Double_t diffusion_11mbar = 0;
   Double_t diffusion_9mbar = 0;
   Double_t sigma_x = 0;
   Double_t sigma_x_9mbar = 0;
   Double_t sigma_x_11mbar = 0;
   Double_t sigma_x_ferbel = 0;
   Double_t delta_x_assran[10] = {0.};
   Double_t delta_x_assran_9mbar[10] = {0.};
   Double_t delta_x_assran_11mbar[10] = {0.};
   Double_t theta[10] = {0,10,20,30,40,50,60,65,70,80};
   Double_t delta_x[10] = {0.};	
   Double_t delta_x_9mbar[10] = {0.};
   Double_t delta_x_11mbar[10] = {0.};
   Double_t delta_x_ferbel[10] = {0.};
   Double_t mult[10] = {0};
   Double_t delta_x_tot[10] = {0.};
   Double_t delta_x_tot_9mbar[10] = {0.};
   Double_t delta_x_tot_11mbar[10] = {0.};
   Double_t delta_x_totWS[10] = {0.};
   Double_t delta_x_totWS_9mbar[10] = {0.};
   Double_t delta_x_totWS_11mbar[10] = {0.};
   Double_t delta_x_WS_fd[10] = {0.};
   
   Double_t thetaEX= 62.5;
   Double_t errTheta = 2.5;
   
   Double_t x_exp_he = 0.;
   Double_t x_exp_li = 0.;
   char histoname[100];
   Double_t theta_He = 0;
   Double_t errThetaHe = 0;
   Double_t theta_Li = 0.;
   Double_t err_theta_Li = 0.;
   Int_t n_point_tot = 0;
   Int_t n_point_he = 0;
   
   TF1 *f = new TF1("f","gaus",-60,60);
   f->SetParameters(1,1,1);
   
   /* Evaluation of the total Charge distribution width */
   
   diffusion_nc = mobility * KB_T;
   /*trans.diff(u/cm) [as a function of the electric field (V_drift/(l_drift*P))] \times l_drift \divided 1000 to obtain mm */
   diffusion_10mbar = 0;                                 // diffusion at 10 mbar, if necessary implement it inverting the relation of the average transversal diffusion
   diffusion_9mbar = 0;                                  // diffusion at 9 mbar, if necessary implement it inverting the relation of the average transversal diffusion
   diffusion_11mbar = 0;                                 // diffusion at 11 mbar, if necessary implement it inverting the relation of the average transversal diffusion
   sigma_x = 140*9.9/1000;                                
   //cout << "Questa è il coefficiente di diffusione: " << diffusion_10mbar << endl;
   sigma_x_9mbar = 145*9.9/1000;                          // Average trasversal diffusion for electrons moving in Ar-isobutane (Assran) at 9 mbar, measured in mm - (Y. Assran p.22/36)
   sigma_x_11mbar = 130*9.9/1000;                         // Average trasversal diffusion for electrons moving in Ar-isobutane (Assran) at 11 mbar, measured in mm - (Y. Assran p.22/36)
   
   sigma_x_ferbel = 1.115;                                // Average transversal diffusion electrons moving in pure isobutane (theory) at 9, 10, 11 mbar, measured in mm - (T. Ferbel, 9.215))
   
           /* fwhm evaluation */
           for(Int_t i=0;i<10;i++){
              delta_x_assran[i] = 4.93*sigma_x*(Cos(theta[i]*Pi()/180));                                     // 4.5766667 means 4.5766667*sigma, i.e. 3: [mu-3sigma;mu+3sigma]
              delta_x_assran_9mbar[i] = 4.93*sigma_x_9mbar*(Cos(theta[i]*Pi()/180));
              delta_x_assran_11mbar[i] = 4.93*sigma_x_11mbar*(Cos(theta[i]*Pi()/180));
              
              delta_x_ferbel[i] = 4.93*sigma_x_ferbel*(Cos(theta[i]*Pi()/180));
              
              delta_x[i] = padLenght*Tan(theta[i]*Pi()/180);
              delta_x_9mbar[i] = padLenght*Tan(theta[i]*Pi()/180);
              delta_x_11mbar[i] = padLenght*Tan(theta[i]*Pi()/180);
              
              delta_x_totWS[i] = sqrt((delta_x_assran[i]*delta_x_assran[i]) + (delta_x[i]*delta_x[i]));

              delta_x_WS_fd[i] = sqrt((delta_x[i]*delta_x[i]) + (delta_x_ferbel[i]*delta_x_ferbel[i]));

              delta_x_totWS_9mbar[i] = sqrt((delta_x_assran[i]*delta_x_assran[i]) + (delta_x[i]*delta_x[i]));

              delta_x_totWS_11mbar[i] = sqrt((delta_x_assran[i]*delta_x_assran[i]) + (delta_x[i]*delta_x[i])); 

              delta_x_tot[i] = delta_x_assran[i] + delta_x[i];
              delta_x_tot_9mbar[i] = delta_x_assran_9mbar[i] + delta_x_9mbar[i];
              delta_x_tot_11mbar[i] = sqrt(delta_x_assran_11mbar[i] + delta_x_11mbar[i]); 
           }

   TGraph *gr1 = new TGraph(10,theta,delta_x_assran);
   gr1->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1->SetLineWidth(1);
   gr1->SetLineStyle(10);
   gr1->SetMarkerStyle(1);
   //gr1->SetMarkerSize(1.2);
   gr1->SetMarkerColor(kBlack);
   gr1->GetXaxis()->SetTitle("#theta (deg)");
   gr1->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_9mbar = new TGraph(10,theta,delta_x_assran_9mbar);
   gr1_9mbar->SetTitle("Charge distribution width");
   gr1_9mbar->SetLineWidth(1);
   gr1_9mbar->SetLineStyle(10);
   gr1_9mbar->SetLineColor(kBlue);
   gr1_9mbar->SetMarkerStyle(1);
   //gr1_9mbar->SetMarkerSize(1.2);
   gr1_9mbar->SetMarkerColor(kBlue);
   gr1_9mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1_9mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_11mbar = new TGraph(10,theta,delta_x_assran_11mbar);
   gr1_11mbar->SetTitle("Charge distribution width");
   gr1_11mbar->SetLineWidth(1);
   gr1_11mbar->SetLineStyle(10);
   gr1_11mbar->SetLineColor(kRed);
   gr1_11mbar->SetMarkerStyle(1);
   //gr1_11mbar->SetMarkerSize(1.2);
   gr1_11mbar->SetMarkerColor(kRed);
   gr1_11mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1_11mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_ferbel = new TGraph(10,theta,delta_x_ferbel);
   gr1_ferbel->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1_ferbel->SetLineWidth(1);
   gr1_ferbel->SetLineStyle(10);
   gr1_ferbel->SetMarkerStyle(1);
   //gr1_ferbel->SetMarkerSize(1.2);
   gr1_ferbel->SetLineColor(kCyan+2);
   gr1_ferbel->GetXaxis()->SetTitle("#theta (deg)");
   gr1_ferbel->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_ferbel_ws = new TGraph(10,theta,delta_x_WS_fd);
   gr1_ferbel_ws->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1_ferbel_ws->SetLineWidth(3);
   gr1_ferbel_ws->SetLineStyle(1);
   gr1_ferbel_ws->SetMarkerStyle(1);
   //gr1_ferbel_ws->SetMarkerSize(1.2);
   gr1_ferbel_ws->SetMarkerColor(kCyan+3);
   gr1_ferbel_ws->SetLineColor(kCyan+2);
   gr1_ferbel_ws->GetXaxis()->SetTitle("#theta (deg)");
   gr1_ferbel_ws->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_geo = new TGraph(10,theta,delta_x);
   gr1_geo->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1_geo->SetLineWidth(2);
   gr1_geo->SetLineStyle(2);
   gr1_geo->SetMarkerStyle(1);
   //gr1_geo->SetMarkerSize(1.2);
   gr1_geo->SetMarkerColor(kBlack);
   gr1_geo->GetXaxis()->SetTitle("#theta (deg)");
   gr1_geo->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_geo_9mbar = new TGraph(10,theta,delta_x_9mbar);
   gr1_geo_9mbar->SetTitle("Charge distribution width");
   gr1_geo_9mbar->SetLineWidth(2);
   gr1_geo_9mbar->SetLineStyle(2);
   gr1_geo_9mbar->SetLineColor(kBlue);
   gr1_geo_9mbar->SetMarkerStyle(1);
   //gr1_geo_9mbar->SetMarkerSize(1.2);
   gr1_geo_9mbar->SetMarkerColor(kBlue);
   gr1_geo_9mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1_geo_9mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1_geo_11mbar = new TGraph(10,theta,delta_x_11mbar);
   gr1_geo_11mbar->SetTitle("Charge distribution width");
   gr1_geo_11mbar->SetLineWidth(2);
   gr1_geo_11mbar->SetLineStyle(2);
   gr1_geo_11mbar->SetLineColor(kRed);
   gr1_geo_11mbar->SetMarkerStyle(1);
   //gr1_geo_11mbar->SetMarkerSize(1.2);
   gr1_geo_11mbar->SetMarkerColor(kRed);
   gr1_geo_11mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1_geo_11mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1WS = new TGraph(10,theta,delta_x_totWS);
   gr1WS->SetTitle("Charge distribution width");
   gr1WS->SetLineWidth(3);
   gr1WS->SetLineStyle(1);
   gr1WS->SetMarkerStyle(1);
   //gr1WS->SetMarkerSize(1.2);
   gr1WS->SetMarkerColor(kBlack);
   gr1WS->GetXaxis()->SetTitle("#theta (deg)");
   gr1WS->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1WS_9mbar = new TGraph(10,theta,delta_x_totWS_9mbar);
   gr1WS_9mbar->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1WS_9mbar->SetLineWidth(3);
   gr1WS_9mbar->SetLineStyle(1);
   gr1WS_9mbar->SetLineColor(kBlue);
   gr1WS_9mbar->SetMarkerStyle(1);
   //gr1WS_9mbar->SetMarkerSize(1.2);
   gr1WS_9mbar->SetMarkerColor(kBlue);
   gr1WS_9mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1WS_9mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
   TGraph *gr1WS_11mbar = new TGraph(10,theta,delta_x_totWS_11mbar);
   gr1WS_11mbar->SetTitle("Charge distribution width ~ M-THGEM without RIM");
   gr1WS_11mbar->SetLineWidth(3);
   gr1WS_11mbar->SetLineStyle(1);
   gr1WS_11mbar->SetLineColor(kRed);
   gr1WS_11mbar->SetMarkerStyle(1);
   //gr1WS_11mbar->SetMarkerSize(1.2);
   gr1WS_11mbar->SetMarkerColor(kRed);
   gr1WS_11mbar->GetXaxis()->SetTitle("#theta (deg)");
   gr1WS_11mbar->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   
//################################################################################################################
// Experimental part
//################################################################################################################

   Int_t li,he = 0;
   char pippo[2];
   
   // Required variables
   TGraphErrors *gr1he[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1he[i]=new TGraphErrors();
      gr1he[i]->GetXaxis()->SetTitle("#theta (deg)");
      gr1he[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
      gr1he[i]->SetMarkerSize(1.2);
      gr1he[i]->SetMarkerStyle(20);
      gr1he[i]->SetMarkerColor(kGreen+2);
      gr1he[i]->SetLineColor(kGreen+2);
      gr1he[i]->SetLineWidth(2);
   }
   
   TGraphErrors *gr1li[5];
   for(Int_t i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr1li[i]=new TGraphErrors();
      gr1li[i]->SetLineWidth(2);
      gr1li[i]->SetMarkerSize(1.2);
      gr1li[i]->SetMarkerStyle(20);
      gr1li[i]->SetMarkerColor(kYellow+1);
      gr1li[i]->SetLineColor(kYellow+2);
      gr1li[i]->GetXaxis()->SetTitle("#theta (deg)");
      gr1li[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
   }
   
   // OpenFiles
   openTrackFile(run);
   tree->Print();
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,10,9);
   cutGa->SetPoint(1,175,198);
   cutGa->SetPoint(2,161,210);
   cutGa->SetPoint(3,5,10);
   cutGa->SetPoint(4,10,9);
   //cutGa->SaveAs(CutFileA);
   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,15,33);
   cutGli->SetPoint(1,77,130);
   cutGli->SetPoint(2,60,145);
   cutGli->SetPoint(3,7,50);
   cutGli->SetPoint(4,15,33);
   //cutGli->SaveAs(CutFileC);
   
   for(Int_t i= 0; i<200; i++){
      tree->GetEntry(i);
      if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
        he+=1;
        theta_He = theta_deg;
        cout << theta_He << endl;
        for(Int_t j=0; j<NRows; j++){
           x_exp_he = 5*cl_padMult[j];
           gr1he[j]->SetPoint(1,theta_He,x_exp_he);
        }     
      }else
      if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
        li+=1;
        theta_Li = theta_deg;
        cout << theta_Li << endl;
        for(Int_t j=0; j<NRows; j++){
           x_exp_li = 5*cl_padMult[j];
           gr1li[j]->SetPoint(1,theta_Li,x_exp_li);
        }
      }

//################################################################################################################
// Visualization block */
//################################################################################################################

   TLegend *l = new TLegend(0.1,0.45,0.5,0.9);
   l->AddEntry(gr1_9mbar,"Theo. predict. - Y. Assran - 9mbar","lp");
   l->AddEntry(gr1,"Theo. predict. - Y. Assran - 10mbar","lp");
   l->AddEntry(gr1_11mbar,"Theo. predict. - Y. Assran - 11mbar","lp");
   l->AddEntry(gr1_ferbel,"Theo. predict. - T. Ferbel - 9,10,11 mbar","lp");
   l->AddEntry(gr1_geo_9mbar,"Theo. predict. - geo - 9mbar","lp");
   l->AddEntry(gr1_geo,"Theo. predict. - geo - 10mbar","lp");
   l->AddEntry(gr1_geo_11mbar,"Theo. predict. - geo - 11mbar","lp");
   l->AddEntry(gr1WS_9mbar,"Theo. predict. - #Sigma^2(Assran-geo) - 9mbar","lp");
   l->AddEntry(gr1WS,"Theo. predict. - #Sigma^2(Assran-geo) - 10mbar","lp");
   l->AddEntry(gr1WS_11mbar,"Theo. predict. - #Sigma^2(Assran-geo) - 11mbar","lp");
   l->AddEntry(gr1_ferbel_ws, "Theo. predict. - #Sigma^2(Felber-geo) - 10mbar","lp");
   l->AddEntry(gr1he[0],"#alpha","lp");
   l->AddEntry(gr1li[0],"^{7}Li - 1400 Hz","lp");
   
   TLine* sep_line1 = new TLine(26.4407,0,26.4407,55);
   sep_line1->SetLineColor(kGray+2);
   sep_line1->SetLineWidth(3);
   sep_line1->SetLineStyle(3);
   TLine* sep_line2 = new TLine(31.0109,0,31.0109,55);
   sep_line2->SetLineColor(kGray+2);
   sep_line2->SetLineWidth(6);
   sep_line2->SetLineStyle(4);
   TText* diffusion = new TText(0.25,18.11,"Diffusion-dominated");
   diffusion->SetTextSize(0.025);
   diffusion->SetTextFont(112);
   TText* geometry = new TText(52.75,8.45,"Geometry-dominated");
   geometry->SetTextSize(0.025);
   geometry->SetTextFont(112);
   
   for(Int_t k=0; k<5;k++){
      TCanvas *c = new TCanvas("c","",900,800);
      c->SetGridx(1);
      c->SetGridy(1);
      c->SetTickx(1);
      c->SetTicky(1);
      c->cd();
      gr1->Draw("APC same");
      gr1->GetXaxis()->SetRangeUser(0,80);
      gr1->GetYaxis()->SetRangeUser(0,65);
      gr1_9mbar->Draw("PC SAME");
      gr1_11mbar->Draw("PC SAME");
      gr1_ferbel->Draw("PC SAME");
      gr1WS->Draw("PC SAME");
      gr1WS_9mbar->Draw("PC SAME");
      gr1WS_11mbar->Draw("PC SAME");
      gr1_geo->Draw("PC SAME");
      gr1_geo_9mbar->Draw("PC SAME");
      gr1_geo_11mbar->Draw("PC SAME");
      gr1li[k]->Draw("P SAME");
      gr1he[k]->Draw("P SAME");
      gr1_ferbel_ws->Draw("PC SAME");      
      sep_line1->Draw("SAME");
      sep_line2->Draw("SAME");
      diffusion->Draw("SAME");
      geometry->Draw("SAME");
      
      l->Draw("same");
      char titlepng[500];
      char titleeps[500];
      sprintf(titlepng,"Pictures_Analysis/Pulsed/Run443/row%d/%d_DEF_SQ_Assran_Ferbel_NEW_Charge distribution width - FWAB - Row%d.png",k,i,k);
      sprintf(titleeps,"Pictures_Analysis/Pulsed/Run443/row%d/%d_DEF_SQ_Assran_Ferbel_NEW_Charge distribution width - FWAB - Row%d.png",k,i,k);
      //gpad->SetLogy();
      c->SaveAs(titlepng);
      c->SaveAs(titleeps);
   }
   
   }
}
