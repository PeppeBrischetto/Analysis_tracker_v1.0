//###################################################################################################
//#   macro that take as input a merged file and look for clusters
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval where you llok for the cluster
//#		This time should be larger of the duration of a cluster and small
//#		enough to make the probability of the overposition of two cluters
//# 		neglegible.
//#
//#   		The macro is able to find cluster of one raw per time
//#
//#             write an output file wher each entry is a cluster 			   
//###################################################################################################
//#   created january 2023 D. Torresi
//#######################################
//###################################################################################################

void B_ClusterFinder_write(int run)
{
   // Dichiarazione variabili
 
   // window the define the event, opened with the first hit
   float DeltaT=1e8;  //in ps

   // energy threshold to defin an hit
   int thresh=0;

   // row where the cluster are seeked
   int row=0;

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
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;
   int flagM=0;
   
   int binmax, max;
   // number of event in the run
   int eventNumber=0;
   
   // cluster parameter
   int hit[64];
   int clusterNumber=0;
   int clusterFlag=0;
   UShort_t Charge_temp[64];
   Double_t Charge_cal_temp[64];
   ULong64_t Timestamp_temp[64];
   UShort_t Charge_max=0;
   int pad_temp;
   int Board_temp;
   int Row_temp;
   int pad_cluster;
   
   for(int i=0; i<64; i++){
      hit[i]=0;
      Charge_temp[i]=0;
      Charge_cal_temp[i]=0;
      Timestamp_temp[i]=0;
   }
    
   // Cluster output variables
   double Centroid_cluster;		//in unit of pad 
   ULong64_t Timestamp_cluster;	// timestamp of the pad with higher charge
   ULong64_t Timestamp_cluster_a;	// weighted average timestamp approximated to next integer
   UShort_t Board_cluster;		// Board  
   UShort_t Charge_cluster;		// total charge
   Double_t Charge_cal_cluster;	// total charge calibrated
   //UInt_t Flags_cluster;			
   UShort_t Row_cluster;			// row
   UShort_t Section_cluster;
   UShort_t multiplicity_cluster;   
   
   // Open output file
   char fileOutName[50];
   //sprintf(fileOutName, "../Cluster_data/run_%i/cluster_Row%i.root", run, row);
   sprintf(fileOutName, "cluster_Run%i_Row%i.root", run, row);
   TFile *out_file = new TFile(fileOutName,"recreate");
   // Output tree single channel events
   TTree *out_tree = new TTree("Data_R","Data tree");

   out_tree->Branch("Board",&Board_cluster,"Board/s");
   out_tree->Branch("Timestamp",&Timestamp_cluster,"Timestamp/l");
   out_tree->Branch("Timestamp_a",&Timestamp_cluster_a,"Timestamp_a/l");   
   out_tree->Branch("Charge",&Charge_cluster,"Charge/s");
   out_tree->Branch("Charge_cal",&Charge_cal_cluster,"Charge_cal/D");   
   out_tree->Branch("Centroid",&Centroid_cluster,"Centroid/D");
   out_tree->Branch("Row",&Row_cluster,"Row/s");
   out_tree->Branch("Section",&Section_cluster,"Section/s");
   out_tree->Branch("Multiplicity",&multiplicity_cluster,"multiplicity/s");         
   
   // Open input file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);
   cout<<fileIn<<endl;
   TFile *fin = new TFile(fileIn);
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Row);
   tree->SetBranchAddress("Section",&Section);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *hrow[5];
   hrow[0]=new TH1F("r0","r0",62,-0.5,61.5);
   hrow[1]=new TH1F("r1","r1",62,-0.5,61.5);
   hrow[2]=new TH1F("r2","r2",62,-0.5,61.5);
   hrow[3]=new TH1F("r3","r3",62,-0.5,61.5);
   hrow[4]=new TH1F("r4","r4",62,-0.5,61.5);

   for(int i=0;i<5; i++){
     hrow[i]->GetXaxis()->SetTitle("pad");
     hrow[i]->GetYaxis()->SetTitle("counts");
   }
   hrow[1]->SetLineColor(kRed);
   hrow[2]->SetLineColor(kBlue);
   hrow[3]->SetLineColor(kGreen);
   hrow[4]->SetLineColor(kViolet);
    
   TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,-0.75,4.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,-0.75,4.75);
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
  
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;

   // detector edges
   TLine *edgeL=new TLine(23.5,-0.75,23.5,4.75);
   TLine *edgeU=new TLine(3.5,-0.75,3.5,4.75);
   
   Charge_cal_cluster=0;
   Charge_cluster=0;
   Timestamp_cluster=0;
   Charge_max=0;
   Timestamp_cluster=0;
   multiplicity_cluster=0;
   Centroid_cluster=0;
   
   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      fin->cd();
      tree->GetEntry(i);
      
      if((Timestamp-timeinit)<DeltaT){
	 if(Row==0 && Charge>thresh){
	    anode->Fill(pad,4,Charge); flag[0]=1;
	    anodeTime->Fill(pad,4,Timestamp-timeinit +10);
	    hrow[0]->Fill(pad,Charge);
           
	    // store the data in temporary storage 
	    hit[pad]++;
	    Charge_temp[pad]=Charge;
	    Timestamp_temp[pad]=Timestamp;
	    Charge_cal_temp[pad]=Charge_cal;   
	    pad_temp=pad;
	    Board_temp=Board;
	    Row_temp=Row;

	 }
	 
      }else{
	 if(flag[0]>0){
           // decide if it is a single cluster or a multiple one
           for(int j=0; j< 32; j++){
              if(hit[j]!=0){
              	 if(clusterFlag==0){
                   clusterNumber++;
                   clusterFlag=1;                 
                 }
                 multiplicity_cluster++;
                 if(Charge_temp[j]>Charge_max){
                    Charge_max=Charge_temp[j];
                    Timestamp_cluster=Timestamp_temp[j];
                 }
                 Charge_cluster=Charge_cluster+Charge_temp[j];
                 Charge_cal_cluster=Charge_cal_cluster+Charge_cal_temp[j];
                 //Timestamp_cluster_a=Timestamp_cluster_a+Timestamp_temp[j]*Charge_temp[j];
                 
              }
              if(hit[j]==0){
                 clusterFlag=0;
              }                        
              //cout<<j<<"\t"<<hit[j]<<endl;
           }
           
           cout<<" Cluster number: "<<clusterNumber<<endl;
           cout<<" Cluster mult:   "<<multiplicity_cluster<<endl;
           // plot cluster(s)
           C1->cd(0);
           anode->Draw("colz");
           //C1->Update();      
           //pad1->GetFrame()->SetFillColor(kGray);  
           axis1->Draw();
           edgeL->Draw();
           edgeU->Draw();
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

           // write data
           Row_cluster=Row;
           pad_cluster=pad_temp;
           //Timestamp_cluster=Timestamp_cluster;
           //cout<<"***"<<Timestamp_cluster<<endl;
           //Timestamp_cluster_a,"Timestamp_a/l");   
           //Charge_cluster=Charge_cluster;
           //Charge_cal_cluster=Charge_cal_cluster;
           //Centroid_cluster,"Centroid/D");
           Section_cluster=Section;              
           out_file->cd();
           out_tree->Fill();

           eventNumber++;
           cout<<"entry "<<i<<endl;
           cout<<"Event number: "<<eventNumber<<endl;
           cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl;
           if(flagM==0)cin>>anykey;
           if(anykey=='q')return 0; // Per uscire dal programma
           if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.pdf");
   	   }        
   	   if(anykey=='c')flagM=1;
                      
        }
        cout<<"---------------"<<endl;
        timeinit=Timestamp;
        Charge_cal_cluster=0;
        Charge_cluster=0;
        Timestamp_cluster=0;
        Charge_max=0;
        Timestamp_cluster=0;
        multiplicity_cluster=0;
        Centroid_cluster=0;
        for(int i=0; i<64; i++){
           hit[i]=0;
           Charge_temp[i]=0;
           Charge_cal_temp[i]=0;
           Timestamp_temp[i]=0;
        }  // zeros the hit array.
        clusterNumber=0;
        clusterFlag=0;
        anode->Reset("ICES");
        anodeTime->Reset("ICES");
        for(int i=0;i<5; i++){
           hrow[i]->Reset("ICES");
           flag[i]=0;
        }
        
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[0]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
           
           hit[pad]++;
        }  
      } 
   }
 
   out_file->cd();
   out_tree->Write();
   out_file->Purge();
   out_file->Close();     
}
