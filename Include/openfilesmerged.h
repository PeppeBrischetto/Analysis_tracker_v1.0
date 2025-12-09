#ifndef OPENFILESMERGED_H
#define OPENFILESMERGED_H

//#################################################################################################
//  OPENING Tracks file
//#################################################################################################

// file variables for tracks file
   Int_t entries;
   UShort_t Board;
   UShort_t Channel;
   UShort_t FineTSInt;
   ULong64_t CoarseTSInt;
   ULong64_t Timestamp;
   UShort_t Charge;
   UInt_t Flags;
   UShort_t Pads;
   Double_t Charge_cal;
   UShort_t Row;
   UShort_t Section;

   TFile *f;
   TTree *tree; 

// Input file variables for tracks file   


void openMergedFile(int runnum ){
   //open file
   char *filename = new char[100];
   if(runnum<10){
      sprintf(filename, "../Merged_data_Br/run_00%i/merg_00%i.root", runnum,runnum);
   }else if(runnum <100){
      sprintf(filename, "../Merged_data_Br/run_0%i/merg_0%i.root", runnum,runnum);
   }else{
      //sprintf(filename, "../Tracks_Br/tracks_run%i.root", runnum);
      sprintf(filename, "../Merged_data_Br/run_%i/merg_%i.root", runnum,runnum);
   } 

   f=new TFile(filename,"READ");
   cout<<"Opening file:  "<<filename<<endl;
   cout<<"Is the file "<<filename<<" a Zomby? "<< f->IsZombie()<<endl;
   //open the tree
   if(f->IsZombie())cout<<"\n\n\t\t\033[40;31;1m No file found!\033[0m\n\n"<<endl;
   tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("Board", &Board);
   tree->SetBranchAddress("Channel", &Channel);
   tree->SetBranchAddress("FineTSInt", &FineTSInt);
   tree->SetBranchAddress("CoarseTSInt", &CoarseTSInt);
   tree->SetBranchAddress("Timestamp", &Timestamp);
   tree->SetBranchAddress("Charge", &Charge);
   tree->SetBranchAddress("Flags", &Flags);
   tree->SetBranchAddress("Pads", &Pads);
   tree->SetBranchAddress("Charge_cal", &Charge_cal);
   tree->SetBranchAddress("Row", &Row);
   tree->SetBranchAddress("Section", &Section);
   
   
   entries=tree->GetEntries();
   
   cout<<"Total entries: "<<entries<<endl;
   
}

#endif
