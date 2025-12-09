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
   
   TH1D *charge = new TH1D("charge","",5000,0,1e7);
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
   
   TFile *f_taglio = new TFile("f_taglio");
   char titolo_file[100];
   char titolo_cut[100];
   
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

   /*sprintf(titolo_file,"../GCut/cut_12c_%d.root",run);
   f_taglio->Open(titolo_file);
   TCutG *cutG = (TCutG*)f_taglio->Get("cutG_new");
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");*/
   
   TCutG *cutG = new TCutG("cutG",9);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,48.204,101.365);
   cutG->SetPoint(1,46.0368,99.2029);
   cutG->SetPoint(2,44.5785,95.4529);
   cutG->SetPoint(3,44.7202,92.7176);
   cutG->SetPoint(4,49.2572,95.2765);
   cutG->SetPoint(5,50.756,97.8353);
   cutG->SetPoint(6,50.7965,100.438);
   cutG->SetPoint(7,50.0471,101.365);
   cutG->SetPoint(8,48.204,101.365);
/*   cutG->SetPoint(9,64.6483,107.479);
   cutG->SetPoint(10,63.8178,107.41);
   cutG->SetPoint(11,62.0058,106.028);
   cutG->SetPoint(12,46.0816,99.3924);
   cutG->SetPoint(13,46.5123,96.646);
   cutG->SetPoint(14,47.4303,96.7187);
   cutG->SetPoint(15,48.0639,97.0585);
   cutG->SetPoint(16,48.4388,97.2162);
   cutG->SetPoint(17,49.0207,97.4952);
   cutG->SetPoint(18,49.2146,97.8956);
   cutG->SetPoint(19,48.9431,98.5265);
   cutG->SetPoint(20,48.2578,98.3809);
   cutG->SetPoint(21,47.8699,98.4415);
   cutG->SetPoint(22,47.7277,98.8419);
   cutG->SetPoint(23,48.1932,99.1452);
   cutG->SetPoint(24,49.3956,99.3272);
   cutG->SetPoint(25,49.5767,99.946);
   cutG->SetPoint(26,49.3827,100.346);
   cutG->SetPoint(27,48.9172,100.443);
   cutG->SetPoint(28,48.0509,100.371);
   cutG->SetPoint(29,47.857,100.419);
   cutG->SetPoint(30,47.857,100.419);
   cutG->SetPoint(31,46.8614,100.528);*/






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
   charge->Fit("f","","",0,4e6);
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
