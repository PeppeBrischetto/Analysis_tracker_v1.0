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
//#   updated : 10 Jun 2024 extension to no-segmented rows (i.e. pads), transforming 5 -> 10    A. Pitronaci
//#   updated : 24 Jun 2024 insert option to not consider the SiC file  G. Brischetto
//#   updated :  8 oct 2024 corrected the zcoordinate D. Torresi
//#   updated : 22 oct 2024 removed TCanvas and plot D. Torresi, add output variables slopeT slopeP interceptT interceptP D. Torresi
//#   updated : 24 oct 2024 cl_y now is written correctly for the tracks with a Sic G. Brischetto D. Torresi
//###################################################################################################


void trackGenerator(int run, bool sicFileOpen)
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
 
  // input file variables (Sic)
   UShort_t ChannelSic;
   UShort_t FTSSic;
   ULong64_t CTSSic;
   ULong64_t TimestampSic;
   UShort_t BoardSic;
   UShort_t ChargeSic;
   Double_t Charge_calSic;
   UInt_t FlagsSic;


  // output file variables
   double cl_charge[11] = {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5] = {0};		// y centroid of a cluster in time [ps]
   double cl_y_mm[5] = {0};		// y centroid of a cluster in mm
   Double_t theta;		// theta of the track in rad
   Double_t theta_deg;		// theta of the track in deg
   Double_t phi;
   Double_t phi_deg;
   Double_t chiSquareTheta;
   Double_t chiSquarePhi;
   
   Int_t sic_charge;
   Double_t energySic; 
  
  
   Int_t a_pads_fired[5][100];
   
// other variables
   
   // fitting variables
   double slopeT, interceptT, chi2T;
   double slopeP, interceptP, chi2F;
   double charge = 0.0;
   double weigthed_pos[60];
   Double_t alpha=-100, alpha_deg=-100;  // auxiliary angle for the calculation of theta
   Double_t beta=-100, beta_deg=-100;    // auxiliary angle for the calculation of phi

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
   for(int i =0; i<11; i++) flag[i]=0;
   int flagM=1;				// Flag used to contine the macro without interruption
   int Fstrip=1;			// flag to plot strip: 0 plot, 1 no plot
   UInt_t FlagSicStop=0;  		// variable used to pause the macro with a SiC   
   UInt_t sicHits=0;    		// number of SiC hit
   UInt_t eventNumber=0;		// number of tracker event (tracks) in the run
   UInt_t tracksWithoutSic=0;
   UInt_t tracksWithSic=0;
   UInt_t sicWithoutTracks=0; 

   int np=0; // number of point of the Tgraph
   int npTime=0; // number of point of the Tgraph of the time
   char dummyString[50];

   //bool sicFileOpen = true; // if the SiC was not mounted in a run, this variable is to be set to false
   TString sicOrNot;

   // control variables
   int tracksCounter=0;		// counts all the tracks
   int trackNoSicsCounter=0;	// counts tracks without SiC
   int tracksSiCCounter=0;	// counts event with tracks and Sic (over threshold
   int SiCCounter=0;		// counts total Sic event over a threshold
   int EventEntries=0;		// entries per event
   
   int kk=0;

// END: Dichiarazione variabili		//////////////////////////////////////

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
   //cout<<"Entries tracker file "<< entriesTracker <<endl;
// END: open tracker file	//////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////   
// open Sic file

   TFile *finSic;
   TTree *treeSic;

   //cout << "Does this run have a SiC file? [y]es or [n]o" << endl;
   //cin >> sicOrNot;
   //sicFileOpen = (sicOrNot=="n" || sicOrNot=="N" || sicOrNot=="No" )? false : true;
   //cout << sicFileOpen << endl;

   int entriesSic = 0;
   
   char fileInSic[50];
   //if (sicFileOpen) {
   if (sicFileOpen) {
      if(run<10){
         sprintf(fileInSic, "Merged_data/run_00%i/sic_00%i.root", run, run);
      }else if(run <100){
         sprintf(fileInSic, "Merged_data/run_0%i/sic_0%i.root", run, run);
      }else{
         sprintf(fileInSic, "Merged_data/run_%i/sic_%i.root", run, run);
      } 
      cout<<fileInSic<<endl;
      finSic = new TFile(fileInSic);
      treeSic = (TTree*)finSic->Get("Data_R");
   
      treeSic->SetBranchAddress("Board",&BoardSic);
      treeSic->SetBranchAddress("Channel",&ChannelSic);
      treeSic->SetBranchAddress("FineTSInt",&FTSSic);
      treeSic->SetBranchAddress("CoarseTSInt",&CTSSic);
      treeSic->SetBranchAddress("Timestamp",&TimestampSic);
      treeSic->SetBranchAddress("Charge",&ChargeSic);
      treeSic->SetBranchAddress("Flags",&FlagsSic);
      treeSic->SetBranchAddress("Charge_cal",&Charge_calSic);
   
      entriesSic=treeSic->GetEntries();
      //cout<<"Entries sic file "<< entriesSic <<endl;
   }
