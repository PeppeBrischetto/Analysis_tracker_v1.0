//###################################################################################################
//#   macro that take as input a merged file, plot the anode map of each event
//#   and plot the track by means of a fit procedure
//#   
//#   required as argument the run number
//#            
//#   Parameters to chage
//# 	 input the DeltaT, time interval that is used to define an event 
//#	 threshold value
//#	 Flag Fstrip if you want to plot strip or not
//#	 rowmult ultiplicita di cluster per definire un evento.
//#		
//#		
//#		
		
//###################################################################################################
//#   created june 2024  by D. Torresi
//#######################################
//# 
//# 
//###################################################################################################

void B_trackFinder_plot(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

   // window that define the event, opened with the first hit
   float DeltaT=1000000;  //in ps
   // energy threshold to defin an hit
   int thresh=0;

   int Fstrip=1;			// flag to plot strip: 0 plot, 1 no plot
   // row multiplicity required to plot the event
   int rowMult=4; 
     
   // tracker variables 
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
   UShort_t Section;
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;
   
   int binmax, max;
   // number of event in the run
   int eventNumber=0;
   
   int np=0; // number of point of the Tgraph

   double *zrow = new double[5];  // zcoordinate of the row
   zrow[0]=18.60;		  // valid for the prototype 2
   zrow[1]=39.80;
   zrow[2]=61.00;
   zrow[3]=82.20;
   zrow[4]=103.40;

   char dummyString[50];
   

   
// END: Dichiarazione variabili		//////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////   
// open tracker file

   char fileIn[50];
   
   if(run<10){
      sprintf(fileIn, "../Merged_data_Br/run_00%i/merg_00%i.root", run, run);
   }else if(run <100){
      sprintf(fileIn, "../Merged_data_Br/run_0%i/merg_0%i.root", run, run);
   }else{
      sprintf(fileIn, "../Merged_data_Br/run_%i/merg_%i.root", run, run);
   }   
   
   TFile *fin = new TFile(fileIn);

   TTree *treeTracker = (TTree*)fin->Get("Data_R");

   treeTracker->SetBranchAddress("Board",&Board);
   treeTracker->SetBranchAddress("Channel",&Channel);
   treeTracker->SetBranchAddress("FineTSInt",&FTS);
   treeTracker->SetBranchAddress("CoarseTSInt",&CTS);
   treeTracker->SetBranchAddress("Timestamp",&Timestamp);
   treeTracker->SetBranchAddress("Charge",&Charge);
   treeTracker->SetBranchAddress("Flags",&Flags);
   treeTracker->SetBranchAddress("Pads",&pad);
   treeTracker->SetBranchAddress("Charge_cal",&Charge_cal);
   treeTracker->SetBranchAddress("Row",&Row);
   treeTracker->SetBranchAddress("Section",&Section);
   
   int entries=treeTracker->GetEntries();
   cout<<" "<<entries<<endl;
// END: open tracker file	//////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Dichiarazione Histo, Canvas, TGraph and Functions
   TH1F *hrow[5];
   for (int i=0; i<5; ++i){
       sprintf(dummyString,"r%i",i);
       hrow[i]=new TH1F(dummyString,dummyString,62,-0.5,61.5);
   }

   TH1D *time[5];
   for (int i=0; i<5; ++i) {
       sprintf(dummyString,"time%i",i);
       time[i]=new TH1D(dummyString,dummyString,1.0E+06,-0.5,1.0E+12);
   }

   for(int i=0;i<5; i++){
     hrow[i]->GetXaxis()->SetTitle("pad");
     hrow[i]->GetYaxis()->SetTitle("counts");
   }
   hrow[1]->SetLineColor(kRed);
   hrow[2]->SetLineColor(kBlue);
   hrow[3]->SetLineColor(kGreen);
   hrow[4]->SetLineColor(kViolet);
   
   // map of the anode fille with charge
   TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,-0.75,4.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
      
   // histo for the track
   TH2D *tracks=new TH2D("tracks","tracks",72,-5.5,66.5,30,-9.25,7.75);
   tracks->SetStats(kFALSE);
   tracks->GetXaxis()->SetTitle("pad");
   tracks->GetYaxis()->SetTitle("row");
   // graph filled with centroids of clusters
   TGraph *grTrack=new TGraph(0);

   // map of anode filled with time
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,-0.75,4.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("row");
   anodeTime->GetYaxis()->SetNdivisions(-11);		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di row
   TGaxis *axis1 = new TGaxis(-0.5,-0.75,-0.5,4.75,-0.75,4.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",1600,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   axis1->Draw();

   TCanvas *C21=new TCanvas("c21","distr1",50,860,900,400);
   C21->SetFillColor(kWhite);

   TCanvas *C22=new TCanvas("c22","distr2",100,900,900,400);
   C22->SetFillColor(kWhite);

   TCanvas *C23=new TCanvas("c23","distr3",150,940,900,400);
   C23->SetFillColor(kWhite);

   TCanvas *C24=new TCanvas("c24","distr4",200,980,900,400);
   C24->SetFillColor(kWhite);

   TCanvas *C25=new TCanvas("c25","distr5",250,1120,900,400);
   C25->SetFillColor(kWhite);


   TCanvas *C3=new TCanvas("c3","c3",1600,450,900,400);
   C3->SetFillColor(kWhite);

   TCanvas *C4=new TCanvas("c4","c4",1700,900,900,400);
   C4->SetFillColor(kWhite);
   
   TCanvas *C5=new TCanvas("c5","c5",900,900,900,400);
   C5->SetFillColor(kWhite);
   C5->cd(0);
   // detector edges
   TLine *edgeLe=new TLine(60.5,-0.75,60.5,4.75);
   TLine *edgeRi=new TLine(-0.5,-0.75,-0.5,4.75);
   TLine *edgeLo=new TLine(-0.5,-0.75,60.5,-0.75);
   TLine *edgeUp=new TLine(60.5,4.75,-0.5,4.75);
   edgeLe->SetLineColor(kViolet);
   edgeRi->SetLineColor(kViolet);
   edgeLo->SetLineColor(kViolet);
   edgeUp->SetLineColor(kViolet);

  // tracks->GetYaxis()->SetRangeUser(-2.0,5.0);
   tracks->Draw();
   edgeLe->Draw();
   edgeRi->Draw();
   edgeLo->Draw();
   edgeUp->Draw();
  
   treeTracker->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   
   double charge = 0.0;
   double total_charge = 0.0;
   double weigthed_pos[62];
   double centroid[5];
   
   TF1 *lin1 = new TF1("lin1","[0]+([1]*x)",0,70);
   double slope,intercept;
   TF1 *fit_result;

   TH2D *anode_fit=new TH2D("anode_fit","anode_fit",620,-0.5,61.5,110,-0.75,4.75);
   anode_fit->SetStats(kFALSE);
   anode_fit->GetXaxis()->SetTitle("pad");
   anode_fit->GetYaxis()->SetTitle("row");
  

   
// END Histo and Canvas 	////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Opening the files

   //cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      treeTracker->GetEntry(i);   

      //if(Charge>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Charge<<"\t("<<Charge_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
         if(Row==0 && Charge>thresh){
           anode->Fill(pad,0.1,Charge); flag[0]=1;
           anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==7 && Charge>thresh && Fstrip==0){ //&& Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
        }
       
        if(Row==1 && Charge>thresh){
        //if(Section==4 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[1]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==10 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==9 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,3.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,3.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==2 && Charge>thresh){
        //if(Section==8 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[2]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
        //if(Section==12 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[3]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
  	}
        if(Row==8 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==4 && Charge>thresh){
        //if(Section==16 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[4]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }
        if(Row==5 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,-0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,-0.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==6 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,Timestamp-timeinit +10);}
        }
      }else{
         if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>rowMult){
            np=0; 
	    for(int j=0; j<5; j++){ 
	       //centroid[j]=0;
	       binmax = hrow[j]->GetMaximumBin(); 
	       max  = hrow[j]->GetBinContent(binmax);
	       //for(int k=1; k<=62; k++){
               //  charge = row[j]->GetBinContent(k);
               //  total_charge = row[j]->Integral(1,62);
	       //weigthed_pos[k] = (k-1)*charge/total_charge;
	       //  centroid[j] = centroid[j] + weigthed_pos[k];
	       //}
	       centroid[j]=hrow[j]->GetMean();
	       if(max>100){grTrack->SetPoint(np++, centroid[j],j);}

	       //cout<<binmax<<"  "<<max<<endl;
	       
               if(j==4){
	          hrow[j]->GetYaxis()->SetRangeUser(0,max*2);
	          anode_fit->Fill(centroid[j],j+1);
                  C4->cd(0);
 	          anode_fit->SetMarkerStyle(20);
	          anode_fit->SetMarkerSize(1);

	          grTrack->Fit("lin1","r");

 	          intercept=lin1->GetParameter(0);
	          slope=lin1->GetParameter(1);

	          fit_result = new TF1("fit_result",Form("%1.8f+(%1.8f*x)",intercept,slope),0.00,62.00);
	          fit_result->SetLineColor(2);
	          fit_result->SetLineWidth(2);
	          fit_result->Draw("same");

	          C5->cd(0);

 	          fit_result->SetLineColor(2);
	          fit_result->SetLineWidth(2);
	          if(np==5){fit_result->Draw("same");}
/*	          if(j==4){
		    anode_fit->Draw();
		    anode_fit->Fit("lin1","r");
		    printf("\n");
		    printf("\n");
		    intercept=lin1->GetParameter(0);
		    slope=lin1->GetParameter(1);
  
  		    TF1 *fit_result = new TF1("fit_result",Form("%1.8f+(%1.8f*x)",intercept,slope),0.00,32.00); 
 		    fit_result->SetLineColor(2);
		    fit_result->SetLineWidth(2);
		    fit_result->Draw("same");
	   
		    C5->cd(0);
		    fit_result->SetLineColor(2);
		    fit_result->SetLineWidth(2);
		    fit_result->Draw("same");//*/
	         }
            }

            // plot the charge map of the anode
            C1->cd(0);
            anode->Draw("colz");
            axis1->Draw();

            // plot centroid
            grTrack->SetMarkerStyle(20);
            grTrack->SetMarkerSize(1);
            grTrack->Draw("P");
            C1->Update();

            // plot the time anode map
            C3->cd(0);
            anodeTime->Draw("colz");
            axis1->Draw();
            C3->Update();

            C21->cd(0);
            hrow[0]->Draw("histo");
            C21->Update();
            C22->cd(0);
	    hrow[1]->Draw("histo");
	    C22->Update();
	    C23->cd(0);
 	    hrow[2]->Draw("histo");
 	    C23->Update();
	    C24->cd(0);
  	    hrow[3]->Draw("histo");
	    C24->Update();
	    C25->cd(0);
	    hrow[4]->Draw("histo");
	    C25->Update();
            C3->Update();
            C4->Update();
            C5->Update();
           
            cout<<"entry "<<i<<endl;
            cout<<"press any key to continue, q to quit, s to save a plot"<<endl;
            cin>>anykey;
            if(anykey=='q')return ; // Per uscire dal programma
            if(anykey=='s'){ 		 // Salvi il plot
               C1->Print("c1.eps");
               C3->Print("C3.eps");
               C5->Print("C5.eps");
   	    }        
	 }
	 cout<<"---------------"<<endl;
	 timeinit=Timestamp;
	 anode->Reset("ICES");
	 anodeTime->Reset("ICES");
	 anode_fit->Reset("ICES");
	 grTrack->Set(0);	 
	 
   	 for(int j=0;j<5; j++){
	    hrow[j]->Reset("ICES");
	    flag[j]=0;
	 }
        
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,0.1,Charge); flag[0]=1;
           anodeTime->Fill(pad,0.1,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==7  && Charge>thresh && Fstrip==0){ //&& Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
        }
       
        if(Row==1 && Charge>thresh){
        //if(Section==4 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[1]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==10 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==9 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==2 && Charge>thresh){
        //if(Section==8 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[2]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
        //if(Section==12 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[3]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
  	}
        if(Row==8 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==4 && Charge>thresh){
        //if(Section==16 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[4]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }
        if(Row==5 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,-0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,-0.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==6 && Charge>thresh && Fstrip==0){
           for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,Timestamp-timeinit +10);}
        }
          
      } 
        
   }
      
}
