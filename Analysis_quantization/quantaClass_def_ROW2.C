#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>

#include "../Include/openfiles.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;

void quantaClass_def_ROW2(int run){

//################################################################################################################
// Needed variables
   Double_t par[9] = {0.};                                             // parameters-array to store single-fits
   char histoname[100];
   char tit[100];
   Double_t minimum1,minimum2 = 0.;
   
   TH1D *histo_x[NRows];                                               // histo-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"histo_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      histo_x[i]=new TH1D("","",9000.,0.,300);
      histo_x[i]->GetXaxis()->SetTitle(tit);
      histo_x[i]->GetXaxis()->SetTitleSize(0.05);
      histo_x[i]->GetXaxis()->SetLabelSize(0.05);
      histo_x[i]->GetXaxis()->SetTitleOffset(0.9);
      histo_x[i]->GetYaxis()->SetTitleSize(0.05);
      histo_x[i]->GetYaxis()->SetLabelSize(0.05);
      histo_x[i]->GetYaxis()->SetTitleOffset(1.);
      histo_x[i]->SetNdivisions(12);
      histo_x[i]->SetFillColor(kBlue-5);
      histo_x[i]->SetFillStyle(3001);
      histo_x[i]->SetLineWidth(2);
   }
   
   TF1 *f1 = new TF1("f1","gaus",139,141.2);
   TF1 *f2 = new TF1("f2","gaus",141.2,143.2);
   TF1 *f3 = new TF1("f3","gaus",143.5,146);
   TF1 *f = new TF1("f","gaus(0)+gaus(3)+gaus(6)",134,147);
   

//################################################################################################################
// OpenFiles
   openTrackFile(run);
   tree->Print();

//################################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",16);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,65.5044,108.12);
   cutG->SetPoint(1,64.8152,108.042);
   cutG->SetPoint(2,64.0144,107.627);
   cutG->SetPoint(3,63.3998,107.159);
   cutG->SetPoint(4,62.7107,106.484);
   cutG->SetPoint(5,62.3009,105.835);
   cutG->SetPoint(6,61.9657,105.497);
   cutG->SetPoint(7,61.8912,103.913);
   cutG->SetPoint(8,62.1333,102.952);
   cutG->SetPoint(9,63.0273,102.874);
   cutG->SetPoint(10,63.9213,103.263);
   cutG->SetPoint(11,65.1319,103.965);
   cutG->SetPoint(12,66.0817,105.731);
   cutG->SetPoint(13,66.3611,107.445);
   cutG->SetPoint(14,66.2493,107.886);
   cutG->SetPoint(15,65.5044,108.12);

   
//################################################################################################################
// Data loop
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      //if(sic_fired==1 && energySic>2000 && cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
         for(Int_t row=0; row<NRows; row++){
            histo_x[row]->Fill(cl_x_mm[row]);
         }
      //}                                                                                 // TCutG-parenthesis
   }
   
//################################################################################################################
// Fitting histograms
   
   histo_x[2]->Fit(f1,"R");
   histo_x[2]->Fit(f2,"R");
   histo_x[2]->Fit(f3,"R");
   f1->GetParameters(&par[0]);
   f2->GetParameters(&par[3]);
   f3->GetParameters(&par[6]);
   f->SetParameters(par);
   
   histo_x[2]->Fit("f","R+");

//################################################################################################################
// Searching extremes of quantization classes      
   
   TF1 *df = new TF1("df","(f1(x+[0])-f1(x-[0]))/(2*[0])",134,147);
   df->FixParameter(0,1E-8);
   TF1 *d2f = new TF1("d2f","(df(x+[0])-df(x-[0]))/(2*[0])",134,147);
   d2f->FixParameter(0,1E-8);

   //cout << "La derivata a x=139 picco vale: " << df->Eval(par[1]) << " e la sua derivata seconda vale: " << d2f->Eval(par[1]) << endl;
   
   minimum1 = f->GetMinimumX(par[1],par[4],1E-8,100000,0);
   minimum2 = f->GetMinimumX(par[4],par[7],1E-8,100000,0);
   cout << "Minimum1: " << minimum1 << "    Minimum2: " << minimum2 << endl;

