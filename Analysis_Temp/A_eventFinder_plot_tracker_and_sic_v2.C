//###################################################################################################
//#   macro that take as input a merged file and plot the anode map of each event
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event
//# 			   array with the number of digitizers used
//###################################################################################################
//#   created march 2021
//#######################################
//# updated for the nudaq acq files   25 11 2022 V.Soukeras D. Torresi
//# updated: read the Row parameter   28 11 2022 D. Torresi
//# updated: plot the "strips"         1 12 2022 D. Torresi
//# updated: rows start from 0         
//#	      events counter 		 5 12 2022 D. Torresi
//# updated: for the new anode	19  7 2023 D. Torresi
//###################################################################################################

int A_eventFinder_plot_tracker_and_sic_v2(int run)
{

   // Dichiarazione variabili

  
   // window the define the event, opened with the first hit
   float DeltaT=2000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;
   
   Float_t timeWindowlow = 0.5E+06; // time (in ps) that the primary electrons need to reach the anode
   Float_t timeWindowhigh = 4.0E+06; 

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
   int flagM=0, flagS=0;
   int Fstrip=1;			// flag to plot strip: 0 plot, 1 no plot
   
   int binmax, max;
   // number of event in the run
   int eventNumber=0;
   
   Int_t tracksWithoutSic,tracksWithSic,sicWithoutTracks;
   tracksWithoutSic=tracksWithSic=sicWithoutTracks=0;
  
   
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
   // Lettura file
  
   finTracker->cd();
  
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init tracker: "<<timeinit<<endl;
   
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<Timestamp<<endl;
   
   int sicHits=0;
   
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
        
        TimestampPre = Timestamp;
        
      }else{
        if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>2){
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

           eventNumber++;
           cout<<"entry "<<i<<endl;
           cout<<"Event number: "<<eventNumber<<endl;
           
           cout << "Timestamp: " << Timestamp << "\t TimestampPre: " << TimestampPre << endl; 


           // here the Sic file is opened
           finSic->cd();

           sicFlag=1;
           
//           for (int j=0;j<entriesSic; j++) { // loop to read the tree of the sic file

             while (sicFlag) { 
               treeSic->GetEntry(sicHits);
              
              if ( TimestampSic>timeinit-timeWindowlow  ) {
                 tracksWithoutSic++;
                 sicFlag = 0;
                 flagS=0;  
              }
              
              else if ( (timeinit-TimestampSic)>timeWindowlow && (TimestampPre-TimestampSic)<timeWindowhigh ) {
              
                      cout << "+++++++++++ Event detected by the SiC" << endl;
                      tracksWithSic++;
                      sicHits++;
                      sicFlag = 0;
                      flagS=1;
                      
              }
              
              else if (  TimestampSic <= (timeinit-timeWindowhigh) ) {
              
                      sicWithoutTracks++;
                      sicHits++;
                      sicFlag=1;
                      flagS=0;
              }
              
              
           }
           
           cout << "tracksWithoutSiC = " << tracksWithoutSic << "\t tracksWithSic = " << tracksWithSic << "\t sicWithoutTracks = " << sicWithoutTracks << endl;
           cout << "sicHits = " << sicHits << endl;
           
           cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl;
           if(flagM==0)cin>>anykey;
           //if(flagS==1) cin>>anykey;
           if(anykey=='q') return 0; // Per uscire dal programma
           if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.eps");
              C3->Print("c3.eps");
   	   }
     	   if(anykey=='c')flagM=1;        
   	   //if(anykey=='c')flagS=0;
                      
        }

        

        cout<<"---------------"<<endl;
         
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
