//#################################################################################################################
//#   This macro allows to evaluate the error on the x^corr_{i} having previously corrected the systematic offset.
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
#include <TMath.h>
#include "TPrincipal.h"
#include "TTree.h"
#include "../Include/openfilesthresh.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void systematicOffset_corr_slit(int run){

//################################################################################################################
// Variables

   char tasto[10]; 
   char titolofile[100];
   char histoname[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   Double_t chi_root = 0.;
   Double_t my_chi = 0.;
   Double_t my_chiRed = 0.;
   Double_t Ndof = 3;
   Double_t offset[NRows] = {-0.512562,1.99404,-0.637832,-2.32577,1.48162};
   Double_t x_corr[NRows] = {0.};
   Double_t newOffset[NRows] = {0.};
   Double_t chiRed_min1 = 0.;
   Double_t chiRed_tot = 0.;
   Double_t rapp = 0.;
   Double_t pearson = 0.;
   Double_t m = 0.;
   Double_t q = 0.;
   Double_t errX[NRows] = {0.};
   Double_t errM = 0.;
   Double_t errQ = 0.;
   Double_t mean_errX[NRows] = {0.};
   Double_t x_foc = 0.;
   
   Double_t theta_deg = 0.;
   Double_t intercetta = 0.;
   Double_t coeffAng = 0.;
   
   ofstream outfile;
   
   char provv;

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *xCorr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"xCorr %i",i);
      xCorr[i]=new TH1D("","",3000.,-0.5,299.5);
      xCorr[i]->GetXaxis()->SetTitle("x_{i} (mm)");
      xCorr[i]->GetXaxis()->SetTitleSize(0.05);
      xCorr[i]->GetXaxis()->SetLabelSize(0.05);
      xCorr[i]->GetXaxis()->SetTitleOffset(1.);
      xCorr[i]->GetYaxis()->SetTitle("Counts");
      xCorr[i]->GetYaxis()->SetTitleSize(0.05);
      xCorr[i]->GetYaxis()->SetLabelSize(0.05);
      xCorr[i]->GetYaxis()->SetTitleOffset(1.);
      xCorr[i]->SetNdivisions(7);
      xCorr[i]->SetFillColor(kBlue-5);
   }
   
   TF1 *norm = new TF1("norm","gaus",-5,5);
   norm->SetParameters(1,0,1);
   
   ofstream outputfile;
   TGraph *retta = new TGraph(5);
   retta->SetTitle("(x_{i},z_{i})-pairs - track reconstruction");
   retta->SetMarkerStyle(20);
   retta->SetLineWidth(0);
   retta->GetXaxis()->SetTitle("x (mm)");
   retta->GetYaxis()->SetTitle("z (mm)");
   
   char f_outName[50];
   if(run<10){
         sprintf(f_outName,"CorrectedX/xCorr_run00%i_th.root",run);
      }else if(run <100){
         sprintf(f_outName,"CorrectedX/xCorr_run0%i_th.root",run);
      }else{
         sprintf(f_outName,"CorrectedX/xCorr_run%i_th.root",run);
      } 
   TFile *f_out = new TFile(f_outName, "recreate");
   TTree *mytree = new TTree("Data_cor", "Offset_Correction");
   
   mytree->Branch("x_corr0", &x_corr[0], "x_corr0/D");
   mytree->Branch("x_corr1", &x_corr[1], "x_corr1/D");
   mytree->Branch("x_corr2", &x_corr[2], "x_corr2/D");
   mytree->Branch("x_corr3", &x_corr[3], "x_corr3/D");
   mytree->Branch("x_corr4", &x_corr[4], "x_corr4/D");
   
//################################################################################################################
// OpenFiles
   openTrackFileTh(run);
   tree->Print();
   
   //sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d_4He.txt",run);
   //outputfile.open(titolofile);
   //outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

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
// Data loop
   for(Int_t i=0; i<entries; i++){
      my_chi = 0.;
      chi_root = 0.;
      my_chiRed = 0.;
      pearson = 0.;
      errM = 0.;
      errQ = 0.;
      x_foc = 0.;
      Int_t o = 0;
      
      Double_t theta_fit = 0.;
      Double_t pad[NRows][100] = {0.};
      Double_t charge[NRows][100] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t y[NRows] = {0.};
      Double_t totalCharge[NRows] = {0.};

      tree->GetEntry(i);
      retta->Set(0);

      if(sic_fired==1 && energySic>2000){
      if(/*theta_deg>=0 && theta_deg<20 &&*/ cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
      o++;
      for(Int_t row = 0; row < NRows; row++){
         x_corr[row] = 0.;
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            anode->Fill(pad[row][p], 2 * row + 1, charge[row][p]);
            x_mm[p] = 2.5 + (5 * pad[row][p]);
            x[row] += (x_mm[p] * charge[row][p]);
         }
         Double_t my_Singlechi = 0.;
         x[row] = x[row] / (totalCharge[row]);
         x_corr[row] = x[row]-offset[row];
         retta->AddPoint(x_corr[row], z[row]);
         xCorr[row]->Fill(x_corr[row]);
      }
      anode->Reset("ICES");
      cout << "Ho riempito il tree per la " << o <<"-esima volta" << endl;
      
      mytree->Fill();
   }                                                                           // TCutG parenthesis
   }                                                                           // SiC_fired==1 & energySic>2000 - cut to select good events
   }                                                                           // Data-loop parenthesis
   
   Int_t effEvt = mytree->GetEntries();
   cout << "Totale eventi del tree corretto: " << effEvt << endl;
   f_out->cd();
   mytree->Write();
   TCanvas *c = new TCanvas("c","c",1600,1600);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      Double_t maxbin = xCorr[row]->GetMaximumBin();
      //xCorr[row]->GetXaxis()->SetRangeUser(maxbin-10,maxbin+10);
      xCorr[row]->Draw();
   }
   
//#################################################################################################
//  Application single slit on ROW4
   
   TF1 *f_gaus = new TF1("f_gaus","gaus",0,300);
   f_gaus->SetParameters(0,0,0);
   xCorr[4]->Fit(f_gaus,"","+",0,300);
   Double_t xRow4 = f_gaus->GetParameter(1);
   cout << xRow4 << endl;
   
   /*TCanvas *c1 = new TCanvas("c1","c1",1600,1600);
   c1->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c1->cd(row+1);
      mytree->Draw("x_corr[row]","x_corr[4]-xRow4<0.2");
   }*/
   
   
   f_out->Close();
   
}
