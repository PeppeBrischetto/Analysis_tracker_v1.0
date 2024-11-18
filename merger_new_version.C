//**************************************************************************************************//
//
//  READ ME!
//  This macro merges five ROOT files into a new ROOT file. 
//  ATTENTION: NO SORTING is done.
//             The macro takes as input the run number (just as an integer).
//             If needed, the digitizers ID can be changed.
//             Remember to put the right path to the input files.
//
//  Created by G. A. Brischetto.
//
//  Last update: 2024-11-18
//
//**************************************************************************************************//

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
   
void merger_new_version(Int_t nRun) {
   
   TString fileInName;
   TString treeInName = "Data_R";

   const int nDigitizers = 5;  // total number of the digiters used in the run. For the moment it is 5 because the macro works only for the tracker
   Int_t digitizers[nDigitizers] = {21247, 22642, 22643, 22644, 22645};  // digitizers ID

   TString fileOutName;
   fileOutName.Form("/home/numen/Analysis_tracker_v1.0/Merged_data/run_%d/merg_%d_prova.root", nRun, nRun);

   TChain *chain = new TChain(treeInName);
   
   for (Int_t i=0; i<nDigitizers; i++) {
       fileInName.Form("/home/numen/Analysis_tracker_v1.0/Cal_data/run_%d/dig_%d_cal.root", nRun, digitizers[i]);
       TFile *fileIn = new TFile(fileInName, "read");
       if (fileIn->IsZombie()) {
         std::cerr << "Error opening data file" << std::endl;
         return;
       } else 
         std::cout << "File correctly opened" << std::endl;
         
       chain->Add(fileInName);
       
       fileIn->Close();
   }

   //chain->SetName("mychain"); // This can be uncommentend in case you want to change the name of the output tree, otherwise it is the same of the input ones

   chain->Merge(fileOutName);
 
} 
