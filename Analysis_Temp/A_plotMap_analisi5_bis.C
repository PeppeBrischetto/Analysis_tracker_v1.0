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

#define rad2deg=3.14159/180

/*double *zrow = new double[5];  // zcoordinate of the row
zrow[0]=10.8;
zrow[1]=21.6+10.80;
zrow[2]=2*21.6+10.80;
zrow[3]=3*21.6+10.80;
zrow[4]=4*21.6+10.80;*/

double calibPad(int pad){
   double xpad=pad*5+2.5;
   return xpad;
};

double rotation(double intercept, double slope, double zcol, double theta){
   double xcol = (zcol - intercept)/slope;
   double xcolprime = (xcol*cos(theta))+(zcol*sin(theta));
   return xcolprime;
};	  



void A_plotMap_analisi5_bis(int run)
{

   // Dichiarazione variabili
   UShort_t Channel; 
   UShort_t Pad; 
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
   float DeltaT=1000000;  	// characteristic time of an event in ps  
   int thresh=0;		// charge threshold
   int evMult=2;		// row multiplicity event (select just event with row multiplicity larger than evMult
   int evNumber=0;		// counter for event
   int evLimit=2000;		// plot on screen tracks for the first evLimit events

   double total_charge = 0;	// total charge of a cluster
   double centroid[5];		// array that contain the centroid of each event
  
   // parameter of each track
   double slope,intercept;
   double slope3,intercept3;
   
   double xcol = 0;
   double zcol = -55;
   double theta = 20*3.14159/180; 
   double xcolprime = 0;  
   
   // File opening
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
   tree->SetBranchAddress("Pads",&Pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Single_Row);
   tree->SetBranchAddress("Section",&Single_Section);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Declaring Histos 
   TH1F *row[5];		//histo with charge per pad for each row
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
   
   TH2D *clusters=new TH2D("clusters","clusters",630,0.0,315.0,120,-5.40,124.2);
   clusters->SetStats(kFALSE);
   clusters->GetXaxis()->SetTitle("Xpad (mm)");
   clusters->GetYaxis()->SetTitle("Zrow (mm)");   
   clusters->SetMarkerStyle(20);
   clusters->SetMarkerSize(1);
   
   TGraph *TGclusters=new TGraph(0);
   TGclusters->SetMarkerStyle(21);
   TGclusters->SetMarkerSize(0.8);
   TGclusters->SetMarkerColor(kRed);
   
   // map of the anode, on top of it the tracks are plot
   TH2D *anode=new TH2D("anode","anode",100,-20.0,320.0,100,-135.0,128);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("Xpad (mm)");
   anode->GetYaxis()->SetTitle("Zrow (mm)");
   TLine *lowedge = new TLine(0,0,300,0);
   TLine *upedge = new TLine(0,108,300,108);
   TLine *leftedge = new TLine(0,0,0,108);
   TLine *rigthedge = new TLine(300,0,300,108);
   upedge->SetLineColor(kViolet);
   lowedge->SetLineColor(kViolet);
   leftedge->SetLineColor(kViolet);
   rigthedge->SetLineColor(kViolet);
   
   TH1D *projection=new TH1D("projection","projection",3150,0.0,315.0);
   TH1D *projection3=new TH1D("projection3","projection3",3150,0.0,315.0);
   projection->SetStats(1);
   projection->GetXaxis()->SetTitle("Xcolprime (mm)");
   projection3->SetLineColor(kGreen);



   // Fitting function
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,160);
   lin1->SetLineColor(4);
   lin1->SetLineWidth(2);
   TF1 *lin3 = new TF1("lin3","[0]+([1]*x)",0,160);
   lin3->SetLineColor(4);
   lin3->SetLineWidth(2);
   TF1 *lin2 = (TF1*) lin1->Clone();     ;

   TCanvas *C31=new TCanvas("c31","proj",800,500,900,400);
   C31->SetFillColor(kWhite);
   
   TCanvas *C4=new TCanvas("c4","c4",800,500,900,400);
   C4->SetFillColor(kWhite);

   TCanvas *C41=new TCanvas("c41","c41",800,500,900,400);
   C41->SetFillColor(kWhite);

   
   TCanvas *CAnode=new TCanvas("CAnode","CAnode",800,500,900,400);
   CAnode->SetFillColor(kWhite);
   CAnode->cd(0);
   anode->Draw();
   upedge->Draw("SAME");
   lowedge->Draw("SAME");
   leftedge->Draw("SAME");
   rigthedge->Draw("SAME");
   
   // take the reference time
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   
   
   //cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      double xpad= (Pad*5.0)+2.50;     

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
	 if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>evMult){
	    
	    cout<<"Event number "<<evNumber++<<endl;
	    for(int j=0; j<5; j++){
	       total_charge = row[j]->Integral(1,63);          
	       centroid[j]=row[j]->GetMean();
	       clusters->Fill(centroid[j],(j*21.60)+10.80); 	   	       
	       TGclusters->SetPoint(j,centroid[j],(j*21.60)+10.80); 	   	       
            }
            
            if(evNumber<evLimit){
               C4->cd(0);
               clusters->Draw();
               clusters->Fit("lin1","r");
               cout<<"-##--------------"<<endl;
               C41->cd(0);
               TGclusters->Draw("AP");
               TGclusters->Fit("lin3","r");
               cout<<"-####--------------"<<endl;
            }else{
               clusters->Fit("lin1","r0");
               TGclusters->Fit("lin3","r");
            }
            intercept=lin1->GetParameter(0);
            slope=lin1->GetParameter(1);            
            intercept3=lin3->GetParameter(0);
            slope3=lin3->GetParameter(1);    

	    if(evNumber<evLimit){
               lin2 = (TF1*) lin1->Clone();     
	    }  
            //  rotation of the reference frame by an angle theta
	    if(intercept>-325 && intercept<-150){	//changed with respect the low intensity a-source	             
 	   
	       double xcolprime=rotation(intercept, slope, zcol, theta);
               double xcolprime3=rotation(intercept3, slope3, zcol, theta); 
               
               projection->Fill(xcolprime,1.0);
               projection3->Fill(xcolprime3,1.0);
               if(evNumber<evLimit){
      	          CAnode->cd(0);
	          lin2->SetLineColor(4);
	          lin2->Draw("same");
	       }
	    }
	       
	    if(evNumber<evLimit){   
   	       C4->Update();
   	       C41->Update();
	       CAnode->Update();
	    }
	    
 	 clusters->Reset("ICES");

         }
         cout<<"---------------"<<endl;
         timeinit=Timestamp;

         for(int j=0;j<5; j++){
            row[j]->Reset("ICES");
            flag[j]=0;
         }
        
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
 
   C31->cd();
   projection->Draw();
   projection3->Draw("same");
   
  
   cout<<projection->GetMean()<<"\t"<<projection->GetStdDev()<<"\t"<<projection->GetStdDev()*2.35<<endl;
   cout<<projection3->GetMean()<<"\t"<<projection3->GetStdDev()<<"\t"<<projection->GetStdDev()*2.35<<endl;
}
