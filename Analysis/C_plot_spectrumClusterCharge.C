//###################################################################################################
//#   plot theta spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created may 2024 by D. Torresi
//#######################################
//#   updated: November 2024 by D. Torresi
//# 
//###################################################################################################

void C_plot_spectrumClusterCharge(int run)
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

   double totalCharge;
   char histoname[20];

   int entries;
   int flagA=0;
   char anykey;


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
   
   // Sic variables
   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("sic_charge",&sic_charge);
   tree->SetBranchAddress("energySic",&energySic);

   entries=tree->GetEntries();

//#################################################################################################
// GRAPHICS
   TCanvas *C1=new TCanvas("c1","c1",250,160,800,600);  

   //TCanvas *C2=new TCanvas("c2","c2",750,160,800,600);   
   
   TH1D *chargeCluster[11];
   for(int h=0; h<11; h++){
      sprintf(histoname,"clch%i",h); 
      chargeCluster[h]=new TH1D(histoname,histoname,6410,-0.5,320099.5);
      chargeCluster[h]->GetXaxis()->SetTitle("charge");
      chargeCluster[h]->GetYaxis()->SetTitle("counts");
   }
 
   TH1F *HTotalCharge = new TH1F("toch","toch",6410,-0.5,320499.5);
   
     
   //TH2D *bg=new TH2D("","bg",6410,-0.5,320499.5,1000,0,10000);
   //bg->Draw();   
  
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries-1;i++){
      tree->GetEntry(i);
      cout<<"entry "<<i<<"  #";
      totalCharge=0;
      for(int j=0; j<5; j++){
         chargeCluster[j]->Fill(cl_charge[j]);  
         totalCharge=totalCharge+cl_charge[j];
         cout<<cl_charge[j]<<"  ";
      }
      cout<<endl;
      HTotalCharge->Fill(totalCharge);
          
   }
   
   
   HTotalCharge->Draw();
   HTotalCharge->SetLineColor(kBlack);
   
   chargeCluster[0]->SetLineColor(kRed);   
   chargeCluster[0]->Draw("same");
   chargeCluster[1]->Draw("same");
   chargeCluster[1]->SetLineColor(kGreen);
   chargeCluster[2]->Draw("same");
   chargeCluster[2]->SetLineColor(kBlue);
   chargeCluster[3]->Draw("same");
   chargeCluster[3]->SetLineColor(kViolet);
   chargeCluster[4]->Draw("same");
   chargeCluster[4]->SetLineColor(kOrange);

   
   
   
   
   
}   
   




