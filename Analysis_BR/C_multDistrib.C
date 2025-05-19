//################################################################################################################
//#   plot of charge multiplicity for each row
//#   before use this macro, be sure you have got the TCut cl_x[1]:cl_x[0] (is necessary, for runs Li + alpha).
//#   have a look to the file "README_dir.txt" for detailed information
//#
//################################################################################################################
//#   creatd May 2025 by A. Pitronaci 
//################################################################################################################
//#   updated:
//################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>

#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const TString CutFileA = "TCutG/alpha_tcut_run79.root";
const TString CutFileC = "TCutG/c_tcut_run79.root";
const Int_t NRows = 5;

void C_multDistrib(int run){

//#################################################################################################
// Required variables

   // counting variables
   Int_t he,li=0;
   
   // filling variable for TH1D
   char filling_li[200];
   char filling_he[200];
   char histoname[200];
   
   // Title .png to save
   char title[200];
   char title1[200];

   // multiplicity distribution array, one for each row
   TH1D *histo_Mli[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Multiplicity distrib. - row%i",i);
      histo_Mli[i]=new TH1D("","",30,-0.5,29.5);
      histo_Mli[i]->GetXaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetYaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_Mli[i]->GetYaxis()->SetTitleOffset(0.60);
      histo_Mli[i]->GetXaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetYaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetXaxis()->SetTitle("Multiplicity");
      histo_Mli[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_Mli[i]->SetLineColor(kYellow+2);
      histo_Mli[i]->GetYaxis()->SetRangeUser(1,1500000);
   }
   
   TH1D *histo_Mhe[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Multiplicity distrib. - row%i",i);
      histo_Mhe[i]=new TH1D("","",30,-0.5,29.5);
      histo_Mhe[i]->GetYaxis()->SetTitleSize(0.06);
      histo_Mhe[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_Mhe[i]->GetYaxis()->SetTitleOffset(0.60);
      histo_Mhe[i]->GetXaxis()->SetLabelSize(0.05);
      histo_Mhe[i]->GetYaxis()->SetLabelSize(0.05);
      histo_Mhe[i]->GetXaxis()->SetTitle("Multiplicity");
      histo_Mhe[i]->GetXaxis()->SetTitle("Multiplicity");
      histo_Mhe[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_Mhe[i]->SetLineColor(kGreen+2);
      histo_Mhe[i]->GetYaxis()->SetRangeUser(1,1500000);
   }
   
   TLegend *l = new TLegend(0.55,0.4,0.9,0.66);
   //l->AddEntry(histo_Mhe[0],"Multiplicity distribution - #alpha","lp");
   l->AddEntry(histo_Mli[0],"Multiplicity distribution - ^{12}C","lp");
   l->SetFillStyle(0);
   
   TText* r0 = new TText(21,44510,"Row0");
   TText* r1 = new TText(21,44510,"Row1");
   TText* r2 = new TText(21,44510,"Row2");
   TText* r3 = new TText(21,44510,"Row3");
   TText* r4 = new TText(21,44510,"Row4");

//#################################################################################################



//#################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,200,18);
   cutGa->SetPoint(1,194,208);
   cutGa->SetPoint(2,177,214);
   cutGa->SetPoint(3,230,29);
   cutGa->SetPoint(4,250,18);
   //cutGa->SaveAs(CutFileA);
   TCutG *cutGc = new TCutG("cutGc",5);
   cutGc->SetVarX("cl_x_mm[0]");
   cutGc->SetVarY("cl_x_mm[1]");
   cutGc->SetPoint(0,17,32);
   cutGc->SetPoint(1,96,133);
   cutGc->SetPoint(2,61,141);
   cutGc->SetPoint(3,5,45);
   cutGc->SetPoint(4,17,32);
   cutGc->SaveAs(CutFileC);
   
   TFile *cutFilea = TFile::Open(CutFileA);
   TFile *cutFileC = TFile::Open(CutFileC);

//#################################################################################################

//  Loop to read multiplicity from Track_File and get the mutliplicity distribution

   for(Int_t i= 0; i<entries; i++){
      tree->GetEntry(i);
      //cout<<"#@@#"<<i<<endl;
      /*for(Int_t j=0; j<NRows;j++){
             histo_Mli[j]->Reset();
             histo_Mhe[j]->Reset();
          }*/
      
      if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
        he+=1;
        for(Int_t j=0; j<NRows; j++){
           histo_Mhe[j]->Fill(cl_padMult[j]);
        }
      }else
      if(cutGc->IsInside(cl_x_mm[0], cl_x_mm[1])){
        li+=1;
        for(Int_t j=0; j<NRows; j++){
           histo_Mli[j]->Fill(cl_padMult[j]);
        }
      }
   }
   
   cout << "Events he: " << he << "  Events c: " << li << endl; 
   
// Visualization block
   TCanvas *c0 = new TCanvas();
   c0->Divide(2,3);
   c0->cd(1);
   gPad->SetLogy();
   histo_Mli[0]->Draw("histo");
   //histo_Mhe[0]->Draw("histo same");
   r0->Draw("same");
   l->Draw("same");
   c0->cd(2);   
   gPad->SetLogy();
   histo_Mli[1]->Draw("histo");
   //histo_Mhe[1]->Draw("histo same");
   l->Draw("same");
   r1->Draw("same");
   c0->cd(3);
   gPad->SetLogy();
   histo_Mli[2]->Draw("histo");
   //histo_Mhe[2]->Draw("histo same");
   l->Draw("same");
   r2->Draw("same");
   c0->cd(4);
   gPad->SetLogy();
   histo_Mli[3]->Draw("histo");
   //histo_Mhe[3]->Draw("histo same");
   r3->Draw("same");
   l->Draw("same");
   c0->cd(5);
   gPad->SetLogy();
   histo_Mli[4]->Draw("histo");
   //histo_Mhe[4]->Draw("histo same");
   l->Draw("same"); 
   r4->Draw("same");
   sprintf(title,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution - Run79.eps");
   sprintf(title1,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution - Run79.png");
   c0->SaveAs(title);
   c0->SaveAs(title1);

}
