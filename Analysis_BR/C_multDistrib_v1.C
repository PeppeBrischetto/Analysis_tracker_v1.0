//################################################################################################################
//#   plot of charge multiplicity for each row and theta distribution
//#   before use this macro, be sure you have got the TCut cl_x[1]:cl_x[0] (is necessary, for runs 7Li + alpha as well as for 12C, to delete uncorrelated events).
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

const TString CutFileA = "TCutG/alpha_tcut_run174.root";
const TString CutFileC = "TCutG/c_tcut_run174.root";
const Int_t NRows = 5;

void C_multDistrib_v1(int run){

//#################################################################################################
// Required variables

   // counting variables
   Int_t he,li=0;
   
   // filling variable for TH1D
   char filling_li[200];
   char filling_he[200];
   char histoname[200];
   Double_t vartheta = 0;
   
   // Title .png to save
   char title[200];
   char title1[200];

   // multiplicity distribution array, one for each row
   TH1D *histo_Mli[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Multiplicity distrib. - row%i",i);
<<<<<<< HEAD:Analysis_BR/C_multDistrib_v2.1.C
      histo_Mli[i]=new TH1D("","",60,-0.5,59.5);
=======
      histo_Mli[i]=new TH1D("","",61,-0.5,60.5);
>>>>>>> 045bb50266653cd0b8a855af5a2f3f0a3acddff5:Analysis_BR/C_multDistrib_v2.C
      histo_Mli[i]->GetXaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetYaxis()->SetTitleSize(0.06);
      histo_Mli[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_Mli[i]->GetYaxis()->SetTitleOffset(0.60);
      histo_Mli[i]->GetXaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetYaxis()->SetLabelSize(0.05);
      histo_Mli[i]->GetXaxis()->SetTitle("Multiplicity");
      histo_Mli[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_Mli[i]->SetLineColor(kCyan+2);
      histo_Mli[i]->GetYaxis()->SetRangeUser(1,1500000);
   }

   TH1D *histo_MliSiC[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Multiplicity distrib. - row%i SiC",i);
      histo_MliSiC[i]=new TH1D("","",60,-0.5,59.5);
      histo_MliSiC[i]->GetXaxis()->SetTitleSize(0.06);
      histo_MliSiC[i]->GetYaxis()->SetTitleSize(0.06);
      histo_MliSiC[i]->GetXaxis()->SetTitleOffset(0.7);
      histo_MliSiC[i]->GetYaxis()->SetTitleOffset(0.60);
      histo_MliSiC[i]->GetXaxis()->SetLabelSize(0.05);
      histo_MliSiC[i]->GetYaxis()->SetLabelSize(0.05);
      histo_MliSiC[i]->GetXaxis()->SetTitle("Multiplicity");
      histo_MliSiC[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_MliSiC[i]->SetLineColor(kRed+2);
      histo_MliSiC[i]->GetYaxis()->SetRangeUser(1,1500000);
   }
   
   TH1D *histo_Mhe[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Multiplicity distrib. - row%i",i);
      histo_Mhe[i]=new TH1D("","",61,-0.5,60.5);
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
   // TH1D for theta
   TH1D *h_theta = new TH1D("h_theta","",1000,0,80);
   h_theta->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetXaxis()->SetRangeUser(0,70);
   h_theta->GetYaxis()->SetRangeUser(1,1E6);
   h_theta->GetXaxis()->SetTitleOffset(0.7);
   h_theta->GetYaxis()->SetTitleOffset(0.60);
   h_theta->GetXaxis()->SetLabelSize(0.05);
   h_theta->GetYaxis()->SetLabelSize(0.05);
   h_theta->GetXaxis()->SetTitleSize(0.06);
   h_theta->GetYaxis()->SetTitleSize(0.06);
   TH1D *h_theta_sic = new TH1D("h_theta_sic","",1000,0,80); 
   h_theta_sic->GetXaxis()->SetTitle("#vartheta (deg)");
   h_theta_sic->GetYaxis()->SetTitle("Counts");
   h_theta_sic->GetXaxis()->SetRangeUser(0,70);
   h_theta_sic->SetLineColor(kRed);
   h_theta_sic->GetXaxis()->SetTitleOffset(0.7);
   h_theta_sic->GetYaxis()->SetTitleOffset(0.60);
   h_theta_sic->GetXaxis()->SetLabelSize(0.05);
   h_theta_sic->GetYaxis()->SetLabelSize(0.05);
   h_theta_sic->GetXaxis()->SetTitleSize(0.06);
   h_theta_sic->GetYaxis()->SetTitleSize(0.06);
   
   TLegend *l = new TLegend(0.1,0.7,0.65,0.9);
   //l->AddEntry(histo_Mhe[0],"Multiplicity distribution - #alpha","lp");
   l->AddEntry(histo_Mli[0],"Multiplicity distribution - ^{16}O","lp");
   l->SetFillStyle(0);
   
   TLegend *l1 = new TLegend(0.1,0.55,0.6,0.75);
   l1->AddEntry(h_theta,"#vartheta distribution - All evts","lp");
   l1->AddEntry(h_theta_sic,"#vartheta distribution - E_{SiC}>2000 a.u.","lp");
   l1->SetFillStyle(0);
   
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
   
   /*TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,16,25);
   cutGa->SetPoint(1,26,15);
   cutGa->SetPoint(2,195,205);
   cutGa->SetPoint(3,174,215);
   cutGa->SetPoint(4,16,25);*/
   //cutGa->SaveAs(CutFileA);
   TCutG *cutGc = new TCutG("cutGc",5);
   cutGc->SetVarX("cl_x_mm[0]");
   cutGc->SetVarY("cl_x_mm[1]");
   cutGc->SetPoint(0,6,38);
   cutGc->SetPoint(1,19,30);
   cutGc->SetPoint(2,98,133);
   cutGc->SetPoint(3,74,141);
   cutGc->SetPoint(4,6,38);
   //cutGc->SaveAs(CutFileC);
   
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
      
      /*if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
        he+=1;
        for(Int_t j=0; j<NRows; j++){
           histo_Mhe[j]->Fill(cl_padMult[j]);
        }
      }else*/
      if(cutGc->IsInside(cl_x_mm[0], cl_x_mm[1])){
        li+=1;
        for(Int_t j=0; j<NRows; j++){
           histo_Mli[j]->Fill(cl_padMult[j]);
        }
        vartheta=theta*180/Pi();
        h_theta->Fill(vartheta);
        if(energySic>2000){
          h_theta_sic->Fill(vartheta);
          for(Int_t j=0; j<NRows; j++){
            histo_MliSiC[j]->Fill(cl_padMult[j]);
          }
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
   histo_MliSiC[0]->Draw("same");
   //histo_Mhe[0]->Draw("histo && same");
   
   r0->Draw("same");
   l->Draw("same");
   c0->cd(2);   
   gPad->SetLogy();
   histo_Mli[1]->Draw("histo");
   histo_MliSiC[1]->Draw("same");
   //histo_Mhe[1]->Draw("histo && same");
   l->Draw("same");
   r1->Draw("same");
   c0->cd(3);
   gPad->SetLogy();
   histo_Mli[2]->Draw("histo");
   histo_MliSiC[2]->Draw("same");
   //histo_Mhe[2]->Draw("histo && same");
   l->Draw("same");
   r2->Draw("same");
   c0->cd(4);
   gPad->SetLogy();
   histo_Mli[3]->Draw("histo");
   histo_MliSiC[3]->Draw("same");
   //histo_Mhe[3]->Draw("histo && same");
   r3->Draw("same");
   l->Draw("same");
   c0->cd(5);
   gPad->SetLogy();
   histo_Mli[4]->Draw("histo");
   histo_MliSiC[4]->Draw("same");
   //histo_Mhe[4]->Draw("histo && same");
   l->Draw("same"); 
   r4->Draw("same");
   c0->cd(6);
   gPad->SetLogy();
   h_theta->Draw();
   h_theta_sic->Draw("same");
   l1->Draw("same");
<<<<<<< HEAD:Analysis_BR/C_multDistrib_v2.1.C
   //sprintf(title,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution_theta_Run174.eps");
   //sprintf(title1,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution_theta_Run174.png");
   c0->SaveAs(title);
   c0->SaveAs(title1);
=======
   sprintf(title,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution_theta_Run174.eps");
   sprintf(title1,"Pictures_Analysis/Multiplicity_distrib/Multiplicity distribution_theta_Run174.png");
   //c0->SaveAs(title);
   //c0->SaveAs(title1);
>>>>>>> 045bb50266653cd0b8a855af5a2f3f0a3acddff5:Analysis_BR/C_multDistrib_v2.C
   
   h_theta_sic->Fit("gaus","","",50,60);

}
