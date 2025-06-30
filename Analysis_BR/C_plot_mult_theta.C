//###################################################################################################
//#   This macro allows to plot the measured angle theta as a function of the multiplicity
//#
//#   required as argument the run number
//#
//###################################################################################################
//#   Created on 30 June 2025 by A. Pitronaci
//#   On ref. C_plot_theta_v6.C by D. Torresi
//#######################################
//#   
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_mult_theta(int run)
{

//###################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
   
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("theta");
   histoTheta->GetYaxis()->SetTitle("counts");
   
   TH1F *h_theta_M[20];
   for(int i=0; i<20; i++){
      sprintf(histoname,"mult %i",i);
      h_theta_M[i]=new TH1F("","",1000,10,80);
      h_theta_M[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta_M[i]->SetLineColor(i);
   }
   
   TGraph *gr1[5];
   for(Int_t r=0; r<5; r++){
      sprintf(histoname,"mult. Row%d",r);
      gr1[r]=new TGraph();
      gr1[r]->GetXaxis()->SetTitle(histoname);
      gr1[r]->GetYaxis()->SetTitle("#vartheta (deg)");  
      gr1[r]->SetMarkerStyle(20);
      gr1[r]->SetLineWidth(0);
   }
   
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",4);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,20.6,47.49);
   cutGli->SetPoint(1,31.97,41.53);
   cutGli->SetPoint(2,137.61,171.42);
   cutGli->SetPoint(3,115.67,177.38);
   
   TCutG *cutGa = new TCutG("cutGa",4);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,16.13,23.67);
   cutGa->SetPoint(1,28.72,15.48);
   cutGa->SetPoint(2,192.86,208.27);
   cutGa->SetPoint(3,174.17,213.48);

//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      // Fill the histo
      
      
      histoTheta->Fill(theta_deg);
      
      for(Int_t i=2; i<20; i++){
        for(Int_t r=0; r<5; r++){
           if(cl_padMult[r]==i && cutGli->IsInside(cl_x_mm[0], cl_x_mm[1]) ){
            h_theta_M[i-2]->Fill(theta_deg);
         }
         gr1[r]->SetPoint(i-2, i, h_theta_M[i-2]->GetMean());
        }
        //cout<<i<<"  "<<h_theta_M[i-2]->GetMean()<<endl;
      }        
   }

//#################################################################################################
// Visualization block

   TCanvas *c1 = new TCanvas("c1","c1",450,360,800,600);
   c1->Divide(3,2);
   for(Int_t r=1; r<6; r++){
      c1->cd(r);
      gr1[r-1]->Draw();
   }
   
   c1->SaveAs("Pictures_Analysis/Mult_vs_theta/Mult_vs_theta_7Li_Run299.png");
   c1->SaveAs("Pictures_Analysis/Mult_vs_theta/Mult_vs_theta_7Li_Run299.eps");
   }
   
