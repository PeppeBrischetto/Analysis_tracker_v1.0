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

void resolution(int run){

//#################################################################################################################
// Variables definition

   Int_t nPoint = 0;
   Double_t row0 = 0.;
   Double_t row4 = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   char histoname[100];
   char rettaname[100];
   char titolox[100];
   Double_t x_fit[5] = {0.};
   Int_t evts = 0;
   Double_t r3_2[NRows] = {0.};
   Double_t err_r3_2[NRows] = {0.};
   
   Double_t semiaperture = 0.25;                                              // semiaperture of the virtual slit(s)
   ofstream diffWidth;
   
   TF1 *f_gaus = new TF1("f_gaus","gaus",0,300);
   f_gaus->SetParameters(0,0,0);
   
   TF1 *linea = new TF1("linea","pol1",0,300);
   linea->SetParameters(0,0);
   
   TH1D *x_NC[NRows];                                                        // histogram-array for all events
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"Row %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      x_NC[i]=new TH1D(histoname,histoname,3000.,0.,300);
      x_NC[i]->GetXaxis()->SetTitle(titolox);
      x_NC[i]->GetXaxis()->SetTitleSize(0.05);
      x_NC[i]->GetXaxis()->SetLabelSize(0.05);
      x_NC[i]->GetXaxis()->SetTitleOffset(0.9);
      x_NC[i]->GetYaxis()->SetTitleSize(0.05);
      x_NC[i]->GetYaxis()->SetLabelSize(0.05);
      x_NC[i]->GetYaxis()->SetTitleOffset(0.9);
      x_NC[i]->SetNdivisions(7);
      x_NC[i]->SetLineColor(kGreen+1);
      x_NC[i]->SetLineWidth(2);
      //x_NC[i]->SetFillColor(kBlue-5);
   }
   
   TH1D *x_NC_cut[NRows];                                                    // histogram-array for evts in TCutG
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"Row %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      x_NC_cut[i]=new TH1D(histoname,histoname,3000.,0.,300);
      x_NC_cut[i]->GetXaxis()->SetTitle(titolox);
      x_NC_cut[i]->GetXaxis()->SetTitleSize(0.05);
      x_NC_cut[i]->GetXaxis()->SetLabelSize(0.05);
      x_NC_cut[i]->GetXaxis()->SetTitleOffset(0.9);
      x_NC_cut[i]->GetYaxis()->SetTitleSize(0.05);
      x_NC_cut[i]->GetYaxis()->SetLabelSize(0.05);
      x_NC_cut[i]->GetYaxis()->SetTitleOffset(0.9);
      x_NC_cut[i]->SetNdivisions(7);
      x_NC_cut[i]->SetLineColor(kGreen+3);
      x_NC_cut[i]->SetFillColor(kGreen+1);
      x_NC_cut[i]->SetFillStyle(3001);
      x_NC_cut[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr[NRows];                                                       // histogram-array for evets in TCutG after offset correction
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"Row %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      xCorr[i]=new TH1D(histoname,histoname,3000.,0.,300);
      xCorr[i]->GetXaxis()->SetTitle(titolox);
      xCorr[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr[i]->GetXaxis()->SetTitleOffset(0.9);
      xCorr[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr[i]->GetYaxis()->SetTitleOffset(0.9);
      xCorr[i]->SetNdivisions(7);
      xCorr[i]->SetFillColor(kBlue-5);
      xCorr[i]->SetFillStyle(3001);
      xCorr[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr_slit[NRows];                                                  // histogram-array after xCorr for evts in virtual slit on Row4
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"Row %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      xCorr_slit[i]=new TH1D(histoname,histoname,3000.,0.,300);
      xCorr_slit[i]->GetXaxis()->SetTitle(titolox);
      xCorr_slit[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr_slit[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr_slit[i]->GetXaxis()->SetTitleOffset(0.9);
      xCorr_slit[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr_slit[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr_slit[i]->GetYaxis()->SetTitleOffset(0.9);
      xCorr_slit[i]->SetNdivisions(7);
      xCorr_slit[i]->SetFillColor(kRed);
      xCorr_slit[i]->SetLineColor(kRed);
      xCorr_slit[i]->SetFillStyle(3001);
      xCorr_slit[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr_slitSlit[NRows];                                              // histogram-array after xCorr_slit for evts between virtual slits in Row0 & Row4
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"Row %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      xCorr_slitSlit[i]=new TH1D(histoname,histoname,3000.,0.,300);
      xCorr_slitSlit[i]->GetXaxis()->SetTitle(titolox);
      xCorr_slitSlit[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr_slitSlit[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr_slitSlit[i]->GetXaxis()->SetTitleOffset(0.9);
      xCorr_slitSlit[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr_slitSlit[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr_slitSlit[i]->GetYaxis()->SetTitleOffset(0.9);
      xCorr_slitSlit[i]->SetNdivisions(7);
      xCorr_slitSlit[i]->SetLineColor(kOrange+1);
      xCorr_slitSlit[i]->SetLineWidth(2);
      xCorr_slitSlit[i]->SetFillColor(kYellow);
   }
   
   TH1D *xfit[NRows];                                                        // histogram-array for xfit evaluated in z=z_{0} (z_{0}=18.6 mm)
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xfit %i",i);
      xfit[i]=new TH1D(histoname,histoname,3000.,0.,300);
      xfit[i]->GetXaxis()->SetTitle("x^{fit} (mm)");
      xfit[i]->GetXaxis()->SetTitleSize(0.05);
      xfit[i]->GetXaxis()->SetLabelSize(0.05);
      xfit[i]->GetXaxis()->SetTitleOffset(0.9);
      xfit[i]->GetYaxis()->SetTitleSize(0.05);
      xfit[i]->GetYaxis()->SetLabelSize(0.05);
      xfit[i]->GetYaxis()->SetTitleOffset(0.9);
      xfit[i]->SetNdivisions(7);
      xfit[i]->SetLineWidth(2);
   }
   
   TH1D *xfitW[NRows];                                                       // histogram-array for xfit coming by a weighted fit - never used
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xfit %i",i);
      xfitW[i]=new TH1D(histoname,histoname,3000.,0.,300);
      xfitW[i]->GetXaxis()->SetTitle("x^{fit} (mm)");
      xfitW[i]->GetXaxis()->SetTitleSize(0.05);
      xfitW[i]->GetXaxis()->SetLabelSize(0.05);
      xfitW[i]->GetXaxis()->SetTitleOffset(0.9);
      xfitW[i]->GetYaxis()->SetTitleSize(0.05);
      xfitW[i]->GetYaxis()->SetLabelSize(0.05);
      xfitW[i]->GetYaxis()->SetTitleOffset(0.9);
      xfitW[i]->SetNdivisions(7);
      xfitW[i]->SetLineWidth(2);
   }
   
   TGraphErrors *retta[5];
    for(int i=0; i<5; i++){
       sprintf(rettaname,"retta%i",i);
       retta[i]=new TGraphErrors();
       retta[i]->GetXaxis()->SetTitle("x (mm)");
       retta[i]->GetYaxis()->SetTitle("z (mm)");
       retta[i]->SetMarkerSize(1.6);
       retta[i]->SetMarkerStyle(20);
       retta[i]->SetMarkerColor(i);
       retta[i]->SetLineColor(i);
    }
    
//################################################################################################################
// OpenFiles
   openTrackFileTh(run);
   tree->Print();
   Double_t entries = tree->GetEntries();
   cout << entries << endl << endl;
//###########################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",10);                                                                                                          
  cutG->SetVarX("cl_x_mm[0]");                                                            
  cutG->SetVarY("cl_x_mm[1]");                                                            
  cutG->SetPoint(0,60.327,105.785);                                                       
  cutG->SetPoint(1,58.9946,104.395);                                                      
  cutG->SetPoint(2,58.5505,102.696);                                                      
  cutG->SetPoint(3,58.7947,101.492);                                                      
  cutG->SetPoint(4,59.5609,101.461);                                                      
  cutG->SetPoint(5,61.0154,102.604);                                                      
  cutG->SetPoint(6,63.0139,104.21);                                                       
  cutG->SetPoint(7,63.5802,105.908);                                                      
  cutG->SetPoint(8,62.6364,107.236);                                                      
  cutG->SetPoint(9,60.327,105.785);                                 
   
//#################################################################################################################
// Data loop for all events

   for(Int_t i=0; i<entries; i++){
   
      Double_t pad[NRows][50] = {0.};
      Double_t charge[NRows][50] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t totalCharge[NRows] = {0.};
      Int_t mult = 0;
      
      tree->GetEntry(i);
      
      for(Int_t row = 0; row < NRows; row++) {
         nPoint = row;

         mult = cl_padMult[row];

         // Debug: guarda cosa sta succedendo
         // cout << "Event " << i << " row " << row << " mult = " << mult << endl;

         // Limita la molteplicità agli array disponibili
         if(mult > 50) {
            //cout << "WARNING: cl_padMult[" << row << "] = " << mult << " > 50, taglio a 50" << endl;
            mult = 50;
         }else
            if(mult <= 50) {
              //cout << "WARNING: cl_padMult[" << row << "] = " << mult << " > NStrips(" << NStrips << "), taglio a NStrips" << endl;
              mult = cl_padMult[row];
            }

        for(Int_t p = 0; p < mult; p++) {
           pad[row][p] = pads_fired[row][p];
           charge[row][p] = pad_charge[row][p];
           totalCharge[row] += charge[row][p];
           x_mm[p] = 2.5 + (5 * pad[row][p]);
           x[row] += (x_mm[p] * charge[row][p]);
        }

        if(totalCharge[row] > 0)
          x[row] = x[row]/totalCharge[row];
          x_NC[row]->Fill(x[row]);
          retta[0]->SetPoint(nPoint,x[row],z[row]);
          if(/*theta_deg>=0 && theta_deg<20 &&*/ sic_fired==1 && energySic>2000 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
            x_NC_cut[row]->Fill(x[row]);
            retta[1]->SetPoint(nPoint,x[row],z[row]);
         }
      }

      retta[0]->Fit(linea,"R+");
      xfit[0]->Fill(linea->GetX(18.6));
      linea->SetParameters(0,0);
      /*retta[0]->Fit(linea,"WL");
      xfitW[0]->Fill(linea->GetX(18.6));*/
      linea->SetParameters(0,0);
      retta[1]->Fit(linea,"R+");
      if(/*theta_deg>=0 && theta_deg<20 &&*/ sic_fired==1 && energySic>2000 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
         xfit[1]->Fill(linea->GetX(18.6));
      }
      /*linea->SetParameters(0,0);
      retta[1]->Fit(linea,"WL");
      if(theta_deg>=0 && theta_deg<20 && sic_fired==1 && energySic>2000 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
         xfitW[1]->Fill(linea->GetX(18.6));
      }*/
   }
   //f->Close();
   
   ofstream output;

//#################################################################################################################
// Opening track file without slit-scattering & offset correction
   
   char fileInName[100];
   Double_t x_corr[NRows] = {0.};
   if(run<10){
         sprintf(fileInName,"CorrectedX/xCorr_run00%i_th.root",run);
      }else if(run <100){
         sprintf(fileInName,"CorrectedX/xCorr_run0%i_th.root",run);
      }else{
         sprintf(fileInName,"CorrectedX/xCorr_run%i_th.root",run);
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
       mytree->GetEntry(i);
       for(Int_t row=0; row<NRows; row++){
          nPoint = row;
          xCorr[row]->Fill(x_corr[row]);
          retta[2]->SetPoint(nPoint,x_corr[row],z[row]);
       }
       linea->SetParameters(0,0);
       retta[2]->Fit(linea,"R+");
       xfit[2]->Fill(linea->GetX(18.6));
       /*linea->SetParameters(0,0);
       retta[2]->Fit(linea,"WL");
       xfitW[2]->Fill(linea->GetX(18.6));*/
   }

//#################################################################################################################
// Output preparation: plots (to save after r.o.i.-selection) & .txt (automatically saved) files, respectively 
   
   char fileOut[100];
   sprintf(fileOut,"resolution/resol_run_%d_cut_th_%.0fmm_slit.txt",run,2*semiaperture);
   output.open(fileOut);
   
   output << "             Preliminary results All evts" << endl << endl;
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   
   c->Divide(3,2);
   
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      f_gaus->SetParameters(0,0,0);
      x_NC[row]->Fit(f_gaus,"Q0","",0,300);
      Double_t stDEVA = f_gaus->GetParameter(2);
      Double_t stDEVA_err = f_gaus->GetParError(2);
      Double_t y = x_NC[row]->GetMaximum();
      char testo[100];
      sprintf(testo,"FWHM: 2.35*%f = %f",stDEVA,2.35*stDEVA);
      x_NC[row]->Draw();
      output << "stDEV_" << row << ": " << stDEVA << "   FWHM: 2.35*stDEV = "<< 2.35*stDEVA << "   error: " << 2.35*stDEVA_err << "   StdDEV(): " << x_NC[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(x_NC[row]->GetStdDev())<< "   error: " << 2.35*(x_NC[row]->GetStdDevError()) << endl;
   }
   
   output << endl << "             Preliminary results CutG" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      f_gaus->SetParameters(0,0,0);
      x_NC_cut[row]->Fit(f_gaus,"Q0","",0,300);
      Double_t stDEVB = f_gaus->GetParameter(2);
      Double_t stDEVB_err = f_gaus->GetParError(2);
      Double_t y = x_NC_cut[row]->GetMaximum();
      char testo[100];
      sprintf(testo,"FWHM: 2.35*%f = %f",stDEVB,2.35*stDEVB);
      x_NC_cut[row]->Draw("SAME");
      output << "stDEV_" << row << ": " << stDEVB << "   FWHM: 2.35*stDEV = "<< 2.35*stDEVB << "   error: " <<  2.35*stDEVB_err << "   StdDEV(): " << x_NC_cut[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(x_NC_cut[row]->GetStdDev())<< "   error: " << 2.35*(x_NC_cut[row]->GetStdDevError()) << endl;
   }
   
   output << endl << "             Preliminary results Offset correction" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      f_gaus->SetParameters(0,0,0);
      xCorr[row]->Fit(f_gaus,"Q0","",0,300);
      Double_t stDEV = f_gaus->GetParameter(2);
      Double_t stDEV_err = f_gaus->GetParError(2);
      Double_t y = xCorr[row]->GetMaximum();
      char testo[100];
      sprintf(testo,"FWHM: 2.35*%f = %f",stDEV,2.35*stDEV);
      xCorr[row]->Draw("SAME");
      output << "stDEV_" << row << ": " << stDEV << "   FWHM: 2.35*stDEV = "<< 2.35*stDEV << "   error: " << 2.35*stDEV << "   StdDEV(): " << xCorr[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr[row]->GetStdDev()) << "   error: " << 2.35*(xCorr[row]->GetStdDevError())<< endl;
   }
   
   row0 = xCorr[0]->GetMean();
   row4 = xCorr[4]->GetMean();
   cout << "Row0: " << row0 << "     Row4: "<< row4 << endl;
   for(Int_t i=0; i<evts; i++){
      mytree->GetEntry(i);
      if(x_corr[4]>=row4-semiaperture && x_corr[4]<=row4+semiaperture){                                                                                  // single-slit in row4
      for(Int_t row=0; row<NRows; row++){
         nPoint = row;
         xCorr_slit[row]->Fill(x_corr[row]);
         if(row!=4){
           retta[3]->SetPoint(nPoint,x_corr[row],z[row]);
         }
      }
      linea->SetParameters(0,0);
      retta[3]->Fit(linea,"R+");
      xfit[3]->Fill(linea->GetX(18.6));
      /*linea->SetParameters(0,0);
      retta[3]->Fit(linea,"WL");
      xfitW[3]->Fill(linea->GetX(18.6));*/
      }
      
   }
   
   output << endl << "       Preliminary results - slit on ROW4" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      f_gaus->SetParameters(0,0,0);
      xCorr_slit[row]->Fit(f_gaus,"Q0","",0,299);
      Double_t stDEV_slit = f_gaus->GetParameter(2);
      Double_t stDEV_slit_err = f_gaus->GetParError(2);
      output << "stDEV_slit_" << row << ": " << stDEV_slit << "   FWHM: 2.35*stDEV_slit = "<< 2.35*stDEV_slit << "   error: " << 2.35*stDEV_slit_err << "   StdDEV(): " << xCorr_slit[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr_slit[row]->GetStdDev()) << "   error: "<< 2.35*(xCorr_slit[row]->GetStdDevError()) << endl;
   }
   
   for(Int_t i=0; i<evts; i++){
      mytree->GetEntry(i);
      if(x_corr[0]>=row0-semiaperture && x_corr[0]<=row0+semiaperture && x_corr[4]>=row4-semiaperture && x_corr[4]<=row4+semiaperture){                                    // double-slit row0 & row4
      for(Int_t row=0; row<NRows; row++){
         nPoint = row;
         xCorr_slitSlit[row]->Fill(x_corr[row]);
         if(row!=4 || row!=0){
           retta[4]->SetPoint(nPoint,x_corr[row],z[row]);
         }
      }
      linea->SetParameters(0,0);
      retta[4]->Fit(linea,"R+");
      xfit[4]->Fill(linea->GetX(18.6));
      /*linea->SetParameters(0,0);
      retta[4]->Fit(linea,"WL");
      xfitW[4]->Fill(linea->GetX(18.6));*/
      }
   }
   
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(x_NC[0], "All events", "f");
   l->AddEntry(x_NC_cut[0], "TCutg appl.", "f");
   l->AddEntry(xCorr[0], "TCutg & offset-corr.", "f");
   l->AddEntry(xCorr_slit[0], "Slit ROW4", "f");
   l->AddEntry(xCorr_slitSlit[0], "2-slit ROWs 0-4", "f");
   
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->SetLineWidth(0);
   l1->AddEntry(xfit[0], "All events", "f");
   l1->AddEntry(xfit[1], "TCutg appl.", "f");
   l1->AddEntry(xfit[2], "TCutg & offset-corr.", "f");
   l1->AddEntry(xfit[3], "Slit ROW4", "f");
   l1->AddEntry(xfit[4], "2-slit ROWs 0-4", "f");
      
   output << endl << "   Preliminary results - slit on ROW0 & ROW4" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      f_gaus->SetParameters(0,0,0);
      xCorr_slitSlit[row]->Fit(f_gaus,"Q0","",0,299);
      Double_t stDEV_slitSlit = f_gaus->GetParameter(2);
      Double_t stDEV_slitSlit_err = f_gaus->GetParError(2);
      c->cd(row+1);
      xCorr_slit[row]->Draw("SAME");
      xCorr_slitSlit[row]->Draw("SAME");
      l->Draw("SAME");
      output << "stDEV_slitSlit_" << row << ": " << stDEV_slitSlit << "   FWHM: 2.35*stDEV_slitSlit = "<< 2.35*stDEV_slitSlit << "   error: " << 2.35*stDEV_slitSlit_err << "   StdDEV(): " << xCorr_slitSlit[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr_slitSlit[row]->GetStdDev()) << "   error: " << 2.35*(xCorr_slitSlit[row]->GetStdDevError()) << endl;
      if(row==4){
         c->cd(row+2);
         xfit[0]->SetLineColor(kGreen+1);
         xfit[0]->Draw();
         xfit[1]->SetLineColor(kGreen+3);
         xfit[1]->SetFillColor(kGreen+1);
         xfit[1]->SetFillStyle(3001);
         xfit[1]->Draw("SAME");
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
      f_gaus->SetParameters(0,0,0);
      xfit[i]->Fit(f_gaus,"Q0","",0,299);
      output << "stDEV_xfit:" << f_gaus->GetParameter(2) << "   FWHM: 2.35*stDEV_xfit = "<< 2.35*f_gaus->GetParameter(2) << "   error: " << 2.35*f_gaus->GetParError(2) << "   StdDEV(): " << xfit[i]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xfit[i]->GetStdDev()) << "   error: " << 2.35*(xfit[i]->GetStdDevError()) << endl;
   }
   
}




