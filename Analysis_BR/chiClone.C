//################################################################################################################
//#   This macro allows to perform a chi^2 quality control on the track events coming from the IRRAD4 experiment.
//#   In particular, for each track, this macro extrapolates the (pad,row)-pairs, thus makes a fit of them and 
//#   plots the chi^2 in order to discern pathological events. Furthermore, the discrepancies and the amplitude 
//#   histograms are plotted.
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

void chiClone(int run){

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
   Double_t Ndof = 3;
   Double_t offset[NRows] = {-0.488407,1.90537,-0.575092,-2.34135,1.48288};
   Double_t x_corr[NRows] = {0.};
   Double_t newOffset[NRows] = {0.};
   Double_t chiRed_min1 = 0.;
   Double_t chiRed_tot = 0.;
   Double_t rapp = 0.;
   
   Double_t theta_deg = 0.;
   Double_t intercetta = 0.;
   
   char provv;

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
 
   TH1D *h_theta = new TH1D("h_theta","",900,0,90);
   h_theta->GetXaxis()->SetTitle("#theta_{foc} (deg)");
   h_theta->GetXaxis()->SetTitleSize(0.05);
   h_theta->GetXaxis()->SetLabelSize(0.05);
   h_theta->GetXaxis()->SetTitleOffset(.9);
   //h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetYaxis()->SetTitleSize(0.05);
   h_theta->GetYaxis()->SetLabelSize(0.05);
   h_theta->GetYaxis()->SetTitleOffset(1.);
   h_theta->SetNdivisions(7);
   h_theta->SetLineWidth(2);
   h_theta->SetStats(0);
   
   TH1D *h_intercetta = new TH1D("h_intercetta","",300,-30,0);
   h_intercetta->GetXaxis()->SetTitle("intecrept (mm)");
   h_intercetta->GetXaxis()->SetTitleSize(0.05);
   h_intercetta->GetXaxis()->SetLabelSize(0.05);
   h_intercetta->GetXaxis()->SetTitleOffset(.9);
   //h_intercetta->GetYaxis()->SetTitle("Counts");
   h_intercetta->GetYaxis()->SetTitleSize(0.05);
   h_intercetta->GetYaxis()->SetLabelSize(0.05);
   h_intercetta->GetYaxis()->SetTitleOffset(1.);
   h_intercetta->SetNdivisions(7);
   h_intercetta->SetLineWidth(2);
   h_intercetta->SetStats(0);
   
   TH1D *h_chi = new TH1D("h_chi","",50,-0.01,10);
   h_chi->GetXaxis()->SetTitle("#chi^{2}");
   h_chi->GetXaxis()->SetTitleSize(0.05);
   h_chi->GetXaxis()->SetLabelSize(0.05);
   h_chi->GetXaxis()->SetTitleOffset(.9);
   h_chi->GetYaxis()->SetTitle("Counts");
   h_chi->GetYaxis()->SetTitleSize(0.05);
   h_chi->GetYaxis()->SetLabelSize(0.05);
   h_chi->GetYaxis()->SetTitleOffset(1.);
   h_chi->SetNdivisions(7);
   h_chi->SetLineWidth(2);
   h_chi->SetStats(0);
   
   TH1D *h_chiRed = new TH1D("h_chiRed","",200,0,10);
   h_chiRed->GetXaxis()->SetTitle("#tilde{#chi}^{2}_red");
   h_chiRed->GetXaxis()->SetTitleSize(0.05);
   h_chiRed->GetXaxis()->SetLabelSize(0.05);
   h_chiRed->GetXaxis()->SetTitleOffset(.9);
   h_chiRed->GetYaxis()->SetTitle("Counts");
   h_chiRed->GetYaxis()->SetTitleSize(0.05);
   h_chiRed->GetYaxis()->SetLabelSize(0.05);
   h_chiRed->GetYaxis()->SetTitleOffset(1.);
   h_chiRed->SetNdivisions(7);
   h_chiRed->SetLineWidth(2);
   
   TH1D *discr[NRows];
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"discr %i",i);
      discr[i]=new TH1D("","",500,-15,15);
      discr[i]->GetXaxis()->SetTitle("(x_{i} - f(z_{i})) (mm)");
      discr[i]->GetXaxis()->SetTitleSize(0.05);
      discr[i]->GetXaxis()->SetLabelSize(0.05);
      discr[i]->GetXaxis()->SetTitleOffset(1.);
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
      amplitude[i]=new TH1D("","",500,-15,15);
      amplitude[i]->GetXaxis()->SetTitle("#sqrt{(x_{i} - f(z_{i}))^{2}} (mm)");
      amplitude[i]->GetXaxis()->SetTitleSize(0.05);
      amplitude[i]->GetXaxis()->SetLabelSize(0.05);
      amplitude[i]->GetXaxis()->SetTitleOffset(1.);
      amplitude[i]->GetYaxis()->SetTitle("Counts");
      amplitude[i]->GetYaxis()->SetTitleSize(0.05);
      amplitude[i]->GetYaxis()->SetLabelSize(0.05);
      amplitude[i]->GetYaxis()->SetTitleOffset(1.);
      amplitude[i]->SetNdivisions(7);
      amplitude[i]->SetFillColor(kBlue-5);
   }
   
   TF1 *norm = new TF1("norm","gaus",-5,5);
   norm->SetParameters(1,0,1);
   
   ofstream outputfile;
   TGraph *retta = new TGraph(5);
   retta->SetTitle("(x_{i},z_{i})-pairs - track reconstruction");
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
   for(Int_t i=0; i<entries; i++){
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

      TF1 *f = new TF1(Form("f_%d", i), "[0] + [1]*x", 0, 300);
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
         x_corr[row] = x[row]-offset[row];
         retta->AddPoint(x_corr[row], z[row]);
         
      }
      
      
      char fit[100];
      sprintf(fit,"f_%d",i);
      retta->Fit(fit,"","+",0,300);
      
      if(i==0 || i==100 || i==200 || i==300 || i==400 || i==500 || i==600 || i==700){
         TCanvas *c_retta = new TCanvas("c_retta");
         c_retta->cd();
         retta->Draw();
         c_retta->Update();
         char tit_retta[100];
         sprintf(tit_retta,"Pictures_Analysis/TrackQualityControl/Run%d/bestTrack_corr%d.png",run,i);
         c_retta->SaveAs(tit_retta);
      }
      
      theta_deg = 90-(ATan(f->GetParameter(1))*180/Pi());
      h_theta->Fill(theta_deg);
      intercetta = f->GetParameter(0);
      h_intercetta->Fill(intercetta);
      
      for(Int_t row=0; row<NRows; row++){
         my_chi += (pow(z[row]-f->Eval(x_corr[row]),2)/1);
         scarto[row] = x_corr[row] - (f->GetX(z[row]));
         discr[row]->Fill(scarto[row]);
         ampiezza[row] = sqrt(scarto[row]*scarto[row]);
         amplitude[row]->Fill(ampiezza[row]);
         if(row==NRows-1){
            h_chi->Fill(my_chi);
         }
      }
      
      my_chiRed = my_chi/Ndof;
      h_chiRed->Fill(my_chiRed);
      
      /* cout trials */
      cout << "My chi: " << my_chi << "    chi^2 reduced: " << endl;
      //}                                                                      // TCutg parenthesis
      /*cout << " *************************************** " << endl;
      cout << "Event: " << i << endl;
      for(Int_t row=0; row<NRows; row++){
         printf("x%d= %1.4f and z%d= %1.4f\n ",i,x[row],i,z[row]);
      }
      cout << "Theta (deg): " << theta_deg << endl;*/

      anode->Reset("ICES");
      
      //cin >> provv;
   }
   
   /* Average discrepancy evaluation by normal fit */
   for(Int_t row=0; row<NRows; row++){
      discr[row]->Fit("norm","","+",-5,5);
      newOffset[row]=norm->GetParameter(1);
      
      cout << "Offset " << row << ": " << newOffset[row] << endl;  
   }
   
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(discr[0], "x_corr[row] - (f(z[row]))", "f");
    
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->AddEntry(amplitude[0], "x_corr[row] - (f(z[row]))", "f");
   l1->SetLineWidth(0);
   
   TCanvas *c = new TCanvas("c","c",1600,1600);
   c->Divide(3,2);
   c->cd(1);
   discr[0]->Draw();
   TLine* centre0 = new TLine(0.,0.,0.,discr[0]->GetMaximum());
   l->AddEntry(centre0,"x_corr[row] - (f(z[row])) = 0","l");
   centre0->SetLineColor(kRed);
   centre0->Draw("SAME");
   //l->Draw("SAME");
   c->cd(2);
   discr[1]->Draw();
   TLine* centre1 = new TLine(0.,0.,0.,discr[1]->GetMaximum());
   centre1->SetLineColor(kRed);
   centre1->Draw("SAME");
   //l->Draw("SAME");
   c->cd(3);
   discr[2]->Draw();
   TLine* centre2 = new TLine(0.,0.,0.,discr[2]->GetMaximum());
   centre2->SetLineColor(kRed);
   centre2->Draw("SAME");
   //l->Draw("SAME");
   c->cd(4);
   discr[3]->Draw();
   TLine* centre3 = new TLine(0.,0.,0.,discr[3]->GetMaximum());
   centre3->SetLineColor(kRed);
   centre3->Draw("SAME");
   //l->Draw("SAME");
   c->cd(5);
   discr[4]->Draw();
   TLine* centre4 = new TLine(0.,0.,0.,discr[4]->GetMaximum());
   centre4->SetLineColor(kRed);
   centre4->Draw("SAME");
   //l->Draw("SAME");
   c->Update();
   
   TCanvas *c1 = new TCanvas("c1","c1",1600,1600);
   c1->Divide(3,2);
   c1->cd(1);
   amplitude[0]->Draw();
   TLine* centre10 = new TLine(0.,0.,0.,amplitude[0]->GetMaximum());
   l1->AddEntry(centre0,"#sqrt{(x_corr[row] - (f(z[row])))^{2}} = 0","l");
   centre10->SetLineColor(kRed);
   centre10->Draw("SAME");
   //l1->Draw("SAME");
   c1->cd(2);
   amplitude[1]->Draw();
   TLine* centre11 = new TLine(0.,0.,0.,amplitude[1]->GetMaximum());
   centre11->SetLineColor(kRed);
   centre11->Draw("SAME");
   //l1->Draw("SAME");
   c1->cd(3);
   amplitude[2]->Draw();
   TLine* centre12 = new TLine(0.,0.,0.,amplitude[2]->GetMaximum());
   centre12->SetLineColor(kRed);
   centre12->Draw("SAME");
   //l1->Draw("SAME");
   c1->cd(4);
   amplitude[3]->Draw();
   TLine* centre13 = new TLine(0.,0.,0.,amplitude[3]->GetMaximum());
   centre13->SetLineColor(kRed);
   centre13->Draw("SAME");
   //l1->Draw("SAME");
   c1->cd(5);
   amplitude[4]->Draw();
   TLine* centre14 = new TLine(0.,0.,0.,amplitude[4]->GetMaximum());
   centre14->SetLineColor(kRed);
   centre14->Draw("SAME");
   //l1->Draw("SAME");
   
   TCanvas *c2 = new TCanvas("c2");
   c2->cd();
   h_chi->Draw();
   
   
         
   char titolo0[100];
   sprintf(titolo0,"Pictures_Analysis/TrackQualityControl/Run%d/Discrepancies_corr_run%d.png",run,run);
   char titolo1[100];
   sprintf(titolo1,"Pictures_Analysis/TrackQualityControl/Run%d/Amplitudes_corr_run%d.png",run,run);
   char titolo2[100];
   sprintf(titolo2,"Pictures_Analysis/TrackQualityControl/Run%d/chi_corr_run%d.png",run,run);
   char chi_rapp[100];
   sprintf(chi_rapp,"#tilde{chi}_{0}^{1}/#tilde{chi}=%.3f",rapp);
   TLatex *lat2 = new TLatex(0.5,0.7,chi_rapp);  
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.45,0.35,0.95,0.95);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->SetLogy();
   zoomPad->Draw();
   zoomPad->cd();
   h_chiRed->Draw();
   Int_t binMin = h_chiRed->FindBin(0);
   Int_t binMax = h_chiRed->FindBin(0.95);
   chiRed_min1 = h_chiRed->Integral(binMin,binMax);
   rapp = chiRed_min1/(h_chiRed->Integral(binMin,200));
   lat2->Draw("SAME");
   
   cout << "binMin:" << binMin << "    binMax: " << binMax << "   Integral: " << chiRed_min1 << "chi_min^max/chi_tot: " << rapp << endl;
   
   c->SaveAs(titolo0);
   c1->SaveAs(titolo1);
   c2->SaveAs(titolo2);
   
   TCanvas *c3 = new TCanvas("c3");
   c3->Divide(2,1);
   c3->cd(1);
   h_theta->Draw();
   c3->cd(2);
   h_intercetta->Draw();
   char titolo3[100];
   sprintf(titolo3,"Pictures_Analysis/TrackQualityControl/Run%d/theta_intercetta_corr%d.png",run,run);
   c3->SaveAs(titolo3);


}
