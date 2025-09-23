//#################################################################################################################
//#   This macro allows to evaluate the error on the x^corr_{i} having previously corrected the systematic offset.
//#################################################################################################################
//#   Created September 2025 by A. Pitronaci 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void crossTalkNEW(int run){

//################################################################################################################
// Variables

   char tasto[10]; 
   char titolofile[100];
   char histoname[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   Double_t chi_root = 0.;
   Double_t my_chi = 0.;
   Double_t my_chiRed = 0.;
   Double_t Ndof = 3;
   Double_t offset[NRows] = {-0.764822,0.8826,0.403131,-0.39966,-0.146673};
   Double_t x_corr[NRows] = {0.};
   Double_t newOffset[NRows] = {0.};
   Double_t chiRed_min1 = 0.;
   Double_t chiRed_tot = 0.;
   Double_t rapp = 0.;
   Double_t pearson = 0.;
   Double_t m = 0.;
   Double_t q = 0.;
   Double_t errX[NRows] = {0.};
   Double_t errM = 0.;
   Double_t errQ = 0.;
   Double_t mean_errX[NRows] = {0.};
   Double_t x_foc = 0.;
   
   Double_t theta_deg = 0.;
   Double_t intercetta = 0.;
   Double_t coeffAng = 0.;
   
   ofstream outfile;
   
   char provv;

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *h_theta = new TH1D("h_theta","",900,0,90);
   h_theta->GetXaxis()->SetTitle("#theta_{foc} (deg)");
   h_theta->GetXaxis()->SetTitleSize(0.05);
   h_theta->GetXaxis()->SetLabelSize(0.05);
   h_theta->GetXaxis()->SetTitleOffset(.9);
   //h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetYaxis()->SetTitleSize(0.05);
   h_theta->GetYaxis()->SetLabelSize(0.05);
   h_theta->GetYaxis()->SetTitleOffset(1.);
   h_theta->SetNdivisions(7);
   h_theta->SetLineWidth(2);
   h_theta->SetStats(0);
   
   TH1D *h_intercetta = new TH1D("h_intercetta","",300,-30,0);
   h_intercetta->GetXaxis()->SetTitle("intercept (mm)");
   h_intercetta->GetXaxis()->SetTitleSize(0.05);
   h_intercetta->GetXaxis()->SetLabelSize(0.05);
   h_intercetta->GetXaxis()->SetTitleOffset(.9);
   //h_intercetta->GetYaxis()->SetTitle("Counts");
   h_intercetta->GetYaxis()->SetTitleSize(0.05);
   h_intercetta->GetYaxis()->SetLabelSize(0.05);
   h_intercetta->GetYaxis()->SetTitleOffset(1.);
   h_intercetta->SetNdivisions(7);
   h_intercetta->SetLineWidth(2);
   h_intercetta->SetStats(0);
   
   TH1D *h_chi = new TH1D("h_chi","",50,-0.01,10);
   h_chi->GetXaxis()->SetTitle("#chi^{2}");
   h_chi->GetXaxis()->SetTitleSize(0.05);
   h_chi->GetXaxis()->SetLabelSize(0.05);
   h_chi->GetXaxis()->SetTitleOffset(.9);
   h_chi->GetYaxis()->SetTitle("Counts");
   h_chi->GetYaxis()->SetTitleSize(0.05);
   h_chi->GetYaxis()->SetLabelSize(0.05);
   h_chi->GetYaxis()->SetTitleOffset(1.);
   h_chi->SetNdivisions(7);
   h_chi->SetLineWidth(2);
   h_chi->SetStats(0);
   
   TH1D *h_chiRed = new TH1D("h_chiRed","",200,0,10);
   h_chiRed->GetXaxis()->SetTitle("#tilde{#chi}^{2}_{red}");
   h_chiRed->GetXaxis()->SetTitleSize(0.05);
   h_chiRed->GetXaxis()->SetLabelSize(0.05);
   h_chiRed->GetXaxis()->SetTitleOffset(.9);
   h_chiRed->GetYaxis()->SetTitle("Counts");
   h_chiRed->GetYaxis()->SetTitleSize(0.05);
   h_chiRed->GetYaxis()->SetLabelSize(0.05);
   h_chiRed->GetYaxis()->SetTitleOffset(1.);
   h_chiRed->SetNdivisions(7);
   h_chiRed->SetLineWidth(2);
   
   TH1D *h_corr = new TH1D("h_corr","",1400,0.995,1.002);
   h_corr->GetXaxis()->SetTitle("r");
   h_corr->GetXaxis()->SetTitleSize(0.05);
   h_corr->GetXaxis()->SetLabelSize(0.05);
   h_corr->GetXaxis()->SetTitleOffset(.9);
   h_corr->GetYaxis()->SetTitle("Counts");
   h_corr->GetYaxis()->SetTitleSize(0.05);
   h_corr->GetYaxis()->SetLabelSize(0.05);
   h_corr->GetYaxis()->SetTitleOffset(1.);
   h_corr->SetNdivisions(7);
   h_corr->SetLineWidth(2);
   
   TH1D *h_foc = new TH1D("h_foc","",1400,10.,60.);
   h_foc->GetXaxis()->SetTitle("x_{z=0} (mm)");
   h_foc->GetXaxis()->SetTitleSize(0.05);
   h_foc->GetXaxis()->SetLabelSize(0.05);
   h_foc->GetXaxis()->SetTitleOffset(.9);
   h_foc->GetYaxis()->SetTitle("Counts");
   h_foc->GetYaxis()->SetTitleSize(0.05);
   h_foc->GetYaxis()->SetLabelSize(0.05);
   h_foc->GetYaxis()->SetTitleOffset(1.);
   h_foc->SetNdivisions(7);
   h_foc->SetLineWidth(2);
   
   TH1D *h_errX[NRows];
   for(Int_t i=0; i<NRows; i++){
      sprintf(histoname,"h_errX_%d",i);
      h_errX[i] = new TH1D("h_errX","",250,0,10);
      h_errX[i]->GetXaxis()->SetTitle("#varepsilon_{x} (mm)");
      h_errX[i]->GetXaxis()->SetTitleSize(0.05);
      h_errX[i]->GetXaxis()->SetLabelSize(0.05);
      h_errX[i]->GetXaxis()->SetTitleOffset(.9);
      h_errX[i]->GetYaxis()->SetTitle("Counts");
      h_errX[i]->GetYaxis()->SetTitleSize(0.05);
      h_errX[i]->GetYaxis()->SetLabelSize(0.05);
      h_errX[i]->GetYaxis()->SetTitleOffset(1.);
      h_errX[i]->SetNdivisions(7);
      h_errX[i]->SetLineWidth(2);
   }
   
   TH1D *discr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"discr %i",i);
      discr[i]=new TH1D("","",500,-15,15);
      discr[i]->GetXaxis()->SetTitle("(x_{i} - f(z_{i})) (mm)");
      discr[i]->GetXaxis()->SetTitleSize(0.05);
      discr[i]->GetXaxis()->SetLabelSize(0.05);
      discr[i]->GetXaxis()->SetTitleOffset(1.);
      discr[i]->GetYaxis()->SetTitle("Counts");
      discr[i]->GetYaxis()->SetTitleSize(0.05);
      discr[i]->GetYaxis()->SetLabelSize(0.05);
      discr[i]->GetYaxis()->SetTitleOffset(1.);
      discr[i]->SetNdivisions(7);
      discr[i]->SetFillColor(kBlue-5);
   }
   
   TH1D *amplitude[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"amplitude %i",i);
      amplitude[i]=new TH1D("","",500,-15,15);
      amplitude[i]->GetXaxis()->SetTitle("#sqrt{(x_{i} - f(z_{i}))^{2}} (mm)");
      amplitude[i]->GetXaxis()->SetTitleSize(0.05);
      amplitude[i]->GetXaxis()->SetLabelSize(0.05);
      amplitude[i]->GetXaxis()->SetTitleOffset(1.);
      amplitude[i]->GetYaxis()->SetTitle("Counts");
      amplitude[i]->GetYaxis()->SetTitleSize(0.05);
      amplitude[i]->GetYaxis()->SetLabelSize(0.05);
      amplitude[i]->GetYaxis()->SetTitleOffset(1.);
      amplitude[i]->SetNdivisions(7);
      amplitude[i]->SetFillColor(kBlue-5);
   }
   
   TH1D *scatter[10];
   for(int i=0; i<10; i++){
      sprintf(histoname,"scatter %i",i);
      scatter[i]=new TH1D("","",500,-15,15);
      scatter[i]->GetXaxis()->SetTitle("(x_{r} - f(z_{r}))-(x_{s} - f(z_{s})) (mm)");
      scatter[i]->GetXaxis()->SetTitleSize(0.05);
      scatter[i]->GetXaxis()->SetLabelSize(0.05);
      scatter[i]->GetXaxis()->SetTitleOffset(1.);
      scatter[i]->GetYaxis()->SetTitle("Counts");
      scatter[i]->GetYaxis()->SetTitleSize(0.05);
      scatter[i]->GetYaxis()->SetLabelSize(0.05);
      scatter[i]->GetYaxis()->SetTitleOffset(1.);
      scatter[i]->SetNdivisions(7);
      scatter[i]->SetFillColor(kBlue-5);
   }
   
   TF1 *norm = new TF1("norm","gaus",-5,5);
   norm->SetParameters(1,0,1);
   
   ofstream outputfile;
   TGraph *retta = new TGraph(5);
   retta->SetTitle("(x_{i},z_{i})-pairs - track reconstruction");
   retta->SetMarkerStyle(20);
   retta->SetLineWidth(0);
   retta->GetXaxis()->SetTitle("x (mm)");
   retta->GetYaxis()->SetTitle("z (mm)");
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   //sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d_4He.txt",run);
   //outputfile.open(titolofile);
   //outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,33,41);
   cutGli->SetPoint(1,135,165);
   cutGli->SetPoint(2,115,176);
   cutGli->SetPoint(3,20,53);
   cutGli->SetPoint(4,33,41);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,25,16);
   cutGa->SetPoint(1,195,207);
   cutGa->SetPoint(2,173,216);
   cutGa->SetPoint(3,13,22);
   cutGa->SetPoint(4,25,16);

