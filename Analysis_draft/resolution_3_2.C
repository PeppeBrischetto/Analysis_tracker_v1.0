//#################################################################################################################
//#   This macro allows to evaluate the resolution on the x_{i} measured on the padPlane opening virtual slists on
//#   the sensing rows of the gas tracker prorotype. Here the resolution is evaluated as the difference between x_3
//#   and x_2, thus determining the FWHM of the corresponding histogram.
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

void resolution_3_2(int run){
   Int_t nPoint = 0;
   Double_t row0 = 0.;
   Double_t row4 = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   char histoname[100];
   char titolox[100];
   Double_t x_fit[5] = {0.};
   Int_t evts = 0;
   Double_t r3_2[NRows] = {0.};
   Double_t err_r3_2[NRows] = {0.};
   
   Double_t semiaperture = 0.15;                                                                  // semiaperture of the virtual slit(s)
   
   TF1 *f_gaus = new TF1("f_gaus","gaus",0,300);
   f_gaus->SetParameters(0,0,0);
   
   TF1 *linea = new TF1("linea","pol1",0,300);
   linea->SetParameters(0,0);
   
   sprintf(titolox,"|x_{3} - x_{2}| (mm)");
   TH1D *x_NC = new TH1D("x_NC","x_NC",3000.,0.,300);
   x_NC->GetXaxis()->SetTitle(titolox);
   x_NC->GetXaxis()->SetTitleSize(0.05);
   x_NC->GetXaxis()->SetLabelSize(0.05);
   x_NC->GetXaxis()->SetTitleOffset(0.9);
   x_NC->GetYaxis()->SetTitleSize(0.05);
   x_NC->GetYaxis()->SetLabelSize(0.05);
   x_NC->GetYaxis()->SetTitleOffset(0.9);
   x_NC->SetNdivisions(7);
   x_NC->SetLineColor(kGreen+1);
   x_NC->SetLineWidth(2);
   //x_NC[i]->SetFillColor(kBlue-5);
   
   TH1D *x_NC_cut = new TH1D("x_NC_cut","x_NC_cut",3000.,0.,300);
   x_NC_cut->GetXaxis()->SetTitle(titolox);
   x_NC_cut->GetXaxis()->SetTitleSize(0.05);
   x_NC_cut->GetXaxis()->SetLabelSize(0.05);
   x_NC_cut->GetXaxis()->SetTitleOffset(0.9);
   x_NC_cut->GetYaxis()->SetTitleSize(0.05);
   x_NC_cut->GetYaxis()->SetLabelSize(0.05);
   x_NC_cut->GetYaxis()->SetTitleOffset(0.9);
   x_NC_cut->SetNdivisions(7);
   x_NC_cut->SetLineColor(kGreen+3);
   x_NC_cut->SetFillColor(kGreen+1);
   x_NC_cut->SetFillStyle(3001);
   x_NC_cut->SetLineWidth(2);
   
   TH1D *x_c[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"x_c %i",i);
      sprintf(titolox,"x_{%d} (mm)",i);
      x_c[i]=new TH1D(histoname,histoname,3000.,0.,300);
      x_c[i]->GetXaxis()->SetTitle(titolox);
      x_c[i]->GetXaxis()->SetTitleSize(0.05);
      x_c[i]->GetXaxis()->SetLabelSize(0.05);
      x_c[i]->GetXaxis()->SetTitleOffset(0.9);
      x_c[i]->GetYaxis()->SetTitleSize(0.05);
      x_c[i]->GetYaxis()->SetLabelSize(0.05);
      x_c[i]->GetYaxis()->SetTitleOffset(0.9);
      x_c[i]->SetNdivisions(7);
      x_c[i]->SetFillColor(kBlue-5);
      x_c[i]->SetFillStyle(3001);
      x_c[i]->SetLineWidth(2);
   }
   
   TH1D *xCorr = new TH1D("xCorr","xCorr",3000.,0.,300);
   xCorr->GetXaxis()->SetTitle(titolox);
   xCorr->GetXaxis()->SetTitleSize(0.05);
   xCorr->GetXaxis()->SetLabelSize(0.05);
   xCorr->GetXaxis()->SetTitleOffset(0.9);
   xCorr->GetYaxis()->SetTitleSize(0.05);
   xCorr->GetYaxis()->SetLabelSize(0.05);
   xCorr->GetYaxis()->SetTitleOffset(0.9);
   xCorr->SetNdivisions(7);
   xCorr->SetFillColor(kBlue-5);
   xCorr->SetFillStyle(3001);
   xCorr->SetLineWidth(2);
     
   TH1D *xCorr_slit = new TH1D("xCorr_slit","xCorr_slit",3000.,0.,300);
   xCorr_slit->GetXaxis()->SetTitle(titolox);
   xCorr_slit->GetXaxis()->SetTitleSize(0.05);
   xCorr_slit->GetXaxis()->SetLabelSize(0.05);
   xCorr_slit->GetXaxis()->SetTitleOffset(0.9);
   xCorr_slit->GetYaxis()->SetTitleSize(0.05);
   xCorr_slit->GetYaxis()->SetLabelSize(0.05);
   xCorr_slit->GetYaxis()->SetTitleOffset(0.9);
   xCorr_slit->SetNdivisions(7);
   xCorr_slit->SetFillColor(kRed);
   xCorr_slit->SetLineColor(kRed);
   xCorr_slit->SetFillStyle(3001);
   xCorr_slit->SetLineWidth(2);
   
   
   TH1D *xCorr_slitSlit = new TH1D("xCorr_slitSlit","xCorr_slitSlit",3000.,0.,300);
   xCorr_slitSlit->GetXaxis()->SetTitle(titolox);
   xCorr_slitSlit->GetXaxis()->SetTitleSize(0.05);
   xCorr_slitSlit->GetXaxis()->SetLabelSize(0.05);
   xCorr_slitSlit->GetXaxis()->SetTitleOffset(0.9);
   xCorr_slitSlit->GetYaxis()->SetTitleSize(0.05);
   xCorr_slitSlit->GetYaxis()->SetLabelSize(0.05);
   xCorr_slitSlit->GetYaxis()->SetTitleOffset(0.9);
   xCorr_slitSlit->SetNdivisions(7);
   xCorr_slitSlit->SetLineColor(kOrange+1);
   xCorr_slitSlit->SetLineWidth(2);
   xCorr_slitSlit->SetFillColor(kYellow);
      
