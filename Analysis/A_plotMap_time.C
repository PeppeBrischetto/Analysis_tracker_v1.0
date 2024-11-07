//###################################################################################################
//#   macro that Shows all the entries on the anode map for a fixed intervall of time DeltaT
//#   
//#   required as argument the run number
//###################################################################################################
//#   created March 2024   D. Torresi
//#######################################
//    updated for the new tracker 2024 Nov 6  D. Torresi
// 


int A_plotMap_time(int run)
{

   // Dichiarazione variabili

   // define the interval of time the entries are plot in the anode map
    float DeltaT=1.0e8;  //in ps
   
   // Dichiarazione variabili
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
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;

   int binmax, max;
   int thresh=10;
   
   // Apertura file
     char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);
   
   TFile *fin = new TFile(fileIn);
   //TFile *fin = new TFile("SDataR_run_65.root");
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *hrow[5];
   hrow[0]=new TH1F("r1","r1",60,-0.5,59.5);
   hrow[1]=new TH1F("r2","r2",60,-0.5,59.5);
   hrow[2]=new TH1F("r3","r3",60,-0.5,59.5);
   hrow[3]=new TH1F("r4","r4",60,-0.5,59.5);
   hrow[4]=new TH1F("r4","r4",60,-0.5,59.5);


   for(int i=0;i<5; i++){
     hrow[i]->GetXaxis()->SetTitle("pad");
     hrow[i]->GetYaxis()->SetTitle("counts");
   }
   hrow[1]->SetLineColor(kRed);
   hrow[2]->SetLineColor(kBlue);
   hrow[3]->SetLineColor(kGreen);
   hrow[4]->SetLineColor(kViolet);
   
   
   TH2D *anode=new TH2D("anode","anode",60,-0.5,59.5,11,0.25,5.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("raw");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",60,-0.5,59.5,11,0.25,5.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("raw");
   anodeTime->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di raw
   TGaxis *axis1 = new TGaxis(-0.5,0.25,-0.5,5.75,0.25,5.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
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

   TCanvas *C2=new TCanvas("c2","c2",800,500,900,400);
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
   TCanvas *C3=new TCanvas("c3","c3",800,500,900,400);
   C3->SetFillColor(kWhite);
   // Lettura file
  
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   
   cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      
      //if(Charge>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if(Timestamp-timeinit<DeltaT){
        if(Row==4 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[0]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[1]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==2 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==1 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[3]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
        }
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,5,Charge); flag[4]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }

      
      }else{
           timeinit=Timestamp;
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
           
           cout<<"entry "<<i<<endl;
           cout<<"press any key to continue, q to quit, s to save a plot"<<endl;
           cin>>anykey;
           if(anykey=='q') return 0; // Per uscire dal programma
           if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.pdf");
   	          
        }
        cout<<"---------------"<<endl;
        timeinit=Timestamp;
        anode->Reset("ICES");
        anodeTime->Reset("ICES");
        for(int i=0;i<5; i++){
           hrow[i]->Reset("ICES");
           flag[i]=0;
        }
       
        
        if(Row==4 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[0]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[1]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==2 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==1 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[3]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
        }
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,5,Charge); flag[4]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }
          
      } 
        
   }
      
}
