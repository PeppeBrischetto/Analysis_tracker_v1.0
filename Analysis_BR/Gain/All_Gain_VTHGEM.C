#ifdef __CLING__
#pragma cling optimize(0)
#endif
void All_Gain_VTHGEM()
{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Sep 16 10:00:50 2025) by ROOT version 6.32.08
   TCanvas *c1 = new TCanvas("c1", "c1",165,159,950,900);
   c1->Range(136.25,2.432631,218.75,4.793527);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLogy();
   c1->SetGridx();
   c1->SetGridy();
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   Double_t Graph_fx1[7] = { 150, 160, 160, 170, 180, 190, 205 };
   Double_t Graph_fy1[7] = { 518.177, 1114.95, 1114.95, 3357.26, 8809.56, 17059.7, 32860 };
   TGraph *graph = new TGraph(7,Graph_fx1,Graph_fy1);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#ff00ff");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","",100,144.5,210.5);
   Graph_Graph1->SetMinimum(466.3593);
   Graph_Graph1->SetMaximum(36094.18);
   Graph_Graph1->SetDirectory(nullptr);
   Graph_Graph1->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("Gain");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph1);
   
   graph->Draw("apl ");
   
   Double_t Graph_fx2[6] = { 150, 160, 170, 180, 190, 200 };
   Double_t Graph_fy2[6] = { 514.579, 1222.41, 3175, 8042.44, 15651, 27330.5 };
   graph = new TGraph(6,Graph_fx2,Graph_fy2);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#990099");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#990099");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","",100,145,205);
   Graph_Graph2->SetMinimum(463.1211);
   Graph_Graph2->SetMaximum(30012.09);
   Graph_Graph2->SetDirectory(nullptr);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetTitleOffset(1);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetTitle("Gain");
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetTitleOffset(1);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph2);
   
   graph->Draw("pl ");
   
   Double_t Graph_fx3[5] = { 150, 160, 170, 180, 190 };
   Double_t Graph_fy3[5] = { 517.765, 1252.71, 3314.95, 7451.59, 14190.4 };
   graph = new TGraph(5,Graph_fx3,Graph_fy3);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#660066");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#660066");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","",100,146,194);
   Graph_Graph3->SetMinimum(465.9885);
   Graph_Graph3->SetMaximum(15557.66);
   Graph_Graph3->SetDirectory(nullptr);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetTitleOffset(1);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetTitle("Gain");
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetTitleOffset(1);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph3);
   
   graph->Draw("pl ");
   
   Double_t Graph_fx4[6] = { 150, 160, 170, 180, 190, 190 };
   Double_t Graph_fy4[6] = { 302.17, 1098.42, 2958.45, 7044.79, 13195.2, 13195.2 };
   graph = new TGraph(6,Graph_fx4,Graph_fy4);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#ff9933");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","",100,146,194);
   Graph_Graph4->SetMinimum(271.953);
   Graph_Graph4->SetMaximum(14484.5);
   Graph_Graph4->SetDirectory(nullptr);
   Graph_Graph4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetTitleOffset(1);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetTitle("Gain");
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetTitleOffset(1);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph4);
   
   graph->Draw("pl ");
   
   Double_t Graph_fx5[5] = { 160, 170, 180, 190, 190 };
   Double_t Graph_fy5[5] = { 2190.61, 2251.28, 6349.59, 14523.5, 14523.5 };
   graph = new TGraph(5,Graph_fx5,Graph_fy5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#cccc00");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#cccc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","",100,157,193);
   Graph_Graph5->SetMinimum(957.321);
   Graph_Graph5->SetMaximum(15756.79);
   Graph_Graph5->SetDirectory(nullptr);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetTitleOffset(1);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetTitle("Gain");
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetTitleOffset(1);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph5);
   
   graph->Draw("pl ");
   
   Double_t Graph_fx6[5] = { 160, 170, 180, 190, 190 };
   Double_t Graph_fy6[5] = { 1519.5, 3188.83, 6349.59, 14523.5, 14523.5 };
   graph = new TGraph(5,Graph_fx6,Graph_fy6);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#00cc00");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","",100,157,193);
   Graph_Graph6->SetMinimum(219.1);
   Graph_Graph6->SetMaximum(15823.9);
   Graph_Graph6->SetDirectory(nullptr);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetTitleOffset(1);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetTitle("Gain");
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetTitleOffset(1);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph6);
   
   graph->Draw("pl ");
   
   Double_t Graph0_fx1001[14] = { 110, 120, 130, 140, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195 };
   Double_t Graph0_fy1001[14] = { 3.162982, 14.12768, 44.33237, 146.365, 442.3855, 763.8623, 1265.77, 2064.735, 3294.649, 5163.408, 7918.819, 12246.12, 18360.79, 27380.56 };
   Double_t Graph0_fex1001[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   Double_t Graph0_fey1001[14] = { 0.5069923, 0.540561, 0.5724622, 0.999392, 2.706464, 4.561151, 7.108719, 13.03431, 17.80478, 29.38534, 45.5381, 71.50489, 105.1503, 151.9851 };
   TGraphErrors *gre = new TGraphErrors(14,Graph0_fx1001,Graph0_fy1001,Graph0_fex1001,Graph0_fey1001);
   gre->SetName("Graph0");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);

   ci = TColor::GetColor("#009999");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009999");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_Graph01001 = new TH1F("Graph_Graph01001","Graph",100,101.5,203.5);
   Graph_Graph01001->SetMinimum(2.390391);
   Graph_Graph01001->SetMaximum(30285.53);
   Graph_Graph01001->SetDirectory(nullptr);
   Graph_Graph01001->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph01001->SetLineColor(ci);
   Graph_Graph01001->GetXaxis()->SetLabelFont(42);
   Graph_Graph01001->GetXaxis()->SetTitleOffset(1);
   Graph_Graph01001->GetXaxis()->SetTitleFont(42);
   Graph_Graph01001->GetYaxis()->SetLabelFont(42);
   Graph_Graph01001->GetYaxis()->SetTitleFont(42);
   Graph_Graph01001->GetZaxis()->SetLabelFont(42);
   Graph_Graph01001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01001);
   
   gre->Draw("pl ");
   
   TLegend *leg = new TLegend(0.1,0.5,0.35,0.9,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("","#alpha","lp");

   ci = TColor::GetColor("#00cc00");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#00cc00");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{7}Li - 1000 Hz","lp");

   ci = TColor::GetColor("#cccc00");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#cccc00");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{12}C - 20 Hz","lp");

   ci = TColor::GetColor("#ff00ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ff00ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{12}C - 250 Hz","lp");

   ci = TColor::GetColor("#cc00cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#cc00cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{12}C - 1000 Hz","lp");

   ci = TColor::GetColor("#990099");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990099");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{12}C - 3000 Hz","lp");

   ci = TColor::GetColor("#660066");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#660066");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("","^{16}O - 250 Hz","lp");

   ci = TColor::GetColor("#ff9933");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ff9933");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph0","#alpha @INFN-LNS","lp");

   ci = TColor::GetColor("#009999");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#009999");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
  
