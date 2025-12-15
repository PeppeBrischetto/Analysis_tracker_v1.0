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

void chargeDistrib(int run){

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
   
   TH1D *h_charge[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_charge %i",i);
      sprintf(titolox,"pad_{row:%i}",i);
      h_charge[i]=new TH1D("","",60,-0.5,59.5);
      h_charge[i]->GetXaxis()->SetTitle(titolox);
      h_charge[i]->GetXaxis()->SetTitleSize(0.05);
      h_charge[i]->GetXaxis()->SetLabelSize(0.05);
      h_charge[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_charge[i]->GetYaxis()->SetTitle("Counts");
      h_charge[i]->GetYaxis()->SetTitleSize(0.05);
      h_charge[i]->GetYaxis()->SetLabelSize(0.05);
      h_charge[i]->GetYaxis()->SetTitleOffset(1.);
      h_charge[i]->SetNdivisions(7);
      h_charge[i]->SetFillColor(kBlue-5);
      h_charge[i]->SetFillStyle(3001);
   }
   
   TH1D *h_chargeTH[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_chargeTH %i",i);
      sprintf(titolox,"pad_{row:%i}",i);
      h_chargeTH[i]=new TH1D("","",60,-0.5,59.5);
      h_chargeTH[i]->GetXaxis()->SetTitle(titolox);
      h_chargeTH[i]->GetXaxis()->SetTitleSize(0.05);
      h_chargeTH[i]->GetXaxis()->SetLabelSize(0.05);
      h_chargeTH[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_chargeTH[i]->GetYaxis()->SetTitle("Counts");
      h_chargeTH[i]->GetYaxis()->SetTitleSize(0.05);
      h_chargeTH[i]->GetYaxis()->SetLabelSize(0.05);
      h_chargeTH[i]->GetYaxis()->SetTitleOffset(1.);
      h_chargeTH[i]->SetNdivisions(7);
      h_chargeTH[i]->SetFillColor(kRed-5);
      h_chargeTH[i]->SetLineColor(kRed);
      h_chargeTH[i]->SetFillStyle(3001);
   }

//#################################################################################################################
// NO-threshold subtraction
//#################################################################################################################

   /*  Open files  */
   openTrackFile(run);
   tree->Print();

   /*  Data loop for NO-threshold subtraction evts.  */
   
   for(Int_t i=evt; i<evt+1; i++){
      for(Int_t row=0; row<NRows; row++){
         h_charge[row]->Reset();
      }
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>2000){
        for(Int_t row = 0; row < NRows; row++){
           for(Int_t p = 0; p < cl_padMult[row]; p++){
              pad[row][p] = pads_fired[row][p];
              charge[row][p] = pad_charge[row][p];
              totalCharge[row] += charge[row][p];
              x_i[p] = 2.5 + (5 * pad[row][p]);
              x[row] += (x_i[p] * charge[row][p]);
              h_charge[row]->SetBinContent(pads_fired[row][p],pad_charge[row][p]);
           }
           x[row] = x[row] / (totalCharge[row]);
           
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
   
   for(Int_t i=evt; i<evt+1; i++){
      for(Int_t row=0; row<NRows; row++){
         h_chargeTH[row]->Reset();
      }
      
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>2000){
        for(Int_t row = 0; row < NRows; row++){
           for(Int_t p = 0; p < cl_padMult[row]; p++){
              pad_provv[row][p] = pads_fired[row][p];
              charge_provv[row][p] = pad_charge[row][p];
              totalCharge_provv[row] += charge_provv[row][p];
              x_i_provv[p] = 2.5 + (5 * pad_provv[row][p]);
              x_provv[row] += (x_i_provv[p] * charge_provv[row][p]);
              h_chargeTH[row]->SetBinContent(pad_provv[row][p],charge_provv[row][p]);
           }
           x_provv[row] = x_provv[row] / (totalCharge_provv[row]);
           
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
   l->AddEntry(h_charge[0], "Charge distrib. + tresh.", "f");
   l->AddEntry(h_chargeTH[0], "Charge distrib. - thresh.", "f");
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      h_charge[row]->Draw();
      h_chargeTH[row]->Draw("SAME");
      l->Draw("SAME");
   }
   
   char titoloCanvas[100];
   sprintf(titoloCanvas,"ChargeDistrib/Run%d/chargeDistrib_%d.png",run,evt);
   c->SaveAs(titoloCanvas);
   sprintf(titoloCanvas,"ChargeDistrib/Run%d/chargeDistrib_%d.eps",run,evt);
   c->SaveAs(titoloCanvas);
}
