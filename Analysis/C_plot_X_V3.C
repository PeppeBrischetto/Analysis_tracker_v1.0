//###################################################################################################
//#   plot X for each row
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created november 2024 by D. Torresi
//#######################################

//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_X_V3(int run)
{

//#################################################################################################
// Define and declare Variables
   char histoname[100];
   int h= 0;
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAPHICS

   TCanvas *C1=new TCanvas("c1","c1",250,160,1600,600);   
   
   // all tracks
   TH1D *Xpos[10];
   for(int h=0; h<10; h++){
      sprintf(histoname,"xpos%i",h); 
      
      //Xpos[h]=new TH1D(histoname,histoname,1200,-50.5,349.5);
      Xpos[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      Xpos[h]->GetXaxis()->SetTitle("x (mm)");
      Xpos[h]->GetYaxis()->SetTitle("counts");
      Xpos[h]->SetStats(0);
   }
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
 
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      
      //for(int h=0; h<5; h++){
         if(cl_padMult[0]==1) Xpos[0]->Fill(cl_x[h]);
         if(cl_padMult[0]==2) Xpos[1]->Fill(cl_x[h]);
         if(cl_padMult[0]==3) Xpos[2]->Fill(cl_x[h]);
         if(cl_padMult[0]==4) Xpos[3]->Fill(cl_x[h]);
         if(cl_padMult[0]==5) Xpos[4]->Fill(cl_x[h]);
         if(cl_padMult[0]==6) Xpos[5]->Fill(cl_x[h]);
         if(cl_padMult[0]==7) Xpos[6]->Fill(cl_x[h]);
         if(cl_padMult[0]==8) Xpos[7]->Fill(cl_x[h]);
         
      //}     
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
   Xpos[5]->SetLineColor(kOrange);
   Xpos[5]->Draw("same");
   Xpos[6]->SetLineColor(kGreen+2);
   Xpos[6]->Draw("same");
   Xpos[7]->SetLineColor(kMagenta +1);
   Xpos[7]->Draw("same");

   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   leg->AddEntry(Xpos[0], " mul 0", "l"); 
   leg->AddEntry(Xpos[1], " mul 1", "l");
   leg->AddEntry(Xpos[2], " mul 2", "l");
   leg->AddEntry(Xpos[3], " mul 3", "l");
   leg->AddEntry(Xpos[4], " mul 4", "l");
   leg->AddEntry(Xpos[5], " mul 5", "l");
   leg->AddEntry(Xpos[6], " mul 6", "l");
   leg->AddEntry(Xpos[7], " mul 7", "l");
   leg->Draw();

}  
   
