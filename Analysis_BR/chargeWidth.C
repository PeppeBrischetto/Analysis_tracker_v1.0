//###################################################################################################
//#   This macro allows you to obtain the lateral dispersion (angular straggling) from a SRIM-output.
//#   REMEMBER: to correctly read the SRIM-output file, first transform "," in "."
//#
//###################################################################################################
//#   creatd jan 2025 by A. Pitronaci
//#
//#######################################
//#
//#   updated: January 2025 by A. Pitronaci
//# 
//###################################################################################################

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


void chargeWidth(int run)
  {
//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//################################################################################################################
// Needed variables
   TH1D *h = new TH1D("h","",800,150,200);
   h->GetXaxis()->SetTitle("x (mm)");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   
   TH1D *h1 = new TH1D("h1","",800,150,200);
   h1->GetXaxis()->SetTitle("x (mm)");
   h1->GetYaxis()->SetTitle("Counts");
   h1->SetLineColor(kMagenta+1);
   
   TH1D *h2 = new TH1D("h2","",800,150,200);
   h2->GetXaxis()->SetTitle("x (mm)");
   h2->GetYaxis()->SetTitle("Counts");
   h2->SetLineColor(kGreen+2);
   
   TH1D *h3 = new TH1D("h3","",800,150,200);
   h3->GetXaxis()->SetTitle("x (mm)");
   h3->GetYaxis()->SetTitle("Counts");
   h3->SetLineColor(kRed);
   

   
//################################################################################################################
// Data loop
   
   tree->Project("h","cl_x_mm[3]-(-1.69285)");
   tree->Project("h1","cl_x_mm[3]-(-1.69285)","cl_x_mm[4]-0.92361 >222 && cl_x_mm[4]-0.92361<224");
   tree->Project("h2","cl_x_mm[3]-(-1.69285)","cl_x_mm[4]-0.92361>222.5 && cl_x_mm[4]-0.92361<223.5");
   tree->Project("h3","cl_x_mm[3]-(-1.69285)","cl_x_mm[4]-0.92361>222.75 && cl_x_mm[4]-0.92361<223.25");
   
//################################################################################################################
// Visualization block

   TLegend *l = new TLegend();
   l->AddEntry(h,"no slit","f");
   l->AddEntry(h1,"2 mm slit","f");
   l->AddEntry(h2,"1 mm slit","f");
   l->AddEntry(h3,"0.5 mm slit","f");
   
   
   
   h1->Scale(1.3);
   h2->Scale(2.4);
   h3->Scale(4.2);
   TCanvas *c = new TCanvas("c","c",900,900);
   c->cd();
   h->Draw();
   h1->Draw("HISTO && SAME");
   h2->Draw("HISTO && SAME");
   h3->Draw("HISTO && SAME");
   l->Draw("SAME");   
   
  }
