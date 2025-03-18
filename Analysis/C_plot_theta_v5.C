//###################################################################################################
//#   plot theta spectra specific for run 332 shows the problem present at high multiplicity of the tracks
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

void C_plot_theta_v5(int run)
{

//###################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",8);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,13,65);
   cutGli->SetPoint(1,9,42);
   cutGli->SetPoint(2,10,39);
   cutGli->SetPoint(3,25,50);
   cutGli->SetPoint(4,54,94);
   cutGli->SetPoint(5,69,126);
   cutGli->SetPoint(6,36,142);
   cutGli->SetPoint(7,13,65);
   cutGli->SetPoint(8,13,65);
   
   TCutG *cutGa = new TCutG("cutGa",8);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,17,40);
   cutGa->SetPoint(1,8,28);
   cutGa->SetPoint(2,8,9);
   cutGa->SetPoint(3,39,30);
   cutGa->SetPoint(4,182,193);
   cutGa->SetPoint(5,154,214);
   cutGa->SetPoint(6,55,85);
   cutGa->SetPoint(7,17,40);


//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   TCanvas *C2=new TCanvas("c2","c2",350,260,800,1400);   
   TCanvas *C3=new TCanvas("c3","c3",450,360,800,600);  
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("theta");
   histoTheta->GetYaxis()->SetTitle("counts");
   
   TH1F *h_theta_M[15];
   for(int i=0; i<10; i++){
      sprintf(histoname,"mult %i",i);
      h_theta_M[i]=new TH1F("","",1000,10,70);
      h_theta_M[i]->GetXaxis()->SetTitle("theta (deg)");
      h_theta_M[i]->SetLineColor(i);
   }
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      // Fill the histo
      
      
      histoTheta->Fill(theta_deg);
      
      for(int i=2; i<13; i++){
         if(cl_padMult[3]==i && cutGa->IsInside(cl_x_mm[0], cl_x_mm[1]) ){
            h_theta_M[i-2]->Fill(theta13_deg);
         }
      }        
   }
   
   
   TGraph *gr1=new TGraph();
   gr1->GetXaxis()->SetTitle("multiplicity");
   gr1->GetYaxis()->SetTitle("theta");  
   
   for(int i=0; i<10; i++){
     gr1->SetPoint(i, i+2, h_theta_M[i]->GetMean());
     cout<<i+2<<"  "<<h_theta_M[i]->GetMean()<<endl;
   }   
   
   C1->cd();
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   
   
   C2->cd();
   C2->Divide(1,7);
   h_theta_M[0]->Draw("same");
   for(int i=0; i<7; i++){
     C2->cd(i);
     h_theta_M[i]->Draw("same");
   }
   
   C3->cd();
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerSize(1);
   gr1->Draw("AP");
   
   
}  
   
   