//################################################################################################################
// OpenFiles
   openTrackFileTh(run);
   tree->Print();
   Double_t entries = tree->GetEntries();
   cout << entries << endl << endl;
//###########################################################################################################
// Graphyical cut definition

TCutG *cutG = new TCutG("cutG",17);                                                                                                          
cutG->SetVarX("cl_x_mm[0]");                                                            
cutG->SetVarY("cl_x_mm[1]");                                                            
cutG->SetPoint(0,61.3231,106.368);                                                      
cutG->SetPoint(1,60.744,106.088);                                                       
cutG->SetPoint(2,60.4596,105.69);                                                       
cutG->SetPoint(3,60.1437,105.175);                                                      
cutG->SetPoint(4,59.8805,104.637);                                                      
cutG->SetPoint(5,59.7857,104.052);                                                      
cutG->SetPoint(6,59.7962,103.21);                                                       
cutG->SetPoint(7,60.5018,102.835);                                                      
cutG->SetPoint(8,61.281,103.139);                                                       
cutG->SetPoint(9,62.0286,103.654);                                                      
cutG->SetPoint(10,62.871,104.192);                                                      
cutG->SetPoint(11,63.4712,105.105);                                                     
cutG->SetPoint(12,63.566,105.971);                                                       
cutG->SetPoint(13,63.3765,106.626);                                                     
cutG->SetPoint(14,62.7552,106.789);                                                     
cutG->SetPoint(15,61.7864,106.532);                                                     
cutG->SetPoint(16,61.3231,106.368);                                              
   
