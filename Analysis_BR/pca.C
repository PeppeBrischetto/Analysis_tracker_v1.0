//################################################################################################################
//#   This macro allows to perform PCA
//#   the runID is required
//#
//################################################################################################################
//#   creatd July 2025 by A. Pitronaci 
//################################################################################################################
//#   updated:
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

void pca(int run){

//################################################################################################################
// Variables
   
   Double_t pad_fired0[NStrips] = {0.};
   Double_t pad_fired1[NStrips] = {0.};
   Double_t pad_fired2[NStrips] = {0.};
   Double_t pad_fired3[NStrips] = {0.};
   Double_t pad_fired4[NStrips] = {0.};
   Double_t pad_charge[NStrips] = {0.};
   
   TPrincipal *obj = new TPrincipal(20,"ND");
   
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//################################################################################################################
// Data loop for PCA
   for(Int_t i=0; i<entries; i++){
      Double_t x_mm[NRows] = {0.};
      Double_t y_mm[NRows] = {0.};
      Double_t std_x_mm[NRows] = {0.};
      Double_t pad_mult[NRows] = {0.};
      Double_t pad_fired[NStrips] = {0.};
      Double_t charge[NStrips] = {0.};
      Double_t phi = 0.;
      Double_t theta = 0.;
      
      tree->GetEntry(i);
      phi = phi_deg;
      theta = theta_deg;
      
      for(Int_t row=0; row<NRows; row++){
         x_mm[row] = cl_x_mm[row];
         y_mm[row] = cl_y_mm[row];
         std_x_mm[row] = 5*cl_x_rms[row];
         pad_mult[row] = cl_padMult[row];
         
         for(Int_t pad=0; pad<cl_padMult[row]; pad++){
            pad_fired[pad] = pads_fired[row][pad];
            charge[pad] = pad_charge[row];
         }
      }
      obj->AddRow(x_mm);
      obj->AddRow(y_mm);
      obj->AddRow(std_x_mm);
      obj->AddRow(pad_mult);
      obj->AddRow(pad_fired);
      obj->AddRow(charge);
   }
  
   obj->MakePrincipals();
   obj->Print();
   obj->Test();
   obj->MakeHistograms();
   

}
