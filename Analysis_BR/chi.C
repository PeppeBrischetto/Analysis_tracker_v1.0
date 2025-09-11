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

void chi(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   char titolofile[100];
   char histoname[100];
   Double_t thetaDeg = 0.;
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   Double_t chi_root = 0.;
   Double_t my_chi = 0.;
   Double_t my_chiRed = 0.;

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *h_chi[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"h_chi %i",i);
      h_chi[i]=new TH1D("","",50,-0.01,8.);
      h_chi[i]->GetXaxis()->SetTitle("#chi^{2}");
      h_chi[i]->GetXaxis()->SetTitleSize(0.05);
      h_chi[i]->GetXaxis()->SetLabelSize(0.05);
      h_chi[i]->GetXaxis()->SetTitleOffset(.9);
      h_chi[i]->GetYaxis()->SetTitle("Counts");
      h_chi[i]->GetYaxis()->SetTitleSize(0.05);
      h_chi[i]->GetYaxis()->SetLabelSize(0.05);
      h_chi[i]->GetYaxis()->SetTitleOffset(1.);
      //h_chi[i]->SetStats(0);
      h_chi[i]->SetNdivisions(7);
      //h_chi[i]->SetFillColor(i+1);
      Double_t r = i+1;
      if(r==5){
         h_chi[i]->SetLineColor(kMagenta+2);
      } else{
                h_chi[i]->SetLineColor(r);
      }
      h_chi[i]->SetLineWidth(2);
   }
   
   TH1D *discr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"discr %i",i);
      discr[i]=new TH1D("","",500,-20,20);
      discr[i]->GetXaxis()->SetTitle("|x_{i} - f(z_{i})| (mm)");
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
      amplitude[i]->GetXaxis()->SetTitle("#sqrt{(x_{i} - f(z_{i}))^{2}} (mm)");
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
   cutGli->SetPoint(0,33,41);
   cutGli->SetPoint(1,135,165);
   cutGli->SetPoint(2,115,176);
   cutGli->SetPoint(3,20,53);
   cutGli->SetPoint(4,33,41);
   
   TCutG *cutGa = new TCutG("cutGa",5);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,25,16);
   cutGa->SetPoint(1,195,207);
   cutGa->SetPoint(2,173,216);
   cutGa->SetPoint(3,13,22);
   cutGa->SetPoint(4,25,16);

//#################################################################################################
// Data loop
   for(Int_t i = 0; i < 5; i++){
      my_chi = 0.;
      chi_root = 0.;
      my_chiRed = 0.;
      
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
         Double_t my_Singlechi = 0.;
         x[row] = x[row] / (totalCharge[row]);
         retta->AddPoint(x[row], z[row]);
         /*if(row==5){
            retta->Fit(f,"","+",0,300);
         scarto[row] = x[row] - (f->GetX(z[row]));
         discr[row]->Fill(scarto[row]);
         ampiezza[row] = sqrt(scarto[row]*scarto[row]);
         amplitude[row]->Fill(ampiezza[row]);
         my_Singlechi = ((x[row] - (f->GetX(z[row])))*(x[row] - (f->GetX(z[row])))/(f->GetX(z[row])));
         //h_chi[row]->Fill(my_Singlechi);
         my_chi[row] += ((x[row] - (f->GetX(z[row])))*(x[row] - (f->GetX(z[row])))/(f->GetX(z[row])));
         chi_root[row] = f->GetChisquare();
         h_chi[row]->Fill(chi_root[row]);*/
      }
      
      TCanvas *c_retta = new TCanvas("c_retta");
      char fit[100];
      sprintf(fit,"f_%d",i);
      c_retta->cd();
      retta->Fit(fit,"","+",0,300);
      retta->Draw();
      
      
      for(Int_t row=0; row<NRows; row++){
         my_chi += (pow(z[row]-f->Eval(x[row]),2)/1);
      }
      
      cout << "My chi: " << my_chi << endl;
      //}                                                                      // TCutg parenthesis
      cout << " *************************************** " << endl;
      for(Int_t row=0; row<NRows; row++){
         cout << "Event: " << i << endl;
         printf("x%d= %1.4f and z%d= %1.4f\n ",i,x[row],i,z[row]);
      }
      
      anode->Reset("ICES");
   }
   
  /* for(Int_t row = 0; row < NRows; row++){
      my_chi[row] = my_chi[row];
      chi_root[row] = chi_root[row];
      cout << "*** ROW " << row << " ***" << endl;
      cout << "Manual chi-square: " << my_chi[row] << "    chi-square from root: " << chi_root[row] << endl;
   }*/
   
   cout << "Entries: " << entries << endl;
   
   
