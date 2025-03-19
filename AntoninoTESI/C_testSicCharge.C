//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created 7 Jun 2024 by A. Pitronaci, based on C_plot_phi.C available in the present directory
//#######################################
//#   updated: 10 June 2024, extended for no-segmeneted rows, i.e. 5,6,7,8,9,10 (strips)
//# 
//###################################################################################################

void C_testSicCharge()
{
   /* Main variables referred to input-tree */
   UShort_t Board = 0;
   UShort_t Channel = 0;
   UShort_t FineTSInt = 0;
   ULong_t CoarseTSInt = 0;
   ULong_t Timestamp = 0;
   UShort_t Charge = 0;
   UInt_t Flags = 0;
   double Charge_cal = 0;

   TFile *fileIn = new TFile("../Merged_data/run_128/sic_128.root");
   TTree *treeTracks = (TTree*)fileIn->Get("Data_R");
   
   /* Setting tree-Branches */
   treeTracks->SetBranchAddress("Board", &Board);
   treeTracks->SetBranchAddress("Channel", &Channel);
   treeTracks->SetBranchAddress("FineTSInt", &FineTSInt);
   treeTracks->SetBranchAddress("CoarseTSInt", &CoarseTSInt);
   treeTracks->SetBranchAddress("Timestamp", &Timestamp);
   treeTracks->SetBranchAddress("Charge",&Charge);
   treeTracks->SetBranchAddress("Flags",&Flags);
   treeTracks->SetBranchAddress("Charge_cal",&Charge_cal);

   
   /* Start reading block */
   int entries=treeTracks->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
   
   /* Filling histogram for SiC charge */
   TH1I *histosic = new TH1I("histosic","",500,0.,5000.);
   histosic->GetXaxis()->SetTitle("Charge (a.u.)");
   histosic->GetYaxis()->SetTitle("Counts");   
   
   //treeTracks->Project("histosic", "Charge");

   for(Int_t i=0;i<entries/2;i++){
      treeTracks->GetEntry(i);
      histosic->Fill(Charge);
      //cout << "**** " << Charge << endl;
      //if(Charge>480)
        //cout << "Entry out of symmetry "<< i << endl;
      
      /*if(Charge<450)
        {
           histosic->Fill(Charge);
        }*/
   }
  
   /* Visualization block */
   TCanvas *c = new TCanvas();
   c->cd();
   histosic->Draw();
   
   
   
   }
