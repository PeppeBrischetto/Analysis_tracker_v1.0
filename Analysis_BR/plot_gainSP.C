//################################################################################################################
//#   This macro allows to plot the gain G=I_{an}/I_{primaries} starting from a Data_Table file
//################################################################################################################
//#   Created September 2025 by A. Pitronaci 
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

void plot_gainSP(){

//################################################################################################################
// Needed variables
   
   ifstream infile;
   char titolofile[100];
   string dummy;
   Double_t v,g=0.;
   
//################################################################################################################ 
// OpenFiles
   
   sprintf(titolofile,"Gain/gainTHGEM.txt");
   infile.open(titolofile);
     
   TGraph *gain = new TGraph();

//################################################################################################################ 
// Loop file
   
   getline(infile,dummy);
   for(Int_t i=1; i<10; i++){
         infile << v << g;
         gain->SetPoint(i,v,g);
      }
   
//################################################################################################################ 
// Visualization block
   
   TCanvas *c = new TCanvas("c");
   c->cd();
   gain->Draw();      
}
