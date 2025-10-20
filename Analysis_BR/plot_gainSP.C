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
   
   ifstream infile1, infile2, infile3, infile4, infile5, infile6, infile7, in4;
   char titolofile1[100];
   char titolofile2[100];
   char titolofile3[100];
   char titolofile4[100];
   char titolofile5[100];
   char titolofile6[100];
   char titolofile7[100];
   string dummy;
   char buffer[200];
   Double_t GF1 = 234.533;
   double V, a, b, c, d, e, f, err_a, err_b, err_c, err_d, err_e, err_f;
   Double_t v=0.;
   Double_t g = 0.;
   
//################################################################################################################ 
// OpenFile 1: 12C
   
   sprintf(titolofile1,"Gain/gainTHGEM_37_43.txt");
   infile1.open(titolofile1);
     
   TGraph *gain1 = new TGraph();
   gain1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain1->GetYaxis()->SetTitle("Gain");
   gain1->SetMarkerStyle(8);
   //gain1->SetLineWidth(0);
   gain1->SetLineColor(kMagenta+1);
   gain1->SetMarkerColor(kMagenta+1);
   gain1->GetXaxis()->SetRangeUser(120,240);
   gain1->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile1,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<8; i++){
         infile1 >> v >> g;
         gain1->SetPoint(i,v,g);
      }

//################################################################################################################ 
// OpenFile 2: 12C
   
   sprintf(titolofile2,"Gain/gainTHGEM_51_57.txt");
   infile2.open(titolofile2);
     
   TGraph *gain2 = new TGraph();
   gain2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain2->GetYaxis()->SetTitle("Gain");
   gain2->SetMarkerStyle(8);
   gain2->SetLineColor(kMagenta);
   //gain2->SetLineWidth(0);
   gain2->SetMarkerColor(kMagenta);
   gain2->GetXaxis()->SetRangeUser(110,240);
   gain2->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile2,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<7; i++){
         infile2 >> v >> g;
         gain2->SetPoint(i,v,g);
      }

//################################################################################################################ 
// OpenFile 3: 12C
   
   sprintf(titolofile3,"Gain/gainTHGEM_63_69.txt");
   infile3.open(titolofile3);
     
   TGraph *gain3 = new TGraph();
   gain3->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain3->GetYaxis()->SetTitle("Gain");
   gain3->SetMarkerStyle(8);
   //gain3->SetLineWidth(0);
   gain3->SetMarkerColor(kMagenta+2);
   gain3->SetLineColor(kMagenta+2);
   gain3->GetXaxis()->SetRangeUser(120,240);
   gain3->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile3,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<6; i++){
         infile3 >> v >> g;
         gain3->SetPoint(i,v,g);
      }

//################################################################################################################ 
// OpenFile 4: 12C
   
   sprintf(titolofile4,"Gain/gainTHGEM_74_79.txt");
   infile4.open(titolofile4);
     
   TGraph *gain4 = new TGraph();
   gain4->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain4->GetYaxis()->SetTitle("Gain");
   gain4->SetMarkerStyle(8);
   //gain4->SetLineWidth(0);
   gain4->SetLineColor(kMagenta+3);
   gain4->SetMarkerColor(kMagenta+3);
   gain4->GetXaxis()->SetRangeUser(120,240);
   gain4->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile4,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<5; i++){
         infile4 >> v >> g;
         gain4->SetPoint(i,v,g);
      }

//################################################################################################################ 
// OpenFile 5: 12C
   
   sprintf(titolofile5,"Gain/gainTHGEM_170_174.txt");
   infile5.open(titolofile5);
     
   TGraph *gain5 = new TGraph();
   gain5->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain5->GetYaxis()->SetTitle("Gain");
   gain5->SetMarkerStyle(8);
   //gain5->SetLineWidth(0);
   gain5->SetLineColor(kOrange+1);
   gain5->SetMarkerColor(kOrange+1);
   gain5->GetXaxis()->SetRangeUser(120,240);
   gain5->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile5,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<6; i++){
         infile5 >> v >> g;
         gain5->SetPoint(i,v,g);
      }   

//################################################################################################################ 
// OpenFile 6: 12C
   
   sprintf(titolofile6,"Gain/gainTHGEM_275_280_li.txt");
   infile6.open(titolofile6);
     
   TGraph *gain6 = new TGraph();
   gain6->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain6->GetYaxis()->SetTitle("Gain");
   gain6->SetMarkerStyle(8);
   //gain6->SetLineWidth(0);
   gain6->SetLineColor(kYellow+1);
   gain6->SetMarkerColor(kYellow+1);
   gain6->GetXaxis()->SetRangeUser(120,240);
   gain6->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile6,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<5; i++){
         infile6 >> v >> g;
         gain6->SetPoint(i,v,g);
      }
      
