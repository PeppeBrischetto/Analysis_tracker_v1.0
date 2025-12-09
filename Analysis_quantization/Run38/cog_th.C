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

const TString InputFileC = "resol_run_52ELLcut.txt";
const TString InputFileC1 = "resol_run_38ELLcut.txt";
const TString InputFileC2 = "resol_run_65ELLcut.txt";
const TString InputFileC3 = "resol_run_76ELLcut.txt";
const TString InputFileLi = "resol_run_298ELLcut.txt";
const TString InputFileO = "resol_run_171ELLcut.txt";

const Int_t NRows = 5;

void cog_th(){

// Needed variables
   
   Double_t threshold[12] = {400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600};
   Double_t xtot0[12] = {64.2239,64.2239,64.2239, 64.2239,64.2239,64.2239,64.2239,64.2239,64.2239,64.2239,64.2239,64.2239};
   Double_t xtot1[12] = {105.644,105.644,105.644,105.644,105.644,105.644,105.644,105.644,105.644,105.644,105.644,105.644};
   Double_t xtot2[12] = {142.486,142.486,142.486,142.486,142.486,142.486,142.486,142.486,142.486,142.486,142.486,142.486};
   Double_t xtot3[12] = {179.384,179.384,179.384,179.384,179.384,179.384,179.384,179.384,179.384,179.384,179.384};
   Double_t xtot4[12] = {221.134,221.134,221.134,221.134,221.134,221.134,221.134,221.134,221.134,221.134,221.134};
   Double_t err_xtot0[12] = {0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447,0.00628447};
   Double_t err_xtot1[12] = {0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105,0.00721105};
   Double_t err_xtot2[12] = {0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158,0.00838158};
   Double_t err_xtot3[12] = {0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212,0.0106212};
   Double_t err_xtot4[12] = {0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926,0.0158926};
   
   Double_t x0[12] = {64.2239,64.2239,64.2237,64.2292,64.2564,64.2856,64.2823,64.2585,64.2044,64.1306,64.0453,63.9572,63.8587};
   Double_t x1[12] = {105.644,105.644,105.644,105.651,105.696,105.742,105.74,105.705,105.625,105.517,105.395,105.279,105.144};
   Double_t x2[12] = {142.486,142.486,142.487,142.508,142.622,142.72,142.728,142.68,142.573,142.435,142.287,142.125,141.974};
   Double_t x3[12] = {179.384,179.384,179.384,179.398,179.478,179.551,179.549,179.488,179.362,179.197,179.019,178.804,178.652};
   Double_t x4[12] = {221.134,221.134,221.134,221.155,221.279,221.389,221.383,221.287,221.091,220.844,220.583,220.305,220.115};
   Double_t err_x0[12] = {0.00628447,0.0062858,0.00629635,0.00635831,0.00658749,0.00657983,0.00658404,0.00676319,0.0073456,0.00857639,0.011086,0.0161739,0.0281741};
   Double_t err_x1[12] = {0.00721105,0.00721304,0.00722374,0.00729632,0.00754118,0.00742938,0.00732536,0.00737391,0.00781975,0.0087957,0.0109405,0.0153617,0.0255526};
   Double_t err_x2[12] = {0.00838158,0.00838474,0.00839935,0.00845055,0.00840634,0.00772648,0.00717938,0.00675228,0.00673293,0.00737799,0.00855565,0.0116377,0.0204224};
   Double_t err_x3[12] = {0.0106212,0.0106256,0.0106452,0.0107609,0.0110782,0.0106766,0.0102086,0.00996071,0.0103822,0.012017,0.014888,0.0217164,0.0391527};
   Double_t err_x4[12] = {0.0158926,0.0158995,0.0159287,0.0161002,0.0165846,0.0160224,0.0154542,0.0151849,0.0158242,0.0182948,0.0227145,0.0330727,0.060219};
   
   Double_t anti_x0[12] = {0,64.3156,64.3144,63.9695,63.9091,63.8766,63.9736,64.1216,64.2573,64.3022,64.2918,64.2644,64.242};
   Double_t anti_x1[12] = {0,106.226,105.728,105.31,105.134,105.09,105.232,105.462,105.676,105.75,105.738,105.699,105.668};
   Double_t anti_x2[12] = {0,142.924,142.282,141.446,141.17,141.169,141.45,141.914,142.339,142.53,142.562,142.541,142.512};
   Double_t anti_x3[12] = {0,179.699,179.407,178.728,178.47,178.443,178.678,179.077,179.421,179.541,179.523,179.472,179.42};
   Double_t anti_x4[12] = {0,221.589,221.01,220.136,219.729,219.697,220.066,220.681,221.207,221.377,221.344,221.26,221.184};
   Double_t err_anti_x0[12] = {0,0.302965,0.0934555,0.0388569,0.0194632,0.0177613,0.0169223,0.014688,0.0114827,0.00897365,0.00750253,0.00675531,0.00641747};
   Double_t err_anti_x1[12] = {0,0.230364,0.115785,0.0436036,0.0211093,0.0203305,0.0202691,0.0181686,0.0142272,0.0109116,0.00891457,0.00788559,0.0074147};
   Double_t err_anti_x2[12] = {0,0.185142,0.103187,0.0368511,0.0186192,0.0224005,0.0256506,0.0247667,0.0194235,0.0141028,0.0110355,0.00942064,0.00869603};
   Double_t err_anti_x3[12] = {0,0.194724,0.118541,0.0541298,0.0293714,0.0314777,0.0327318,0.0295657,0.0226174,0.0165772,0.0133389,0.0116334,0.0109088};
   Double_t err_anti_x4[12] = {0,0.209545,0.174419,0.0811504,0.0429623,0.0462488,0.0478579,0.0434158,0.0334437,0.0246253,0.0198828,0.0174015,0.0163222};
   
   TGraphErrors *gr_xtot[NRows];                                               // TGraphErrors-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"gr_xtot %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr_xtot[i]=new TH1D("","",9000.,0.,300);
      gr_xtot[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr_xtot[i]->GetYaxis()->SetTitle(tit);
      gr_xtot[i]->GetXaxis()->SetTitleSize(0.05);
      gr_xtot[i]->GetXaxis()->SetLabelSize(0.05);
      gr_xtot[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_xtot[i]->GetYaxis()->SetTitleSize(0.05);
      gr_xtot[i]->GetYaxis()->SetLabelSize(0.05);
      gr_xtot[i]->GetYaxis()->SetTitleOffset(1.);
      gr_xtot[i]->SetLineColor(kBlue+1);
      gr_xtot[i]->SetLineWidth(2);
   }
   
   TGraphErrors *gr_x[NRows];                                               // TGraphErrors-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"gr_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr_x[i]=new TH1D("","",9000.,0.,300);
      gr_x[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr_x[i]->GetYaxis()->SetTitle(tit);
      gr_x[i]->GetXaxis()->SetTitleSize(0.05);
      gr_x[i]->GetXaxis()->SetLabelSize(0.05);
      gr_x[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_x[i]->GetYaxis()->SetTitleSize(0.05);
      gr_x[i]->GetYaxis()->SetLabelSize(0.05);
      gr_x[i]->GetYaxis()->SetTitleOffset(1.);
      gr_x[i]->SetLineColor(kBlue+1);
      gr_x[i]->SetLineWidth(2);
   }
   
   TGraphErrors *gr_anti_x[NRows];                                               // TGraphErrors-array for the rows
   for(int i=0; i<NRows; i++){
      sprintf(histoname,"gr_anti_x %i",i);
      sprintf(tit,"x_{%d} (mm)",i);
      gr_anti_x[i]=new TH1D("","",9000.,0.,300);
      gr_anti_x[i]->GetXaxis()->SetTitle("Threshold (bit)");
      gr_anti_x[i]->GetYaxis()->SetTitle(tit);
      gr_anti_x[i]->GetXaxis()->SetTitleSize(0.05);
      gr_anti_x[i]->GetXaxis()->SetLabelSize(0.05);
      gr_anti_x[i]->GetXaxis()->SetTitleOffset(0.9);
      gr_anti_x[i]->GetYaxis()->SetTitleSize(0.05);
      gr_anti_x[i]->GetYaxis()->SetLabelSize(0.05);
      gr_anti_x[i]->GetYaxis()->SetTitleOffset(1.);
      gr_anti_x[i]->SetLineColor(kBlue+1);
      gr_anti_x[i]->SetLineWidth(2);
   }
   
   
   for(Int_t row=0; row<NRwos; row++){
      
   }

}
