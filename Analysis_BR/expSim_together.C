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
   
   Int_t e = 0;
   Double_t pad[NRows][100] = {0.};
   Double_t charge[NRows][100] = {0.};
   char tasto;
   Int_t simul_pad0[7] = {6,7,8,9,10,11,12};
   Int_t simul_pad1[7] = {12,13,14,15,16,0,0};
   Int_t simul_pad2[7] = {17,18,19,20,21,22,0};
   Int_t simul_pad3[7] = {23,24,25,26,27,28,0};
   Int_t simul_pad4[7] = {28,29,30,31,32,33,34};
   Int_t simul_charge0[7] = {140202,3434945,9411047,9673926,5555497,490706,17525};
   Int_t simul_charge1[7] = {841211,7308020,9708976,8569836,2032926,0,0};
   Int_t simul_charge2[7] = {87626,2628784,9323421,10059481,5783325,525757,0};
   Int_t simul_charge3[7] = {876261,6519385,10077006,8657462,2488582,35050,0};
   Int_t simul_charge4[7] = {87626,2874137,9165694,9971855,5835901,473181,17525};
   
   char histoname[100];
      
   TH1D *chargeDistr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"chargeDistr %i",i);
      chargeDistr[i]=new TH1D("","",60,-0.5,59.5);
      chargeDistr[i]->GetXaxis()->SetTitle("pad number");
      chargeDistr[i]->GetXaxis()->SetTitleSize(0.04);
      chargeDistr[i]->GetXaxis()->SetLabelSize(0.04);
      chargeDistr[i]->GetXaxis()->SetTitleOffset(.9);
      chargeDistr[i]->GetYaxis()->SetTitle("Q (bit)");
      chargeDistr[i]->GetYaxis()->SetTitleSize(0.04);
      chargeDistr[i]->GetYaxis()->SetLabelSize(0.04);
      chargeDistr[i]->GetYaxis()->SetTitleOffset(1.36);
      //chargeDistr[i]->SetStats(0);
      chargeDistr[i]->SetNdivisions(7);
      //chargeDistr[i]->SetFillColor(i+1);
      chargeDistr[i]->SetLineColor(kRed);
      chargeDistr[i]->SetLineWidth(2);
      chargeDistr[i]->SetFillColor(kRed-7);
      chargeDistr[i]->SetFillStyle(3004);
      //chargeDistr[i]->GetXaxis()->SetRangeUser(0,1.2e4);
   }
   
   TH1I *simul[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"simul %i",i);
      simul[i]=new TH1I("","",60,-0.5,59.5);
      simul[i]->GetXaxis()->SetTitle("pad number");
      simul[i]->GetXaxis()->SetTitleSize(0.04);
      simul[i]->GetXaxis()->SetLabelSize(0.04);
      simul[i]->GetXaxis()->SetTitleOffset(.9);
      simul[i]->GetYaxis()->SetTitle("Q (bit)");
      simul[i]->GetYaxis()->SetTitleSize(0.04);
      simul[i]->GetYaxis()->SetLabelSize(0.04);
      simul[i]->GetYaxis()->SetTitleOffset(1.36);
      //simul[i]->SetStats(0);
      simul[i]->SetNdivisions(7);
      //simul[i]->SetFillColor(i+1);
      simul[i]->SetLineColor(kCyan+2);
      simul[i]->SetLineWidth(2);
      simul[i]->SetFillColor(kCyan+1);
      simul[i]->SetFillStyle(3005);
      //simul[i]->GetXaxis()->SetRangeUser(0,1.2e4);
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

   TCutG *cutG = new TCutG("cutG",14);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,84.5357,115.814);
   cutG->SetPoint(1,84.1774,115.997);
   cutG->SetPoint(2,83.6477,115.89);
   cutG->SetPoint(3,83.0556,115.388);
   cutG->SetPoint(4,82.6661,114.672);
   cutG->SetPoint(5,82.5648,114.154);
   cutG->SetPoint(6,82.5726,113.788);
   cutG->SetPoint(7,82.7284,113.392);
   cutG->SetPoint(8,83.2426,113.194);
   cutG->SetPoint(9,83.9281,113.53);
   cutG->SetPoint(10,84.4968,114.245);
   cutG->SetPoint(11,84.7461,115.053);
   cutG->SetPoint(12,84.6915,115.555);
   cutG->SetPoint(13,84.5357,115.814);

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

   for (Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      for(Int_t r=0; r<NRows; r++){
         chargeDistr[r]->Reset();
      }
      if(/*cl_charge[0]>7000 && cl_charge[1]>7000 && cl_charge[2]>7000 && cl_charge[3]>7000 && cl_charge[4]>7000 &&*/theta_deg>53 && theta_deg<55 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      cout << "Event inside TCutG: " << i << endl;
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
   TCanvas *c_provv = new TCanvas("c_provv","c_provv",1000,800);
   c_provv->Divide(3,2);
   for(Int_t j=0; j<NRows; j++){
      c_provv->cd(j+1);
      chargeDistr[j]->Draw();
      simul[j]->Draw("SAME");
      //l->Draw("SAME");
   }
   cout << "Do you wabt to save? Type [Y]es or [N]o: " ;
   cin >> tasto;
   if(tasto== 'Y'){
      break;
     }else{
       cout << "Ok!" << endl;
     }
   }else{                                                                             // TCutg parenthesis
            cout << "Evt: " << endl;
         }
   }
   
   for(Int_t k=0; k<7; k++){
      simul[0]->SetBinContent(simul_pad0[k]+9,simul_charge0[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000)*2/3);
      simul[1]->SetBinContent(simul_pad1[k]+9,simul_charge1[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000)*2/3);
      simul[2]->SetBinContent(simul_pad2[k]+9,simul_charge2[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000)*2/3);
      simul[3]->SetBinContent(simul_pad3[k]+9,simul_charge3[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000)*2/3);
      simul[4]->SetBinContent(simul_pad4[k]+9,simul_charge4[k]*1.5*(20 * (pow(2,16))) / (2.762e5 * 8000)*2/3);
   }
    
   
//#################################################################################################
// Visualization block

   TLegend *l = new TLegend(0.78,0.7,0.98,0.9);
   l->SetTextSize(0.035);
   //l->SetLineWidth(0);
   //l->SetFillStyle(0);
   l->AddEntry(chargeDistr[0], "Exp.", "f");
   l->AddEntry(simul[0], "Sim.", "f");

   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t j=0; j<NRows; j++){
      c->cd(j+1);
      chargeDistr[j]->Draw();
      gStyle->SetOptStat(0);
      simul[j]->Draw("SAME");
      l->Draw("SAME");
      c->Update();
   }
   
}
