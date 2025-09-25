//#################################################################################################################
//#   This macro allows to evaluate the error on the x_{i} measured on the padPlane. It is pretty similar to chi.C. but in this case ROW4 is used to filter the events within a very small window.
//#################################################################################################################
//#   Created September 2025 by A. Pitronaci 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

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

void chi_row4Filtering(int run){

//################################################################################################################
// Variables
   char tasto[10]; 
   char titolofile[100];
   char histoname[100];
   Double_t z[5] = {18.6, 39.8, 61.0, 82.2, 103.4};
   Double_t chi_root = 0.;
   Double_t my_chi = 0.;
   Double_t my_chiRed = 0.;
   Double_t Ndof = 3;
   Double_t offset[NRows] = {-0.487176,1.9054,-0.577267,-2.34564,1.48374};
   Double_t x_corr[NRows] = {0.};
   Double_t errX[NRows] = {0.};
   Double_t chiRed_min1 = 0.;
   Double_t chiRed_tot = 0.;
   Double_t pearson = 0.;
   Double_t m = 0.;
   Double_t q = 0.;
   Double_t errM = 0.;
   Double_t errQ = 0.;
   Double_t avErrM = 0.;
   Double_t avErrQ = 0.;
   Double_t mean_errX[NRows] = {0.};
   Double_t x_foc = 0.;
   Int_t eo = 0;
   Int_t ei = 0;
   Double_t thetaDeg = 0.;
   Double_t intercetta = 0.;
   Double_t coeffAng = 0.;
   
   ofstream outfile;
   
   char provv;

   TH2D *anode = new TH2D("anode", "anode", 60, -0.5, 59.5, 11, -0.5, 10.5);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11);
   anode->GetYaxis()->SetLabelSize(0);
   
   TH1D *h_row4 = new TH1D("h_row4","", 1000,0.,300.);
   h_row4->GetXaxis()->SetTitle("x_{4} (mm)");
   h_row4->GetYaxis()->SetTitle("Counts");
   
   
   TH1D *h_theta = new TH1D("h_theta","",450,40,80);
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
   
   TH1D *h_intercetta = new TH1D("h_intercetta","",250,-40,0);
   h_intercetta->GetXaxis()->SetTitle("intercept (mm)");
   h_intercetta->GetXaxis()->SetTitleSize(0.05);
   h_intercetta->GetXaxis()->SetLabelSize(0.05);
   h_intercetta->GetXaxis()->SetTitleOffset(.9);
   //h_intercetta->GetYaxis()->SetTitle("Counts");
   h_intercetta->GetYaxis()->SetTitleSize(0.05);
   h_intercetta->GetYaxis()->SetLabelSize(0.05);
   h_intercetta->GetYaxis()->SetTitleOffset(1.);
   h_intercetta->SetNdivisions(7);
   h_intercetta->SetLineWidth(2);
   
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
   h_chiRed->GetXaxis()->SetTitle("#tilde{#chi}^{2}_{red}");
   h_chiRed->GetXaxis()->SetTitleSize(0.05);
   h_chiRed->GetXaxis()->SetLabelSize(0.05);
   h_chiRed->GetXaxis()->SetTitleOffset(.9);
   h_chiRed->GetYaxis()->SetTitle("Counts");
   h_chiRed->GetYaxis()->SetTitleSize(0.05);
   h_chiRed->GetYaxis()->SetLabelSize(0.05);
   h_chiRed->GetYaxis()->SetTitleOffset(1.);
   h_chiRed->SetNdivisions(7);
   h_chiRed->SetLineWidth(2);
   
   TH1D *h_corr = new TH1D("h_corr","",1400,0.995,1.002);
   h_corr->GetXaxis()->SetTitle("r");
   h_corr->GetXaxis()->SetTitleSize(0.05);
   h_corr->GetXaxis()->SetLabelSize(0.05);
   h_corr->GetXaxis()->SetTitleOffset(.9);
   h_corr->GetYaxis()->SetTitle("Counts");
   h_corr->GetYaxis()->SetTitleSize(0.05);
   h_corr->GetYaxis()->SetLabelSize(0.05);
   h_corr->GetYaxis()->SetTitleOffset(1.);
   h_corr->SetNdivisions(7);
   h_corr->SetLineWidth(2);
   
   TH1D *h_foc = new TH1D("h_foc","",1400,10,60);
   h_foc->GetXaxis()->SetTitle("x_{z=0} (mm)");
   h_foc->GetXaxis()->SetTitleSize(0.05);
   h_foc->GetXaxis()->SetLabelSize(0.05);
   h_foc->GetXaxis()->SetTitleOffset(.9);
   h_foc->GetYaxis()->SetTitle("Counts");
   h_foc->GetYaxis()->SetTitleSize(0.05);
   h_foc->GetYaxis()->SetLabelSize(0.05);
   h_foc->GetYaxis()->SetTitleOffset(1.);
   h_foc->SetNdivisions(7);
   h_foc->SetLineWidth(2);
   
   TH1D *h_errX[NRows];
   for(Int_t i=0; i<NRows; i++){
      sprintf(histoname,"h_errX_%d",i);
      h_errX[i] = new TH1D("h_errX","",250,0,10);
      h_errX[i]->GetXaxis()->SetTitle("#varepsilon_{x} (mm)");
      h_errX[i]->GetXaxis()->SetTitleSize(0.05);
      h_errX[i]->GetXaxis()->SetLabelSize(0.05);
      h_errX[i]->GetXaxis()->SetTitleOffset(.9);
      h_errX[i]->GetYaxis()->SetTitle("Counts");
      h_errX[i]->GetYaxis()->SetTitleSize(0.05);
      h_errX[i]->GetYaxis()->SetLabelSize(0.05);
      h_errX[i]->GetYaxis()->SetTitleOffset(1.);
      h_errX[i]->SetNdivisions(7);
      h_errX[i]->SetLineWidth(2);
   }
   
   TH1D *h_errM = new TH1D("h_errM","",100,0,0.1);
   h_errM->GetXaxis()->SetTitle("#varepsilon_{m} (mm)");
   h_errM->GetXaxis()->SetTitleSize(0.05);
   h_errM->GetXaxis()->SetLabelSize(0.05);
   h_errM->GetXaxis()->SetTitleOffset(.9);
   h_errM->GetYaxis()->SetTitle("Counts");
   h_errM->GetYaxis()->SetTitleSize(0.05);
   h_errM->GetYaxis()->SetLabelSize(0.05);
   h_errM->GetYaxis()->SetTitleOffset(1.);
   h_errM->SetNdivisions(7);
   h_errM->SetLineWidth(2);
      
   TH1D *h_errQ = new TH1D("h_errQ","",50,0,4);
   h_errQ->GetXaxis()->SetTitle("#varepsilon_{q} (mm)");
   h_errQ->GetXaxis()->SetTitleSize(0.05);
   h_errQ->GetXaxis()->SetLabelSize(0.05);
   h_errQ->GetXaxis()->SetTitleOffset(.9);
   h_errQ->GetYaxis()->SetTitle("Counts");
   h_errQ->GetYaxis()->SetTitleSize(0.05);
   h_errQ->GetYaxis()->SetLabelSize(0.05);
   h_errQ->GetYaxis()->SetTitleOffset(1.);
   h_errQ->SetNdivisions(7);
   h_errQ->SetLineWidth(2);
   
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
   
   //sprintf(titolofile,"pearson_coefficient/r_coefficient_Tracks_run%d.txt",run);
   //outputfile.open(titolofile);
   //outputfile << "************ Run " << run << "_ - pearson coefficient ***********" << endl; 

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
// Data loop 1.0: searching the right ROW4 slit 
   
   Double_t row4 = 0.;
   
   for(Int_t q=0; q<entries; q++){
      tree->GetEntry(q);
      h_row4->Fill(cl_x_mm[4]-offset[4]);
   }
   
   row4 = h_row4->GetMean();
   
   TLine* l_inf = new TLine(0.,0.,0.,h_row4->GetMean()-0.2);
   l_inf->SetLineColor(kRed);
   TLine* l_max = new TLine(0.,0.,0.,h_row4->GetMean()+0.2);
   l_max->SetLineColor(kRed);
   TCanvas *cs = new TCanvas("cs");
   cs->cd();
   h_row4->Draw();
   l_inf->Draw("SAME");
   l_max->Draw("SAME");
   