//#################################################################################################
// Data Loop
   for(Int_t i=0; i<entries; i++){
   
      Double_t pad[NRows][50] = {0.};
      Double_t charge[NRows][50] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t totalCharge[NRows] = {0.};
      
      tree->GetEntry(i);
      
      for(Int_t row = 0; row < NRows; row++){
         nPoint = row;
         Int_t mult = cl_padMult[row];
         if(mult > 50) {
          // cout << "WARNING: cl_padMult[" << row << "] = " << mult << " > 50, taglio a 50" << endl;
            mult = 50;
         }
         if(mult > NStrips) {
           // cout << "WARNING: cl_padMult[" << row << "] = " << mult << " > NStrips(" << NStrips << "), taglio a NStrips" << endl;
           mult = NStrips;
         }
         for(Int_t p = 0; p < mult; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            x_mm[p] = 2.5 + (5 * pad[row][p]);
            x[row] += (x_mm[p] * charge[row][p]);
         }
         if(totalCharge[row] > 0){
           x[row] = x[row]/totalCharge[row];
           x_c[row]->Fill(x[row]);
           if(row==3){
             x_NC->Fill(Abs(x[3]-x[2]));
             if(/*theta_deg>=0 && theta_deg<20 &&*/ sic_fired==1 && energySic>2000 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
               x_NC_cut->Fill(Abs(x[3]-x[2]));
             }
           }
         }
      }
      
   }
   
   ofstream output;
   
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

   for(Int_t j=0; j<evts; j++){
       mytree->GetEntry(j);
       
       xCorr->Fill(x_corr[3]-x_corr[2]);
       
   }
   
   /*char fileOut[100];
   sprintf(fileOut,"resolution/resol_run_%d_cut_th_%.0fmm_slit.txt",run,2*semiaperture);
   output.open(fileOut);
   */
   //output << "             Preliminary results" << endl << endl;
   
   /*TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      f_gaus->SetParameters(0,0,0);
      xCorr[row]->Fit(f_gaus,"N","N",0,300);
      Double_t stDEV = f_gaus->GetParameter(2);
      Double_t y = xCorr[row]->GetMaximum();
      char testo[100];
      sprintf(testo,"FWHM: 2.35*%f = %f",stDEV,2.35*stDEV);
      x_NC[row]->Draw();
      x_NC_cut[row]->Draw("SAME");
      xCorr[row]->Draw("SAME");
      output << "stDEV_" << row << ": " << stDEV << "   FWHM: 2.35*stDEV = "<< 2.35*stDEV << "   StdDEV(): " << xCorr[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr[row]->GetStdDev())<< endl;
   }*/
   
   row0 = x_c[0]->GetMean();
   row4 = x_c[4]->GetMean();
   cout << "Row0: " << row0 << "     Row4: "<< row4 << endl;
   for(Int_t i=0; i<evts; i++){
      mytree->GetEntry(i);
      if(x_corr[4]>=row4-semiaperture && x_corr[4]<=row4+semiaperture){                                                                                  // single-slit in row4
        xCorr_slit->Fill(x_corr[3]-x_corr[2]);
      }
   }
   
   /*output << endl << "       Preliminary results - slit on ROW4" << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      f_gaus->SetParameters(0,0,0);
      xCorr_slit[row]->Fit(f_gaus,"N","N",0,299);
      Double_t stDEV_slit = f_gaus->GetParameter(2);
      output << "stDEV_slit_" << row << ": " << stDEV_slit << "   FWHM: 2.35*stDEV_slit = "<< 2.35*stDEV_slit << "   StdDEV(): " << xCorr_slit[row]->GetStdDev() << "   FWHM: 2.35*stDEV = " << 2.35*(xCorr_slit[row]->GetStdDev())<< endl;
   }*/
   
   for(Int_t j=0; j<evts; j++){
      mytree->GetEntry(j);
      if(x_corr[0]>=row0-semiaperture && x_corr[0]<=row0+semiaperture && x_corr[4]>=row4-semiaperture && x_corr[4]<=row4+semiaperture){                                    // double-slit row0 & row4
        xCorr_slitSlit->Fill(x_corr[3]-x_corr[2]);
      }
   }