// END: open SiC file	//////////////////////////////////////////////////////

// OPEN output ROOT file //
   char fileOutName[50];
   sprintf(fileOutName,"Tracks/tracks_run%i.root",run);
   TFile *fileOut = new TFile(fileOutName, "recreate");
   TTree *treeOut = new TTree("Data_R", "Third level tree");

   // Tracker Variables
   treeOut->Branch("cl_x", cl_x, "cl_x[5]/D");
   treeOut->Branch("cl_x_mm", cl_x_mm, "cl_x_mm[5]/D"); 
   treeOut->Branch("cl_y", cl_y, "cl_y[5]/D");
   treeOut->Branch("cl_y_mm", cl_y_mm, "cl_y_mm[5]/D");
   treeOut->Branch("cl_x_rms", cl_x_rms, "cl_x_rms[5]/D");
   treeOut->Branch("cl_charge", cl_charge, "cl_charge[11]/D");
   //treeOut->Branch("cl_padMult",cl_padMult,"cl_padMult[5]/I");
   treeOut->Branch("cl_padMult0",&cl_padMult[0],"cl_padMult0/I");
   treeOut->Branch("cl_padMult1",&cl_padMult[1],"cl_padMult1/I");
   treeOut->Branch("cl_padMult2",&cl_padMult[2],"cl_padMult2/I");
   treeOut->Branch("cl_padMult3",&cl_padMult[3],"cl_padMult3/I");
   treeOut->Branch("cl_padMult4",&cl_padMult[4],"cl_padMult4/I");
   treeOut->Branch("pads_fired0",&a_pads_fired[0],"a_pads_fired0[cl_padMult0]/I");
   treeOut->Branch("pads_fired1",&a_pads_fired[1],"a_pads_fired1[cl_padMult1]/I");
   treeOut->Branch("pads_fired2",&a_pads_fired[2],"a_pads_fired2[cl_padMult2]/I");
   treeOut->Branch("pads_fired3",&a_pads_fired[3],"a_pads_fired3[cl_padMult3]/I");
   treeOut->Branch("pads_fired4",&a_pads_fired[4],"a_pads_fired4[cl_padMult4]/I");
   
   //treeOut->Branch("pads_fired0",&a_pads_fired[0],"a_pads_fired0[100]/I");
   //treeOut->Branch("pads_fired1",&a_pads_fired[1],"a_pads_fired1[100]/I");
   //treeOut->Branch("pads_fired2",&a_pads_fired[2],"a_pads_fired2[100]/I");
   //treeOut->Branch("pads_fired3",&a_pads_fired[3],"a_pads_fired3[100]/I");
   //treeOut->Branch("pads_fired4",&a_pads_fired[4],"a_pads_fired4[100]/I");
   //treeOut->Branch("lastEntryEvent",&last,"a_pads_fired4[cl_padMult4]/I");          
   
   // Fit variables
   treeOut->Branch("phi",&phi,"phi/D");
   treeOut->Branch("theta",&theta,"theta/D");
   treeOut->Branch("phi_deg",&phi_deg,"phi_deg/D");      
   treeOut->Branch("theta_deg",&theta_deg,"theta_deg/D");
   treeOut->Branch("chiSquareTheta",&chiSquareTheta,"chiSquareTheta/D");   
   treeOut->Branch("chiSquarePhi",&chiSquarePhi,"chiSquarePhi/D");      
   treeOut->Branch("slopeT",&slopeT,"slopeT/D");      
   treeOut->Branch("interceptT",&interceptT,"interceptT/D");      
   treeOut->Branch("slopeP",&slopeP,"slopeP/D");      
   treeOut->Branch("interceptP",&interceptP,"interceptP/D");  

   // Sic variables
   treeOut->Branch("sic_fired",&FlagSicStop,"sic_fired/I");
   treeOut->Branch("sic_charge",&sic_charge,"sic_charge/I");
   treeOut->Branch("energySic",&energySic,"energySic/D");

