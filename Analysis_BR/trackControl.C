//################################################################################################################
//#   This macro allows to perform PCA
//#   the runID is required
//#
//################################################################################################################
//#   Created July 2025 by A. Pitronaci 
//################################################################################################################
//#   Updated:
//################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

void trackControl(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   char titolofile[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   ofstream outputfile;
   TGraph *retta = new TGraph(5);
   retta->SetMarkerStyle(20);
   retta->SetLineWidth(0);
   retta->GetXaxis()->SetTitle("x (mm)");
   retta->GetYaxis()->SetTitle("z (mm)");
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();
   
   sprintf(titolofile,"pTracks_run%d.txt",run);
   outputfile.open(titolofile);
   outputfile << "************ Run " << run << "_4He - theta-discrepancies > 0°.2 ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,30,40);
   cutGli->SetPoint(1,134,167);
   cutGli->SetPoint(2,113,180);
   cutGli->SetPoint(3,22,54);
   cutGli->SetPoint(4,30,39);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,23,12);
   cutGa->SetPoint(1,192,206);
   cutGa->SetPoint(2,172,216);
   cutGa->SetPoint(3,13,26);
   cutGa->SetPoint(4,23,12);

//#################################################################################################
// Data loop
   for(Int_t i = 0; i < entries; i++){
   
      Double_t theta_fit = 0.;
      Double_t pad[NRows][100] = {0.};
      Double_t charge[NRows][100] = {0.};
      Double_t x[NRows] = {0.};
      Double_t x_mm[NStrips] = {0.};
      Double_t y[NRows] = {0.};
      Double_t totalCharge[NRows] = {0.};

      tree->GetEntry(i);
      retta->Set(0);

      TF1* f = new TF1(Form("f_%d", i), "[0] + [1]*x", 0, 300);
      f->SetParameters(0, 0);

      //if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      for(Int_t row = 0; row < NRows; row++){
         for(Int_t p = 0; p < cl_padMult[row]; p++){
            pad[row][p] = pads_fired[row][p];
            charge[row][p] = pad_charge[row][p];
            totalCharge[row] += charge[row][p];
            anode->Fill(pad[row][p], 2 * row + 1, charge[row][p]);
            x_mm[p] = 2.5 + (5 * pad[row][p]);
            x[row] += (x_mm[p] * charge[row][p]);
         }
         x[row] = x[row] / (totalCharge[row]);
         retta->AddPoint(x[row], z[row]);
      }

      retta->Fit(f, "", "+", 0, 300);
      theta_fit = 90 - ((ATan(f->GetParameter(1))) * 180 / Pi());
      thetaDeg = ATan((x[4] - x[0]) / 84.8) * 180 / Pi();

      if(theta_deg>0){
        Double_t discr = Abs(theta_fit - theta_deg);
        if(discr>0.2){
           cout << "Find strange event: Evt: " << i << "   sic_fired: " << sic_fired << "   sic_charge: " << sic_charge << endl;
           outputfile << "  Evt: " << i << "   theta_fit: " << theta_fit << "   theta_deg: " << theta_deg << endl;
          } else {
           //cout << "Track control: positive!" << endl;
          }
     } else{
        Double_t discropp = Abs(theta_fit-(180+theta_deg));
        if(discropp>0.2){
           cout << "Find strange event: Evt: " << i << "   sic_fired: " << sic_fired << "   sic_charge: " << sic_charge << endl;
           outputfile << "  Evt: " << i << "   theta_fit: " << theta_fit << "   theta_deg: " << 180+theta_deg << endl;
          } else {
           //cout << "Track control: positive!" << endl;
          }
        }
      //} // TCut parenthesis

      anode->Reset("ICES");
   }

   outputfile << "*************************** entries: " << entries << " *****************************" << endl;
}
