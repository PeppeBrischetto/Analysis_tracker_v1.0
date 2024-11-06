//###################################################################################################
//#   plot charge spectra for the SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

void C_plot_ChargeSiC(int run)
{

   double cl_charge[5] = {0};	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5]={0};		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5] = {0};		// y centroid of a cluster in time
   double cl_y_mm[5] = {0};		// y centroid of a cluster in mm
   Double_t theta=-1000;		// theta of the track in rad
   Double_t theta_deg=-1000;		// theta of the track in deg
   Double_t phi=-1000;
   Double_t phi_deg=-1000;
   Int_t sic_charge;
   Double_t energySic;
   int sic_fired;
   Int_t a_pads_fired[5][100];
   
   char fileInName[50];
   if(run<10){
      sprintf(fileInName, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileInName, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileInName, "../Tracks/tracks_run%i.root", run);
   } 
   TFile *fileIn = new TFile(fileInName);
   TTree *treeTracks = (TTree*)fileIn->Get("Data_R");
      
   treeTracks->SetBranchAddress("cl_x", cl_x);
   treeTracks->SetBranchAddress("cl_x_mm", cl_x_mm); 
   treeTracks->SetBranchAddress("cl_y", cl_y);
   treeTracks->SetBranchAddress("cl_y_mm", cl_y_mm);
   treeTracks->SetBranchAddress("cl_x_rms", cl_x_rms);
   treeTracks->SetBranchAddress("cl_padMult0",&cl_padMult[0]);
   treeTracks->SetBranchAddress("cl_padMult1",&cl_padMult[1]);
   treeTracks->SetBranchAddress("cl_padMult2",&cl_padMult[2]);
   treeTracks->SetBranchAddress("cl_padMult3",&cl_padMult[3]);
   treeTracks->SetBranchAddress("cl_padMult4",&cl_padMult[4]);
   treeTracks->SetBranchAddress("pads_fired0",&a_pads_fired[0]);
   treeTracks->SetBranchAddress("pads_fired1",&a_pads_fired[1]);
   treeTracks->SetBranchAddress("pads_fired2",&a_pads_fired[2]);
   treeTracks->SetBranchAddress("pads_fired3",&a_pads_fired[3]);
   treeTracks->SetBranchAddress("pads_fired4",&a_pads_fired[4]);
   treeTracks->SetBranchAddress("phi",&phi);
   treeTracks->SetBranchAddress("theta",&theta);
   treeTracks->SetBranchAddress("phi_deg",&phi_deg);      
   treeTracks->SetBranchAddress("theta_deg",&theta_deg);
   treeTracks->SetBranchAddress("sic_fired",&sic_fired);
   treeTracks->SetBranchAddress("sic_charge",&sic_charge);
   treeTracks->SetBranchAddress("energySic",&energySic);

   int entries=treeTracks->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
   
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *sicCharge=new TH1F("","",2000,0,20000);
   sicCharge->SetStats(0);
   sicCharge->GetXaxis()->SetTitle("charge");
   sicCharge->GetYaxis()->SetTitle("counts");
   
   TH1F *sicChargeNT=new TH1F("","",2000,0,20000);
   TH1F *sicChargeT=new TH1F("","",2000,0,20000);
   
   
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeTracks->GetEntry(i);
      sicCharge->Fill(sic_charge);
      if(sic_fired){
         sicChargeT->Fill(sic_charge);
      }else{
         sicChargeNT->Fill(sic_charge);
      }
      
   }
   sicCharge->SetLineColor(kBlack);
   sicChargeT->SetLineColor(kRed);
   sicChargeNT->SetLineColor(kBlue);
   sicCharge->Draw();
   sicChargeT->Draw("SAME");
   sicChargeNT->Draw("SAME");
   
 }
 
