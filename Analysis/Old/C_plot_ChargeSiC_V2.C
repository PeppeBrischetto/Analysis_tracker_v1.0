//###################################################################################################
//#   plot charge spectra for the SiC compare the spectra coming from two runs
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_ChargeSiC_V2(int run1, int run2)
{

//###################################################################
//    VARIABLES

  int sic_fired2, sic_charge2, entries2;
//#################################################################################################
//OpenFile 1
   openTrackFile(run1);
 
//Open file 2
   char *filename2 = new char[100];
   if(run2<10){
      sprintf(filename2, "../Tracks/tracks_run00%i.root", run2);
   }else if(run2 <100){
      sprintf(filename2, "../Tracks/tracks_run0%i.root", run2);
   }else{
      sprintf(filename2, "../Tracks/tracks_run%i.root", run2);
   } 
   TFile *f2=new TFile(filename2,"READ");
   TTree *tree2 = (TTree*)f2->Get("Data_R");
   tree->SetBranchAddress("sic_fired",&sic_fired2);
   tree->SetBranchAddress("sic_charge",&sic_charge2);
   entries2=tree2->GetEntries();
//#################################################################################################
// GRAPHICS
  
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *sicCharge=new TH1F("","",2000,0,20000);
   //sicCharge->SetStats(0);
   sicCharge->GetXaxis()->SetTitle("charge");
   sicCharge->GetYaxis()->SetTitle("counts");
   
   TH1F *sicChargeNT=new TH1F("","",2000,0,20000);
   TH1F *sicChargeT=new TH1F("","",2000,0,20000);
   
   TH1F *sicCharge2=new TH1F("","",2000,0,20000);
   TH1F *sicCharge2NT=new TH1F("","",2000,0,20000);
   TH1F *sicCharge2T=new TH1F("","",2000,0,20000);
   
//#################################################################################################
// Data LOOP

   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      tree->GetEntry(i);
      sicCharge->Fill(sic_charge);
      if(sic_fired){
         sicChargeT->Fill(sic_charge);
      }else{
         sicChargeNT->Fill(sic_charge);
      }
      
   }
   
   
   for(int i=0; i<entries2; i++){
   //for(int i=0; i<50; i++){
      tree2->GetEntry(i);
      sicCharge2->Fill(sic_charge2);
      if(sic_fired){
         sicCharge2T->Fill(sic_charge2);
      }else{
         sicCharge2NT->Fill(sic_charge2);
      }
      
   }
   
   
   sicCharge->SetLineColor(kBlack);
   sicCharge2->SetLineColor(kRed);
   //sicChargeT->SetLineColor(kRed);
   //sicChargeNT->SetLineColor(kBlue);
   sicCharge->Draw();
   sicCharge2->Draw("SAME");
   //sicChargeT->Draw("SAME");
   //sicChargeNT->Draw("SAME");
   
 }
 
 
