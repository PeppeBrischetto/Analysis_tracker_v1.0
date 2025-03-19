//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created 7 Jun 2024 by A. Pitronaci, based on C_plot_theta.C available in the present directory
//#######################################
//#   updated: 10 June 2024, extended for no-segmeneted rows, i.e. 5,6,7,8,9,10 (strips)
//# 
//###################################################################################################

void C_plot_sicCharge(int run)
{
   /* Main variables referred to input-tree */
   UShort_t ChannelSic;
   UShort_t FTSSic;
   ULong64_t CTSSic;
   ULong64_t TimestampSic;
   UShort_t BoardSic;
   UShort_t ChargeSic;
   Double_t Charge_calSic;
   UInt_t FlagsSic;
   
   Int_t counts = 0;
   
   /* Reading block variables */  
   char fileInName[50];
   sprintf(fileInName, "../Merged_data/run_%i/sic_%i.root", run,run);
    
   TFile *fileIn = new TFile(fileInName);
   TTree *treeSic = (TTree*)fileIn->Get("Data_R");
   
   /* Setting tree-Branches */
   treeSic->SetBranchAddress("Board",&BoardSic);
   treeSic->SetBranchAddress("Channel",&ChannelSic);
   treeSic->SetBranchAddress("FineTSInt",&FTSSic);
   treeSic->SetBranchAddress("CoarseTSInt",&CTSSic);
   treeSic->SetBranchAddress("Timestamp",&TimestampSic);
   treeSic->SetBranchAddress("Charge",&ChargeSic);
   treeSic->SetBranchAddress("Flags",&FlagsSic);
   treeSic->SetBranchAddress("Charge_cal",&Charge_calSic);
   
   /* Start reading block */
   int entries=treeSic->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
     
   TH1F *sicCharge = new TH1F("sicCharge","",1000,1000,9000);
   sicCharge->GetXaxis()->SetTitle("Charge_{SiC} (a.u.)");
   sicCharge->GetYaxis()->SetTitle("Counts");
   
   TH2F *chargeTimestamp = new TH2F("chargeTimestamp", "", 10000,0,80000,10000,0,800000);
      
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeSic->GetEntry(i);
      sicCharge->Fill(ChargeSic);
      chargeTimestamp->Fill(ChargeSic,TimestampSic);
      if(ChargeSic>1000 && ChargeSic<9000){
          counts++;
        }
    }
    
    cout << "Count under Charge in [1000;9000]: " << counts << endl;
    
    treeSic->GetEntry(entries-1);
    
    Int_t Delta_time_sec = TimestampSic/1E+12;
    Double_t rate =(double)counts/Delta_time_sec;
    
    cout << "Entries: " << entries << "    Delta_time_sec: " << Delta_time_sec << "   Rate: " << rate << endl;
   
   
   /* Solid angle estimation */
   Double_t a,b,c,d;
   a = 306;                                       //distance source-collimator
   b = 2.46;                                       // radious of collimator
   c = b*b*3.1415/(a*a);                          // solid angle of the collimator
   d = rate * 4 * 3.1415/c;                       // activity
   
   cout << "Solid angle: " << c << "   Activity: " << d << endl;
   
   /* Visualisation Block*/   
   TCanvas *c1=new TCanvas("c1","SiC Charge energy distribution",1600, 100,1000.,600.);
   //c1->Divide(2,1);
   c1->cd();
   //c1->SetLogy();
   c1->SetFillColor(kWhite);
   sicCharge->Draw();
   //c1->cd(2);
   //chargeTimestamp->Draw("COLZ");

   
 }
