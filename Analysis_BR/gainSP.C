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
   Double_t dE = 0.317e6;
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

   TCutG *cutG = new TCutG("cutG",16);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,80.5734,113.448);
   cutG->SetPoint(1,80.107,113.189);
   cutG->SetPoint(2,79.8539,112.9);
   cutG->SetPoint(3,79.6007,112.397);
   cutG->SetPoint(4,79.4408,111.788);
   cutG->SetPoint(5,79.4142,111.316);
   cutG->SetPoint(6,79.5874,110.935);
   cutG->SetPoint(7,79.8672,110.752);
   cutG->SetPoint(8,80.2136,110.828);
   cutG->SetPoint(9,80.5867,111.255);
   cutG->SetPoint(10,80.8398,111.651);
   cutG->SetPoint(11,80.9864,111.97);
   cutG->SetPoint(12,81.1196,112.443);
   cutG->SetPoint(13,81.133,112.991);
   cutG->SetPoint(14,80.9731,113.296);
   cutG->SetPoint(15,80.5734,113.448);





















   
//###########################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      Double_t tot_charge = 0.;
      if(/*theta_deg>=-5 && theta_deg <=5 &&*/ cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      //tot_charge = cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10];
      charge->Fill(cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10]);
      }                                                                        //TCutg parenthesis
   }
   charge->Fit("f","","",0.,4e6);
   mean_charge = ((f->GetParameter(1))*2*61e3)/(20*3.62);
   primaries = dE/w_gas;
   gain=mean_charge/primaries;
   cout << f->GetParameter(1) << "   " << charge->GetMean() << endl;
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
