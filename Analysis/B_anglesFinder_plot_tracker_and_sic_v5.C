//###################################################################################################
//#   macro that take as input a single file from a single channel of SiC and a 
//#   merged file from the tracker.  
//#   make a fit of five clusters of a track and return the slop. Plot the angles of the tracks.
//#   print on screen some information and counts also the number of tracks with and without a Sic hit    
//#   The macro has been created with the purpose to check the data not for extensive analyis.
//#
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event, and the Time window
//#            where to seek the tracks.
//# 	       The array with the number of digitizers used array with the number of digitizers used
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created april 2024 from A_eventFinder_plot.C by G. Brischetto
//#######################################
//#   updated: 30-04-2024  cleaning, comments and correction D. Torresi
//#   updated: 08-10-2024  corrected in particular the values of the z coordinate in relation with
//#			   the reference frame. Adjusted all the plot and the variables for counting
//#	 		   event in coincdence with Sic choosen and the size of the plot bg.
//#			    D. Torresi
//###################################################################################################

void B_anglesFinder_plot_tracker_and_sic_v5(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

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

    // tracker variables 
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

   // SiC variables 
   UShort_t ChannelSic; 
   UShort_t FTSSic;
   ULong64_t CTSSic;
   ULong64_t TimestampSic;
   UShort_t BoardSic;
   UShort_t ChargeSic;
   Double_t Charge_calSic;      
   UInt_t FlagsSic;
   
   // fitting variables
   double slope,intercept;
   double charge = 0.0;
   double total_charge = 0.0;
   double weigthed_pos[60];
   double centroid[5], centroid_mm[5];

   // Secondary variables  
   ULong64_t TimestampSicTemp;   
   ULong64_t TimestampTrackerEv;	// variables used for checks
   UInt_t SicLoopFlag;			// variable used to stop the loop on the Sic file
   Long64_t TimeDiff;                   // variable needed to properly calculate the difference between timeinit and timestampSic, otherwise it is not a Long64 but maybe an Int

   Double_t timeAverage[5] = {0.}; // variable used to calculate the average time (weighted by the charge) on a raw
   Double_t time = 0.;             // variable used to store the time of a single pad (it is used for the calculation of the average time
   Double_t driftTime = 0.;        // variable used to store the drift time of electrons corresponding to  row 2 (i.e. driftTime=timeSiC-timeRow2)

   int binmax=-100, max=-100;		// variables used to plot the histos
   // number of event in the run
   
   Double_t theta=-1000., theta_deg=-1000.;
   Double_t alpha=-1000., alpha_deg=-1000.;  // auxiliary angle for the calculation of theta
   Double_t phi=-1000., phi_deg=-1000.;
   Double_t beta=-1000., beta_deg=-1000.;    // auxiliary angle for the calculation of phi

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

   double *zrow = new double[5];  // zcoordinate (in mm) of the row from the begnning of the active volume
   zrow[0]=18.60-7;		  // valid for the prototype 2
   zrow[1]=39.80-7;
   zrow[2]=61.00-7;
   zrow[3]=82.20-7;
   zrow[4]=103.40-7;

   double yrow[5] = {0.};

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
   
   TH1D *chargeSiC[3];
   chargeSiC[0]=new TH1D("all Sic"," ",2000,0,20000);
   chargeSiC[1]=new TH1D("Tracker"," ",2000,0,20000);
   chargeSiC[2]=new TH1D("noTr"," ",2000,0,20000);
   
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
 
   TH2D *bg= new TH2D("track","track",300,00,300,107, 0,107);
   bg->GetXaxis()->SetTitle("X (mm)");
   bg->GetYaxis()->SetTitle("Z (mm)");
   TGraph *grTrack=new TGraph(0);
   grTrack->SetMarkerStyle(20);
   grTrack->SetMarkerSize(1);
   
   TH2D *bg2= new TH2D("trackV","trackV",107,00,107,150, -75,75);
   bg2->GetXaxis()->SetTitle("X (mm)");
   bg2->GetYaxis()->SetTitle("Y (mm)");
   TGraph *grPhi=new TGraph(0);
   grPhi->SetMarkerStyle(20);
   grPhi->SetMarkerSize(1);

   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,300);
   TF1 *fit_result;

   TF1 *lin2 = new TF1("lin2","[0]+([1]*x)",0,200);

   TCanvas *C1=new TCanvas("c1","alpha",900.,50.,900,900);
   C1->SetFillColor(kWhite);
   C1->SetLogy();
   TCanvas *C2=new TCanvas("C2","C2",920.,250.,1960,700);
   bg->Draw();
   bg->SetStats(0);
   TCanvas *C3=new TCanvas("C3","theta",940.,450.,900,900);
   C3->SetFillColor(kWhite);
   C3->SetLogy();
   TCanvas *C4=new TCanvas("C4","C4",1400.,50.,450,900);
   C4->SetFillColor(kWhite);
   bg2->Draw();
   bg2->SetStats(0);
   TCanvas *C5=new TCanvas("C5","C5",1420.,250.,900,900);
   C5->SetFillColor(kWhite);
   TCanvas *C6 = new TCanvas("C6","C6",1440.,450.,900,900);
   C6->SetFillColor(kWhite);
   TCanvas *C7 = new TCanvas("C7","C7",1640.,50.,900,900);
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
   
   for(int i=0; i<entriesTracker/10; i++){
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
      	 // cout << "\n-------- Event finished ---------\n" << endl;

         total_charge = 0.;
      	 for (int j=0; j<5; j++)
      	     timeAverage[j] = 0.;
      	     
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){
            
            eventNumber++;
            np=0; 
            npTime=0;
	    for(int j=0; j<5; j++){ 
	       //centroid[j]=0;
	       binmax = row[j]->GetMaximumBin(); 
	       max  = row[j]->GetBinContent(binmax);
	       //cout<<binmax<<"  "<<max<<endl;
	       for (int k=0; k<60; k++) {
                   charge = row[j]->GetBinContent(k);
                   time   = h_time[j]->GetBinContent(k);
                   total_charge = row[j]->Integral(0,59);
	           //weigthed_pos[k] = (k-1)*charge/total_charge;
	           //centroid[j] = centroid[j] + weigthed_pos[k];
	           timeAverage[j] += charge*time;
	           //cout << "+++++++++++++ " << j << "\t " << k << "\t" << charge << "\t " << time << "\t " << total_charge << endl;
	       }
      
 	       centroid[j] = row[j]->GetMean();
	       centroid_mm[j] = centroid[j] * 5 + padWidth/2.;
	       if(max>100){grTrack->SetPoint(np++, centroid_mm[j], zrow[j]);}
	       
      	       timeAverage[j] = (double)(timeAverage[j]/total_charge);
      	       yrow[j] = timeAverage[j]*velocity_mm_ps;
	       //printf("timeAverage[%d] = %10.2f (ps) \t yrow[%d] = %6.2f (mm) \n\n", j, timeAverage[j], j, yrow[j]);
      	       if(max>100){grPhi->SetPoint(npTime++, zrow[j], yrow[j]);}

            }
            
            // loop on the SiC file 
            finSic->cd();
            SicLoopFlag=1;
                        
            while(SicLoopFlag){ 
               treeSic->GetEntry(sicHits);
               TimeDiff=timeinit-TimestampSic;
                /*
               cout<< "*** Sic entry  "<<sicHits<<endl;
               cout<< "*** timeinit "<<timeinit<<endl;
               cout<< "*** TimestampSic "<<TimestampSic<<endl;
               cout<< "*** Time Diff "<<TimeDiff<<endl;
               cout<< "*** Time Windowlow "<<timeWindowlow<<endl;
               cout<< "*** Time Windowhigh "<<timeWindowhigh<<endl;
               */
               
	       if (TimeDiff<= timeWindowlow){    // The Sic is after the Tracker, stop reading the SiC file and go further with the tracks
                  tracksWithoutSic++;
                  SicLoopFlag=0;
                  FlagSicStop=0;  
               }else if(TimeDiff>timeWindowlow && TimeDiff<timeWindowhigh){  // the time of SiC is compatible with the track
                  cout << "+++++++++++ Event detected by the SiC" << endl;
                  tracksWithSic++;
                  sicHits++;
                  SicLoopFlag=0;
                  FlagSicStop=1;
                  chargeSiC[0]->Fill(ChargeSic);	
                  chargeSiC[1]->Fill(ChargeSic);	
                  for(int m=0;m<5;m++){
                     driftTime = timeAverage[m]+timeinit-timeOffset-TimestampSic;
                     //cout << "timeAverage[2]=" << timeAverage[2] << "\t TimestampSic=" << TimestampSic << "\t driftTime=" << driftTime  << endl;
                     h_driftTime[m]->Fill(driftTime/1000000);
                  }
               }else if(TimeDiff >= timeWindowhigh) {		// the SiC is is before the Tracker, to this SiC no track can be associated.
                  
                  sicWithoutTracks++;
                  chargeSiC[0]->Fill(ChargeSic);	
                  chargeSiC[2]->Fill(ChargeSic);	
                  sicHits++;
                  SicLoopFlag=1;
                  FlagSicStop=0;
                  cout << "----------- Event without SiC" << endl;
               }
            }
            cout <<"entriesTracker "<< eventNumber  <<"\t tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t sicWithoutTrack " << sicWithoutTracks <<endl;
	    
	    row[4]->GetYaxis()->SetRangeUser(0,max*2);
	    C4->cd();
            //h_time[0]->Draw("HIST");
            //h_time[1]->Draw("HIST same");
	    
   	    grTrack->Fit("lin1","RQ");
    	    intercept = lin1->GetParameter(0);
	    slope = lin1->GetParameter(1);
   	    //alpha = TMath::ATan(slope);
	    //alpha_deg = alpha*180./TMath::Pi();
	    theta = TMath::ATan(slope);
            theta_deg = theta*180./TMath::Pi();
	    //if(theta_deg<-90){
	    //   theta_deg=-90-alpha_deg+180;
	    //}
            //cout << "slope " << slope << "\t theta " << theta << "\t theta_deg " << theta_deg << endl;
       	    grPhi->Fit("lin2","RQ");
    	    intercept = lin2->GetParameter(0);
	    slope = lin2->GetParameter(1);
   	    //beta = TMath::ATan(slope);
	    //beta_deg = beta*180./TMath::Pi();
	    //phi_deg=90-beta_deg;
            //if (beta_deg<0) {
            //   phi_deg=
            //}
            phi = TMath::ATan(slope);
	    phi_deg = phi*180./TMath::Pi();
            //cout << "\n\n alpha_deg = " << alpha_deg << "\t theta_deg = " << theta_deg << "\t beta_deg = " << beta_deg << "\t phi_deg = " << phi_deg << "\n\n" << endl;
            //cin >> anykey;
            
	    //h_alpha[0]->Fill(alpha_deg);	       
	    h_angles[0]->Fill(theta_deg);
            h_angles[3]->Fill(phi_deg);
            if(FlagSicStop==0){
               //h_alpha[1]->Fill(alpha_deg);
      	       h_angles[1]->Fill(theta_deg);
               h_angles[4]->Fill(phi_deg);
            }else{
               //h_alpha[2]->Fill(alpha_deg);
               h_angles[2]->Fill(theta_deg);
               h_angles[5]->Fill(phi_deg);
               //cout << "alpha_deg " << alpha_deg << endl;
               //cin >> anykey;
            }  
            
            if(FlagSicStop==1 && flagM==1 ){           
               C2->cd(0);
               if(np>0){
                  grTrack->Draw("P");
                  grTrack->Fit("lin1","Q");
               }
               
               C2->Update();
               C4->cd(0);
               if(np>0){
                  grPhi->Draw("P");
                  grPhi->Fit("lin2","Q");
               }   
               C4->Update();
               //cout << "alpha_deg " << alpha_deg << endl;
               //cout << "slope " << slope << endl;
               //cout << lin1->GetParameter(1)<<"  "<<90-abs(atan(lin1->GetParameter(1))*180/3.1415)<<endl;
              
               cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl; 
               if(flagM)
                  cin>>anykey;
            }
 
            if(anykey=='q') return; // Per uscire dal programma
            if(anykey=='s'){ 		 // Salvi il plot
              C2->Print("c2.eps");
  	    }
      	    if(anykey=='c')flagM=0;     
                
	 }
	 	 
	 // Start a new Event
	 timeinit=Timestamp;
	 grTrack->Set(0);
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

   C7->cd();
   chargeSiC[0]->Draw(); 
   chargeSiC[0]->GetXaxis()->SetTitle("charge");
   chargeSiC[0]->GetYaxis()->SetTitle("counts");
   chargeSiC[0]->SetLineColor(kBlack); 
   chargeSiC[1]->Draw("same");  
   chargeSiC[1]->SetLineColor(kBlue);
   chargeSiC[2]->Draw("same");  
   chargeSiC[2]->SetLineColor(kRed);
   TLegend *leg4 = new TLegend(0.80,0.65,0.90,0.85);
   leg4->AddEntry(chargeSiC[0],"all","L");
   leg4->AddEntry(chargeSiC[1],"with SiC","L");
   leg4->AddEntry(chargeSiC[2],"without SiC","L");
   leg4->Draw("same"); 
      
}
