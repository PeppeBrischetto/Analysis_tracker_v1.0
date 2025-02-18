//###################################################################################################
//#   plot spectra of charga distribution for aach row
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   creatd jan 2025 by A. Pitronaci - based on D. Torresi's sources
//#######################################
//#   updated: January 2025 by A. Pitronaci
//# 
//###################################################################################################

#include "../Include/ninoopenfiles.h"

void C_plot_ChargeDistr(int run)
{

//###################################################################
//    VARIABLES
  
   char histoname[100];
   ifstream output;
   int flagA=0;
   int rate_tracks = 1400;  // rate of tracks                              
   double average_mult_tot[5] = {0};
   double average_mult_sic[5] = {0};
   double average_mult_wsic[5] = {0};
   int n_point_tot = 0;
   int n_point_wsic = 0;
   int n_point_sic = 0;
   int tot = 0;
   int sic = 0;
   int wsic = 0;
   char t_tot[100];
   char t_sic[100];
   char t_wsic[100];
   char asseX[100];
   char asseY[100];
   char title[100];
   Double_t average_charge[5] = {0};
   Double_t rate[5] = {20,250,1000,3000};
//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// GRAHICS & fitting function

   TCanvas *c0=new TCanvas("c0","c0",250,120,900,700);
   TCanvas *c1=new TCanvas("c1","c1",250,120,900,700);   
   TCanvas *c2=new TCanvas("c2","c2",250,120,900,700);
   TCanvas *c3=new TCanvas("c3","c3",250,120,900,700);
   TCanvas *c4=new TCanvas("c4","c4",250,120,900,700);
   
   // all charg distributions
   TH1F *histo_tot[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_tot[i]=new TH1F("","",1000,0,200000);
      histo_tot[i]->GetXaxis()->SetTitle("Charge (a.u.)");
      histo_tot[i]->GetYaxis()->SetTitle("Counts");
      histo_tot[i]->GetYaxis()->SetRangeUser(0,350);
      histo_tot[i]->SetLineColor(kBlue);
   }
   
   TH1F *histo_sic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_sic[i]=new TH1F("","",1000,0,200000);
      histo_sic[i]->GetXaxis()->SetTitle("Charge (a.u.)");
      histo_sic[i]->GetYaxis()->SetTitle("Counts");
      histo_sic[i]->GetYaxis()->SetRangeUser(0,350);
      histo_sic[i]->SetLineColor(kRed);
   }
   
   TH1F *histo_wsic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      histo_wsic[i]=new TH1F("","",1000,0,200000);
      histo_wsic[i]->GetXaxis()->SetTitle("Charge (a.u.)");
      histo_wsic[i]->GetYaxis()->SetTitle("Counts");
      histo_wsic[i]->GetYaxis()->SetRangeUser(0,350);
      histo_wsic[i]->SetLineColor(kGreen+2);
   }
   
   TH1F *cartoon_sic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge Distrib. - row%i",i);
      cartoon_sic[i]=new TH1F("","",1000,0,30000);
      cartoon_sic[i]->GetXaxis()->SetTitle("x (mm)");
      cartoon_sic[i]->GetYaxis()->SetTitle("Counts");
      cartoon_sic[i]->GetYaxis()->SetRangeUser(0,20);
      cartoon_sic[i]->SetLineColor(kRed);
   }
   
   // all x distributions
   TH1F *x_tot[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"x Distrib. (tot. evts) - row%i",i);
      x_tot[i]=new TH1F("","",10000,0,400);
      x_tot[i]->GetXaxis()->SetTitle("x (mm)");
      x_tot[i]->GetYaxis()->SetTitle("Counts");
      x_tot[i]->GetYaxis()->SetRangeUser(0,350);
      x_tot[i]->SetLineColor(kBlue);
   }
   
   TH1F *x_sic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"x Distrib. (SiC evts) - row%i",i);
      x_sic[i]=new TH1F("","",10000,0,400);
      x_sic[i]->GetXaxis()->SetTitle("x (mm)");
      x_sic[i]->GetYaxis()->SetTitle("Counts");
      x_sic[i]->GetYaxis()->SetRangeUser(0,350);
      x_sic[i]->SetLineColor(kRed);
   }
   
   TH1F *x_wsic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"x Distrib. (evts without SiC) - row%i",i);
      x_wsic[i]=new TH1F("","",10000,0,400);
      x_wsic[i]->GetXaxis()->SetTitle("x (mm)");
      x_wsic[i]->GetYaxis()->SetTitle("Counts");
      x_wsic[i]->GetYaxis()->SetRangeUser(0,350);
      x_wsic[i]->SetLineColor(kGreen+2);
   }
   
   // 2D-Histos
   TH2D *chargeX_tot[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charg vs x - row%i",i);
      chargeX_tot[i]=new TH2D("","",1000,0,300,1000,0,200000);
      chargeX_tot[i]->GetXaxis()->SetTitle("x (mm)");
      chargeX_tot[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      chargeX_tot[i]->SetMarkerColor(kBlue);
   }
   
   
   TH2D *chargeX_sic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge vs x - row%i",i);
      chargeX_sic[i]=new TH2D("","",1000,0,300,1000,0,200000);
      chargeX_sic[i]->GetXaxis()->SetTitle("x (mm)");
      chargeX_sic[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      chargeX_sic[i]->SetMarkerColor(kRed);
   }
   
   TH2D *chargeX_wsic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Charge vs x - row%i",i);
      chargeX_wsic[i]=new TH2D("","",1000,0,300,1000,0,200000);
      chargeX_wsic[i]->GetXaxis()->SetTitle("x (mm)");
      chargeX_wsic[i]->GetYaxis()->SetTitle("Charge (a.u.)");
      chargeX_wsic[i]->SetMarkerColor(kGreen+2);
   }   
   
   // all pad multiplicity histos
   TH1I *padmult_tot[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Pad Multiplicity (all evts) - row%i",i);
      padmult_tot[i]=new TH1I("","",100,0,20);
      padmult_tot[i]->GetXaxis()->SetTitle(histoname);
      padmult_tot[i]->GetYaxis()->SetTitle("Counts");
      padmult_tot[i]->GetYaxis()->SetRangeUser(0,35000);
      padmult_tot[i]->SetLineColor(kBlue);
   }
   
   TH1I *padmult_sic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Pad Multiplicity (SiC evts) - row%i",i);
      padmult_sic[i]=new TH1I("","",100,0,20);
      padmult_sic[i]->GetXaxis()->SetTitle(histoname);
      padmult_sic[i]->GetYaxis()->SetTitle("Counts");
      padmult_sic[i]->GetYaxis()->SetRangeUser(0,35000);
      padmult_sic[i]->SetLineColor(kRed);
   }
   
   TH1I *padmult_wsic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"Pad Multiplicity (evts without SiC) - row%i",i);
      padmult_wsic[i]=new TH1I("","",100,0,20);
      padmult_wsic[i]->GetXaxis()->SetTitle(histoname);
      padmult_wsic[i]->GetYaxis()->SetTitle("Counts");
      padmult_wsic[i]->GetYaxis()->SetRangeUser(0,35000);
      padmult_wsic[i]->SetLineColor(kGreen+2);
   }
   
   // TGraphs
   TGraph *gr_tot[5];
   TGraph *gr_sic[5];
   TGraph *gr_wsic[5];
   for(int i=0; i<5; i++){
      sprintf(histoname,"row%i",i);
      gr_tot[i]=new TGraph();
      gr_tot[i]->GetXaxis()->SetTitle("time (s)");
      gr_tot[i]->GetYaxis()->SetTitle("Average multiplicity");
      gr_tot[i]->SetMarkerStyle(20);
      gr_tot[i]->SetMarkerColor(kBlue);
      gr_tot[i]->GetYaxis()->SetRangeUser(0,11);
      gr_tot[i]->SetLineWidth(0);
      
      gr_sic[i]=new TGraph();
      gr_sic[i]->GetXaxis()->SetTitle("time (s)");
      gr_sic[i]->GetYaxis()->SetTitle("Average multiplicity");
      gr_sic[i]->SetMarkerStyle(20);
      gr_sic[i]->SetMarkerColor(kRed);
      gr_sic[i]->GetYaxis()->SetRangeUser(0,11);
      gr_sic[i]->SetLineWidth(0);
      
      gr_wsic[i]=new TGraph();
      gr_wsic[i]->GetXaxis()->SetTitle("time (s)");
      gr_wsic[i]->GetYaxis()->SetTitle("Average multiplicity");
      gr_wsic[i]->SetMarkerStyle(20);
      gr_wsic[i]->SetMarkerColor(kGreen+2);
      gr_wsic[i]->GetYaxis()->SetRangeUser(0,11);
      gr_wsic[i]->SetLineWidth(0);
   }
   
   // (x_i;x_j)-scatterplot
   TH2D *xixj[4];
   for(int i=0; i<4; i++){
      sprintf(histoname,"(x_{i};x_{j})-scatterplot");
      sprintf(asseX,"x_%i (mm)",i);
      sprintf(asseY,"x_%i (mm)",i+1);
      xixj[i]=new TH2D("","",1000,0,420,1000,0,420);
      xixj[i]->GetXaxis()->SetTitle(asseX);
      xixj[i]->GetYaxis()->SetTitle(asseY);
      xixj[i]->SetMarkerColor(kBlue);
   }
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      
      
      tot += 1;
      switch(sic_fired){
         case 0: wsic += 1;
                 break;
         case 1: sic += 1;
                 break;
      }
      
      // Fill the histos
      for(int h=0; h<5; h++){
         histo_tot[h]->Fill(cl_charge[h]);
         chargeX_tot[h]->Fill(cl_x_mm[h],cl_charge[h]);
         x_tot[h]->Fill(cl_x_mm[h]);
         padmult_tot[h]->Fill(cl_padMult[h]);
         sprintf(t_tot,"Total evts: %d",tot);
         switch(sic_fired){
            case 0: histo_wsic[h]->Fill(cl_charge[h]);
                    chargeX_wsic[h]->Fill(cl_x_mm[h],cl_charge[h]);
                    x_wsic[h]->Fill(cl_x_mm[h]);
                    padmult_wsic[h]->Fill(cl_padMult[h]);
                    sprintf(t_wsic,"Total evts without SiC: %d",wsic);
                    break;
            case 1: histo_sic[h]->Fill(cl_charge[h]);
                    chargeX_sic[h]->Fill(cl_x_mm[h],cl_charge[h]);
                    x_sic[h]->Fill(cl_x_mm[h]);
                    padmult_sic[h]->Fill(cl_padMult[h]);
                    sprintf(t_sic,"Total evts with SiC: %d",sic);
                    cartoon_sic[h]->Fill(cl_x_mm[h]);
                    break;
         }
      
      
      }
      
      // Fill the TGraphs
      for(int h=0; h<5; h++){
//         histo[h]->Fill(cl_padMult[h]);
         average_mult_tot[h] = average_mult_tot[h] + cl_padMult[h];    
         switch(sic_fired){
             case 0: average_mult_wsic[h] = average_mult_wsic[h] + cl_padMult[h];
                     break;
             case 1: average_mult_sic[h] = average_mult_sic[h] + cl_padMult[h];
                     break;
         }    
      }
      
      if(i%rate_tracks==0){
         for(int l=0;l<5;l++){
           average_mult_tot[l] = average_mult_tot[l]/rate_tracks;
           gr_tot[l]->SetPoint(n_point_tot,n_point_tot,average_mult_tot[l]);
           //cout << "aveMult0: " << average_mult_tot[l] << endl;
           average_mult_tot[l]=0;
           
           switch(sic_fired){
                 case 0: average_mult_wsic[l] = average_mult_wsic[l]/rate_tracks;
                         gr_wsic[l]->SetPoint(n_point_wsic,n_point_wsic,average_mult_wsic[l]);
                         average_mult_wsic[l] = 0;
                         n_point_wsic ++;
                         break;
                 case 1: average_mult_sic[l] = average_mult_sic[l]/rate_tracks;
                         gr_sic[l]->SetPoint(n_point_sic,n_point_sic,average_mult_sic[l]);
                         average_mult_sic[l] = 0;
                         n_point_sic ++;
                         break;
           }
         }
       n_point_tot ++;
      }
      
      // Fill th histos
      for(int h=0; h<4; h++){
         int r = h+1;
         xixj[h]->Fill(cl_x_mm[h],cl_x_mm[r]);
      }
     
      // TCanvas for cartoonize the charge distribution in each row
      /*if(i<=500){
      TCanvas *c_c = new TCanvas("c_c");
      c_c->Divide(3,2);
      c_c->cd(1);
      cartoon_sic[0]->GetXaxis()->SetRangeUser(60,70);
      cartoon_sic[0]->Draw();
      c_c->cd(2);
      cartoon_sic[1]->GetXaxis()->SetRangeUser(90,115);
      cartoon_sic[1]->Draw();
      c_c->cd(3);
      cartoon_sic[2]->GetXaxis()->SetRangeUser(135,150);
      cartoon_sic[2]->Draw();
      c_c->cd(4);
      cartoon_sic[3]->GetXaxis()->SetRangeUser(165,195);
      cartoon_sic[3]->Draw();
      c_c->cd(5);
      cartoon_sic[4]->GetXaxis()->SetRangeUser(205,235);
      cartoon_sic[4]->Draw();
      sprintf(title,"../../NinoAnalysis/Run38/loopCD/frame%i.png",i);
      c_c->SaveAs(title);
      }*/
   
   /* Fitting histos_clCharge to obtain average charge for each row */
   for(Int_t r=0; r>5; r++){
       histo_sic[r]->Fit("gaus","R");
   }
  }
