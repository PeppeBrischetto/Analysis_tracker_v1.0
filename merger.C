// Macro to merge two root files. The input is from Cal_data and the output also to Cal_data. Please transfer manualy the final file to Merged_data and delete the intermediate files.
//Last Update: 15 Nov 2022	-> some comments were added.

#include <iostream>
#include "TFile.h"
#include "TTree.h"

void merger (const char *data_file_name0, const char *data_file_name1, const char *output_file_name) {

   // NB: Board and channel numbers start at 0

   // Branch variables of raw data tree0
   UShort_t Single_Board_0;
   UShort_t Single_Channel_0;
   UShort_t Single_FTS_0;
   ULong64_t Single_CTS_0;
   ULong64_t Single_Time_ps_0;
   UShort_t Single_Charge_0;
   UInt_t Single_Flags_0;
   UShort_t Single_Pads_0;
   Double_t Single_Charge_cal_0;
   UShort_t Single_Row_0;
   UShort_t Single_Section_0;

   // Branch variables of raw data tree1
   UShort_t Single_Board_1;
   UShort_t Single_Channel_1;
   UShort_t Single_FTS_1;
   ULong64_t Single_CTS_1;
   ULong64_t Single_Time_ps_1;
   UShort_t Single_Charge_1;
   UInt_t Single_Flags_1;
   UShort_t Single_Pads_1;
   Double_t Single_Charge_cal_1;
   UShort_t Single_Row_1;
   UShort_t Single_Section_1;


   // OPEN DATA FILE 0 AND GET TREE
   TFile *datafile0 = new TFile(data_file_name0);
   if (datafile0->IsZombie()) {
      cout << "Error opening data file" << std::endl;
      return;
   }
   TTree *raw_tree0 = (TTree*)datafile0->Get("Data_R");
  
   raw_tree0->SetBranchAddress("Board",&Single_Board_0);
   raw_tree0->SetBranchAddress("Channel",&Single_Channel_0);
   raw_tree0->SetBranchAddress("FineTSInt",&Single_FTS_0);
   raw_tree0->SetBranchAddress("CoarseTSInt",&Single_CTS_0);
   raw_tree0->SetBranchAddress("Timestamp",&Single_Time_ps_0);
   raw_tree0->SetBranchAddress("Charge",&Single_Charge_0);
   raw_tree0->SetBranchAddress("Flags",&Single_Flags_0);
   raw_tree0->SetBranchAddress("Pads",&Single_Pads_0);
   raw_tree0->SetBranchAddress("Charge_cal",&Single_Charge_cal_0);
   raw_tree0->SetBranchAddress("Row",&Single_Row_0);
   raw_tree0->SetBranchAddress("Section",&Single_Section_0);

   cout << " Entries first input file ( " << data_file_name0<<") : "<<raw_tree0->GetEntries() << std::endl;

   // OPEN DATA FILE 1 AND GET TREE
   TFile *datafile1 = new TFile(data_file_name1);
   if (datafile1->IsZombie()) {
      std::cout << "Error opening data file" << std::endl;
      return;
   }
   TTree *raw_tree1 = (TTree*)datafile1->Get("Data_R");
  
   raw_tree1->SetBranchAddress("Board",&Single_Board_1);
   raw_tree1->SetBranchAddress("Channel",&Single_Channel_1);
   raw_tree1->SetBranchAddress("FineTSInt",&Single_FTS_1);
   raw_tree1->SetBranchAddress("CoarseTSInt",&Single_CTS_1);
   raw_tree1->SetBranchAddress("Timestamp",&Single_Time_ps_1);
   raw_tree1->SetBranchAddress("Charge",&Single_Charge_1);
   raw_tree1->SetBranchAddress("Flags",&Single_Flags_1);
   raw_tree1->SetBranchAddress("Pads",&Single_Pads_1);
   raw_tree1->SetBranchAddress("Charge_cal",&Single_Charge_cal_1);
   raw_tree1->SetBranchAddress("Row",&Single_Row_1);
   raw_tree1->SetBranchAddress("Section",&Single_Section_1);

   cout << " Entries second input file ( "<< data_file_name1 <<") : "<< raw_tree1->GetEntries() << std::endl;

   // Output file
   TFile *out_file = new TFile(output_file_name,"recreate");

   // Output tree
   //TTree *out_tree = new TTree("event_tree","merged data tree");
   TTree *out_tree = new TTree("Data_R","merged data tree");

   UShort_t Board;
   UShort_t Channel;
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Charge;
   UInt_t Flags;
   UShort_t Single_Pads;
   Double_t Single_Charge_cal;
   UShort_t Single_Row;
   UShort_t Single_Section;
  
   out_tree->Branch("Board",&Board,"Board/s");
   out_tree->Branch("Channel",&Channel,"Channel/s");
   out_tree->Branch("FineTSInt",&FTS,"FineTSInt/s");
   out_tree->Branch("CoarseTSInt",&CTS,"CoarseTSInt/l");
   out_tree->Branch("Timestamp",&Timestamp,"Timestamp/l");
   out_tree->Branch("Charge",&Charge,"Charge/s");
   out_tree->Branch("Flags",&Flags,"Flags/i");
   out_tree->Branch("Pads",&Single_Pads,"Pads/s");
   out_tree->Branch("Charge_cal",&Single_Charge_cal,"Charge_cal/D");
   out_tree->Branch("Row",&Single_Row,"Row/s");
   out_tree->Branch("Section",&Single_Section,"Section/s");
 
   ULong64_t nwritten0 = 0;
   ULong64_t nwritten1 = 0;
  
   Long64_t ientry0 = 0;
   raw_tree0->GetEntry(0);
  
   //raw_tree1->GetEntry(0);
 
   //scan events of "1" (1st for loop)
   for ( Long64_t ientry1 = 0; ientry1<raw_tree1->GetEntries() ; ientry1++ ) {
      raw_tree1->GetEntry(ientry1);
   
      //while time of "0" is smaller than time of "1", write the event of "0"
      while ( Single_Time_ps_1>Single_Time_ps_0 && ientry0<raw_tree0->GetEntries()) {
         
         Board = Single_Board_0;
         Channel = Single_Channel_0;
         FTS = Single_FTS_0;
         CTS = Single_CTS_0;
         Timestamp = Single_Time_ps_0;
         Charge = Single_Charge_0;
         Flags = Single_Flags_0;
         Single_Pads = Single_Pads_0;
         Single_Charge_cal = Single_Charge_cal_0;
         Single_Row=Single_Row_0;
         Single_Section=Single_Section_0;
          
         out_tree->Fill();
         nwritten0++;
  
         ientry0++;
         if(ientry0<raw_tree0->GetEntries()){
	    raw_tree0->GetEntry(ientry0);
         }
   
      } //end of while
 
      //if while is not true write "1"
      Board = Single_Board_1;
      Channel = Single_Channel_1;
      FTS = Single_FTS_1;
      CTS = Single_CTS_1;
      Timestamp = Single_Time_ps_1;
      Charge = Single_Charge_1;
      Flags = Single_Flags_1;
      Single_Pads = Single_Pads_1;
      Single_Charge_cal = Single_Charge_cal_1;
      Single_Row=Single_Row_1;
      Single_Section=Single_Section_1;
          
      out_tree->Fill();
      nwritten1++;
   } //end of 1st for loop
 

   //scan rest of events of "0", if any (2nd for loop)

   for ( ; ientry0<raw_tree0->GetEntries() ; ientry0++ ) {
      raw_tree0->GetEntry(ientry0);

      Board = Single_Board_0;
      Channel = Single_Channel_0;
      FTS = Single_FTS_0;
      CTS = Single_CTS_0;
      Timestamp = Single_Time_ps_0;
      Charge = Single_Charge_0;
      Flags = Single_Flags_0;
      Single_Pads = Single_Pads_0;
      Single_Charge_cal = Single_Charge_cal_0;

      out_tree->Fill();
      nwritten0++;

   } //end of 2nd for loop


   std::cout << "Events written to the output tree: " << out_tree->GetEntries() << std::endl;
   std::cout << "Events from input file 0: " << nwritten0 << std::endl;
   std::cout << "Events from input file 1: " << nwritten1 << std::endl;
   //std::cout << "Events from input file 0 (test): " << ientry0 << std::endl;
   //std::cout << "Events from input file 1 (test): " << ientry1 << std::endl;

   out_file->cd();
   out_tree->Write();
   out_file->Close();

   datafile0->Close();
   datafile1->Close();

   return;
}
