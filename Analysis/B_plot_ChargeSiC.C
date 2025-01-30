//###################################################################################################
//#   plot charge spectra for the SiC from the Sic file
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

void B_plot_ChargeSiC(int run)
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
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Row;

   // Apertura file
   char fileIn[50];
  
   if(run<10){
      sprintf(fileIn, "../Merged_data/run_00%i/sic_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileIn, "../Merged_data/run_0%i/sic_0%i.root", run, run);
   }else{
      sprintf(fileIn, "../Merged_data/run_%i/sic_%i.root", run, run);
   } 

  

   TFile *fin = new TFile(fileIn);
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

 
//#################################################################################################
// GRAPHICS
  
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *sicCharge=new TH1F("","",10000,0,20000);
   //sicCharge->SetStats(0);
   sicCharge->GetXaxis()->SetTitle("charge");
   sicCharge->GetYaxis()->SetTitle("counts");
   
   
//#################################################################################################
// Data LOOP

   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      tree->GetEntry(i);
      sicCharge->Fill(Charge);    
   }
   sicCharge->SetLineColor(kBlack);
   sicCharge->Draw();
   
 }
 
