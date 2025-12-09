//#################################################################################################################
//#   This macro allows to obtain the centre of gravity of each rows from a tracks_ID.root file, thus studying its
//#   behaviour as a function of the thresholds. It refers to a single track event.
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

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 6;
const Int_t NPads = 60;

void signalToNoise_Vthgem(int run){

//#################################################################################################################
// Nedeed variables
   char histoname[100];
   char titolox[100];
   char tit_canva[100];
   Double_t v = 0.;
   Double_t x[NRows] = {0.};
   Double_t x_i[NRows] = {0.};
   Double_t totalCharge[NRows] = {0.};
   Double_t pad[NRows][100] = {0.};
   Double_t charge[NRows][100] = {0.};
   Int_t thresh = 0;
   Double_t th[NRows] = {400,400,650,300,300};
   Double_t noise[NRows] = {0.};
   Double_t sn[NRows] = {0.};
   ofstream output;
   
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
   
   TH1D *h_sn[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_sn %i",i);
      sprintf(titolox,"S/N_{row:%i}",i);
      h_sn[i]=new TH1D("","",10000,-100.,100.);
      h_sn[i]->GetXaxis()->SetTitle(titolox);
      h_sn[i]->GetXaxis()->SetTitleSize(0.05);
      h_sn[i]->GetXaxis()->SetLabelSize(0.05);
      h_sn[i]->GetXaxis()->SetTitleOffset(0.8);
      //h_sn[i]->GetYaxis()->SetTitle("Counts");
      h_sn[i]->GetYaxis()->SetTitleSize(0.05);
      h_sn[i]->GetYaxis()->SetLabelSize(0.05);
      h_sn[i]->GetYaxis()->SetTitleOffset(1.);
      h_sn[i]->SetNdivisions(7);
      h_sn[i]->SetFillColor(kBlue-5);
      h_sn[i]->SetFillStyle(3001);
   }
   
//#################################################################################################################
// Open files
   openTrackFile(run);
   tree->Print();
   
   cout << "Insert V_THGEM: ";
   cin >> v;
//#################################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
   
      tree->GetEntry(i);
      if(sic_fired==1 && sic_charge>2000){
        
        for(Int_t row = 0; row < NRows; row++){
           for(Int_t p = 0; p < cl_padMult[row]; p++){
              pad[row][p] = pads_fired[row][p];
              charge[row][p] = pad_charge[row][p];
              charge[row][p] = pad_charge[row][p];
              h_charge[row]->SetBinContent(pads_fired[row][p], pad_charge[row][p]);
              totalCharge[row] += charge[row][p];
              x_i[p] = 2.5 + (5 * pad[row][p]);
              x[row] += (x_i[p] * charge[row][p]);
              totalCharge[row] += charge[row][p];
           }
           x[row] = x[row] / (totalCharge[row]);
           noise[row] = cl_padMult[row]*th[row];
           sn[row] = cl_charge[row]/noise[row];
           cout << "Tot charge: "<< totalCharge[row] << "   threshold: " << noise[row] << "   S/N: " << sn[row] << endl; 
           h_sn[row]->Fill(sn[row]);
           totalCharge[row] = 0.;
        }
      }
   }
   
   f->Close();
   
//#################################################################################################################
// Visualization-results block
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      cout << "x_" << row << ": " << x[row] << endl;
      c->cd(row+1);
      h_sn[row]->Draw();
   }
   sprintf(tit_canva,"SN_ratio_run%d.png",run);
   c->SaveAs(tit_canva);

   output.open("SNratio.txt",std::ios::app);
   if(!output.is_open()){
     cout << "Opening output file failed!" << endl;
   }else{
           output << v;
           for(Int_t row=0; row<NRows; row++){
              output << "   " << h_sn[row]->GetMean() << "   " << h_sn[row]->GetMeanError();
           }
           output << endl;
    }
   
}


