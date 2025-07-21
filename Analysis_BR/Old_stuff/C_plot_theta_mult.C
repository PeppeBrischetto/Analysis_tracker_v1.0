//###########################################################################################################
//#   Plot row multiplicty as a function of theta in [a,b], where a,b are extremes for multiplicity search.
//#   If GraphicalCuts are needed, please be sure they are properly set before launch the macro
//#  (and rightly take them into account in the selection inside hte for-cycle).
//#
//#   required as argument the run number
//#
//###########################################################################################################
//#   created july 2025 by A. Pitronaci
//#######################################
//#   
//# 
//###########################################################################################################

#include "../Include/openfiles.h"

void C_plot_theta_mult(int run)
{

//###########################################################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
   char titleCanv[100];
   Double_t x[5] = {0.};
   Double_t y[5] = {0.};
//###########################################################################################################
//OpenFile
   openTrackFile(run);

//###########################################################################################################
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

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",350,160,1350,1400);   
   TCanvas *C2=new TCanvas("c2","c2",350,160,1350,1400);
   TCanvas *C3=new TCanvas("c3","c3",450,360,800,600);  
   
   // all tracks
   TH1F *h_mult[6];
   for(Int_t i=0; i<6; i++){
      sprintf(histoname,"mult row %d",i);
      h_mult[i]=new TH1F("","",60,-0.5,59.5);
      h_mult[i]->GetXaxis()->SetTitle("Multiplicity");
      h_mult[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult[i]->GetYaxis()->SetTitle("counts");
      h_mult[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult[i]->GetYaxis()->SetTitleOffset(1.1);
      h_mult[i]->SetLineColor(kGreen+2);
   }
   
   TH1F *h_theta_T[6];
   for(int i=0; i<6; i++){
      sprintf(histoname,"mult %i",i);
      h_theta_T[i]=new TH1F("","",1000,30,80);
      h_theta_T[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta_T[i]->GetXaxis()->SetTitleSize(0.05);
      h_theta_T[i]->GetXaxis()->SetLabelSize(0.05);
      h_theta_T[i]->GetXaxis()->SetTitleOffset(.9);
      h_theta_T[i]->GetYaxis()->SetTitle("counts");
      h_theta_T[i]->GetYaxis()->SetTitleSize(0.05);
      h_theta_T[i]->GetYaxis()->SetLabelSize(0.05);
      h_theta_T[i]->GetYaxis()->SetTitleOffset(1.1);
      h_theta_T[i]->SetLineColor(kGreen+2);
      //h_theta_T[i]->GetXaxis()->SetRangeUser(55,75);
   }

//#################################################################################################
// Data LOOP
   for(int j=0; j<entries; j++){
      tree->GetEntry(j);
      // Fill the histo
      
      for(Int_t r=0; r<6; r++){
         Double_t t=52+(r*3);
         if(theta_deg>=t && theta_deg<t+3 && cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
            h_theta_T[r]->Fill(theta_deg);
            h_mult[r]->Fill(cl_padMult[0]);
         }
      }
   }
   
   for(Int_t k=0; k<6; k++){
      x[k] = h_theta_T[k]->GetMean();
      y[k] = h_mult[k]->GetMean();
   }
   
   TGraph *gr1=new TGraph(5,x,y);
   gr1->GetXaxis()->SetTitle("#vartheta (deg)");
   gr1->GetYaxis()->SetTitle("Multiplicity");  
   
   
   for(int i=0; i<6; i++){
     cout<<"Point: " << i <<"   thetaMean: " << h_theta_T[i]->GetMean() << "   multMean: " << h_mult[i]->GetMean() <<endl;
   }   
   
   C1->Divide(3,2);
   for(Int_t s=0; s<6; s++){
      C1->cd(s+1);
      //h_mult[s]->Fit("gaus","","+",-0.5,59.5);
      h_mult[s]->Draw();
   }
   C1->SaveAs("../Pictures/run278/7Li/mult_vs_theta/row0/Mult_row0.png");
   
   C2->cd();
   C2->Divide(3,2);
   for(int i=0; i<6; i++){
     Int_t t=52+(i*3);
     Int_t s = t+3;
     C2->cd(i+1);
     sprintf(histoname,"#vartheta #in [%d;%d]",t,s);
     Double_t h = 0.5*h_theta_T[i]->GetMaximum();
     TLatex*text = new TLatex(35.,h,histoname);
     h_theta_T[i]->Draw("same");
     //h_theta_T[i]->Fit("gaus","","+",t,s);
     text->Draw("SAME");
   }
   C2->SaveAs("../Pictures/run278/7Li/mult_vs_theta/row0/theta_row0.png");
   
   C3->cd();
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerSize(1);
   gr1->Draw("AP");
   C3->SaveAs("../Pictures/run278/7Li/mult_vs_theta/row0/MultVsTheta_row0.png");
   
}  