// ------------>Primitives in pad: zoomPad
   TPad *zoomPad__0 = new TPad("zoomPad", "Zoom",0.5221519,0.1004566,0.899789,0.5159817);
   zoomPad__0->Draw();
   zoomPad__0->cd();
   zoomPad__0->Range(156.2975,2.882684,163.7225,3.500679);
   zoomPad__0->SetFillColor(0);
   zoomPad__0->SetBorderMode(0);
   zoomPad__0->SetBorderSize(2);
   zoomPad__0->SetLogy();
   zoomPad__0->SetGridx();
   zoomPad__0->SetGridy();
   zoomPad__0->SetFrameFillStyle(0);
   zoomPad__0->SetFrameBorderMode(0);
   zoomPad__0->SetFrameFillStyle(0);
   zoomPad__0->SetFrameBorderMode(0);
   
   Double_t gain2_cl_fx7[7] = { 150, 160, 160, 170, 180, 190, 205 };
   Double_t gain2_cl_fy7[7] = { 518.177, 1114.95, 1114.95, 3357.26, 8809.56, 17059.7, 32860 };
   graph = new TGraph(7,gain2_cl_fx7,gain2_cl_fy7);
   graph->SetName("gain2_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#ff00ff");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#ff00ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain2_cl7 = new TH1F("Graph_gain2_cl7","",100,144.5,210.5);
   Graph_gain2_cl7->SetMinimum(880.0025);
   Graph_gain2_cl7->SetMaximum(2747.135);
   Graph_gain2_cl7->SetDirectory(nullptr);
   Graph_gain2_cl7->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain2_cl7->SetLineColor(ci);
   Graph_gain2_cl7->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain2_cl7->GetXaxis()->SetRange(20,28);
   Graph_gain2_cl7->GetXaxis()->SetLabelFont(42);
   Graph_gain2_cl7->GetXaxis()->SetTitleOffset(1);
   Graph_gain2_cl7->GetXaxis()->SetTitleFont(42);
   Graph_gain2_cl7->GetYaxis()->SetTitle("Gain");
   Graph_gain2_cl7->GetYaxis()->SetRange(1,1);
   Graph_gain2_cl7->GetYaxis()->SetLabelFont(42);
   Graph_gain2_cl7->GetYaxis()->SetTitleFont(42);
   Graph_gain2_cl7->GetZaxis()->SetLabelFont(42);
   Graph_gain2_cl7->GetZaxis()->SetTitleOffset(1);
   Graph_gain2_cl7->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain2_cl7);
   
   graph->Draw("apl ");
   
   Double_t gain3_cl_fx8[6] = { 150, 160, 170, 180, 190, 200 };
   Double_t gain3_cl_fy8[6] = { 514.579, 1222.41, 3175, 8042.44, 15651, 27330.5 };
   graph = new TGraph(6,gain3_cl_fx8,gain3_cl_fy8);
   graph->SetName("gain3_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#990099");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#990099");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain3_cl8 = new TH1F("Graph_gain3_cl8","",100,145,205);
   Graph_gain3_cl8->SetMinimum(463.1211);
   Graph_gain3_cl8->SetMaximum(30012.09);
   Graph_gain3_cl8->SetDirectory(nullptr);
   Graph_gain3_cl8->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain3_cl8->SetLineColor(ci);
   Graph_gain3_cl8->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain3_cl8->GetXaxis()->SetLabelFont(42);
   Graph_gain3_cl8->GetXaxis()->SetTitleOffset(1);
   Graph_gain3_cl8->GetXaxis()->SetTitleFont(42);
   Graph_gain3_cl8->GetYaxis()->SetTitle("Gain");
   Graph_gain3_cl8->GetYaxis()->SetLabelFont(42);
   Graph_gain3_cl8->GetYaxis()->SetTitleFont(42);
   Graph_gain3_cl8->GetZaxis()->SetLabelFont(42);
   Graph_gain3_cl8->GetZaxis()->SetTitleOffset(1);
   Graph_gain3_cl8->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain3_cl8);
   
   graph->Draw("pl ");
   
   Double_t gain4_cl_fx9[5] = { 150, 160, 170, 180, 190 };
   Double_t gain4_cl_fy9[5] = { 517.765, 1252.71, 3314.95, 7451.59, 14190.4 };
   graph = new TGraph(5,gain4_cl_fx9,gain4_cl_fy9);
   graph->SetName("gain4_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#660066");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#660066");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain4_cl9 = new TH1F("Graph_gain4_cl9","",100,146,194);
   Graph_gain4_cl9->SetMinimum(465.9885);
   Graph_gain4_cl9->SetMaximum(15557.66);
   Graph_gain4_cl9->SetDirectory(nullptr);
   Graph_gain4_cl9->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain4_cl9->SetLineColor(ci);
   Graph_gain4_cl9->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain4_cl9->GetXaxis()->SetLabelFont(42);
   Graph_gain4_cl9->GetXaxis()->SetTitleOffset(1);
   Graph_gain4_cl9->GetXaxis()->SetTitleFont(42);
   Graph_gain4_cl9->GetYaxis()->SetTitle("Gain");
   Graph_gain4_cl9->GetYaxis()->SetLabelFont(42);
   Graph_gain4_cl9->GetYaxis()->SetTitleFont(42);
   Graph_gain4_cl9->GetZaxis()->SetLabelFont(42);
   Graph_gain4_cl9->GetZaxis()->SetTitleOffset(1);
   Graph_gain4_cl9->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain4_cl9);
   
   graph->Draw("pl ");
   
   Double_t gain5_cl_fx10[6] = { 150, 160, 170, 180, 190, 190 };
   Double_t gain5_cl_fy10[6] = { 302.17, 1098.42, 2958.45, 7044.79, 13195.2, 13195.2 };
   graph = new TGraph(6,gain5_cl_fx10,gain5_cl_fy10);
   graph->SetName("gain5_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#ff9933");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#ff9933");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain5_cl10 = new TH1F("Graph_gain5_cl10","",100,146,194);
   Graph_gain5_cl10->SetMinimum(271.953);
   Graph_gain5_cl10->SetMaximum(14484.5);
   Graph_gain5_cl10->SetDirectory(nullptr);
   Graph_gain5_cl10->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain5_cl10->SetLineColor(ci);
   Graph_gain5_cl10->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain5_cl10->GetXaxis()->SetLabelFont(42);
   Graph_gain5_cl10->GetXaxis()->SetTitleOffset(1);
   Graph_gain5_cl10->GetXaxis()->SetTitleFont(42);
   Graph_gain5_cl10->GetYaxis()->SetTitle("Gain");
   Graph_gain5_cl10->GetYaxis()->SetLabelFont(42);
   Graph_gain5_cl10->GetYaxis()->SetTitleFont(42);
   Graph_gain5_cl10->GetZaxis()->SetLabelFont(42);
   Graph_gain5_cl10->GetZaxis()->SetTitleOffset(1);
   Graph_gain5_cl10->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain5_cl10);
   
   graph->Draw("pl ");
   
   Double_t gain6_cl_fx11[5] = { 160, 170, 180, 190, 190 };
   Double_t gain6_cl_fy11[5] = { 2190.61, 2251.28, 6349.59, 14523.5, 14523.5 };
   graph = new TGraph(5,gain6_cl_fx11,gain6_cl_fy11);
   graph->SetName("gain6_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#cccc00");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#cccc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain6_cl11 = new TH1F("Graph_gain6_cl11","",100,157,193);
   Graph_gain6_cl11->SetMinimum(957.321);
   Graph_gain6_cl11->SetMaximum(15756.79);
   Graph_gain6_cl11->SetDirectory(nullptr);
   Graph_gain6_cl11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain6_cl11->SetLineColor(ci);
   Graph_gain6_cl11->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain6_cl11->GetXaxis()->SetLabelFont(42);
   Graph_gain6_cl11->GetXaxis()->SetTitleOffset(1);
   Graph_gain6_cl11->GetXaxis()->SetTitleFont(42);
   Graph_gain6_cl11->GetYaxis()->SetTitle("Gain");
   Graph_gain6_cl11->GetYaxis()->SetLabelFont(42);
   Graph_gain6_cl11->GetYaxis()->SetTitleFont(42);
   Graph_gain6_cl11->GetZaxis()->SetLabelFont(42);
   Graph_gain6_cl11->GetZaxis()->SetTitleOffset(1);
   Graph_gain6_cl11->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain6_cl11);
   
   graph->Draw("pl ");
   
   Double_t gain7_cl_fx12[5] = { 160, 170, 180, 190, 190 };
   Double_t gain7_cl_fy12[5] = { 1519.5, 3188.83, 6349.59, 14523.5, 14523.5 };
   graph = new TGraph(5,gain7_cl_fx12,gain7_cl_fy12);
   graph->SetName("gain7_cl");
   graph->SetTitle("");
   graph->SetFillStyle(1000);

   ci = TColor::GetColor("#00cc00");
   graph->SetLineColor(ci);

   ci = TColor::GetColor("#00cc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(8);
   
   TH1F *Graph_gain7_cl12 = new TH1F("Graph_gain7_cl12","",100,157,193);
   Graph_gain7_cl12->SetMinimum(219.1);
   Graph_gain7_cl12->SetMaximum(15823.9);
   Graph_gain7_cl12->SetDirectory(nullptr);
   Graph_gain7_cl12->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gain7_cl12->SetLineColor(ci);
   Graph_gain7_cl12->GetXaxis()->SetTitle("V_{THGEM} (V)");
   Graph_gain7_cl12->GetXaxis()->SetLabelFont(42);
   Graph_gain7_cl12->GetXaxis()->SetTitleOffset(1);
   Graph_gain7_cl12->GetXaxis()->SetTitleFont(42);
   Graph_gain7_cl12->GetYaxis()->SetTitle("Gain");
   Graph_gain7_cl12->GetYaxis()->SetLabelFont(42);
   Graph_gain7_cl12->GetYaxis()->SetTitleFont(42);
   Graph_gain7_cl12->GetZaxis()->SetLabelFont(42);
   Graph_gain7_cl12->GetZaxis()->SetTitleOffset(1);
   Graph_gain7_cl12->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_gain7_cl12);
   
   graph->Draw("pl ");
   
   Double_t gr4_cl_fx1002[14] = { 110, 120, 130, 140, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195 };
   Double_t gr4_cl_fy1002[14] = { 3.162982, 14.12768, 44.33237, 146.365, 442.3855, 763.8623, 1265.77, 2064.735, 3294.649, 5163.408, 7918.819, 12246.12, 18360.79, 27380.56 };
   Double_t gr4_cl_fex1002[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   Double_t gr4_cl_fey1002[14] = { 0.5069923, 0.540561, 0.5724622, 0.999392, 2.706464, 4.561151, 7.108719, 13.03431, 17.80478, 29.38534, 45.5381, 71.50489, 105.1503, 151.9851 };
   gre = new TGraphErrors(14,gr4_cl_fx1002,gr4_cl_fy1002,gr4_cl_fex1002,gr4_cl_fey1002);
   gre->SetName("gr4_cl");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);

   ci = TColor::GetColor("#009999");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009999");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gr4_cl1002 = new TH1F("Graph_gr4_cl1002","Graph",100,101.5,203.5);
   Graph_gr4_cl1002->SetMinimum(2.390391);
   Graph_gr4_cl1002->SetMaximum(30285.53);
   Graph_gr4_cl1002->SetDirectory(nullptr);
   Graph_gr4_cl1002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gr4_cl1002->SetLineColor(ci);
   Graph_gr4_cl1002->GetXaxis()->SetLabelFont(42);
   Graph_gr4_cl1002->GetXaxis()->SetTitleOffset(1);
   Graph_gr4_cl1002->GetXaxis()->SetTitleFont(42);
   Graph_gr4_cl1002->GetYaxis()->SetLabelFont(42);
   Graph_gr4_cl1002->GetYaxis()->SetTitleFont(42);
   Graph_gr4_cl1002->GetZaxis()->SetLabelFont(42);
   Graph_gr4_cl1002->GetZaxis()->SetTitleOffset(1);
   Graph_gr4_cl1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gr4_cl1002);
   
   gre->Draw("pl ");
   zoomPad__0->Modified();
   c1->cd();
   c1->Modified();
   c1->SetSelected(c1);
}
