//################################################################################################################################################

//Macro that reads the information from a third level root file and calculates the horizontal(Xres) and vertical(Yres) position resolution of the tracker//
//For the determination of Xres, three different methods were adopted:

// i)   Determine the intersection point of the ions trajectories with a plane parallel to the tracker's front face.
// ii)  Rotate the reference frame of the tracker by an angle "theta_tilt", calculate the slope and intercept of the trajectories in the new reference frame and subsequently, 
//      determine the interection point between the "rotated" ions trajectories and a plane paraller to the collimator's front face. 
//iii)  Determine the interection point between the ions tracjectories and a line parallel to the collimators front face//

//In every scenario mentioned above, the distance of the plane/line form the tracker is determined through a grid search//
//The deduced resolutions adopting each one of the methods mentioned above are then saved in a .txt file named: resolution.txt//

//For the determination of Yres, method (i) was adopted (see description above)//

//Input variables: Introduce in line 88 the collimator's(or beam spot) size and the angle between the collimator(beam) and the tracker in line 89//  

//To run simply open a root session and type: .x XY_resolution.C(run number,z), where "z" is the distance (in mm) between the plane/line and the tracker //

//Created in October 2024 by O. Sgouros//

//################################################################################################################################################



#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

#include <fstream>
#include <sstream>
#include <string>


using namespace std;

//Function "rot_tracks" uses the equation of the ion track in a reference system rotated by "theta_tilt" with respect to the tracker reference frame and provides as an output//
//the horizontal position at the interection point between each track and a plane paraller to the collimator which is found at distance z0 from the tracker front face.
double rot_tracks(double theta_tilt,double slopeT_inv,double interceptT_inv,double z0) {


   const double PI = 3.14159; //Definition of Pi
   double factor=theta_tilt*PI/180;
   double result= (z0 - (interceptT_inv/(cos(factor)-(slopeT_inv*sin(factor))))) * (cos(factor)-(slopeT_inv*sin(factor))) / ((slopeT_inv*cos(factor))+sin(factor));
   return result;

}



