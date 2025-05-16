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

void C_plot_theta(int run)
{

//###################################################################
//    VARIABLES
  
   int flagA=0;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   int a=0;
     int b=0;

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("charge");
   histoTheta->GetYaxis()->SetTitle("counts");
   
   TH1F *h_theta_geq2000=new TH1F("","",1000,-10,90);
   TH1F *h_theta_rms=new TH1F("","",1000,-10,90);
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      // Fill the histo
      
      if(cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
        histoTheta->Fill(-theta_deg);
        a++;
      }
      
      
      if(energySic>2000){
         h_theta_geq2000->Fill(-theta_deg);
      }
        
      if(energySic>2000 && cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
         h_theta_rms->Fill(-theta_deg);
         b++;
         //sicCounts++;
      }
    
      
   }
   
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   h_theta_rms->SetLineColor(kRed);
   h_theta_rms->Draw("same");
   
   
   cout<<a<<"  "<<b<<"  "<<(double)a/b<<endl;
}  
   
   





