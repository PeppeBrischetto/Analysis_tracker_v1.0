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
   Double_t simul_pad[NRows][100] = {0.};
   Double_t simul_charge[NRows][100] = {0.};
   char histoname[100];
   ifstream simulFile;
      
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
      simul[i]->SetLineColor(kRed);
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

   for (Int_t i=0; i<1; i++){
      tree->GetEntry(i);
      
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            chargeDistr[row]->SetBinContent(pad[row][p],charge[row][p]);
         }
      }
   }

//#################################################################################################
// Visualization block

   TLegend *l = new TLegend(0.1,0.7,0.3,0.9);
   l->SetTextSize(0.035);
   //l->AddEntry(discr[0], "x[row] - (f(z[row]))", "f");

   TCanvas *c = new TCanvas("c");
   c->Divide(3,2);
   for(Int_t j=0; j<NRows; j++){
      c->cd(j+1);
      chargeDistr[j]->Draw();
   }
   
}
