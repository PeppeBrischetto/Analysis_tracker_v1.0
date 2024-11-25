//###################################################################################################
//#   plot spectra of pad multiplicity of clusters for each row
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

void C_plot_clusterMolt(int run)
{

//###################################################################
//    VARIABLES
  
   char histoname[100];
   int flagA=0;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   
   // all tracks
   TH1F *histo[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      histo[i]=new TH1F("","",20,-0.5,19.5);
      histo[i]->GetXaxis()->SetTitle("charge");
      histo[i]->GetYaxis()->SetTitle("counts");
   }
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      // Fill the histo
      for(int h=0; h<5; h++){
         histo[h]->Fill(cl_padMult[h]);        
      }
   }
   
   histo[0]->SetLineColor(kBlack);
   histo[0]->Draw();
   histo[1]->SetLineColor(kRed);
   histo[1]->Draw("same");
   histo[2]->SetLineColor(kBlue);
   histo[2]->Draw("same");
   histo[3]->SetLineColor(kGreen);
   histo[3]->Draw("same");
   histo[4]->SetLineColor(kViolet);
   histo[4]->Draw("same");



}  
   
   





