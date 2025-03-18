//###################################################################################################
//#   plot spectra of multiplicity theta and phi for Gcut alpha and Li
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   creatd march 2025 by D. Torresi
//#######################################

//# 
//###################################################################################################

#include "../Include/openfiles.h"


void C_plot_multiplicity_CUT(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   char cond[100];

   

  
   
   // all charg distributions
   TH1F *histo_li[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_li[i]=new TH1F("","",60,-0.5,59.5);
      histo_li[i]->GetXaxis()->SetTitle("Pad number");
      histo_li[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_li[i]->SetLineColor(kBlue);
   }



   
   TH1F *histo_he[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_he[i]=new TH1F("","",60,-0.5,59.5);
      histo_he[i]->GetXaxis()->SetTitle("Pad number");
      histo_he[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_he[i]->SetLineColor(kRed);
   }
   
   TH1F *histo_phi_li=new TH1F("","",500,-30,30);
   TH1F *histo_phi_he=new TH1F("","",500,-30,30);
   
   TH1F *histo_theta_li=new TH1F("","",500,-5,80);
   TH1F *histo_theta_he=new TH1F("","",500,-5,80);   
   
   TCanvas *mult = new TCanvas("mult");
   TCanvas *phi = new TCanvas("phi");
   TCanvas *theta = new TCanvas("theta");
   
//#################################################################################################
// OpenFile
   openTrackFile(run);
   tree->Print();

//#################################################################################################

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
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################



   for(Int_t i=0; i<entries; i++){
       tree->GetEntry(i);
       
       if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
          histo_phi_li->Fill(phi_deg);
          histo_theta_li->Fill(theta_deg);
          for(int j=0; j<5; j++){
             histo_li[j]->Fill(cl_padMult[j]);
             
          }
          
       }

       if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
          histo_phi_he->Fill(phi_deg);
          histo_theta_he->Fill(theta_deg);
          for (Int_t j = 0; j < 5; j++) {
             histo_he[j]->Fill(cl_padMult[j]);
          }
 
       } 
   }

   mult->cd();
   histo_li[3]->SetLineColor(kRed);
   histo_li[3]->Draw();
   histo_he[3]->SetLineColor(kGreen);
   histo_he[3]->Draw("Same");
   phi->cd();
   histo_phi_li->Draw();
   histo_phi_li->SetLineColor(kRed);
   histo_phi_he->Draw("same");
   histo_phi_he->SetLineColor(kGreen);
   theta->cd();
   histo_theta_li->Draw();
   histo_theta_li->SetLineColor(kRed);
   histo_theta_he->Draw("same");
   histo_theta_he->SetLineColor(kGreen);   
}
   
