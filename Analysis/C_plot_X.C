//###################################################################################################
//#   plot theta spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated: November 2024 by D. Torresi
//#   updated: 2024 November 20 add openTrackFile function D.Torresi
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_X(int run)
{

//#################################################################################################
// Define and declare Variables
   char histoname[100];

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,1600,600);   
   
   // all tracks
   TH1D *Xpos[5];
   for(int h=0; h<5; h++){
      sprintf(histoname,"xpos%i",h); 
      //Xpos[h]=new TH1D(histoname,histoname,1200,-50.5,349.5);
      Xpos[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      Xpos[h]->GetXaxis()->SetTitle("x (mm)");
      Xpos[h]->GetYaxis()->SetTitle("counts");
   }
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
 
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      
      for(int h=0; h<5; h++){Xpos[h]->Fill(cl_x[h]);}     
   }
   cout<<endl;
   
//#################################################################################################
// Draw histos     
   Xpos[0]->SetLineColor(kBlack);
   Xpos[0]->Draw();
   Xpos[1]->SetLineColor(kRed);
   Xpos[1]->Draw("same");
   Xpos[2]->SetLineColor(kGreen);
   Xpos[2]->Draw("same");
   Xpos[3]->SetLineColor(kBlue);
   Xpos[3]->Draw("same");
   Xpos[4]->SetLineColor(kViolet);
   Xpos[4]->Draw("same");

   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   leg->AddEntry(Xpos[0], " row 0", "l"); 
   leg->AddEntry(Xpos[1], " row 1", "l");
   leg->AddEntry(Xpos[2], " row 2", "l");
   leg->AddEntry(Xpos[3], " row 3", "l");
   leg->AddEntry(Xpos[4], " row 4", "l");
   leg->Draw();

}  
   
