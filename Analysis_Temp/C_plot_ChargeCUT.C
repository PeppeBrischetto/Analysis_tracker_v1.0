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

const TString OutputFileLi = "../OutputFiles/Diffusion_Run332Li.txt";
const TString OutputFileA = "../OutputFiles/Diffusion_Run332Alpha.txt";
const TString CutFileA = "../GCut/cutGa.root";
const TString CutFileLi = "../GCut/cutGli.root";


void C_plot_ChargeCUT(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   char cond[100];
   Double_t sigma_he = 0;
   Double_t width_he[5] = {0};
   Double_t sigma_li = 0;
   Double_t width_li[5] = {0};
   Int_t n_point_tot = 0;
   Double_t media_he[5] = {0.};
   Double_t media_li[5] = {0.};
   Double_t theta_he = 0;
   
   Int_t li = 0;
   Int_t he = 0;
   
   ofstream outFitLi, outFitA;
   
   TF1 *f = new TF1("f","gaus",-60,60);
   f->SetParameters(1,1,1);
   
   outFitLi.open(OutputFileLi,std::ios_base::app);
   outFitA.open(OutputFileA,std::ios_base::app);
  
   
   // all charg distributions
   TH1F *histo_li[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_li[i]=new TH1F("","",60,0,59);
      histo_li[i]->GetXaxis()->SetTitle("Pad number");
      histo_li[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_li[i]->SetLineColor(kBlue);
   }
   TH1F *histo_liStat[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_liStat[i]=new TH1F("","",60,0,59);
      histo_liStat[i]->GetXaxis()->SetTitle("Pad number");
      histo_liStat[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      histo_liStat[i]->SetLineColor(kBlue);
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

   TCutG *cutGli = new TCutG("cutGli",8);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,13,65);
   cutGli->SetPoint(1,9,42);
   cutGli->SetPoint(2,10,39);
   cutGli->SetPoint(3,25,50);
   cutGli->SetPoint(4,54,94);
   cutGli->SetPoint(5,69,126);
   cutGli->SetPoint(6,36,142);
   cutGli->SetPoint(7,13,65);
   cutGli->SaveAs(CutFileLi);
   
   TCutG *cutGa = new TCutG("cutGa",8);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,17,40);
   cutGa->SetPoint(1,8,28);
   cutGa->SetPoint(2,8,9);
   cutGa->SetPoint(3,39,30);
   cutGa->SetPoint(4,182,193);
   cutGa->SetPoint(5,154,214);
   cutGa->SetPoint(6,55,85);
   cutGa->SetPoint(7,17,40);
   cutGa->SaveAs(CutFileA);
   
//#################################################################################################
// Loop to get charge distribution referred to every single rows for each event
//#################################################################################################

   char pippo;
   
   TFile *cutFilea = TFile::Open(CutFileA);
   TFile *cutFileli = TFile::Open(CutFileLi);
   //TCutG cutG = (TCutG)cutFile->Get("cutG");
 
   for(Int_t i=0; i<entries; i++){
       tree->GetEntry(i);
       cout<<"#@@#"<<i<<endl;
       //if(i<100){                                              // loop to save an evtSet
          for(Int_t j=0; j<5;j++){
             histo_li[j]->Reset();
             histo_he[j]->Reset();
          }
          //cout << "Evento " << i << ": cl_x_mm[0] = " << cl_x_mm[0] << ", cl_x_mm[1] = " << cl_x_mm[1] << endl;
          //tree->Draw("cl_x_mm[1]:cl_x_mm[0]"); // Disegna cl_x_mm[1] in funzione di cl_x_mm[0]
          //cutG->Draw("L SAME"); // Sovrapponi il taglio grafico
          if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
             cout << "Event " << i << " satisfies TCutGa" << endl;
             he += 1;
             
             //cout << "Eventi di alpha: " << he << endl;
             for (Int_t j = 0; j < 5; j++) {
                 outFitA << cl_padMult[j] << "	" ;
                 for (Int_t k = 0; k < cl_padMult[j]; k++) {
                     //cout << "  j = " << j << ", k = " << k << ", pads_fired = " << pads_fired[j][k] << ", pads_charge = " << pads_charge[j][k] << endl;
                     histo_he[j]->SetBinContent(pads_fired[j][k], pads_charge[j][k]);
                 }
                 
             }
             theta_he = (theta)*(180/3.1415);
             outFitA << theta_he << endl;
             if(he<100){
             for(Int_t j=0; j<5; j++){
                histo_he[j]->Fit(f,"R+");
                sigma_he = f->GetParameter(2);
                width_he[j] += sigma_he;
                media_he[j] += cl_padMult[j];
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
             
             sprintf(histoname,"../../Pictures_Analysis/Run332_proof/pictHe/ChargeDistribHe%d.png",i);
             che->SaveAs(histoname);}
          }else
          if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){
             cout << "Event " << i << " satisfies TCutGli" << endl;
             li += 1;
             
             //cout << "Eventi di 7Li: " << li << endl;
             for (Int_t j = 0; j < 5; j++) {
                 for (Int_t k = 0; k < cl_padMult[j]; k++) {
                     histo_liStat[j]->Fill(cl_padMult[j]);
                     //cout << "  j = " << j << ", k = " << k << ", pads_fired = " << pads_fired[j][k] << ", pads_charge = " << pads_charge[j][k] << endl;
                     histo_li[j]->SetBinContent(pads_fired[j][k], pads_charge[j][k]);
                 }
                 
             }
             if(li<100){
             
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
             sprintf(histoname,"../../Pictures_Analysis/Run332_proof/pictLi/ChargeDistribLi%d.png",i);
             cli->SaveAs(histoname);}
          }
       } 
       
       outFitLi << "<M0>	<M1>	<M2>	<M3>	<M4>" << endl;
       
       for(Int_t j=0; j<5; j++){
          histo_liStat[j]->Fit(f,"R+");
          media_li[j] = f->GetParameter(1);
          width_li[j] = f->GetParameter(2);;
          outFitLi << media_li[j] << "	";
          
          
          outFitA << media_he[j] << "	";
       }
       outFitLi << endl;
       outFitA << endl;
       outFitLi << "<E0>	<E1>	<E2>	<E3>	<E4>" << endl;
       outFitA << "<E0>	<E1>	<E2>	<E3>	<E4>" << endl;
       for(Int_t j=0; j<5; j++){
          outFitLi << width_li[j] << "	";
          outFitA << width_he[j] << "	";
       }  
       cout << "Theta_he: " << theta_he << endl;
       
   }
