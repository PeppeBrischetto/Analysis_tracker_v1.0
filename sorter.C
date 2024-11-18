//***************************************************************************************************************************************//
// Macro that sorts the elements of a tree by selecting a specific branch (e.g. Timestamp) as the one to sort out the entries.
// It takes in input a ROOT file in the "Merged_data" directory and in the same directory
// produces in output a ROOT file where the entries are sorted according to their timestamp value.
//
// Created by G. A. Brischetto.
//
// Last update: 2024-11-18
//
//**************************************************************************************************************************************//

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
   
void sorter(Int_t nRun) {
   
   TString fileInName, fileOutName;
   fileInName.Form("/home/numen/Analysis_tracker_v1.0/Merged_data/run_%d/merg_%d_prova.root", nRun, nRun);
   fileOutName.Form("/home/numen/Analysis_tracker_v1.0/Merged_data/run_%d/sort_%d.root",nRun, nRun);
   TFile *fileIn = new TFile(fileInName,"read");  //the input file
   if (fileIn->IsZombie()) {
      std::cerr << "Error opening data file" << std::endl;
      return;
   } else 
      std::cout << "File correctly opened" << std::endl;
   
   TTree *treeIn = (TTree*)fileIn->Get("Data_R"); //get the input Tree
   ULong64_t N = treeIn->GetEntries();
   std::cout << "Total entries = " << N << std::endl;
   
   TFile *fileOut = new TFile(fileOutName,"recreate");
   TTree *treeOut = treeIn->CloneTree(0);
   
   ULong64_t *index = new ULong64_t[N];
   ULong64_t indexNo;

   //treeIn->SetEstimate(-1); // this should be uncommented if the expected number of selected rows is greater than 10^6
   treeIn->Draw("Timestamp","","goff"); // the function "draw" is used to generate the "Timestamp" variable
   Double_t *var = treeIn->GetV1();
   
   TMath::Sort(N, var, index, 0); // the array "index" contains the indices of the sorted array
   
   for (indexNo=0; indexNo<N; indexNo++) {
      treeIn->GetEntry(index[indexNo]);
      treeOut->Fill();
      //std::cout <<"index(i) = " << index[indexNo] << " i = "<< indexNo <<std::endl;   
   }

   //treeOut->Print();
   treeOut->Write();
   
} 
