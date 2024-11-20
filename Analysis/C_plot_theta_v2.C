//###################################################################################################
//#   plot theta spectra with different condition
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created november 2024 by D. Torresi
//#######################################
//#   updated:
//# 
//###################################################################################################

void C_plot_theta_v2(int run)
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
   int qualityFlag2=0;
   int qualityFlag3=0;
   int flagA=0;
   double totalCharge=0;


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
   
   // all tracksq
   TH1F *histoTheta=new TH1F("","",400,-30,90);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("charge");
   histoTheta->GetYaxis()->SetTitle("counts");

   // histo with selected tracks 
   TH1F *histoThetaS=new TH1F("","",1000,-30,90);
   histoThetaS->SetStats(0);
   histoThetaS->GetXaxis()->SetTitle("charge");
   histoThetaS->GetYaxis()->SetTitle("counts");
   
   // histo with rejected tracks 
   TH1F *histoThetaD1=new TH1F("","",1000,-30,90);
   histoThetaD1->SetStats(0);
   histoThetaD1->GetXaxis()->SetTitle("charge");
   histoThetaD1->GetYaxis()->SetTitle("counts");
   TH1F *histoThetaD2=new TH1F("","",1000,-30,90);
   histoThetaD2->SetStats(0);
   histoThetaD2->GetXaxis()->SetTitle("charge");
   histoThetaD2->GetYaxis()->SetTitle("counts");
   
//#################################################################################################
// Data LOOP
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      qualityFlag=0;
      qualityFlag2=0;
      qualityFlag3=0;
      totalCharge=0;
      //###########################################################################################
      // condition      
      // reject tracks with first or last pad of the last row hit
      for (int j=0; j<cl_padMult[4]; j++) {
                   
         //cout<<a_pads_fired[4][j]<<"\t";
         if ((a_pads_fired[4][j] == 1) || (a_pads_fired[4][j] == 2) || (a_pads_fired[4][j] == 58) || (a_pads_fired[4][j] == 57)){
            qualityFlag=1;
         }
      }   
      //cout<<endl;
     
      // selection on PadMultiplicity
      //for(int k=0; k<5; k++){
      //   cout<<cl_padMult[k]<<"\t";
      //   if(cl_padMult[k]<4)qualityFlag=1;         
      //}
     
      // selection on total charge on the last row, make the average of the first 4 row ad if the charge of the last 
      // row is smaller or larger of 50% of the average of the first row the track is removed
      //cout<<"Charge: ";
      for(int k=0; k<4; k++){
         cout<<cl_charge[k]<<"\t";
         totalCharge=totalCharge+cl_charge[k];
      }
      cout<<endl;
      if(abs(totalCharge/4-cl_charge[4])>(totalCharge/8)) qualityFlag2=1;
      cout<<"\n"<<"# "<<totalCharge/4<<"\t"<<cl_charge[4]<<"\t"<<abs(totalCharge/4-cl_charge[4])<<"\t"<<totalCharge/40<<endl;
      //if(cl_padMult[k]<4)qualityFlag=2;         
    
      if(totalCharge>200000){qualityFlag3=1;}
    
    
    
      // Fill the histo
      histoTheta->Fill(theta_deg);
      if(qualityFlag==0 && qualityFlag2==0){   
         histoThetaS->Fill(theta_deg);        
      }
      if(qualityFlag==1){
         histoThetaD1->Fill(theta_deg);        
      }
      if(qualityFlag2==1){
         histoThetaD2->Fill(theta_deg);        
      }
      
      
   }
   
   histoTheta->SetLineColor(kBlack);
   histoTheta->Draw();
   histoThetaS->SetLineColor(kGreen);
   histoThetaS->Draw("same");   
   histoThetaD1->SetLineColor(kViolet);
   histoThetaD1->Draw("same");
   histoThetaD2->SetLineColor(kRed);
   histoThetaD2->Draw("same");
   
   
   
   TLegend* l = new TLegend();
   l->SetTextSize(0.035);
   l->AddEntry(histoTheta, "All tracks", "l");
   l->AddEntry(histoThetaS, "selected tracks", "l");   
   l->AddEntry(histoThetaD1, "no edge pads on row4","l");
   l->AddEntry(histoThetaD2, "Charge on row4","l");
   //l->AddEntry(theta_min, "#theta_{calc} [-38.8;-41.2] (deg)","lep");
   l->Draw("same");}




