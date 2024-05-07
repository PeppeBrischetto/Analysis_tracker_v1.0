//###################################################################################################
//#   macro that take as input a single file from a single channel of SiC and a 
//#   merged file from the tracker.
//#   It Plot the anode map of an event when there is a SiC in coincidence.
//#   The SiC file is used as a master, a time window is open and the tracks of the gas detector
//#   are seeked inside the time window.
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event, and the Time window
//#            where to seek the tracks.
//# 	       The array with the number of digitizers used
//###################################################################################################
//#   created april 2024 from A_eventFinder_plot.C by G. Brischetto
//#######################################
//#   updated: 30-4-2024  cleaning and comments  D. Torresi
//# 
//###################################################################################################

int A_eventFinderTrSiC_plot(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

   // window the define the event, opened with the first hit in ps
   float DeltaT=1000000;  	
   // energy threshold to defin an hit
   int thresh=0;
   
   // time window in ps where you expect to find the tracker hit after the sic was hit.
   // in other word the delay of pad hit respect to the Sic hit.
   Float_t timeWindowlow = 0.5E+06; 
   Float_t timeWindowhigh = 4.0E+06; 

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
   // Secondary variables  
   ULong64_t TimestampSicTemp;   
   ULong64_t TimestampTrackerEv;	// variables used for checks
   UInt_t SicLoopFlag;			// variable used to stop the loop on the Sic file
   UInt_t rowMultiplicity=4; 	// consider tracks with a number of hit row bigger than rowMultiplicity
   UInt_t binmax, max;

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
// Dichiarazione Histo E Canvas
   TH1F *hrow[5];
   hrow[0]=new TH1F("r0","r0",60,-0.5,59.5);
   hrow[1]=new TH1F("r1","r1",60,-0.5,59.5);
   hrow[2]=new TH1F("r2","r2",60,-0.5,59.5);
   hrow[3]=new TH1F("r3","r3",60,-0.5,59.5);
   hrow[4]=new TH1F("r4","r4",60,-0.5,59.5);

   for(int i=0;i<5; i++){
     hrow[i]->GetXaxis()->SetTitle("pad");
     hrow[i]->GetYaxis()->SetTitle("counts");
   }
   hrow[1]->SetLineColor(kRed);
   hrow[2]->SetLineColor(kBlue);
   hrow[3]->SetLineColor(kGreen);
   hrow[4]->SetLineColor(kViolet);
   
   TH2D *anode=new TH2D("anode","anode",60,-0.5,59.5,11,-0.75,4.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",60,-0.5,59.5,11,-0.75,4.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("row");
   anodeTime->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di row
   TGaxis *axis1 = new TGaxis(-0.5,-0.75,-0.5,4.75,-0.75,4.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",800,0,900,350);
   C1->SetFillColor(kWhite);
   //C1->SetFrameFillColor(kWhite);
   /*TPad *pad1 = new TPad("pad1","pad1", 0.02,0.05,0.98,0.95,21);
   pad1->Draw();   
   pad1->cd();
   pad1->SetFillColor(kGray);
   pad1->GetFrame()->SetFillColor(18);
   pad1->GetFrame()->SetBorderMode(-1);
   pad1->GetFrame()->SetBorderSize(1);
   pad1->SetGridy(); 	*/
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   axis1->Draw();

   TCanvas *C2=new TCanvas("c2","c2",200,550,900,350);
   C2->SetFillColor(kWhite);
/*   TPad *pad2 = new TPad("pad2","pad2", 0.02,0.05,0.98,0.95,21);
   pad2->Draw();   
   pad2->cd();
   pad2->SetFillColor(kWhite);
   pad2->GetFrame()->SetFillColor(18);
   pad2->GetFrame()->SetBorderMode(-1);
   pad2->GetFrame()->SetBorderSize(1);
   pad2->SetGridy(); 	
  */ 
   TCanvas *C3=new TCanvas("c3","c3",800,500,900,350);
   C3->SetFillColor(kWhite);
// END Histo and Canvas 	////////////////////////////////////


////////////////////////////////////////////////////////////////////
// Opening the files
  
   finTracker->cd();
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;		// take the reference time from the fiorst hit of the tracker
   cout<<" time init tracker: "<<timeinit<<endl;
   
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<Timestamp<<endl; 
     
   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entriesTracker; i++){
      treeTracker->GetEntry(i);
           
      //if(Energy>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Energy<<"\t("<<Energy_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
         if(Row==0 && Charge>thresh){
            anode->Fill(pad,0.1,Charge); flag[0]=1;
            anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
            hrow[0]->Fill(pad,Charge);
         }
         if(Row==7 && Charge>thresh && Fstrip==0){ //&& Charge>thresh){
            for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
         }
        
         if(Row==1 && Charge>thresh){
         //if(Section==4 && Charge>thresh){
            anode->Fill(pad,1,Charge); flag[1]=1;
            anodeTime->Fill(pad,1,Timestamp-timeinit +10);
            hrow[1]->Fill(pad,Charge);
         }
         if(Row==10 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==9 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,3.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,3.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==2 && Charge>thresh){
         //if(Section==8 && Charge>thresh){
            anode->Fill(pad,2,Charge); flag[2]=1;
            anodeTime->Fill(pad,2,Timestamp-timeinit +10);
            hrow[2]->Fill(pad,Charge);
         }
         if(Row==3 && Charge>thresh){
         //if(Section==12 && Charge>thresh){
            anode->Fill(pad,3,Charge); flag[3]=1;
            anodeTime->Fill(pad,3,Timestamp-timeinit +10);
            hrow[3]->Fill(pad,Charge);
    	 }
         if(Row==8 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==4 && Charge>thresh){
         //if(Section==16 && Charge>thresh){
            anode->Fill(pad,4,Charge); flag[4]=1;
            anodeTime->Fill(pad,4,Timestamp-timeinit +10);
            hrow[4]->Fill(pad,Charge);
         }
         if(Row==5 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,-0.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,-0.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==6 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,Timestamp-timeinit +10);}
         }
         TimestampTrackerEv = Timestamp;
         
      }else{
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMultiplicity){
          
            // loop on the SiC file 
            finSic->cd();
            SicLoopFlag=1;
            while(SicLoopFlag){ 
               treeSic->GetEntry(sicHits);
               if (TimestampSic>timeinit-timeWindowlow){    // The Sic is after the Tracker, stop reading the SiC file and go further with the tracks
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
            cout<< "# tracks without SiC = " << tracksWithoutSic <<endl;
            cout<< "#    tracks with SiC = " << tracksWithSic <<endl;
            cout<< "# sic without tracks = " << sicWithoutTracks << endl;
            cout<< "#           sic hits = " << sicHits <<"\n"<<endl;
            
            if(FlagSicStop){           
               C1->cd(0);
               anode->Draw("colz");
               //C1->Update();      
               //pad1->GetFrame()->SetFillColor(kGray);  
               axis1->Draw();
               C1->Update();
               C3->cd(0);
               anodeTime->Draw("colz");
               //C1->Update();      
               //pad1->GetFrame()->SetFillColor(kGray);  
               axis1->Draw();
               C3->Update();
 
               C2->cd(0);
               hrow[0]->Draw("histo");
               binmax = hrow[0]->GetMaximumBin(); 
               max = hrow[0]->GetBinContent(binmax);
               //cout<<binmax<<"  "<<max<<endl;
               hrow[0]->GetYaxis()->SetRangeUser(0,max*2);
               for(int i=1; i<5; i++){hrow[i]->Draw("histo same");}
               C2->Update();
              
               cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl; 
               if(flagM)cin>>anykey;
            }
           
            if(anykey=='q') return 0; // Per uscire dal programma
            if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.eps");
              C3->Print("c3.eps");
  	    }
      	    if(anykey=='c')flagM=0;    
      	    
      	    eventNumber++;
            cout<<"entry "<<i<<endl;
            cout<<"Event number: "<<eventNumber<<endl;
            cout << " - Timestamp Track: " << Timestamp <<endl;
            cout << " - Timestamp SiC  : " << TimestampTrackerEv <<"\n"<< endl; 
       	    cout<<"---------------"<<endl;      
    	 }
         
         timeinit=Timestamp;
         anode->Reset("ICES");
         anodeTime->Reset("ICES");
         for(int i=0;i<5; i++){
            hrow[i]->Reset("ICES");
            flag[i]=0;
         }
         if(Row==0 && Charge>thresh){
            anode->Fill(pad,0.1,Charge); flag[0]=1;
            anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
            hrow[0]->Fill(pad,Charge);
         }
         if(Row==7  && Charge>thresh && Fstrip==0){ //&& Charge>thresh){
            for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==1 && Charge>thresh){
         //if(Section==4 && Charge>thresh){
            anode->Fill(pad,1,Charge); flag[1]=1;
            anodeTime->Fill(pad,1,Timestamp-timeinit +10);
            hrow[1]->Fill(pad,Charge);
         }
         if(Row==10 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==9 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==2 && Charge>thresh){
         //if(Section==8 && Charge>thresh){
            anode->Fill(pad,2,Charge); flag[2]=1;
            anodeTime->Fill(pad,2,Timestamp-timeinit +10);
            hrow[2]->Fill(pad,Charge);
         }
         if(Row==3 && Charge>thresh){
         //if(Section==12 && Charge>thresh){
            anode->Fill(pad,3,Charge); flag[3]=1;
            anodeTime->Fill(pad,3,Timestamp-timeinit +10);
            hrow[3]->Fill(pad,Charge);
      	 }
         if(Row==8 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==4 && Charge>thresh){
         //if(Section==16 && Charge>thresh){
            anode->Fill(pad,4,Charge); flag[4]=1;
            anodeTime->Fill(pad,4,Timestamp-timeinit +10);
            hrow[4]->Fill(pad,Charge);
         }
         if(Row==5 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,-0.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,-0.5,(Timestamp-timeinit +10)/10);}
         }
         if(Row==6 && Charge>thresh && Fstrip==0){
            for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
            for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,Timestamp-timeinit +10);}
         } 
      }     
   }   
}
