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
   Int_t stats[30] = {0};
   Double_t theta_err[30] = {0.};
   Int_t evtCounter = 0;
//###################################################################################################
//OpenFile
   openTrackFile(run);

//###################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,32,37);
   cutGli->SetPoint(1,133,160);
   cutGli->SetPoint(2,106,170);
   cutGli->SetPoint(3,17,47);
   cutGli->SetPoint(4,33,47);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,28,18);
   cutGa->SetPoint(1,186,203);
   cutGa->SetPoint(2,170,210);
   cutGa->SetPoint(3,17,26);
   cutGa->SetPoint(4,28,18);
   
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
      h_theta_M[i]=new TH1F("","",1000,45,70);
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
         if(cl_padMult[4]==i /*&& cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])*/ ){
            evtCounter += 1;
            h_theta_M[i]->Fill(theta_deg);
         }
      }        
   }
   
   TGraphErrors *gr1=new TGraphErrors();
   gr1->GetXaxis()->SetTitle("pad multiplicity");
   gr1->GetYaxis()->SetTitle("#vartheta (deg)");
   gr1->SetMarkerColor(kGreen+2);
   gr1->SetMarkerStyle(53);
   gr1->SetMarkerSize(1);
   TGraphErrors *gr1_ls=new TGraphErrors();                                     // TGraph for low-statistics 
   gr1_ls->GetXaxis()->SetTitle("pad multiplicity");
   gr1_ls->GetYaxis()->SetTitle("#vartheta (deg)");  
   gr1_ls->SetMarkerStyle(53);
   gr1_ls->SetMarkerSize(1);
   gr1_ls->SetMarkerColor(kRed);
   gr1_ls->SetLineWidth(0);
   TLegend *l = new TLegend(0.6,0.2,0.9,0.3);
   l->AddEntry(gr1,"statistics > 5 % of the total","p");
   l->AddEntry(gr1_ls,"statistics < 5 % of the total","p");
   
   
   for(int i=0; i<30; i++){
      Double_t numEvts = 0;
      numEvts = h_theta_M[i]->GetEntries();
      theta_err[i] = h_theta_M[i]->GetStdDevError();
     
     if(numEvts/evtCounter<0.05){
        gr1_ls->SetPoint(i, i, h_theta_M[i]->GetMean());
        gr1_ls->SetPointError(i,0.,theta_err[i]);
     }else{
              gr1->SetPoint(i, i, h_theta_M[i]->GetMean());
              gr1->SetPointError(i, 0.,theta_err[i]);
           }
         
     cout<<i<<"  "<<h_theta_M[i]->GetMean()<<endl;
   }   
   
   C1->cd();
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   C1->SaveAs("../Pictures/run41/theta_vs_mult/theta.png");
   
   C2->cd();
   C2->Divide(4,4);
   for(int i=0; i<16; i++){
     C2->cd(i+1);
     Double_t h_pos = 0.5*h_theta_M[i]->GetMaximum();
     if(h_pos==0){
        h_pos=0.5;
     }
     sprintf(histoname,"Mult: %i",i);
     TText *t = new TText(50.,h_pos,histoname);
     t->SetTextSize(0.07);
     h_theta_M[i]->Draw();
     //h_theta_M[i]->Fit("gaus","","+",55,75);
     t->Draw("SAME");
   }
   C2->SaveAs("../Pictures/run41/theta_vs_mult/row4/theta_for_mult1.png");
   
   C2a->cd();
   C2a->Divide(4,4);
   for(int i=16; i<30; i++){
     C2a->cd(i-15);
     Double_t h_pos = 0.5*h_theta_M[i]->GetMaximum();
     if(h_pos==0){
        h_pos=0.5;
     }
     sprintf(histoname,"Mult: %i",i);
     TLatex *t = new TText(50.,h_pos,histoname);
     t->SetTextSize(0.07);
     h_theta_M[i]->Draw();
     t->Draw("SAME");
   }  
   C2a->SaveAs("../Pictures/run41/theta_vs_mult/row4/theta_for_mult2.png");
   
   C3->cd();
   //gr1->GetYaxis()->SetRangeUser(45.,70.);
   //gr1_ls->GetYaxis()->SetRangeUser(45.,70.);
   gr1_ls->Draw("AP");
   gr1->Draw("P SAME");
   l->Draw("SAME");
   C3->SaveAs("../Pictures/run41/theta_vs_mult/row4/ThetaVsMult_row4.png");
   
}  
   
   





