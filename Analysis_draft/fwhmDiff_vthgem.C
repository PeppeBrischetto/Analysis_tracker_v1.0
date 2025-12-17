//#################################################################################################################
//#   This macro allows to evaluate the resolution on the x_{i} measured on the padPlane opening virtual slists on
//#   the sensing rows of the gas tracker prorotype.
//#################################################################################################################
//#   Created December 2025 by A. Pitronaci 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TLatex.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfilesthresh.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

const TString InputFile = "resolution/FWHM_3_2_diff_2mm_slit.txt";

void fwhmDiff_vthgem(){

// Needed variables
   Double_t vthgem = 0.;
   Double_t diff[5] = {0.};
   Double_t err_diff[5] = {0.};
   string dummy;
   char graphname[100];
   ifstream input;
   
   TGraphErrors *gr[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      gr[i] = new TGraphErrors();
      gr[i]->GetXaxis()->SetTitle("V_{THGEM} (V)");
      gr[i]->GetYaxis()->SetTitle("FWHM_{3} - FWHM_{2} (mm)");
      gr[i]->GetXaxis()->SetTitleSize(0.05);
      gr[i]->GetXaxis()->SetLabelSize(0.05);
      gr[i]->GetXaxis()->SetTitleOffset(0.9);
      gr[i]->GetYaxis()->SetTitleSize(0.05);
      gr[i]->GetYaxis()->SetLabelSize(0.05);
      gr[i]->GetYaxis()->SetTitleOffset(1.0);
      gr[i]->SetLineColor(kBlue);
      gr[i]->SetMarkerStyle(20);
      gr[i]->SetMarkerColor(kBlue);
   }

//#################################################################################################################
// Open file
   input.open(InputFile);
   
   if(!input.is_open()){
     cout << "Opening output file failed!" << endl;
   }
   for(Int_t i=0; i<2; i++){
      getline(input,dummy);
      cout << dummy << endl;
   }
   for(Int_t r=0; r<3; r++){
   
   input >> vthgem >> diff[0] >> diff[1] >> diff[2] >> diff[3] >> diff[4];
   //cout << vthgem << "   " << diff[0] << "   " << diff[1] << "   " << diff[2] << "   " << diff[3] << "   " << diff[4] << endl;
   input >> dummy >> err_diff[0] >> err_diff[1] >> err_diff[2] >> err_diff[3] >> err_diff[4];
   //cout << dummy << "   " << err_diff[0] << "   " << err_diff[1] << "   " << err_diff[2] << "   " << err_diff[3] << "   " << err_diff[4] << endl;
   for(Int_t n=0; n<5; n++){
      gr[n]->SetPoint(r,vthgem,Abs(diff[n]));
      gr[n]->SetPointError(r,0,Abs(err_diff[n]));
   }
   
   }

//#################################################################################################################
// Visualization block
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t canva=0; canva<5; canva++){
      c->cd(canva+1);
      gr[canva]->Draw();
   }
}
