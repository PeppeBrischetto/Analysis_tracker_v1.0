

//######## OBSOLETE ################

// Macro determine the resolution of the detector with run done with a collimator whose
// relative position is not well known.
// Three figures are plotted: 
// 1) the fit to obtain the particle trajectory
// 2) all the "good/reasonable" trajectories
// 3) the obtained position for a given zcol and theta
// It is suggested to perform a fit on the plot 3) to obtain the sigma and FWHM
//
// Suggested ROOT version: 6.26 
// Compatibility issues in v. 5.34.
//
//###################################################################################################
//#   created january 2023 based on A_plotMap_analysis4.C V. Soukeras
//#######################################
//# updated for the new stadard of data 22-02-2023  V. Soukeras
//# updated optimization of the speed 
//# and other minor change1             29-02-2023  D.Torresi
//# The FWHM of the final distribution is now calculated also with a fit (before there was just the 
//# RMS of the  histo			 08-03-2023  D. Torresi
//# FWHM calculated directly on histo   15-03-2023  M. Fisichella	
//###################################################################################################


double deg2rad = 3.14159/180;

double padtox(int pad){
   double xpad=pad*5+2.5;
   return xpad;
};

double rotation(double intercept, double slope, double zcol, double theta){
   double xcol = (zcol - intercept)/slope;
   double xcolprime = (xcol*cos(theta))+(zcol*sin(theta));
   return xcolprime;
};	  