void XY_resolution(int run, double z)
{

cout<<""<<endl;
cout<<""<<endl;

   const double PI = 3.14159;   //Definition of Pi

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
   



// Geometric variables required for positioning the collimator respect to the detector

   double coll_size=0.20;	       // Dimension of the collimator front face (in mm)
   double theta_tilt=55.0;	       // Rotation angle of the collimator with respect to the beam axis (in mm)
   
   
// Geometric variables required for the definition of a line parallel to the collimator front face

   double ax=150.0;			  // x coordinate of the point A(ax,az) (in mm) belonging to a line parallel to the collimator's front face
   double az;				  // z coordinate of the point A(ax,az) (in mm) belonging to a line parallel to the collimator's front face - To be scanned
   
   double bx;				  // x coordinate (in mm) of the intersection point B(bx,bz) between each track and a line parallel to the collimator's front face
   double bz;				  // z coordinate (in mm) of the intersection point B(bx,bz) between each track and a line parallel to the collimator's front face

   double ang_coeff_line=tan(-theta_tilt*PI/180); // Angular coefficient of a line forming an angle "-theta_tilt" with respect to the X axis of the tracker
   double intercept_line; 			// Intercept of a line forming an angle "-theta_tilt" with respect to the X axis of the tracker


// Geometric variables required for the definition of a plane z'= const. parallel to the tracker

   double z0; 			// z coordinate of the plane (in mm) - (i.e. its distance from the tracker)




// Variables for the estimation of the position resolution

   double sigmaDistCollX;	// Sigma of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the tracker
   double fwhmX;		// FWHM of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the tracker
   double resX;			// Resolution in the determination of the collimator apperture (in mm) in a plane paraller to the tracker
   
   double sigmaDistCollX2;	// Sigma of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the collimator - Method with tracks' rotation
   double fwhmX2;		// FWHM of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the collimator - Method with tracks' rotation
   double resX2;		// Resolution in the determination of the collimator apperture (in mm) in a plane paraller to the collimator - Method with tracks' rotation
   
   double sigmaDistCollX3;	// Sigma of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the collimator - Method with intersection of lines
   double fwhmX3;		// FWHM of the distribution of the reconstructed X coordinate of the collimator (in mm) in a plane paraller to the collimator - Method with intersection of lines
   double resX3;		// Resolution in the determination of the collimator apperture (in mm) in a plane paraller to the collimator - Method with intersection of lines

   
   double sigmaDistCollY;	// Sigma of the distribution of the reconstructed Y coordinate of the collimator (in mm) in a plane paraller to tracker
   double fwhmY;		// FWHM of the distribution of the reconstructed Y coordinate of the collimator (in mm) in a plane paraller to tracker
   double resY;			// Resolution in the determination of the collimator apperture (in mm) in a plane paraller to tracker



// other variables
   double slopeT_inv; 		//Slope of the linear fit Z=f(X) describing the ion track
   double interceptT_inv; 	//Intercept of the linear fit Z=f(X) describing the ion track (in mm)
   double xcol; 		//Reconstructed X position of the collimator in a plane parallel to the tracker (in mm)
   double xcol_rot; 		//Reconstructed X position of the collimator in a plane parallel to the collimator (in mm) - Method with tracks' rotation
   double xcol3; 		//Reconstructed X position of the collimator in a plane parallel to the collimator (in mm) - Method with intersection of lines

   
   double slopeP_inv; 		//Slope of the linear fit Z=f(Y) describing the ion track
   double interceptP_inv; 	//Intercept of the linear fit Z=f(Y) describing the ion track (in mm)
   double ycol; 		//Reconstructed Y position of the collimator in a plane parallel to the tracker (in mm)
   
   int binmax; 			// Find the bin with the most statistics
   double xmax; 		// X position of the bin with the most statistics
   double ymax; 		// Y position of the bin with the most statistics
   
   double totalCharge=0.0;
   int qualityFlag=0;
   
   string filename = "resolution.txt";  //File to store the values of Z0 and horizontal position resolution
   int lineCount = 0;			//Variable to store the lines' number of the file "resolution.txt"
   double temp1[10000], temp2[10000], temp3[10000], temp4[10000], temp5[10000], temp6[10000], temp7[10000];  //Matrices to store the values of Z0 and X, Y resolutions from the file resolution.txt, before passing them to resolution2.txt

   TFitResultPtr fitResX;
   TFitResultPtr fitResX2;
   TFitResultPtr fitResX3;
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



// Declaring Histos 

   TH1F *plx = new TH1F("plx","",20000,-300.0,500.0);  //Histogram with the reconstructed X coordinate of the colimator in a plane parallel to the tracker
   TH1F *plx2 = new TH1F("plx2","",20000,-300.0,500.0);//Histogram with the reconstructed X coordinate of the colimator in a plane paraller to the collimator - Method with tracks' rotation
   TH1F *plx3 = new TH1F("plx3","",20000,-300.0,500.0);//Histogram with the reconstructed X coordinate of the colimator in a plane paraller to the collimator - Method with intersectionof lines
   TH1F *ply = new TH1F("ply","",3000,-50.0,250.0);  //Histogram with the reconstructed Y coordinate of the colimator
   
  TH2D *spot = new TH2D("spot","",20000,-300.0,500.0,600,-50.0,250.0);  //Histogram with the reconstructed beam profile//
   
   
   
   plx->GetXaxis()->SetTitle("X_{col.} (mm)");
   plx->GetXaxis()->SetTitleOffset(1.05);
   plx->GetXaxis()->SetTitleSize(0.040);
   plx->GetXaxis()->SetTitleFont(62);
   plx->GetXaxis()->SetLabelFont(62);
   plx->GetXaxis()->SetLabelOffset(0.008);
   plx->GetXaxis()->SetLabelSize(0.030);
   plx->GetYaxis()->SetNdivisions(505);
   plx->GetXaxis()->CenterTitle();

   plx->GetYaxis()->SetTitle("N (Counts)");
   plx->GetYaxis()->SetTitleOffset(1.10);
   plx->GetYaxis()->SetTitleSize(0.040);
   plx->GetYaxis()->SetTitleFont(62);
   plx->GetYaxis()->SetLabelFont(62);
   plx->GetYaxis()->SetLabelOffset(0.008);
   plx->GetYaxis()->SetLabelSize(0.030);
   plx->GetYaxis()->SetNdivisions(505);
   plx->GetYaxis()->CenterTitle();
   
   plx2->GetXaxis()->SetTitle("X_{col.2} (mm)");
   plx2->GetXaxis()->SetTitleOffset(1.05);
   plx2->GetXaxis()->SetTitleSize(0.040);
   plx2->GetXaxis()->SetTitleFont(62);
   plx2->GetXaxis()->SetLabelFont(62);
   plx2->GetXaxis()->SetLabelOffset(0.008);
   plx2->GetXaxis()->SetLabelSize(0.030);
   plx2->GetYaxis()->SetNdivisions(505);
   plx2->GetXaxis()->CenterTitle();

   plx2->GetYaxis()->SetTitle("N (Counts)");
   plx2->GetYaxis()->SetTitleOffset(1.10);
   plx2->GetYaxis()->SetTitleSize(0.040);
   plx2->GetYaxis()->SetTitleFont(62);
   plx2->GetYaxis()->SetLabelFont(62);
   plx2->GetYaxis()->SetLabelOffset(0.008);
   plx2->GetYaxis()->SetLabelSize(0.030);
   plx2->GetYaxis()->SetNdivisions(505);
   plx2->GetYaxis()->CenterTitle();
   
   
   plx3->GetXaxis()->SetTitle("X_{col.3} (mm)");
   plx3->GetXaxis()->SetTitleOffset(1.05);
   plx3->GetXaxis()->SetTitleSize(0.040);
   plx3->GetXaxis()->SetTitleFont(62);
   plx3->GetXaxis()->SetLabelFont(62);
   plx3->GetXaxis()->SetLabelOffset(0.008);
   plx3->GetXaxis()->SetLabelSize(0.030);
   plx3->GetYaxis()->SetNdivisions(505);
   plx3->GetXaxis()->CenterTitle();

   plx3->GetYaxis()->SetTitle("N (Counts)");
   plx3->GetYaxis()->SetTitleOffset(1.10);
   plx3->GetYaxis()->SetTitleSize(0.040);
   plx3->GetYaxis()->SetTitleFont(62);
   plx3->GetYaxis()->SetLabelFont(62);
   plx3->GetYaxis()->SetLabelOffset(0.008);
   plx3->GetYaxis()->SetLabelSize(0.030);
   plx3->GetYaxis()->SetNdivisions(505);
   plx3->GetYaxis()->CenterTitle();


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



   //TCanvas *colplaneX=new TCanvas("colplaneX","colplaneX",800,500,600,600);
   //TCanvas *colplaneX2=new TCanvas("colplaneX2","colplaneX2",800,500,600,600);
   TCanvas *colplaneX3=new TCanvas("colplaneX3","colplaneX3",10,450,600,600);
   TCanvas *colplaneY=new TCanvas("colplaneY","colplaneY",690,450,600,600);
   TCanvas *beam=new TCanvas("beam","beam",1300,450,600,600);
   
 /*  TH2F *bg=new TH2F("bg","",100,0,300,100,-100,400);
   colplaneY->cd();
   bg->Draw();*/
   
   TF1 *f1 = new TF1("fit","[0]+([1]*x)",0,300);  //Function for ploting the reconstructed ion tracks
   TF1 *f2 = new TF1("fit2","[0]+([1]*x)",0,300); //Function for ploting the reconstructed ion tracks after rotation
   TF1 *plot;		// Function in order to plot all the tracks in the same graph

   f->cd();
   
   z0=z*1.0;
   az=z0;
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
   qualityFlag=1; //This event is marked as bad and will not be processed//
  }     

   	
   if(qualityFlag==0){   

   //slopeT_inv=atan((-0.5*PI)-theta);
   slopeT_inv=(1./slopeT); //Retrieve the slope of the lines describing ion tracks from the inverse function
   interceptT_inv=-(interceptT/slopeT); //Retrieve the intercept of the lines describing ion tracks from the inverse function
	
  //printf("Z0: %1.2f,  slope: %1.4f,  intercept: %1.4f \n",z0,slopeT_inv,interceptT_inv);
   xcol=(z0-interceptT_inv)/slopeT_inv; //Reconstructed horizontal position of the collimator in a plane paraller to the tracker
   plx->Fill(xcol,1);
   
   
   slopeP_inv=(1./slopeP); //Retrieve the slope of the lines describing ion tracks from the inverse function
   interceptP_inv=-(interceptP/slopeP); //Retrieve the intercept of the lines describing ion tracks from the inverse function
	
   ycol=(z0-interceptP_inv)/slopeP_inv; //Reconstructed vertical position of the collimator in a plane paraller to the tracker
   ply->Fill(ycol,1);

   
	
   xcol_rot=rot_tracks(theta_tilt,slopeT_inv,interceptT_inv,z0); //Reconstructed horizontal position of the collimator in a plane parallel to the collimator - Method with tracks' rotation 
   //printf("Z0: %1.2f,  slope: %1.4f\n",z0,xcol_rot);
   plx2->Fill(xcol_rot,1);
   
   intercept_line= az-(ang_coeff_line*ax);
   bx= (intercept_line-interceptT_inv)/(slopeT_inv-ang_coeff_line); //x coordinate of the interesction point B(bx,bz) between each track and a line parallel to the collimator
   bz= (ang_coeff_line*bx) + intercept_line;    //z coordinate of the interesction point B(bx,bz) between each track and a line parallel to the collimator
 
   xcol3= sqrt((pow(ax-bx,2.)) + (pow(az-bz,2.))); //Reconstructed horizontal position of the collimator in a plane parallel to the collimator - Method with intersection of lines  
   plx3->Fill(xcol3,1);
   
  spot->Fill(xcol3,ycol);//Reconstructed beam profile in a plane paraller to the tracker
   
   //printf("Z0: %1.2f,  slope_rot: %1.4f,  Intercept_rot: %1.4f \n",z0,slope_rot,inter_rot);
     
  /* colplaneY->Update();
   colplaneY->cd();
   plot=new TF1("fit","[0]+([1]*x)",0,300);
   plot->SetParameter(0,interceptT_inv);
   plot->SetParameter(1,slopeT_inv);
   plot->SetLineColor(kGreen+1);
   plot->Draw("same");
   colplaneY->Update();*/
   //gPad->WaitPrimitive();
   
   }
   	}

   		
