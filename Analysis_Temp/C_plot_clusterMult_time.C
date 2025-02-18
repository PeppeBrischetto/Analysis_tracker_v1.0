//###################################################################################################
//#   plot spectra of pad multiplicity of clusters for each row
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by A. Pitronaci
//#######################################
//#   updated: 
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_clusterMult_time(int run)
{

//###################################################################
//    VARIABLES
  
   char histoname[100];
   int flagA=0;
   int n_tracks = 20;  // rate of tracks                              
   double average_mult[5] = {0};
   int n_point = 0;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *c1=new TCanvas("c1","c1",250,160,800,600);   
   
   TGraph *gr[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr[i]=new TGraph();
      gr[i]->GetXaxis()->SetTitle("time");
      gr[i]->GetYaxis()->SetTitle("average_mult");
   }

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
//         histo[h]->Fill(cl_padMult[h]);
         average_mult[h] = average_mult[h] + cl_padMult[h];        
      }
      
      if(i%n_tracks==0){
         for(int l=0;l<5;l++){
           average_mult[l] = average_mult[l]/n_tracks;
           gr[l]->SetPoint(n_point,n_point,average_mult[l]);
           //cout << "aveMult0: " << average_mult[l] << endl;
           average_mult[l]=0;
         }
       n_point ++;
        }
   }
   
   //histo[0]->SetLineColor(kBlack);
   //histo[0]->Draw();
   //histo[1]->SetLineColor(kRed);
   //histo[1]->Draw("same");
   //histo[2]->SetLineColor(kBlue);
   //histo[2]->Draw("same");
   //histo[3]->SetLineColor(kGreen);
   //histo[3]->Draw("same");
   //histo[4]->SetLineColor(kViolet);
   //histo[4]->Draw("same");
   
   gr[0]->Draw("AP");
   for(int l=0;l<5;l++){
     gr[l]->SetMarkerStyle(20);
     gr[l]->SetMarkerSize(1);
     gr[l]->SetMarkerColor(l+1);
     gr[l]->GetYaxis()->SetRangeUser(0,11);
     gr[l]->Draw("P");
     }

   c1->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run296/averageMult_time.png");



}  
   
   





