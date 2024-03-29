// Macro to find the source position.
// Three figures are plotted: 
// 1) the fit to obtain the particle trajectory
// 2) all the "good/reasonable" trajectories
// 3) the obtained position for a given zcol and theta
// It is suggested to perform a fit on the plot 3) to obtain the sigma and FWHM
// Two different methods may be applied (plese "comment" the apropriate command lines below) 
//
// Suggested ROOT version: 6.26 
// Compatibility issues in v. 5.34.
//
// Last update 22-02-2023  V. Soukeras
// based on A_plotMap_analysis4.C developed for low intensity a-source data
//


void A_plotMap_analisi5(int run)
{

   // Dichiarazione variabili
   UShort_t Channel; 
   UShort_t Single_Pads; 
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Charge;
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Single_Row;
   UShort_t Single_Section;
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;
   float DeltaT=1000000;  //in ps
   
   int binmax, max;
   int thresh=0;
   
   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_264/merg_%d.root", run);
   TFile *fin = new TFile(fileIn);
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&Single_Pads);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Single_Row);
   tree->SetBranchAddress("Section",&Single_Section);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *row[5];
   row[0]=new TH1F("r1","r1",63,0.0,315.0);
   row[1]=new TH1F("r2","r2",63,0.0,315.0);
   row[2]=new TH1F("r3","r3",63,0.0,315.0);
   row[3]=new TH1F("r4","r4",63,0.0,315.0);
   row[4]=new TH1F("r5","r5",63,0.0,315.0);

   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("Xpad (mm)");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   
   
   TH2D *anode=new TH2D("anode","anode",63,0.0,315.0,12,-5.40,124.2);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("Xpad (mm)");
   anode->GetYaxis()->SetTitle("Zrow (mm)");
   anode->GetYaxis()->SetNdivisions(505); 		// setta il numero di divisioni per la grid
   
   TH2D *tracks=new TH2D("tracks","tracks",63,0.0,315.0,24,-135.0,124.2);
   tracks->SetStats(kFALSE);
   tracks->GetXaxis()->SetTitle("Xpad (mm)");
   tracks->GetYaxis()->SetTitle("Zrow (mm)");

   TH2D *anode_fit=new TH2D("anode_fit","anode_fit",630,0.0,315.0,120,-5.40,124.2);
   anode_fit->SetStats(kFALSE);
   anode_fit->GetXaxis()->SetTitle("Xpad (mm)");
   anode_fit->GetYaxis()->SetTitle("Zrow (mm)");
 
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",63,0.0,315.0,12,-5.40,124.2);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("Xpad (mm)");
   anodeTime->GetYaxis()->SetTitle("Zrow (mm)");
   anodeTime->GetYaxis()->SetNdivisions(505); 		// setta il numero di divisioni per la grid
   

   TH1D *projection=new TH1D("projection","projection",630,0.0,315.0);
   //projection->SetStats(kFALSE);
   projection->GetXaxis()->SetTitle("Xcolprime (mm)");


   TCanvas *C31=new TCanvas("c31","proj",800,500,900,400);
   C31->SetFillColor(kWhite);
   
   
   //TCanvas *C4=new TCanvas("c4","c4",800,500,900,400);
   //C4->SetFillColor(kWhite);
   
   //TCanvas *C5=new TCanvas("c5","c5",800,500,900,400);
   //C5->SetFillColor(kWhite);
   //C5->cd(0);
   //tracks->Draw();
  
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   
   double charge = 0.0;
   double total_charge = 0.0;
   double weigthed_pos[63];
   double centroid[5];
  
   double xcol = 0.0;
   double zcol = -55.0;
   double theta = 20.0*3.14159/180.0; 
   double xcolprime = 0.0;
   //double xcol0 = 48.60113; //useful only at the second method introduced @ 15 feb 2023 
 
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,160);
   double slope,intercept;

   
   cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
   
