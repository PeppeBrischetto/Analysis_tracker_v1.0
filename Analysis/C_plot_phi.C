//###################################################################################################
//#   plot phi spectra of a single SiC
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
   
   // all tracks
   TH1F *histoPhi=new TH1F("","",800,-40,40);
   histoPhi->SetStats(0);
   histoPhi->GetXaxis()->SetTitle("charge");
   histoPhi->GetYaxis()->SetTitle("counts");

//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
  
      // Fill the histo
      histoPhi->Fill(phi_deg);
      cout<<i<<"  "<<  phi_deg<<endl;
     
   }
   
   histoPhi->SetLineColor(kBlack);
   histoPhi->Draw();

   
}
