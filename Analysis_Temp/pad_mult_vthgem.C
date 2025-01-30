//*******************************************************************************
//*                                                                             *
//*    This macro plots the pad multiclicity as a function of the potential     *
//*    difference established at the THGEM.                                     *
//*                                                                             *
//*******************************************************************************

#define N  6

void pad_mult_vthgem()
   {
       
       // Declaration of variables of interest
       
       Double_t vthgem[N] = {150.,160.,175.,180.,190.,200.};
       Double_t pad_mult[N] = {2.994,3.54,5.414,5.798,6.372,6.8};
       
       TGraph *gr = new TGraph(N,vthgem,pad_mult);
       gr->SetTitle("Pad multiplicity vs V_{THGEM}");
       gr->GetXaxis()->SetTitle("V_{THGEM} (V)");
       gr->GetYaxis()->SetTitle("pad multiplicity");
       gr->GetXaxis()->SetRangeUser(140.,220.);
       gr->GetYaxis()->SetRangeUser(0,8);
       gr->SetLineWidth(0);
       gr->SetMarkerStyle(8);
       gr->SetMarkerColor(kBlue);
       
       // TLatex objects to identify each run
       TLatex *t1 = new TLatex(147.93,3.47,"Run 11");
       t1->SetTextSize(0.04);
       TLatex *t2 = new TLatex(157.46,3.78,"Run 10");
       t2->SetTextSize(0.04);
       TLatex *t3 = new TLatex(172.15,5.58,"Run 6");
       t3->SetTextSize(0.04);
       TLatex *t4 = new TLatex(178.28,6.04,"Run 7");
       t4->SetTextSize(0.04);
       TLatex *t5 = new TLatex(187.93,6.54,"Run 8");
       t5->SetTextSize(0.04);
       TLatex *t6 = new TLatex(197.06,7.07,"Run 9");
       t6->SetTextSize(0.04);
       
       
       // Visualization block
       TCanvas *c = new TCanvas("c");
       c->cd();
       gr->Draw();
       t1->Draw("same");
       t2->Draw("same");
       t3->Draw("same");
       t4->Draw("same");
       t5->Draw("same");
       t6->Draw("same");
       
   
   
   } 
