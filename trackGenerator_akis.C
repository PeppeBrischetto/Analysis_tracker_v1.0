//###################################################################################################
//#   Purpose: macro that writes a third level root file containing a tree with the theta and phi angles of the tracks,
//#   as well as information on the centroid, rms, drift times for each row.
//#   it takes as input a single file from a single channel of SiC and a 
//#   merged file from the tracker.  
//#   make a fit of five clusters of a track and return the slope. 
//#   it can plot the histograms with the angles of the tracks, but it is not necessary
//#   
//#   required as argument the run number and a int that is 0 if there is no Sic and 1 if there is a Sic file
//#           * as input the DeltaT, time interval that is used to define an event, and the Time window
//#            		where to seek the tracks.
//#           
//# 	       			
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created may 2024 from B_anglesFinder_plot_tracker_and_sic_v4.C by G. Brischetto
//#######################################
//#   modified: 6 Jun 2024 commenting 
//# 			   renaming of variables  
//#			   solved bug on track without sic    D. Torresi
//#   updated : 10 Jun 2024 extension to no-segmented rows (i.e. pads), transforming 5 -> 11    A. Pitronaci
//#   updated : 24 Jun 2024 insert option to not consider the SiC file  G. Brischetto
//#   updated :  8 oct 2024 corrected the zcoordinate D. Torresi
//#   updated : 22 oct 2024 removed TCanvas and plot D. Torresi, add output variables slopeT slopeP interceptT interceptP D. Torresi
//#   updated : 24 oct 2024 cl_y now is written correctly for the tracks with a Sic G. Brischetto D. Torresi
//#   updated :  8 nov 2024 Added theta with less row and added for each event the corresponding entry of the Merged file. D. Torresi
//###################################################################################################


