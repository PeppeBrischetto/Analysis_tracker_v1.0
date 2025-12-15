//#################################################################################################################
//#   This macro allows to obtain the centre of gravity of each rows from a tracks_ID.root file, thus studying its
//#   behaviour as a function of the thresholds. It refers to a single track event.
//#################################################################################################################
//#   Created December 2025 by A. Pitronaci
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

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 6;
const Int_t NPads = 60;

const TString InputFile = "CoG_Th_38_i.txt";
const TString InputFile1 = "CoG_Th_39_i.txt";
const TString InputFile2 = "CoG_Th_40_i.txt";
const TString InputFile3 = "CoG_Th_41_i.txt";
const TString InputFile4 = "CoG_Th_42_i.txt";

void plot_cogVsth(){
   
//#################################################################################################################
// Nedeed variables
   ifstream file,file1,file2,file3,file4;
   string dummy;
   Double_t x[NRows],x1[NRows],x2[NRows],x3[NRows],x4[NRows] = {0.};
   Double_t th[14] = {0,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600};
   char graphname[100];
   char tit[100]; 
   
   TGraph *gr[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr[i] = new TGraph();
      gr[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr[i]->GetYaxis()->SetTitle(tit);
      gr[i]->GetXaxis()->SetTitleSize(0.05);
      gr[i]->GetXaxis()->SetLabelSize(0.05);
      gr[i]->GetXaxis()->SetTitleOffset(0.9);
      gr[i]->GetYaxis()->SetTitleSize(0.05);
      gr[i]->GetYaxis()->SetLabelSize(0.05);
      gr[i]->GetYaxis()->SetTitleOffset(1.5);
      gr[i]->SetLineColor(kBlue);
      gr[i]->SetMarkerStyle(20);
      gr[i]->SetMarkerColor(kBlue);
   }
   TGraph *gr1[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr1[i] = new TGraph();
      gr1[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr1[i]->GetYaxis()->SetTitle(tit);
      gr[i]->GetXaxis()->SetTitleSize(0.05);
      gr1[i]->GetXaxis()->SetLabelSize(0.05);
      gr1[i]->GetXaxis()->SetTitleOffset(0.9);
      gr1[i]->GetYaxis()->SetTitleSize(0.05);
      gr1[i]->GetYaxis()->SetLabelSize(0.05);
      gr1[i]->GetYaxis()->SetTitleOffset(1.5);
      gr1[i]->SetLineColor(kRed);
      gr1[i]->SetMarkerStyle(20);
      gr1[i]->SetMarkerColor(kRed);
   }
   
   TGraph *gr2[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr2[i] = new TGraph();
      gr2[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr2[i]->GetYaxis()->SetTitle(tit);
      gr2[i]->GetXaxis()->SetTitleSize(0.05);
      gr2[i]->GetXaxis()->SetLabelSize(0.05);
      gr2[i]->GetXaxis()->SetTitleOffset(0.9);
      gr2[i]->GetYaxis()->SetTitleSize(0.05);
      gr2[i]->GetYaxis()->SetLabelSize(0.05);
      gr2[i]->GetYaxis()->SetTitleOffset(1.5);
      gr2[i]->SetLineColor(kGreen+2);
      gr2[i]->SetMarkerStyle(20);
      gr2[i]->SetMarkerColor(kGreen+2);
   }
   
   TGraph *gr3[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr3[i] = new TGraph();
      gr3[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr3[i]->GetYaxis()->SetTitle(tit);
      gr3[i]->GetXaxis()->SetTitleSize(0.05);
      gr3[i]->GetXaxis()->SetLabelSize(0.05);
      gr3[i]->GetXaxis()->SetTitleOffset(0.9);
      gr3[i]->GetYaxis()->SetTitleSize(0.05);
      gr3[i]->GetYaxis()->SetLabelSize(0.05);
      gr3[i]->GetYaxis()->SetTitleOffset(1.5);
      gr3[i]->SetLineColor(kMagenta+2);
      gr3[i]->SetMarkerStyle(20);
      gr3[i]->SetMarkerColor(kMagenta+2);
   }
   
   TGraph *gr4[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr4[i] = new TGraph();
      gr4[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr4[i]->GetYaxis()->SetTitle(tit);
      gr4[i]->GetXaxis()->SetTitleSize(0.05);
      gr4[i]->GetXaxis()->SetLabelSize(0.05);
      gr4[i]->GetXaxis()->SetTitleOffset(0.9);
      gr4[i]->GetYaxis()->SetTitleSize(0.05);
      gr4[i]->GetYaxis()->SetLabelSize(0.05);
      gr4[i]->GetYaxis()->SetTitleOffset(1.5);
      gr4[i]->SetLineColor(kOrange+2);
      gr4[i]->SetMarkerStyle(20);
      gr4[i]->SetMarkerColor(kOrange+2);
   }

//#################################################################################################################
// Open file
   file.open(InputFile);
   file1.open(InputFile1);
   file2.open(InputFile2);
   file3.open(InputFile3);
   file4.open(InputFile4);
   
   if(!file.is_open()){
     cout << "Opening output file failed!" << endl;
   }else{
           for(Int_t j=0; j<14; j++){
              file >> dummy >> x[0] >> x[1] >> x[2] >> x[3] >> x[4];
              file1 >> dummy >> x1[0] >> x1[1] >> x1[2] >> x1[3] >> x1[4];
              file2 >> dummy >> x2[0] >> x2[1] >> x2[2] >> x2[3] >> x2[4];
              file3 >> dummy >> x3[0] >> x3[1] >> x3[2] >> x3[3] >> x3[4];
              file4 >> dummy >> x4[0] >> x4[1] >> x4[2] >> x4[3] >> x4[4];
              for(Int_t row=0; row<NRows; row++){
                 //cout << "   x_" << row << ": " << x[row] << endl;
                 gr[row]->SetPoint(j,th[j],x[row]);
                 gr1[row]->SetPoint(j,th[j],x1[row]);
                 gr2[row]->SetPoint(j,th[j],x2[row]);
                 gr3[row]->SetPoint(j,th[j],x3[row]);
                 gr4[row]->SetPoint(j,th[j],x4[row]);
              }
           }
           
     }

//#################################################################################################################
// Visualization block
   
   TLegend* l = new TLegend(0.1,0.75,0.9,0.9);
   //l->SetNColumns(2);
   l->SetTextSize(0.04);
   l->SetLineWidth(0);
   //l->AddEntry(gr[0], "x(thresh.) - 160 V - last evt", "lp");
   //l->AddEntry(gr1[0], "x(thresh.) - 170 V - 1^{st} evt", "lp");
   //l->AddEntry(gr2[0], "x(thresh.) - 180 V", "lp");
   l->AddEntry(gr3[0], "x(thresh.) - 190 V", "lp");
   l->AddEntry(gr4[0], "x(thresh.) - 200 V", "lp");
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      //gr[row]->Draw("ALP SAME");
      //gr1[row]->Draw("ALP SAME");
      //gr2[row]->Draw("ALP SAME");
      gr3[row]->Draw("ALP SAME");
      gr4[row]->Draw("LP SAME");
      l->Draw("SAME");
   }

}