//#################################################################################################
// Data loop
   for(Int_t i=0; i<entries; i++){
      my_chi = 0.;
      chi_root = 0.;
      my_chiRed = 0.;
      pearson = 0.;
      errM = 0.;
      errQ = 0.;
      x_foc = 0.;
      
      Double_t theta_fit = 0.;
      Double_t pad[NRows][100] = {0.};
      Double_t charge[NRows][100] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t y[NRows] = {0.};
      Double_t totalCharge[NRows] = {0.};
      Double_t scarto[NRows] = {0.};
      Double_t ampiezza[NRows] = {0.};

      tree->GetEntry(i);
      retta->Set(0);

      TF1 *f = new TF1(Form("f_%d", i), "[0] + [1]*x", 0, 300);
      f->SetParameters(0, 0);
      
      if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            anode->Fill(pad[row][p], 2 * row + 1, charge[row][p]);
            x_mm[p] = 2.5 + (5 * pad[row][p]);
            x[row] += (x_mm[p] * charge[row][p]);
         }
         Double_t my_Singlechi = 0.;
         x[row] = x[row] / (totalCharge[row]);
         x_corr[row] = x[row]-offset[row];
         retta->AddPoint(x_corr[row], z[row]);
         
      }
      
      char fit[100];
      sprintf(fit,"f_%d",i);
      retta->Fit(fit,"","+",0,300);
      intercetta = f->GetParameter(0);
      coeffAng = f->GetParameter(1);
      errM = f->GetParError(1);
      errQ = f->GetParError(0);
      pearson = retta->GetCorrelationFactor();
      h_corr->Fill(pearson);
      
      if(i==0 || i==100 || i==200 || i==300 || i==400 || i==500 || i==600 || i==700){
         //TCanvas *c_retta = new TCanvas("c_retta");
         //c_retta->cd();
         //retta->Draw();
         //c_retta->Update();
         char tit_retta[100];
         sprintf(tit_retta,"Pictures_Analysis/TrackQualityControl/Run%d/bestTrack_corr%d.png",run,i);
         //c_retta->SaveAs(tit_retta);
      }
      
      theta_deg = 90-(ATan(f->GetParameter(1))*180/Pi());
      h_theta->Fill(theta_deg);
      
      h_intercetta->Fill(intercetta);
      
      for(Int_t row=0; row<NRows; row++){
         my_chi += (pow(z[row]-f->Eval(x_corr[row]),2)/1);
         scarto[row] = x_corr[row] - (f->GetX(z[row]));
         discr[row]->Fill(scarto[row]);
         ampiezza[row] = sqrt(scarto[row]*scarto[row]);
         amplitude[row]->Fill(ampiezza[row]);
         errX[row] = sqrt(pow((1/coeffAng)*errM,2) /*+ pow((-1/coeffAng)*10,2)*/ + pow(((intercetta-f->Eval(x[row]))/(pow(coeffAng,2))*errM),2) /*pow(scarto[row],2)*/);
         if(row==NRows-1){
            h_chi->Fill(my_chi);
         }
         h_errX[row]->Fill(errX[row]);
         h_errX[row]->Fit("norm","","",0,5);
         mean_errX[row] = norm->GetParameter(1);
      }
      
      scatter[0]->Fill(scarto[1]-scarto[0]);
      scatter[1]->Fill(scarto[2]-scarto[0]);
      scatter[2]->Fill(scarto[3]-scarto[0]);
      scatter[3]->Fill(scarto[4]-scarto[0]);
      scatter[4]->Fill(scarto[2]-scarto[1]);
      scatter[5]->Fill(scarto[3]-scarto[1]);
      scatter[6]->Fill(scarto[4]-scarto[1]);
      scatter[7]->Fill(scarto[3]-scarto[2]);
      scatter[8]->Fill(scarto[4]-scarto[2]);
      scatter[9]->Fill(scarto[4]-scarto[3]);
      
      
      x_foc = -intercetta/coeffAng;
      cout << "x_foc: " << x_foc << endl;
      h_foc->Fill(x_foc);
      my_chiRed = my_chi/Ndof;
      h_chiRed->Fill(my_chiRed);
      
      /* cout trials */
      cout << "My chi: " << my_chi << "    chi^2 reduced: " << endl;
      }                                                                      // TCutg parenthesis
      /*cout << " *************************************** " << endl;
      cout << "Event: " << i << endl;
      for(Int_t row=0; row<NRows; row++){
         printf("x%d= %1.4f and z%d= %1.4f\n ",i,x[row],i,z[row]);
      }
      cout << "Theta (deg): " << theta_deg << endl;*/

      anode->Reset("ICES");
      
      //cin >> provv;
   }
   
   TCanvas *cprova = new TCanvas("cprova","cprova",1600,1600);
   cprova->Divide(4,3);
   for(Int_t b=0; b<10; b++){
      cprova->cd(b+1);
      scatter[b]->Draw();
   }
   
}