// Data loop 2.0: selection over the ROW4 slit (ϕ = 0.2mm)

   for(Int_t i=0; i<entries; i++){
      my_chi = 0.;
      chi_root = 0.;
      my_chiRed = 0.;
      pearson = 0.;
      errM = 0.;
      errQ = 0.;
      x_foc = 0.;
      
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
      if(sic_fired==1 && energySic>2000){
      //if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
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
      intercetta = f->GetParameter(0);
      coeffAng = f->GetParameter(1);
      
      
      if(i==0 || i==100 || i==200 || i==300 || i==400 || i==500 || i==600 || i==700){
         TCanvas *c_retta = new TCanvas("c_retta");
         c_retta->cd();
         retta->Draw();
         c_retta->Update();
         char tit_retta[100];
         sprintf(tit_retta,"Pictures_Analysis/TrackQualityControl/Run%d/bestTrack_row4Filtering%d.png",run,i);
         c_retta->SaveAs(tit_retta);
      }
      
      theta_deg = 90-(ATan(f->GetParameter(1))*180/Pi());
      
      
      if(Abs(cl_x_mm[4]-row4)<0.2){
      for(Int_t row=0; row<NRows; row++){
         my_chi += (pow(z[row]-f->Eval(x_corr[row]),2)/1);
         scarto[row] = x_corr[row] - (f->GetX(z[row]));
         discr[row]->Fill(scarto[row]);
         ampiezza[row] = sqrt(scarto[row]*scarto[row]);
         amplitude[row]->Fill(ampiezza[row]);
         errX[row] = sqrt(pow((-1/coeffAng)*errQ,2) /*+ pow((-1/coeffAng)*10,2)*/ + pow(((intercetta-f->Eval(x[row]))/(pow(coeffAng,2))*errM),2) /*pow(scarto[row],2)*/);
         if(row==NRows-1){
            h_chi->Fill(my_chi);
         }
         h_errX[row]->Fill(errX[row]);
         h_errX[row]->Fit("norm","","",0,5);
         mean_errX[row] = norm->GetParameter(1);
      }
      
      x_foc = -intercetta/coeffAng;
      cout << "x_foc: " << x_foc << endl;
      h_foc->Fill(x_foc);
      my_chiRed = my_chi/Ndof;
      h_chiRed->Fill(my_chiRed);
      
      errM = f->GetParError(1);
      errQ = f->GetParError(0);
      h_errM->Fill(errM);
      h_errQ->Fill(errQ);
      pearson = retta->GetCorrelationFactor();
      h_corr->Fill(pearson);
      h_theta->Fill(theta_deg);
      h_intercetta->Fill(intercetta);
      
      ei++;
      cout << "Evts. inside the filter: " << ei << endl;
      }else{
               eo++;
               cout << "Evts. outside the filter: " << eo << endl;
      }
      
      /* cout trials */
      cout << "My chi: " << my_chi << "    chi^2 reduced: " << endl;
      /*cout << " *************************************** " << endl;
      cout << "Event: " << i << endl;
      for(Int_t row=0; row<NRows; row++){
         printf("x%d= %1.4f and z%d= %1.4f\n ",i,x[row],i,z[row]);
      }
      cout << "Theta (deg): " << theta_deg << endl;*/

      anode->Reset("ICES");
      
      //cin >> provv;
   //}                                                                           // TCutG parenthesis
   }                                                                           // SiC_fired==1 & energySic>2000 - cut to select good events
   }
   
   /* Average discrepancy evaluation by normal fit */
   for(Int_t row=0; row<NRows; row++){
      discr[row]->Fit("norm","","+",-5,5);
      offset[row]=norm->GetParameter(1);
      cout << "Offset " << row << ": " << offset[row] << endl;  
   }
   
   char tFile[100];
   sprintf(tFile,"TrackQuality_txtFiles/qualityTest_run%d_ROW4Filtering.txt",run);
   outfile.open(tFile);
   
   outfile << "============================== Preliminary quality test results ==============================" << endl << endl;
   outfile << "                    error_coeffAng: " << h_errM->GetMean() << "    error_intercept: " << h_errQ->GetMean() << endl << endl;
   for(Int_t row=0; row<NRows; row++){
      outfile << "                    Mean_error_x_" << row << ": " << mean_errX[row] << endl << endl;
   } 
   outfile << "                    Pearson_coeff.: " << h_corr->GetMean() << "    chi_{red}: " << h_chiRed->GetMean() << endl << endl;
   outfile << "                    Evts. inside the filter: " << ei << "/" << entries<< "    Evts. outside the filter: " << eo << "/" << entries << endl;
   TLegend* l = new TLegend(0.1,0.7,0.48,0.9);
   l->SetTextSize(0.035);
   l->SetLineWidth(0);
   l->AddEntry(discr[0], "x_corr[row] - (f(z[row]))", "f");
    
   TLegend* l1 = new TLegend(0.1,0.7,0.48,0.9);
   l1->SetTextSize(0.035);
   l1->AddEntry(amplitude[0], "x_corr[row] - (f(z[row]))", "f");
   l1->SetLineWidth(0);
   
   TCanvas *c = new TCanvas("c","c",1600,1200);
   c->Divide(3,2);
   c->cd(1);
   discr[0]->Draw();
   TLine* centre0 = new TLine(0.,0.,0.,discr[0]->GetMaximum());
   l->AddEntry(centre0,"x[row] - (f(z[row])) = 0","l");
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
   
   TCanvas *c1 = new TCanvas("c1","c1",1600,1200);
   c1->Divide(3,2);
   c1->cd(1);
   amplitude[0]->Draw();
   TLine* centre10 = new TLine(0.,0.,0.,amplitude[0]->GetMaximum());
   l1->AddEntry(centre0,"#sqrt{(x[row] - (f(z[row])))^{2}} = 0","l");
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
   
   cout << "binMin:" << binMin << "    binMax: " << binMax << "   Integral: " << chiRed_min1 << "   chi_min^max/chi_tot: " << chiRed_min1/(h_chiRed->Integral(binMin,200)) << endl;
      
   char titolo0[100];
   sprintf(titolo0,"Pictures_Analysis/TrackQualityControl/Run%d/Discrepancies_run%d_row4Filtering.png",run,run);
   char titolo1[100];
   sprintf(titolo1,"Pictures_Analysis/TrackQualityControl/Run%d/Amplitudes_run%d_row4Filtering.png",run,run);
   char titolo2[100];
   sprintf(titolo2,"Pictures_Analysis/TrackQualityControl/Run%d/chi_run%d_row4Filtering.png",run,run);
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
   sprintf(titolo3,"Pictures_Analysis/TrackQualityControl/Run%d/theta_intercetta%d_row4Filtering.png",run,run);
   c3->SaveAs(titolo3);
   
   TCanvas *c4 = new TCanvas("c4");
   c4->Divide(3,2);
   c4->cd();
   h_corr->Draw();
   char titolo4[100];
   sprintf(titolo4,"Pictures_Analysis/TrackQualityControl/Run%d/pearson_%d_row4Filtering.png",run,run);
   c4->SaveAs(titolo4);
   
   TCanvas *c5 = new TCanvas("c5","c5",900,500);
   c5->Divide(3,2);
   for(Int_t p=0; p<NRows; p++){
      c5->cd(p+1);
      h_errX[p]->Draw();
   }
   char titolo5[100];
   sprintf(titolo5,"Pictures_Analysis/TrackQualityControl/Run%d/errX_run%d_row4Filtering.png",run,run);
   c5->SaveAs(titolo5);
   
   TCanvas *c6 = new TCanvas("c6");
   c6->cd();
   h_foc->Draw();
   char titolo6[100];
   sprintf(titolo6,"Pictures_Analysis/TrackQualityControl/Run%d/x_foc_run%d_row4Filtering.png",run,run);
   c6->SaveAs(titolo6);
   
   TCanvas *c7 = new TCanvas("c7","c7",900,900);
   c7->Divide(2,1);
   c7->cd(1);
   h_errM->Draw();
   c7->cd(2);
   h_errQ->Draw();
   char titolo7[100];
   sprintf(titolo7,"Pictures_Analysis/TrackQualityControl/Run%d/errMQ_run%d_ROW4Filtering.png",run,run);
   c7->SaveAs(titolo7);
}