//   outputfile << "*************************** entries: " << entries << " *****************************" << endl;
   
   TLegend* l = new TLegend(0.1,0.7,0.3,0.9);
   l->SetTextSize(0.035);
   l->AddEntry(discr[0], "x[row] - (f(z[row]))", "f");
   
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->AddEntry(amplitude[0], "x[row] - (f(z[row]))", "f");
   
   TCanvas *c = new TCanvas("c","c",1600,500);
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
   
   TCanvas *c1 = new TCanvas("c1","c1",1600,500);
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
   
   TCanvas *c2 = new TCanvas("c2");
   TLegend* l_chi = new TLegend(0.1,0.6,0.3,0.9);
   l_chi->SetTextSize(0.035);
   for(Int_t j=0; j<NRows; j++){
      char testo_h[50];
      sprintf(testo_h,"#chi^{2}_{row%d}",j);
      l_chi->AddEntry(h_chi[j],testo_h, "f");
   }
   c2->cd();
   h_chi[1]->SetLineStyle(5);
   h_chi[0]->Draw();
   h_chi[1]->Draw("SAME");
   h_chi[2]->Draw("SAME");
   h_chi[3]->Draw("SAME");
   h_chi[4]->Draw("SAME");
   l_chi->Draw("SAME");
   
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.35,0.35,0.95,0.95);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->Draw();
   zoomPad->cd();

   TH1D *hZoom0 = (TH1D*)h_chi[0]->Clone("hZoom0");
   hZoom0->GetXaxis()->SetRangeUser(0.1, 8);
   hZoom0->GetYaxis()->SetRangeUser(0.,1400);
   hZoom0->Draw();
   TH1D *hZoom1 = (TH1D*)h_chi[1]->Clone("hZoom1");
   hZoom1->GetXaxis()->SetRangeUser(0.1, 8); 
   hZoom1->GetYaxis()->SetRangeUser(0.,1400);
   hZoom1->Draw("SAME");
   TH1D *hZoom2 = (TH1D*)h_chi[2]->Clone("hZoom2");
   hZoom2->GetXaxis()->SetRangeUser(0.1, 8); 
   hZoom2->GetYaxis()->SetRangeUser(0.,1400);
   hZoom2->Draw("SAME");
   TH1D *hZoom3 = (TH1D*)h_chi[3]->Clone("hZoom3");
   hZoom3->GetXaxis()->SetRangeUser(0.1, 8); 
   hZoom3->GetYaxis()->SetRangeUser(0.,1400);
   hZoom3->Draw("SAME");
   TH1D *hZoom4 = (TH1D*)h_chi[4]->Clone("hZoom4");
   hZoom4->GetXaxis()->SetRangeUser(0.1, 8); 
   hZoom4->GetYaxis()->SetRangeUser(0.,1400);
   hZoom4->Draw("SAME");
   
   char titolo0[100];
   sprintf(titolo0,"Discrepancies_run%d.png",run);
   char titolo1[100];
   sprintf(titolo1,"Amplitudes_run%d.png",run);
   //c->SaveAs(titolo0);
   //c1->SaveAs(titolo1);


}
