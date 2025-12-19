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

const TString InputFile = "resolution/FWHM_3_2_diff_0mm_slit.txt";

void fwhmDiff_vthgem(){

// Needed variables
   Double_t vthgem = 0.;
   Double_t vthgem52 = 0.;
   Double_t vthgem65 = 0.;
   Double_t vthgem76 = 0.;
   Double_t diff[5] = {0.};
   Double_t err_diff[5] = {0.};
   Double_t diff52[5] = {0.};
   Double_t err_diff52[5] = {0.};
   Double_t diff65[5] = {0.};
   Double_t err_diff65[5] = {0.};
   Double_t diff76[5] = {0.};
   Double_t err_diff76[5] = {0.};
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
   
   TGraphErrors *gr_52[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      gr_52[i] = new TGraphErrors();
      gr_52[i]->GetXaxis()->SetTitle("V_{THGEM} (V)");
      gr_52[i]->GetYaxis()->SetTitle("FWHM_{3} - FWHM_{2} (mm)");
      gr_52[i]->GetXaxis()->SetTitleSize(0.05);
      gr_52[i]->GetXaxis()->SetLabelSize(0.05);
      gr_52[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_52[i]->GetYaxis()->SetTitleSize(0.05);
      gr_52[i]->GetYaxis()->SetLabelSize(0.05);
      gr_52[i]->GetYaxis()->SetTitleOffset(1.0);
      gr_52[i]->SetLineColor(kRed);
      gr_52[i]->SetMarkerStyle(20);
      gr_52[i]->SetMarkerColor(kRed);
   }
   
   TGraphErrors *gr_65[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      gr_65[i] = new TGraphErrors();
      gr_65[i]->GetXaxis()->SetTitle("V_{THGEM} (V)");
      gr_65[i]->GetYaxis()->SetTitle("FWHM_{3} - FWHM_{2} (mm)");
      gr_65[i]->GetXaxis()->SetTitleSize(0.05);
      gr_65[i]->GetXaxis()->SetLabelSize(0.05);
      gr_65[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_65[i]->GetYaxis()->SetTitleSize(0.05);
      gr_65[i]->GetYaxis()->SetLabelSize(0.05);
      gr_65[i]->GetYaxis()->SetTitleOffset(1.0);
      gr_65[i]->SetLineColor(kGreen+2);
      gr_65[i]->SetMarkerStyle(20);
      gr_65[i]->SetMarkerColor(kGreen+2);
   }
   
   TGraphErrors *gr_76[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      gr_76[i] = new TGraphErrors();
      gr_76[i]->GetXaxis()->SetTitle("V_{THGEM} (V)");
      gr_76[i]->GetYaxis()->SetTitle("FWHM_{3} - FWHM_{2} (mm)");
      gr_76[i]->GetXaxis()->SetTitleSize(0.05);
      gr_76[i]->GetXaxis()->SetLabelSize(0.05);
      gr_76[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_76[i]->GetYaxis()->SetTitleSize(0.05);
      gr_76[i]->GetYaxis()->SetLabelSize(0.05);
      gr_76[i]->GetYaxis()->SetTitleOffset(1.0);
      gr_76[i]->SetLineColor(kOrange+1);
      gr_76[i]->SetMarkerStyle(20);
      gr_76[i]->SetMarkerColor(kOrange+1);
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
   for(Int_t j=0; j<3; j++){
      getline(input,dummy);
      //cout << dummy << endl;
   }
   
   input >> vthgem >> diff52[0] >> diff52[1] >> diff52[2] >> diff52[3] >> diff52[4];
   //cout << vthgem << "   " << diff52[0] << "   " << diff52[1] << "   " << diff52[2] << "   " << diff52[3] << "   " << diff52[4] << endl;
   input >> dummy >> err_diff52[0] >> err_diff52[1] >> err_diff52[2] >> err_diff52[3] >> err_diff52[4];
   for(Int_t n=0; n<5; n++){
      gr_52[n]->SetPoint(0,vthgem,Abs(diff52[n]));
      gr_52[n]->SetPointError(0,0,Abs(err_diff52[n]));
   }
   
   for(Int_t j=0; j<3; j++){
      getline(input,dummy);
      //cout << dummy << endl;
   }
   
   input >> vthgem65 >> diff65[0] >> diff65[1] >> diff65[2] >> diff65[3] >> diff65[4];
   //cout << vthgem65 << "   " << diff65[0] << "   " << diff65[1] << "   " << diff65[2] << "   " << diff65[3] << "   " << diff65[4] << endl;
   input >> dummy >> err_diff65[0] >> err_diff65[1] >> err_diff65[2] >> err_diff65[3] >> err_diff65[4];
   for(Int_t n=0; n<5; n++){
      gr_65[n]->SetPoint(0,vthgem65,Abs(diff65[n]));
      gr_65[n]->SetPointError(0,0,Abs(err_diff65[n]));
   }
   
   for(Int_t j=0; j<3; j++){
      getline(input,dummy);
      //cout << dummy << endl;
   }
   
   input >> vthgem76 >> diff76[0] >> diff76[1] >> diff76[2] >> diff76[3] >> diff76[4];
   //cout << vthgem76 << "   " << diff76[0] << "   " << diff76[1] << "   " << diff76[2] << "   " << diff76[3] << "   " << diff76[4] << endl;
   input >> dummy >> err_diff76[0] >> err_diff76[1] >> err_diff76[2] >> err_diff76[3] >> err_diff76[4];
   for(Int_t n=0; n<5; n++){
      gr_76[n]->SetPoint(0,vthgem76,Abs(diff76[n]));
      gr_76[n]->SetPointError(0,0,Abs(err_diff76[n]));
   }
      
// Visualization block
   TLegend* l = new TLegend(0.1,0.77,0.9,0.9);
   l->SetNColumns(2);
   l->SetTextSize(0.04);
   l->SetLineWidth(1);
   l->AddEntry(gr[0], "^{12}C 250 Hz", "lp");
   l->AddEntry(gr_52[0], "^{12}C 20 Hz", "lp");
   l->AddEntry(gr_65[0], "^{12}C 1000 Hz", "lp");
   l->AddEntry(gr_76[0], "^{12}C 3000 Hz", "lp");
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t canva=0; canva<5; canva++){
      c->cd(canva+1);
      gr[canva]->Draw();
      gr_52[canva]->Draw("LP SAME");
      gr_65[canva]->Draw("LP SAME");
      gr_76[canva]->Draw("LP SAME");
      l->Draw("SAME");
   }
}
