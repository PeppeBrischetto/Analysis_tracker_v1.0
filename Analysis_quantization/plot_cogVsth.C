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

const TString InputFile = "CoG_Th.txt";

void plot_cogVsth(){
   
//#################################################################################################################
// Nedeed variables
   ifstream file;
   string dummy;
   Double_t x[NRows] = {0.};
   Double_t th[14] = {0,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600};
   char graphname[100];
   char tit[100]; 
   
   TGraph *gr[NRows];                                                                      // TGraph-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(graphname,"row %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr[i] = new TGraph();
      gr[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr[i]->GetYaxis()->SetTitle(tit);
      gr[i]->GetXaxis()->SetTitleSize(0.05);
      gr[i]->GetXaxis()->SetLabelSize(0.05);
      gr[i]->GetXaxis()->SetTitleOffset(0.9);
      gr[i]->GetYaxis()->SetTitleSize(0.05);
      gr[i]->GetYaxis()->SetLabelSize(0.05);
      gr[i]->GetYaxis()->SetTitleOffset(1.5);
      gr[i]->SetLineColor(kBlue);
      gr[i]->SetMarkerStyle(20);
      gr[i]->SetMarkerColor(kBlue);
   }

//#################################################################################################################
// Open file
   file.open(InputFile);
   if(!file.is_open()){
     cout << "Opening output file failed!" << endl;
   }else{
           for(Int_t j=0; j<14; j++){
              file >> dummy >> x[0] >> x[1] >> x[2] >> x[3] >> x[4];
              for(Int_t row=0; row<NRows; row++){
                 cout << "   x_" << row << ": " << x[row] << endl;
                 gr[row]->SetPoint(j,th[j],x[row]);
              }
           }
           
     }

//#################################################################################################################
// Visualization block
   
   TCanvas *c = new TCanvas("c","c",2200,2200);
   c->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
      c->cd(row+1);
      gr[row]->Draw();
   }

}
