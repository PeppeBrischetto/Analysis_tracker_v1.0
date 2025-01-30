//################################################################################################################################################

//Macro that reads the information from a third level root file and calculates the vertical position resolution (Yres) of the tracker//
//For the determination of Yres we considered the intersection point of the ions trajectories with a plane parallel to the tracker's front face.//

//Input variables: Introduce in line 56 the collimator's(or beam spot) size//  

//To run simply open a root session and type: .x C_Y_resolution.C(run number)//

//Created in December 2024 by O. Sgouros//

//################################################################################################################################################



#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

#include <fstream>
#include <sstream>
#include <string>





void C_Y_resolution(int run)
{

cout<<""<<endl;
cout<<""<<endl;

   const double PI = 3.14159;   //Definition of Pi

// Input file variables (tracker)

   double cl_charge[11];	 // Charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5];		// Number of pads of a cluster
   double cl_y[5] = {0};	// Y centroid of a cluster in time
   double cl_y_mm[5] = {0};	// Y centroid of a cluster in mm
   int a_pads_fired[5][100];
   
   double slopeP;		// Slope of the linear fit Y= f(Z)
   double interceptP;		// Intercept of the linear fit Y= f(Z) (in mm)
   double phi;			// Phi of the track in rad
   double chiSquarePhi;
   int sic_fired;
   double energySic;
   



// Geometric variables required for positioning the collimator respect to the detector

   double coll_size=0.20;	       // Dimension of the collimator front face (in mm)
 
   

// Geometric variables required for the definition of a plane z'= const. parallel to the tracker

   double z0=-2.0; 			// z coordinate of the plane (in mm) - (i.e. its distance from the tracker)




// Variables for the estimation of the position resolution

   double sigmaDistColly;	// Sigma of the distribution of the reconstructed Y coordinate of the collimator (in mm) in a plane paraller to the tracker
   double fwhmy;		// FWHM of the distribution of the reconstructed Y coordinate of the collimator (in mm) in a plane paraller to the tracker
   double resy;			// Resolution in the determination of the collimator apperture (in mm) in a plane paraller to the tracker
   



// other variables
   
   double slopeP_inv; 		//Slope of the linear fit Z=f(Y) describing the ion track
   double interceptP_inv; 	//Intercept of the linear fit Z=f(Y) describing the ion track (in mm)
   double ycol; 		//Reconstructed vertical position of the collimator in a plane parallel to the tracker (in mm)
   
   int binmax; 			// Find the bin with the most statistics
   double xmax; 		// X position of the bin with the most statistics
   double ymax; 		// Y position of the bin with the most statistics
   
   double totalCharge=0.0;
   int qualityFlag=0;
   

   TFitResultPtr fitResY;

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
   
   tree->Branch("cl_y", cl_y, "cl_y[5]/D");
   tree->Branch("cl_y_mm", cl_y_mm, "cl_y_mm[5]/D");
   tree->SetBranchAddress("phi",&phi);
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



// Declaring Histos 

   TH1F *ply = new TH1F("ply","",200,-10.0,10.0);  //Histogram with the reconstructed Y coordinate of the colimator
   
   ply->GetXaxis()->SetTitle("Y_{col.} (mm)");
   ply->GetXaxis()->SetTitleOffset(1.05);
   ply->GetXaxis()->SetTitleSize(0.040);
   ply->GetXaxis()->SetTitleFont(62);
   ply->GetXaxis()->SetLabelFont(62);
   ply->GetXaxis()->SetLabelOffset(0.008);
   ply->GetXaxis()->SetLabelSize(0.030);
   ply->GetYaxis()->SetNdivisions(505);
   ply->GetXaxis()->CenterTitle();

   ply->GetYaxis()->SetTitle("N (Counts)");
   ply->GetYaxis()->SetTitleOffset(1.10);
   ply->GetYaxis()->SetTitleSize(0.040);
   ply->GetYaxis()->SetTitleFont(62);
   ply->GetYaxis()->SetLabelFont(62);
   ply->GetYaxis()->SetLabelOffset(0.008);
   ply->GetYaxis()->SetLabelSize(0.030);
   ply->GetYaxis()->SetNdivisions(505);
   ply->GetYaxis()->CenterTitle();



   TCanvas *colplaneY=new TCanvas("colplaneY","colplaneY",800,500,1000,800);
   
   
   TH2F *bg=new TH2F("bg","",100,-20,100,100,-10,10);
   colplaneY->cd();
   bg->Draw();
   
   TF1 *f1 = new TF1("fit","[0]+([1]*x)",-20,100);  //Function for ploting the reconstructed ion tracks
   TF1 *plot;		// Function in order to plot all the tracks in the same graph

   f->cd();
   
   totalCharge=0;
   
   for(int i=0; i<entries; i++){
   tree->GetEntry(i);
   
   qualityFlag=0;
   totalCharge=0;

   //Reject tracks that do not have a hit on SiC 
   if(energySic<0.0){
   qualityFlag=1;//This event is marked as bad and will not be processed//
  }
	
   //Reject tracks with first 2 or last 3 pads of the last row hit
   for(int j=0; j<cl_padMult[4]; j++){ 
   
   if((a_pads_fired[4][j] <= 2) || (a_pads_fired[4][j] >= 57)){
   qualityFlag=1; //This event is marked as bad and will not be processed//
   
   }
   	}
   	
   	
   // Selection on total charge on the last row. Calculate the average charge of the first 4 rows and if the charge of the last 
   // row is smaller or larger of 50% of the average charge of the first rows the track is removed	
   for(int k=0; k<4; k++){
   totalCharge=totalCharge+cl_charge[k];
      }
      
   if(abs(totalCharge/4-cl_charge[4])>(totalCharge/8)){
   qualityFlag=1;//This event is marked as bad and will not be processed//
  }     

   	
   if(qualityFlag==0){   

   //slopeT_inv=atan((-0.5*PI)-theta);
   slopeP_inv=(1./slopeP); //Retrieve the slope of the lines describing ion tracks from the inverse function
   interceptP_inv=-(interceptP/slopeP); //Retrieve the intercept of the lines describing ion tracks from the inverse function
	
  //printf("Z0: %1.2f,  slope: %1.4f,  intercept: %1.4f \n",z0,slopeP_inv,interceptP_inv);
   ycol=(z0-interceptP_inv)/slopeP_inv; //Reconstructed vertical position of the collimator in a plane paraller to the tracker
   ply->Fill(ycol,1);

	
   /*  
   colplaneY->Update();
   colplaneY->cd();
   plot=new TF1("fit","[0]+([1]*x)",-2,100);
   plot->SetParameter(0,interceptP_inv);
   plot->SetParameter(1,slopeP_inv);
   plot->SetLineColor(kGreen+1);
   plot->Draw("same");
   colplaneY->Update();*/
   //gPad->WaitPrimitive();
   
   }
   	}

   		
// Calculate the vertical position resolution in a plane parallel to the tracker

   colplaneY->cd();
   //ply->Draw();
   
   binmax = ply->GetMaximumBin(); 
   xmax = ply->GetXaxis()->GetBinCenter(binmax);
   ply->GetXaxis()->SetRangeUser(xmax-4,xmax+4);


   fitResY=ply->Fit("gaus","S");
   sigmaDistColly=fitResY->Value(2);
   fwhmy=(2.35*sigmaDistColly);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resy=sqrt(abs(pow(fwhmy,2.)-pow(coll_size,2.)));
 
 
   cout<<"Y position resolution: "<< resy <<" mm"<<endl;
   
   cout<<""<<endl;
   cout<<""<<endl;

   
}

