//###################################################################################################
//#   plot Average charge (a.u.) vs rate for each row of the gas tracker
//#
//###################################################################################################
//#   creatd jan 2025 by A. Pitronaci
//#
//#######################################
//#
//#   updated: January 2025 by A. Pitronaci
//# 
//###################################################################################################


const Int_t N_rows = 5;

void C_plot_ChargevsRate()
  {
      /* Required variables*/
      string first_line;
      Int_t n_points = 0;
      Double_t run = 0;
      Float_t r_z = 0;
      Float_t r_u = 0;
      Float_t r_d = 0;
      Float_t r_t = 0;
      Float_t r_q = 0;
      Double_t rate[4] = {20,250,1000,3000};
      Double_t av_charge_z[4] = {9884,13710,10410,10510};
      Double_t av_charge_u[4] = {7877,10760,8113,8325};
      Double_t av_charge_d[4] = {7597,10770,7885,8102};
      Double_t av_charge_t[4] = {9537,13340,9727,10040};
      Double_t av_charge_q[4] = {9492,13820,10340,11150};
      
      /* Defining and Filling of TGraphs*/
      TGraph *g_z = new TGraph(4,rate,av_charge_z);
      TGraph *g_u = new TGraph(4,rate,av_charge_u);
      TGraph *g_d = new TGraph(4,rate,av_charge_d);
      TGraph *g_t = new TGraph(4,rate,av_charge_t);
      TGraph *g_q = new TGraph(4,rate,av_charge_q);
       
       g_z->SetTitle("");  
       g_z->SetLineWidth(0);
       g_z->SetMarkerStyle(20);
       g_z->SetMarkerColor(kBlue);
       g_u->SetTitle("");
       g_u->SetLineWidth(0);
       g_u->SetMarkerStyle(20);
       g_u->SetMarkerColor(kRed);
       g_d->SetTitle("");
       g_d->SetLineWidth(0);
       g_d->SetMarkerStyle(20);
       g_d->SetMarkerColor(kGreen+2);
       g_t->SetTitle("");
       g_t->SetLineWidth(0);
       g_t->SetMarkerStyle(20);
       g_t->SetMarkerColor(kOrange+2);
       g_q->SetTitle("");
       g_q->SetLineWidth(0);
       g_q->SetMarkerStyle(20);
       g_q->SetMarkerColor(kMagenta+2);
       g_z->GetXaxis()->SetTitle("Rate (Hz)");
       g_z->GetYaxis()->SetTitle("Average charge (a.u.)");
       g_u->GetXaxis()->SetTitle("Rate (Hz)");
       g_u->GetYaxis()->SetTitle("Average charge (a.u.)");
       g_d->GetXaxis()->SetTitle("Rate (Hz)");
       g_d->GetYaxis()->SetTitle("Average charge (a.u.)");
       g_t->GetXaxis()->SetTitle("Rate (Hz)");
       g_t->GetYaxis()->SetTitle("Average charge (a.u.)");
       g_q->GetXaxis()->SetTitle("Rate (Hz)");
       g_q->GetYaxis()->SetTitle("Average charge (a.u.)");
       
       /* Visualization block */
       TText *t0 = new TText(500,10000,"Average charge (a.u.) -  Row0");
       t0->SetTextColor(kBlue);
       TText *t1 = new TText(500,10000,"Average charge (a.u.) -  Row1");
       t1->SetTextColor(kRed);
       TText *t2 = new TText(500,10000,"Average charge (a.u.) -  Row2");
       t2->SetTextColor(kGreen+2);
       TText *t3 = new TText(500,10000,"Average charge (a.u.) -  Row3");
       t3->SetTextColor(kOrange+2);
       TText *t4 = new TText(500,10000,"Average charge (a.u.) -  Row4");
       t4->SetTextColor(kMagenta+2);
       TCanvas *c = new TCanvas("c");
       c->Divide(3,2);
       c->cd(1);
       g_z->Draw();
       t0->Draw("same");
       c->cd(2);
       g_u->Draw();
       t1->Draw("same");
       c->cd(3);
       g_d->Draw();
       t2->Draw("same");
       c->cd(4);
       g_t->Draw();
       t3->Draw("same");
       c->cd(5);
       g_q->Draw();
       t4->Draw("same");
  
  
  
  }
