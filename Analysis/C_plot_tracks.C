//###################################################################################################
//#   macro that plot tracks from files Tracks
//#   
//#   required as argument the run number and y/n for the presence of a SiC file
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created october 2024 from scratch D. Torresi
//#######################################
//#   modified:
//###################################################################################################

void C_plot_tracks(int run){

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
   Double_t sic_charge;
   Double_t energySic; 
   Int_t a_pads_fired[5][100];
   double slope, intercept;
   int sic_fired;

   int entries;

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
   tree->SetBranchAddress("intercept",&intercept);
   tree->SetBranchAddress("slope",&slope);
   
   // Sic variables
   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("sic_charge",&sic_charge);
   tree->SetBranchAddress("energySic",&energySic);

   entries=tree->GetEntries();

//#########################################################
// GRAPHICS

   int xmin=-10;
   int xmax= 310;
   int DeltaX=xmax-xmin;
   int ymin=-80;
   int ymax= 118;
   int DeltaY=ymax-ymin;

   TCanvas *C1=new TCanvas("tracker","tracker",250,160,DeltaX*5,DeltaY*5);
   TH2F *bg=new TH2F("bg","",100,xmin,xmax,100,ymin,ymax); 
   bg->SetStats(0); 
   TLine *edgeLe=new TLine(0,0,0,108);
   TLine *edgeRi=new TLine(300,0,300,108);
   TLine *edgeLo=new TLine(0,0,300,0);
   TLine *edgeUp=new TLine(0,108,300,108);

   bg->Draw();
   edgeLe->SetLineColor(kViolet);
   edgeLe->Draw("SAME"	);
   edgeRi->SetLineColor(kViolet);
   edgeRi->Draw("SAME"	);
   edgeLo->SetLineColor(kViolet);
   edgeLo->Draw("SAME"	);
   edgeUp->SetLineColor(kViolet);
   edgeUp->Draw("SAME"	); 
   
   TF1 *line = new TF1("line","[0]+([1]*x)",-100,200);
   line->SetLineColor(kRed);
   TF1 *line2 = new TF1("line2","[0]+([1]*x)",-100,200);
   line->SetLineColor(kGreen);
   
   TMarker *X1=new TMarker();
   X1->SetMarkerStyle(20);
   X1->SetMarkerSize(1);
   X1->SetMarkerColor(kRed);
   TMarker *X2=new TMarker();
   X2->SetMarkerStyle(20);
   X2->SetMarkerSize(1);
   X2->SetMarkerColor(kGreen);

   
   for(int i=0; i<100;i++){
      tree->GetEntry(i);
      for(int j=0; j<5; j++){cout<<cl_x[j]<<"  "; }
      cout<<"|"<<theta<<endl;
      cout<<endl;
      line->SetParameter(0,intercept);
      line->SetParameter(1,slope);
      line->Draw("SAME");
      line2->SetParameter(0,intercept);
      line2->SetParameter(1,atan((-0.5*3.1415)-theta));
      line2->Draw("SAME");
      
      X1->SetX(0);
      X1->SetY(intercept);
      X1->Draw("SAME");      
      X2->SetX(intercept);
      X2->SetY(0);
      X2->Draw("SAME");      
      
            
      C1->Update();
      cout<<"press anykey to continue"<<endl;
      cin>>pippo;
      
      
   }


}


