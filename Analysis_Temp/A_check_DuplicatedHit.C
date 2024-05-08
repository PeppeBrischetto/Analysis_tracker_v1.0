// Check that all the event are ordered by the total time (sum of the coarse and fine time).
// if the etry i+1 has a time smaer than entry i give a warning.

void A_check_DuplicatedHit(int run)
{

   // Dichiarazione variabili
   UShort_t Channel, ChannelPrev; 
   UShort_t FTS, FTSPrev;
   ULong64_t CTS, CTSPrev;   
   ULong64_t Timestamp;
   UShort_t Board, BoardPrev;
   UShort_t Charge, ChargePrev;
   UInt_t Flags, FlagsPrev;
   char anykey;
   
   int contD=0;
   
   
   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_00%i/merg_00%i.root", run, run);
   //sprintf(fileIn, "../aaaa.root", run);
   TFile *fin = new TFile(fileIn);

   TTree *treeTracker = (TTree*)fin->Get("Data_R");
   treeTracker->SetBranchAddress("Board",&Board);
   treeTracker->SetBranchAddress("Channel",&Channel);
   treeTracker->SetBranchAddress("FineTSInt",&FTS);
   treeTracker->SetBranchAddress("CoarseTSInt",&CTS);
   treeTracker->SetBranchAddress("Timestamp",&Timestamp);
   treeTracker->SetBranchAddress("Charge",&Charge);
   treeTracker->SetBranchAddress("Flags",&Flags);

   
   int entries=treeTracker->GetEntries();
   cout<<" "<<entries<<endl;

   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
     
   treeTracker->GetEntry(0);  
   ChargePrev=Charge;
   ChannelPrev=Channel; 
   FTSPrev=FTS;
   CTSPrev=CTS;  
   BoardPrev=Board;
   ChargePrev=Charge;
   FlagsPrev=Flags;

   TH1F *ChargeH=new TH1F("ChargeH","",64000,0,64000);
   TH1F *ChannelH=new TH1F("ChannelH","",64,0,64);
   
   TH1F *ChargeHi=new TH1F("ChargeHi","",64000,0,64000);
   
   TH1F *ChargeHF=new TH1F("ChargeHF","",64000,0,64000);
   TH1F *ChannelHF=new TH1F("ChannelHF","",64,0,64);
   ChannelHF->SetLineColor(kRed);
   ChargeHF->SetLineColor(kRed);
   ChargeHi->SetLineColor(kGreen);

   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=1;i <entries; i++){
      treeTracker->GetEntry(i);
    
      ChargeHF->Fill(Charge);
      ChannelHF->Fill(Channel); 
      if(Charge==ChargePrev){
           
           
           ChargeH->Fill(Charge);
           ChannelH->Fill(Channel);
           if(Channel==60)ChargeHi->Fill(Charge);
           cout<<"WARNING"<<endl;
           cout<<" entry "<<i<<endl;
           cout<<" D hit "<<++contD<<endl;
           
           cout<<" dig: "<<Board<<endl;
           cout<<"---------------------"<<endl;
           cout<<"Charge: \t"<<ChargePrev<<"\t"<<Charge<<endl;
           cout<<"Channel \t"<<ChannelPrev<<"\t"<<Channel<<endl; 
           cout<<"FTS     \t"<<FTSPrev<<"\t"<<FTS<<endl;
           cout<<"CTS     \t"<<CTSPrev<<"\t"<<CTS<<endl;  
           cout<<"Board   \t"<<BoardPrev<<"\t"<<Board<<endl;
           cout<<"Flags   \t"<<FlagsPrev<<"\t"<<Flags<<endl;
           cout<<"---------------------"<<endl;
 
           cout<<"press any key to continue, q to quit"<<endl;
 	   
           cin>>anykey; 	   //*/
           
           if(anykey=='q')return ; // Per uscire dal programma
   	           
      }
      
    ChargePrev=Charge;
    ChannelPrev=Channel; 
    FTSPrev=FTS;
    CTSPrev=CTS;  
    BoardPrev=Board;
    ChargePrev=Charge;
    FlagsPrev=Flags;
      
        
   }
   //cout<<" entry "<<i<<endl;
   //cout<<" D hit "<<contD<<endl;
   
   TCanvas* C1=new TCanvas("c1","c1");
   ChargeH->Draw();
   ChargeHF->Draw("same");
   ChargeHi->Draw("same");

   TCanvas* C2=new TCanvas("c2","c2");      
   ChannelH->Draw();
   ChannelHF->Draw("same");
}

