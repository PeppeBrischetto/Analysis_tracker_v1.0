//###################################################################################################
//#   macro that take as input a single file from a single channel of SiC and a 
//#   merged file from the tracker.  
//#   make a fit of five clusters of a track and return the slop. 
//#   Plot the angles of the tracks.
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event, and the Time window
//#            where to seek the tracks.
//# 	       The array with the number of digitizers used			   array with the number of digitizers used
//#   alpha is the angle taken as atan of the slope of the linear fit 
//#   theta instead is the angle respect to the z-axis 
//#   theta is defined as  theta=-90-alpha  (it is negative since the x goes from rigth to left).
//###################################################################################################
//#   created april 2024 from A_eventFinder_plot.C by G. Brischetto
//#######################################
//#   updated: 30-4-2024  cleaning, comments and correction D. Torresi
//# 
//###################################################################################################

void B_anglesFinder_plot_tracker_and_sic_v2(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

   // window that define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;

   Float_t timeWindowlow = 0.5E+06; // time (in ps) that the primary electrons need to reach the anode
   Float_t timeWindowhigh = 2.0E+06; 

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
   UInt_t SicLoopFlag;	// variable used to stop the loop on the Sic file
   
   int binmax=-100, max=-100;		// variables used to plot the histos
   // number of event in the run
   
   Double_t theta=-1000., theta_deg=-1000.;
   Double_t alpha=-1000., alpha_deg=-1000.;
   Double_t phi=-1000., phi_deg=-1000.;

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

   double *zrow = new double[5];  // zcoordinate of the row
   zrow[0]=18.60;		  // valid for the prototype 2
   zrow[1]=39.80;
   zrow[2]=61.00;
   zrow[3]=82.20;
   zrow[4]=103.40;

   char dummyString[50];

// END: Dichiarazione variabili		//////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////   
// open tracker file
   char fileInTracker[50];
   sprintf(fileInTracker, "../Merged_data/run_00%i/merg_00%i.root", run, run);
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
   sprintf(fileInSic, "../Merged_data/run_00%i/sic_00%i.root", run, run);
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

   TH1D *time[5];
   for (int i=0; i<5; ++i) {
       sprintf(dummyString,"time%i",i);
       time[i]=new TH1D(dummyString,dummyString,1.0E+06,-0.5,1.0E+12);
   }

   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   
   Int_t nbintheta=3600;
   Int_t nbinalpha=3600;   
   Int_t nbinphi=50;
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
       else h_angles[k]->GetXaxis()->SetTitle("#phi (deg)");
       h_angles[k]->GetYaxis()->SetTitle("counts");
       h_angles[k]->SetLineColor(k+1);
      
   } 
 
   TH2D *bg= new TH2D("bg","bg",130,70,200,107, 0,107);
   TGraph *grTrack=new TGraph(0);
   grTrack->SetMarkerStyle(20);
   grTrack->SetMarkerSize(1);

   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,300);
   TF1 *fit_result;


   TCanvas *C1=new TCanvas("c1","alpha",900.,800.);
   C1->SetFillColor(kWhite);
   C1->SetLogy();
   TCanvas *C2=new TCanvas("C2","C2",900.,800.);
   bg->Draw();
   bg->SetStats(0);
   TCanvas *C3=new TCanvas("C3","theta",900.,800.);
   C3->SetFillColor(kWhite);
   C3->SetLogy();
   
