//###################################################################################################
//#   Plot row theta as a function of multiplicity for integer multiplicity searches.
//#   If GraphicalCuts are needed, please be sure they are properly set before launch the macro
//#  (and rightly take them into account in the selection inside the for-cycle).
//#
//#   required as argument the run number
//#
//###################################################################################################
//#   created july 2025 by A. Pitronaci
//#######################################
//#   
//# 
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_multRowCorr(int run)
{

//###################################################################################################
// VARIABLES
  
   int flagA=0;
   char histoname[100];
   char titleCanv[100];
   char titleYaxis[100];
   Int_t stats[30] = {0};
   Double_t theta_err[30] = {0.};
   Int_t evtCounter = 0;
//###################################################################################################
// OpenFile
   openTrackFile(run);

//###################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,32,37);
   cutGli->SetPoint(1,133,160);
   cutGli->SetPoint(2,106,170);
   cutGli->SetPoint(3,17,47);
   cutGli->SetPoint(4,33,47);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,28,18);
   cutGa->SetPoint(1,186,203);
   cutGa->SetPoint(2,170,210);
   cutGa->SetPoint(3,17,26);
   cutGa->SetPoint(4,28,18);
   
//###################################################################################################
   
   // TH2D-histograms multiplicity 
   TH2D *h_mult_M0[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"mult %i",i);
      sprintf(titleYaxis,"Pad multiplicity - Row%d",i);
      h_mult_M0[i]=new TH2D("","",60,0,59,60,0,59);
      h_mult_M0[i]->GetXaxis()->SetTitle("Pad multiplicity - Row0");
      h_mult_M0[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult_M0[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult_M0[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult_M0[i]->GetYaxis()->SetTitle(titleYaxis);
      h_mult_M0[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult_M0[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult_M0[i]->GetYaxis()->SetTitleOffset(1.);
   }

   TH2D *h_mult_M1[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"mult %i",i);
      sprintf(titleYaxis,"Pad multiplicity - Row%d",i);
      h_mult_M1[i]=new TH2D("","",60,0,59,60,0,59);
      h_mult_M1[i]->GetXaxis()->SetTitle("Pad multiplicity - Row1");
      h_mult_M1[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult_M1[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult_M1[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult_M1[i]->GetYaxis()->SetTitle(titleYaxis);
      h_mult_M1[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult_M1[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult_M1[i]->GetYaxis()->SetTitleOffset(1.);
   }
   
   TH2D *h_mult_M2[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"mult %i",i);
      sprintf(titleYaxis,"Pad multiplicity - Row%d",i);
      h_mult_M2[i]=new TH2D("","",60,0,59,60,0,59);
      h_mult_M2[i]->GetXaxis()->SetTitle("Pad multiplicity - Row2");
      h_mult_M2[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult_M2[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult_M2[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult_M2[i]->GetYaxis()->SetTitle(titleYaxis);
      h_mult_M2[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult_M2[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult_M2[i]->GetYaxis()->SetTitleOffset(1.);
   }
   
   TH2D *h_mult_M3[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"mult %i",i);
      sprintf(titleYaxis,"Pad multiplicity - Row%d",i);
      h_mult_M3[i]=new TH2D("","",60,0,59,60,0,59);
      h_mult_M3[i]->GetXaxis()->SetTitle("Pad multiplicity - Row3");
      h_mult_M3[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult_M3[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult_M3[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult_M3[i]->GetYaxis()->SetTitle(titleYaxis);
      h_mult_M3[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult_M3[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult_M3[i]->GetYaxis()->SetTitleOffset(1.);
   }
   
   TH2D *h_mult_M4[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"mult %i",i);
      sprintf(titleYaxis,"Pad multiplicity - Row%d",i);
      h_mult_M4[i]=new TH2D("","",60,0,59,60,0,59);
      h_mult_M4[i]->GetXaxis()->SetTitle("Pad multiplicity - Row4");
      h_mult_M4[i]->GetXaxis()->SetTitleSize(0.05);
      h_mult_M4[i]->GetXaxis()->SetLabelSize(0.05);
      h_mult_M4[i]->GetXaxis()->SetTitleOffset(.9);
      h_mult_M4[i]->GetYaxis()->SetTitle(titleYaxis);
      h_mult_M4[i]->GetYaxis()->SetTitleSize(0.05);
      h_mult_M4[i]->GetYaxis()->SetLabelSize(0.05);
      h_mult_M4[i]->GetYaxis()->SetTitleOffset(1.);
   }
//#################################################################################################
// Data LOOP
   for(int j=0; j<entries; j++){
      tree->GetEntry(j);
      // Fill the histo
      
      for(Int_t m=0; m<5; m++){
         h_mult_M0[m]->Fill(cl_padMult[0],cl_padMult[m]);
         h_mult_M1[m]->Fill(cl_padMult[1],cl_padMult[m]);
         h_mult_M2[m]->Fill(cl_padMult[2],cl_padMult[m]);
         h_mult_M3[m]->Fill(cl_padMult[3],cl_padMult[m]);
         h_mult_M4[m]->Fill(cl_padMult[4],cl_padMult[m]);
      }
   }
   
   TLegend *l = new TLegend(0.6,0.2,0.9,0.3);
   //l->AddEntry(gr1,"statistics > 5 % of the total","p");
   //l->AddEntry(gr1_ls,"statistics < 5 % of the total","p");
   
   TCanvas *C1=new TCanvas("C1","C1",250,160,800,600);   
   C1->Divide(3,2);
   for(Int_t c=0; c<5; c++){
      C1->cd(c+1);
      h_mult_M0[c]->Draw("COLZ");
      gPad->Update();
      TPaveStats* st = (TPaveStats*)h_mult_M0[c]->GetListOfFunctions()->FindObject("stats");
      st->SetX1NDC(0.6);
      st->SetX2NDC(0.8);
      st->SetY1NDC(0.6),
      st->SetY2NDC(0.8);
      gPad->Modified();
      gPad->Update();
   }
   
   TCanvas *C2=new TCanvas("C2","C2",250,160,800,600);
   C2->Divide(3,2);
   for(Int_t c=0; c<5; c++){
      C2->cd(c+1);
      h_mult_M1[c]->Draw("COLZ");
      gPad->Update();
      TPaveStats* st = (TPaveStats*)h_mult_M1[c]->GetListOfFunctions()->FindObject("stats");
      st->SetX1NDC(0.6);
      st->SetX2NDC(0.8);
      st->SetY1NDC(0.6),
      st->SetY2NDC(0.8);
      gPad->Modified();
      gPad->Update();
   }
   
   TCanvas *C3=new TCanvas("C3","C3",250,160,800,600);
   C3->Divide(3,2);
   for(Int_t c=0; c<5; c++){
      C3->cd(c+1);
      h_mult_M2[c]->Draw("COLZ");
      gPad->Update();
      TPaveStats* st = (TPaveStats*)h_mult_M2[c]->GetListOfFunctions()->FindObject("stats");
      st->SetX1NDC(0.6);
      st->SetX2NDC(0.8);
      st->SetY1NDC(0.6),
      st->SetY2NDC(0.8);
      gPad->Modified();
      gPad->Update();
   }
   
   TCanvas *C4=new TCanvas("C4","C4",250,160,800,600);
   C4->Divide(3,2);
   for(Int_t c=0; c<5; c++){
      C4->cd(c+1);
      h_mult_M3[c]->Draw("COLZ");
      gPad->Update();
      TPaveStats* st = (TPaveStats*)h_mult_M3[c]->GetListOfFunctions()->FindObject("stats");
      st->SetX1NDC(0.6);
      st->SetX2NDC(0.8);
      st->SetY1NDC(0.6),
      st->SetY2NDC(0.8);
      gPad->Modified();
      gPad->Update();
   }
   
   TCanvas *C5=new TCanvas("C5","C5",250,160,800,600);
   C5->Divide(3,2);
   for(Int_t c=0; c<5; c++){
      C5->cd(c+1);
      h_mult_M4[c]->Draw("COLZ");
      gPad->Update();
      TPaveStats* st = (TPaveStats*)h_mult_M4[c]->GetListOfFunctions()->FindObject("stats");
      st->SetX1NDC(0.6);
      st->SetX2NDC(0.8);
      st->SetY1NDC(0.6),
      st->SetY2NDC(0.8);
      gPad->Modified();
      gPad->Update();
   }
 
   
}