//////////////////////////////////////////////////////////////////////////////
// Dichiarazione Histo, Canvas, TGraph and Functions
   TH1F *row[11];
   for (int i=0; i<5; ++i){
       sprintf(dummyString,"r%i",i);
       row[i]=new TH1F(dummyString,dummyString,60,-0.5,59.5);
   }
   for (int i=5; i<11; ++i){
       sprintf(dummyString,"r%i",i);
       row[i]=new TH1F(dummyString,dummyString,4,-0.5,3.5);
   }
   TH1D *h_time[5];
   for (int i=0; i<5; ++i) {
       sprintf(dummyString,"h_time%i",i);
       h_time[i]=new TH1D(dummyString,dummyString,60,-0.5,59.5);
   }

   for(int i=0;i<11; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   row[5]->SetLineColor(kGray+2);
   row[6]->SetLineColor(kGray+2);
   row[7]->SetLineColor(kGray+2);
   row[8]->SetLineColor(kGray+2);
   row[9]->SetLineColor(kGray+2);
   row[10]->SetLineColor(kGray+2);
   
   for(int i=0;i<5; i++){
     h_time[i]->GetXaxis()->SetTitle("pad");
     h_time[i]->GetYaxis()->SetTitle("timestamp");
   }
   h_time[1]->SetLineColor(kRed);

   /*TH1D *h_driftTime[11];
   for(int i=0;i<11;i++){
      sprintf(dummyString,"h_driftTime%i",i); 
      h_driftTime[i] = new TH1D(dummyString,dummyString,500,0.5E+00,5.0E+00);
      h_driftTime[i]->GetXaxis()->SetTitle("driftTime (#mu s)");
      h_driftTime[i]->GetYaxis()->SetTitle("counts");
      h_driftTime[i]->SetLineColor(i+1);
   }

   Int_t nbintheta=3600;
   Int_t nbinalpha=3600;   
   Int_t nbinphi=600;
   Double_t binmintheta=-180., binmaxtheta=180.;
   Double_t binminalpha=-180., binmaxalpha=180.;
   Double_t binminphi=-30., binmaxphi=30.;
      
   TH1D *h_angles[6];
   h_angles[0]=new TH1D("h_theta_","h_theta",nbintheta,binmintheta,binmaxtheta);
   h_angles[1]=new TH1D("h_theta_without_sic","h_theta_without_sic",nbintheta,binmintheta,binmaxtheta);
   h_angles[2]=new TH1D("h_theta_with_sic","h_theta_with_sic",nbintheta,binmintheta,binmaxtheta);
   
   h_angles[3]=new TH1D("h_phi","h_phi",nbinphi,binminphi,binmaxphi);
   h_angles[4]=new TH1D("h_phi_without_sic","h_phi_without_sic",nbinphi,binminphi,binmaxphi);
   h_angles[5]=new TH1D("h_phi_with_sic","h_phi_with_sic",nbinphi,binminphi,binmaxphi);
  
 
   TH1D *h_alpha[3];
   h_alpha[0]=new TH1D("h_alpha_","h_alpha",nbinalpha,binminalpha,binmaxalpha);
   h_alpha[1]=new TH1D("h_alpha_without_sic","h_alpha_without_sic",nbinalpha,binminalpha,binmaxalpha);
   h_alpha[2]=new TH1D("h_alpha_with_sic","h_alpha_with_sic",nbinalpha,binminalpha,binmaxalpha);
   
   for (Int_t k=0; k<6; k++) {
       if (k<3){
          h_angles[k]->GetXaxis()->SetTitle("#theta (deg)");
          h_alpha[k]->GetYaxis()->SetTitle("counts");
          h_alpha[k]->SetLineColor(k+1);
       }
       else
          h_angles[k]->GetXaxis()->SetTitle("#phi (deg)");
       
       h_angles[k]->GetYaxis()->SetTitle("counts");

       if (k%3==0)
          h_angles[k]->SetLineColor(kBlack);
       else if (k%3==1)
          h_angles[k]->SetLineColor(kBlue);
       else if (k%3==2)
          h_angles[k]->SetLineColor(kRed);
   }*/
 
  
   // Fitting TGraphs and functions
   TH2D *bg= new TH2D("bg","bg",107,0,107,130, 70,200);
   bg->GetXaxis()->SetTitle("z (mm)");
   bg->GetYaxis()->SetTitle("x (mm)");
   //fit x vs z  
   TGraph *grTheta=new TGraph(0);
   grTheta->SetMarkerStyle(20);
   grTheta->SetMarkerSize(1);
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,300);
   TFitResultPtr fitResultTheta;
   TF1 *theta_fit_result = new TF1("theta_fit_result","[0]+([1]*x)",0.,107.);; // this function is only for drawing the fit result

   //fit y vs z
   TGraph *grPhi=new TGraph(0);
   grPhi->SetMarkerStyle(20);
   grPhi->SetMarkerSize(1);
   TF1 *lin2 = new TF1("lin2","[0]+([1]*x)",0,200);
   TFitResultPtr fitResultPhi;
   TF1 *phi_fit_result; // this function is only for drawing the fit result
   
