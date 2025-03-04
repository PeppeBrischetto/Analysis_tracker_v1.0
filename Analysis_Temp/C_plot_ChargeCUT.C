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


void C_plot_ChargeCUT(int run)
{
//#################################################################################################
// Required variables
  
   char histoname[100];
   Double_t sigma = 0;
   Int_t n_point_tot = 0;
   
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
   
   TH1F *histo_c[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_c[i]=new TH1F("","",60,0,59);
      histo_c[i]->GetXaxis()->SetTitle("Pad number");
      histo_c[i]->GetYaxis()->SetTitle("");
      histo_c[i]->SetLineColor(kRed);
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

   char pippo;
   
   
   //for(Int_t i=0; i<entries; i++){
   for(Int_t i=0; i<entries; i++){
       tree->GetEntry(i);
       cout<<"#@@#"<<i<<endl;
       if(i<100){                                              // loop to save the last 100 evts
       for(Int_t j=0; j<5; j++){
          
          Double_t charge[60] = {0.};
          for(Int_t k=0; k<cl_padMult[j]; k++){
             //charge[k] = pads_charge[j][k];
             histo_c[j]->SetBinContent(a_pads_fired[j][k],pads_charge[j][k]);
          }
       //cout << "Evt: " << i << "  Row: " << j << endl; 
       //cout << "Charge integral: " << charge[j] << "   Compared to: " << cl_charge[j] << endl;
       //histo_c[j]->Fit(f,"R+");

       sigma=f->GetParameter(2);
       if(sic_fired==1){
         gr[j]->SetPoint(n_point_tot,n_point_tot,2.35*sigma);
         charge_evts[j]->SetPoint(n_point_tot,n_point_tot,cl_charge[j]);
         outFit << cl_padMult[j] << "	";
         }
       
       }
       n_point_tot ++;
       outFit << endl;
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

       
       sprintf(histoname,"../../Pictures_Analysis/Run332/pict/ChargeDistrib%d.png",i);
       c->SaveAs(histoname);
       
       //cin>> pippo;      
       for(Int_t j=0; j<5; j++){
          histo_c[j]->Reset();
       }
       
     }   // end loop for the last 100 evts

   }
   /*TCanvas *c = new TCanvas("c");
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
   c->SaveAs("../../NinoAnalysis/Run332/ChargeDistribEND.png");*/
   TCanvas *c2 = new TCanvas("c2");
   c2->Divide(3,2);
   c2->cd(1);
   gr[0]->Draw();
   c2->cd(2);
   gr[1]->Draw();
   c2->cd(3);
   gr[2]->Draw();
   c2->cd(4);
   gr[3]->Draw();
   c2->cd(5);
   gr[4]->Draw();
   TCanvas *c3 = new TCanvas("c3");
   c3->Divide(3,2);
   c3->cd(1);
   charge_evts[0]->Draw();
   c3->cd(2);
   charge_evts[1]->Draw();
   c3->cd(3);
   charge_evts[2]->Draw();
   c3->cd(4);
   charge_evts[3]->Draw();
   c3->cd(5);
   charge_evts[4]->Draw();
   
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
   
//##################################################################################################
// Visualization Block
   
   
}