//################################################################################################################
// Quantiazion classes creation - multiplicity correlation
   
   Int_t p1,p2,p3 = 0;
   
   TH1D *histo_m[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_m %i",i);
      histo_m[i]=new TH1D("","",60,-0.5,59.5);
      histo_m[i]->GetXaxis()->SetTitle("multiplicity");
      histo_m[i]->GetXaxis()->SetTitleSize(0.05);
      histo_m[i]->GetXaxis()->SetLabelSize(0.05);
      histo_m[i]->GetXaxis()->SetTitleOffset(1.);
      histo_m[i]->GetYaxis()->SetTitleSize(0.05);
      histo_m[i]->GetYaxis()->SetLabelSize(0.05);
      histo_m[i]->GetYaxis()->SetTitleOffset(1.);
      histo_m[i]->SetNdivisions(12);
      histo_m[i]->SetFillColor(kBlue-5);
      histo_m[i]->SetFillStyle(3001);
      histo_m[i]->SetLineWidth(2);
   }
   
   TH1D *histo_theta[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_theta %i",i);
      histo_theta[i]=new TH1D("","",360,0,90);
      histo_theta[i]->GetXaxis()->SetTitle("#vartheta (deg)");
      histo_theta[i]->GetXaxis()->SetTitleSize(0.05);
      histo_theta[i]->GetXaxis()->SetLabelSize(0.05);
      histo_theta[i]->GetXaxis()->SetTitleOffset(1.);
      histo_theta[i]->GetYaxis()->SetTitleSize(0.05);
      histo_theta[i]->GetYaxis()->SetLabelSize(0.05);
      histo_theta[i]->GetYaxis()->SetTitleOffset(1.);
      histo_theta[i]->SetNdivisions(12);
      histo_theta[i]->SetFillColor(kBlue-5);
      histo_theta[i]->SetFillStyle(3001);
      histo_theta[i]->SetLineWidth(2);
   }
   
   TH1D *histo_intercept[3];                                               // histo-array for the rows
   for(int i=0; i<3; i++){
      sprintf(histoname,"histo_intercept %i",i);
      histo_intercept[i]=new TH1D("","",360,-20,20);
      histo_intercept[i]->GetXaxis()->SetTitle("intercept (mm)");
      histo_intercept[i]->GetXaxis()->SetTitleSize(0.05);
      histo_intercept[i]->GetXaxis()->SetLabelSize(0.05);
      histo_intercept[i]->GetXaxis()->SetTitleOffset(1.);
      histo_intercept[i]->GetYaxis()->SetTitleSize(0.05);
      histo_intercept[i]->GetYaxis()->SetLabelSize(0.05);
      histo_intercept[i]->GetYaxis()->SetTitleOffset(1.);
      histo_intercept[i]->SetNdivisions(12);
      histo_intercept[i]->SetFillColor(kBlue-5);
      histo_intercept[i]->SetFillStyle(3001);
      histo_intercept[i]->SetLineWidth(2);
   }
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      if(cl_x_mm[2]<minimum1 /*&& cutG->IsInside(cl_x_mm[0], cl_x_mm[1])*/){
        p1 ++;
        histo_m[0]->Fill(cl_padMult[2]);
        if(cl_padMult[2]==6){
          histo_theta[0]->Fill(theta_deg);
          histo_intercept[0]->Fill(interceptP);
        }
      }else
       if(cl_x_mm[2]>minimum1 && cl_x_mm[2]<minimum2 /*&& cutG->IsInside(cl_x_mm[0], cl_x_mm[1])*/){
         p2 ++;
         histo_m[1]->Fill(cl_padMult[2]);
         if(cl_padMult[2]==7){
           histo_theta[1]->Fill(theta_deg);
           histo_intercept[1]->Fill(interceptP);
         }
       }else
        if(cl_x_mm[2]>minimum2 /*&& cutG->IsInside(cl_x_mm[0], cl_x_mm[1])*/){
          p3 ++;
          histo_m[2]->Fill(cl_padMult[2]);
          if(cl_padMult[2]==6){
            histo_theta[2]->Fill(theta_deg);
            histo_intercept[2]->Fill(interceptP);
          }
        }
   }
   
   cout << "Evts. pop1: " << p1 << "   Evts. pop2: " << p2 << "   Evts. pop3: " << p3 << endl;
      
//################################################################################################################
// Visualization block

   TCanvas *c0 = new TCanvas("c0","c0",2200,2200);
   c0->Divide(3,2);
   for(Int_t row=0; row<NRows; row++){
     c0->cd(row+1);
     histo_x[row]->Draw();
   }
   
   TText *t_m[3];
   TText *t_theta[3];
   TText *t_intercept[3];
   char titolo[100];
   
   TCanvas *c1 = new TCanvas("c1","c1",1700,700);
   c1->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c1->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_m[col] = new TText(0.5,0.5*histo_m[col]->GetMaximum(),titolo);
      histo_m[col]->Draw();
      t_m[col]->Draw();
   }
   
   TCanvas *c2 = new TCanvas("c2","c2",1700,700);
   c2->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c2->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_theta[col] = new TText(0.5,0.5*histo_theta[col]->GetMaximum(),titolo);
      histo_theta[col]->Draw();
      t_theta[col]->Draw();
   }
   
   TCanvas *c3 = new TCanvas("c3","c3",1700,700);
   c3->Divide(3,1);
   for(Int_t col=0; col<3; col++){
      c3->cd(col+1);
      sprintf(titolo,"%d-quantization class",col+1);
      t_theta[col] = new TText(0.5,0.5*histo_intercept[col]->GetMaximum(),titolo);
      histo_intercept[col]->Draw();
      t_theta[col]->Draw();
   }
}
