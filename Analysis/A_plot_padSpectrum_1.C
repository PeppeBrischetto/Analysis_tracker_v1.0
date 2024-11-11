//###################################################################################################
//#   macro that take as input a merged file, plot the charge spectrum of multiple pads
//#   in the same plot
//#   
//#   required as argument the run number
//###################################################################################################
//#   created May 2024   D. Torresi
//#######################################
//


void A_plot_padSpectrum_1(int run)
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
   tree->SetBranchAddress("Row",&Row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   char histoName[10];

   // Dichiarazione Histo 
   TH1F *chargepad[64];
   for(int i=0; i<64; i++){
      sprintf(histoName,"pad%i",i);
      chargepad[i]=new TH1F(histoName,histoName,4000,-0.5,64099.5);
      chargepad[i]->GetXaxis()->SetTitle("charge");
      chargepad[i]->GetYaxis()->SetTitle("counts");
   }


 
   int k=0;
   double intpart, fracpart;
   
   //cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries/10; i++){
      tree->GetEntry(i);
      //modf((((float)i/entries)*10),&intpart);
      
      for(int j=0; j<64; j++){
        if(pad==j && Row==0){
           chargepad[j]->Fill(Charge);
        }                 
      }
      
   }
   
   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   chargepad[10]->Draw();   
  
   for(int i=20; i<27; i++){
      chargepad[i]->SetLineColor(i-20);
      chargepad[i]->Draw("same");   
   }
  
   char naam[100];
   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   for(int i=20; i<27; i++){
      sprintf(naam,"Pad %d",i);
      leg->AddEntry(chargepad[i], naam, "l");
   }
   leg->Draw();
 
   TCanvas *C2=new TCanvas("c2","c2",800,100,900,400);
   C2->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
 
   for(int i=27; i<32; i++){
      chargepad[i]->SetLineColor(i-27);
      chargepad[i]->Draw("same");   
   }
  
   TLegend *leg1= new TLegend(0.7,0.65,0.9,0.9);
   for(int i=27; i<32; i++){
      sprintf(naam,"Pad %d",i);
      leg1->AddEntry(chargepad[i], naam, "l");
   }
   leg1->Draw();
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
}