double xpad= (Single_Pads*5.0)+2.50;     

      if((Timestamp-timeinit)<DeltaT){
        if(Board==21247 && Charge>thresh){
	flag[0]=1;
           row[0]->Fill(xpad,Charge);
        }
        if(Board==22645 && Charge>thresh){
	flag[1]=1;
           row[1]->Fill(xpad,Charge);
        }
        if(Board==22644 && Charge>thresh){
	flag[2]=1;
           row[2]->Fill(xpad,Charge);
        }
        if(Board==22643 && Charge>thresh){
	flag[3]=1;
           row[3]->Fill(xpad,Charge);
        }
        if(Board==22642 && Charge>thresh){
	flag[4]=1;
           row[4]->Fill(xpad,Charge);
        }

      }else{
        if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>2){

           for(int j=0; j<5; j++){

           centroid[j]=0.0;
           
           binmax = row[j]->GetMaximumBin(); 
           total_charge = row[j]->Integral(1,63);          

           	for(int k=1; k<=63; k++){
           	charge = row[j]->GetBinContent(k);
                weigthed_pos[k] = (((k-1)*5)+2.5)*charge/total_charge;
                centroid[j] = centroid[j] + weigthed_pos[k];
           	}

        	
           max = row[j]->GetBinContent(binmax);
           //cout<<binmax<<"  "<<max<<endl;

           row[j]->GetYaxis()->SetRangeUser(0,max*2);
          // cin>>anykey;
           anode_fit->Fill(centroid[j],(j*21.60)+10.80); 
           //C4->cd(0);
           anode_fit->SetMarkerStyle(20);
           anode_fit->SetMarkerSize(1);
           if(j==4){
           //anode_fit->Draw();
           anode_fit->Fit("lin1","r0");
           printf("\n");
           printf("\n");
           intercept=lin1->GetParameter(0);
           slope=lin1->GetParameter(1);            

           //TF1 *fit_result = new TF1("fit_result",Form("%1.8f+(%1.8f*x)",intercept,slope),0.00,160.00);
	   

	   if(intercept>-325.0 && intercept<-150.0){	//changed with respect the low intensity a-source
           //fit_result->SetLineColor(4);

           //C31->cd(0);
	   xcol = (zcol - intercept)/slope;
	   xcolprime = (xcol*cos(theta))+(zcol*sin(theta)); 			//basic -> keep open for method 1
	   // xcolprime = (zcol-(xcol0*tan(-theta))-intercept)/(slope-tan(-theta)); 	//test -> keep open for method 2 (15feb 2023)
	   
           projection->Fill(xcolprime,1.0);
           //projection->Draw("same");
           
           //C5->cd(0);
           //fit_result->SetLineWidth(2);
           //fit_result->Draw("same");
           	   }
	   
	   else{
	        //fit_result->SetLineColor(2);
		}
           
           }

           }
           //C4->Update();
	   //C5->Update();
           C31->Update();
          
        }
        cout<<"---------------"<<endl;
        timeinit=Timestamp;

        for(int j=0;j<5; j++){
           row[j]->Reset("ICES");
           flag[j]=0;
        }
        anode_fit->Reset("ICES");
        
        if(Board==21247 && Charge>thresh){
	flag[0]=1;
           row[0]->Fill(xpad,Charge);
        }
        if(Board==22645 && Charge>thresh){
	flag[1]=1;
           row[1]->Fill(xpad,Charge);
        }
        if(Board==22644 && Charge>thresh){
	flag[2]=1;
           row[2]->Fill(xpad,Charge);
        }
        if(Board==22643 && Charge>thresh){
	flag[3]=1;
           row[3]->Fill(xpad,Charge);
        }
        if(Board==22642 && Charge>thresh){
	flag[4]=1;
           row[4]->Fill(xpad,Charge);
        }
          
      } 
        
   }
      
      C31->cd(0);
      projection->Draw("same");
}
