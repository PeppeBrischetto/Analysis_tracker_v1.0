//###################################################################################################
//#   plot charge spectra for the SiC from the Sic file, compare the spectra coming from two files
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created December 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void B_plot_ChargeSiC_V2(int run1, int run2)
{

//###################################################################
//    VARIABLES


//#################################################################################################
//OpenFile
   UShort_t Channel; 
   UShort_t pad; 
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Charge;
   UShort_t Charge2;
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Row;

   // Apertura file
   char fileIn1[50],fileIn2[50];
  
   if(run1<10){
      sprintf(fileIn1, "../Merged_data/run_00%i/sic_00%i.root", run1, run1);
   }else if(run1 <100){
      sprintf(fileIn1, "../Merged_data/run_0%i/sic_0%i.root", run1, run1);
   }else{
      sprintf(fileIn1, "../Merged_data/run_%i/sic_%i.root", run1, run1);
   } 

   if(run2<10){
      sprintf(fileIn2, "../Merged_data/run_00%i/sic_00%i.root", run2, run2);
   }else if(run2 <100){
      sprintf(fileIn2, "../Merged_data/run_0%i/sic_0%i.root", run2, run2);
   }else{
      sprintf(fileIn2, "../Merged_data/run_%i/sic_%i.root", run2, run2);
   } 

   TFile *fin1 = new TFile(fileIn1);
   TTree *tree1 = (TTree*)fin1->Get("Data_R");

   tree1->SetBranchAddress("Board",&Board);
   tree1->SetBranchAddress("Channel",&Channel);
   tree1->SetBranchAddress("FineTSInt",&FTS);
   tree1->SetBranchAddress("CoarseTSInt",&CTS);
   tree1->SetBranchAddress("Timestamp",&Timestamp);
   tree1->SetBranchAddress("Charge",&Charge);
   tree1->SetBranchAddress("Flags",&Flags);
   tree1->SetBranchAddress("Pads",&pad);
   tree1->SetBranchAddress("Charge_cal",&Charge_cal);
   
   int entries1=tree1->GetEntries();
   cout<<" "<<entries1<<endl;

   TFile *fin2 = new TFile(fileIn2);
   TTree *tree2 = (TTree*)fin2->Get("Data_R");

   tree2->SetBranchAddress("Charge",&Charge2);
   
   int entries2=tree2->GetEntries();
   cout<<" "<<entries2<<endl;

 
//#################################################################################################
// GRAPHICS
  
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *sicCharge=new TH1F("","",10000,0,20000);
   //sicCharge->SetStats(0);
   sicCharge->GetXaxis()->SetTitle("charge");
   sicCharge->GetYaxis()->SetTitle("counts");
   TH1F *sicCharge2=new TH1F("","",10000,0,20000);
   
//#################################################################################################
// Data LOOP

   for(int i=0; i<entries1; i++){
   //for(int i=0; i<50; i++){
      tree1->GetEntry(i);
      sicCharge->Fill(Charge);    
   }
   for(int i=0; i<entries2; i++){
      tree2->GetEntry(i);
      sicCharge2->Fill(Charge2);    
   }

   sicCharge->SetLineColor(kBlack);
   sicCharge2->SetLineColor(kRed);   
   sicCharge->Draw();
   sicCharge2->Draw("same");  
 }
 
 
