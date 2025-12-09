//#################################################################################################################
//#   This macro allows to obtain the charge spectrum for the pad of each rows from a tracks_ID.root file.
//#################################################################################################################
//#   Created November 2025 by A. Pitronaci
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

#include "../Include/openfilesmerged.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void charge_per_pad_alfa(int run){

//################################################################################################################
// Needed variables
   
   char histoname[100];
   char titolox[100];
   Int_t pad0 = 0;
   Int_t pad1 = 0;
   
   TH2I *h_padPad = new TH2I("","",60,-0.5,59.5,60,-0.5,59.5);
   h_padPad->GetXaxis()->SetTitle("Pads_{row 0}");
   h_padPad->GetXaxis()->SetTitleSize(0.05);
   h_padPad->GetXaxis()->SetLabelSize(0.05);
   h_padPad->GetXaxis()->SetTitleOffset(0.92);
   h_padPad->GetYaxis()->SetTitle("Pads_{row 1}");
   h_padPad->GetYaxis()->SetTitleSize(0.05);
   h_padPad->GetYaxis()->SetLabelSize(0.05);
   h_padPad->GetYaxis()->SetTitleOffset(1.);
   
//################################################################################################################
// OpenFiles
   openMergedFile(run);
   tree->Print();   

//################################################################################################################
// Data-loop
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      if(Row==0){
        pad0=Pads;
      }else
       if(Row==1){
         pad1=Pads;
       }
      h_padPad->Fill(pad0,pad1);
   }   

//################################################################################################################
// Visualization block
   
   TCanvas *c = new TCanvas("c","c",800,800);
   c->cd();
   h_padPad->Draw("COLZ");
}
