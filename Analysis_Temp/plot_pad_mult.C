//#####################################################################################################################

//Macro that read the information from a third level root file and plots the pad multiplicity for each row//
//To run simply open a root session and type: .x plot_pad_mult.C(run number) //

//Created on 15 November 2015 by O. Sgouros//

//#####################################################################################################################



#include <string.h>


void plot_pad_mult(int run)
{

cout<<""<<endl;
cout<<""<<endl;


// Input file variables (tracker)

   double cl_charge[11];	 // Charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5];		// Number of pads of a cluster
   double cl_x[5];			// X centroid of a cluster in pads unit
   double cl_x_mm[5];			// Y centroid of a cluster in mm
   double cl_x_rms[5];  		// RMS of the charge distribution of a cluster in pads unit
   double cl_y[5] = {0};		// Y centroid of a cluster in time
   double cl_y_mm[5] = {0};		// Y centroid of a cluster in mm
   int a_pads_fired[5][100];
   
// Input file variables (SiC)
   int sic_fired;
   double energySic;
   


//////////////////////////////////////////////////////////////////////////////

// Open file
   char fileIn[50];

   if(run<10){
   sprintf(fileIn, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i.root", run);
   } 

   cout<<fileIn<<endl;
   cout<<""<<endl;
   cout<<""<<endl;

   TFile *f = new TFile(fileIn);
   TTree *tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("cl_padMult0",&cl_padMult[0]);
   tree->SetBranchAddress("cl_padMult1",&cl_padMult[1]);
   tree->SetBranchAddress("cl_padMult2",&cl_padMult[2]);
   tree->SetBranchAddress("cl_padMult3",&cl_padMult[3]);
   tree->SetBranchAddress("cl_padMult4",&cl_padMult[4]);
   tree->SetBranchAddress("cl_charge",&cl_charge);
   tree->SetBranchAddress("pads_fired0",&a_pads_fired[0]);
   tree->SetBranchAddress("pads_fired1",&a_pads_fired[1]);
   tree->SetBranchAddress("pads_fired2",&a_pads_fired[2]);
   tree->SetBranchAddress("pads_fired3",&a_pads_fired[3]);
   tree->SetBranchAddress("pads_fired4",&a_pads_fired[4]);
   
   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("energySic",&energySic);

   int entries=tree->GetEntries();
   cout<<"Entries in file "<< entries <<endl;
   cout<<""<<endl;
   cout<<""<<endl;

// END: Open file 

//////////////////////////////////////////////////////////////////////////////



// Declaring Histos 

   TH1D *pad_mult[5]; //Histograms fo ploting the the pad multiplicity in the 5 rows
   TString hist_name;

   for(int i=0;i<5;i++) {
   
   hist_name.Form("Pad multiplicity row%d",i);
   pad_mult[i] = new TH1D(hist_name,"",100,0.0,20.0);
   
   pad_mult[i]->GetXaxis()->SetTitle("Pad multiplicity");
   pad_mult[i]->GetXaxis()->SetTitleOffset(1.10);
   pad_mult[i]->GetXaxis()->SetTitleSize(0.040);
   pad_mult[i]->GetXaxis()->SetTitleFont(62);
   pad_mult[i]->GetXaxis()->SetLabelFont(62);
   pad_mult[i]->GetXaxis()->SetLabelOffset(0.008);
   pad_mult[i]->GetXaxis()->SetLabelSize(0.030);
   pad_mult[i]->GetYaxis()->SetNdivisions(505);
   pad_mult[i]->GetXaxis()->CenterTitle();

   pad_mult[i]->GetYaxis()->SetTitle("N (Counts)");
   pad_mult[i]->GetYaxis()->SetTitleOffset(1.25);
   pad_mult[i]->GetYaxis()->SetTitleSize(0.040);
   pad_mult[i]->GetYaxis()->SetTitleFont(62);
   pad_mult[i]->GetYaxis()->SetLabelFont(62);
   pad_mult[i]->GetYaxis()->SetLabelOffset(0.008);
   pad_mult[i]->GetYaxis()->SetLabelSize(0.030);
   pad_mult[i]->GetYaxis()->SetNdivisions(505);
   pad_mult[i]->GetYaxis()->CenterTitle();
   
   }
   


   TCanvas *c0=new TCanvas("","Multiplicity row 0",800,500,1000,800);
   TCanvas *c1=new TCanvas("","Multiplicity row 1",800,500,1000,800);
   TCanvas *c2=new TCanvas("","Multiplicity row 2",800,500,1000,800);
   TCanvas *c3=new TCanvas("","Multiplicity row 3",800,500,1000,800);
   TCanvas *c4=new TCanvas("","Multiplicity row 4",800,500,1000,800);

   
   for(int j=0; j<entries; j++){
   tree->GetEntry(j);



   for(int k=0; k<5; k++){
   pad_mult[k]->Fill(cl_padMult[k],1);
      }
      
  }     


   
   c0->cd();
   pad_mult[0]->Draw();
   
   c1->cd();
   pad_mult[1]->Draw();

   c2->cd();
   pad_mult[2]->Draw();

   c3->cd();
   pad_mult[3]->Draw();
   
   c4->cd();
   pad_mult[4]->Draw();


 }

