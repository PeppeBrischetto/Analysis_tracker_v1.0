//###################################################################################################
//#   Purpose: macro that writes a third level root file containing a tree with the theta and phi angles of the tracks,
//#   as well as information on the centroid, rms, drift times for each row.
//#   it takes as input a single file from a single channel of SiC and a 
//#   merged file from the tracker.  
//#   make a fit of five clusters of a track and return the slope. 
//#   it can plot the histograms with the angles of the tracks, but it is not necessary
//#   
//#   required as argument the run number
//#           * as input the DeltaT, time interval that is used to define an event, and the Time window
//#            		where to seek the tracks.
//# 	       			
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created may 2024 from B_anglesFinder_plot_tracker_and_sic_v4.C by G. Brischetto
//#######################################
//#   updated: 6 june 2024 commenting 
//# 			   renaming of variables  
//#			   solved bug on track without sic    D. Torresi
//# 
//###################################################################################################

void B_trackGenerator(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

  // setted parameters
   // window that define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;

   Float_t timeWindowlow = 0.5E+06; // time (in ps) that the primary electrons need to reach the anode
   Float_t timeWindowhigh = 2.0E+06;

   Double_t timeOffset = 10.; // time (in ps) used to avoid that the time difference Timestamp-timeinit 
                              // is equal to zero for the first event entry (see where time histos are filled)
                              // This is important when you want to calculate the time average weighted by the charge

   Double_t padWidth = 5.; // pad width in mm

   Double_t velocity = 50.;  // drift velocity (in mm/us) of electrons in isobutane;  
   Double_t velocity_mm_ps = velocity/1.0E+06;  // drift velocity (in mm/ps) of electrons in isobutane;

   double *zrow = new double[5];  // zcoordinate (in mm) of the row
   zrow[0]=18.60;		  // valid for the prototype 2
   zrow[1]=39.80;
   zrow[2]=61.00;
   zrow[3]=82.20;
   zrow[4]=103.40;
   
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
   double cl_charge[5] = {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5] = {0};		// y centroid of a cluster in time
   double cl_y_mm[5] = {0};		// y centroid of a cluster in mm
   Double_t theta=-1000;		// theta of the track in rad
   Double_t theta_deg=-1000;		// theta of the track in deg
   Double_t phi=-1000;
   Double_t phi_deg=-1000;
   Double_t chiSquareTheta;
   Double_t chiSquarePhi;
   Double_t sic_charge;
   Double_t energySic=-1000; 
  
   //Int_t pads_fired[5][60]={-1000};
   //std::vector<Int_t> pads_fired0, pads_fired1, pads_fired2, pads_fired3, pads_fired4;
   std::vector<Int_t> pads_fired[5];
   //for (Int_t i=0; i<5; ++i) { 
   //    pads_fired[i] = {-1000};
   //    cout << pads_fired[i] << endl;
   //}

// other variables
   
   // fitting variables
   double slope, intercept, chi2;
   double charge = 0.0;
   double weigthed_pos[60];
   Double_t alpha=-1000, alpha_deg=-1000;  // auxiliary angle for the calculation of theta
   Double_t beta=-1000, beta_deg=-1000;    // auxiliary angle for the calculation of phi

   // Secondary variables  
   ULong64_t TimestampSicTemp;   
   UInt_t SicLoopFlag;			// variable used to stop the loop on the Sic file

   Double_t timeAverage[5] = {0.}; 	// variable used to calculate the average time (weighted by the charge) on a raw
   Double_t time = 0.;             	// variable used to store the time of a single pad (it is used for the calculation of the average time
   Double_t driftTime = 0.;        	// variable used to store the drift time of electrons corresponding to  row 2 (i.e. driftTime=timeSiC-timeRow2)

   int binmax=-100, max=-100;		// variables used to plot the histos
   // number of event in the run

   UInt_t flagTrackWithSiC=0;  
   UInt_t rowMultiplicity=4; 	// consider tracks with a number of hit row bigger than rowMultiplicity
   
   char anykey;		// variable used to pause the macro
   int flag[5];		// variable that are true when there is at least one hit in the Row
   for(int i =0; i<5; i++) flag[i]=0;
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

// END: Dichiarazione variabili		//////////////////////////////////////

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
   cout<<"Entries tracker file "<< entriesTracker <<endl;
// END: open tracker file	//////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////   
// open Sic file
   char fileInSic[50];
   if(run<10){
      sprintf(fileInSic, "../Merged_data/run_00%i/sic_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileInSic, "../Merged_data/run_0%i/sic_0%i.root", run, run);
   }else{
      sprintf(fileInSic, "../Merged_data/run_%i/sic_%i.root", run, run);
   } 
   cout<<fileInSic<<endl;
   TFile *finSic = new TFile(fileInSic);
   TTree *treeSic = (TTree*)finSic->Get("Data_R");
   
   treeSic->SetBranchAddress("Board",&BoardSic);
   treeSic->SetBranchAddress("Channel",&ChannelSic);
   treeSic->SetBranchAddress("FineTSInt",&FTSSic);
   treeSic->SetBranchAddress("CoarseTSInt",&CTSSic);
   treeSic->SetBranchAddress("Timestamp",&TimestampSic);
   treeSic->SetBranchAddress("Charge",&ChargeSic);
   treeSic->SetBranchAddress("Flags",&FlagsSic);
   treeSic->SetBranchAddress("Charge_cal",&Charge_calSic);
   
   int entriesSic=treeSic->GetEntries();
   cout<<"Entries sic file "<< entriesSic <<endl;
// END: open SiC file	//////////////////////////////////////////////////////

// OPEN output ROOT file //
   char fileOutName[50];
   sprintf(fileOutName,"../Tracks/tracks_run%i.root",run);
   TFile *fileOut = new TFile(fileOutName, "recreate");
   TTree *treeOut = new TTree("Data_R", "Third level tree");
   
   treeOut->Branch("cl_x", cl_x, "cl_x[5]/D");
   treeOut->Branch("cl_x_mm", cl_x_mm, "cl_x_mm[5]/D"); 
   treeOut->Branch("cl_y", timeAverage, "cl_y[5]/D");
   treeOut->Branch("cl_y_mm", cl_y_mm, "cl_y_mm[5]/D");
   treeOut->Branch("cl_x_rms", cl_x_rms, "cl_x_rms[5]/D");
   treeOut->Branch("cl_padMult",cl_padMult,"cl_padMult[5]/I");
   treeOut->Branch("cl_charge", cl_charge, "cl_charge[5]/D");
   //treeOut->Branch("cl_padMult0",&cl_padMult[0],"cl_padMult0/I");
   //treeOut->Branch("cl_padMult1",&cl_padMult[1],"cl_padMult1/I");
   //treeOut->Branch("pads_fired0",&pads_fired[0],"pads_fired0[cl_padMult0]/I");
   //treeOut->Branch("pads_fired1",&pads_fired[1],"pads_fired1[cl_padMult1]/I");
   treeOut->Branch("phi",&phi,"phi/D");
   treeOut->Branch("theta",&theta,"theta/D");
   treeOut->Branch("phi_deg",&phi_deg,"phi_deg/D");      
   treeOut->Branch("theta_deg",&theta_deg,"theta_deg/D");
   treeOut->Branch("chiSquareTheta",&chiSquareTheta,"chiSquareTheta/D");   
   treeOut->Branch("chiSquarePhi",&chiSquarePhi,"chiSquarePhi/D");      
   treeOut->Branch("sic_fired",&FlagSicStop,"sic_fired/I");
   treeOut->Branch("sic_charge",&sic_charge,"sic_charge/D");
   treeOut->Branch("energySic",&energySic,"energySic/D");

//////////////////////////////////////////////////////////////////////////////
// Dichiarazione Histo, Canvas, TGraph and Functions
   TH1F *row[5];
   for (int i=0; i<5; ++i){
       sprintf(dummyString,"r%i",i);
       row[i]=new TH1F(dummyString,dummyString,62,-0.5,61.5);
   }

   TH1D *h_time[5];
   for (int i=0; i<5; ++i) {
       sprintf(dummyString,"h_time%i",i);
       h_time[i]=new TH1D(dummyString,dummyString,64,-0.5,63.5);
   }

   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   
   for(int i=0;i<5; i++){
     h_time[i]->GetXaxis()->SetTitle("pad");
     h_time[i]->GetYaxis()->SetTitle("timestamp");
   }
   h_time[1]->SetLineColor(kRed);

   TH1D *h_driftTime[5];
   for(int i=0;i<5;i++){
      sprintf(dummyString,"h_driftTime%i",i); 
      h_driftTime[i] = new TH1D(dummyString,dummyString,500,0.5E+00,5.0E+00);
      h_driftTime[i]->GetXaxis()->SetTitle("driftTime (us)");
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
   }
 
  
   // Fitting TGraphs and functions
   TH2D *bg= new TH2D("bg","bg",130,70,200,107, 0,107);
   //fit x vs z  
   TGraph *grTheta=new TGraph(0);
   grTheta->SetMarkerStyle(20);
   grTheta->SetMarkerSize(1);
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,300);
   TFitResultPtr fitResultTheta;

   //fit y vs z
   TGraph *grPhi=new TGraph(0);
   grPhi->SetMarkerStyle(20);
   grPhi->SetMarkerSize(1);
   TF1 *lin2 = new TF1("lin2","[0]+([1]*x)",0,200);
   TFitResultPtr fitResultPhi;

   TCanvas *C1=new TCanvas("c1","alpha",900.,800.);
   C1->SetFillColor(kWhite);
   C1->SetLogy();
   TCanvas *C2=new TCanvas("C2","C2",900.,800.);
   bg->Draw();
   bg->SetStats(0);
   TCanvas *C3=new TCanvas("C3","theta",900.,800.);
   C3->SetFillColor(kWhite);
   C3->SetLogy();
   TCanvas *C4=new TCanvas("C4","C4",900.,800.);
   C4->SetFillColor(kWhite);
   TCanvas *C5=new TCanvas("C5","C5",900.,800.);
   C5->SetFillColor(kWhite);
   TCanvas *C6 = new TCanvas("C6","C6",900.,800.);
   C6->SetFillColor(kWhite);
   
// END Histo and Canvas 	////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Opening the files

   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init tracker: "<<timeinit<<endl;
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<TimestampSic<<endl;
   
   for(int i=0; i<entriesTracker; i++){
   //for(int i=0; i<50; i++){
      treeTracker->GetEntry(i);
      //if (i%1000==0) cout << "Entry: " << i << endl;
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
         }
      } else {
      	 // The event is finished. Plot if there is something
      	  //cout << "\n-------- Event finished ---------\n" << endl;

         //cl_charge = 0.;
      	 for (int j=0; j<5; j++) {
      	     timeAverage[j] = 0.;
      	     cl_charge[j] = 0.;
             cl_padMult[j] = 0;
             pads_fired[j].clear();
         }

         //for(int q=0; q<5; ++q) {
         //   std::cout << "*** " << q << "\t" << pads_fired[q].size() << std::endl;
         //   for(int h=0; h<pads_fired[q].size(); ++h)
         //   std::cout << "+++" << q << "\t" << h << "\t" << pads_fired[q].at(h) << std::endl;
         //}
         
         theta_deg = -1000.;
         phi_deg = -1000.;
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){
            np=0;
            npTime=0;
	    for(int j=0; j<5; j++){
	       //cl_x[j]=0;
	       binmax = row[j]->GetMaximumBin();
	       max  = row[j]->GetBinContent(binmax);
	       //cout<<binmax<<"  "<<max<<endl;
	       for (int k=0; k<60; k++) {
                   charge = row[j]->GetBinContent(k);
                   time   = h_time[j]->GetBinContent(k);
                   cl_charge[j] = row[j]->Integral(0,59);
	           //weigthed_pos[k] = (k-1)*charge/cl_charge;
	           //cl_x[j] = cl_x[j] + weigthed_pos[k];
	           timeAverage[j] += charge*time;
	           //cout << "+++++++++++++ " << j << "\t " << k << "\t" << charge << "\t " << time << "\t " << cl_charge[j] << endl;
                   if (charge) {cl_padMult[j]++; pads_fired[j].push_back(k);} 
	       }
 	       cl_x[j] = row[j]->GetMean();
               cl_x_rms[j] = row[j]->GetRMS();
	       cl_x_mm[j] = cl_x[j] * 5 + padWidth/2.;
	       if(max>100){grTheta->SetPoint(np++, zrow[j], cl_x_mm[j]);}

      	       timeAverage[j] = (double)(timeAverage[j]/cl_charge[j]);
      	       cl_y_mm[j] = timeAverage[j]*velocity_mm_ps;
	       //printf("timeAverage[%d] = %10.2f (ps) \t cl_y_mm[%d] = %6.2f (mm) \n\n", j, timeAverage[j], j, cl_y_mm[j]);
      	       if(max>100){grPhi->SetPoint(npTime++, zrow[j], cl_y_mm[j]);}

            }
            
            //for (int q=0; q<5; ++q) {
                //for(int h=0; h<pads_fired[q].size(); ++h)
                //std::cout << "pads fired: " << q << "\t" << h << "\t" << pads_fired[q].at(h) << std::endl;
            //}

            // loop on the SiC file 
            finSic->cd();
            SicLoopFlag=1;
            while(SicLoopFlag){
            //cout << "Inside the while" << endl;             
            treeSic->GetEntry(sicHits);

	       if(TimestampSic>=(timeinit-timeWindowlow)){    // The Sic is after the Tracker, stop reading the SiC file and go further with the tracks
                  //cout << "********* Tracks without SiC" << endl;
                  tracksWithoutSic++;
                  energySic = -1000.;
                  ChargeSic= -1000;
                  SicLoopFlag=0;
                  FlagSicStop=0;
               }else if((timeinit-TimestampSic)>timeWindowlow && (timeinit-TimestampSic)<timeWindowhigh){  // the time of SiC is compatible with the track
                  cout << "+++++++++++ Event detected by the SiC" << endl;
                  energySic = ChargeSic;
                  sic_charge = ChargeSic;
                  tracksWithSic++;
                  sicHits++;
                  SicLoopFlag=0;
                  FlagSicStop=1;
                  for(int m=0;m<5;m++){
                     driftTime = timeAverage[m]+timeinit-timeOffset-TimestampSic;
                     //cout << "timeAverage[2]=" << timeAverage[2] << "\t TimestampSic=" << TimestampSic << "\t driftTime=" << driftTime  << endl;
                     h_driftTime[m]->Fill(driftTime/1000000.);
                  }
               }else if(TimestampSic <= (timeinit-timeWindowhigh) ) {		// the SiC is is before the Tracker, to this SiC no track can be associated.
                  sicWithoutTracks++;
                  sicHits++;
                  SicLoopFlag=1;
                  FlagSicStop=0;
                  cout << "----------- SiC without track" << endl;
               }
            }
            //cout << "tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t flagTrackWithSiC " << flagTrackWithSiC << "\t flagS " << flagS << endl;
	    row[4]->GetYaxis()->SetRangeUser(0,max*2);

	    C4->cd();
         
   	    fitResultTheta=grTheta->Fit("lin1","S");
   	    if(fitResultTheta==0){
   	       cout<<"### TF ### "<<fitResultTheta<<endl;
       	       intercept = fitResultTheta->Value(0);
	       slope = fitResultTheta->Value(1);
    	       chiSquareTheta = fitResultTheta->Chi2();
    	       cout<<"chi2 "<<chiSquareTheta<<endl;
               
	       theta = TMath::ATan(slope);
               theta_deg = theta*180./TMath::Pi();
            }else{
               theta = -1000;
               theta_deg = -1000;
            }
            //cout << "slope " << slope << "\t theta " << theta << "\t theta_deg " << theta_deg << endl;
       	    fitResultPhi=grPhi->Fit("lin2","S");
            if(fitResultPhi==0){
    	       intercept = fitResultPhi->Value(0);
   	       slope = fitResultPhi->Value(1);
	       chiSquarePhi = fitResultPhi->Chi2();
               
               phi = TMath::ATan(slope);
	       phi_deg = phi*180./TMath::Pi();
            }else{
               phi = -1000;
               phi_deg = -1000;
            }
            
            if(FlagSicStop==1 && flagM==1 ){           
               C2->cd(0);
               if(np>0){
                  grTheta->Draw("P");
                  grTheta->Fit("lin1","Q");
               }
                            
               cout<<"press any key to continue, q to quit, c to continue till the end"<<endl; 
               if(flagM)
                  cin>>anykey;
            }

            if(anykey=='q'){ 
              fileOut->Write();
              fileOut->Close();
              return; // Per uscire dal programma
            }

      	    if(anykey=='c')flagM=0;     

            treeOut->Fill();
            //cout << "Filling the tree" << endl;
            cout << "Tracks without SiC " << tracksWithoutSic << "\t tracks with SiC " << tracksWithSic << "\t SiC without tracks " << sicWithoutTracks << endl;
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

   C1->cd();
   h_alpha[0]->Draw();
   h_alpha[1]->Draw("same");
   h_alpha[2]->Draw("same");
   C3->cd();
   h_angles[0]->Draw();
   h_angles[1]->Draw("same");
   h_angles[2]->Draw("same");
   TLegend *leg1 = new TLegend(0.75,0.8,0.85,0.85);
   leg1->AddEntry(h_angles[2],"Tracks hitting the SiC","L");
   leg1->AddEntry(h_angles[1],"Tracks not hitting the SiC","L");
   leg1->AddEntry(h_angles[0],"Sum","L");
   leg1->Draw("same");
   C3->Update();
   C5->cd();
   h_angles[3]->Draw();
   h_angles[4]->Draw("same");
   h_angles[5]->Draw("same");
   TLegend *leg2 = new TLegend(0.75,0.8,0.85,0.85);
   leg2->AddEntry(h_angles[5],"Tracks hitting the SiC","L");
   leg2->AddEntry(h_angles[4],"Tracks not hitting the SiC","L");
   leg2->AddEntry(h_angles[3],"Sum","L");
   leg2->Draw("same");
   C5->Update();
   C6->cd();
   for(int i=0;i<5;i++)
      h_driftTime[i]->Draw("same");

   TLegend *leg3 = new TLegend(0.80,0.65,0.90,0.85);
   for(int i=0;i<5;i++){
      sprintf(dummyString,"Row %i",i);
      leg3->AddEntry(h_driftTime[i],dummyString,"L");
   }
   leg3->Draw("same");
   //cout << "Col sic: " << h_angles[2]->Integral(0,nbinalpha) << endl;

   cout << "Before writing file output" << endl;
   fileOut->cd();
   treeOut->Write();
   fileOut->Purge();
   fileOut->Close();


}
