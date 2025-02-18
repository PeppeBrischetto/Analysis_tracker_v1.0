//###################################################################################################
//#   plot spectra of charga distribution for aach row
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   creatd feb 2025 by A. Pitronaci - based on D. Torresi's sources
//#######################################
//#   updated: Febbruary 2025 by A. Pitronaci
//# 
//###################################################################################################

#include "../Include/ninoopenfiles.h"

const Int_t nPad = 60;
const Int_t minPad = 0;
const Int_t maxPad = 59;

const TString OutputFile = "outFit_MULT_provv.txt";


void C_plot_ChargeCUT(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   Double_t sigma = 0;
   
   ofstream outFit;
   
   TF1 *f = new TF1("f","gaus",-0.5,60);
   f->SetParameters(1,1,1);
   
   outFit.open(OutputFile,std::ios_base::app);
   
  
   
   // all charg distributions
   TH1F *histo_li[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_li[i]=new TH1F("","",60,-0.5,59.5);
      histo_li[i]->GetXaxis()->SetTitle("Pad number");
      histo_li[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_li[i]->SetLineColor(kBlue);
   }
   
   TH1F *histo_he[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_he[i]=new TH1F("","",60,-0.5,59.5);
      histo_he[i]->GetXaxis()->SetTitle("Pad number");
      histo_he[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_he[i]->SetLineColor(kRed);
   }
   
   TH1F *histo_c[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_c[i]=new TH1F("","",60,-0.5,59.5);
      histo_c[i]->GetXaxis()->SetTitle("Pad number");
      histo_c[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_c[i]->SetLineColor(kRed);
   }
         
//#################################################################################################
// OpenFile
   openTrackFile(run);
   tree->Print();

//#################################################################################################

//#################################################################################################
// Graphyical cut definition

   TCutG *alpha = new TCutG("alpha",8);
   alpha->SetVarX("cl_x_mm[0]");
   alpha->SetVarY("cl_x_mm[1]");
   alpha->SetPoint(0,17,40);
   alpha->SetPoint(1,8,28);
   alpha->SetPoint(2,8,9);
   alpha->SetPoint(3,39,30);
   alpha->SetPoint(4,182,193);
   alpha->SetPoint(5,154,214);
   alpha->SetPoint(6,55,85);
//#################################################################################################
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################

   
   
   for(Int_t i=0; i<entries; i++){
       tree->GetEntry(i);
       
       for(Int_t j=0; j<5; j++){
          for(Int_t k=0; k<60; k++){
             histo_c[j]->Fill(k,cl_chargePad[j][k]);
          }
       histo_c[j]->Fit(f,"R+");
       sigma=f->GetParameter(2);
       outFit << sigma << endl;
       }
       
       /*if("alpha==0"){
         for(Int_t j=0; j<5; j++){
            for(Int_t k=0; k<60; k++){
               histo_li[j]->Fill(k,cl_chargePad[j][k]);
            }
         }
       }else
        if("alpha==1"){
          for(Int_t j=0; j<5; j++){
             for(Int_t k=0; k<60; k++){
                histo_he[j]->Fill(k,cl_chargePad[j][k]);
             }
          }
       }*/    
   }
   
//##################################################################################################
// Visualization Block
   
   TCanvas *c = new TCanvas("c");
   c->Divide(3,2);
   c->cd(1);
   histo_c[0]->Draw("histo");
   c->cd(2);
   histo_c[1]->Draw("histo");
   c->cd(3);
   histo_c[2]->Draw("histo");
   c->cd(4);
   histo_c[3]->Draw("histo");
   c->cd(5);
   histo_c[4]->Draw("histo");
}
