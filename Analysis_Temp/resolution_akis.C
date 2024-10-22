#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif


double calc_z0(double theta_tilt, double source_holder_length, double l1){

	const double PI = 3.14159;
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
   Double_t intercept;		// intercept of the linear equation describing the ion track (in mm)
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
   double sigmaDistColl;	// sigma of the distribution of the reconstructed x coordinate of the collimator (in mm)
   double fwhm;			// FWHM of the distribution of the reconstructed x coordinate of the collimator (in mm)
   double res;			// Resolution in the determination of the collimator apperture (in mm)


// other variables
   double slope; 		// slope of the ion track 
   double xcol; 		// reconstructed x position of the collimator in a plane parallel to the tracker (in mm)

   TFitResultPtr fitRes;


//////////////////////////////////////////////////////////////////////////////

// open file
   char fileIn[50];

      if(run<10){
      sprintf(fileIn, "../Tracks/tracks_run00%i_B.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i_B.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i_B.root", run);
   } 
//      sprintf(fileIn, "../Tracks/tracks_run%i_B.root", run);
   cout<<fileIn<<endl;
   cout<<""<<endl;
   cout<<""<<endl;

   TFile *f = new TFile(fileIn);
   TTree *tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("theta",&theta);
   tree->SetBranchAddress("phi",&phi);
   tree->SetBranchAddress("intercept",&intercept);

   int entries=tree->GetEntries();
   cout<<"Entries in file "<< entries <<endl;
   cout<<""<<endl;
   cout<<""<<endl;

// END: open file 

//////////////////////////////////////////////////////////////////////////////



// Declaring Histos 
   TH1F *pl = new TH1F("pl","",10000,0.0,500.0);  //histo with the reconstructed x coordinate of the colimator
   
   pl->GetXaxis()->SetTitle("X_{col.} (mm)");
   pl->GetXaxis()->SetTitleOffset(1.05);
   pl->GetXaxis()->SetTitleSize(0.040);
   pl->GetXaxis()->SetTitleFont(62);
   pl->GetXaxis()->SetLabelOffset(0.008);
   pl->GetXaxis()->SetLabelSize(0.030);
   pl->GetYaxis()->SetNdivisions(505);
   pl->GetXaxis()->CenterTitle();

   pl->GetYaxis()->SetTitle("N (Counts)");
   pl->GetYaxis()->SetTitleOffset(1.30);
   pl->GetYaxis()->SetTitleSize(0.040);
   pl->GetYaxis()->SetTitleFont(62);
   pl->GetYaxis()->SetLabelOffset(0.008);
   pl->GetYaxis()->SetLabelSize(0.030);
   pl->GetYaxis()->SetNdivisions(505);
   pl->GetYaxis()->CenterTitle();


   TCanvas *colplane=new TCanvas("colplane","colplane",800,500,1000,800);

   f->cd();
   
	//z0=calc_z0(theta_tilt, source_holder_length, l1);
	z0=-z*1.0;
   for(int i=0; i<entries; i++){
	tree->GetEntry(i);
	slope=atan((-0.5*PI)-theta);
	//printf("Z0: %1.2f,  slope: %1.4f,  intercept: %1.4f \n",z0,slope,intercept);
	xcol=(z0-intercept)/slope;
	pl->Fill(xcol,1);
	
	}


   colplane->cd();
   pl->Draw();
   
   int binmax = pl->GetMaximumBin(); 
   double xmax = pl->GetXaxis()->GetBinCenter(binmax);
   pl->GetXaxis()->SetRangeUser(xmax-15,xmax+15);


   fitRes=pl->Fit("gaus","S");
   sigmaDistColl=fitRes->Value(2);
   fwhm=(2.35*sigmaDistColl);
   
   cout<<""<<endl;
   cout<<""<<endl;
   
   res=sqrt(pow(fwhm,2.)-pow(coll_size/cos(0.5*PI-(theta_tilt*PI/180.0)),2.));
   
   cout<<"Position resolution: "<< res <<" mm"<<endl;
   cout<<""<<endl;
   cout<<""<<endl;
  
  printf("%1.2f %1.4f %1.2f \n",z0,sigmaDistColl,fitRes->Value(1));
 
 gPad->WaitPrimitive(); 
   
}

