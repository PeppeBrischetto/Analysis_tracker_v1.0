//###################################################################################################
//#   plot theta spectra specific for run 332 shows the problem present at high multiplicity of the tracks
//#   Specific for Li
//#
//#   required as argument the run number
//#
//###################################################################################################
//#   created march 2025 by D. Torresi
//#######################################
//#   
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_theta_mult(int run)
{

//###################################################################
//    VARIABLES
  
   int flagA=0;
   char histoname[100];
   char titleCanv[100];
   Double_t x[5] = {0.};
   Double_t y[5] = {0.};
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,5,44);
   cutGli->SetPoint(1,16.71,36.46);
   cutGli->SetPoint(2,94.6,141.95);
   cutGli->SetPoint(3,70.46,153.71);
   cutGli->SetPoint(5,5,44);
   
   TCutG *cutGa = new TCutG("cutGa",4);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,6.47,26.12);
   cutGa->SetPoint(1,20.37,16.14);
   cutGa->SetPoint(2,141.03,166.9);
   cutGa->SetPoint(3,119.46,175.09);
   cutGa->SetPoint(5,6.47,26.12);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   TCanvas *C2=new TCanvas("c2","c2",350,160,1350,1400);
   TCanvas *C2a=new TCanvas("c2a","c2a",550,260,800,1400);      
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
      h_mult[i]->GetYaxis()->SetTitle("Counts");
      h_mult[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult[i]->GetYaxis()->SetTitleOffset(1.);
   }
   
   TH1F *h_theta_T[6];
   for(int i=0; i<6; i++){
      sprintf(histoname,"mult %i",i);
      h_theta_T[i]=new TH1F("","",100,40,80);
      h_theta_T[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      h_theta_T[i]->GetXaxis()->SetTitleSize(0.05);
      h_theta_T[i]->GetXaxis()->SetLabelSize(0.05);
      h_theta_T[i]->GetXaxis()->SetTitleOffset(.9);
      h_theta_T[i]->GetYaxis()->SetTitle("Counts");
      h_theta_T[i]->GetYaxis()->SetTitleSize(0.05);
      h_theta_T[i]->GetYaxis()->SetLabelSize(0.05);
      h_theta_T[i]->GetYaxis()->SetTitleOffset(1.);
      if(i==5){
        h_theta_T[i]->SetLineColor(kYellow+1);
      }else if(i>=10){
                        h_theta_T[i]->SetLineColor(3*i);
                     }else{
                               h_theta_T[i]->SetLineColor(i);
                          }
      h_theta_T[i]->GetXaxis()->SetRangeUser(55,75);
   }

//#################################################################################################
// Data LOOP
   for(int j=0; j<entries; j++){
      tree->GetEntry(j);
      // Fill the histo
      
      for(Int_t r=0; r<6; r++){
         Double_t t=52+(r*5);
         if(theta_deg>=t && theta_deg<t+5){
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
   gr1->GetXaxis()->SetTitle("multiplicity");
   gr1->GetYaxis()->SetTitle("theta");  
   
   
   for(int i=0; i<6; i++){
     cout<<"Point: " << i <<"   thetaMean: " << h_theta_T[i]->GetMean() << "   multMean: " << h_mult[i]->GetMean() <<endl;
   }   
   
   C1->Divide(3,2);
   for(Int_t s=0; s<5; s++){
      C1->cd(s+1);
      h_mult[s]->SetLineColor(kGreen+s);
      h_mult[s]->Fit("gaus","","+",-0.5,59.5);
      h_mult[s]->Draw();
   }
   
   C2->cd();
   C2->Divide(3,2);
   h_theta_T[0]->Draw("same");
   for(int i=0; i<16; i++){
     C2->cd(i+1);
     sprintf(histoname,"Mult: %i",i);
     TText *t = new TText(70.,0.,histoname);
     h_theta_T[i]->Draw("same");
     h_theta_T[i]->Fit("gaus","","+",55,75);
     t->Draw("SAME");
   }
   
   C3->cd();
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerSize(1);
   gr1->Draw("AP");
   
   
}  
   
   





