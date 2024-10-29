//###################################################################################################
//#   plot energy spectrum of the cluster for each raw
//#      
//#   required as argument the run number
//#            
//###################################################################################################
//#   created october 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

void C_plot_clusterCharge(int run)
{

//###################################################################
//    VARIABLES

   // input file variables
   double cl_charge[11];	   	// charge sum of the pads belonging to a cluster
   Int_t cl_padMult[5];		// number of pads of a cluster
   double cl_x[5];			// x centroid of a cluster in pads unit
   double cl_x_mm[5];			// x centroid of a cluster in mm
   double cl_x_rms[5];  		// rms of the charge distribution of a cluster in pads unit
   double cl_y[5];		// y centroid of a cluster in time
   double cl_y_mm[5];		// y centroid of a cluster in mm
   Double_t theta;		// theta of the track in rad
   Double_t theta_deg;		// theta of the track in deg
   Double_t phi;
   Double_t phi_deg;
   Double_t chiSquareTheta;
   Double_t chiSquarePhi;   
   Int_t sic_charge;
   Double_t energySic; 
   Int_t a_pads_fired[5][100];
   double slopeT, interceptT;
   double slopeP, interceptP;
   int sic_fired;

   int entries;

   int qualityFlag=0;
   int flagA=0;
   double totalCharge=0;

   char pippo;

// open file
   char fileIn[50];
   if(run<10){
      sprintf(fileIn, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i.root", run);
   } 
//      sprintf(fileIn, "../Tracks/tracks_run%i_B.root", run);
   cout<<fileIn<<endl;

   TFile *f = new TFile(fileIn);
   TTree *tree = (TTree*)f->Get("Data_R");
   
   tree->SetBranchAddress("cl_x",&cl_x);
   tree->SetBranchAddress("cl_x_mm",&cl_x_mm);
   tree->SetBranchAddress("cl_y",&cl_y);
   tree->SetBranchAddress("cl_y_mm",&cl_y_mm);
   tree->SetBranchAddress("cl_x_rms",&cl_x_rms);
   tree->SetBranchAddress("cl_charge",&cl_charge);
   tree->SetBranchAddress("cl_padMult0",&cl_padMult[0]);
   tree->SetBranchAddress("cl_padMult1",&cl_padMult[1]);
   tree->SetBranchAddress("cl_padMult2",&cl_padMult[2]);
   tree->SetBranchAddress("cl_padMult3",&cl_padMult[3]);
   tree->SetBranchAddress("cl_padMult4",&cl_padMult[4]);
   tree->SetBranchAddress("pads_fired0",&a_pads_fired[0]);
   tree->SetBranchAddress("pads_fired1",&a_pads_fired[1]);
   tree->SetBranchAddress("pads_fired2",&a_pads_fired[2]);
   tree->SetBranchAddress("pads_fired3",&a_pads_fired[3]);
   tree->SetBranchAddress("pads_fired4",&a_pads_fired[4]);
   
   tree->SetBranchAddress("theta",&theta);
   tree->SetBranchAddress("phi",&phi);
   tree->SetBranchAddress("theta_deg",&theta_deg);
   tree->SetBranchAddress("phi_deg",&phi_deg);
   tree->SetBranchAddress("chiSquareTheta",&chiSquareTheta);   
   tree->SetBranchAddress("chiSquarePhi",&chiSquarePhi);      
   tree->SetBranchAddress("interceptT",&interceptT);
   tree->SetBranchAddress("slopeT",&slopeT);
   tree->SetBranchAddress("interceptP",&interceptP);
   tree->SetBranchAddress("slopeP",&slopeP);
   
   
   entries=tree->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
   
   TCanvas *C1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   C1->SetFillColor(kWhite);
   
   TH1D *histoCC[5];
   histoCC[0]=new TH1D("","",1000,0,100000);
   histoCC[1]=new TH1D("","",1000,0,100000);
   histoCC[2]=new TH1D("","",1000,0,100000);
   histoCC[3]=new TH1D("","",1000,0,100000);
   histoCC[4]=new TH1D("","",1000,0,100000);
   histoCC[0]->SetStats(0);
   histoCC[0]->GetXaxis()->SetTitle("charge");
   histoCC[0]->GetYaxis()->SetTitle("counts");
   
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      tree->GetEntry(i);
      for(int j=0;j<5;j++){histoCC[j]->Fill(cl_charge[j]);}
   
   }
   
   histoCC[0]->Draw();
   histoCC[0]->SetLineColor(kRed);
   histoCC[1]->Draw("same");
   histoCC[1]->SetLineColor(kBlue);
   histoCC[2]->Draw("same");
   histoCC[2]->SetLineColor(kGreen);
   histoCC[3]->Draw("same");
   histoCC[3]->SetLineColor(kViolet);
   histoCC[4]->Draw("same");
   histoCC[4]->SetLineColor(kOrange);
 }
 
 
