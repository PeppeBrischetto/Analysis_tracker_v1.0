//################################################################################################################
//#   This macro allows to perform a quality control on the track events coming from the IRRAD4 experiment.
//#   In particular, for each track, this macro extrapolates the (pad,row)-pairs, thus makes a fit of them and 
//#   plots the R-coefficient (i.e. the Pearson correlation coefficient) in order to discern pathological events.
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

void trackQualityControl(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   char titolofile[100];
   char histoname[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   Double_t chi_root[NRows] = {0.};
   Double_t my_chi[NRows] = {0.};

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *discr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"discr %i",i);
      discr[i]=new TH1D("","",500,-20,20);
      discr[i]->GetXaxis()->SetTitle("|x_{i} - f(z_{i})|");
      discr[i]->GetXaxis()->SetTitleSize(0.05);
      discr[i]->GetXaxis()->SetLabelSize(0.05);
      discr[i]->GetXaxis()->SetTitleOffset(.9);
      discr[i]->GetYaxis()->SetTitle("Counts");
      discr[i]->GetYaxis()->SetTitleSize(0.05);
      discr[i]->GetYaxis()->SetLabelSize(0.05);
      discr[i]->GetYaxis()->SetTitleOffset(1.);
      discr[i]->SetNdivisions(7);
      discr[i]->SetFillColor(kBlue-5);
   }
   
   TH1D *amplitude[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"amplitude %i",i);
      amplitude[i]=new TH1D("","",500,-20,20);
      amplitude[i]->GetXaxis()->SetTitle("#sqrt{(x_{i} - f(z_{i}))^{2}}");
      amplitude[i]->GetXaxis()->SetTitleSize(0.05);
      amplitude[i]->GetXaxis()->SetLabelSize(0.05);
      amplitude[i]->GetXaxis()->SetTitleOffset(.9);
      amplitude[i]->GetYaxis()->SetTitle("Counts");
      amplitude[i]->GetYaxis()->SetTitleSize(0.05);
      amplitude[i]->GetYaxis()->SetLabelSize(0.05);
      amplitude[i]->GetYaxis()->SetTitleOffset(1.);
      amplitude[i]->SetNdivisions(7);
      amplitude[i]->SetFillColor(kBlue-5);
   }
   
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
   
   //sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d_4He.txt",run);
   //outputfile.open(titolofile);
   //outputfile << "************ Run " << run << "_4He - pearson coefficient ***********" << endl; 

//###########################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",5);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,14.6,30.);
   cutGli->SetPoint(1,88,128.6);
   cutGli->SetPoint(2,58.6,140.6);
   cutGli->SetPoint(3,6.4,57.);
   cutGli->SetPoint(4,14.6,30.);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,11.7,13.7);
   cutGa->SetPoint(1,134.5,158.4);
   cutGa->SetPoint(2,116.6,168.3);
   cutGa->SetPoint(3,5.4,30.);
   cutGa->SetPoint(4,11.7,13.7);

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
      Double_t scarto[NRows] = {0.};
      Double_t ampiezza[NRows] = {0.};

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
         retta->Fit(f,"","+",0,300);
         scarto[row] = x[row] - (f->GetX(z[row]));
         discr[row]->Fill(scarto[row]);
         ampiezza[row] = sqrt(scarto[row]*scarto[row]);
         amplitude[row]->Fill(ampiezza[row]);
         my_chi[row] += ((x[row] - (f->GetX(z[row])))*(x[row] - (f->GetX(z[row])))/(f->GetX(z[row])));
         chi_root[row] += f->GetChisquare();
      }
      
      //}                                                                      // TCutg parenthesis
      
      
      anode->Reset("ICES");
   }
   
   for(Int_t row = 0; row < NRows; row++){
      my_chi[row] = my_chi[row]/((entries-1)*(entries-1));
      chi_root[row] = chi_root[row]/((entries-1)*(entries-1));
      cout << "*** ROW " << row << " ***" << endl;
      cout << "Manual chi-square: " << my_chi[row] << "    chi-square from root: " << chi_root[row] << endl;
   }
   
   cout << "Entries: " << entries << endl;
   
   
