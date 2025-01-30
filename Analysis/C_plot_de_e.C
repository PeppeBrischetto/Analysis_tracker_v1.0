//################################################################################################################################################

//Macro that reads the information from a third level root file and plots a dE-Er plot using as dE the charge measured by the 5 rows and as Er the energy measured by the SiC detector//

//To run simply open a root session and type: .x C_plot_de_e.C(run number)//

//Created in December 2024 by O. Sgouros//

//################################################################################################################################################



void C_plot_de_e(int run, int run2)
{

cout<<""<<endl;
cout<<""<<endl;


// Input file variables

   double cl_charge[11];	 // Charge sum of the pads belonging to a cluster
   double cl_x_rms[5];  	// RMS of the charge distribution of a cluster in pad units
   double cl_y_mm[5] = {0};  

   int sic_fired;
   double energySic;



//#################################################################################################
//Open two files

   char fileIn[50];
   char fileIn2[50];

   if(run<10){
      sprintf(fileIn, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileIn, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileIn, "../Tracks/tracks_run%i.root", run);
   } 

   if(run2<10){
      sprintf(fileIn2, "../Tracks/tracks_run00%i.root", run2);
   }else if(run2 <100){   
      sprintf(fileIn2, "../Tracks/tracks_run0%i.root", run2);
   }else{      
      sprintf(fileIn2, "../Tracks/tracks_run%i.root", run2);
   } 

   cout<<fileIn<<endl;
   cout<<""<<endl;
   cout<<""<<endl;
   
   cout<<"£"<<fileIn2<<endl;
   cout<<""<<endl;
   cout<<""<<endl;


   TFile *f1 = new TFile(fileIn);
   TFile *f2 = new TFile(fileIn2);
   
   TTree *tree = (TTree*)f1->Get("Data_R");
   TTree *tree2 = (TTree*)f2->Get("Data_R");
   
   tree->SetBranchAddress("cl_charge",&cl_charge);
   tree->SetBranchAddress("cl_x_rms",&cl_x_rms);
   tree->SetBranchAddress("cl_y_mm",&cl_y_mm);

   tree->SetBranchAddress("sic_fired",&sic_fired);
   tree->SetBranchAddress("energySic",&energySic);
   
   
   tree2->SetBranchAddress("cl_charge",&cl_charge);
   tree2->SetBranchAddress("cl_x_rms",&cl_x_rms);
   tree2->SetBranchAddress("cl_y_mm",&cl_y_mm); 

   tree2->SetBranchAddress("sic_fired",&sic_fired);
   tree2->SetBranchAddress("energySic",&energySic);


   int entries=tree->GetEntries();
   int entries2=tree2->GetEntries();
 
// END: Open file 

//#################################################################################################



// Declaring Histos 

   TH2F *de_e1 = new TH2F("de_e1","",500,0.0,10000.0,500,0.0,1000000.0);  //dE-E histogram with the data from the first file//
   TH2F *de_e2 = new TH2F("de_e2","",500,0.0,10000.0,500,0.0,1000000.0);  //dE-E histogram with the data from the second file//

   
  
   de_e1->GetXaxis()->SetTitle("E_{SiC} (Channels)");
   de_e1->GetXaxis()->SetTitleOffset(1.05);
   de_e1->GetXaxis()->SetTitleSize(0.040);
   de_e1->GetXaxis()->SetTitleFont(62);
   de_e1->GetXaxis()->SetLabelFont(62);
   de_e1->GetXaxis()->SetLabelOffset(0.008);
   de_e1->GetXaxis()->SetLabelSize(0.030);
   de_e1->GetYaxis()->SetNdivisions(505);
   de_e1->GetXaxis()->CenterTitle();

   de_e1->GetYaxis()->SetTitle("#DeltaE (Channels)");
   de_e1->GetYaxis()->SetTitleOffset(1.10);
   de_e1->GetYaxis()->SetTitleSize(0.040);
   de_e1->GetYaxis()->SetTitleFont(62);
   de_e1->GetYaxis()->SetLabelFont(62);
   de_e1->GetYaxis()->SetLabelOffset(0.008);
   de_e1->GetYaxis()->SetLabelSize(0.030);
   de_e1->GetYaxis()->SetNdivisions(505);
   de_e1->GetYaxis()->CenterTitle();
   
   de_e2->GetXaxis()->SetTitle("E_{SiC} (Channels)");
   de_e2->GetXaxis()->SetTitleOffset(1.05);
   de_e2->GetXaxis()->SetTitleSize(0.040);
   de_e2->GetXaxis()->SetTitleFont(62);
   de_e2->GetXaxis()->SetLabelFont(62);
   de_e2->GetXaxis()->SetLabelOffset(0.008);
   de_e2->GetXaxis()->SetLabelSize(0.030);
   de_e2->GetYaxis()->SetNdivisions(505);
   de_e2->GetXaxis()->CenterTitle();

   de_e2->GetYaxis()->SetTitle("#DeltaE (Channels)");
   de_e2->GetYaxis()->SetTitleOffset(1.10);
   de_e2->GetYaxis()->SetTitleSize(0.040);
   de_e2->GetYaxis()->SetTitleFont(62);
   de_e2->GetYaxis()->SetLabelFont(62);
   de_e2->GetYaxis()->SetLabelOffset(0.008);
   de_e2->GetYaxis()->SetLabelSize(0.030);
   de_e2->GetYaxis()->SetNdivisions(505);
   de_e2->GetYaxis()->CenterTitle();
   



   TCanvas *c1=new TCanvas("c1","",800,500,1000,800);
   
   
  
     
   for(int i=0; i<entries; i++){ //Loop on the events of the first file to fill the dE-E plot
   tree->GetEntry(i);
   
   if(sic_fired) {
   de_e1->Fill(energySic,cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]);
   
   //de_e1->Fill(energySic,cl_charge[3]);
   }
   
 }
 
 
   for(int j=0; j<entries2; j++){ //Loop on the events of the second file to fill the dE-E plot
      tree2->GetEntry(j);
   
   if(sic_fired&&cl_y_mm[0]>100) {
       de_e2->Fill(energySic,cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]);
   
   }
   
 }
    
     

   c1->cd();
   de_e1->Draw("colz");
   de_e2->SetMarkerColor(kRed);
   de_e2->Draw("SAME");



   
}

