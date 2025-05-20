//################################################################################################################
//#   plot of cluster charge distribution for each row
//#   before use this macro, be sure you have got the TCut cl_x[1]:cl_x[0].
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

const TString CutFileA = "TCutG/alpha_tcut_run299.root";
const TString CutFileLi = "TCutG/li_tcut_run299.root";
const Int_t NRows = 5;

void C_clCharge(int run){

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
      sprintf(histoname,"Cluster charge distrib. - row%i",i);
      histo_Mli[i]=new TH1D("","",100,0,3.E+5);
      histo_Mli[i]->GetXaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetYaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_Mli[i]->GetYaxis()->SetTitleOffset(0.90);
      histo_Mli[i]->GetXaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetYaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetXaxis()->SetTitle("Charge (a.u.)");
      histo_Mli[i]->GetYaxis()->SetTitle("Counts");
      histo_Mli[i]->SetLineColor(kYellow+2);
      histo_Mli[i]->GetYaxis()->SetRangeUser(1,1.E+6);
   }
   
   TH1D *histo_Mhe[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Cluster charge distrib. - row%i",i);
      histo_Mhe[i]=new TH1D("","",100,0,3.E+5);
      histo_Mhe[i]->GetYaxis()->SetTitleSize(0.06);
      histo_Mhe[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_Mhe[i]->GetYaxis()->SetTitleOffset(0.90);
      histo_Mhe[i]->GetXaxis()->SetLabelSize(0.05);
      histo_Mhe[i]->GetYaxis()->SetLabelSize(0.05);
      histo_Mhe[i]->GetXaxis()->SetTitle("Charge (a.u.)");
      histo_Mhe[i]->GetYaxis()->SetTitle("Counts");
      histo_Mhe[i]->SetLineColor(kGreen+2);
      histo_Mhe[i]->GetYaxis()->SetRangeUser(1,1.E+6);
   }
   
   TLegend *l = new TLegend(0.1,0.8,0.6,0.9);
   l->AddEntry(histo_Mhe[0],"Cluster charge distribution - #alpha","lp");
   l->AddEntry(histo_Mli[0],"Cluster charge distribution - ^{7}Li","lp");
   l->SetFillStyle(0);
   
   TText* r0 = new TText(2E5,44510,"Row0");
   TText* r1 = new TText(2E5,44510,"Row1");
   TText* r2 = new TText(2E5,44510,"Row2");
   TText* r3 = new TText(2E5,44510,"Row3");
   TText* r4 = new TText(2E5,44510,"Row4");

//#################################################################################################



//#################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   TFile *cutFileLi = new TFile(CutFileLi,"READ");
   TCutG *cutGli = (TCutG*)cutFileLi->Get("cutGli");
   TFile *cutFileA = new TFile(CutFileA,"READ");
   TCutG *cutGa = (TCutG*)cutFileA->Get("cutGa");
   
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
           histo_Mhe[j]->Fill(cl_charge[j]);
        }
      }else
      if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
        //cout << "Sono dentro il taglio!" << endl;
        li+=1;
        for(Int_t j=0; j<NRows; j++){
           histo_Mli[j]->Fill(cl_charge[j]);
        }
      }
      /*else{
        cout << "Sono fuori dal taglo!" << endl;
      }*/
   }
   
   cout << "Events he: " << he << "  Events c: " << li << endl; 
   
// Visualization block
   TCanvas *c0 = new TCanvas();
   c0->Divide(2,3);
   c0->cd(1);
   gPad->SetLogy();
   histo_Mli[0]->Draw("histo");
   histo_Mhe[0]->Draw("histo same");
   r0->Draw("same");
   l->Draw("same");
   c0->cd(2);   
   gPad->SetLogy();
   histo_Mli[1]->Draw("histo");
   histo_Mhe[1]->Draw("histo same");
   l->Draw("same");
   r1->Draw("same");
   c0->cd(3);
   gPad->SetLogy();
   histo_Mli[2]->Draw("histo");
   histo_Mhe[2]->Draw("histo same");
   l->Draw("same");
   r2->Draw("same");
   c0->cd(4);
   gPad->SetLogy();
   histo_Mli[3]->Draw("histo");
   histo_Mhe[3]->Draw("histo same");
   r3->Draw("same");
   l->Draw("same");
   c0->cd(5);
   gPad->SetLogy();
   histo_Mli[4]->Draw("histo");
   histo_Mhe[4]->Draw("histo same");
   l->Draw("same"); 
   r4->Draw("same");
   sprintf(title,"Pictures_Analysis/Cluster_charge/Cluster_charge_distribution_Run299.eps");
   sprintf(title1,"Pictures_Analysis/Cluster_charge/Cluster_charge_distribution_Run299.png");
   c0->SaveAs(title);
   c0->SaveAs(title1);

}
