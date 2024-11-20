//###################################################################################################
//#   plot theta spectra of a single SiC
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

void C_plot_spectrumClusterCharge_v2(int run)
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
   
   TH1D *chargeCluster[50];
   for(int h=0; h<50; h++){
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
         if(a_pads_fired[j][0]>15 && a_pads_fired[j][0]<18){
            chargeCluster[j]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<20){
            chargeCluster[j+5]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<22){
            chargeCluster[j+10]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<24){
            chargeCluster[j+15]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<26){
            chargeCluster[j+20]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<28){
            chargeCluster[j+25]->Fill(cl_charge[j]);  
         }

         
         totalCharge=totalCharge+cl_charge[j];
         //cout<<cl_charge[j]<<"  ";
      }
      cout<<endl;
      HTotalCharge->Fill(totalCharge);
          
   }
   
   
   //HTotalCharge->Draw();
   HTotalCharge->SetLineColor(kBlack);
    
   int z=4;   // row number
   int a=0+z;
   int b=5+z;
   int c=10+z;
   int d=15+z;
   int e=20+z;
   int f=25+z;
   int g=30+z;

   chargeCluster[a]->Draw();
   chargeCluster[a]->SetLineColor(kRed);   
   chargeCluster[b]->Draw("same");
   chargeCluster[b]->SetLineColor(kGreen);
   chargeCluster[c]->Draw("same");
   chargeCluster[c]->SetLineColor(kBlue);
   chargeCluster[d]->Draw("same");
   chargeCluster[d]->SetLineColor(kViolet);
   chargeCluster[e]->Draw("same");
   chargeCluster[e]->SetLineColor(kOrange);
   chargeCluster[f]->Draw("same");
   chargeCluster[f]->SetLineColor(kRed +2);

   
   
   TLegend* l = new TLegend(0.6,0.8,0.9,0.9);
   l->SetTextSize(0.035);
   l->AddEntry(chargeCluster[a], "16-17", "l");
   l->AddEntry(chargeCluster[b], "18-20", "l");   
   l->AddEntry(chargeCluster[c], "20-22","l");
   l->AddEntry(chargeCluster[d], "22-24","l");
   l->AddEntry(chargeCluster[e], "24-26","l");
   l->AddEntry(chargeCluster[f], "26-28","l");
   l->Draw("same"); 
   
   
}   
   




