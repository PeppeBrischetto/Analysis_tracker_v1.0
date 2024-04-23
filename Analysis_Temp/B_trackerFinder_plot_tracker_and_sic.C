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



void B_trackerFinder_plot_tracker_and_sic(int run)
{

   // Dichiarazione variabili

   // window that define the event, opened with the first hit
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
   
   int binmax=-100, max=-100;
   // number of event in the run
   int eventNumber=0;
   
   
   Int_t flagM=0,flagS=0,track_with_sic_flag=0;
   
   Int_t eventSic,tracksWithoutSic,tracksWithSic,sicWithoutTracks;
   eventSic=tracksWithoutSic=tracksWithSic=sicWithoutTracks=0;   

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
   
   // map of the anode fille with charge
   TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,-0.75,4.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
      
   // histo for the track without sic
   TH2D *tracks=new TH2D("tracks","tracks",62,-0.5,61.5,26,-7.25,5.75);
   tracks->SetStats(kFALSE);
   tracks->GetXaxis()->SetTitle("pad");
   tracks->GetYaxis()->SetTitle("row");
   // graph filled with centroids of clusters
   TGraph *grTrack=new TGraph(0);

   
   //histo for tracks hitting the SiC
   TH2D *tracks_with_sic=new TH2D("tracks_with_sic","tracks_with_sic",62,-0.5,61.5,26,-7.25,5.75);
   tracks_with_sic->SetStats(kFALSE);
   tracks_with_sic->GetXaxis()->SetTitle("pad");
   tracks_with_sic->GetYaxis()->SetTitle("row");
   // graph filled with centroids of clusters
   TGraph *grTrack_with_sic=new TGraph(0);   
   

   // map of anode filled with time
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,-0.75,4.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("row");
   anodeTime->GetYaxis()->SetNdivisions(-11);		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di row
   TGaxis *axis1 = new TGaxis(-0.5,-0.75,-0.5,4.75,-0.75,4.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   axis1->Draw();

   TCanvas *C21=new TCanvas("c21","distr1",800,500,900,400);
   C21->SetFillColor(kWhite);

   TCanvas *C22=new TCanvas("c22","distr2",800,500,900,400);
   C22->SetFillColor(kWhite);

   TCanvas *C23=new TCanvas("c23","distr3",800,500,900,400);
   C23->SetFillColor(kWhite);

   TCanvas *C24=new TCanvas("c24","distr4",800,500,900,400);
   C24->SetFillColor(kWhite);

   TCanvas *C25=new TCanvas("c25","distr5",800,500,900,400);
   C25->SetFillColor(kWhite);


   TCanvas *C3=new TCanvas("c3","c3",800,500,900,400);
   C3->SetFillColor(kWhite);

   TCanvas *C4=new TCanvas("c4","c4",800,500,900,400);
   C4->SetFillColor(kWhite);
   
   TCanvas *C5=new TCanvas("c5","c5",800,500,900,400);
   C5->SetFillColor(kWhite);
   C5->cd(0);
  // tracks->GetYaxis()->SetRangeUser(-2.0,5.0);
   tracks->Draw();
   
   TCanvas *C6=new TCanvas("c6","c6",800,500,900,400);
   C6->SetFillColor(kWhite);
   C6->cd(0);
   tracks_with_sic->Draw();
   
  
   finTracker->cd();
  
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init tracker: "<<timeinit<<endl;
   
   treeSic->GetEntry(0);
   cout<<" time init SiC: "<<Timestamp<<endl;
   
   int sicHits=0;
  
  

   double charge = 0.0;
   double total_charge = 0.0;
   double weigthed_pos[62];
   double centroid[5];
   
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",-60,60);
   double slope,intercept;
   TF1 *fit_result;
   
   int np=0;

   TH2D *anode_fit=new TH2D("anode_fit","anode_fit",620,-0.5,61.5,110,-0.75,4.75);
   anode_fit->SetStats(kFALSE);
   anode_fit->GetXaxis()->SetTitle("pad");
   anode_fit->GetYaxis()->SetTitle("row");
  
   // detector edges
   TLine *edgeL=new TLine(60.,-0.75,60.,4.75);
   TLine *edgeU=new TLine(0.,-0.75,0.,4.75);

  
   cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   // Lettura file      
   for(int i=0;i <entriesTracker; i++){
      treeTracker->GetEntry(i);   

      //if(Charge>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,0.1,Charge); flag[0]=1;
           anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
           row[0]->Fill(pad,Charge);
        }
        if(Row==1 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[1]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           row[1]->Fill(pad,Charge);
        }
        if(Row==2 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[2]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           row[2]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[3]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           row[3]->Fill(pad,Charge);
        }
        if(Row==4 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[4]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           row[4]->Fill(pad,Charge);
        }

      }else{
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
	       centroid[j]=row[j]->GetMean();
	       if(max>100){grTrack->SetPoint(np++, centroid[j],j);}

	       //cout<<binmax<<"  "<<max<<endl;
	     
	     
             if(j==4){
             
                   // here the Sic file is opened
               finSic->cd();

               sicFlag=1;
           
//             for (int j=0;j<entriesSic; j++) { // loop to read the tree of the sic file

               while (sicFlag) {
                  cout << "Entro nel while" << endl; 
                  treeSic->GetEntry(sicHits);
              
                  if ( TimestampSic>timeinit-timeWindowlow  ) {
                     cout << "primo if" << endl;
                     tracksWithoutSic++;
                     sicFlag = 0;
                     flagS=0;
                     track_with_sic_flag=0;  
                  }
              
                  else if ( (timeinit-TimestampSic)>timeWindowlow && (TimestampPre-TimestampSic)<timeWindowhigh ) {
              
                     cout << "+++++++++++ Event detected by the SiC" << endl;
                     tracksWithSic++;
                     sicHits++;
                     sicFlag = 0;
                     flagS=1;
                     track_with_sic_flag=1;
                     //cin>>anykey;
                  }
              
                  else if (  TimestampSic <= (timeinit-timeWindowhigh) ) {
                     cout << "terzo if" << endl;
                     sicWithoutTracks++;
                     sicHits++;
                     sicFlag=1;
                     flagS=0;
                     track_with_sic_flag=0;                     
                  }
              
               }        
               cout << "tracksWithoutSic " << tracksWithoutSic << "\t tracksWithSic " << tracksWithSic << "\t track_with_sic_flag " << track_with_sic_flag << "\t flagS " << flagS << endl;
             
	       row[j]->GetYaxis()->SetRangeUser(0,max*2);
	       anode_fit->Fill(centroid[j],j+1);
	       C4->cd(0);
	       anode_fit->SetMarkerStyle(20);
	       anode_fit->SetMarkerSize(1);

	       grTrack->Fit("lin1","r");

 	       intercept=lin1->GetParameter(0);
	       slope=lin1->GetParameter(1);

	       fit_result = new TF1("fit_result",Form("%1.8f+(%1.8f*x)",intercept,slope),0.00,64.00);
	       if (track_with_sic_flag==0)
	          fit_result->SetLineColor(2);
	       else
	          fit_result->SetLineColor(4);
	           
	       fit_result->SetLineWidth(2);
	       //fit_result->Draw("same");

	       C5->cd(0);
	       if (track_with_sic_flag==0)
	          fit_result->SetLineColor(2);
	       else {
                  fit_result->SetLineColor(4);
                  fit_result->Draw("same");
               }
	          
	       fit_result->SetLineWidth(2);
	       if(np==5){
	          //fit_result->Draw("same");
	          C6->cd();
	       	  fit_result->Draw("same");
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

            // plot the charge map of the anode
            C1->cd(0);
            anode->Draw("colz");
            axis1->Draw();
            edgeL->Draw();
            edgeU->Draw();

            // plot centroid
            grTrack->SetMarkerStyle(20);
            grTrack->SetMarkerSize(1);
            grTrack->Draw("P");
            C1->Update();

            // plot the time anode map
            C3->cd(0);
            anodeTime->Draw("colz");
            axis1->Draw();
            C3->Update();

            C21->cd(0);
            row[0]->Draw("histo");
            C21->Update();
            C22->cd(0);
	    row[1]->Draw("histo");
	    C22->Update();
	    C23->cd(0);
 	    row[2]->Draw("histo");
 	    C23->Update();
	    C24->cd(0);
  	    row[3]->Draw("histo");
	    C24->Update();
	    C25->cd(0);
	    row[4]->Draw("histo");
	    C25->Update();
            C3->Update();
            C4->Update();
            C5->Update();
            C6->Update();
           
            cout<<"entry "<<i<<endl;
            cout<<"press any key to continue, q to quit, s to save a plot"<<endl;
            //cin>>anykey;
            if(flagM==0)cin>>anykey;
            //if(flagS==1) cin>>anykey;
            if(anykey=='q') return ; // Per uscire dal programma
            if(anykey=='s'){ 		 // Salvi il plot
               C1->Print("c1.eps");
               C5->Print("C5.eps");
   	    }
            //if(anykey=='c')flagM=1;
            //if(anykey=='c')flagS=0;        
	 }
	 cout<<"---------------"<<endl;
	 timeinit=Timestamp;
	 anode->Reset("ICES");
	 anodeTime->Reset("ICES");
	 anode_fit->Reset("ICES");
	 grTrack->Set(0);
	 
	 
   	 for(int j=0;j<5; j++){
	    row[j]->Reset("ICES");
	    flag[j]=0;
	 }
        
	 if(Row==0 && Charge>thresh){
	    anode->Fill(pad,0.1,Charge); flag[0]=1;
	    anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
	    row[0]->Fill(pad,Charge);
	 }
	  if(Row==1 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[1]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           row[1]->Fill(pad,Charge);
         }
         if(Row==2 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[2]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           row[2]->Fill(pad,Charge);
         }
         if(Row==3 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[3]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           row[3]->Fill(pad,Charge);
         }
         if(Row==4 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[4]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           row[4]->Fill(pad,Charge);
	 }
          
      } 
        
   }
      
}
