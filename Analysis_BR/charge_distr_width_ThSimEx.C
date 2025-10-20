//###########################################################################################################################
//#   This macro allows to plot the charge distribution width as a function of theta in each of the following blocks:
//#
//#      1. Theoretical bl.: here the charge distribution width is predicted performing the direct of two contributions:
//#         a. diffusion c.: it is due to the vertical drift motion of the electrons to reach the anode;
//#         b. geometricalo c.: it is due to the geometry, such that more inclined tracks trigger a higher number of pads;
//#
//#      2. Simulation bl.: here the charge distribution width is directly computed by means of a python simulation returning:
//#         a. the anode hitmap;
//#         b. the charge distribution (in #.electrons) for each of the pad-segmented rows;
//#         c. the charge distribution (in Ch. of the VX2745-Caen-digitizer) for each of the pad-segmented rows;
//#
//#      3. Experimental bl.: here the charge distribution width is obtained analyzing data. An important feature to take 
//#         into account is the presence of the alpha-source on the beam-data: here a "cl_x_mm[1]:cl_x_mm[0]-TCutg is needed.
//#         Be sure your TCutg is well defined all over the run: verify it in advance opnening on root the track_runID.root
//#         file, thus drwaing the TCutg.
//#
//#   The runID has to be given in the macro devoted const TString at the beginning.
//#
//###########################################################################################################################
//#   creatd July 2025 by A. Pitronaci 
//###########################################################################################################################
//#   updated: July 2025 by A. Pitronaci: simulation is now ok, it reports also the charge distr. width in channels;
//#   updated: July 2025 by A. Pitornaci: first tempt python simulation;
//#   updated: July 2025 by A. Pitronaci: changing Th. contr.: direct sum, alwyas taking into account the thresholds;
//#   updated: June 2025 by A. Pitronaci: confirming Th. contribution as squared sum (NUMEN-CM & MAYORANA-2025);
//#   updated: June 2025 by A. Pitronaci: changing Th. contribution: take into account the VX2745-Caen-digitizer thresholds;
//#   updated: June 2025 by A. Pitronaci: changing Th. contr.: squared sum;
//#   updated: June 2025 by A. Pitronaci: changing Th. contr.: squared sum;
//#   updated: June 2025 by A. Pitronaci: changing Th. contr.: folding;
//#   updated: May 2025 by A. Pitronaci: changing Th. contr.: direct sum;
//#   updated: May 2025 by A. Pitronaci: Assran & Ferbel data;
//#   updated: April 2025 by A. Pitronaci: squared sum of the two contribution to the charge distribution width;
//#   updated: March 2025: study on the two contributions (diffusion and geometrical one) separately;
//###########################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TString.h>
#include <TLegend.h>

using namespace std;
using namespace TMath;

const Double_t drift_time = 1.02E-6;                                                  // drift time in s
const Double_t mobility = 9.37E+6;                                                    // electron mobility (in mm^2V^-1s^-1) in I-C4H10 with u as drift velocity
const Double_t drift_velocity = 5.0E6;                                                // electron drift velocity (mm/s) in I-C4H10 at saturation regime
const Double_t me = 9.11E-31;                                                         // electron mass expressed in Kg
const Double_t e = 1.6022E-19;                                                        // electron charge
const Double_t padWidth = 5.0;                                                        // pad width expressed in mm
const Double_t padLenght = 10.2;                                                      // pad lenght expressed in mm
const Double_t KB_T = 0.0259;                                                         // product of the Boltzmann constato and the temperature (K_B \cdot T) expressed in eV    
const Double_t l_drift = 99.0;                                                        // electron drift path expressed in mm
const Double_t sigma_x = 140*9.9/1000;                                                // Average trans. diff. w. for e- moving in Ar-isobutane (Assran) at 10 mbar, measured in mm - (Y. Assran p.22/36)
const Double_t sigma_x_9mbar = 145*9.9/1000;                                          // Average trans. diff. w. for e- moving in Ar-isobutane (Assran) at 9 mbar, measured in mm - (Y. Assran p.22/36)
const Double_t sigma_x_11mbar = 130*9.9/1000;                                         // Average trans. diff. w. for e- moving in Ar-isobutane (Assran) at 11 mbar, measured in mm - (Y. Assran p.22/36)
const Int_t NPads = 60;                                                               // Number of active pads of the gas tracker
const Int_t NRows = 5;                                                                // Number of the pad-segmented rows of the as tracker
const Int_t NAngles = 10;                                                             // Number of adopted angles to obtain the theoretical predictions

