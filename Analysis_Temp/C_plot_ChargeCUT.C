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

const Int_t nPads = 60;
const Int_t minPad = 0;
const Int_t maxPad = 59;

const TString OutputFile = "../../OutputFiles/run332.txt";
const TString CutFile = "cutG.root";


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
   
   ofstream outFit;
   
   TF1 *f = new TF1("f","gaus",-0.5,60);
   f->SetParameters(1,1,1);
   
   outFit.open(OutputFile,std::ios_base::app);
   
  
   
   // all charg distributions
   TH1F *histo_li[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_li[i]=new TH1F("","",60,0,59);
      histo_li[i]->GetXaxis()->SetTitle("Pad number");
      histo_li[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_li[i]->SetLineColor(kBlue);
   }
   
   TH1F *histo_he[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_he[i]=new TH1F("","",60,0,59);
      histo_he[i]->GetXaxis()->SetTitle("Pad number");
      histo_he[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_he[i]->SetLineColor(kRed);
   }
   
   TGraph *gr[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr[i]=new TGraph();
      gr[i]->GetXaxis()->SetTitle("#. evts");
      gr[i]->GetYaxis()->SetTitle("Average multiplicity");
      gr[i]->SetMarkerStyle(20);
      gr[i]->SetMarkerColor(kBlue);
      gr[i]->GetYaxis()->SetRangeUser(0,11);
      gr[i]->SetLineWidth(0);   
   }  
   
   TGraph *charge_evts[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      charge_evts[i]=new TGraph();
      charge_evts[i]->GetXaxis()->SetTitle("#. evts");
      charge_evts[i]->GetYaxis()->SetTitle("Cluster charge");
      charge_evts[i]->SetMarkerStyle(20);
      charge_evts[i]->SetMarkerColor(kBlue);
      charge_evts[i]->GetYaxis()->SetRangeUser(0,11);
      charge_evts[i]->SetLineWidth(0);   
   }  
   
//#################################################################################################
// OpenFile
   openTrackFile(run);
   tree->Print();

//#################################################################################################

//#################################################################################################
// Graphyical cut definition

   TCutG *cutG = new TCutG("cutG",8);
   cutG->SetVarX("cl_x_mm[0]");
   cutG->SetVarY("cl_x_mm[1]");
   cutG->SetPoint(0,17,40);
   cutG->SetPoint(1,8,28);
   cutG->SetPoint(2,8,9);
   cutG->SetPoint(3,39,30);
   cutG->SetPoint(4,182,193);
   cutG->SetPoint(5,154,214);
   cutG->SetPoint(6,55,85);
   cutG->SetPoint(7,17,40);
   cutG->SaveAs(CutFile);
   
//#################################################################################################
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################

   char pippo;
   
   
   //TCutG cutG = (TCutG)cutFile->Get("cutG");
 
   for(Int_t i=0; i<entries; i++){
       TFile *cutFile = TFile::Open(CutFile);
       tree->GetEntry(i);
       cout<<"#@@#"<<i<<endl;
       if(i<5){                                              // loop to save an evtSet
          for(Int_t j=0; j<5;j++){
             histo_li[j]->Reset();
             histo_he[j]->Reset();
          }
          cout << "Evento " << i << ": cl_x_mm[0] = " << cl_x_mm[0] << ", cl_x_mm[1] = " << cl_x_mm[1] << endl;
          tree->Draw("cl_x_mm[1]:cl_x_mm[0]"); // Disegna cl_x_mm[1] in funzione di cl_x_mm[0]
cutG->Draw("L SAME"); // Sovrapponi il taglio grafico
          if(cutG->IsInside(cl_x_mm[0], cl_x_mm[1])){
             cout << "Event " << i << " satisfies TCutG" << endl;
             for (Int_t j = 0; j < 5; j++) {
                 for (Int_t k = 0; k < cl_padMult[j]; k++) {
                     cout << "  j = " << j << ", k = " << k << ", a_pads_fired = " << a_pads_fired[j][k] << ", pads_charge = " << pads_charge[j][k] << endl;
                     histo_he[j]->SetBinContent(a_pads_fired[j][k], pads_charge[j][k]);
                 }
             }
             TCanvas *che = new TCanvas("che");
             che->Divide(3,2);
             che->cd(1);
             histo_he[0]->Draw("histo");
             che->cd(2);
             histo_he[1]->Draw("histo");
             che->cd(3);
             histo_he[2]->Draw("histo");
             che->cd(4);
             histo_he[3]->Draw("histo");
             che->cd(5);
             histo_he[4]->Draw("histo");
             sprintf(histoname,"../../Pictures_Analysis/Run332/pictHe/ChargeDistribHe%d.png",i);
             che->SaveAs(histoname);
          }else{
             for(Int_t j=0; j<5; j++){
                for(Int_t k=0; k<cl_padMult[j]; k++){
                   histo_li[j]->SetBinContent(a_pads_fired[j][k],pads_charge[j][k]);    
                }
             }
             TCanvas *cli = new TCanvas("cli");
             cli->Divide(3,2);
             cli->cd(1);
             histo_li[0]->Draw("histo");
             cli->cd(2);
             histo_li[1]->Draw("histo");
             cli->cd(3);
             histo_li[2]->Draw("histo");
             cli->cd(4);
             histo_li[3]->Draw("histo");
             cli->cd(5);
             histo_li[4]->Draw("histo");
             sprintf(histoname,"../../Pictures_Analysis/Run332/pictLi/ChargeDistribLi%d.png",i);
             cli->SaveAs(histoname);
          }
          
          /*for(Int_t j=0; j<5; j++){
             sprintf(histoname,"a_pads_fired%d>>histo_li[%d]",j,j);
             cout << histoname << endl;
             sprintf(cond,"pads_charge%d && alpha==0",j);
             cout << cond << endl;
             tree->Draw(histoname, cond);
       
             sprintf(histoname,"a_pads_fired%d>>histo_he[%d]",j,j);
             cout << histoname << endl;
             sprintf(cond,"pads_charge%d && alpha==1",j);
             cout << cond << endl;
             tree->Draw(histoname, cond);*/
             
             /* TCanvas block for Li & He */  
             
          }
       
       cin >> pippo;
       
       cutFile->Close();
       }   // end loop for the evtSet
     
       
   }
