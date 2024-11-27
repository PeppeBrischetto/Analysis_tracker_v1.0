#include <iostream>
#include "TFile.h"
#include "TTree.h"

void A_sechs_centroid_algorithm (int run) {

   cout<<" "<<endl;
   cout<<" "<<endl;
  
   float DeltaT=1000000;  //in ps
   int thresh=0;

   Double_t padWidth = 5.; // pad width in mm


   double *zrow = new double[5];  // zcoordinate (in mm) of the row from the beginning of the active volume
   zrow[0]=18.60-7;		  // valid for the prototype 2
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


   UInt_t rowMultiplicity=4; 	// consider tracks with a number of hit row bigger than rowMultiplicity
   int flag[11];		// variable that are true when there is at least one hit in the Row
   int binmax=0 ;
   int max=0 ;
   int entryMerged=-1;

   for(int i=0; i<11; i++) flag[i]=0;

   int np=0; // number of point of the Tgraph
   char dummyString[50];
   char dummyString2[50];


   int kk=0;
   double charge= 0.0;

   double cl_charge[11]= {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   Int_t a_pads_fired[5][100];
   double cl_x[5];			// x centroid of a cluster in pads unit calculated with the Weighted COG algorithm
   double cl_x_mm[5];			// x centroid of a cluster in mm calculated with the Weighted COG algorithm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit calculated with the Weighted COG algorithm
   double theta;
   double theta_deg;

 
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
   treeOut->Branch("cl_x", cl_x, "cl_x[5]/D");
   treeOut->Branch("cl_x_mm", cl_x_mm, "cl_x_mm[5]/D"); 
 

   int i,j,k;

   TH1F *row[5];
   TH1F *xpos[5];

   for (int i=0; i<5; i++){

   sprintf(dummyString,"r%i",i);
   sprintf(dummyString2,"XWCOG_%i",i);

   row[i]=new TH1F(dummyString,dummyString,60,-0.5,59.5);
   xpos[i]=new TH1F(dummyString2,dummyString2,60,-0.5,59.5);

   row[i]->GetXaxis()->SetTitle("pad");
   row[i]->GetYaxis()->SetTitle("counts");

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

 if(cl_padMult[0]>2) {
   func->SetParLimits(1,xmax-5,xmax+5);
   func->SetParLimits(2,1.0,10.0);
   
   row[j]->Fit("func","QW");
   cl_x[j]= func->GetParameter(1);
   cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;
 

   //row[j]->Draw();
   //gPad->WaitPrimitive();
   //cl_x[j] = row[j]->GetMean();
   //cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;


}
 

      }//End of the "row" loop//
      
   treeOut->Fill();

    }//End of the acceptance if
    
 
 	 // Start a new Event
	 timeinit=Timestamp;
	 entryMerged=i;
	// grTheta->Set(0);
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