//################################################################################################################ 
// OpenFile 7: 12C
   
   sprintf(titolofile7,"Gain/gainTHGEM_293_298_a.txt");
   infile7.open(titolofile7);
     
   TGraph *gain7 = new TGraph();
   gain7->GetXaxis()->SetTitle("V_{THGEM} (V)");
   gain7->GetYaxis()->SetTitle("Gain");
   gain7->SetMarkerStyle(8);
   //gain7->SetLineWidth(0);
   gain7->SetLineColor(kGreen+1);
   gain7->SetMarkerColor(kGreen+1);
   gain7->GetXaxis()->SetRangeUser(120,240);
   gain7->GetYaxis()->SetRangeUser(1e0,1e8);
   
   for(Int_t r=0; r<2; r++){
      getline(infile7,dummy);
      cout << dummy << endl;
   }
   for(Int_t i=0; i<5; i++){
         infile7 >> v >> g;
         gain7->SetPoint(i,v,g);
      }
      
//################################################################################################################ 
// OpenFile 8: INFN-LNS characterization - alpha source
      
   in4.open("../../Antonino/Analisi_PICO2023/DataTable/VTHGEM_run400_414_10mbar_withoutRim.txt");
   in4.getline(buffer,200);
   TGraphErrors *gr4=new TGraphErrors(0); // Graph for 10 mbar/no rim
   gr4->SetMarkerStyle(20);
   gr4->SetMarkerColor(kCyan+2);
   gr4->SetLineColor(kCyan+2);
   for(int i=0; i<14; i++){
      in4 >> V >> a >> err_a >> b >> err_b >> c >> err_c >> d >> err_d >> e >> err_e >> f >> err_f;
      cout<<V<<"\t"<<a<<"\t"<<err_a<<"\t"<<b<<"\t"<<err_b<<"\t"<<c<<"\t"<<err_c<<"\t"<<d<<"\t"<<err_d<<"\t"<<e<<"\t"<<err_e<<"\t"<<f<<err_f<<endl;
      
      gr4->SetPoint(i, V, (a)*GF1);  	//anode
      gr4->SetPointError( i, 0., TMath::Sqrt(err_a*err_a)*GF1);
   }
      
//################################################################################################################ 
// Visualization block
   
   TLegend *l = new TLegend(0.1,0.55,0.35,0.9);
   l->AddEntry(gain7,"#alpha","lp");
   l->AddEntry(gain6,"^{7}Li - 1000 Hz","lp");
   l->AddEntry(gain2,"^{12}C - 20 Hz","lp");
   l->AddEntry(gain1,"^{12}C - 250 Hz","lp");
   l->AddEntry(gain3,"^{12}C - 1000 Hz","lp");
   l->AddEntry(gain4,"^{12}C - 3000 Hz","lp");
   l->AddEntry(gain5,"^{16}O - 250 Hz","lp");
   l->AddEntry(gr4,"#alpha @INFN-LNS","lp");
 
   TCanvas *c1 = new TCanvas("c1","c1",950.,900.);
   c1->SetLogy();
   c1->SetGrid();
   c1->cd();
   gain1->Draw("PL SAME");
   gain2->Draw("APL SAME");
   gain3->Draw("PL SAME");
   gain4->Draw("PL SAME");
   gain5->Draw("PL SAME");
   gain6->Draw("PL SAME");
   gain7->Draw("PL SAME");  
   gr4->Draw("PL SAME");    
   l->Draw("SAME");
   
   TGraph *gain1_cl = (TGraph*)gain1->Clone("gain1_cl");
   TGraph *gain2_cl = (TGraph*)gain2->Clone("gain2_cl");
   TGraph *gain3_cl = (TGraph*)gain3->Clone("gain3_cl");
   TGraph *gain4_cl = (TGraph*)gain4->Clone("gain4_cl");
   TGraph *gain5_cl = (TGraph*)gain5->Clone("gain5_cl");
   TGraph *gain6_cl = (TGraph*)gain6->Clone("gain6_cl");
   TGraph *gain7_cl = (TGraph*)gain7->Clone("gain7_cl");
   TGraph *gr4_cl = (TGraph*)gr4->Clone("gr4_cl");
   gain2_cl->GetXaxis()->SetRangeUser(155,165);
   gain2_cl->GetYaxis()->SetRangeUser(1e2,1e4);
   
   TPad *zoomPad = new TPad("zoomPad", "Zoom", 0.6,0.1,0.9,0.4);
   zoomPad->SetFillColor(0);
   zoomPad->SetFrameFillStyle(0);
   zoomPad->SetLogy();
   zoomPad->Draw();
   zoomPad->cd();
   gain1_cl->Draw("PL SAME");
   gain2_cl->Draw("APL SAME");
   gain3_cl->Draw("PL SAME");
   gain4_cl->Draw("PL SAME");
   gain5_cl->Draw("PL SAME");
   gain6_cl->Draw("PL SAME");
   gain7_cl->Draw("PL SAME");  
   gr4_cl->Draw("PL SAME");

   

}
