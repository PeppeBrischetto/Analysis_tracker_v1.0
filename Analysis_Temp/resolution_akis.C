#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

#include <fstream>
#include <sstream>
#include <string>

using namespace std;


double calc_z0(double theta_tilt, double source_holder_length, double l1){

	const double PI = 3.14159; //Definition of Pi
	double l2;
	double result;
	l2=(l1-0.50*source_holder_length)*tan(theta_tilt*PI/180.0);
	result=-l2/cos((0.50*PI)-(theta_tilt*PI/180.0));
	return result;

}

void resolution_akis(int run, double z)
{

cout<<""<<endl;
cout<<""<<endl;


   const double PI = 3.14159;   //Definition of Pi

// input file variables (tracker)
   double cl_charge[11] = {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5] = {0};		// y centroid of a cluster in time
   double cl_y_mm[5] = {0};		// y centroid of a cluster in mm
   Double_t slopeT;		// slope of the linear fit X= f(Z)
   Double_t interceptT;		// intercept of the linear fit X= f(Z) (in mm)
   Double_t slopeP;		// slope of the linear fit Y= f(Z)
   Double_t interceptP;		// intercept of the linear fit Y= f(Z) (in mm)
   Double_t theta;		// theta of the track in rad
   Double_t theta_deg;		// theta of the track in deg
   Double_t phi;		// phi of the track in rad
   Double_t phi_deg;		// phi of the track in deg
   Double_t chiSquareTheta;
   Double_t chiSquarePhi;



// Geometric variables required for positioning the collimator respect to the detector
   double coll_size= 1.0;	       // Dimension of the collimator front face (in mm)
   double source_holder_length= 30.0;  // Length of the source holder (in mm)
   double theta_tilt= 40.0;	       // Rotation angle of the collimator with respect to the beam axis (in mm)
   double l1= 99.0;	       	       // Minimum horizontal distance between the collimator and the tracker (in mm)
   double zcol=-38.0;		       // z coordinate of the collimator (in mm)
   
   
// Geometric variables required for the definition of a plane z'= const. parallel to the tracker
 
   double l2;	       	// Minimum distance between the collimator and the tracker (in mm)
   double z0; 		// z coordinate of the plane (in mm) - (i.e. its distance from the tracker)



// Variables for the estimation of the resolution
   double sigmaDistCollX;	// sigma of the distribution of the reconstructed X coordinate of the collimator (in mm)
   double fwhmX;		// FWHM of the distribution of the reconstructed X coordinate of the collimator (in mm)
   double resX;			// Resolution in the determination of the collimator apperture (in mm)
   
   double sigmaDistCollY;	// sigma of the distribution of the reconstructed Y coordinate of the collimator (in mm)
   double fwhmY;		// FWHM of the distribution of the reconstructed Y coordinate of the collimator (in mm)
   double resY;			// Resolution in the determination of the collimator apperture (in mm)



// other variables
   double slopeT_inv; 		//slope of the linear fit Z=f(X) describing the ion track
   double interceptT_inv; 	//intercept of the linear fit Z=f(X) describing the ion track (in mm)
   double xcol; 		// reconstructed X position of the collimator in a plane parallel to the tracker (in mm)
   
   double slopeP_inv; 		//slope of the linear fit Z=f(Y) describing the ion track
   double interceptP_inv; 	//intercept of the linear fit Z=f(Y) describing the ion track (in mm)
   double ycol; 		// reconstructed Y position of the collimator in a plane parallel to the tracker (in mm)
   
   int binmax; 			// Find the bin with the most statistics
   double xmax; 		// X position of the bin with the most statistics
   double ymax; 		// Y position of the bin with the most statistics

   
   string filename = "resolution.txt";  //File to store the values of Z0 and horizontal position resolution
   int lineCount = 0;			//Variable to store the lines' number of the file "resolution.txt"
   double temp1[10000], temp2[10000], temp3[10000];  //Matrices to store the values of Z0 and X, Y resolutions from the file resolution.txt, before passing them to resolution2.txt

   TFitResultPtr fitResX;
   TFitResultPtr fitResY;

//////////////////////////////////////////////////////////////////////////////

// open file
   char fileIn[50];

      if(run<10){
      sprintf(fileIn, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i.root", run);
   } 
//      sprintf(fileIn, "../Tracks/tracks_run%i_B.root", run);
   cout<<fileIn<<endl;
   cout<<""<<endl;
   cout<<""<<endl;

   TFile *f = new TFile(fileIn);
   TTree *tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("theta",&theta);
   tree->SetBranchAddress("phi",&phi);
   tree->SetBranchAddress("slopeT",&slopeT);
   tree->SetBranchAddress("interceptT",&interceptT);
   tree->SetBranchAddress("slopeP",&slopeP);
   tree->SetBranchAddress("interceptP",&interceptP);

   int entries=tree->GetEntries();
   cout<<"Entries in file "<< entries <<endl;
   cout<<""<<endl;
   cout<<""<<endl;

// END: open file 

//////////////////////////////////////////////////////////////////////////////



// Declaring Histos 
   TH1F *plx = new TH1F("plx","",20000,-300.0,500.0);  //histo with the reconstructed X coordinate of the colimator
   TH1F *ply = new TH1F("ply","",20000,-300.0,500.0);  //histo with the reconstructed Y coordinate of the colimator
   
   plx->GetXaxis()->SetTitle("X_{col.} (mm)");
   plx->GetXaxis()->SetTitleOffset(1.05);
   plx->GetXaxis()->SetTitleSize(0.040);
   plx->GetXaxis()->SetTitleFont(62);
   plx->GetXaxis()->SetLabelOffset(0.008);
   plx->GetXaxis()->SetLabelSize(0.030);
   plx->GetYaxis()->SetNdivisions(505);
   plx->GetXaxis()->CenterTitle();

   plx->GetYaxis()->SetTitle("N (Counts)");
   plx->GetYaxis()->SetTitleOffset(1.30);
   plx->GetYaxis()->SetTitleSize(0.040);
   plx->GetYaxis()->SetTitleFont(62);
   plx->GetYaxis()->SetLabelOffset(0.008);
   plx->GetYaxis()->SetLabelSize(0.030);
   plx->GetYaxis()->SetNdivisions(505);
   plx->GetYaxis()->CenterTitle();
   
   ply->GetXaxis()->SetTitle("Y_{col.} (mm)");
   ply->GetXaxis()->SetTitleOffset(1.05);
   ply->GetXaxis()->SetTitleSize(0.040);
   ply->GetXaxis()->SetTitleFont(62);
   ply->GetXaxis()->SetLabelOffset(0.008);
   ply->GetXaxis()->SetLabelSize(0.030);
   ply->GetYaxis()->SetNdivisions(505);
   ply->GetXaxis()->CenterTitle();

   ply->GetYaxis()->SetTitle("N (Counts)");
   ply->GetYaxis()->SetTitleOffset(1.30);
   ply->GetYaxis()->SetTitleSize(0.040);
   ply->GetYaxis()->SetTitleFont(62);
   ply->GetYaxis()->SetLabelOffset(0.008);
   ply->GetYaxis()->SetLabelSize(0.030);
   ply->GetYaxis()->SetNdivisions(505);
   ply->GetYaxis()->CenterTitle();



   TCanvas *colplaneX=new TCanvas("colplaneX","colplaneX",800,500,1000,800);
   TCanvas *colplaneY=new TCanvas("colplaneY","colplaneY",800,500,1000,800);

   f->cd();
   
	//z0=calc_z0(theta_tilt, source_holder_length, l1);
	z0=-z*1.0;
   for(int i=0; i<entries; i++){
	tree->GetEntry(i);
	//slopeT_inv=atan((-0.5*PI)-theta);
	slopeT_inv=(1./slopeT);
	slopeP_inv=(1./slopeP);
	
	interceptT_inv=-(interceptT/slopeT);
	interceptP_inv=-(interceptP/slopeP);
	
	//printf("Z0: %1.2f,  slope: %1.4f,  intercept: %1.4f \n",z0,slopeP_inv,interceptP_inv);
	xcol=(z0-interceptT_inv)/slopeT_inv;
	ycol=(z0-interceptP_inv)/slopeP_inv;
	plx->Fill(xcol,1);
	ply->Fill(ycol,1);
	
	}

// Calculate the horizontal position resolution

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
   

   
// Calculate the vertical position resolution
  
   colplaneY->cd();
   ply->Draw();
   
   binmax = ply->GetMaximumBin(); 
   ymax = ply->GetXaxis()->GetBinCenter(binmax);
   ply->GetXaxis()->SetRangeUser(ymax-15,ymax+15);


   fitResY=ply->Fit("gaus","S");
   sigmaDistCollY=fitResY->Value(2);
   fwhmY=(2.35*sigmaDistCollY);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   resY=sqrt(abs(pow(fwhmY,2.)-pow(coll_size/cos(0.5*PI-(theta_tilt*PI/180.0)),2.)));
   
   cout<<"X position resolution: "<< resX <<" mm"<<endl;
   cout<<"Y position resolution: "<< resY <<" mm"<<endl;
   cout<<""<<endl;
   cout<<""<<endl;

 
 
 
  ifstream file(filename.c_str());
  
  if (!file)
    {
    cout << "The file " << filename << " has been created." << endl;
    ofstream file;
    file.open("resolution.txt");
    //file << "Zo (mm)	X resolution (mm) Y resolution (mm) \n";
    file << z0 << " " << resX << " " << resY << endl;
    file.close();
    }
    

    else{
    
    cout << "The file " << filename << " is being updated ..." << endl;
    
    ifstream file(filename.c_str());
    while(!file.eof()) {
    file >> temp1[lineCount] >> temp2[lineCount] >> temp3[lineCount];
    lineCount++ ;
    }
    
    //cout << lineCount << "\n" << endl;
    
    temp1[lineCount-1]=z0;
    temp2[lineCount-1]=resX;
    temp3[lineCount-1]=resY;
        
    ofstream file2;  
    file2.open("resolution2.txt");
    
    for(int lines=0; lines<lineCount; lines++){
    file2 << temp1[lines] << "\t" << temp2[lines] << "\t" << temp3[lines] << endl;
    }
    
    file.close();
    file2.close();
    remove("resolution.txt");
    rename("resolution2.txt","resolution.txt");
    }

    cout << "\n" << endl;
   
}

