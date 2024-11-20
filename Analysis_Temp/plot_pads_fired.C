//################################################################################################################################################

//Macro that reads the information from a third level root file and calculates the horizontal position resolution (Xres) of the tracker//
//For the determination of Xres, three different methods were adopted:

// i)   Determine the intersection point of the ions trajectories with a plane parallel to the tracker's front face.
// ii)  Rotate the reference frame of the tracker by an angle "theta_tilt", calculate the slope and intercept of the trajectories in the new reference frame and subsequently, 
//      determine the interection point between the "rotated" ions trajectories and a plane paraller to the collimator's front face. 
//iii)  Determine the interection point between the ions tracjectories and a line parallel to the collimators front face//

//In every scenario mentioned above, the distance of the plane/line form the tracker is determined through a grid search//
//The deduced resolutions adopting each one of the methods mentioned above are then saved in a .txt file named: resolution.txt// 

//Input variables: Introduce in line 86 the collimator's(or beam spot) size and the angle between the collimator(beam) and the tracker in line 87//  

//To run simply open a root session and type: .x resolution.C(run number,z), where "z" is the distance (in mm) between the plane/line and the tracker //

//Created in October 2024 by O. Sgouros//

//################################################################################################################################################



#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

#include <fstream>
#include <sstream>
#include <string>


using namespace std;



int plot_pads_fired(int run)
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
   
   double slopeT;		// Slope of the linear fit X= f(Z)
   double interceptT;		// Intercept of the linear fit X= f(Z) (in mm)
   double slopeP;		// Slope of the linear fit Y= f(Z)
   double interceptP;		// Intercept of the linear fit Y= f(Z) (in mm)
   double theta;		// Theta of the track in rad
   double theta_deg;		// Theta of the track in deg
   double phi;			// Phi of the track in rad
   double phi_deg;		// Phi of the track in deg
   double chiSquareTheta;
   double chiSquarePhi;
   int sic_fired;
   double energySic;
   

//////////////////////////////////////////////////////////////////////////////

// Open file
   char fileIn[50];

   if(run<10){
   sprintf(fileIn, "../Tracks/tracks_run00%i_rowmult.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i_rowmult.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i_rowmult.root", run);
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
   
   tree->SetBranchAddress("theta",&theta);
   tree->SetBranchAddress("phi",&phi);
   tree->SetBranchAddress("slopeT",&slopeT);
   tree->SetBranchAddress("interceptT",&interceptT);
   tree->SetBranchAddress("slopeP",&slopeP);
   tree->SetBranchAddress("interceptP",&interceptP);
   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("energySic",&energySic);

   int entries=tree->GetEntries();
   cout<<"Entries in file "<< entries <<endl;
   cout<<""<<endl;
   cout<<""<<endl;

// END: Open file 

//////////////////////////////////////////////////////////////////////////////



  
   int flagM=0;
   char anykey;


   TCanvas *c1=new TCanvas("c1","c1",800,500,1000,800);

   f->cd();
   
   for(int i=0; i<entries; i++){
   tree->GetEntry(i);
   
   TH1F *h1 = new TH1F("h1","",120,-0.50,59.50);
   
   h1->GetXaxis()->SetTitle("Pad number");
   h1->GetXaxis()->SetTitleOffset(1.05);
   h1->GetXaxis()->SetTitleSize(0.040);
   h1->GetXaxis()->SetTitleFont(62);
   h1->GetXaxis()->SetLabelFont(62);
   h1->GetXaxis()->SetLabelOffset(0.008);
   h1->GetXaxis()->SetLabelSize(0.030);
   h1->GetYaxis()->SetNdivisions(505);
   h1->GetXaxis()->CenterTitle();

   h1->GetYaxis()->SetTitle("N (Counts)");
   h1->GetYaxis()->SetTitleOffset(1.10);
   h1->GetYaxis()->SetTitleSize(0.040);
   h1->GetYaxis()->SetTitleFont(62);
   h1->GetYaxis()->SetLabelFont(62);
   h1->GetYaxis()->SetLabelOffset(0.008);
   h1->GetYaxis()->SetLabelSize(0.030);
   h1->GetYaxis()->SetNdivisions(505);
   h1->GetYaxis()->CenterTitle();

   
    for(int j=0; j<cl_padMult[0]; j++){ 
   
   h1->Fill(a_pads_fired[0][j],1);
   
   }
   
   c1->cd();
   h1->Draw();
   c1->Update();
   
   cout<<"entry "<<i<<endl;
   cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl;
   if(flagM==0)cin>>anykey;
   if(anykey=='q')return 0; // Per uscire dal programma
   
   if(anykey=='c')flagM=1;

   
  	
   }
    
return 0;
   
}
