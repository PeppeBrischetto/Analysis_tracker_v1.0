//**************************************************************************************************//
//
//  READ ME!
//  This macro merges five ROOT files into a new ROOT file. 
//  ATTENTION: NO SORTING is done.
//  Remember to put the right path to the input files.
//  Created by G. A. Brischetto.
//
//**************************************************************************************************//

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
   
void C_merge(Int_t nRun) {
   
   TString fileInName;
   TString treeInName = "Data_R";

   const int nRows = 5;
   Int_t row[nRows] = {0, 1, 2, 3, 4};

   TString fileOutName;
   fileOutName.Form("../Cluster/Cluster_merged/run_%d/cluster_merg_Run%d.root", nRun, nRun);

   TChain *chain = new TChain(treeInName);
   
   for (Int_t i=0; i<nRows; i++) {
       fileInName.Form("../Cluster/Cluster_rows/run_%d/cluster_Run%d_Row%d.root", nRun, nRun, row[i]);
       TFile *fileIn = new TFile(fileInName, "read");
       if (fileIn->IsZombie()) {
         std::cout << "Error opening data file" << std::endl;
         return;
       } else 
         std::cout << "File correctly opened" << std::endl;
         
       chain->Add(fileInName);
       
       fileIn->Close();
   }

   //chain->SetName("mychain"); // This can be uncommentend in case you want to change the name of the output tree, otherwise it is the same of the input ones

   chain->Merge(fileOutName);
 
} 
