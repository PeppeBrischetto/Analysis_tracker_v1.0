//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event, and the Time window
//#            where to seek the tracks.
//# 	       The array with the number of digitizers used			   array with the number of digitizers used
//#  alpha is the angle taken as atan of the slope of the linear fit 
//#  theta instead is the angle respect to the z-axis 
//#  theta is defined as  theta=-90-alpha  (it is negative since the x goes from rigth to left).
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

void C_plot_phi(int run)
{

   Int_t n_pads_fired[5]={0};
   double centroid[5], centroid_mm[5];
   double rms[5];
   double total_charge[5] = {0};
   double yrow[5] = {0.}; 
   Double_t theta_deg;  
   Double_t phi_deg;
   Int_t sic_fired=0;  
   Double_t energySic;
  
   
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
   

   treeTracks->SetBranchAddress("centroid", centroid);
   treeTracks->SetBranchAddress("rms", rms);
   treeTracks->SetBranchAddress("n_pads_fired",n_pads_fired);
   treeTracks->SetBranchAddress("centroid_mm", centroid_mm);
   treeTracks->SetBranchAddress("total_charge", total_charge);
   treeTracks->SetBranchAddress("yrow", yrow);
   treeTracks->SetBranchAddress("theta_deg",&theta_deg);
   treeTracks->SetBranchAddress("phi_deg",&phi_deg);
   treeTracks->SetBranchAddress("sic_fired",&sic_fired);
   treeTracks->SetBranchAddress("energySic",&energySic);
   
   
   int entries=treeTracks->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
   
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1F *histoPhi=new TH1F("","",1000,-50,50);
   histoPhi->SetStats(0);
   histoPhi->GetXaxis()->SetTitle("charge");
   histoPhi->GetYaxis()->SetTitle("counts");
   
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeTracks->GetEntry(i);
      histoPhi->Fill(phi_deg);
   
   }
   
   histoPhi->Draw();
   
 }
