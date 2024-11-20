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

void C_plot_X_V2(int run)
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

   TCanvas *C1=new TCanvas("c1","c1",250,160,1600,600);   
   
   // all tracks
   TH1D *XposH[5];
   TH1D *XposL[5];
   for(int h=0; h<5; h++){
      sprintf(histoname,"xpos%i",h); 
      //Xpos[h]=new TH1D(histoname,histoname,1200,-50.5,349.5);
      XposH[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      XposH[h]->GetXaxis()->SetTitle("x (pad)");
      XposH[h]->GetYaxis()->SetTitle("counts");
      
      XposL[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      XposL[h]->GetXaxis()->SetTitle("x (pad)");
      XposL[h]->GetYaxis()->SetTitle("counts");
   }


   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      flag=0;
      
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      
      if(cl_padMult[0]>3 && cl_padMult[1]>3 &&cl_padMult[2]>3 &&cl_padMult[3]>3 &&cl_padMult[4]>3) flag=1;
      if(cl_padMult[0]<3 && cl_padMult[1]<3 &&cl_padMult[2]<3 &&cl_padMult[3]<3 &&cl_padMult[4]<3) flag=2;
      
      
      for(int h=0; h<5; h++){
         
         if(flag==1) XposH[h]->Fill(cl_x[h]);
         if(flag==2) XposL[h]->Fill(cl_x[h]);
      }
     
     
   }
   cout<<endl;
   
   TCanvas *c1=new TCanvas("c1","High mult");
   XposH[0]->SetLineColor(kBlack);
   XposH[0]->Draw();
   XposH[1]->SetLineColor(kRed);
   XposH[1]->Draw("same");
   XposH[2]->SetLineColor(kGreen);
   XposH[2]->Draw("same");
   XposH[3]->SetLineColor(kBlue);
   XposH[3]->Draw("same");
   XposH[4]->SetLineColor(kViolet);
   XposH[4]->Draw("same");
   

   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   leg->AddEntry(XposH[0], " row 0", "l"); 
   leg->AddEntry(XposH[1], " row 1", "l");
   leg->AddEntry(XposH[2], " row 2", "l");
   leg->AddEntry(XposH[3], " row 3", "l");
   leg->AddEntry(XposH[4], " row 4", "l");
   leg->Draw();

   TCanvas *c2=new TCanvas("c2","Low mult");
   XposL[0]->SetLineColor(kBlack);
   XposL[0]->Draw();
   XposL[1]->SetLineColor(kRed);
   XposL[1]->Draw("same");
   XposL[2]->SetLineColor(kGreen);
   XposL[2]->Draw("same");
   XposL[3]->SetLineColor(kBlue);
   XposL[3]->Draw("same");
   XposL[4]->SetLineColor(kViolet);
   XposL[4]->Draw("same");
   leg->Draw();
 }  
   





