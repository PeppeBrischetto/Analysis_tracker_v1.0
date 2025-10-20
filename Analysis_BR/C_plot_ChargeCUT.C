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

#include "../Include/openfiles.h"

const Int_t nPads = 60;
const Int_t minPad = 0;
const Int_t maxPad = 59;

void C_plot_ChargeCUT(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   char cond[100];
   Double_t sigma = 0;
   Double_t width[5] = {0};
   Int_t n_point_tot = 0;
   Double_t media[5] = {0.};
   Double_t theta = 0;
   Double_t err_theta = 0;
   Int_t c = 0;
   ofstream outPut;
   
   TF1 *f = new TF1("f","gaus",-60,60);
   f->SetParameters(1,1,1);
   
   char titoloFile[100];
   sprintf(titoloFile,"OutputFile_chargeDistrib/Run%d.txt",run);
   outPut.open(titoloFile,std::ios_base::app);
  
   
   // all charg distributions
   TH1F *histo[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo[i]=new TH1F("","",60,0,59);
      histo[i]->GetXaxis()->SetTitle("Pad number");
      histo[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo[i]->SetLineColor(kBlue);
   }
   
   TH1F *histo_Stat[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_Stat[i]=new TH1F("","",60,0,59);
      histo_Stat[i]->GetXaxis()->SetTitle("Pad number");
      histo_Stat[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_Stat[i]->SetLineColor(kBlue);
   }
   
   TH1F *histo_Theta = new TH1F("histo_Theta","",100,30,70);
   histo_Theta=new TH1F("","",90,0,90);
   histo_Theta->GetXaxis()->SetTitle("Pad number");
   histo_Theta->GetYaxis()->SetTitle("Charge (a.u.)");
   histo_Theta->SetLineColor(kBlue);
   
//#################################################################################################
// OpenFile
   openTrackFile(run);
   tree->Print();

//#################################################################################################

//#################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",5);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,47.07,97.98);
   cutG->SetPoint(1,47.07,97.23);
   cutG->SetPoint(2,47.59,97.3);
   cutG->SetPoint(3,47.83,97.99);
   cutG->SetPoint(4,47.07,97.98);
         
//#################################################################################################
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################

   char pippo;
   
   for(Int_t i=0; i<entries; i++){
      tree->GetEntry(i);
      for(Int_t j=0; j<5;j++){
          histo[j]->Reset();
       }
       
       if(cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
          c++;
          for(Int_t j = 0; j < 5; j++){
             histo_Stat[j]->Fill(cl_padMult[j]);
             for(Int_t k = 0; k < cl_padMult[j]; k++) {
                 histo[j]->SetBinContent(pads_fired[j][k],pad_charge[j][k]);
             }
          }
          histo_Theta->Fill(theta_deg);
       }
    }
  
    histo_Theta->Fit(f,"","+",0,90);
    theta = f->GetParameter(1);
    err_theta = f->GetParError(2)/sqrt(c);
    
    outPut << "Theta: " << theta << "   ErrTheta: " << err_theta << endl;
       for(Int_t j=0; j<5; j++){
          f->SetParameters(0,0,0);
          histo_Stat[j]->Fit(f,"R+");
          media[j] = 5*f->GetParameter(1);
          width[j] = media[j]/sqrt(c);
       }
       
     //outPut << "<M0>	<M1>	<M2>	<M3>	<M4>" << endl;
     for(Int_t j=0; j<5; j++){
        outPut << media[j] << "	";
     }
     
     //outPut << endl << "<E0>	<E1>	<E2>	<E3>	<E4>" << endl;
     for(Int_t j=0; j<5; j++){
        outPut << endl << width[j] << "	";
     }  
       
   }
