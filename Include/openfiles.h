#ifndef OPENFILES_H
#define OPENFILES_H

 
 
 
 
// file variables for tracks file
   double cl_charge[11];	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5];		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5];		// y centroid of a cluster in time
   double cl_y_mm[5];		// y centroid of a cluster in mm
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
   Int_t a_pads_fired[5][100];
   double slopeT, interceptT;
   double slopeP, interceptP;
   int sic_fired;
   
   
   
// input file variables for tracks file   
   
   
   
   
   
   
   
#endif
