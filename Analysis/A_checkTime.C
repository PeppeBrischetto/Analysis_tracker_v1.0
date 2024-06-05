// Check that all the event are ordered by the total time (sum of the coarse and fine time).
// if the etry i+1 has a time smaer than entry i give a warning.

void A_checkTime(int run)
{

   // Dichiarazione variabili
   UShort_t Channel; 
   UShort_t pad; 
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Energy;
   Double_t Energy_cal;
   UInt_t Flags;
   char anykey;
   
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
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Energy",&Energy);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Energy_cal",&Energy_cal);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
     
   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      
      //if(Energy>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Energy<<"\t("<<Energy_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((long int)(Timestamp-timeinit)<0){
           
           cout<<"WARNING - entry "<<i<<endl;
           cout<<"press any key to continue, q to quit"<<endl;
           cin>>anykey;
           if(anykey=='q')return ; // Per uscire dal programma
   	           
        }
      //cout<<"---------------"<<endl;
      timeinit=Timestamp;
 
       
        
   }
      
}
