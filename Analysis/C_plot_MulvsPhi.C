//###################################################################################################
//#   plot pad multiplicity of the cluster of a given row vs phi
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created november 2024 by D. Torresi
//#######################################
//###################################################################################################

#include "../Include/openfiles.h"



void C_plot_MulvsPhi(int run)
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
   TCanvas *C2=new TCanvas("c2","c2",350,360,1600,600);      
   TCanvas *C3=new TCanvas("c3","c3",350,360,1600,500);     
   
   // all tracks
   TH1D *Xpos[5];
   for(int h=0; h<5; h++){
      sprintf(histoname,"xpos%i",h); 
      //Xpos[h]=new TH1D(histoname,histoname,1200,-50.5,349.5);
      Xpos[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      Xpos[h]->GetXaxis()->SetTitle("x (mm)");
      Xpos[h]->GetYaxis()->SetTitle("counts");
   }
   
   TH1D *Hphi=new TH1D("","",1000, -10,10);
   
   TH2D *HtcVSp=new TH2D("","",400, -0.5,0.5,16,-0.5,15.5);

   TGraph *gr1= new TGraph();
//#################################################################################################
// Data LOOP
   //entries=1000;
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
 
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      Hphi->Fill(phi);
      gr1->SetPoint(i,phi,cl_padMult[4]);  
      HtcVSp->Fill(phi,cl_padMult[4]);   
   }
   cout<<endl;
   
//#################################################################################################
// Draw Graphs
   C1->cd();
   Hphi->Draw();

   C2->cd();
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerSize(1);
   gr1->Draw("AP");

   C3->cd();
   HtcVSp->SetStats(0);
   HtcVSp->GetXaxis()->SetTitle("#phi (deg)");
   HtcVSp->GetYaxis()->SetTitle("multiplicity row 4");
   HtcVSp->Draw("colz"); 
   
}  
   
