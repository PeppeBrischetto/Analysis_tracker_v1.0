//###################################################################################################
//#   macro that take as input a merged file, plot a 2D histo of the charge as a function of the pad 
//#   number for each row
//#   
//#   required as argument the run number
//###################################################################################################
//#   created March 2024   D. Torresi
//#######################################
//


void A_plot_EnergyVsPad(int run)
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

   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);

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
      digit[i]->GetXaxis()->SetTitle("pad");
      digit[i]->GetYaxis()->SetTitle("energy");
   }

   // Ciclo sui dati   
   for(int i=0; i <entries; i++){
      tree->GetEntry(i);
      if(row<5){digit[row]->Fill(pad,Charge);}
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
