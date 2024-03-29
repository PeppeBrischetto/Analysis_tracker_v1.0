void B_plot_ClusterCharge(int run)
{


   ULong64_t Timestamp;	// timestamp of the pad with higher charge
   ULong64_t Timestamp_a;	// weighted average timestamp approximated to next integer
   UShort_t Board;		// Board  
   UShort_t Charge;		// total charge
   Double_t Charge_cal;	// total charge calibrated
   UShort_t Row;		// row
   double Centroid;

   UShort_t Section;
   UShort_t multiplicity;   

   char fileIn[50];
   sprintf(fileIn, "cluster_Row%i.root", run);
   cout<<fileIn<<endl;
   TFile *fin = new TFile(fileIn);
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   //tree->SetBranchAddress("Timestamp_a",&Timestamp_a);   
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);   
   //tree->SetBranchAddress("Centroid",&Centroid);
   tree->SetBranchAddress("Row",&Row);
   tree->SetBranchAddress("Section",&Section);
   tree->SetBranchAddress("Multiplicity",&multiplicity);         
   
 
   TH1F *hrow[5];
   hrow[0]=new TH1F("r0","r0",62,-0.5,61.5);
   hrow[1]=new TH1F("r1","r1",62,-0.5,61.5);
   hrow[2]=new TH1F("r2","r2",62,-0.5,61.5);
   hrow[3]=new TH1F("r3","r3",62,-0.5,61.5);
   hrow[4]=new TH1F("r4","r4",62,-0.5,61.5);
  
   TH1F *Crow[5];
   Crow[0]=new TH1F("r0","r0",500,-0.5,63999.5);
   Crow[1]=new TH1F("r1","r1",500,-0.5,63999.5);
   Crow[2]=new TH1F("r2","r2",500,-0.5,63999.5);
   Crow[3]=new TH1F("r3","r3",500,-0.5,63999.5);
   Crow[4]=new TH1F("r4","r4",500,-0.5,63999.5);
   
   TH1F *Ccrow[5];
   Ccrow[0]=new TH1F("rc0","rc0",500,-0.5,63999.5);
   Ccrow[1]=new TH1F("rc1","rc1",500,-0.5,63999.5);
   Ccrow[2]=new TH1F("rc2","rc2",500,-0.5,63999.5);
   Ccrow[3]=new TH1F("rc3","rc3",500,-0.5,63999.5);
   Ccrow[4]=new TH1F("rc4","rc4",500,-0.5,63999.5);
   
 
   int entries=tree->GetEntries();
   
   for(int i=0; i<entries; i++){
      tree->GetEntry(i);
      if(multiplicity>2 ){
         Crow[0]->Fill(Charge);
         Ccrow[0]->Fill(Charge_cal);
         hrow[0]->Fill(Centroid);
      }
   }
 
   TCanvas *c1=new TCanvas("c1");
   Crow[0]->Draw();  
   Ccrow[0]->SetLineColor(kRed);
   Ccrow[0]->Draw("SAME");
   
   TCanvas *c2=new TCanvas("c2");
   hrow[0]->Draw();  
}
