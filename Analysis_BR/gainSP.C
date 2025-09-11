//################################################################################################################
//#   This macro allows to evaluate the gain G=I_{an}/I_{primaries}.
//################################################################################################################
//#   Created August 2025 by A. Pitronaci 
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

void gainSP(int run){

//###########################################################################################################
// Needed Variables
   
   TH1D *charge = new TH1D("charge","",1000,0,50000);
   
   Double_t mean_charge = 0.;
   Double_t primaries = 0.;
   Double_t w_gas = 0.;
   Double_t solid_angle =0.;
   Double_t rate = 0.;
   Double_t t = 0.;
   Double_t gain = 0.;
   Double_t dE = 0.;
   
     
//###########################################################################################################
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

//###########################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
      Double_t tot_charge = 0.;
      //if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      tot_charge = cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10];
      charge->Fill(tot_charge)
      //}
   }
   
   mean_charge = tot_charge->Fit("gaus","R+");
   primaries = (dE/w_gas);
   gain=tot_charge/primaries;
   cout << "Gain: " << gain << endl;

}
