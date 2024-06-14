//###################################################################################################
//#   macro that take as input a merged file, plot a 2D histo of the charge as a function of the digitizer channel
//#   for each digitizer
//#   
//#   required as argument the run number
//###################################################################################################
//#   created March 2024   D. Torresi
//#######################################
//


void A_plot_EnergyVsChannel(int run)
{

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
   UShort_t row;

   Int_t board_id[20]={22642,22643,22644,22645,21247};

   // Apertura file
   char fileIn[50];
   if(run<10){
      sprintf(fileIn, "../Merged_data/run_00%i/merg_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileIn, "../Merged_data/run_0%i/merg_0%i.root", run, run);
   }else{
      sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);
   }   

   TFile *fin = new TFile(fileIn);
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
   tree->SetBranchAddress("Row",&row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH2F *digit[5];
   for(int i=0; i<5; i++){
      digit[i]=new TH2F("dig","dig",64,-0.5,63.5, 6400,0.5,64000.5);
      digit[i]->GetXaxis()->SetTitle("Channel");
      digit[i]->GetYaxis()->SetTitle("energy");
      digit[i]->SetStats(0);
   }

   // Ciclo sui dati   
   for(int i=0; i <1000000; i++){
      tree->GetEntry(i);
      cout<<"entry: "<<i<<endl;
      if(Board==board_id[0]){digit[0]->Fill(Channel,Charge);}
      if(Board==board_id[1]){digit[1]->Fill(Channel,Charge);}
      if(Board==board_id[2]){digit[2]->Fill(Channel,Charge);}
      if(Board==board_id[3]){digit[3]->Fill(Channel,Charge);}
      if(Board==board_id[4]){digit[4]->Fill(Channel,Charge);}
      
   }
   
   TCanvas *C1=new TCanvas("c1","c1",100,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   digit[0]->Draw("colz");   

   
   TCanvas *C2=new TCanvas("c2","c2",1000,50,900,400);
   C2->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   digit[1]->Draw("colz");   
   
   TCanvas *C3=new TCanvas("c3","c3",100,500,900,400);
   C3->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   digit[2]->Draw("colz");   

   TCanvas *C4=new TCanvas("c4","c4",1000,500,900,400);
   C4->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   digit[3]->Draw("colz");   

   TCanvas *C5=new TCanvas("c5","c5",100,950,900,400);
   C5->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   digit[4]->Draw("colz");   


}
