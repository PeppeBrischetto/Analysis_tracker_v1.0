//#################################################################################################################
//#   This macro allows to obtain the centre of gravity distribution for each rows from a tracks_ID.root file.
//#################################################################################################################
//#   Created November 2025 by A. Pitronaci
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>
#include "../Include/openfiles.h"
//#include "../Include/openfilesclone.h"

using namespace std;
using namespace TMath;

const TString InputFileC = "resol_run_52ELLcut.txt";
const TString InputFileC1 = "resol_run_38ELLcut.txt";
const TString InputFileC2 = "resol_run_65ELLcut.txt";
const TString InputFileC3 = "resol_run_76ELLcut.txt";
const TString InputFileLi = "resol_run_298ELLcut.txt";
const TString InputFileO = "resol_run_171ELLcut.txt";

const Int_t NRows = 5;

void quanta_x_mult_LI(int run){

//################################################################################################################
// Needed variables
   Double_t par[9] = {0.};                                             // parameters-array to store single-fits
   char histoname[100];
   char tit[100];
   char titoloFile[100];
   Double_t minimum1,minimum2 = 0.;
   Int_t mult[4] = {4,5,6,7};                                          // multiplicity-array to cut histograms
   
   ofstream outfit;
   
   TH1D *histo_x[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x[i]=new TH1D("","",9000.,0.,300);
      histo_x[i]->GetXaxis()->SetTitle(tit);
      histo_x[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x[i]->SetNdivisions(12);
      histo_x[i]->SetLineColor(kCyan+1);
      histo_x[i]->SetFillColor(kCyan-10);
      //histo_x[i]->SetFillStyle(3001);
      histo_x[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x1[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x1 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x1[i]=new TH1D("","",9000.,0.,300);
      histo_x1[i]->GetXaxis()->SetTitle(tit);
      histo_x1[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x1[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x1[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x1[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x1[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x1[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x1[i]->SetNdivisions(12);
      histo_x1[i]->SetLineColor(kViolet+1);
      //histo_x1[i]->SetFillColor(kTeal-5);
      //histo_x1[i]->SetFillStyle(3001);
      histo_x1[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x2[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x2 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x2[i]=new TH1D("","",9000.,0.,300);
      histo_x2[i]->GetXaxis()->SetTitle(tit);
      histo_x2[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x2[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x2[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x2[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x2[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x2[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x2[i]->SetNdivisions(12);
      histo_x2[i]->SetLineColor(kRed-1);
      //histo_x2[i]->SetFillColor(kGreen-5);
      //histo_x2[i]->SetFillStyle(3001);
      histo_x2[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x3[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x3 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x3[i]=new TH1D("","",9000.,0.,300);
      histo_x3[i]->GetXaxis()->SetTitle(tit);
      histo_x3[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x3[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x3[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x3[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x3[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x3[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x3[i]->SetNdivisions(12);
      histo_x3[i]->SetLineColor(kYellow+2);
      //histo_xe[i]->SetFillColor(kGreen-5);
      //histo_xe[i]->SetFillStyle(3001);
      histo_x3[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x4[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x4 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x4[i]=new TH1D("","",9000.,0.,300);
      histo_x4[i]->GetXaxis()->SetTitle(tit);
      histo_x4[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x4[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x4[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x4[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x4[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x4[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x4[i]->SetNdivisions(12);
      histo_x4[i]->SetLineColor(kRed+1);
      //histo_x4[i]->SetFillColor(kRed-5);
      //histo_x4[i]->SetFillStyle(3001);
      histo_x4[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x5[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x5 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x5[i]=new TH1D("","",9000.,0.,300);
      histo_x5[i]->GetXaxis()->SetTitle(tit);
      histo_x5[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x5[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x5[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x5[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x5[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x5[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x5[i]->SetNdivisions(12);
      //histo_x5[i]->SetFillColor(kBlue-5);
      //histo_x5[i]->SetFillStyle(3001);
      histo_x5[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x6[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x6 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x6[i]=new TH1D("","",9000.,0.,300);
      histo_x6[i]->GetXaxis()->SetTitle(tit);
      histo_x6[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x6[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x6[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x6[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x6[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x6[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x6[i]->SetNdivisions(12);
      histo_x6[i]->SetLineColor(kGreen+1);
      //histo_x6[i]->SetFillColor(kGreen-5);
      //histo_x6[i]->SetFillStyle(3001);
      histo_x6[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x7[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x7 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x7[i]=new TH1D("","",9000.,0.,300);
      histo_x7[i]->GetXaxis()->SetTitle(tit);
      histo_x7[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x7[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x7[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x7[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x7[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x7[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x7[i]->SetNdivisions(12);
      histo_x7[i]->SetLineColor(kOrange+1);
      //histo_x7[i]->SetFillColor(kOrange-5);
      //histo_x7[i]->SetFillStyle(3001);
      histo_x7[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x8[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x8 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x8[i]=new TH1D("","",9000.,0.,300);
      histo_x8[i]->GetXaxis()->SetTitle(tit);
      histo_x8[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x8[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x8[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x8[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x8[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x8[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x8[i]->SetNdivisions(12);
      histo_x8[i]->SetLineColor(kYellow+1);
      //histo_x8[i]->SetFillColor(kYellow-5);
      //histo_x8[i]->SetFillStyle(3001);
      histo_x8[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x9[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x9 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x9[i]=new TH1D("","",9000.,0.,300);
      histo_x9[i]->GetXaxis()->SetTitle(tit);
      histo_x9[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x9[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x9[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x9[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x9[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x9[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x9[i]->SetNdivisions(12);
      histo_x9[i]->SetLineColor(kMagenta+2);
      //histo_x9[i]->SetFillColor(kMagenta-5);
      //histo_x9[i]->SetFillStyle(3001);
      histo_x9[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x10[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x10 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x10[i]=new TH1D("","",9000.,0.,300);
      histo_x10[i]->GetXaxis()->SetTitle(tit);
      histo_x10[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x10[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x10[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x10[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x10[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x10[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x10[i]->SetNdivisions(12);
      histo_x10[i]->SetLineColor(kPink+2);
      //histo_x10[i]->SetFillColor(kMagenta-5);
      //histo_x10[i]->SetFillStyle(3001);
      histo_x10[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x11[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x11 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x11[i]=new TH1D("","",9000.,0.,300);
      histo_x11[i]->GetXaxis()->SetTitle(tit);
      histo_x11[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x11[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x11[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x11[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x11[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x11[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x11[i]->SetNdivisions(12);
      histo_x11[i]->SetLineColor(kBlue-1);
      //histo_x11[i]->SetFillColor(kMagenta-5);
      //histo_x11[i]->SetFillStyle(3001);
      histo_x11[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x12[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x12 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x12[i]=new TH1D("","",9000.,0.,300);
      histo_x12[i]->GetXaxis()->SetTitle(tit);
      histo_x12[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x12[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x12[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x12[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x12[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x12[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x12[i]->SetNdivisions(12);
      histo_x12[i]->SetLineColor(kBlack);
      //histo_x12[i]->SetFillColor(kMagenta-5);
      //histo_x12[i]->SetFillStyle(3001);
      histo_x12[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x13[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x13 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x13[i]=new TH1D("","",9000.,0.,300);
      histo_x13[i]->GetXaxis()->SetTitle(tit);
      histo_x13[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x13[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x13[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x13[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x13[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x13[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x13[i]->SetNdivisions(12);
      histo_x13[i]->SetLineColor(kCyan+2);
      //histo_x13[i]->SetFillColor(kMagenta-5);
      //histo_x13[i]->SetFillStyle(3001);
      histo_x13[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x14[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x14 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x14[i]=new TH1D("","",9000.,0.,300);
      histo_x14[i]->GetXaxis()->SetTitle(tit);
      histo_x14[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x14[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x14[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x14[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x14[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x14[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x14[i]->SetNdivisions(12);
      histo_x14[i]->SetLineColor(kOrange+3);
      //histo_x14[i]->SetFillColor(kMagenta-5);
      //histo_x14[i]->SetFillStyle(3001);
      histo_x14[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x15[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x15 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x15[i]=new TH1D("","",9000.,0.,300);
      histo_x15[i]->GetXaxis()->SetTitle(tit);
      histo_x15[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x15[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x15[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x15[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x15[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x15[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x15[i]->SetNdivisions(12);
      histo_x15[i]->SetLineColor(kRed-10);
      //histo_x15[i]->SetFillColor(kMagenta-5);
      //histo_x15[i]->SetFillStyle(3001);
      histo_x15[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x16[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x16 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x16[i]=new TH1D("","",9000.,0.,300);
      histo_x16[i]->GetXaxis()->SetTitle(tit);
      histo_x16[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x16[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x16[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x16[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x16[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x16[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x16[i]->SetNdivisions(12);
      histo_x16[i]->SetLineColor(kBlue-9);
      //histo_x16[i]->SetFillColor(kMagenta-5);
      //histo_x16[i]->SetFillStyle(3001);
      histo_x16[i]->SetLineWidth(2);
   }
   
   TH1D *histo_x17[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x17 %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x17[i]=new TH1D("","",9000.,0.,300);
      histo_x17[i]->GetXaxis()->SetTitle(tit);
      histo_x17[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x17[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x17[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x17[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x17[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x17[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x17[i]->SetNdivisions(12);
      histo_x17[i]->SetLineColor(kMagenta-7);
      //histo_x17[i]->SetFillColor(kMagenta-5);
      //histo_x17[i]->SetFillStyle(3001);
      histo_x17[i]->SetLineWidth(2);
   }
   
   TF1 *f1 = new TF1("f1","gaus",139,141.2);
   TF1 *f2 = new TF1("f2","gaus",141.2,143.2);
   TF1 *f3 = new TF1("f3","gaus",143.5,146);
   TF1 *f = new TF1("f","gaus(0)+gaus(3)+gaus(6)",134,147);
   

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//################################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",5);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,22.0711,27.5907);
   cutG->SetPoint(1,176.674,208.212);
   cutG->SetPoint(2,185.042,194.43);
   cutG->SetPoint(3,27.5105,16.3471);
   cutG->SetPoint(4,22.0711,27.5907);
   
//################################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      if(/*sic_fired==1 && energySic>2000 && cl_x_mm[0]>12 && cl_x_mm[1]>12 && cl_x_mm[2]>12 && cl_x_mm[3]>12 && cl_x_mm[4]>12 &&*/ cutG->IsInside(cl_x_mm[0], cl_x_mm[1]) /*&& theta_deg>35 && theta_deg<40*/){
         for(Int_t row=0; row<NRows; row++){
            histo_x[row]->Fill(cl_x_mm[row]);
            if(cl_padMult[row]==1){
              histo_x1[row]->Fill(cl_x_mm[row]);
            }else
            if(cl_padMult[row]==2){
              histo_x2[row]->Fill(cl_x_mm[row]);
            }else
            if(cl_padMult[row]==3){
              histo_x3[row]->Fill(cl_x_mm[row]);
            }else
            if(cl_padMult[row]==4){
              histo_x4[row]->Fill(cl_x_mm[row]);
            }else
             if(cl_padMult[row]==5){
               histo_x5[row]->Fill(cl_x_mm[row]);
             }else
              if(cl_padMult[row]==6){
                histo_x6[row]->Fill(cl_x_mm[row]);
              }else
               if(cl_padMult[row]==7){
                 histo_x7[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==8){
                 histo_x8[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==9){
                 histo_x9[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==10){
                 histo_x10[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==11){
                 histo_x11[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==12){
                 histo_x12[row]->Fill(cl_x_mm[row]);
               }else
                if(cl_padMult[row]==13){
                 histo_x13[row]->Fill(cl_x_mm[row]);
                }else
                 if(cl_padMult[row]==14){
                 histo_x14[row]->Fill(cl_x_mm[row]);
                 }else
                  if(cl_padMult[row]==15){
                    histo_x15[row]->Fill(cl_x_mm[row]);
                  }else
                   if(cl_padMult[row]==16){
                     histo_x16[row]->Fill(cl_x_mm[row]);
                   }else
                    if(cl_padMult[row]==17){
                      histo_x17[row]->Fill(cl_x_mm[row]);
                    }
         }
      }                                                                                 // TCutG-parenthesis
   }
   
//################################################################################################################
// Fitting histograms
   
   /*histo_x[2]->Fit(f1,"R");
   histo_x[2]->Fit(f2,"R");
   histo_x[2]->Fit(f3,"R");
   f1->GetParameters(&par[0]);
   f2->GetParameters(&par[3]);
   f3->GetParameters(&par[6]);
   f->SetParameters(par);
   
   histo_x[2]->Fit("f","R+");*/

//################################################################################################################
// Searching extremes of quantization classes      
   
   /*TF1 *df = new TF1("df","(f1(x+[0])-f1(x-[0]))/(2*[0])",134,147);
   df->FixParameter(0,1E-8);
   TF1 *d2f = new TF1("d2f","(df(x+[0])-df(x-[0]))/(2*[0])",134,147);
   d2f->FixParameter(0,1E-8);

   //cout << "La derivata a x=139 picco vale: " << df->Eval(par[1]) << " e la sua derivata seconda vale: " << d2f->Eval(par[1]) << endl;
   
   minimum1 = f->GetMinimumX(par[1],par[4],1E-8,100000,0);
   minimum2 = f->GetMinimumX(par[4],par[7],1E-8,100000,0);
   cout << "Minimum1: " << minimum1 << "    Minimum2: " << minimum2 << endl;*/

//################################################################################################################
// Quantiazion classes creation - multiplicity correlation
   
   /*Int_t p1,p2,p3 = 0.;
   
   TH1D *histo_m[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_m %i",i);
      histo_m[i]=new TH1D("","",60,-0.5,59.5);
      histo_m[i]->GetXaxis()->SetTitle("multiplicity");
      histo_m[i]->GetXaxis()->SetTitleSize(0.05);
      histo_m[i]->GetXaxis()->SetLabelSize(0.05);
      histo_m[i]->GetXaxis()->SetTitleOffset(1.);
      histo_m[i]->GetYaxis()->SetTitleSize(0.05);
      histo_m[i]->GetYaxis()->SetLabelSize(0.05);
      histo_m[i]->GetYaxis()->SetTitleOffset(1.);
      histo_m[i]->SetNdivisions(12);
      histo_m[i]->SetFillColor(kBlue-5);
      histo_m[i]->SetFillStyle(3001);
      histo_m[i]->SetLineWidth(2);
   }
   
   TH1D *histo_theta[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_theta %i",i);
      histo_theta[i]=new TH1D("","",360,0,90);
      histo_theta[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      histo_theta[i]->GetXaxis()->SetTitleSize(0.05);
      histo_theta[i]->GetXaxis()->SetLabelSize(0.05);
      histo_theta[i]->GetXaxis()->SetTitleOffset(1.);
      histo_theta[i]->GetYaxis()->SetTitleSize(0.05);
      histo_theta[i]->GetYaxis()->SetLabelSize(0.05);
      histo_theta[i]->GetYaxis()->SetTitleOffset(1.);
      histo_theta[i]->SetNdivisions(12);
      histo_theta[i]->SetFillColor(kBlue-5);
      histo_theta[i]->SetFillStyle(3001);
      histo_theta[i]->SetLineWidth(2);
   }
   
   TH1D *histo_intercept[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_intercept %i",i);
      histo_intercept[i]=new TH1D("","",360,-20,20);
      histo_intercept[i]->GetXaxis()->SetTitle("intercept (mm)");
      histo_intercept[i]->GetXaxis()->SetTitleSize(0.05);
      histo_intercept[i]->GetXaxis()->SetLabelSize(0.05);
      histo_intercept[i]->GetXaxis()->SetTitleOffset(1.);
      histo_intercept[i]->GetYaxis()->SetTitleSize(0.05);
      histo_intercept[i]->GetYaxis()->SetLabelSize(0.05);
      histo_intercept[i]->GetYaxis()->SetTitleOffset(1.);
      histo_intercept[i]->SetNdivisions(12);
      histo_intercept[i]->SetFillColor(kBlue-5);
      histo_intercept[i]->SetFillStyle(3001);
      histo_intercept[i]->SetLineWidth(2);
   }
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      if(cl_x_mm[2]<minimum1 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
        p1 ++;
        histo_m[0]->Fill(cl_padMult[2]);
        histo_theta[0]->Fill(theta_deg);
        histo_intercept[0]->Fill(interceptP);
      }else
       if(cl_x_mm[2]>minimum1 && cl_x_mm[2]<minimum2 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
         p2 ++;
         histo_m[1]->Fill(cl_padMult[2]);
         histo_theta[1]->Fill(theta_deg);
         histo_intercept[1]->Fill(interceptP);
       }else
        if(cl_x_mm[2]>minimum2){
          p3 ++;
          histo_m[2]->Fill(cl_padMult[2] && cutG->IsInside(cl_x_mm[0], cl_x_mm[1]));
          histo_theta[2]->Fill(theta_deg);
          histo_intercept[2]->Fill(interceptP);
        }
   }
   
   cout << "Evts. pop1: " << p1 << "Evts. pop2: " << p2 << "Evts. pop3: " << p3 << endl;
   */   

// Writing main info (mean and errors) on a .txt file for each threshold
   
   sprintf(titoloFile,"Run%d/CoG_run_%d_th1000.txt",run,run);
   outfit.open(titoloFile);
   outfit << "                  Centre of Gravity reconstruction - run: " << run << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      outfit << endl;
      outfit << "Centre of Gravity (>thr) row " << row << ": " << histo_x[row]->GetMean() << "   Error of Centre of Gravity Tot): " << histo_x[row]->GetMeanError() << endl;
   }
   
//################################################################################################################
// Visualization block

   TText *t[5];
   char titolo[100];
   
   TLegend* l = new TLegend(0.1,0.35,0.26,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(histo_x[0], "All events", "f");
   l->AddEntry(histo_x1[0], "M = 1", "f");
   l->AddEntry(histo_x2[0], "M = 2", "f");
   l->AddEntry(histo_x3[0], "M = 3", "f");
   l->AddEntry(histo_x4[0], "M = 4", "f");
   l->AddEntry(histo_x5[0], "M = 5", "f");
   l->AddEntry(histo_x6[0], "M = 6", "f");
   l->AddEntry(histo_x7[0], "M = 7", "f");
   l->AddEntry(histo_x8[0], "M = 8", "f");
   l->AddEntry(histo_x9[0], "M = 9", "f");
   l->AddEntry(histo_x10[0], "M = 10", "f");
   l->AddEntry(histo_x11[0], "M = 11", "f");
   l->AddEntry(histo_x12[0], "M = 12", "f");
   l->AddEntry(histo_x13[0], "M = 13", "f");
   l->AddEntry(histo_x14[0], "M = 14", "f");
   l->AddEntry(histo_x15[0], "M = 15", "f");
   l->AddEntry(histo_x16[0], "M = 16", "f");
   l->AddEntry(histo_x17[0], "M = 17", "f");
   
   TCanvas *c0 = new TCanvas("c0","c0",2200,2200);
   c0->Divide(3,2);
   
   for(Int_t row=0; row<NRows; row++){
      c0->cd(row+1);
      histo_x[row]->Draw();
      histo_x1[row]->Draw("SAME");
      histo_x2[row]->Draw("SAME");
      histo_x3[row]->Draw("SAME");
      histo_x13[row]->Draw("SAME");
      histo_x12[row]->Draw("SAME");
      histo_x11[row]->Draw("SAME");
      histo_x10[row]->Draw("SAME");
      histo_x9[row]->Draw("SAME");
      histo_x8[row]->Draw("SAME");
      histo_x7[row]->Draw("SAME");
      histo_x6[row]->Draw("SAME");
      histo_x5[row]->Draw("SAME");
      histo_x4[row]->Draw("SAME");
      histo_x17[row]->Draw("SAME");
      histo_x16[row]->Draw("SAME");
      histo_x15[row]->Draw("SAME");
      histo_x14[row]->Draw("SAME");
      l->Draw();
   }
 
   /*c0->cd(1);
   histo_x[0]->Draw();
   histo_x8[0]->Draw("SAME");
   histo_x7[0]->Draw("SAME");
   histo_x6[0]->Draw("SAME");
   histo_x5[0]->Draw("SAME");
   histo_x4[0]->Draw("SAME");
   histo_x9[0]->Draw("SAME");
   l->Draw();
   
   c0->cd(2);
   histo_x[1]->Draw();
   histo_x6[1]->Draw("SAME");
   histo_x7[1]->Draw("SAME");
   histo_x5[1]->Draw("SAME");
   histo_x8[1]->Draw("SAME");
   histo_x4[1]->Draw("SAME");
   histo_x9[1]->Draw("SAME");
   l->Draw();
   
   c0->cd(3);
   histo_x[2]->Draw();
   histo_x5[2]->Draw("SAME");
   histo_x6[2]->Draw("SAME");
   histo_x7[2]->Draw("SAME");
   histo_x8[2]->Draw("SAME");
   histo_x4[2]->Draw("SAME");
   histo_x9[2]->Draw("SAME");
   l->Draw();
   
   c0->cd(4);
   histo_x[3]->Draw();
   histo_x8[3]->Draw("SAME");
   histo_x5[3]->Draw("SAME");
   histo_x6[3]->Draw("SAME");
   histo_x7[3]->Draw("SAME");
   histo_x4[3]->Draw("SAME");
   histo_x9[3]->Draw("SAME");
   l->Draw();
   
   c0->cd(5);
   histo_x[4]->Draw();
   histo_x8[4]->Draw("SAME");
   histo_x6[4]->Draw("SAME");
   histo_x5[4]->Draw("SAME");
   histo_x7[4]->Draw("SAME");
   histo_x4[4]->Draw("SAME");
   histo_x9[4]->Draw("SAME");
   l->Draw();*/
   
     //t[row]->Draw("SAME");
   /*TText *t_m[3];
   TText *t_theta[3];
   TText *t_intercept[3];
   char titolo[100];
   
   TCanvas *c1 = new TCanvas("c1","c1",1700,700);
   c1->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c1->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_m[col] = new TText(0.5,0.5*histo_m[col]->GetMaximum(),titolo);
      histo_m[col]->Draw();
      t_m[col]->Draw();
   }
   
   TCanvas *c2 = new TCanvas("c2","c2",1700,700);
   c2->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c2->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_theta[col] = new TText(0.5,0.5*histo_theta[col]->GetMaximum(),titolo);
      histo_theta[col]->Draw();
      t_theta[col]->Draw();
   }
   
   TCanvas *c3 = new TCanvas("c3","c3",1700,700);
   c3->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c3->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_theta[col] = new TText(0.5,0.5*histo_intercept[col]->GetMaximum(),titolo);
      histo_intercept[col]->Draw();
      t_theta[col]->Draw();
   }*/
}
