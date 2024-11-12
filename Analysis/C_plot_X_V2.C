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

void C_plot_X_V2(int run)
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
   int flagA=0;
   char histoname[100];
   
   int flag=0;

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

   TCanvas *C1=new TCanvas("c1","c1",250,160,1600,600);   
   
   // all tracks
   TH1D *XposH[5];
   TH1D *XposL[5];
   for(int h=0; h<5; h++){
      sprintf(histoname,"xpos%i",h); 
      //Xpos[h]=new TH1D(histoname,histoname,1200,-50.5,349.5);
      XposH[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      XposH[h]->GetXaxis()->SetTitle("x (pad)");
      XposH[h]->GetYaxis()->SetTitle("counts");
      
      XposL[h]=new TH1D(histoname,histoname,2500,-10.5,69.5);
      XposL[h]->GetXaxis()->SetTitle("x (pad)");
      XposL[h]->GetYaxis()->SetTitle("counts");
   }


   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      flag=0;
      
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      
      if(cl_padMult[0]>3 && cl_padMult[1]>3 &&cl_padMult[2]>3 &&cl_padMult[3]>3 &&cl_padMult[4]>3) flag=1;
      if(cl_padMult[0]<3 && cl_padMult[1]<3 &&cl_padMult[2]<3 &&cl_padMult[3]<3 &&cl_padMult[4]<3) flag=2;
      
      
      for(int h=0; h<5; h++){
         
         if(flag==1) XposH[h]->Fill(cl_x[h]);
         if(flag==2) XposL[h]->Fill(cl_x[h]);
      }
     
     
   }
   cout<<endl;
   
   TCanvas *c1=new TCanvas("c1","High mult");
   XposH[0]->SetLineColor(kBlack);
   XposH[0]->Draw();
   XposH[1]->SetLineColor(kRed);
   XposH[1]->Draw("same");
   XposH[2]->SetLineColor(kGreen);
   XposH[2]->Draw("same");
   XposH[3]->SetLineColor(kBlue);
   XposH[3]->Draw("same");
   XposH[4]->SetLineColor(kViolet);
   XposH[4]->Draw("same");
   

   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   leg->AddEntry(XposH[0], " row 0", "l"); 
   leg->AddEntry(XposH[1], " row 1", "l");
   leg->AddEntry(XposH[2], " row 2", "l");
   leg->AddEntry(XposH[3], " row 3", "l");
   leg->AddEntry(XposH[4], " row 4", "l");
   leg->Draw();

   TCanvas *c2=new TCanvas("c2","Low mult");
   XposL[0]->SetLineColor(kBlack);
   XposL[0]->Draw();
   XposL[1]->SetLineColor(kRed);
   XposL[1]->Draw("same");
   XposL[2]->SetLineColor(kGreen);
   XposL[2]->Draw("same");
   XposL[3]->SetLineColor(kBlue);
   XposL[3]->Draw("same");
   XposL[4]->SetLineColor(kViolet);
   XposL[4]->Draw("same");
   leg->Draw();
 }  
   