// END Histo and Canvas 	////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Opening the files

   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   //cout<<" time init tracker: "<<timeinit<<endl;
   
   if (sicFileOpen) {
   treeSic->GetEntry(0);
   //cout<<" time init SiC: "<<TimestampSic<<endl;
   } else{cerr<<" Error, SiC file not found!"<<endl;}
   
   
//################### Event loop ######################################
   for(int i=0; i<entriesTracker; i++){
   //for(int i=0; i<10; i++){
      treeTracker->GetEntry(i);
      
      //if(Charge>thresh){cout<<i<<" \t"<<Board<<" \t"<<Row<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<CTS<<"\t"<<FTS<<"\t"<<Timestamp<<"\t"<<Flags<<"\t\t"<<Timestamp-timeinit+timeOffset<<endl;}
      if((Timestamp-timeinit)<DeltaT){
         //Fill histos
         if (Charge > thresh) {
            for (int k=0; k<5; ++k) {	
               if (Row==k) {
                  flag[k]=1;
                  row[k]->Fill(pad,Charge);
                  h_time[k]->Fill(pad,Timestamp-timeinit+timeOffset);                  
               }
            }
            
            //filling the charge for row from 5 to 10
            for (int k=5; k<11; ++k) {
               if (Row==k) {
                  flag[k]=1;
                  row[k]->Fill(binStrip,Charge);
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
         for (int j=5; j<11; j++) {
             cl_charge[j] = 0.;
         }
         
         np=0;
         npTime=0;
         
         theta_deg = -1000;
         phi_deg = -1000;
         theta=-1000;
         phi=-1000;
         //if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]+flag[5]+flag[6]+flag[7]+flag[8]+flag[9]+flag[10]>rowMultiplicity){
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){

	    for(int j=0; j<5; j++){
	       //cl_x[j]=0;
	       binmax = row[j]->GetMaximumBin();
	       max  = row[j]->GetBinContent(binmax);
	       //cout<<binmax<<"  "<<max<<endl;
	      
               kk=0;
	       for (int k=0; k<60; k++) {
                   charge = row[j]->GetBinContent(k);
                   time   = h_time[j]->GetBinContent(k+1);
                   //weigthed_pos[k] = (k-1)*charge/cl_charge;
	           //cl_x[j] = cl_x[j] + weigthed_pos[k];
	           timeAverage[j] += charge*time;
	           //cl_charge[j] = row[j]->Integral(0,57);
	           cl_charge[j] += charge;
	           //cout << "+++++++++++++ " << j << "\t " << k << "\t" << charge << "\t " << time << "\t " << cl_charge[j] << endl;
                   //if (charge) {cl_padMult[j]++; pads_fired[j].push_back(k);} // commented out 2024-06-26 by G.B.
                   if (charge) {cl_padMult[j]++; a_pads_fired[j][kk]=k; kk++;}  // 2024-06-26 G.B. writing the fired pads in a 2D array, one array for each row
                   
	       }
	       
	       //cout << "****** " << cl_padMult[j] << endl;
	       
 	       cl_x[j] = row[j]->GetMean();
               cl_x_rms[j] = row[j]->GetRMS();
	       cl_x_mm[j] = cl_x[j] * padWidth + padWidth/2;
	       if(max>100){grTheta->SetPoint(np++, zrow[j], cl_x_mm[j]);}

      	       timeAverage[j] = (double)(timeAverage[j]/cl_charge[j]); 
      	       cl_y[j] = timeAverage[j];    	       
      	       cl_y_mm[j] = timeAverage[j]*velocity_mm_ps;
	       //printf("timeAverage[%d] = %10.2f (ps) \t cl_y_mm[%d] = %6.2f (mm) \n\n", j, timeAverage[j], j, cl_y_mm[j]);
      	       if(max>100){grPhi->SetPoint(npTime++, zrow[j], cl_y_mm[j]);}

            }
            
            for(int j=5; j<11; j++){
               charge = row[j]->GetBinContent(1);
               cl_charge[j] = charge;
            }
                        
            //for (int q=0; q<5; ++q) {
            //    for (int h=0; h<pads_fired[q].size(); ++h)
            //        std::cout << "pads fired: " << q << "\t" << h << "\t" << pads_fired[q].at(h) << std::endl;
            //}

            // loop on the SiC file 
            if (sicFileOpen) {
               finSic->cd();
               SicLoopFlag=1;
               npTime=0;    // rezeroing the number of point of grPhi becouse can be rewritten after this point.
               TimeDiff=timeinit-TimestampSic;
               
               /*
               cout<< "*** Sic entry  "<<sicHits<<endl;
               cout<< "*** timeinit "<<timeinit<<endl;
               cout<< "*** TimestampSic "<<TimestampSic<<endl;
               cout<< "*** Time Diff "<<TimeDiff<<endl;
               cout<< "*** Time Windowlow "<<timeWindowlow<<endl;
               cout<< "*** Time Windowhigh "<<timeWindowhigh<<endl;
               */
               
               while(SicLoopFlag){

                  treeSic->GetEntry(sicHits);
                  TimeDiff=timeinit-TimestampSic;
                  //cout<< timeinit<<"  "<<TimestampSic<<endl;;
                  //cout<< "Time Diff "<<TimeDiff<<"   "<<sicHits <<endl;
                  //energySicTot->Fill(sic_charge);
                  
	          if(TimeDiff<= timeWindowlow){    // The Sic is after the Tracker, stop reading the SiC file and go further with the tracks
                     //cout << "********* Tracks without SiC" << endl;
                     tracksWithoutSic++;
                     sic_charge = -100;
                     energySic = -100;
                     ChargeSic= -100;
                     SicLoopFlag=0;
                     FlagSicStop=0;
                  }else if(TimeDiff>timeWindowlow && TimeDiff<timeWindowhigh){  // the time of SiC is compatible with the track
                     //cout << "+++++++++++ Event detected by the SiC" << endl;
                     energySic = ChargeSic;
                     sic_charge = ChargeSic;
                     tracksWithSic++;
                     sicHits++;
                     SicLoopFlag=0;
                     FlagSicStop=1;
                     for(int m=0;m<5;m++){
                        driftTime = timeAverage[m]+timeinit-timeOffset-TimestampSic;
                        //cout << "timeAverage[2]=" << timeAverage[2] << "\t TimestampSic=" << TimestampSic << "\t driftTime=" << driftTime  << endl;     
                        //timeAverage[m]=driftTime/100000;
                        cl_y[m] = driftTime;
                        cl_y_mm[m] = driftTime*velocity_mm_ps;
                        //h_driftTime[m]->Fill(driftTime/1000000); 	// drift time expressed in us
                        
                        // if there is a SiC fill again the grphi Tgraph with the new cl_y_mm considering the starting time 
                        // for the drift time from the SiC hit
                        if(max>100){grPhi->SetPoint(npTime++, zrow[m], cl_y_mm[m]);}
                        
                     }
                  }else if(TimeDiff >= timeWindowhigh) {		// the SiC is is before the Tracker, to this SiC no track can be associated.
                     sicWithoutTracks++;
                     sicHits++;
                     SicLoopFlag=1;
                     FlagSicStop=0;
                     //cout << "----------- SiC without track" << endl;
                     if (sicHits > entriesSic)
                        break;

                  }
               }

               //cout << "tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t flagTrackWithSiC " << flagTrackWithSiC << "\t flagS " << flagS << endl;
            }
	    
	    row[4]->GetYaxis()->SetRangeUser(0,max*2);
         
            fitResultTheta=grTheta->Fit("lin1","SQ");
   	    if(fitResultTheta==0){
   	       //cout<<"### TF ### "<<fitResultTheta<<endl;
       	       interceptT = fitResultTheta->Value(0);
	       slopeT = fitResultTheta->Value(1);
	       //theta_fit_result = new TF1("theta_fit_result",Form("%f+(%f*x)",interceptT,slopeT),0.,107.);
	       theta_fit_result->SetParameter(0,interceptT);
	       theta_fit_result->SetParameter(1,slopeT);	       
    	       chiSquareTheta = fitResultTheta->Chi2();
    	       //cout<<"intercept "<< interceptT <<"\t slope "<< slopeT << "\t chi2 "<< chiSquareTheta << endl;
	       theta = TMath::ATan(slopeT);
               theta_deg = theta*180./TMath::Pi();
            }else{
               theta = -1000;
               theta_deg = -1000;
            }
            
            //cout << "slope " << slopeT << "\t theta " << theta << "\t theta_deg " << theta_deg << endl;
       	    fitResultPhi=grPhi->Fit("lin2","SQ");
            if(fitResultPhi==0){
    	       interceptP = fitResultPhi->Value(0);
   	       slopeP = fitResultPhi->Value(1);
	       chiSquarePhi = fitResultPhi->Chi2();
               phi = TMath::ATan(slopeP);
	       phi_deg = phi*180./TMath::Pi();
            }else{
               phi = -1000;
               phi_deg = -1000;
            }
            
            if(FlagSicStop==1 && flagM==1 ){           

               if(np>0){
                   //grTheta->Draw("P");
                   //grTheta->Fit("lin1","Q");
                   //theta_fit_result->Draw("same");
                   //cout << "\n\n +++++++++++++++++++++++++++++++++++ theta_fit_result formula: " << theta_fit_result->GetExpFormula() << "\n\n" << endl;
               }
          
            }

            //for (int q=0; q<5; ++q) {
                //for (int h=0; h<cl_padMult[q]; ++h)
                    //std::cout << "------------  pads fired: " << q << "\t" << h << "\t" << a_pads_fired[q][h] << std::endl;
            //}
            
            treeOut->Fill();
            //cout << "Filling the tree" << endl;
            if (sicFileOpen){ 
               //cout << "Tracks without SiC " << tracksWithoutSic << "\t tracks with SiC " << tracksWithSic << "\t SiC without tracks " << sicWithoutTracks << endl;
               }
            else {
               //cout << "Tracks " << tracksWithoutSic << endl;
               }
            /*
            cout << "########## Sic entry  "<<sicHits<<endl;
            cout << "########## Time track "<< timeinit<<endl;
            cout << "########## Time Sic   "<< TimestampSic<<endl;
            cout << "########## time diff  "<< timeinit-TimestampSic<<endl;
            cout << "########## TimeDiff   "<< TimeDiff<<endl;
            */
	 }

         
	 // Start a new Event
	 timeinit=Timestamp;
	 grTheta->Set(0);
	 grPhi->Set(0);
	
   	 for(int j=0;j<5; j++){
	    row[j]->Reset("ICES");
	    h_time[j]->Reset("ICES");
	    flag[j]=0;
         }
         for(int j=5;j<11; j++){
	    row[j]->Reset("ICES");
	 }
         if (Charge > thresh) {
            for (int k=0; k<5; ++k) {
               if (Row==k) {
                  flag[k]=1;
                  row[k]->Fill(pad,Charge);
                  h_time[k]->Fill(pad,Timestamp-timeinit+timeOffset);
               }
            }
         }
      }
   }
//################### END Event loop ######################################
   


   //cout << "Col sic: " << h_angles[2]->Integral(0,nbinalpha) << endl;   
   //cout<<"--------------------------------------"<<endl;
   //cout<<" tracksCounter         "<<tracksCounter<<endl;
   //cout<<" trackNoSicsCounter    "<<trackNoSicsCounter<<endl;
   //cout<<" tracksSiCCounter      "<<tracksSiCCounter<<endl;
   //cout<<" SiCCounter   	 "<<SiCCounter<<endl;
   //cout<<" EventEntries   	 "<<EventEntries<<endl; 
   //cout<<"--------------------------------------"<<endl;
      

   //cout << "Before writing file output" << endl;
   fileOut->cd();
   treeOut->Write();
   fileOut->Purge();
   fileOut->Close();


}