// END Histo and Canvas 	////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Opening the files

   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init tracker: "<<timeinit<<endl;
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<TimestampSic<<endl;
   
   for(int i=0; i<entriesTracker/100; i++){
      treeTracker->GetEntry(i);   
      //if (i%1000==0) cout << "Entry: " << i << endl;
      if(Charge>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
         //Fill histos
         if (Charge > thresh) {
            for (int k=0; k<5; ++k) {
                if (Row==k) {
                   flag[k]=1;
                   row[k]->Fill(pad,Charge);
                   time[k]->Fill(pad,timeinit);
                }
            }
         }
      } else {
      	 // The event is finished plot if there is something 
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){
            np=0; 
	    for(int j=0; j<5; j++){ 
	       //centroid[j]=0;
	       binmax = row[j]->GetMaximumBin(); 
	       max  = row[j]->GetBinContent(binmax);
	       //cout<<binmax<<"  "<<max<<endl;
	       //for(int k=1; k<=62; k++){
               //  charge = row[j]->GetBinContent(k);
               //  total_charge = row[j]->Integral(1,62);
	       //weigthed_pos[k] = (k-1)*charge/total_charge;
	       //  centroid[j] = centroid[j] + weigthed_pos[k];
	       //}
	       centroid[j] = row[j]->GetMean();
	       centroid_mm[j] = centroid[j] * 5 + 2.5;
	       if(max>100){grTrack->SetPoint(np++, centroid_mm[j],zrow[j]);}
            }
            
            // loop on the SiC file 
            finSic->cd();
            SicLoopFlag=1;
            while(SicLoopFlag){ 
            treeSic->GetEntry(sicHits);
	       if (TimestampSic>(timeinit-timeWindowlow)){    // The Sic is after the Tracker, stop reading the SiC file and go further with the tracks
                  tracksWithoutSic++;
                  SicLoopFlag=0;
                  FlagSicStop=0;  	
               }else if((timeinit-TimestampSic)>timeWindowlow && (TimestampTrackerEv-TimestampSic)<timeWindowhigh){  // the time of SiC is compatible with the track
                  cout << "+++++++++++ Event detected by the SiC" << endl;
                  tracksWithSic++;
                  sicHits++;
                  SicLoopFlag=0;
                  FlagSicStop=1;
               }else if(TimestampSic <= (timeinit-timeWindowhigh) ) {		// the SiC is is before the Tracker, to this SiC no track can be associated.
                  sicWithoutTracks++;
                  sicHits++;
                  SicLoopFlag=1;
                  FlagSicStop=0;
               }
            }
            //cout << "tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t flagTrackWithSiC " << flagTrackWithSiC << "\t flagS " << flagS << endl;
	    row[4]->GetYaxis()->SetRangeUser(0,max*2);
   	    grTrack->Fit("lin1","R");
    	    intercept = lin1->GetParameter(0);
	    slope = lin1->GetParameter(1);
   	    alpha = TMath::ATan(slope);
	    alpha_deg = alpha*180./TMath::Pi();
	    theta_deg=-90-alpha_deg;
	    if(theta_deg<-90){
	       theta_deg=-90-alpha_deg+180;
	    }
            //cout << "slope " << slope << "\t theta " << theta << "\t theta_deg " << theta_deg << endl;
            cin >> anykey; 
	    h_alpha[0]->Fill(alpha_deg);	       
	    h_angles[0]->Fill(theta_deg);
            if(FlagSicStop==0){
               h_alpha[1]->Fill(alpha_deg);
      	       h_angles[1]->Fill(theta_deg);
            }else{
               h_alpha[2]->Fill(alpha_deg);
               h_angles[2]->Fill(theta_deg);
               //cout << "alpha_deg " << alpha_deg << endl;
               //cin >> anykey;
            }  
            
            if(FlagSicStop==1 && flagM==1 ){           
               C2->cd(0);
               if(np>0){
                  grTrack->Draw("P");
                  grTrack->Fit("lin1","");
               }
               
               C2->Update();
               cout << "alpha_deg " << alpha_deg << endl;
               cout << "slope " << slope << endl;
               cout << lin1->GetParameter(1)<<"  "<<90-abs(atan(lin1->GetParameter(1))*180/3.1415)<<endl;
              
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
	
   	 for(int j=0;j<5; j++){
	    row[j]->Reset("ICES");
	    time[j]->Reset("ICES");
	    flag[j]=0;
	 }
         if (Charge > thresh) {
            for (int k=0; k<5; ++k) {
                if (Row==k) {
                   flag[k]=1;
                   row[k]->Fill(pad,Charge);
                   time[k]->Fill(pad,timeinit);
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
   
   //cout << "Col sic: " << h_angles[2]->Integral(0,nbinalpha) << endl;
      
}
