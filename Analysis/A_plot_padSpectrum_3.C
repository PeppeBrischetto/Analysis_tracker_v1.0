//###################################################################################################
//#   macro that take as input a merged file, plot the charge spectrum of multiple pads
//#   in the same plot
//#   
//#   required as argument the run number
//###################################################################################################
//#   created May 2024   D. Torresi
//#######################################
//


void A_plot_padSpectrum_3(int run)
{

   // Dichiarazione variabili	

   UShort_t Channel; 
   UShort_t pad; 
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Charge;
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Row;

   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);

   TFile *fin = new TFile(fileIn);
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Row);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   char histoName[10];

   // Dichiarazione Histo 
   TH1F *chargepad[320];
   for(int i=0; i<320; i++){
      sprintf(histoName,"pad%i",i);
      chargepad[i]=new TH1F(histoName,histoName,4000,-0.5,64099.5);
      chargepad[i]->GetXaxis()->SetTitle("charge");
      chargepad[i]->GetYaxis()->SetTitle("counts");
   }

   TH1F *chargepadZ[320];
   for(int i=0; i<320; i++){
      sprintf(histoName,"padZ%i",i);
      chargepadZ[i]=new TH1F(histoName,histoName,200,30000,50000);
      chargepadZ[i]->GetXaxis()->SetTitle("charge");
      chargepadZ[i]->GetYaxis()->SetTitle("counts");
   }
 
   int k=0;
   double intpart, fracpart;
   int massimo[320];
   
   //cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries/10; i++){
      tree->GetEntry(i);
      //modf((((float)i/entries)*10),&intpart);
      
      for(int j=0; j<64; j++){
        if(pad==j && Row==0){
           chargepad[j]->Fill(Charge);
           chargepadZ[j]->Fill(Charge);
        }
        if(pad==j && Row==1){
           chargepad[j+64]->Fill(Charge);
           chargepadZ[j+64]->Fill(Charge);
        }
        if(pad==j && Row==2){
           chargepad[j+128]->Fill(Charge);
           chargepadZ[j+128]->Fill(Charge);
        }
        if(pad==j && Row==3){
           chargepad[j+192]->Fill(Charge);
           chargepadZ[j+192]->Fill(Charge);
        }
        if(pad==j && Row==4){
           chargepad[j+256]->Fill(Charge);
           chargepadZ[j+256]->Fill(Charge);
        }
        
                         
      }
      
   }
   
   TGraph *gr0= new TGraph(); 
   TGraph *gr1= new TGraph();
   TGraph *gr2= new TGraph(); 
   TGraph *gr3= new TGraph();
   TGraph *gr4= new TGraph(); 
   for(int j=0; j<64; j++){
       massimo[j]=chargepadZ[j]->GetMaximumBin();
       gr0->SetPoint(j,j,massimo[j]*(10000./200)+30000);
       cout<<j<< "  "<<massimo[j]*(10000./200)+30000<<endl;      
       massimo[j]=chargepadZ[j+64]->GetMaximumBin();
       gr1->SetPoint(j,j,massimo[j+64]*(10000./200)+30000);
       massimo[j]=chargepadZ[j+128]->GetMaximumBin();
       gr2->SetPoint(j,j,massimo[j+128]*(10000./200)+30000); 
       massimo[j]=chargepadZ[j+192]->GetMaximumBin();
       gr3->SetPoint(j,j,massimo[j+192]*(10000./200)+30000);
       massimo[j]=chargepadZ[j+256]->GetMaximumBin();
       gr4->SetPoint(j,j,massimo[j+256]*(10000./200)+30000);                    
       
   }
   
   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   chargepadZ[10]->Draw();   
  
   for(int i=20; i<27; i++){
      chargepad[i+128]->SetLineColor(i-20);
      chargepad[i+128]->Draw("same");   
   }
  
   char naam[100];
   TLegend *leg= new TLegend(0.7,0.65,0.9,0.9);
   for(int i=20; i<27; i++){
      sprintf(naam,"Pad %d",i);
      leg->AddEntry(chargepadZ[i], naam, "l");
   }
   leg->Draw();
 
   TCanvas *C2=new TCanvas("c2","c2",800,100,900,400);
   C2->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
 
   for(int i=27; i<32; i++){
      chargepad[i+128]->SetLineColor(i-27);
      chargepad[i+128]->Draw("same");   
   }
  
   TLegend *leg1= new TLegend(0.7,0.65,0.9,0.9);
   for(int i=27; i<32; i++){
      sprintf(naam,"Pad %d",i);
      leg1->AddEntry(chargepadZ[i], naam, "l");
   }
   leg1->Draw();
 
   TCanvas *C3=new TCanvas("c3","c3",800,100,900,400);
   C3->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   gr0->SetMarkerStyle(20);
   gr0->SetMarkerSize(1);
   gr0->SetMarkerColor(kBlack);
   gr0->Draw("AP");
   gr1->SetMarkerStyle(20);
   gr1->SetMarkerSize(1);
   gr1->SetMarkerColor(kRed);
   gr1->Draw("P");
   gr2->SetMarkerStyle(20);
   gr2->SetMarkerSize(1);
   gr2->SetMarkerColor(kGreen);
   gr2->Draw("P");
   gr3->SetMarkerStyle(20);
   gr3->SetMarkerSize(1);
   gr3->SetMarkerColor(kBlue);
   gr3->Draw("P");
   gr4->SetMarkerStyle(20);
   gr4->SetMarkerSize(1);
   gr4->SetMarkerColor(kViolet);
   gr4->Draw("P");

 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
}
