//###################################################################################################
//#   plot phi spectra
//#      
//#   required as argument the run number
//#            
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated: new file format 2024 November  D. Torresi
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_phi(int run)
{

//###################################################################
//    VARIABLES

   int flagA=0;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);   
   TCanvas *C2=new TCanvas("c2","c2",350,260,800,600);   
   
   // all tracks
   TH1F *histoPhi=new TH1F("","",800,-40,40);
   histoPhi->SetStats(0);
   histoPhi->GetXaxis()->SetTitle("phi (deg)");
   histoPhi->GetYaxis()->SetTitle("counts");
   // track with SiC
   TH1F *histoPhiS=new TH1F("","",800,-40,40);
   histoPhiS->SetStats(0);
   histoPhiS->GetXaxis()->SetTitle("phi (deg)");
   histoPhiS->GetYaxis()->SetTitle("counts");



//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
  
      // Fill the histo
      histoPhi->Fill(phi_deg);
      cout<<i<<"  "<<  phi_deg<<endl;
      if(sic_fired){
         histoPhiS->Fill(phi_deg);
      }

      

   }
   C1->cd();
   histoPhi->SetLineColor(kBlack);
   histoPhi->Draw();
   histoPhiS->SetLineColor(kRed);
   histoPhiS->Draw("same");

   
   
   
}
