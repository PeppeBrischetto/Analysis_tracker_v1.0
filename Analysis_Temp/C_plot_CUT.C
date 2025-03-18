//###################################################################################################
//#   plot x0_mm vs x1_mm and the cut for alfa and Li
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   creatd march 2025 by D. Torresi
//#######################################
//#   updated: 
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_CUT(int run)
{
//#################################################################################################
// Required variables
  

   

   

  
   
// HISTOS
   TH2F *histoX0X1 = new TH2F("","",1000,0,200,1000,0,200);
   
//#################################################################################################
// OpenFile
   openTrackFile(run);

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
   cutGli->SetPoint(5,69,118);
   cutGli->SetPoint(6,96,148);
   cutGli->SetPoint(7,24,158);
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
       histoX0X1->Fill(cl_x_mm[0],cl_x_mm[1]);
   }
   
   histoX0X1->Draw();
   cutGli->Draw("same");
   cutGli->SetLineColor(kRed);
   cutGa->Draw("same");

}




