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
   
   TH1D *charge = new TH1D("charge","",250,0,1e7);
   TF1 *f = new TF1("f","gaus",0,5e5);
   f->SetParameters(0.,0.,0.);
   
   Double_t mean_charge = 0.;
   Double_t primaries = 0.;
   Double_t w_gas = 23.4;
   Double_t solid_angle =0.;
   Double_t rate = 0.;
   Double_t t = 0.;
   Double_t gain = 0.;
   Double_t dE = 0.351e6;
   char titolofile[100];
   Double_t v = 0.;
   
   ofstream outputfile;
   
     
//###########################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   sprintf(titolofile,"Gain/gainTHGEM.txt");
   outputfile.open(titolofile,std::ios::app);
   //outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,25,39);
   cutGli->SetPoint(1,122,150);
   cutGli->SetPoint(2,112,169);
   cutGli->SetPoint(3,21,51);
   cutGli->SetPoint(4,25,39);
   
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
      tree->GetEntry(i);
      Double_t tot_charge = 0.;
      if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      //tot_charge = cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10];
      charge->Fill(cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10]);
      }                                                                        //TCutg parenthesis
   }
   charge->Fit("f","","",0.1,1e5);
   mean_charge = ((f->GetParameter(1))*2*61e3)/(20*3.62);
   primaries = dE/w_gas;
   gain=mean_charge/primaries;
   cout << "Mean charge: " << mean_charge << endl;
   cout << "Primaries: " << primaries << endl;
   cout << "Gain: " << gain << endl;
   
   cout << "Insert V_THGEM: " << endl;
   cin >> v;
   //outputfile << v << "	" << gain << endl;
   
   TCanvas *c = new TCanvas("c");
   c->cd();
   charge->Draw();
   
   
}