//#################################################################################################
// Visualization block
   
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(x_NC, "All events", "f");
   l->AddEntry(x_NC_cut, "TCutg appl.", "f");
   l->AddEntry(xCorr, "TCutg & offset-corr.", "f");
   l->AddEntry(xCorr_slit, "Slit ROW4", "f");
   l->AddEntry(xCorr_slitSlit, "2-slit ROWs 0-4", "f");
   
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   x_NC->Draw();
   x_NC_cut->Draw("SAME");
   xCorr->Draw("SAME");
   xCorr_slit->Draw("SAME");
   xCorr_slitSlit->Draw("SAME");
   l->Draw("SAME");
   
   char fileOut[100];
   sprintf(fileOut,"resolution/resolDiff3_2_run_%d_cut_th_%.0fmm_slit.txt",run,2*semiaperture);
   output.open(fileOut);
   Double_t st[5] = {0.};
   x_NC->Fit(f_gaus,"N","N",0,300);
   st[0] = f_gaus->GetParameter(2);
   f_gaus->SetParameters(0,0,0);
   x_NC_cut->Fit(f_gaus,"N","N",0,300);
   st[1] = f_gaus->GetParameter(2);
   f_gaus->SetParameters(0,0,0);
   xCorr->Fit(f_gaus,"N","N",0,300);
   st[2] = f_gaus->GetParameter(2);
   f_gaus->SetParameters(0,0,0);
   xCorr_slit->Fit(f_gaus,"N","N",0,300);
   st[3] = f_gaus->GetParameter(2);
   f_gaus->SetParameters(0,0,0);
   xCorr_slitSlit->Fit(f_gaus,"N","N",0,300);
   st[4] = f_gaus->GetParameter(2);
   
   output << "             Preliminary results" << endl << endl;
   output << "stDEV: " << st[0] << "   FWHM: 2.35*stDEV = " << 2.35*st[0] << "   stDEV()"  << ": " << x_NC->GetStdDev() << "   FWHM: 2.35*stDEV() = "<< 2.35*x_NC->GetStdDev() << endl;
   output << "stDEV: " << st[1] << "   FWHM: 2.35*stDEV = " << 2.35*st[1] << "   stDEV()"  << ": " << x_NC_cut->GetStdDev() << "   FWHM: 2.35*stDEV() = "<< 2.35*x_NC_cut->GetStdDev() << endl;
   output << "stDEV: " << st[2] << "   FWHM: 2.35*stDEV = " << 2.35*st[2] << "   stDEV()"  << ": " << xCorr->GetStdDev() << "   FWHM: 2.35*stDEV() = "<< 2.35*xCorr->GetStdDev() << endl;
   output << "stDEV: " << st[3] << "   FWHM: 2.35*stDEV = " << 2.35*st[3] << "   stDEV()"  << ": " << xCorr_slit->GetStdDev() << "   FWHM: 2.35*stDEV() = "<< 2.35*xCorr_slit->GetStdDev() << endl;
   output << "stDEV: " << st[4] << "   FWHM: 2.35*stDEV = " << 2.35*st[4] << "   stDEV()"  << ": " << xCorr_slitSlit->GetStdDev() << "   FWHM: 2.35*stDEV() = "<< 2.35*xCorr_slitSlit->GetStdDev() << endl;
   
}
