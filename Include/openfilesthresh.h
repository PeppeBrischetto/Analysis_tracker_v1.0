#ifndef OPENFILESTHRESH_H
#define OPENFILESTHRESH_H

//#################################################################################################
//  OPENING Tracks file
//#################################################################################################

// file variables for tracks file
   Double_t cl_charge[11];	   	// charge sum of the pads belonging to a cluster
   Int_t pad_charge[5][60];                // 2025.02.14 - by A. Pitronaci: charge info for signle Pad in each row
   Int_t cl_padMult[5];		// number of pads of a cluster
   Double_t cl_x[5];			// x centroid of a cluster in pads unit
   Double_t cl_x_mm[5];			// x centroid of a cluster in mm
   Double_t cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   Double_t cl_y[5];		// y centroid of a cluster in time
   Double_t cl_y_mm[5];		// y centroid of a cluster in mm
   Double_t theta;		// theta of the track in rad
   Double_t theta_deg;		// theta of the track in deg
   Double_t theta123;		// theta of the track in rad
   Double_t theta123_deg;		// theta of the track in deg
   Double_t theta13;		// theta of the track in rad
   Double_t theta13_deg;		// theta of the track in deg
   Double_t theta04;		// theta of the track in rad
   Double_t theta04_deg;		// theta of the track in deg         
   Double_t theta024;		// theta of the track in rad
   Double_t theta024_deg;		// theta of the track in deg     
   Double_t phi;
   Double_t phi_deg;
   Double_t chiSquareTheta;
   Double_t chiSquarePhi;   
   Int_t sic_charge;
   Double_t energySic; 
   Int_t pads_fired[5][100];
   Double_t slopeT, interceptT;
   Double_t slopeP, interceptP;
   Int_t sic_fired;
   Int_t entryMF;
   
   Int_t entries;
   
   TFile *f;
   TTree *tree; 
   
// input file variables for tracks file   


void openTrackFileTh(int runnum ){
   //open file
   char *filename = new char[100];
   if(runnum<10){
      sprintf(filename, "../Tracks_Br/tracks_run00%i_th.root", runnum);
   }else if(runnum <100){
      sprintf(filename, "../Tracks_Br/tracks_run0%i_th.root", runnum);
   }else{
      //sprintf(filename, "../Tracks_Br/tracks_run%i.root", runnum);
      sprintf(filename, "../Tracks_Br/tracks_run%i_th.root", runnum);
   } 

   f=new TFile(filename,"READ");
   cout<<"Opening file:  "<<filename<<endl;
   cout<<"Is the file "<<filename<<" a Zomby? "<< f->IsZombie()<<endl;
   //open the tree
   if(f->IsZombie())cout<<"\n\n\t\t\033[40;31;1m No file found!\033[0m\n\n"<<endl;
   tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("entryMF", &entryMF);
   tree->SetBranchAddress("cl_x",&cl_x);
   tree->SetBranchAddress("cl_x_mm",&cl_x_mm);
   tree->SetBranchAddress("cl_y",&cl_y);
   tree->SetBranchAddress("cl_y_mm",&cl_y_mm);
   tree->SetBranchAddress("cl_x_rms",&cl_x_rms);
   tree->SetBranchAddress("cl_charge",&cl_charge);
   tree->SetBranchAddress("pad_charge0",&pad_charge[0]);     // 2025.14.02 - by A. Pitronaci
   tree->SetBranchAddress("pad_charge1",&pad_charge[1]);
   tree->SetBranchAddress("pad_charge2",&pad_charge[2]);
   tree->SetBranchAddress("pad_charge3",&pad_charge[3]);
   tree->SetBranchAddress("pad_charge4",&pad_charge[4]);
   tree->SetBranchAddress("cl_padMult0",&cl_padMult[0]);
   tree->SetBranchAddress("cl_padMult1",&cl_padMult[1]);
   tree->SetBranchAddress("cl_padMult2",&cl_padMult[2]);
   tree->SetBranchAddress("cl_padMult3",&cl_padMult[3]);
   tree->SetBranchAddress("cl_padMult4",&cl_padMult[4]);
   tree->SetBranchAddress("pads_fired0",&pads_fired[0]);
   tree->SetBranchAddress("pads_fired1",&pads_fired[1]);
   tree->SetBranchAddress("pads_fired2",&pads_fired[2]);
   tree->SetBranchAddress("pads_fired3",&pads_fired[3]);
   tree->SetBranchAddress("pads_fired4",&pads_fired[4]);
   
   tree->SetBranchAddress("theta",&theta);
   tree->SetBranchAddress("phi",&phi);
   tree->SetBranchAddress("theta_deg",&theta_deg);
   tree->SetBranchAddress("phi_deg",&phi_deg);
   tree->SetBranchAddress("chiSquareTheta",&chiSquareTheta);   
   tree->SetBranchAddress("chiSquarePhi",&chiSquarePhi);      
   tree->SetBranchAddress("interceptT",&interceptT);
   tree->SetBranchAddress("slopeT",&slopeT);
   tree->SetBranchAddress("interceptP",&interceptP);
   tree->SetBranchAddress("slopeP",&slopeP);
   
   tree->SetBranchAddress("theta123",&theta123);
   tree->SetBranchAddress("theta123_deg",&theta123_deg);
   tree->SetBranchAddress("theta13",&theta13);
   tree->SetBranchAddress("theta13_deg",&theta13_deg);   
   tree->SetBranchAddress("theta04",&theta04);
   tree->SetBranchAddress("theta04_deg",&theta04_deg);
   tree->SetBranchAddress("theta024",&theta024);
   tree->SetBranchAddress("theta024_deg",&theta024_deg);
   tree->SetBranchAddress("theta024",&theta024);
   tree->SetBranchAddress("theta024_deg",&theta024_deg);
   
   // Sic variables
   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("sic_charge",&sic_charge);
   tree->SetBranchAddress("energySic",&energySic);

   entries=tree->GetEntries();
   
   cout<<"Total entries: "<<entries<<endl;
   
}
   
   
   
   
   
   
#endif
