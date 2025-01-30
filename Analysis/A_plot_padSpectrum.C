//###################################################################################################
//#   macro that take as input a merged file, plot the charge spectrum of a single pad
//#   It divides the total length of a run in ten sections and fill ten histrograms
//#   
//#   required as argument the run number
//###################################################################################################
//#   created January 2023   D. Torresi
//#######################################
//


void A_plot_padSpectrum(int run)
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
   UShort_t Row;

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
   tree->SetBranchAddress("Row",&Row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *chargepad[10];
   for(int i=0; i<10; i++){
      chargepad[i]=new TH1F("ep","ep",4000,-0.5,63999.5);
      chargepad[i]->GetXaxis()->SetTitle("charge");
      chargepad[i]->GetYaxis()->SetTitle("counts");
   }

   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
 
   int k=0;
   double intpart, fracpart;
   
   cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      modf((((float)i/entries)*10),&intpart);
      if(pad==20 && Row==0){chargepad[(int)intpart]->Fill(Charge);}                 
   }
     
   for(int i=0; i<10; i++){
      chargepad[i]->SetLineColor(i);
      chargepad[i]->Draw("same");   
   }
  
   char naam[100];
   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   for(int i=0; i<10; i++){
      sprintf(naam,"Pad %d",i);
      leg->AddEntry(chargepad[i], naam, "l");
   }
   leg->Draw();
   
}
