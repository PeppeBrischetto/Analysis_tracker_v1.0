//###################################################################################################
//#   plot charge of the cluster for each row event by event
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

int C_plot_eventClusterCharge(int run)
{

//###################################################################
//    VARIABLES

   int flagA=0;
   char anykey;

//#################################################################################################
//OpenFile
   openTrackFile(run);
   
//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("charge");
   histoTheta->GetYaxis()->SetTitle("counts");

   TGraph * gr1=new TGraph();
   
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      cout<<"entry "<<i<<endl;
      
      for(int j=0; j<5; j++){
         gr1->SetPoint(j,j,cl_charge[j]);
         gr1->SetMarkerStyle(20);
         gr1->SetMarkerSize(1);
      }
      gr1->Draw("AP");
      C1->Update();
      cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl;
      //if(flagM==0)cin>>anykey;
      cin>>anykey;
      if(anykey=='q')return 0; // Per uscire dal programma
      //if(anykey=='s'){ 		 // Salvi il plot
         //C1->Print("c1.eps");
         //C3->Print("c3.eps");
      //}        
   	//if(anykey=='c')flagM=1;
                      
        
      cout<<"---------------"<<endl;
      
      //gr1->Reset("ICES");
   }
   
}   
   




