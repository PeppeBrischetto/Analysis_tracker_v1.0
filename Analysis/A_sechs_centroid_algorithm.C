//################################################################################################################################################

//Macro that reads the information from a first-level root file (dig_xxxxx.root) and determines the centroid and the RMS of the charge distribution using the SECHS algorithm and subsequently, the horizontal angle of the ion track. The deduced information is written to a third-level root file.

//To run simply open a root session and type: .x A_sechs_centroid_algorithm(run number)//

//Created in November 2024 by O. Sgouros based on the trackGenereator.C macro//

//################################################################################################################################################


#include <iostream>
#include "TFile.h"
#include "TTree.h"

void A_sechs_centroid_algorithm (int run) {

   cout<<" "<<endl;
   cout<<" "<<endl;
  
   float DeltaT=1000000;  // Time window (in ps) to defeine an event
   int thresh=0;	  // Consider a threshold in the charge distribution for each entry

   Double_t padWidth = 5.; // Pad width in mm


   double *zrow = new double[5];  // Z coordinate (in mm) of the row from the beginning of the active volume
   zrow[0]=18.60-7;		  // Valid for the prototype 2
   zrow[1]=39.80-7;
   zrow[2]=61.00-7;
   zrow[3]=82.20-7;
   zrow[4]=103.40-7;
   
  // input file variables (tracker)
   UShort_t Channel; 
   UShort_t pad; 
   UShort_t FTS;
   ULong64_t CTS;   
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Charge;
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Row;
   UShort_t Section;


   UInt_t rowMultiplicity=4; 	// Accept tracks with a number of hit row bigger than rowMultiplicity
   int flag[11];		// Variable to mark when a row is fired
   for(int i=0; i<11; i++) flag[i]=0; //Initialize matrix flag[11]
   
   int binmax=0 ;
   int max=0 ;
   int entryMerged=-1;



   int np=0; // Number of points of the Tgraph for plotting pairs (x,z)
   char dummyString[50];

   int kk=0;
   double charge= 0.0;

// output file variables

   double cl_charge[11]= {0};	   	// Cumulative charge of the pads belonging to the same cluster
   Int_t cl_padMult[5]={0};		// Number of pads of a cluster
   Int_t a_pads_fired[5][100];
   double cl_x[5];			// X centroid of a cluster in pads unit calculated with the SECHS algorithm
   double cl_x_mm[5];			// X centroid of a cluster in mm calculated with the the SECHS algorithm
   double cl_x_rms[5];  		// RMS of the charge distribution of a cluster in pads units
   
   double slopeT, interceptT;
   double theta;			// Horizontal angle using data from Rows 0-4 in rad
   double theta_deg;			// Horizontal angle using data from Rows 0-4 in deg

 
//////////////////////////////////////////////////////////////////////////////   
// open tracker file
   char fileInTracker[50];
      if(run<10){
      sprintf(fileInTracker, "../Merged_data/run_00%i/merg_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileInTracker, "../Merged_data/run_0%i/merg_0%i.root", run, run);
   }else{
      sprintf(fileInTracker, "../Merged_data/run_%i/merg_%i.root", run, run);
   } 


   TFile *finTracker = new TFile(fileInTracker);
   TTree *treeTracker = (TTree*)finTracker->Get("Data_R");
   
   treeTracker->SetBranchAddress("Board",&Board);
   treeTracker->SetBranchAddress("Channel",&Channel);
   treeTracker->SetBranchAddress("FineTSInt",&FTS);
   treeTracker->SetBranchAddress("CoarseTSInt",&CTS);
   treeTracker->SetBranchAddress("Timestamp",&Timestamp);
   treeTracker->SetBranchAddress("Charge",&Charge);
   treeTracker->SetBranchAddress("Flags",&Flags);
   treeTracker->SetBranchAddress("Pads",&pad);
   treeTracker->SetBranchAddress("Charge_cal",&Charge_cal);
   treeTracker->SetBranchAddress("Row",&Row);
   treeTracker->SetBranchAddress("Section",&Section);
   
   int entriesTracker=treeTracker->GetEntries();
 
   
   // OPEN output ROOT file //
   char fileOutName[50];
   if(run<10){
         sprintf(fileOutName,"../Tracks/tracks_run00%i_akis.root",run);
      }else if(run <100){
         sprintf(fileOutName,"../Tracks/tracks_run0%i_akis.root",run);
      }else{
         sprintf(fileOutName,"../Tracks/tracks_run%i_akis.root",run);
      } 
   
   TFile *fileOut = new TFile(fileOutName, "recreate");
   TTree *treeOut = new TTree("Data_R", "Third level tree");

   // Tracker Variables
   treeOut->Branch("cl_x",cl_x,"cl_x[5]/D");
   treeOut->Branch("cl_x_mm",cl_x_mm,"cl_x_mm[5]/D");
   treeOut->Branch("cl_x_rms",cl_x_rms,"cl_x_rms[5]/D");
   treeOut->Branch("cl_padMult0",&cl_padMult[0],"cl_padMult0/I");
   treeOut->Branch("cl_padMult1",&cl_padMult[1],"cl_padMult1/I");
   treeOut->Branch("cl_padMult2",&cl_padMult[2],"cl_padMult2/I");
   treeOut->Branch("cl_padMult3",&cl_padMult[3],"cl_padMult3/I");
   treeOut->Branch("cl_padMult4",&cl_padMult[4],"cl_padMult4/I");


   // Fit variables
   treeOut->Branch("interceptT",&interceptT,"interceptT/D");      
   treeOut->Branch("slopeT",&slopeT,"slopeT/D");      

   treeOut->Branch("theta",&theta,"theta/D");   
   treeOut->Branch("theta_deg",&theta_deg,"theta_deg/D");


   int i,j,k;

   TH1F *row[5];

   for (int i=0; i<5; i++){

   sprintf(dummyString,"r%i",i);
   row[i]=new TH1F(dummyString,dummyString,60,-0.5,59.5);

   row[i]->GetXaxis()->SetTitle("Pad");
   row[i]->GetYaxis()->SetTitle("N (Counts)");

   }


   TGraph *grTheta=new TGraph(0);
   grTheta->SetMarkerStyle(20);
   grTheta->SetMarkerSize(1);
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,300);
   TFitResultPtr fitResultTheta;
   TF1 *theta_fit_result = new TF1("theta_fit_result","[0]+([1]*x)",0.,107.);
   
   TF1  *func = new TF1("func","[0]/(pow(((exp(3.14159*(x-[1])/[2]) + exp(-3.14159*(x-[1])/[2]))/2),2))",5,55);
   func->SetNpx(2000);
   //func->FixParameter(0,10);
   //func->FixParameter(1,30);
   //func->FixParameter(2,0.40); 
// Opening the files

   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;



//################### Event loop ######################################
   for(int i=0; i<entriesTracker; i++){

   treeTracker->GetEntry(i);
   
   if((Timestamp-timeinit)<DeltaT){

   for (int k=0; k<5; k++){	

   if (Row==k){
   flag[k]=1;
   row[k]->Fill(pad,Charge); 
              
     }
   }

 } else{ //1st Else



   for (int j=0; j<5; j++){
   cl_padMult[j] = 0;
   cl_charge[j] = 0.;
   }
   
   
   np=0;

   theta_deg= -1000;
   theta=-1000;
   
   
 if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){ //Accept events that gave hits to all rows
 
 
 for(int j=0;j<5;j++){ //Loop on the rows//

   binmax = row[j]->GetMaximumBin();
   max = row[j]->GetBinContent(binmax);
   double xmax = row[j]->GetXaxis()->GetBinCenter(binmax);

    kk=0;
   for (int k=0; k<60; k++) {
   charge = row[j]->GetBinContent(k);
   cl_charge[j] += charge;
   if (charge) {cl_padMult[j]++; a_pads_fired[j][kk]=k; kk++; }  // 2024-06-26 G.B. writing the fired pads in a 2D array, one array for each row
  }

  
   //printf("%d\n",cl_padMult[1]);

 //if(cl_padMult[0]==2) {
   func->SetParLimits(1,xmax-5,xmax+5);
   func->SetParLimits(2,1.0,10.0);

   cl_x_rms[j] = row[j]->GetRMS();   
   row[j]->Fit("func","QW");
   cl_x[j]= func->GetParameter(1);
   cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;
   
   if(max>100){grTheta->SetPoint(np++, zrow[j], cl_x_mm[j]);}
 

   //row[j]->Draw();
   //gPad->WaitPrimitive();
   //cl_x[j] = row[j]->GetMean();
   //cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;


//}
 

      }//End of the "row" loop//
      
   fitResultTheta=grTheta->Fit("lin1","SQ");
   
   if(fitResultTheta==0){
    
   interceptT = fitResultTheta->Value(0);
   slopeT = fitResultTheta->Value(1);
   
   theta = TMath::ATan(slopeT);
   theta_deg = theta*180./TMath::Pi();
  
   }
   
 else{
   theta = -1000;
   theta_deg = -1000;
               
           }
      
   treeOut->Fill();

    }//End of the acceptance if
    
 
 	 // Start a new Event
	 timeinit=Timestamp;
	 entryMerged=i;
	 grTheta->Set(0);
	// grPhi->Set(0);
	
   	 for(int j=0;j<5; j++){
	    row[j]->Reset("ICES");
	    flag[j]=0;
         }

            for (int k=0; k<5; ++k) {
               if (Row==k) {
                  flag[k]=1;
                  row[k]->Fill(pad,Charge);
               }
            }
            
 

  }//End of the 1st Else//

 } //End of the loop on entries//
 
   fileOut->cd();
   treeOut->Write();
   fileOut->Purge();
   fileOut->Close();


}
