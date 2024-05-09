//###################################################################################################
//#   macro that take as input a merged file, plot the charge spectrum with a condition on the multiplicity
//#   of hit inside a time window
//#   
//#   required as argument the run number
//###################################################################################################
//#   created May 2024   D. Torresi
//#######################################
//


void A_plot_padSpectrum_2(int run)
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
     
   // window the define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;
   // row multiplicity required to plot the event
   int rowMult=0; 
   // multiplicity of hit inside the time window DeltaT
   int multiplicity=0;
   
   const int maxMultiplicity=30;
   int chargeA[100];
   
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
   /*TH1F *chargepad[64];
   for(int i=0; i<64; i++){
      chargepad[i]=new TH1F("ep","ep",64100,-0.5,64099.5);
      chargepad[i]->GetXaxis()->SetTitle("charge");
      chargepad[i]->GetYaxis()->SetTitle("counts");
   }*/

   TH1F *Totpad=new TH1F("ep","ep",6410,-0.5,64099.5);    
   TH1F *LMpad=new TH1F("ep1","ep1",6410,-0.5,64099.5);
   TH1F *HMpad=new TH1F("ep2","ep2",6410,-0.5,64099.5);
   TH1F *MultH=new TH1F("Mult","ep1",101,-0.5,100.5);
   

   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
 
   int k=0;
 
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   chargeA[multiplicity++]=Charge;
   Totpad->Fill(Charge); 
   
   //cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=1;i <entries/10; i++){
      tree->GetEntry(i);
      Totpad->Fill(Charge);
      
      if((Timestamp-timeinit)<DeltaT){
         chargeA[multiplicity++]=Charge;
      }else{
         //cout<<"coiao"<<endl;

         if(multiplicity>maxMultiplicity){
            //fill the histo with high multiplicity
            //for(int j=0; j<64; j++){
            //if(pad==j){
            for(int r=0;r<multiplicity; r++)HMpad->Fill(chargeA[r]);
         }
         if(multiplicity<=maxMultiplicity){
            //fill the histo with low multiplicity
            for(int r=0;r<multiplicity; r++){
               LMpad->Fill(chargeA[r]);
               //cout<<"pippo: "<<r<<"  "<<chargeA[r]<<endl;
            }
            
         }
         // zero multiplicity and charge array
         MultH->Fill(multiplicity);
         timeinit=Timestamp;
         multiplicity=0;
         for(int g=0; g<100; g++){chargeA[g]=0;}
         chargeA[multiplicity++]=Charge;   
      }
      
      //for(int j=0; j<64; j++){
      //  if(pad==j){
      //     chargepad[j]->Fill(Charge);
      //  }                 
      //}  
   }

   Totpad->SetLineColor(kBlack);
   Totpad->Draw();   
   
   LMpad->SetLineColor(kBlue);
   LMpad->Draw("same");   
   
   HMpad->SetLineColor(kRed);
   HMpad->Draw("same");   
  
   char naam[100];
   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   /*for(int i=20; i<25; i++){
      sprintf(naam,"Pad %d",i);
      leg->AddEntry(chargepad[i], naam, "l");
   }*/
  
   leg->AddEntry(LMpad, " total", "l"); 
   leg->AddEntry(LMpad, " low mult (<=15)", "l");
   leg->AddEntry(HMpad, " high mult (>15)", "l");
   leg->Draw();
  
   TCanvas *C2=new TCanvas("c2","c2",1200,550,900,400); 
   MultH->Draw();
   
}
