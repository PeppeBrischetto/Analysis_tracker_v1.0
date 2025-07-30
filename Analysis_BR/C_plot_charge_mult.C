//###################################################################################################
//#   Plot row charge as a function of multiplicity for integer multiplicity searches.
//#   If GraphicalCuts are needed, please be sure they are properly set before launch the macro
//#  (and rightly take them into account in the selection inside the for-cycle).
//#
//#   required as argument the run number
//#
//###################################################################################################
//#   created july 2025 by A. Pitronaci
//#######################################
//#   
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_charge_mult(int run)
{

//###################################################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
   char titleCanv[100];
   Double_t charge_err[5] = {0.};
   Int_t evtCounter = 0;
//###################################################################################################
//OpenFile
   openTrackFile(run);

//###################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,29,40);
   cutGli->SetPoint(1,137,171);
   cutGli->SetPoint(2,114,179);
   cutGli->SetPoint(3,21,48);
   cutGli->SetPoint(4,29,40);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,25,16);
   cutGa->SetPoint(1,195,207);
   cutGa->SetPoint(2,173,216);
   cutGa->SetPoint(3,13,22);
   cutGa->SetPoint(4,25,16);
   
//###################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   TCanvas *C2=new TCanvas("c2","c2",350,160,1350,1400);
   TCanvas *C2a=new TCanvas("c2a","c2a",350,160,1350,1400);      
   TCanvas *C3=new TCanvas("c3","c3",450,360,1100,600);  
   
   // all tracks
   TH1F *histoCharge=new TH1F("","",250,0,5e5);
   histoCharge->SetStats(0);
   histoCharge->GetXaxis()->SetTitle("charge");
   histoCharge->GetYaxis()->SetTitle("counts");
   histoCharge->SetNdivisions(7);
   
   TH1F *h_charge_M[30];
   for(int i=0; i<30; i++){
      sprintf(histoname,"mult %i",i);
      h_charge_M[i]=new TH1F("","",250,0,5e5);
      h_charge_M[i]->GetXaxis()->SetTitle("charge");
      h_charge_M[i]->GetXaxis()->SetTitleSize(0.05);
      h_charge_M[i]->GetXaxis()->SetLabelSize(0.05);
      h_charge_M[i]->GetXaxis()->SetTitleOffset(.9);
      h_charge_M[i]->GetYaxis()->SetTitle("Counts");
      h_charge_M[i]->GetYaxis()->SetTitleSize(0.05);
      h_charge_M[i]->GetYaxis()->SetLabelSize(0.05);
      h_charge_M[i]->GetYaxis()->SetTitleOffset(1.);
      h_charge_M[i]->SetNdivisions(7);
      if(i==5){
        h_charge_M[i]->SetLineColor(kYellow+1);
      }else if(i>=10){
                        h_charge_M[i]->SetLineColor(3*i);
                     }else{
                               h_charge_M[i]->SetLineColor(i);
                          }
      //h_charge_M[i]->GetXaxis()->SetRangeUser(55,75);
   }

//#################################################################################################
// Data LOOP
   for(int j=0; j<entries; j++){
      tree->GetEntry(j);
      // Fill the histo
      
      histoCharge->Fill(cl_charge[4]/*+cl_charge[1]+cl_charge[3]+cl_charge[3]+cl_charge[4]*/);
      
      for(int i=0; i<30; i++){
         if(cl_padMult[4]==i && cutGli->IsInside(cl_x_mm[0], cl_x_mm[1]) ){
            evtCounter += 1;
            h_charge_M[i]->Fill(cl_charge[4]);
         }
      }        
   }
   
   
   TGraphErrors *gr1=new TGraphErrors();
   gr1->GetXaxis()->SetTitle("pad multiplicity");
   gr1->GetYaxis()->SetTitle("charge (Ch)");
   gr1->SetMarkerColor(kGreen+2);
   gr1->SetMarkerStyle(53);
   gr1->SetMarkerSize(1);
   TGraphErrors *gr1_ls=new TGraphErrors();                                     // TGraph for low-statistics 
   gr1_ls->GetXaxis()->SetTitle("pad multiplicity");
   gr1_ls->GetYaxis()->SetTitle("charge (Ch)");  
   gr1_ls->SetMarkerStyle(53);
   gr1_ls->SetMarkerSize(1);
   gr1_ls->SetMarkerColor(kRed);
   gr1_ls->SetLineWidth(0);
   TLegend *l = new TLegend(0.6,0.2,0.9,0.3);
   l->AddEntry(gr1,"statistics > 5% of the total","p");
   l->AddEntry(gr1_ls,"statistics < 5% of the total","p");
   
   for(int i=0; i<30; i++){
      Double_t numEvts = 0;
      numEvts = h_charge_M[i]->GetEntries();
      charge_err[i] = h_charge_M[i]->GetStdDevError();
     
     if(numEvts/evtCounter<0.05){
        gr1_ls->SetPoint(i, i, h_charge_M[i]->GetMean());
        gr1_ls->SetPointError(i,0.,charge_err[i]);
     }else{
              gr1->SetPoint(i, i, h_charge_M[i]->GetMean());
              gr1->SetPointError(i, 0.,charge_err[i]);
           }
         
     cout<<i<<"  "<<h_charge_M[i]->GetMean()<<endl;
   }  
   
   C1->cd();
   //histoCharge->SetLineColor(kBlack);
   histoCharge->Draw();
   C1->SaveAs("../Pictures/run299/7Li/charge_vs_mult/charge_row4.png");
   
   C2->cd();
   C2->Divide(4,4);
   for(int i=0; i<16; i++){
     C2->cd(i+1);
     Double_t h_pos = 0.5*h_charge_M[i]->GetMaximum();
     sprintf(histoname,"Mult: %i",i);
     if(h_pos==0){
        h_pos=0.5;
     }
     TText *t = new TText(2.5e5,h_pos,histoname);
     t->SetTextSize(0.07);
     h_charge_M[i]->Draw();
     //h_charge_M[i]->Fit("gaus","","+",55,75);
     t->Draw("SAME");
   }
   C2->SaveAs("../Pictures/run299/7Li/charge_vs_mult/row4/charge_for_mult1.png");
   
   C2a->cd();
   C2a->Divide(4,4);
   for(int i=16; i<30; i++){
     C2a->cd(i-15);
     Double_t h_pos = 0.5*h_charge_M[i]->GetMaximum();
     if(h_pos==0){
        h_pos=0.5;
     }
     sprintf(histoname,"Mult: %i",i);
     TText *t = new TText(2.5e5,h_pos,histoname);
     t->SetTextSize(0.07);
     h_charge_M[i]->Draw();
     t->Draw("SAME");
   }  
   C2a->SaveAs("../Pictures/run299/7Li/charge_vs_mult/row4/charge_for_mult2.png");
   
   C3->cd();
   gr1_ls->Draw("AP");
   gr1->Draw("P SAME");
   l->Draw("SAME");
   C3->SaveAs("../Pictures/run299/7Li/charge_vs_mult/row4/ChargeVsMult_row4.png");
   
}  
   
   





