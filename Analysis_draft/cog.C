//#################################################################################################################
//#   This macro allows to obtain the charge distributions for each rows, i.e. 5-TH1D eachone containing the 
//#   charge collected by each pad (0,..,59) for a single track event. In particular, here we can have the
//#   comparison between the track events coming by trackGenerator.C (with no threshold subtraction on the 
//#   the pad_charge) and by trackGenerator_th.C (whith the threshold subtraction on the pad_charge).
//#   To launch the macro the runNumber has to be done: in a root terminal type ".x chargeDistrib(runNumber)". 
//#################################################################################################################
//#   Created December 2025 by A. Pitronaci
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>

#include "../Include/openfiles.h"
#include "../Include/openfilesthresh.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 6;
const Int_t NPads = 60;

void cog(int run){

   /*  Needed variables  */
   
   char histoname[100];
   char titolox[100];
   Double_t x_i[NRows] = {0.};
   Double_t x[NRows] = {0.};
   Double_t pad[NRows][100] = {0.};
   Double_t charge[NRows][100] = {0.};
   Double_t totalCharge[NRows] = {0.};
   Double_t x_i_provv[NRows] = {0.};
   Double_t x_provv[NRows] = {0.};
   Double_t pad_provv[NRows][100] = {0.};
   Double_t charge_provv[NRows][100] = {0.};
   Double_t totalCharge_provv[NRows] = {0.};
   
   Int_t evt = 0;
   
   TH1D *h_cog[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_cog %i",i);
      sprintf(titolox,"x_{%i} (mm)",i);
      h_cog[i]=new TH1D("","",9000,0,300);
      h_cog[i]->GetXaxis()->SetTitle(titolox);
      h_cog[i]->GetXaxis()->SetTitleSize(0.05);
      h_cog[i]->GetXaxis()->SetLabelSize(0.05);
      h_cog[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_cog[i]->GetYaxis()->SetTitle("Counts");
      h_cog[i]->GetYaxis()->SetTitleSize(0.05);
      h_cog[i]->GetYaxis()->SetLabelSize(0.05);
      h_cog[i]->GetYaxis()->SetTitleOffset(1.);
      h_cog[i]->SetNdivisions(7);
      h_cog[i]->SetFillColor(kBlue-5);
      h_cog[i]->SetFillStyle(3001);
   }
   
   TH1D *h_cogTH[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_cogTH %i",i);
      sprintf(titolox,"x_{%i} (mm)",i);
      h_cogTH[i]=new TH1D("","",9000,0,300);
      h_cogTH[i]->GetXaxis()->SetTitle(titolox);
      h_cogTH[i]->GetXaxis()->SetTitleSize(0.05);
      h_cogTH[i]->GetXaxis()->SetLabelSize(0.05);
      h_cogTH[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_cogTH[i]->GetYaxis()->SetTitle("Counts");
      h_cogTH[i]->GetYaxis()->SetTitleSize(0.05);
      h_cogTH[i]->GetYaxis()->SetLabelSize(0.05);
      h_cogTH[i]->GetYaxis()->SetTitleOffset(1.);
      h_cogTH[i]->SetNdivisions(7);
      h_cogTH[i]->SetFillColor(kRed-5);
      h_cogTH[i]->SetLineColor(kRed);
      h_cogTH[i]->SetFillStyle(3001);
   }

//#################################################################################################################
// NO-threshold subtraction
//#################################################################################################################

   /*  Open files  */
   openTrackFile(run);
   tree->Print();

   /*  Data loop for NO-threshold subtraction evts.  */
   
   for(Int_t i=0; i<entries; i++){
      /*for(Int_t row=0; row<NRows; row++){
         h_cog[row]->Reset();
      }*/
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>2000){
        for(Int_t row = 0; row < NRows; row++){
           for(Int_t p = 0; p < cl_padMult[row]; p++){
              pad[row][p] = pads_fired[row][p];
              charge[row][p] = pad_charge[row][p];
              totalCharge[row] += charge[row][p];
              x_i[p] = 2.5 + (5 * pad[row][p]);
              x[row] += (x_i[p] * charge[row][p]);
           }
           x[row] = x[row] / (totalCharge[row]);
           h_cog[row]->Fill(x[row]);
           totalCharge[row] = 0.;
        }
      }
   }
   
//#################################################################################################################
// Threshold subtraction
//#################################################################################################################
   
   /*  Open files  */
   openTrackFileTh(run);
   tree->Print();

   /*  Data loop for threshold subtraction evts.  */
   
   for(Int_t i=0; i<entries; i++){
      /*for(Int_t row=0; row<NRows; row++){
         h_cogTH[row]->Reset();
      }*/
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>2000){
        for(Int_t row = 0; row < NRows; row++){
           for(Int_t p = 0; p < cl_padMult[row]; p++){
              pad_provv[row][p] = pads_fired[row][p];
              charge_provv[row][p] = pad_charge[row][p];
              totalCharge_provv[row] += charge_provv[row][p];
              x_i_provv[p] = 2.5 + (5 * pad_provv[row][p]);
              x_provv[row] += (x_i_provv[p] * charge_provv[row][p]);
           }
           x_provv[row] = x_provv[row] / (totalCharge_provv[row]);
           h_cogTH[row]->Fill(x_provv[row]);
           totalCharge_provv[row] = 0.;
        }
      }
   }

//#################################################################################################################
// Viusalization block
//#################################################################################################################

   TLegend* l = new TLegend(0.1,0.9,0.6,1.);
   l->SetTextSize(0.035);
   l->SetLineWidth(1);
   l->SetFillStyle(0);
   l->AddEntry(h_cog[0], "Charge distrib. + tresh.", "f");
   l->AddEntry(h_cogTH[0], "Charge distrib. - thresh.", "f");
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      h_cog[row]->GetXaxis()->SetRangeUser(h_cog[row]->GetMean()-8,h_cog[row]->GetMean()+8);
      h_cog[row]->Draw();
      h_cogTH[row]->GetXaxis()->SetRangeUser(h_cogTH[row]->GetMean()-8,h_cogTH[row]->GetMean()+8);
      h_cogTH[row]->Draw("SAME");
      l->Draw("SAME");
   }
   
   char titoloCanvas[100];
   sprintf(titoloCanvas,"COG/cog_%d.png",run);
   c->SaveAs(titoloCanvas);
   sprintf(titoloCanvas,"COG/cog_%d.eps",run);
   c->SaveAs(titoloCanvas);
}
