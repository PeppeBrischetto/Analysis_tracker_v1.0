//###################################################################################################
//#   macro that take as input a merged file and plot the anode map of each event
//#   
//#   required as argument the run number
//#            as input the DeltaT, time interval that is used to define an event
//# 			   array with the number of digitizers used
//###################################################################################################
//#   created march 2021
//#######################################
//# updated for the nudaq acq files   25 11 2022 V.Soukeras D. Torresi
//# updated: read the Row parameter   28 11 2022 D. Torresi
//# updated: plot the "strips"         1 12 2022 D. Torresi
//###################################################################################################


void A_eventFinder_plot(int run)
{

   // Dichiarazione variabili

   // window the define the event, opened with the first hit
   float DeltaT=2000000;  //in ps

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
   int flagM=0;
   
   int binmax, max;
   // energy threshold to defin an hit
   int thresh=0;
   // number of event in the run
   int eventNumber=0;
   
   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "../Merged_data/run_%i/merg_%i.root", run, run);
   //sprintf(fileIn, "../Merged_data/run_184/merg_184.root", run);
   //sprintf(fileIn, "../Merged_data/run_238/merg_238.root", run);
   cout<<fileIn<<endl;
   TFile *fin = new TFile(fileIn);
   //TFile *fin = new TFile("SDataR_run_65.root");
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Charge",&Charge);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Charge_cal",&Charge_cal);
   tree->SetBranchAddress("Row",&Row);
   tree->SetBranchAddress("Section",&Section);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *hrow[5];
   hrow[0]=new TH1F("r1","r1",62,-0.5,61.5);
   hrow[1]=new TH1F("r2","r2",62,-0.5,61.5);
   hrow[2]=new TH1F("r3","r3",62,-0.5,61.5);
   hrow[3]=new TH1F("r4","r4",62,-0.5,61.5);
   hrow[4]=new TH1F("r4","r4",62,-0.5,61.5);

   for(int i=0;i<5; i++){
     hrow[i]->GetXaxis()->SetTitle("pad");
     hrow[i]->GetYaxis()->SetTitle("counts");
   }
   hrow[1]->SetLineColor(kRed);
   hrow[2]->SetLineColor(kBlue);
   hrow[3]->SetLineColor(kGreen);
   hrow[4]->SetLineColor(kViolet);
   
   
   //TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,-0.75,4.75);
   TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,0.25,5.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("row");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,-0.75,4.75);
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,0.25,5.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("row");
   anodeTime->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di row
   TGaxis *axis1 = new TGaxis(-0.5,0.25,-0.5,5.75,0.25,5.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",800,0,900,350);
   C1->SetFillColor(kWhite);
   //C1->SetFrameFillColor(kWhite);
   /*TPad *pad1 = new TPad("pad1","pad1", 0.02,0.05,0.98,0.95,21);
   pad1->Draw();   
   pad1->cd();
   pad1->SetFillColor(kGray);
   pad1->GetFrame()->SetFillColor(18);
   pad1->GetFrame()->SetBorderMode(-1);
   pad1->GetFrame()->SetBorderSize(1);
   pad1->SetGridy(); 	*/
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   axis1->Draw();

   TCanvas *C2=new TCanvas("c2","c2",200,550,900,350);
   C2->SetFillColor(kWhite);
/*   TPad *pad2 = new TPad("pad2","pad2", 0.02,0.05,0.98,0.95,21);
   pad2->Draw();   
   pad2->cd();
   pad2->SetFillColor(kWhite);
   pad2->GetFrame()->SetFillColor(18);
   pad2->GetFrame()->SetBorderMode(-1);
   pad2->GetFrame()->SetBorderSize(1);
   pad2->SetGridy(); 	
  */ 
   TCanvas *C3=new TCanvas("c3","c3",800,500,900,350);
   C3->SetFillColor(kWhite);
   // Lettura file
  
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   
   TLine *edgeL=new TLine(23.5,0.25,23.5,5.75);
   TLine *edgeU=new TLine(4.5,0.25,4.5,5.75);
   
   
   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      
      //if(Energy>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Energy<<"\t("<<Energy_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if((Timestamp-timeinit)<DeltaT){
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,5,Charge); flag[0]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==7 && Charge>thresh){ //&& Charge>thresh){
           cout<<" pippo "<<endl;
           for(int g=0;g<62;g++){anode->Fill(g,3.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,3.5,(Timestamp-timeinit +10)/10);}
        }
       
        if(Row==1 && Charge>thresh){
        //if(Section==4 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[1]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==10 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==9 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==2 && Charge>thresh){
        //if(Section==8 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
        //if(Section==12 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[3]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
  	}
        if(Row==8 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==4 && Charge>thresh){
        //if(Section==16 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[4]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }
        if(Row==5 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,5.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,5.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==6 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,Timestamp-timeinit +10);}
        }
      }else{
        if(flag[0]+flag[1]+flag[2]+flag[3]+flag[4]>=0){
           C1->cd(0);
           anode->Draw("colz");
           //C1->Update();      
           //pad1->GetFrame()->SetFillColor(kGray);  
           axis1->Draw();
           edgeL->Draw();
           edgeU->Draw();
           C1->Update();
           C3->cd(0);
           anodeTime->Draw("colz");
           //C1->Update();      
           //pad1->GetFrame()->SetFillColor(kGray);  
           axis1->Draw();
           C3->Update();

           C2->cd(0);
           hrow[0]->Draw("histo");
           binmax = hrow[0]->GetMaximumBin(); 
           max = hrow[0]->GetBinContent(binmax);
           //cout<<binmax<<"  "<<max<<endl;
           hrow[0]->GetYaxis()->SetRangeUser(0,max*2);
           for(int i=1; i<5; i++){hrow[i]->Draw("histo same");}
           C2->Update();

           eventNumber++;
           cout<<"entry "<<i<<endl;
           cout<<"Event number: "<<eventNumber<<endl;
           cout<<"press any key to continue, q to quit, s to save a plot, c to continue till the end"<<endl;
           if(flagM==0)cin>>anykey;
           if(anykey=='q')return 0; // Per uscire dal programma
           if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.pdf");
   	   }        
   	   if(anykey=='c')flagM=1;
                      
        }
        cout<<"---------------"<<endl;
        timeinit=Timestamp;
        anode->Reset("ICES");
        anodeTime->Reset("ICES");
        for(int i=0;i<5; i++){
           hrow[i]->Reset("ICES");
           flag[i]=0;
        }
        
        if(Row==0 && Charge>thresh){
           anode->Fill(pad,5,Charge); flag[0]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           hrow[0]->Fill(pad,Charge);
        }
        if(Row==7  && Charge>thresh){ //&& Charge>thresh){
           cout<<" pippo "<<endl;
           for(int g=0;g<62;g++){anode->Fill(g,3.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,3.5,(Timestamp-timeinit +10)/10);}
        }
       
        if(Row==1 && Charge>thresh){
        //if(Section==4 && Charge>thresh){
           anode->Fill(pad,4,Charge); flag[1]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           hrow[1]->Fill(pad,Charge);
        }
        if(Row==10 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,0.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,0.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==9 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,1.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,1.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==2 && Charge>thresh){
        //if(Section==8 && Charge>thresh){
           anode->Fill(pad,3,Charge); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           hrow[2]->Fill(pad,Charge);
        }
        if(Row==3 && Charge>thresh){
        //if(Section==12 && Charge>thresh){
           anode->Fill(pad,2,Charge); flag[3]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           hrow[3]->Fill(pad,Charge);
  	}
        if(Row==8 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,2.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,2.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==4 && Charge>thresh){
        //if(Section==16 && Charge>thresh){
           anode->Fill(pad,1,Charge); flag[4]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           hrow[4]->Fill(pad,Charge);
        }
        if(Row==5 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,5.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,5.5,(Timestamp-timeinit +10)/10);}
        }
        if(Row==6 && Charge>thresh){
           for(int g=0;g<62;g++){anode->Fill(g,4.5,Charge);}
           for(int g=0;g<62;g++){anodeTime->Fill(g,4.5,Timestamp-timeinit +10);}
        }

          
      } 
        
   }
      
}
