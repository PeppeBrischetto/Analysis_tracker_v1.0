//###################################################################################################
//#   plot difference between theta calculated using different rows
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created november 2024 by D. Torresi
//#######################################
//#
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_thetaDiff(int run)
{

  //###################################################################
//    VARIABLES

   int flagA=0;
   int flag=0;
   char histoname[100];
   
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,10);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("theta (deg)");
   histoTheta->GetYaxis()->SetTitle("counts");
   TH1F *histoTheta1=new TH1F("","",1000,-10,10);
   //histoTheta1->SetStats(0);
   histoTheta1->GetXaxis()->SetTitle("theta");
   histoTheta1->GetYaxis()->SetTitle("counts");
   TH1F *histoTheta2=new TH1F("","",1000,-10.01,10.01);
   histoTheta2->SetStats(0);
   histoTheta2->GetXaxis()->SetTitle("theta");
   histoTheta2->GetYaxis()->SetTitle("counts");   

   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
  
    
      // Fill the histo
      histoTheta->Fill(theta_deg-theta123_deg);
      histoTheta1->Fill(theta_deg-theta04_deg);
      histoTheta2->Fill(theta_deg-theta024_deg);
     
     
   }
   
   histoTheta1->SetLineColor(kBlack);
   histoTheta1->Draw();
   histoTheta->SetLineColor(kRed);
   histoTheta->Draw("same");
   histoTheta2->SetLineColor(kGreen);
   histoTheta2->Draw("same");
   
   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   leg->AddEntry(histoTheta, "01234 - 123", "l"); 
   leg->AddEntry(histoTheta1, "01234 - 04", "l"); 
   leg->AddEntry(histoTheta2, "01234 - 024", "l"); 
   leg->Draw();
} 