void B_resolutionCalc(int run)
{

   // Dichiarazione variabili
   // variables for the input file
   UShort_t Channel; 
   UShort_t Pad; 
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Charge;
   Double_t Charge_cal;
   UInt_t Flags;
   UShort_t Row;
   UShort_t Section;

   // 
   float DeltaT=1000000;  	// characteristic time of an event in ps  
   int thresh=0;		// charge threshold
   int evMult=4;		// row multiplicity event (selct just event with row multiplicity larger than evMult
   int evNumber=0;		// counter for event
   int evLimit=100;		// plot on screen track of the first evLimit events

   double total_charge = 0;	// total charge of a cluster
   double centroid[5];		// array that contain the centroid of each event
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0; 
  
   // output fit parameter of each track
   double slope,intercept;
   TFitResultPtr fitRes1;

   // variables used for the FWHM
   double fwhm, lowerBin, upperBin;
 
   // geometric variable required for positioning the collimator respect to the detector 
   double xcol = 0;
   double zcol = 0;		// starting value for the position of the collimator
   const int nh=70;		// number of step the collimator is moved far
   double deltaz =-1;		// step of z the collimator is moved
   double theta = 20*deg2rad; 
   double xcolprime = 0;  		// x coordinate of the collimator after rotation.
   double sigmaDistColl[nh];		// sigma of the distribution of the track crossing with the plane of the collimator
   
   double *zrow = new double[5];  // zcoordinate of the row
   zrow[4]=10.8;		// valid for the prototype 1.5
   zrow[3]=21.6+10.80;
   zrow[2]=2*21.6+10.80;
   zrow[1]=3*21.6+10.80;
   zrow[0]=4*21.6+10.80;
 
   // variables file name
   char fileIn[50];
   
   // File opening
   sprintf(fileIn, "../Merged_data/run_%d/merg_%d.root", run, run);
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
   tree->SetBranchAddress("Row",&Row);
   tree->SetBranchAddress("Section",&Section);
   
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
   
   // Graph where the position x-z of the centroid of the cluster are drawn and then fitted to get the track
   TGraph *clusters=new TGraph(0);
   clusters->SetMarkerStyle(21);
   clusters->SetMarkerSize(0.8);
   clusters->SetMarkerColor(kRed);
   
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
   
   TH1D *projection=new TH1D("projection","projection",1000,0,100);
   projection->SetStats(1);
   projection->GetXaxis()->SetTitle("Xcolprime (mm)");

   // define a vector of histograms
   char nomehisto[8];
   std::vector<TH1F*> vlist; 
   for (int nu=0;nu<nh;nu++){ 
      sprintf(nomehisto,"histo%i",nu);
      TH1F *source = new TH1F(Form("h%d",nu),nomehisto,4000,0,200); 
      vlist.push_back(source); 
   } 

   // Fitting function
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,160);
   lin1->SetLineColor(4);
   lin1->SetLineWidth(2);
   TF1 *lin2 = (TF1*) lin1->Clone();     ;

   TCanvas *C31=new TCanvas("c31","proj",800,500,900,400);
   C31->SetFillColor(kWhite);
   TCanvas *C32=new TCanvas("c32","multi proj",800,500,900,400);
   C32->SetFillColor(kWhite);   
   TCanvas *C4=new TCanvas("c4","track fit",800,500,900,400);
   C4->SetFillColor(kWhite);

   TCanvas *CAnode=new TCanvas("CAnode","CAnode",800,500,900,400);
   CAnode->SetFillColor(kWhite);
   CAnode->cd(0);
   anode->Draw();
   upedge->Draw("SAME");
   lowedge->Draw("SAME");
   leftedge->Draw("SAME");
   rigthedge->Draw("SAME");
   
   // take the first reference time
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
     
   //cout<<"Row Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      double xpad= padtox(Pad);    // transform pad number in x position   

      if((Timestamp-timeinit)<DeltaT){
         if(Row==0 && Charge>thresh){
	     flag[0]=1;
             row[0]->Fill(xpad,Charge);
         }
         if(Row==1 && Charge>thresh){
	     flag[1]=1;
	     row[1]->Fill(xpad,Charge);
         }
         if(Row==2 && Charge>thresh){
	     flag[2]=1;
	     row[2]->Fill(xpad,Charge);
         }
         if(Row==3 && Charge>thresh){
	     flag[3]=1;
	     row[3]->Fill(xpad,Charge);
         }
         if(Row==4 && Charge>thresh){
	     flag[4]=1;
	     row[4]->Fill(xpad,Charge);
         }
      }else{
	 if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>evMult){
	    
	    cout<<"Event number "<<evNumber++<<endl;
	    for(int j=0; j<5; j++){
	       total_charge = row[j]->Integral(1,63);          
	       centroid[j]=row[j]->GetMean();	   	       
	       //clusters->SetPoint(j,centroid[j],zrow[j]); 	   	       
	    }//*/
	    
	    // If you want to make fit with less than 5 rows
	    //total_charge = row[0]->Integral(1,63);          
	    int c1=2;int c2=4;
	    centroid[0]=row[c1]->GetMean();	   	       
	    clusters->SetPoint(0,centroid[0],zrow[c1]); 
	    //total_charge = row[2]->Integral(1,63);          
	    centroid[1]=row[c2]->GetMean();	   	       
	    clusters->SetPoint(1,centroid[1],zrow[c2]);  //*/
            
            
	    if(evNumber<evLimit){        
               C4->cd(0);
               clusters->Draw("AP");
               clusters->Fit("lin1","r");
               cout<<"-####--------------"<<endl;
            }else{
               clusters->Fit("lin1","r");
            }
            intercept=lin1->GetParameter(0);
            slope=lin1->GetParameter(1);            
 
	    if(evNumber<evLimit){
               lin2 = (TF1*) lin1->Clone();     
	    }  
            
          //////////////////////// replace with:
          //////// first find intercept between the track and a line tilted of theta and passing per thesupposed position of the collimator
          //////// and tan apply a rotation of -theta to obtain the histo
          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //  rotation of the reference frame by an angle theta
	    if(intercept>-325 && intercept<-150){	//changed with respect the low intensity a-source	             
 	    //if(1){	
 	       xcolprime=rotation(intercept, slope, zcol, theta);  
               projection->Fill(xcolprime);                    
               for(int j=0; j<nh; j++){
                  xcolprime=rotation(intercept, slope, zcol+j*deltaz, theta);  
                  //cout<<j<<"\t"<<zcol+j*deltaz<<"\t"<<xcolprime<<endl;  
                  vlist.at(j)->Fill(xcolprime);
               }
                             
               if(evNumber<evLimit){
      	          CAnode->cd(0);
	          lin2->SetLineColor(4);
	          lin2->Draw("same");
	       }
	    }
         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	       
	    if(evNumber<evLimit){   
   	       C4->Update();
	       CAnode->Update();
	    }
	    
         }
         cout<<"---------------"<<endl;
         timeinit=Timestamp;

         for(int j=0;j<5; j++){
            row[j]->Reset("ICES");
            flag[j]=0;
         }
        
         if(Row==0 && Charge>thresh){
	    flag[0]=1;
            row[0]->Fill(xpad,Charge_cal);
         }
         if(Row==1 && Charge>thresh){
	    flag[1]=1;
            row[1]->Fill(xpad,Charge_cal);
         }
         if(Row==2 && Charge>thresh){
	    flag[2]=1;
            row[2]->Fill(xpad,Charge_cal);
         }
         if(Row==3 && Charge>thresh){
  	    flag[3]=1;
            row[3]->Fill(xpad,Charge_cal);
         }
         if(Row==4 && Charge>thresh){
	    flag[4]=1;
            row[4]->Fill(xpad,Charge_cal);
         }  
      }      
   }     
 
   C31->cd();
   projection->Draw();  
   cout<<"######"<<endl;
   cout<<"\t"<<projection->GetMean()<<"\t"<<projection->GetRMS()<<"\t"<<projection->GetRMS()*2.35<<endl;
   C32->cd();
   for(int j=0; j<nh; j++){
      vlist.at(j)->SetLineColor(j);
      vlist.at(j)->Draw("SAME");
      fitRes1=vlist.at(j)->Fit("gaus","S");
      sigmaDistColl[j]=fitRes1->Value(2);
      //cout<<j<<" "<<zcol+j*deltaz<<"  "<<vlist.at(j)->GetMean()<<"\t"<<vlist.at(j)->GetRMS()<<"\t"<<vlist.at(j)->GetRMS()*2.35;
      //cout<<"|\t|"<<fitRes1->Value(2)<<"\t"<<fitRes1->Value(2)*2.35<<endl;
   }
   
   for(int i=0; i<nh; i++){
   
      lowerBin = vlist.at(i)->FindFirstBinAbove(vlist.at(i)->GetMaximum()/2);
      upperBin = vlist.at(i)->FindLastBinAbove(vlist.at(i)->GetMaximum()/2);
      fwhm = vlist.at(i)->GetBinCenter(upperBin+1) - vlist.at(i)->GetBinCenter(lowerBin-1);
   
      cout<<i<<" "<<zcol+i*deltaz<<"  "<<vlist.at(i)->GetMean()<<"\t"<<vlist.at(i)->GetRMS()<<"\t"<<vlist.at(i)->GetRMS()*2.35;
      cout<<"    \t| "<<sigmaDistColl[i]<<"\t"<<sigmaDistColl[i]*2.35<<" \t| "<<fwhm<<endl;
   }
}
