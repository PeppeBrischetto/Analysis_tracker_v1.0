//################################################################################################################
//#   This macro allows to perform a chi^2 quality control on the track events coming from the IRRAD4 experiment.
//#   In particular, for each track, this macro extrapolates the (pad,row)-pairs, thus makes a fit of them and 
//#   plots the chi^2 in order to discern pathological events.
//################################################################################################################
//#   Created July 2025 by A. Pitronaci 
//################################################################################################################
//#   Updated:
//################################################################################################################

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

void expSim_together(int run){

//################################################################################################################
// Needed variables
   
   Double_t pad[NRows][100] = {0.};
   Double_t charge[NRows][100] = {0.};
   
   Double_t simul_pad0[9] = {7,8,9,10,11,12,13,14,15};
   Double_t simul_pad1[9] = {14,15,16,17,18,19,20,21,22};
   Double_t simul_pad2[9] = {22,23,24,25,26,27,28,29,30};
   Double_t simul_pad3[9] = {30,31,32,33,34,35,36,37,38};
   Double_t simul_pad4[9] = {38,39,40,41,42,43,44,45,46};
   Double_t simul_charge0[9] = {43147,743716,2140312,2738691,2605844,1799679,423521,15896,1135};
   Double_t simul_charge1[9] = {2271,85158,962857,2294733,2680784,2586542,1619143,303164,7948};
   Double_t simul_charge2[9] = {2271,139660,1154747,2446882,2719389,2487758,1404544,211193,4542};
   Double_t simul_charge3[9] = {4542,190755,1418170,2528634,2646720,2416225,1092297,141930,1135};
   Double_t simul_charge4[9] = {11354,338362,1706573,2596761,2646720,2256128,887917,77210,0};
   
   char histoname[100];
      
   TH1D *chargeDistr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"chargeDistr %i",i);
      chargeDistr[i]=new TH1D("","",60,-0.5,59.5);
      chargeDistr[i]->GetXaxis()->SetTitle("pad number");
      chargeDistr[i]->GetXaxis()->SetTitleSize(0.05);
      chargeDistr[i]->GetXaxis()->SetLabelSize(0.05);
      chargeDistr[i]->GetXaxis()->SetTitleOffset(.9);
      chargeDistr[i]->GetYaxis()->SetTitle("Charge (bit)");
      chargeDistr[i]->GetYaxis()->SetTitleSize(0.05);
      chargeDistr[i]->GetYaxis()->SetLabelSize(0.05);
      chargeDistr[i]->GetYaxis()->SetTitleOffset(1.);
      //chargeDistr[i]->SetStats(0);
      chargeDistr[i]->SetNdivisions(7);
      //chargeDistr[i]->SetFillColor(i+1);
      chargeDistr[i]->SetLineColor(kRed);
      chargeDistr[i]->SetLineWidth(1);
      chargeDistr[i]->SetFillColor(kRed-7);
      chargeDistr[i]->SetFillStyle(3001);
      chargeDistr[i]->GetXaxis()->SetRangeUser(0,2800);
   }
   
   TH1D *simul[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"simul %i",i);
      simul[i]=new TH1D("","",60,-0.5,59.5);
      simul[i]->GetXaxis()->SetTitle("pad number");
      simul[i]->GetXaxis()->SetTitleSize(0.05);
      simul[i]->GetXaxis()->SetLabelSize(0.05);
      simul[i]->GetXaxis()->SetTitleOffset(.9);
      simul[i]->GetYaxis()->SetTitle("Charge (bit)");
      simul[i]->GetYaxis()->SetTitleSize(0.05);
      simul[i]->GetYaxis()->SetLabelSize(0.05);
      simul[i]->GetYaxis()->SetTitleOffset(1.);
      //simul[i]->SetStats(0);
      simul[i]->SetNdivisions(7);
      //simul[i]->SetFillColor(i+1);
      simul[i]->SetLineColor(kCyan+2);
      simul[i]->SetLineWidth(1);
      simul[i]->SetFillColor(kCyan+1);
      simul[i]->SetFillStyle(3001);
   }
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   //sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d_4He.txt",run);
   //outputfile.open(titolofile);
   //outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",12);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,59.8067,103.758);
   cutG->SetPoint(1,59.572,103.989);
   cutG->SetPoint(2,59.0896,104.023);
   cutG->SetPoint(3,58.692,103.758);
   cutG->SetPoint(4,58.5095,103.493);
   cutG->SetPoint(5,58.4182,103.151);
   cutG->SetPoint(6,58.5225,102.787);
   cutG->SetPoint(7,58.9201,102.556);
   cutG->SetPoint(8,59.4547,102.677);
   cutG->SetPoint(9,59.8067,103.074);
   cutG->SetPoint(10,59.8979,103.482);
   cutG->SetPoint(11,59.8067,103.758);

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

   for (Int_t i=18; i<19; i++){
      tree->GetEntry(i);
      for(Int_t r=0; r<NRows; r++){
         chargeDistr[r]->Reset();
      }
      if(/*theta_deg>=20 && theta_deg<30. &&*/ cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            chargeDistr[row]->SetBinContent(pad[row][p],charge[row][p]);
         }
         
         
      }
   
   /*char t;
   cout << "Evt: " << i << endl;
   cin >> t ;*/
   }else{                                                                             // TCutg parenthesis
            cout << "Evt: " << endl;
         }
   }
   
   for(Int_t k=0; k<9; k++){
      simul[0]->SetBinContent(simul_pad0[k],simul_charge0[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[1]->SetBinContent(simul_pad1[k],simul_charge1[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[2]->SetBinContent(simul_pad2[k],simul_charge2[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[3]->SetBinContent(simul_pad3[k],simul_charge3[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[4]->SetBinContent(simul_pad4[k],simul_charge4[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000));
   }
    
   
//#################################################################################################
// Visualization block

   TLegend *l = new TLegend(0.78,0.7,0.98,0.9);
   l->SetTextSize(0.035);
   //l->SetLineWidth(0);
   //l->SetFillStyle(0);
   l->AddEntry(chargeDistr[0], "Exp.", "f");
   l->AddEntry(simul[0], "Sim.", "f");

   TCanvas *c = new TCanvas("c","c",1400,500);
   c->Divide(5,1);
   for(Int_t j=0; j<NRows; j++){
      c->cd(j+1);
      chargeDistr[j]->Draw();
      simul[j]->Draw("SAME");
      l->Draw("SAME");
   }
   
}
