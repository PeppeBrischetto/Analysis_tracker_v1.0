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
   Double_t dE = 0.511e6;
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

   TCutG *cutG = new TCutG("cutG",5);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,80,112.8);
   cutG->SetPoint(1,80,111.6);
   cutG->SetPoint(2,80.6,111.6);
   cutG->SetPoint(3,80.9,112.8);
   cutG->SetPoint(4,80.,112.8);
   
//###########################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      Double_t tot_charge = 0.;
      if(cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      //tot_charge = cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10];
      charge->Fill(cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10]);
      }                                                                        //TCutg parenthesis
   }
   charge->Fit("f","","",0.,4e6);
   mean_charge = ((f->GetParameter(1))*2*61e3)/(20*3.62);
   primaries = dE/w_gas;
   gain=mean_charge/primaries;
   cout << "Mean charge: " << mean_charge << endl;
   cout << "Primaries: " << primaries << endl;
   cout << "Gain: " << gain << endl;
   
   cout << "Insert V_THGEM: " << endl;
   cin >> v;
   outputfile << v << "	" << gain << endl;
   
   TCanvas *c = new TCanvas("c");
   c->cd();
   charge->Draw();
   
   
}
