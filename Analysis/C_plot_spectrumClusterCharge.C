//###################################################################################################
//#   Plot the specta of charge of the cluster for each row and one for the sum of all the rows
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created november 2024 by D. Torresi
//#######################################
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_spectrumClusterCharge(int run)
{

 //###################################################################
//    VARIABLES

   double totalCharge;
   char histoname[20];
   int flagA=0;
   char anykey;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS
   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);  

   //TCanvas *C2=new TCanvas("c2","c2",750,160,800,600);   
   
   TH1D *chargeCluster[11];
   for(int h=0; h<11; h++){
      sprintf(histoname,"clch%i",h); 
      chargeCluster[h]=new TH1D(histoname,histoname,6410,-0.5,320099.5);
      chargeCluster[h]->GetXaxis()->SetTitle("charge");
      chargeCluster[h]->GetYaxis()->SetTitle("counts");
   }
 
   TH1F *HTotalCharge = new TH1F("toch","toch",6410,-0.5,320499.5);
   
     
   //TH2D *bg=new TH2D("","bg",6410,-0.5,320499.5,1000,0,10000);
   //bg->Draw();   
  
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries-1;i++){
      tree->GetEntry(i);
      cout<<"entry "<<i<<"  #";
      totalCharge=0;
      for(int j=0; j<5; j++){
         chargeCluster[j]->Fill(cl_charge[j]);  
         totalCharge=totalCharge+cl_charge[j];
         cout<<cl_charge[j]<<"  ";
      }
      cout<<endl;
      HTotalCharge->Fill(totalCharge);
          
   }
   
   
   HTotalCharge->Draw();
   HTotalCharge->SetLineColor(kBlack);
   
   chargeCluster[0]->SetLineColor(kRed);   
   chargeCluster[0]->Draw("same");
   chargeCluster[1]->Draw("same");
   chargeCluster[1]->SetLineColor(kGreen);
   chargeCluster[2]->Draw("same");
   chargeCluster[2]->SetLineColor(kBlue);
   chargeCluster[3]->Draw("same");
   chargeCluster[3]->SetLineColor(kViolet);
   chargeCluster[4]->Draw("same");
   chargeCluster[4]->SetLineColor(kOrange);

   
   
   
   
   
}   
   