//################################################################################################
// Visualization block

   TLatex *t = new TLatex(0.5,0.5,"^{7}Li @ 24.0 MeV (R = 1400 Hz) +#alpha");
   t->SetTextColor(kBlack);
   TText *t0 = new TText(0.5,0.5,t_tot);
   t0->SetTextColor(kBlue);
   TText *t1 = new TText(0.5,0.5,t_sic);
   t1->SetTextColor(kRed);
   TText *t2 = new TText(0.5,0.5,t_wsic);
   t2->SetTextColor(kGreen+2);
   
   
   c0->Divide(3,2);
   c0->cd(1);
   histo_tot[0]->Draw();
   histo_sic[0]->Draw("same");
   histo_wsic[0]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c0->cd(2);
   x_tot[0]->Draw();
   x_sic[0]->Draw("same");
   x_wsic[0]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c0->cd(3);
   padmult_tot[0]->Draw();
   padmult_sic[0]->Draw("same");
   padmult_wsic[0]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c0->cd(4);
   gr_tot[0]->Draw();
   gr_tot[0]->Fit("pol0");
   gr_sic[0]->Draw("same");
   gr_wsic[0]->Draw("same");
   c0->cd(5);
   chargeX_tot[0]->Draw("scat");
   chargeX_sic[0]->Draw("scat && same");
   chargeX_wsic[0]->Draw("scat && same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c0->cd(6);
   xixj[0]->Draw();
   //c0->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run38/ChargeDistrib0.png");
   
   c1->Divide(3,2);
   c1->cd(1);
   histo_tot[1]->Draw();
   histo_sic[1]->Draw("same");
   histo_wsic[1]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c1->cd(2);
   x_tot[1]->Draw();
   x_sic[1]->Draw("same");
   x_wsic[1]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c1->cd(3);
   padmult_tot[1]->Draw();
   padmult_sic[1]->Draw("same");
   padmult_wsic[1]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c1->cd(4);
   gr_tot[1]->Draw();
   gr_tot[1]->Fit("pol0");
   gr_sic[1]->Draw("same");
   gr_wsic[1]->Draw("same");
   c1->cd(5);
   chargeX_tot[1]->Draw("scat");
   chargeX_sic[1]->Draw("scat && same");
   chargeX_wsic[1]->Draw("scat && same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c1->cd(6);
   xixj[1]->Draw();
   //c1->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run38/ChargeDistrib1.png");
   
   c2->Divide(3,2);
   c2->cd(1);
   histo_tot[2]->Draw();
   histo_sic[2]->Draw("same");
   histo_wsic[2]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c2->cd(2);
   x_tot[2]->Draw();
   x_sic[2]->Draw("same");
   x_wsic[2]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c2->cd(3);
   padmult_tot[2]->Draw();
   padmult_sic[2]->Draw("same");
   padmult_wsic[2]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c2->cd(4);
   gr_tot[2]->Draw();
   gr_tot[2]->Fit("pol0");
   gr_sic[2]->Draw("same");
   gr_wsic[2]->Draw("same");
   c2->cd(5);
   chargeX_tot[2]->Draw("scat");
   chargeX_sic[2]->Draw("scat && same");
   chargeX_wsic[2]->Draw("scat && same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c2->cd(6);
   xixj[2]->Draw();
   //c2->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run38/ChargeDistrib2.png");
   
   c3->Divide(3,2);
   c3->cd(1);
   histo_tot[3]->Draw();
   histo_sic[3]->Draw("same");
   histo_wsic[3]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c3->cd(2);
   x_tot[3]->Draw();
   x_sic[3]->Draw("same");
   x_wsic[3]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c3->cd(3);
   padmult_tot[3]->Draw();
   padmult_sic[3]->Draw("same");
   padmult_wsic[3]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c3->cd(4);
   gr_tot[3]->Draw();
   gr_tot[3]->Fit("pol0");
   gr_sic[3]->Draw("same");
   gr_wsic[3]->Draw("same");
   c3->cd(5);
   chargeX_tot[3]->Draw("scat");
   chargeX_sic[3]->Draw("scat && same");
   chargeX_wsic[3]->Draw("scat && same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c3->cd(6);
   xixj[3]->Draw();
   //c3->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run38/ChargeDistrib3.png");
   
   c4->Divide(3,2);
   c4->cd(1);
   histo_tot[4]->Draw();
   histo_sic[4]->Draw("same");
   histo_wsic[4]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c4->cd(2);
   x_tot[4]->Draw();
   x_sic[4]->Draw("same");
   x_wsic[4]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c4->cd(3);
   padmult_tot[4]->Draw();
   padmult_sic[4]->Draw("same");
   padmult_wsic[4]->Draw("same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   c4->cd(4);
   gr_tot[4]->Draw();
   gr_tot[4]->Fit("pol0");
   gr_sic[4]->Draw("same");
   gr_wsic[4]->Draw("same");
   c4->cd(5);
   chargeX_tot[4]->Draw("scat");
   chargeX_sic[4]->Draw("scat && same");
   chargeX_wsic[4]->Draw("scat && same");
   t->Draw("same");
   t0->Draw("same");
   t1->Draw("same");
   t2->Draw("same");
   //c4->SaveAs("/home/numeneg/irrad4/NinoAnalysis/Run38/ChargeDistrib4.png");
   

}
