//###################################################################################################
//#   plot charge spectra for the SiC
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

void C_plot_ChargeSiC(int run)
{

//###################################################################
//    VARIABLES


//#################################################################################################
//OpenFile
   openTrackFile(run);
 
//#################################################################################################
// GRAPHICS
  
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *sicCharge=new TH1F("","",2000,0,20000);
   sicCharge->SetStats(0);
   sicCharge->GetXaxis()->SetTitle("charge");
   sicCharge->GetYaxis()->SetTitle("counts");
   
   TH1F *sicChargeNT=new TH1F("","",2000,0,20000);
   TH1F *sicChargeT=new TH1F("","",2000,0,20000);
   
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
   sicCharge->SetLineColor(kBlack);
   sicChargeT->SetLineColor(kRed);
   sicChargeNT->SetLineColor(kBlue);
   sicCharge->Draw();
   sicChargeT->Draw("SAME");
   sicChargeNT->Draw("SAME");
   
 }
 