//   outputfile << "*************************** entries: " << entries << " *****************************" << endl;
   
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->AddEntry(discr[0], "x[row] - (f(z[row]))", "f");
   
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->AddEntry(amplitude[0], "x[row] - (f(z[row]))", "f");
   
   TCanvas *c = new TCanvas();
   c->Divide(3,2);
   c->cd(1);
   discr[0]->Draw();
   TLine* centre0 = new TLine(0.,0.,0.,discr[0]->GetMaximum());
   l->AddEntry(centre0,"x[row] - (f(z[row])) = 0","l");
   centre0->SetLineColor(kRed);
   centre0->Draw("SAME");
   l->Draw("SAME");
   c->cd(2);
   discr[1]->Draw();
   TLine* centre1 = new TLine(0.,0.,0.,discr[1]->GetMaximum());
   centre1->SetLineColor(kRed);
   centre1->Draw("SAME");
   l->Draw("SAME");
   c->cd(3);
   discr[2]->Draw();
   TLine* centre2 = new TLine(0.,0.,0.,discr[2]->GetMaximum());
   centre2->SetLineColor(kRed);
   centre2->Draw("SAME");
   l->Draw("SAME");
   c->cd(4);
   discr[3]->Draw();
   TLine* centre3 = new TLine(0.,0.,0.,discr[3]->GetMaximum());
   centre3->SetLineColor(kRed);
   centre3->Draw("SAME");
   l->Draw("SAME");
   c->cd(5);
   discr[4]->Draw();
   TLine* centre4 = new TLine(0.,0.,0.,discr[4]->GetMaximum());
   centre4->SetLineColor(kRed);
   centre4->Draw("SAME");
   l->Draw("SAME");
   
   TCanvas *c1 = new TCanvas();
   c1->Divide(3,2);
   c1->cd(1);
   amplitude[0]->Draw();
   TLine* centre10 = new TLine(0.,0.,0.,amplitude[0]->GetMaximum());
   l1->AddEntry(centre0,"x[row] - (f(z[row])) = 0","l");
   centre10->SetLineColor(kRed);
   centre10->Draw("SAME");
   l1->Draw("SAME");
   c1->cd(2);
   amplitude[1]->Draw();
   TLine* centre11 = new TLine(0.,0.,0.,amplitude[1]->GetMaximum());
   centre11->SetLineColor(kRed);
   centre11->Draw("SAME");
   l1->Draw("SAME");
   c1->cd(3);
   amplitude[2]->Draw();
   TLine* centre12 = new TLine(0.,0.,0.,amplitude[2]->GetMaximum());
   centre12->SetLineColor(kRed);
   centre12->Draw("SAME");
   l1->Draw("SAME");
   c1->cd(4);
   amplitude[3]->Draw();
   TLine* centre13 = new TLine(0.,0.,0.,amplitude[3]->GetMaximum());
   centre13->SetLineColor(kRed);
   centre13->Draw("SAME");
   l1->Draw("SAME");
   c1->cd(5);
   amplitude[4]->Draw();
   TLine* centre14 = new TLine(0.,0.,0.,amplitude[4]->GetMaximum());
   centre14->SetLineColor(kRed);
   centre14->Draw("SAME");
   l1->Draw("SAME");
   
   /*Int_t hpos = 0.5*pearson->GetMaximum();
   char testo[100];
   sprintf(testo,"Run %d - ^{4}He tracks",run);
   TLatex *t = new TLatex(0.93,hpos,testo);
   TCanvas *c = new TCanvas();
   c->cd();
   pearson->Draw();
   t->Draw("SAME");
   sprintf(testo,"Pictures_Analysis/Perason_correlation_analysis/run%d_4He.png",run);
   c->SaveAs(testo);*/


}