const TString InputSimul = "SimulFile.txt";

const TString InputFileC = "OutputFile_chargeDistrib/Run202.txt";
const TString InputFileLi = "OutputFile_chargeDistrib/Run215.txt";
const TString InputFileHe = "OutputFile_chargeDistrib/Run298Alpha.txt";
const TString InputFileO = "OutputFile_chargeDistrib/Run171.txt";
const TString InputFileC1 = "OutputFile_chargeDistrib/Run184.txt";
const TString InputFileC2 = "OutputFile_chargeDistrib/Run193.txt";
const TString InputFileC3 = "OutputFile_chargeDistrib/Run76.txt";


void charge_distr_width_ThSimEx(){

//###########################################################################################################################
//# THEORETICAL BLOCK:
   
    // Needed variables
    Double_t theta_deg[NAngles] = {0.,10.,20.,30.,40.,50.,60.,65.,70.,80.};
    
    Double_t n_dev_std[NRows] = {4.61,4.61,4.19,4.93,4.93};
    Double_t x_mm[NAngles] = {0.};
    Double_t x_mm_9mbar[NAngles] = {0.};
    Double_t x_mm_11mbar[NAngles] = {0.};
    Double_t x_geo_mm[NAngles] = {0.};
    Double_t x_diff_mm[10] = {0.};
    Double_t x_diff_mm_9mbar[10] = {0.};
    Double_t x_diff_mm_11mbar[10] = {0.};
    
    TGraph *gr1[5];
    for(Int_t r=0; r<NRows; r++){
       gr1[r] = new TGraph();
       gr1[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1[r]->SetLineWidth(3);
       gr1[r]->SetLineStyle(1);
       gr1[r]->SetMarkerStyle(1);
       gr1[r]->SetLineColor(kBlack);
       gr1[r]->SetMarkerColor(kBlack);
       gr1[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_9mbar[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_9mbar[r] = new TGraph();
       gr1_9mbar[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_9mbar[r]->SetLineWidth(3);
       gr1_9mbar[r]->SetLineStyle(1);
       gr1_9mbar[r]->SetMarkerStyle(1);
       gr1_9mbar[r]->SetMarkerColor(kBlue);
       gr1_9mbar[r]->SetLineColor(kBlue);
       gr1_9mbar[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_9mbar[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_11mbar[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_11mbar[r] = new TGraph();
       gr1_11mbar[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_11mbar[r]->SetLineWidth(3);
       gr1_11mbar[r]->SetLineStyle(1);
       gr1_11mbar[r]->SetMarkerStyle(1);
       gr1_11mbar[r]->SetMarkerColor(kRed);
       gr1_11mbar[r]->SetLineColor(kRed);
       gr1_11mbar[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_11mbar[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_geo[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_geo[r] = new TGraph();
       gr1_geo[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_geo[r]->SetLineWidth(2);
       gr1_geo[r]->SetLineStyle(2);
       gr1_geo[r]->SetMarkerStyle(1);
       gr1_geo[r]->SetMarkerColor(kBlack);
       gr1_geo[r]->SetLineColor(kBlack);
       gr1_geo[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_geo[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_diff[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_diff[r] = new TGraph();
       gr1_diff[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_diff[r]->SetLineWidth(1);
       gr1_diff[r]->SetLineStyle(10);
       gr1_diff[r]->SetMarkerStyle(1);
       gr1_diff[r]->SetMarkerColor(kBlack);
       gr1_diff[r]->SetLineColor(kBlack);
       gr1_diff[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_diff[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_diff_9mbar[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_diff_9mbar[r] = new TGraph();
       gr1_diff_9mbar[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_diff_9mbar[r]->SetLineWidth(1);
       gr1_diff_9mbar[r]->SetLineStyle(10);
       gr1_diff_9mbar[r]->SetMarkerStyle(1);
       gr1_diff_9mbar[r]->SetMarkerColor(kBlue);
       gr1_diff_9mbar[r]->SetLineColor(kBlue);
       gr1_diff_9mbar[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_diff_9mbar[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    TGraph *gr1_diff_11mbar[5];
    for(Int_t r=0; r<NRows; r++){
       gr1_diff_11mbar[r] = new TGraph();
       gr1_diff_11mbar[r]->SetTitle("Charge distribution width ~ M-THGEM with RIM");
       gr1_diff_11mbar[r]->SetLineWidth(1);
       gr1_diff_11mbar[r]->SetLineStyle(10);
       gr1_diff_11mbar[r]->SetMarkerStyle(1);
       gr1_diff_11mbar[r]->SetMarkerColor(kRed);
       gr1_diff_11mbar[r]->SetLineColor(kRed);
       gr1_diff_11mbar[r]->GetXaxis()->SetTitle("#vartheta (deg)");
       gr1_diff_11mbar[r]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
    }
    
    // Theoretical calculation
    for(Int_t r=0; r<NRows; r++){
       for(Int_t a=0; a<NAngles; a++){
          x_mm[a] = (n_dev_std[r]*sigma_x*(Cos(theta_deg[a]*Pi()/180))) + (padLenght*Tan(theta_deg[a]*Pi()/180));
          x_mm_9mbar[a] = (n_dev_std[r]*sigma_x_9mbar*(Cos(theta_deg[a]*Pi()/180))) + (padLenght*Tan(theta_deg[a]*Pi()/180));
          x_mm_11mbar[a] = (n_dev_std[r]*sigma_x_11mbar*(Cos(theta_deg[a]*Pi()/180))) + (padLenght*Tan(theta_deg[a]*Pi()/180));
          x_geo_mm[a] = padLenght*Tan(theta_deg[a]*Pi()/180);
          x_diff_mm[a] = n_dev_std[r]*sigma_x*(Cos(theta_deg[a]*Pi()/180));
          x_diff_mm_9mbar[a] = n_dev_std[r]*sigma_x_9mbar*(Cos(theta_deg[a]*Pi()/180));
          x_diff_mm_11mbar[a] = n_dev_std[r]*sigma_x_11mbar*(Cos(theta_deg[a]*Pi()/180));
          gr1[r]->AddPoint(theta_deg[a],x_mm[a]);
          gr1_9mbar[r]->AddPoint(theta_deg[a],x_mm_9mbar[a]);
          gr1_11mbar[r]->AddPoint(theta_deg[a],x_mm_11mbar[a]);
          gr1_geo[r]->AddPoint(theta_deg[a],x_geo_mm[a]);
          gr1_diff[r]->AddPoint(theta_deg[a],x_diff_mm[a]);
          gr1_diff_9mbar[r]->AddPoint(theta_deg[a],x_diff_mm_9mbar[a]);
          gr1_diff_11mbar[r]->AddPoint(theta_deg[a],x_diff_mm_11mbar[a]);
       }
       
    }
    
//###########################################################################################################################
//# SIMULATION BLOCK:
    /* TGraph tracker_simulator - UP (gain by anode+top) & DOWN (gain by anode solo)*/
    /*Double_t thetaSimul[9] = {0.};
    Double_t xSimuld0[9] = {0.};
    Double_t xSimulu0[9] = {0.};
    ifstream sim;
    string badlines;
    
    sim.open(InputSimul);*/
    
    /* Reading-loop */
    /*for(Int_t l=0; l<6; l++){
       getline(sim,badlines);
       cout << badlines << endl;
    }
    
    for(Int_t l=6; l<9; l++){
       for(Int_t c0=0; c0<9; c0++){
          sim >> thetaSimul[c0];
       }
       for(Int_t c1=0; c1<9; c1++){
          sim >> xSimuld0[c1];
       }
       for(Int_t c2=0; c2<9; c2++){
          sim >> xSimulu0[c2];
       }
    }
    
    
    
    TGraph *trackSimuld = new TGraph(9,thetaSimul,xSimuld);
    trackSimuld->SetTitle("Charge distribution width ~ M-THGEM with RIM");
    trackSimuld->SetLineWidth(3);
    trackSimuld->SetLineStyle(8);
    trackSimuld->SetLineColor(kRed-7);
    trackSimuld->SetMarkerStyle(26);
    trackSimuld->SetMarkerColor(kRed-7);
    trackSimuld->SetMarkerSize(1.5);
   
    
    TGraph *trackSimulu = new TGraph(9,thetaSimul,xSimulu);
    trackSimulu->SetTitle("Charge distribution width ~ M-THGEM with RIM");
    trackSimulu->SetLineWidth(3);
    trackSimulu->SetLineStyle(8);
    trackSimulu->SetLineColor(kRed-7);
    trackSimulu->SetMarkerStyle(23);
    trackSimulu->SetMarkerColor(kRed-7);
    trackSimulu->SetMarkerSize(1.5);*/
   
//###########################################################################################################################
//# EXPERIMENTAL BLOCK:    
    
    char histoname[100];
    char gr_name[100];
    ifstream f_he,f_li,f_c,f_c1,f_c2,f_c3,f_o;
    f_li.open(InputFileLi);
    f_c.open(InputFileC);
    f_c1.open(InputFileC1);
    f_c2.open(InputFileC2);
    f_c3.open(InputFileC3);
    f_o.open(InputFileO);
    
    Double_t theta_he,theta_li,theta_c,theta_c1,theta_c2,theta_c3,theta_o,err_theta_he,err_theta_li,err_theta_c,err_theta_c1,err_theta_c2,err_theta_c3,err_theta_o = 0;
    string line,dummy;
    Double_t w_li[NRows],err_w_li[NRows],w_c[NRows],err_w_c[NRows],w_c1[NRows],err_w_c1[NRows],w_c2[NRows],err_w_c2[NRows],w_c3[NRows],err_w_c3[NRows],w_o[NRows],err_w_o[NRows] = {0.};
    
    TGraphErrors *gr_c[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_c[i]=new TGraphErrors();
       gr_c[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_c[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_c[i]->SetMarkerSize(1.6);
       gr_c[i]->SetMarkerStyle(53);
       gr_c[i]->SetMarkerColor(kMagenta);
       gr_c[i]->SetLineColor(kMagenta);
    }
    
    TGraphErrors *gr_c1[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_c1[i]=new TGraphErrors();
       gr_c1[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_c1[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_c1[i]->SetMarkerSize(1.6);
       gr_c1[i]->SetMarkerStyle(53);
       gr_c1[i]->SetMarkerColor(kMagenta+1);
       gr_c1[i]->SetLineColor(kMagenta+1);
    }
    TGraphErrors *gr_c2[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_c2[i]=new TGraphErrors();
       gr_c2[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_c2[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_c2[i]->SetMarkerSize(1.6);
       gr_c2[i]->SetMarkerStyle(53);
       gr_c2[i]->SetMarkerColor(kMagenta+2);
       gr_c2[i]->SetLineColor(kMagenta+2);
    }
    TGraphErrors *gr_c3[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_c3[i]=new TGraphErrors();
       gr_c3[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_c3[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_c3[i]->SetMarkerSize(1.6);
       gr_c3[i]->SetMarkerStyle(53);
       gr_c3[i]->SetMarkerColor(kMagenta+3);
       gr_c3[i]->SetLineColor(kMagenta+3);
    }
    TGraphErrors *gr_li[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_li[i]=new TGraphErrors();
       gr_li[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_li[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_li[i]->SetMarkerSize(1.6);
       gr_li[i]->SetMarkerStyle(53);
       gr_li[i]->SetMarkerColor(kYellow+1);
       gr_li[i]->SetLineColor(kYellow+1);
    }
    TGraphErrors *gr_o[5];
    for(int i=0; i<5; i++){
       sprintf(histoname,"row%i",i);
       gr_o[i]=new TGraphErrors();
       gr_o[i]->GetXaxis()->SetTitle("#theta (deg)");
       gr_o[i]->GetYaxis()->SetTitle("#delta_{x}^{tot} (mm)");
       gr_o[i]->SetMarkerSize(1.6);
       gr_o[i]->SetMarkerStyle(53);
       gr_o[i]->SetMarkerColor(kOrange+2);
       gr_o[i]->SetLineColor(kOrange+2);
    }
    
    f_c >> dummy >> theta_c >> dummy >> err_theta_c;
    f_c1 >> dummy >> theta_c1 >> dummy >> err_theta_c1;
    f_c2 >> dummy >> theta_c2 >> dummy >> err_theta_c2;
    f_c3 >> dummy >> theta_c3 >> dummy >> err_theta_c3;
    f_li >> dummy >> theta_li >> dummy >> err_theta_li;
    f_o >> dummy >> theta_o >> dummy >> err_theta_o;

    f_c >> w_c[0] >> w_c[1] >> w_c[2] >> w_c[3] >> w_c[4];
    f_c1 >> w_c1[0] >> w_c1[1] >> w_c1[2] >> w_c1[3] >> w_c1[4];
    f_c2 >> w_c2[0] >> w_c2[1] >> w_c2[2] >> w_c2[3] >> w_c2[4];
    f_c3 >> w_c3[0] >> w_c3[1] >> w_c3[2] >> w_c3[3] >> w_c3[4];
    f_li >> w_li[0] >> w_li[1] >> w_li[2] >> w_li[3] >> w_li[4];
    f_o >> w_o[0] >> w_o[1] >> w_o[2] >> w_o[3] >> w_o[4];

    f_c >> err_w_c[0] >> err_w_c[1] >> err_w_c[2] >> err_w_c[3] >> err_w_c[4];
    f_c1 >> err_w_c1[0] >> err_w_c1[1] >> err_w_c1[2] >> err_w_c1[3] >> err_w_c1[4];
    f_c2 >> err_w_c2[0] >> err_w_c2[1] >> err_w_c2[2] >> err_w_c2[3] >> err_w_c2[4];
    f_c3 >> err_w_c3[0] >> err_w_c3[1] >> err_w_c3[2] >> err_w_c3[3] >> err_w_c3[4];
    f_li >> err_w_li[0] >> err_w_li[1] >> err_w_li[2] >> err_w_li[3] >> err_w_li[4];
    f_o >> err_w_o[0] >> err_w_o[1] >> err_w_o[2] >> err_w_o[3] >> err_w_o[4];
       
    for(Int_t row=0; row<NRows; row++){
       gr_c[row]->SetPoint(0,theta_c,w_c[row]);
       gr_c[row]->SetPointError(0,err_theta_c,err_w_c[row]);
       gr_c1[row]->SetPoint(0,theta_c1,w_c1[row]);
       gr_c1[row]->SetPointError(0,err_theta_c1,err_w_c1[row]);
       gr_c2[row]->SetPoint(0,theta_c2,w_c2[row]);
       gr_c2[row]->SetPointError(0,err_theta_c2,err_w_c2[row]);
       gr_c3[row]->SetPoint(0,theta_c3,w_c3[row]);
       gr_c3[row]->SetPointError(0,err_theta_c3,err_w_c3[row]);
       gr_li[row]->SetPoint(0,theta_li,w_li[row]);
       gr_li[row]->SetPointError(0,err_theta_li,err_w_li[row]);
       gr_o[row]->SetPoint(0,theta_o,w_o[row]);
       gr_o[row]->SetPointError(0,err_theta_o,err_w_o[row]);
    }
    
    
   
//###########################################################################################################################
//# VISUALIZATION BLOCK:

   TLegend *leg = new TLegend(0.1,0.45,0.5,0.9);
   //leg->SetNColumns(2);
   leg->AddEntry(gr1_9mbar[0],"#delta x_{diff} - 9mbar","lp");
   leg->AddEntry(gr1[0],"#delta x_{diff} - 10mbar","lp");
   leg->AddEntry(gr1_11mbar[0],"#delta x_{diff} - 11mbar","lp");
   leg->AddEntry(gr1_geo[0],"#delta x_{geo} ","lp");
   leg->AddEntry(gr1_diff_9mbar[0],"#delta x_{tot} - 9mbar","lp");
   leg->AddEntry(gr1_diff[0],"#delta x_{diff} - 10mbar","lp");
   leg->AddEntry(gr1_diff_11mbar[0],"#delta x_{tot} - 11mbar","lp");
   //leg->AddEntry(gr1he6[0],"#alpha","lp");
   //leg->AddEntry(gr1he1[0],"#alpha - [20#circ-30#circ]","lp");
   //leg->AddEntry(gr1he2[0],"#alpha - [30#circ-40#circ]","lp");
   //leg->AddEntry(gr1he3[0],"#alpha - [40#circ-50#circ]","lp");
   //leg->AddEntry(gr1he4[0],"#alpha - [50#circ-60#circ]","lp");
   leg->AddEntry(gr_li[0],"^{7}Li - 2000 Hz","lp");
   leg->AddEntry(gr_c[0],"^{12}C - 260 Hz","lp");
   leg->AddEntry(gr_c1[0],"^{12}C - 1300 Hz","lp");
   leg->AddEntry(gr_c2[0],"^{12}C - 3000 Hz","lp");
   //leg->AddEntry(gr_c3[0],"^{12}C - 3000 Hz","lp");
   //leg->AddEntry(gr_o[0],"^{16}O - 250 Hz","lp");
   //leg->AddEntry(trackSimuld,"python sim. - gain by anode","p");
   //leg->AddEntry(trackSimulu,"python sim. - gain by anode+top","p");
   //leg->AddEntry(gr_band,"gain ROI (by python sim.)","f");
   
   TText* diffusion = new TText(0.25,21,"Diffusion-dominated");
   diffusion->SetTextSize(0.035);
   diffusion->SetTextFont(112);
   TText* geometry = new TText(48.75,8.45,"Geometry-dominated");
   geometry->SetTextSize(0.035);
   geometry->SetTextFont(112);
   
   TLine* sep_line[3];
   sep_line[0] = new TLine(28.89,0,28.89,55);
   sep_line[1] = new TLine(26.67,0,26.67,55);
   sep_line[2] = new TLine(30.23,0,30.23,55);
   for(Int_t th=0; th<3; th++){
      sep_line[th]->SetLineColor(kGray+2);
      sep_line[th]->SetLineWidth(6);
      sep_line[th]->SetLineStyle(4);
   }
   
   
    TCanvas *c[NRows];
    for(Int_t r=0; r<NRows; r++){
       char tit[4];
       sprintf(tit,"c_%d",r);
       c[r] = new TCanvas(tit,tit,900,800);
       c[r]->SetGridx(1);
       c[r]->SetGridy(1);
       c[r]->SetTickx(1);
       c[r]->SetTicky(1);
       gr1[r]->Draw("APC");
       gr1[r]->GetXaxis()->SetRangeUser(0,80);
       gr1[r]->GetYaxis()->SetRangeUser(0,65);
       gr1_9mbar[r]->Draw("PC SAME");
       gr1_11mbar[r]->Draw("PC SAME");
       gr1_geo[r]->Draw("PC SAME");
       gr1_diff[r]->Draw("PC SAME");
       gr1_diff_9mbar[r]->Draw("PC SAME");
       gr1_diff_11mbar[r]->Draw("PC SAME");
       gr_c[r]->Draw("PC SAME");
       gr_c1[r]->Draw("PC SAME");
       gr_c2[r]->Draw("PC SAME");
       //gr_c3[r]->Draw("PC SAME");
       gr_li[r]->Draw("PC SAME");
       //gr_o[r]->Draw("PC SAME");
       if(r==0 || r==1){
          sep_line[0]->Draw("SAME");
       }else
        if(r==2){
           sep_line[1]->Draw("SAME");
        }else
         if(r==3 || r==4){
            sep_line[2]->Draw("SAME");
         }
       leg->Draw("SAME");
       diffusion->Draw("SAME");
       geometry->Draw("SAME");
       //trackSimuld->Draw("P SAME");
       //trackSimulu->Draw("P SAME");
       //gr_band->Draw("F SAME");
       
       char titlepng[500];
       char titleeps[500];
       sprintf(titlepng,"../../Pictures_Analysis/Study on the diffusion/DEF_DS_Assran_Ferbel_SIM_NEW_ChargeDistributionWwidth_FWAB_Row%d.png",r);
       sprintf(titleeps,"../../Pictures_Analysis/Study on the diffusion/DEF_DS_Assran_Ferbel_SIM_NEW_ChargeDistributionWidth_FWAB_Row%d.eps",r);
       c[r]->SaveAs(titlepng);
       c[r]->SaveAs(titleeps);
    }
    
}
