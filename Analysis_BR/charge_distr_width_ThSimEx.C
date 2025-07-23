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

const TString InputFileC = "../../../OutputFiles/Run52.txt";
const TString InputFileLi = "../../../OutputFiles/Run298Li.txt";
const TString InputFileHe = "../../../OutputFiles/Run298Alpha.txt";
const TString InputFileO = "../../../OutputFiles/Run171.txt";
const TString InputFileC1 = "../../../OutputFiles/Run52.txt";
const TString InputFileC2 = "../../../OutputFiles/Run65.txt";
const TString InputFileC3 = "../../../OutputFiles/Run76.txt";


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
       gr1[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_9mbar[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_11mbar[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_geo[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_diff[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_diff_9mbar[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
       gr1_diff_11mbar[r]->SetTitle("Charge distribution width ~ M-THGEM without RIM");
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
    Double_t thetaSimul[9] = {0.};
    Double_t xSimuld0[9] = {0.};
    Double_t xSimulu0[9] = {0.};
    ifstream sim;
    string badlines;
    
    sim.open(InputSimul);
    
    /* Reading-loop */
    for(Int_t l=0; l<6; l++){
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
    trackSimuld->SetTitle("Charge distribution width ~ M-THGEM without RIM");
    trackSimuld->SetLineWidth(3);
    trackSimuld->SetLineStyle(8);
    trackSimuld->SetLineColor(kRed-7);
    trackSimuld->SetMarkerStyle(26);
    trackSimuld->SetMarkerColor(kRed-7);
    trackSimuld->SetMarkerSize(1.5);
   
    
    TGraph *trackSimulu = new TGraph(9,thetaSimul,xSimulu);
    trackSimulu->SetTitle("Charge distribution width ~ M-THGEM without RIM");
    trackSimulu->SetLineWidth(3);
    trackSimulu->SetLineStyle(8);
    trackSimulu->SetLineColor(kRed-7);
    trackSimulu->SetMarkerStyle(23);
    trackSimulu->SetMarkerColor(kRed-7);
    trackSimulu->SetMarkerSize(1.5);
   
    TGraph *gr_band = new TGraph(18);
    for(Int_t p=0; p<9; p++){
       gr_band->SetPoint(p,thetaSimul[p],xSimuld[p]);
       gr_band->SetPoint(17-p,thetaSimul[p],xSimulu[p]);
    }
    gr_band->SetFillColorAlpha(kRed-7,0.2);
    gr_band->SetLineColor(kRed-7);
    gr_band->SetFillStyle(3001);
    
//###########################################################################################################################
//# EXPERIMENTAL BLOCK:    
    
    
//###########################################################################################################################
//# VISUALIZATION BLOCK:
    for(Int_t r=0; r<NRows; r++){
       TCanvas *c = new TCanvas("c","",900,800);
       c->SetGridx(1);
       c->SetGridy(1);
       c->SetTickx(1);
       c->SetTicky(1);
       c->cd();
       gr1[r]->Draw("APC");
       gr1[r]->GetXaxis()->SetRangeUser(0,80);
       gr1[r]->GetYaxis()->SetRangeUser(0,65);
       gr1_9mbar[r]->Draw("PC SAME");
       gr1_11mbar[r]->Draw("PC SAME");
       gr1_geo[r]->Draw("PC SAME");
       gr1_diff[r]->Draw("PC SAME");
       gr1_diff_9mbar[r]->Draw("PC SAME");
       gr1_diff_11mbar[r]->Draw("PC SAME");
       trackSimuld->Draw("P SAME");
       trackSimulu->Draw("P SAME");
       gr_band->Draw("F SAME");
    }
}
