//###################################################################################################
//#   Plot row theta as a function of multiplicity for integer multiplicity searches.
//#   If GraphicalCuts are needed, please be sure they are properly set before launch the macro
//#  (and rightly take them into account in the selection inside the for-cycle).
//#
//#   required as argument the run number
//#
//###################################################################################################
//#   created march 2025 by A. Pitronaci
//#######################################
//#   
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_mult_theta(int run)
{

//###################################################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
   char titleCanv[100];
//###################################################################################################
//OpenFile
   openTrackFile(run);

//###################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,30,39);
   cutGli->SetPoint(1,129,171);
   cutGli->SetPoint(2,99,176);
   cutGli->SetPoint(3,22,53);
   cutGli->SetPoint(4,30,39);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,29,17);
   cutGa->SetPoint(1,187,201);
   cutGa->SetPoint(2,168,208);
   cutGa->SetPoint(3,17,24);
   cutGa->SetPoint(4,29,17);
   
//###################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   TCanvas *C2=new TCanvas("c2","c2",350,160,1350,1400);
   TCanvas *C2a=new TCanvas("c2a","c2a",350,160,1350,1400);      
   TCanvas *C3=new TCanvas("c3","c3",450,360,800,600);  
   
   // all tracks
   TH1F *histoTheta=new TH1F("","",1000,-10,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("#vartheta");
   histoTheta->GetYaxis()->SetTitle("counts");
   
   TH1F *h_theta_M[30];
   for(int i=0; i<30; i++){
      sprintf(histoname,"mult %i",i);
      h_theta_M[i]=new TH1F("","",1000,10,80);
      h_theta_M[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta_M[i]->GetXaxis()->SetTitleSize(0.05);
      h_theta_M[i]->GetXaxis()->SetLabelSize(0.05);
      h_theta_M[i]->GetXaxis()->SetTitleOffset(.9);
      h_theta_M[i]->GetYaxis()->SetTitle("Counts");
      h_theta_M[i]->GetYaxis()->SetTitleSize(0.05);
      h_theta_M[i]->GetYaxis()->SetLabelSize(0.05);
      h_theta_M[i]->GetYaxis()->SetTitleOffset(1.);
      if(i==5){
        h_theta_M[i]->SetLineColor(kYellow+1);
      }else if(i>=10){
                        h_theta_M[i]->SetLineColor(3*i);
                     }else{
                               h_theta_M[i]->SetLineColor(i);
                          }
      //h_theta_M[i]->GetXaxis()->SetRangeUser(55,75);
   }

//#################################################################################################
// Data LOOP
   for(int j=0; j<entries; j++){
      tree->GetEntry(j);
      // Fill the histo
      
      histoTheta->Fill(theta_deg);
      
      for(int i=0; i<30; i++){
         if(cl_padMult[0]==i && cutGli->IsInside(cl_x_mm[0], cl_x_mm[1]) ){
            h_theta_M[i]->Fill(theta_deg);
         }
      }        
   }
   
   
   TGraph *gr1=new TGraph();
   gr1->GetXaxis()->SetTitle("multiplicity");
   gr1->GetYaxis()->SetTitle("theta");  
   
   for(int i=0; i<30; i++){
     gr1->SetPoint(i, i, h_theta_M[i]->GetMean());
     cout<<i<<"  "<<h_theta_M[i]->GetMean()<<endl;
   }   
   
   C1->cd();
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   C1->SaveAs("../Pictures/run278/7Li/theta_vs_mult/theta.png");
   
   C2->cd();
   C2->Divide(4,4);
   for(int i=0; i<16; i++){
     C2->cd(i+1);
     sprintf(histoname,"Mult: %i",i);
     TText *t = new TText(70.,0.,histoname);
     h_theta_M[i]->Draw();
     //h_theta_M[i]->Fit("gaus","","+",55,75);
     t->Draw("SAME");
   }
   C2->SaveAs("../Pictures/run278/7Li/theta_vs_mult/row0/theta_for_mult1.png");
   
   C2a->cd();
   C2a->Divide(4,4);
   for(int i=16; i<30; i++){
     C2a->cd(i-15);
     sprintf(histoname,"Mult: %i",i);
     TText *t = new TText(70.,0.,histoname);
     h_theta_M[i]->Draw();
     t->Draw("SAME");
   }  
   C2a->SaveAs("../Pictures/run278/7Li/theta_vs_mult/row0/theta_for_mult2.png");
   
   C3->cd();
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerSize(1);
   gr1->Draw("AP");
   C3->SaveAs("../Pictures/run278/7Li/theta_vs_mult/row0/ThetaVsMult_row0.png");
   
}  
   
   