// Calculate the horizontal position resolution in a plane parallel to the tracker
/*
   colplaneX->cd();
   plx->Draw();
   
   binmax = plx->GetMaximumBin(); 
   xmax = plx->GetXaxis()->GetBinCenter(binmax);
   plx->GetXaxis()->SetRangeUser(xmax-15,xmax+15);


   fitResX=plx->Fit("gaus","S");
   sigmaDistCollX=fitResX->Value(2);
   fwhmX=(2.35*sigmaDistCollX);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resX=sqrt(abs(pow(fwhmX,2.)-pow(coll_size/cos(0.5*PI-(theta_tilt*PI/180.0)),2.)));
 
   */
   // Calculate the horizontal position resolution in a plane parallel to the collimator - Method with tracks' rotation 
/*
   colplaneX2->cd();
   plx2->Draw();
   
   binmax = plx2->GetMaximumBin(); 
   xmax = plx2->GetXaxis()->GetBinCenter(binmax);
   plx2->GetXaxis()->SetRangeUser(xmax-15,xmax+15);


   fitResX2=plx2->Fit("gaus","S");
   sigmaDistCollX2=fitResX2->Value(2);
   fwhmX2=(2.35*sigmaDistCollX2);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resX2=sqrt(abs(pow(fwhmX2,2.)-pow(coll_size,2.)));

   //double error=fitResX2->Error(2)/sigmaDistCollX2;
   //cout<<"Error: "<< error <<" %"<<endl;
   */
   
   // Calculate the horizontal position resolution in a plane parallel to the collimator - Method with intersection of lines
   
   colplaneX3->cd();
   plx3->Draw();
   
   binmax = plx3->GetMaximumBin(); 
   xmax = plx3->GetXaxis()->GetBinCenter(binmax);
   plx3->GetXaxis()->SetRangeUser(xmax-15,xmax+15);


   fitResX3=plx3->Fit("gaus","S");
   sigmaDistCollX3=fitResX3->Value(2);
   fwhmX3=(2.35*sigmaDistCollX3);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resX3=sqrt(abs(pow(fwhmX3,2.)-pow(coll_size,2.)));


   colplaneY->cd();
   ply->Draw();
   
   binmax = ply->GetMaximumBin(); 
   xmax = ply->GetXaxis()->GetBinCenter(binmax);
   ply->GetXaxis()->SetRangeUser(xmax-4,xmax+4);


   fitResY=ply->Fit("gaus","S");
   sigmaDistCollY=fitResY->Value(2);
   fwhmY=(2.35*sigmaDistCollY);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resY=sqrt(abs(pow(fwhmY,2.)-pow(coll_size,2.)));


   //cout<<"X position resolution: "<< resX <<" mm"<<endl;
   //cout<<"X position resolution2: "<< resX2 <<" mm"<<endl;
   cout<<"X position resolution3: "<< resX3 <<" mm"<<endl;
   cout<<"Y position resolution: "<< resY <<" mm"<<endl;
   
   cout<<""<<endl;
   cout<<""<<endl;

  beam->cd();
  spot->Draw();

 
 
   ifstream file(filename.c_str());
  
   if (!file) {
   cout << "The file " << filename << " has been created." << endl;
   ofstream file;
   file.open("resolution.txt");
    
   //file << "Zo (mm)	X resolution (mm) Y resolution (mm) \n";
   file << z0 << " " << resX << " " << fwhmX << " "  << resX2 << " " << fwhmX2 << " "  << resX3 << " " << fwhmX3<< endl;

   file.close();
   }
    
   else{
    
   cout << "The file " << filename << " is being updated ..." << endl;
    
   ifstream file(filename.c_str());
   while(!file.eof()) {
   file >> temp1[lineCount] >> temp2[lineCount] >> temp3[lineCount] >> temp4[lineCount] >> temp5[lineCount] >> temp6[lineCount] >> temp7[lineCount];
   lineCount++ ;
   }
    
 
   temp1[lineCount-1]=z0;
   temp2[lineCount-1]=resX;
   temp3[lineCount-1]=fwhmX;
   temp4[lineCount-1]=resX2;
   temp5[lineCount-1]=fwhmX2;
   temp6[lineCount-1]=resX3;
   temp7[lineCount-1]=fwhmX3;

   ofstream file2;  
   file2.open("resolution2.txt");
    
   for(int lines=0; lines<lineCount; lines++){
   file2 << temp1[lines] << "\t" << temp2[lines] << "\t" << temp3[lines] << "\t" << temp4[lines] << "\t" << temp5[lines] << "\t" << temp6[lines] << "\t" << temp7[lines] << endl;
   }
    
   file.close();
   file2.close();
   remove("resolution.txt");
   rename("resolution2.txt","resolution.txt");
   }

   
}

