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
   
   Double_t simul_pad0[3] = {5,6,7};
   Double_t simul_pad1[4] = {6,7,8,9};
   Double_t simul_pad2[4] = {8,9,10,11};
   Double_t simul_pad3[4] = {10,11,12,13};
   Double_t simul_pad4[4] = {12,13,14,15};
   Double_t simul_charge0[3] = {689761.5,3037646.6,258379.7};
   Double_t simul_charge1[4] = {2246.8,1139117.5,2610758.4,128066.5};
   Double_t simul_charge2[4] = {22467.8,1579486.3,2273741.4,53922.7};
   Double_t simul_charge3[4] = {40442,2152415.2,1707552.8,8987.1};
   Double_t simul_charge4[4] = {103351.9,2547848.5,1316613.1,8987.1};
   
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
      chargeDistr[i]->SetLineWidth(2);
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
      simul[i]->SetLineWidth(2);
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

   for (Int_t i=158; i<159; i++){
      tree->GetEntry(i);
      
      if(theta_deg>=20 && theta_deg<30. && cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            chargeDistr[row]->SetBinContent(pad[row][p],charge[row][p]);
         }
         
         
      }
   
   char t;
   cout << "Evt: " << i << endl;
   cin >> t ;
   }else{                                                                             // TCutg parenthesis
            cout << "Evt: " << endl;
         }
   }
   
   for(Int_t k=0; k<8; k++){
      simul[0]->SetBinContent(simul_pad0[k]+13,simul_charge0[k]*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[1]->SetBinContent(simul_pad1[k]+14,simul_charge1[k]*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[4]->SetBinContent(simul_pad4[k]+16,simul_charge2[k]*(20 * (pow(2,16))) / (2.762e5 * 8000));
   }
   
   for(Int_t k=0; k<9; k++){
      simul[2]->SetBinContent(simul_pad2[k]+14,simul_charge2[k]*(20 * (pow(2,16))) / (2.762e5 * 8000));
      simul[3]->SetBinContent(simul_pad3[k]+14,simul_charge3[k]*(20 * (pow(2,16))) / (2.762e5 * 8000));
   }
    
   
//#################################################################################################
// Visualization block

   TLegend *l = new TLegend(0.78,0.58,0.98,0.78);
   l->SetTextSize(0.035);
   //l->SetLineWidth(0);
   //l->SetFillStyle(0);
   l->AddEntry(chargeDistr[0], "Charge_{exp}(pad)", "f");
   l->AddEntry(simul[0], "Charge_{simul}(pad)", "f");

   TCanvas *c = new TCanvas("c","c",1600,200);
   c->Divide(5,1);
   for(Int_t j=0; j<NRows; j++){
      c->cd(j+1);
      chargeDistr[j]->Draw();
      simul[j]->Draw("SAME");
      l->Draw("SAME");
   }
   
}
