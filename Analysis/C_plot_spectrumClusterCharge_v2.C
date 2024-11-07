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

void C_plot_spectrumClusterCharge_v2(int run)
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

   TFile *file = new TFile(fileIn);
   TTree *tree = (TTree*)file->Get("Data_R");
   
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
   
   TH1D *chargeCluster[50];
   for(int h=0; h<50; h++){
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
         if(a_pads_fired[j][0]>15 && a_pads_fired[j][0]<18){
            chargeCluster[j]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<20){
            chargeCluster[j+5]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<22){
            chargeCluster[j+10]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<24){
            chargeCluster[j+15]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<26){
            chargeCluster[j+20]->Fill(cl_charge[j]);  
         }else if(a_pads_fired[j][0]<28){
            chargeCluster[j+25]->Fill(cl_charge[j]);  
         }

         
         totalCharge=totalCharge+cl_charge[j];
         //cout<<cl_charge[j]<<"  ";
      }
      cout<<endl;
      HTotalCharge->Fill(totalCharge);
          
   }
   
   
   //HTotalCharge->Draw();
   HTotalCharge->SetLineColor(kBlack);
    
   int z=4;   // row number
   int a=0+z;
   int b=5+z;
   int c=10+z;
   int d=15+z;
   int e=20+z;
   int f=25+z;
   int g=30+z;

   chargeCluster[a]->Draw();
   chargeCluster[a]->SetLineColor(kRed);   
   chargeCluster[b]->Draw("same");
   chargeCluster[b]->SetLineColor(kGreen);
   chargeCluster[c]->Draw("same");
   chargeCluster[c]->SetLineColor(kBlue);
   chargeCluster[d]->Draw("same");
   chargeCluster[d]->SetLineColor(kViolet);
   chargeCluster[e]->Draw("same");
   chargeCluster[e]->SetLineColor(kOrange);
   chargeCluster[f]->Draw("same");
   chargeCluster[f]->SetLineColor(kRed +2);

   
   
   TLegend* l = new TLegend(0.6,0.8,0.9,0.9);
   l->SetTextSize(0.035);
   l->AddEntry(chargeCluster[a], "16-17", "l");
   l->AddEntry(chargeCluster[b], "18-20", "l");   
   l->AddEntry(chargeCluster[c], "20-22","l");
   l->AddEntry(chargeCluster[d], "22-24","l");
   l->AddEntry(chargeCluster[e], "24-26","l");
   l->AddEntry(chargeCluster[f], "26-28","l");
   l->Draw("same"); 
   
   
}   
   




