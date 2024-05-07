//###################################################################################################
//#   macro that take as input a merged file, plot the anode map of each event
//#   and plot the track by means of a fit procedure
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event
//# 			   array with the number of digitizers used
//###################################################################################################
//#   created march 2021
//#######################################
//# updated  for the nudaq acq files   25 11 2022 V.Soukeras
//# updated: to the new data standard     12 2022 D. Torresi
//# 
//###################################################################################################



void B_anglesFinder_plot_tracker_and_sic(int run)
{

   // Dichiarazione variabili

   // window that define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;

   Float_t timeWindowlow = 0.5E+06; // time (in ps) that the primary electrons need to reach the anode
   Float_t timeWindowhigh = 2.0E+06; 

   UShort_t Channel, ChannelSic; 
   UShort_t pad; 
   UShort_t FTS, FTSSic;
   ULong64_t CTS,CTSSic;
   ULong64_t Timestamp, TimestampSic, TimestampPre, TimestampSicTemp;
   UShort_t Board, BoardSic;
   UShort_t Charge, ChargeSic;
   Double_t Charge_cal, Charge_calSic;
   UInt_t Flags, FlagsSic, sicFlag;
   UShort_t Row;
   UShort_t Section;
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;
   
   int binmax=-100, max=-100;
   // number of event in the run
   int eventNumber=0;
   
   Double_t theta=-1000., theta_deg=-1000.;
   Double_t phi=-1000., phi_deg=-1000.;
   
   Int_t flagM=0,flagS=0,track_with_sic_flag=0;
   
   Int_t eventSic,tracksWithoutSic,tracksWithSic,sicWithoutTracks;
   eventSic=tracksWithoutSic=tracksWithSic=sicWithoutTracks=0;   

   double *zrow = new double[5];  // zcoordinate of the row
   zrow[0]=18.60;		  // valid for the prototype 2
   zrow[1]=39.80;
   zrow[2]=61.00;
   zrow[3]=82.20;
   zrow[4]=103.40;


   // Apertura file tracker
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


   // Apertura file sic
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

   // Dichiarazione Histo 
   TH1F *row[5];
   row[0]=new TH1F("r0","r0",62,-0.5,61.5);
   row[1]=new TH1F("r1","r1",62,-0.5,61.5);
   row[2]=new TH1F("r2","r2",62,-0.5,61.5);
   row[3]=new TH1F("r3","r3",62,-0.5,61.5);
   row[4]=new TH1F("r4","r4",62,-0.5,61.5);

   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   
   
   Int_t nbintheta=400;
   Int_t nbinphi=50;
   Double_t binmintheta=-180., binmaxtheta=180.;
   Double_t binminphi=-30., binmaxphi=30.;
      
   TH1D *h_angles[6];
   h_angles[0]=new TH1D("h_theta_","h_theta",nbintheta,binmintheta,binmaxtheta);
   h_angles[1]=new TH1D("h_theta_without_sic","h_theta_without_sic",nbintheta,binmintheta,binmaxtheta);
   h_angles[2]=new TH1D("h_theta_with_sic","h_theta_with_sic",nbintheta,binmintheta,binmaxtheta);
   h_angles[3]=new TH1D("h_phi","h_phi",nbinphi,binminphi,binmaxphi);
   h_angles[4]=new TH1D("h_phi_without_sic","h_phi_without_sic",nbinphi,binminphi,binmaxphi);
   h_angles[5]=new TH1D("h_phi_with_sic","h_phi_with_sic",nbinphi,binminphi,binmaxphi);
   

   for (Int_t k=0; k<6; k++) {
   
       if (k<3)
          h_angles[k]->GetXaxis()->SetTitle("#theta (deg)");
       else
          h_angles[k]->GetXaxis()->SetTitle("#phi (deg)");
         
       h_angles[k]->GetYaxis()->SetTitle("counts");
       
       h_angles[k]->SetLineColor(k+1);
   } 

   TCanvas *c1=new TCanvas("c1","c1",900.,800.);
   c1->SetFillColor(kWhite);
   c1->SetLogy();

   finTracker->cd();
  
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init tracker: "<<timeinit<<endl;
   
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<Timestamp<<endl;
   
   int sicHits=0;
  
   TGraph *grTrack=new TGraph(0);

   double charge = 0.0;
   double total_charge = 0.0;
   double weigthed_pos[62];
   double centroid[5], centroid_mm[5];
   
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",-60,60);
   double slope,intercept;
   TF1 *fit_result;
   
   int np=0;
  
   // Lettura file      
   for(int i=0; i<entriesTracker/100; i++){
   //for (int i=0; i<3694953; i++) {
      treeTracker->GetEntry(i);   
      if (i%1000==0) cout << "Entry: " << i << endl;
      //if(Charge>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
      
        if (Charge > thresh) {
           if (Row==0) {
              flag[0]=1;
              row[0]->Fill(pad,Charge);
           }
           if (Row==1) {
              flag[1]=1;
              row[1]->Fill(pad,Charge);
           }
           if (Row==2) {
              flag[2]=1;
              row[2]->Fill(pad,Charge);
           }
           if (Row==3) {
              flag[3]=1;
              row[3]->Fill(pad,Charge);
           }
           if (Row==4) {
              flag[4]=1;
              row[4]->Fill(pad,Charge);
           }
        }

      } else {
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>4){
            np=0; 
	    for(int j=0; j<5; j++){ 
	       //centroid[j]=0;
	       binmax = row[j]->GetMaximumBin(); 
	       max  = row[j]->GetBinContent(binmax);
	       //for(int k=1; k<=62; k++){
               //  charge = row[j]->GetBinContent(k);
               //  total_charge = row[j]->Integral(1,62);
	       //weigthed_pos[k] = (k-1)*charge/total_charge;
	       //  centroid[j] = centroid[j] + weigthed_pos[k];
	       //}
	       centroid[j] = row[j]->GetMean();
	       centroid_mm[j] = centroid[j] * 5;
	       if(max>100){grTrack->SetPoint(np++, centroid_mm[j],zrow[j]);}

	       //cout<<binmax<<"  "<<max<<endl;
	     
	     
             if(j==4){
             
                   // here the Sic file is opened
               finSic->cd();

               sicFlag=1;
           
//             for (int j=0;j<entriesSic; j++) { // loop to read the tree of the sic file
               while (sicFlag) {

                  //track_with_sic_flag=0;

                  //cout << "Entro nel while" << endl; 
                  treeSic->GetEntry(sicHits);
              
                  if ( TimestampSic>timeinit-timeWindowlow  ) {
                     //cout << "primo if" << endl;
                     tracksWithoutSic++;
                     sicFlag = 0;
                     flagS=0;
                     track_with_sic_flag=0;  
                  }
              
                  else if ( (timeinit-TimestampSic)>timeWindowlow && (TimestampPre-TimestampSic)<timeWindowhigh ) {
              
                     //cout << "+++++++++++ Event detected by the SiC" << endl;
                     tracksWithSic++;
                     sicHits++;
                     sicFlag = 0;
                     flagS=1;
                     track_with_sic_flag=1;
                     //cin>>anykey;
                  }
              
                  else if (  TimestampSic <= (timeinit-timeWindowhigh) ) {
                     //cout << "terzo if" << endl;
                     sicWithoutTracks++;
                     sicHits++;
                     sicFlag=1;
                     flagS=0;
                     track_with_sic_flag=0;                     
                  }
              
               }        
               //cout << "tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t track_with_sic_flag " << track_with_sic_flag << "\t flagS " << flagS << endl;
             
	       row[j]->GetYaxis()->SetRangeUser(0,max*2);

	       grTrack->Fit("lin1","r");

 	       intercept = lin1->GetParameter(0);
	       slope = lin1->GetParameter(1);
	       
	       theta = TMath::ATan(slope);
	       theta_deg = theta*180./TMath::Pi();

               //cout << "slope " << slope << "\t theta " << theta << "\t theta_deg " << theta_deg << endl;
               //cin >> anykey; 
	       
	       h_angles[0]->Fill(theta_deg);	       
	       
	       if (track_with_sic_flag==0)
	          h_angles[1]->Fill(theta_deg);
	          
               else {
                  h_angles[2]->Fill(theta_deg);
                  //cout << "theta_deg " << theta_deg << endl;
                  //cin >> anykey;
               }   
 
/*	       if(j==4){
		   anode_fit->Draw();
		   anode_fit->Fit("lin1","r");
		   printf("\n");
		   printf("\n");
		   intercept=lin1->GetParameter(0);
		   slope=lin1->GetParameter(1);
 
		   TF1 *fit_result = new TF1("fit_result",Form("%1.8f+(%1.8f*x)",intercept,slope),0.00,32.00);
		   fit_result->SetLineColor(2);
		   fit_result->SetLineWidth(2);
		   fit_result->Draw("same");
	   
		   C5->cd(0);
		   fit_result->SetLineColor(2);
		   fit_result->SetLineWidth(2);
		   fit_result->Draw("same");*/
	       }
            }

    
	 }
	 //cout<<"---------------"<<endl;
	 timeinit=Timestamp;
	 grTrack->Set(0);
	 
	 
   	 for(int j=0;j<5; j++){
	    row[j]->Reset("ICES");
	    flag[j]=0;
	 }
        
        if (Charge > thresh) {
           if (Row==0) {
              flag[0]=1;
              row[0]->Fill(pad,Charge);
           }
           if (Row==1) {
              flag[1]=1;
              row[1]->Fill(pad,Charge);
           }
           if (Row==2) {
              flag[2]=1;
              row[2]->Fill(pad,Charge);
           }
           if (Row==3) {
              flag[3]=1;
              row[3]->Fill(pad,Charge);
           }
           if (Row==4) {
              flag[4]=1;
              row[4]->Fill(pad,Charge);
           }
        }
          
      } 
        
   }
   
   c1->cd();
   h_angles[0]->Draw();
   h_angles[1]->Draw("same");
   h_angles[2]->Draw("same");
   
   cout << "Col sic: " << h_angles[2]->Integral(0,nbintheta) << endl;
      
}
