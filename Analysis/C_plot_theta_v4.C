//###################################################################################################
//#   plot theta spectra
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated: November 2024 by D. Torresi
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_theta_v4(int run)
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
   histoTheta->GetXaxis()->SetTitle("theta");
   histoTheta->GetYaxis()->SetTitle("counts");
   TH1F *histoTheta2=new TH1F("","",1000,-10,90);
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      // Fill the histo
      histoTheta->Fill(theta_deg);
      if(energySic<3000 && (cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4])>300){ histoTheta2->Fill(theta_deg);}
   }
   
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   histoTheta2->SetLineColor(kRed);
   histoTheta2->Draw("same");

}  
   
   