void trackGenerator_akis(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

  // setted parameters
   // window that define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;

   Float_t timeWindowlow = 0.5E+06; // time (in ps) that the primary electrons need to reach the anode
   Float_t timeWindowhigh = 3.0E+06;

   Double_t timeOffset = 10.; // time (in ps) used to avoid that the time difference Timestamp-timeinit 
                              // is equal to zero for the first event entry (see where time histos are filled)
                              // This is important when you want to calculate the time average weighted by the charge

   Double_t padWidth = 5.; // pad width in mm

   Double_t velocity = 50.;  // drift velocity (in mm/us) of electrons in isobutane;  
   Double_t velocity_mm_ps = velocity/1.0E+06;  // drift velocity (in mm/ps) of electrons in isobutane;

   double *zrow = new double[5];  // zcoordinate (in mm) of the row from the begnning of the active volume
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
 


  // output file variables
   double cl_charge[11] = {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit

   // Secondary variables  
   ULong64_t TimestampSicTemp;   
   UInt_t SicLoopFlag;			// variable used to stop the loop on the Sic file
   
   Long64_t TimeDiff;                   // variable needed to properly calculate the difference between timeinit and timestampSic, otherwise it is not a Long64 but maybe an Int

   Double_t timeAverage[5] = {0.}; 	// variable used to calculate the average time (weighted by the charge) on a raw
   Double_t time = 0.;             	// variable used to store the time of a single pad (it is used for the calculation of the average time
   Double_t driftTime = 0.;        	// variable used to store the drift time of electrons corresponding to  row 2 (i.e. driftTime=timeSiC-timeRow2)

   int binmax=-100, max=-100;		// variables used to plot the histos
   // number of event in the run

   double binStrip=0;
   UInt_t flagTrackWithSiC=0;  
   UInt_t rowMultiplicity=4; 	// consider tracks with a number of hit row bigger than rowMultiplicity
   
   char anykey;			// variable used to pause the macro
   int flag[11];		// variable that are true when there is at least one hit in the Row

//////////////////////////////////////////////////////////////////////////////   
// open tracker file
   char fileInTracker[50];
      if(run<10){
      sprintf(fileInTracker, "Merged_data/run_00%i/merg_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileInTracker, "Merged_data/run_0%i/merg_0%i.root", run, run);
   }else{
      sprintf(fileInTracker, "Merged_data/run_%i/merg_%i.root", run, run);
   } 
   cout<<fileInTracker<<endl;
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
         sprintf(fileOutName,"Tracks/tracks_run00%i_test.root",run);
      }else if(run <100){
         sprintf(fileOutName,"Tracks/tracks_run0%i_test.root",run);
      }else{
         sprintf(fileOutName,"Tracks/tracks_run%i_test.root",run);
      } 
   
   TFile *fileOut = new TFile(fileOutName, "recreate");
   TTree *treeOut = new TTree("Data_R", "Third level tree");

   // Tracker Variables
   treeOut->Branch("cl_x", cl_x, "cl_x[5]/D");
   treeOut->Branch("cl_x_mm", cl_x_mm, "cl_x_mm[5]/D"); 
   treeOut->Branch("cl_x_rms", cl_x_rms, "cl_x_rms[5]/D");
   treeOut->Branch("cl_charge", cl_charge, "cl_charge[11]/D");
   treeOut->Branch("cl_padMult0",&cl_padMult[0],"cl_padMult0/I");
   treeOut->Branch("cl_padMult1",&cl_padMult[1],"cl_padMult1/I");
   treeOut->Branch("cl_padMult2",&cl_padMult[2],"cl_padMult2/I");
   treeOut->Branch("cl_padMult3",&cl_padMult[3],"cl_padMult3/I");
   treeOut->Branch("cl_padMult4",&cl_padMult[4],"cl_padMult4/I");


   TH1F *row[11];
   char dummyString[50];
      double charge = 0.0;
   
   
   for (int i=0; i<5; ++i){
       sprintf(dummyString,"r%i",i);
       row[i]=new TH1F(dummyString,dummyString,60,-0.5,59.5);
   }


   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);

   
  
   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   //cout<<" time init tracker: "<<timeinit<<endl;

  
   
//################### Event loop ######################################
   for(int i=0; i<1000000; i++){
      treeTracker->GetEntry(i);

      //cout<<"entry: "<<i<<"/"<<entriesTracker<<endl;      
      printf("\rentry %i/%i.", i+1, entriesTracker);
      fflush(stdout);
      

      if((Timestamp-timeinit)<DeltaT){
         //Fill histos
         if (Charge > thresh) {
            for (int k=0; k<5; ++k) {	
               if (Row==k) {
                  flag[k]=1;
                  row[k]->Fill(pad,Charge);               
               }
            }
            

         }
      }else{
        
      	 // The event is finished. Plot if there is something
      	 //cout << "\n-------- End Event ---------\n" << endl;

         //cl_charge = 0.;
      	 for (int j=0; j<5; j++) {
      	     timeAverage[j] = 0.;
             cl_padMult[j] = 0;
             cl_charge[j] = 0.;
         }
         
         //if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]+flag[5]+flag[6]+flag[7]+flag[8]+flag[9]+flag[10]>rowMultiplicity){
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){

	    for(int j=0; j<5; j++){
	       //cl_x[j]=0;
	       binmax = row[j]->GetMaximumBin();
	       max  = row[j]->GetBinContent(binmax);
	       //cout<<binmax<<"  "<<max<<endl;
	      

	       for (int k=0; k<60; k++) {
                   charge = row[j]->GetBinContent(k);
	           cl_charge[j] += charge;
                   
	       }
	       
	       //cout << "****** " << cl_padMult[j] << endl;
	       
 	       cl_x[j] = row[j]->GetMean();
               cl_x_rms[j] = row[j]->GetRMS();
	       cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;
      
            }
            
            }//Flag IF//
	    

         //treeOut->Fill();
	 }

         
	 // Start a new Event
	 timeinit=Timestamp;

      }
    row[3]->Draw("HIST");
      
  
/*   fileOut->cd();
   treeOut->Write();
   fileOut->Purge();
   fileOut->Close(); */   
      
      
   }

