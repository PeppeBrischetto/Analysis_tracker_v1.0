//#################################################################################################################
//#   This macro allows to evaluate the error on the x_{i} measured on the padPlane.
//#################################################################################################################
//#   Created September 2025 by A. Pitronaci 
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
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void resolution_v2(int run){

   Int_t nPoint = 0;
   Double_t row0 = 0.;
   Double_t row4 = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   char histoname[100];
   Double_t x_fit[5] = {0.};
   Int_t evts = 0;
   char titoloF[100];
   
   /*TF1 *f_gaus = new TF1("f_gaus","gaus",0,300);
   f_gaus->SetParameters(0,0,0);
   TF1 *f_gaus[4];
   for(Int_t i=0; i<4; i++){
      sprintf(titoloF,"f_gaus%i",i);
      f_gaus[i] = new TF1("","gaus",0,300);
      f_gaus[i]->SetParameters(0,0,0);
   }*/
   
   TF1 *linea0 = new TF1("linea0","pol1",0,300);
   linea0->SetParameters(0,0);
   TF1 *linea1 = new TF1("linea1","pol1",0,300);
   linea1->SetParameters(0,0);
   TF1 *linea2 = new TF1("linea2","pol1",0,300);
   linea2->SetParameters(0,0);
   TF1 *linea3 = new TF1("linea3","pol1",0,300);
   linea3->SetParameters(0,0);
   TF1 *linea4 = new TF1("linea4","pol1",0,300);
   linea4->SetParameters(0,0);
   
   TF1 *lineaw0 = new TF1("lineaw0","pol1",0,300);
   lineaw0->SetParameters(0,0);
   TF1 *lineaw1 = new TF1("lineaw1","pol1",0,300);
   lineaw1->SetParameters(0,0);
   TF1 *lineaw2 = new TF1("lineaw2","pol1",0,300);
   lineaw2->SetParameters(0,0);
   TF1 *lineaw3 = new TF1("lineaw3","pol1",0,300);
   lineaw3->SetParameters(0,0);
   TF1 *lineaw4 = new TF1("lineaw4","pol1",0,300);
   lineaw4->SetParameters(0,0);
   
   
   TH1D *xCorr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xCorr %i",i);
      xCorr[i]=new TH1D("","",3000.,0.,300);
      xCorr[i]->GetXaxis()->SetTitle("x_{i}^{corr} (mm)");
      xCorr[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr[i]->GetXaxis()->SetTitleOffset(1.);
      xCorr[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr[i]->GetYaxis()->SetTitleOffset(1.);
      xCorr[i]->SetNdivisions(7);
      xCorr[i]->SetFillColor(kBlue-5);
      xCorr[i]->SetFillStyle(3001);
      xCorr[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr_slit[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xCorr_slit %i",i);
      xCorr_slit[i]=new TH1D("","",3000.,0.,300);
      xCorr_slit[i]->GetXaxis()->SetTitle("x_{i}^{corr} (mm)");
      xCorr_slit[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr_slit[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr_slit[i]->GetXaxis()->SetTitleOffset(1.);
      xCorr_slit[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr_slit[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr_slit[i]->GetYaxis()->SetTitleOffset(1.);
      xCorr_slit[i]->SetNdivisions(7);
      xCorr_slit[i]->SetFillColor(kRed);
      xCorr_slit[i]->SetLineColor(kRed);
      xCorr_slit[i]->SetFillStyle(3001);
      xCorr_slit[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr_slitSlit[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xCorr_slitSlit %i",i);
      xCorr_slitSlit[i]=new TH1D("","",3000.,0.,300);
      xCorr_slitSlit[i]->GetXaxis()->SetTitle("x_{i}^{corr} (mm)");
      xCorr_slitSlit[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr_slitSlit[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr_slitSlit[i]->GetXaxis()->SetTitleOffset(1.);
      xCorr_slitSlit[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr_slitSlit[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr_slitSlit[i]->GetYaxis()->SetTitleOffset(1.);
      xCorr_slitSlit[i]->SetNdivisions(7);
      xCorr_slitSlit[i]->SetLineColor(kOrange+1);
      xCorr_slitSlit[i]->SetLineWidth(2);
      xCorr_slitSlit[i]->SetFillColor(kYellow);
   }
   
   TH1D *xfit[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xfit %i",i);
      xfit[i]=new TH1D("","",3000.,0.,300);
      xfit[i]->GetXaxis()->SetTitle("x^{fit} (mm)");
      xfit[i]->GetXaxis()->SetTitleSize(0.05);
      xfit[i]->GetXaxis()->SetLabelSize(0.05);
      xfit[i]->GetXaxis()->SetTitleOffset(1.);
      xfit[i]->GetYaxis()->SetTitleSize(0.05);
      xfit[i]->GetYaxis()->SetLabelSize(0.05);
      xfit[i]->GetYaxis()->SetTitleOffset(1.);
      xfit[i]->SetNdivisions(7);
      xfit[i]->SetLineWidth(2);
   }
   
   TH1D *xfitW[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xfit %i",i);
      xfitW[i]=new TH1D("","",3000.,0.,300);
      xfitW[i]->GetXaxis()->SetTitle("x^{fit} (mm)");
      xfitW[i]->GetXaxis()->SetTitleSize(0.05);
      xfitW[i]->GetXaxis()->SetLabelSize(0.05);
      xfitW[i]->GetXaxis()->SetTitleOffset(1.);
      xfitW[i]->GetYaxis()->SetTitleSize(0.05);
      xfitW[i]->GetYaxis()->SetLabelSize(0.05);
      xfitW[i]->GetYaxis()->SetTitleOffset(1.);
      xfitW[i]->SetNdivisions(7);
      xfitW[i]->SetLineWidth(2);
   }
   
   TGraphErrors *retta[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       retta[i]=new TGraphErrors();
       retta[i]->GetXaxis()->SetTitle("x (mm)");
       retta[i]->GetYaxis()->SetTitle("z (mm)");
       retta[i]->SetMarkerSize(1.6);
       retta[i]->SetMarkerStyle(20);
       retta[i]->SetMarkerColor(i);
       retta[i]->SetLineColor(i);
    }
   
   ofstream output;
   
   char fileInName[100];
   Double_t x_corr[NRows] = {0.};
   if(run<10){
         sprintf(fileInName,"CorrectedX/xCorr_run00%i.root",run);
      }else if(run <100){
         sprintf(fileInName,"CorrectedX/xCorr_run0%i.root",run);
      }else{
         sprintf(fileInName,"CorrectedX/xCorr_run%i.root",run);
      } 
      
   TFile *f_out = new TFile(fileInName,"READ");
   TTree *mytree;
   
   mytree = (TTree*)f_out->Get("Data_cor");
   mytree->SetBranchAddress("x_corr0", &x_corr[0]);
   mytree->SetBranchAddress("x_corr1", &x_corr[1]);
   mytree->SetBranchAddress("x_corr2", &x_corr[2]);
   mytree->SetBranchAddress("x_corr3", &x_corr[3]);
   mytree->SetBranchAddress("x_corr4", &x_corr[4]);
   
   evts = mytree->GetEntries();
   cout << "Evts: " << evts << endl;
   
   for(Int_t i=0; i<evts; i++){
       linea2->SetParameters(0,0);
       lineaw2->SetParameters(0,0);
      
       mytree->GetEntry(i);
       for(Int_t row=0; row<NRows; row++){
          nPoint = row;
          xCorr[row]->Fill(x_corr[row]);
          retta[2]->SetPoint(nPoint,x_corr[row],z[row]);
       }
       retta[2]->Fit(linea2,"R+");
       xfit[2]->Fill(linea2->GetX(18.6));
       retta[2]->Fit(lineaw2,"WL");
       xfitW[2]->Fill(lineaw2->GetX(18.6));
   }
   
   row0 = xCorr[0]->GetMean();
   row4 = xCorr[4]->GetMean();
   
   char fileOut[100];
   sprintf(fileOut,"resol_run_%dNEWcut.txt",run);
   output.open(fileOut);
   
   output << "             Preliminary results" << endl << endl;
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   
   for(Int_t i=0; i<evts; i++){
      linea3->SetParameters(0,0);
      lineaw3->SetParameters(0,0);
      mytree->GetEntry(i);
      if(x_corr[4]>=row4-0.2 && x_corr[4]<=row4+0.2){
      for(Int_t row=0; row<NRows; row++){
         nPoint = row;
         xCorr_slit[row]->Fill(x_corr[row]);
         retta[3]->SetPoint(nPoint,x_corr[row],z[row]);
      }
      retta[3]->Fit(linea3,"R+");
      xfit[3]->Fill(linea3->GetX(18.6));
      retta[3]->Fit(lineaw3,"WL");
      xfitW[3]->Fill(lineaw3->GetX(18.6));
      }
   }
   
   output << endl << "       Preliminary results - slit on ROW4" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      /*f_gaus->SetParameters(0,0,0);
      xCorr_slit[row]->Fit(f_gaus[1],"N","N",0,299);
      Double_t stDEV_slit = f_gaus[1]->GetParameter(2);*/
      output /*<< "stDEV_slit_" << row << ": " << stDEV_slit << "   FWHM: 2.35*stDEV_slit = "<< 2.35*stDEV_slit*/ << "   StdDEV(): " << xCorr_slit[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr_slit[row]->GetStdDev())<< "   FWHM_error: " << 2.35*(xCorr_slit[row]->GetStdDevError()) << endl;
   }
   
   for(Int_t i=0; i<evts; i++){
      linea4->SetParameters(0,0);
      lineaw4->SetParameters(0,0);
      
      mytree->GetEntry(i);
      if(x_corr[0]>=row0-0.2 && x_corr[0]<=row0+0.2 && x_corr[4]>=row4-0.2 && x_corr[4]<=row4+0.2){
      for(Int_t row=0; row<NRows; row++){
         nPoint = row;
         xCorr_slitSlit[row]->Fill(x_corr[row]);
         retta[4]->SetPoint(nPoint,x_corr[row],z[row]);
      }
      retta[4]->Fit(linea4,"R+");
      xfit[4]->Fill(linea4->GetX(18.6));
      retta[4]->Fit(lineaw4,"WL");
      xfitW[4]->Fill(lineaw4->GetX(18.6));
      }
   }
   
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(xCorr[0], "TCutg & offset-corr.", "f");
   l->AddEntry(xCorr_slit[0], "Slit ROW4", "f");
   l->AddEntry(xCorr_slitSlit[0], "2-slit ROWs 0-4", "f");
   
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->SetLineWidth(0);
   l1->AddEntry(xfit[2], "TCutg & offset-corr.", "f");
   l1->AddEntry(xfit[3], "Slit ROW4", "f");
   l1->AddEntry(xfit[4], "2-slit ROWs 0-4", "f");
      
   output << endl << "   Preliminary results - slit on ROW0 & ROW4" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      /*f_gaus->SetParameters(0,0,0);
      xCorr_slitSlit[row]->Fit(f_gaus[2],"N","N",0,299);
      Double_t stDEV_slitSlit = f_gaus[2]->GetParameter(2);*/
      c->cd(row+1);
      xCorr[row]->Draw();
      xCorr_slit[row]->Draw("SAME");
      xCorr_slitSlit[row]->Draw("SAME");
      l->Draw("SAME");
      output /*<< "stDEV_slitSlit_" << row << ": " << stDEV_slitSlit << "   FWHM: 2.35*stDEV_slitSlit = "<< 2.35*stDEV_slitSlit*/ << "   StdDEV(): " << xCorr_slitSlit[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr_slitSlit[row]->GetStdDev()) << "   FWHM_error: " << 2.35*(xCorr_slit[row]->GetStdDevError()) << endl;
      if(row==4){
         c->cd(row+2);
         /*xfit[0]->SetLineColor(kGreen+1);
         xfit[0]->Draw();
         xfit[1]->SetLineColor(kGreen+3);
         xfit[1]->SetFillColor(kGreen+1);
         xfit[1]->SetFillStyle(3001);
         xfit[1]->Draw("SAME");*/
         xfit[2]->SetFillColor(kBlue-5);
         xfit[2]->SetFillStyle(3001);
         xfit[2]->Draw("SAME");
         xfit[3]->SetFillColor(kRed);
         xfit[3]->SetLineColor(kRed);
         xfit[3]->SetFillStyle(3001);
         xfit[3]->Draw("SAME");
         xfit[4]->SetLineColor(kOrange+1);
         xfit[4]->SetLineWidth(2);
         xfit[4]->SetFillColor(kYellow);
         xfit[4]->Draw("SAME");
         l1->Draw("SAME");
      }
   }
   
   output << endl << "   Preliminary results - xf_fit improvement" << endl << endl;
   for(Int_t i=0; i<NRows; i++){
      /*f_gaus->SetParameters(0,0,0);
      xfit[i]->Fit(f_gaus[3],"N","N",0,299);*/
      output /*<< "stDEV_xfit:" << f_gaus[3]->GetParameter(2) << "   FWHM: 2.35*stDEV_xfit = "<< 2.35*f_gaus[3]->GetParameter(2)*/ << "   StdDEV(): " << xfit[i]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xfit[i]->GetStdDev()) << "   FWHM_error: " << 2.35*(xCorr_slit[i]->GetStdDevError())<< endl;
   }
   
   /*TCanvas *cNC = new TCanvas("cNC","cNC",2200,300);
   cNC->Divide(2,1);
   for(Int_t row=0; row<NRows; row++){
      cNC->cd(1);
      xfit[row]->Draw();
      cNC->cd(2);
      xfitW[row]->Draw();
   }*/
}




