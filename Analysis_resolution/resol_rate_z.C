#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>

using namespace std;
using namespace TMath;

const TString InputFileC = "resol_run_52ELLcut.txt";
const TString InputFileC1 = "resol_run_38ELLcut.txt";
const TString InputFileC2 = "resol_run_65ELLcut.txt";
const TString InputFileC3 = "resol_run_76ELLcut.txt";
const TString InputFileLi = "resol_run_298ELLcut.txt";
const TString InputFileO = "resol_run_171ELLcut.txt";

void resol_rate_z(){

   // Needed variables
   ifstream f_c,f_c1,f_c2,f_c3,f_li,f_o;
   string badlines;
   Double_t c = 0.;
   Double_t zc[5] = {45,45,45,45,45};
   Double_t zli[5] = {24,24,24,24,24};
   Double_t zo[5] = {64,64,64,64,64};
   Double_t c_v[5] = {0.};
   Double_t c1_v[5] = {0.};
   Double_t c2_v[5] = {0.};
   Double_t c3_v[5] = {0.};
   Double_t li_v[5] = {0.};
   Double_t o_v[5] = {0.};
   
   // Opening file
   f_c.open(InputFileC);
   f_c1.open(InputFileC1);
   f_c2.open(InputFileC2);
   f_c3.open(InputFileC3);
   f_li.open(InputFileLi);
   f_o.open(InputFileO);
   
   // Reading-loop
   for(Int_t i=0; i<26; i++){
      getline(f_c,badlines);
      getline(f_c1,badlines);
      getline(f_c2,badlines);
      getline(f_c3,badlines);
      getline(f_li,badlines);
      getline(f_o,badlines);
      //cout << badlines << endl;
   }
   
   for(Int_t i=26; i<30; i++){
      f_c >> c >> c >> c >> c_v[0];
      f_c >> c >> c >> c >> c_v[1];
      f_c >> c >> c >> c >> c_v[2];
      f_c >> c >> c >> c >> c_v[3];
      f_c >> c >> c >> c >> c_v[4];
      
      f_c1 >> c >> c >> c >> c1_v[0];
      f_c1 >> c >> c >> c >> c1_v[1];
      f_c1 >> c >> c >> c >> c1_v[2];
      f_c1 >> c >> c >> c >> c1_v[3];
      f_c1 >> c >> c >> c >> c1_v[4];
      
      f_c2 >> c >> c >> c >> c2_v[0];
      f_c2 >> c >> c >> c >> c2_v[1];
      f_c2 >> c >> c >> c >> c2_v[2];
      f_c2 >> c >> c >> c >> c2_v[3];
      f_c2 >> c >> c >> c >> c2_v[4];
      
      f_c3 >> c >> c >> c >> c3_v[0];
      f_c3 >> c >> c >> c >> c3_v[1];
      f_c3 >> c >> c >> c >> c3_v[2];
      f_c3 >> c >> c >> c >> c3_v[3];
      f_c3 >> c >> c >> c >> c3_v[4];
      
      f_li >> c >> c >> c >> li_v[0];
      f_li >> c >> c >> c >> li_v[1];
      f_li >> c >> c >> c >> li_v[2];
      f_li >> c >> c >> c >> li_v[3];
      f_li >> c >> c >> c >> li_v[4];
      
      f_o >> c >> c >> c >> o_v[0];
      f_o >> c >> c >> c >> o_v[1];
      f_o >> c >> c >> c >> o_v[2];
      f_o >> c >> c >> c >> o_v[3];
      f_o >> c >> c >> c >> o_v[4];
   }
   //cout << c_v[0] << "  " << c_v[1] << "   " << c_v[2] << "   " << c_v[3] << "   " << c_v[4] << endl;
   
   TGraph *g_c = new TGraph(5,zc,c_v);
   g_c->SetTitle("Resolution vs E (FWHM)");
   g_c->GetXaxis()->SetTitle("Z");
   g_c->GetYaxis()->SetTitle("Resolution (mm)");
   g_c->SetLineWidth(2);
   g_c->SetLineStyle(5);
   g_c->SetLineColor(kMagenta);
   g_c->SetMarkerStyle(81);
   g_c->SetMarkerSize(2.5);
   g_c->SetMarkerColor(kMagenta);
   
   TGraph *g_c1 = new TGraph(5,zc,c1_v);
   g_c1->SetTitle("Resolution vs E (FWHM)");
   g_c1->GetXaxis()->SetTitle("Z");
   g_c1->GetYaxis()->SetTitle("Resolution (mm)");
   g_c1->SetLineWidth(2);
   g_c1->SetLineStyle(5);
   g_c1->SetLineColor(kMagenta+1);
   g_c1->SetMarkerStyle(81);
   g_c1->SetMarkerSize(2.5);
   g_c1->SetMarkerColor(kMagenta+1);
   
   TGraph *g_c2 = new TGraph(5,zc,c2_v);
   g_c2->SetTitle("Resolution vs E (FWHM)");
   g_c2->GetXaxis()->SetTitle("Z");
   g_c2->GetYaxis()->SetTitle("Resolution (mm)");
   g_c2->SetLineWidth(2);
   g_c2->SetLineStyle(5);
   g_c2->SetLineColor(kMagenta+2);
   g_c2->SetMarkerStyle(81);
   g_c2->SetMarkerSize(2.5);
   g_c2->SetMarkerColor(kMagenta+2);
   
   TGraph *g_c3 = new TGraph(5,zc,c3_v);
   g_c3->SetTitle("Resolution vs Z");
   g_c3->GetXaxis()->SetTitle("Z");
   g_c3->GetYaxis()->SetTitle("Resolution (mm)");
   g_c3->SetLineWidth(2);
   g_c3->SetLineStyle(5);
   g_c3->SetLineColor(kMagenta+3);
   g_c3->SetMarkerStyle(81);
   g_c3->SetMarkerSize(2.5);
   g_c3->SetMarkerColor(kMagenta+3);
   
   TGraph *g_li = new TGraph(5,zli,li_v);
   g_li->SetTitle("Resolution vs E (FWHM)");
   g_li->GetXaxis()->SetTitle("Z");
   g_li->GetYaxis()->SetTitle("Resolution (mm)");
   g_li->SetLineWidth(2);
   g_li->SetLineStyle(5);
   g_li->SetLineColor(kYellow+1);
   g_li->SetMarkerStyle(81);
   g_li->SetMarkerSize(2.5);
   g_li->SetMarkerColor(kYellow+1);
   
   TGraph *g_o = new TGraph(5,zo,o_v);
   g_o->SetTitle("Resolution vs E (FWHM)");
   g_o->GetXaxis()->SetTitle("Z");
   g_o->GetYaxis()->SetTitle("Resolution (mm)");
   g_o->SetLineWidth(2);
   g_o->SetLineStyle(5);
   g_o->SetLineColor(kOrange+1);
   g_o->SetMarkerStyle(81);
   g_o->SetMarkerSize(2.5);
   g_o->SetMarkerColor(kOrange+1);
   g_o->SetPoint(5,0,0.5);
   
   Double_t rate[4] = {20,250,1000,3000};
   TGraph *gC[5];
    for(Int_t r=0; r<5; r++){
       gC[r] = new TGraph();
       gC[r]->SetTitle("Resolution vs rate");
       gC[r]->SetLineWidth(1);
       gC[r]->SetLineStyle(10);
       gC[r]->SetMarkerStyle(81);
       gC[r]->SetMarkerSize(2.5);
       //gC[r]->SetMarkerColor();
       gC[r]->SetLineColor(kRed);
       gC[r]->GetXaxis()->SetTitle("Rate (hz)");
       gC[r]->GetYaxis()->SetTitle("Resolution (mm)");
    }
   
   for(Int_t g=0; g<5; g++){
      for(Int_t p=0; p<4; p++){
         gC[g]->SetPoint(p,rate[p],c_v[g]);
         gC[g]->SetPoint(p,rate[p],c1_v[g]);
         gC[g]->SetPoint(p,rate[p],c2_v[g]);
         gC[g]->SetPoint(p,rate[p],c3_v[g]);
      }
   }
   
   // Visualization block
   TLegend *leg = new TLegend(0.1,0.45,0.5,0.9);
   leg->AddEntry(g_li,"^{7}Li - 1000 Hz","lp");
   leg->AddEntry(g_c,"^{12}C - 20 Hz","lp");
   leg->AddEntry(g_c1,"^{12}C - 250 Hz","lp");
   leg->AddEntry(g_c2,"^{12}C - 1000 Hz","lp");
   leg->AddEntry(g_c3,"^{12}C - 3000 Hz","lp");
   leg->AddEntry(g_o,"^{16}O - 1300 Hz","lp");
   TCanvas *canva = new TCanvas("canva","canva",900,900);
   canva->SetLogy();
   canva->SetGrid();
   canva->cd();
   g_o->Draw("APL");
   g_c->Draw("PL SAME");
   g_c1->Draw("PL SAME");
   g_c2->Draw("PL SAME");
   g_c3->Draw("PL SAME");
   g_li->Draw("PL SAME");
   leg->Draw("SAME");
   
   /*TCanvas *c_rate = new TCanvas("c_rate","c_rate",900,900);
   c_rate->cd();
   gC[0]->Draw("APL");
   gC[1]->Draw("PL SAME");
   gC[2]->Draw("PL SAME");
   gC[3]->Draw("PL SAME");
   gC[4]->Draw("PL SAME");*/
   
   
   
}
