//###################################################################################################
//#   plot theta spectra for different way to build theta
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created November 2024 by D. Torresi
//#######################################
//#   updated: 
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_theta_v3(int run)
{

 //###################################################################
//    VARIABLES

   int flagA=0;

//#################################################################################################
//OpenFile
   openTrackFile(run);
   
//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("#theta");
   histoTheta->GetYaxis()->SetTitle("counts");
   TH1F *histoTheta123=new TH1F("","",1000,-10,90);
   TH1F *histoTheta13=new TH1F("","",1000,-10,90);
   TH1F *histoTheta04=new TH1F("","",1000,-10,90);
   TH1F *histoTheta024=new TH1F("","",1000,-10,90);            

   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
  
    
      // Fill the histo
      
      histoTheta->Fill(theta_deg);
      
      histoTheta123->Fill(theta123_deg);
      histoTheta13->Fill(theta13_deg);
      histoTheta04->Fill(theta04_deg);
      histoTheta024->Fill(theta024_deg);
     
     
   }
   
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   histoTheta123->SetLineColor(kRed);
   //histoTheta123->Draw("same");
   histoTheta13->SetLineColor(kGreen);
   histoTheta13->Draw("same");
   histoTheta04->SetLineColor(kViolet);
   histoTheta04->Draw("same");
   histoTheta024->SetLineColor(kOrange);
   //histoTheta024->Draw("same");
   
   
}


