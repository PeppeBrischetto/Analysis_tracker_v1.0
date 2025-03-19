//###################################################################################################
//#   plot spectra of multiplicity
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   creatd feb 2025 by A. Pitronaci - based on D. Torresi's sources
//#######################################
//#   updated: Febbruary 2025 by A. Pitronaci
//#   updated: march 2025 by D. Torresi selection is done with the angle and not with the GCut
//# 
//###################################################################################################

#include "../Include/openfiles.h"

const Int_t nPads = 60;
const Int_t minPad = 0;
const Int_t maxPad = 59;

void C_plot_multiplicity_v2(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   char cond[100];
   Double_t sigma_he = 0;
   Double_t width_he[5] = {0};
   Double_t sigma_li = 0;
   Double_t width_li[5] = {0};
   Int_t n_point_tot = 0;
   Double_t media_he[5] = {0.};
   Double_t media_li[5] = {0.};
   Double_t theta_he = 0;
   
   Int_t li = 0;
   Int_t he = 0;
   
   TF1 *f = new TF1("f","gaus",-60,60);
   f->SetParameters(1,1,1);
  
   
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
   
 
   
   
   TCanvas *che = new TCanvas("che");
//#################################################################################################
// OpenFile
   openTrackFile(run);
   
//#################################################################################################

   
//#################################################################################################
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################


   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      
      if(phi_deg<5 && phi_deg>-5){
         for(int j=0; j<5; j++){
            histo_li[j]->Fill(cl_padMult[j]);
         }
          
      }else{

         for(Int_t j = 0; j < 5; j++) {
  	    histo_he[j]->Fill(cl_padMult[j]);
         }
      } 
       
      histo_he[3]->SetLineColor(kGreen);
      histo_he[3]->Draw();
      histo_li[3]->SetLineColor(kRed);
      histo_li[3]->Draw("Same");    
   }
  
}
   
