
void A_plotMap_time(int run)
{

   // Dichiarazione variabili
   UShort_t Channel; 
   UShort_t pad; 
   UShort_t FTS;
   ULong64_t CTS;
   ULong64_t Timestamp;
   UShort_t Board;
   UShort_t Energy;
   Double_t Energy_cal;
   UInt_t Flags;
   
   char anykey;
   int flag[5];
   for(int i =0; i<5; i++) flag[i]=0;
   float DeltaT=1.0e12;  //in ps
   
   int binmax, max;
   int thresh=10;
   
   // Apertura file
   char fileIn[50];
   sprintf(fileIn, "Merged_Data/merged_run%d_cal.root", run);
   TFile *fin = new TFile(fileIn);
   //TFile *fin = new TFile("SDataR_run_65.root");
   TTree *tree = (TTree*)fin->Get("Data_R");

   tree->SetBranchAddress("Board",&Board);
   tree->SetBranchAddress("Channel",&Channel);
   tree->SetBranchAddress("FineTSInt",&FTS);
   tree->SetBranchAddress("CoarseTSInt",&CTS);
   tree->SetBranchAddress("Timestamp",&Timestamp);
   tree->SetBranchAddress("Energy",&Energy);
   tree->SetBranchAddress("Flags",&Flags);
   tree->SetBranchAddress("Pads",&pad);
   tree->SetBranchAddress("Energy_cal",&Energy_cal);
   
   int entries=tree->GetEntries();
   cout<<" "<<entries<<endl;

   // Dichiarazione Histo 
   TH1F *row[5];
   row[0]=new TH1F("r1","r1",62,-0.5,61.5);
   row[1]=new TH1F("r2","r2",62,-0.5,61.5);
   row[2]=new TH1F("r3","r3",62,-0.5,61.5);
   row[3]=new TH1F("r4","r4",62,-0.5,61.5);
   row[4]=new TH1F("r4","r4",62,-0.5,61.5);

   for(int i=0;i<5; i++){
     row[i]->GetXaxis()->SetTitle("pad");
     row[i]->GetYaxis()->SetTitle("counts");
   }
   row[1]->SetLineColor(kRed);
   row[2]->SetLineColor(kBlue);
   row[3]->SetLineColor(kGreen);
   row[4]->SetLineColor(kViolet);
   
   
   TH2D *anode=new TH2D("anode","anode",62,-0.5,61.5,11,0.25,5.75);
   anode->SetStats(kFALSE);
   anode->GetXaxis()->SetTitle("pad");
   anode->GetYaxis()->SetTitle("raw");
   anode->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anode->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   TH2D *anodeTime=new TH2D("anodeTime","anodeTime",62,-0.5,61.5,11,0.25,5.75);
   anodeTime->SetStats(kFALSE);
   anodeTime->GetXaxis()->SetTitle("pad");
   anodeTime->GetYaxis()->SetTitle("raw");
   anodeTime->GetYaxis()->SetNdivisions(-11); 		// setta il numero di divisioni per la grid
   anodeTime->GetYaxis()->SetLabelSize(0);		// rimuovi il label di questo asse
   
   //crea un nuovo asse per il numero di raw
   TGaxis *axis1 = new TGaxis(-0.5,0.25,-0.5,5.75,0.25,5.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",800,50,900,400);
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

   TCanvas *C2=new TCanvas("c2","c2",800,500,900,400);
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
   TCanvas *C3=new TCanvas("c3","c3",800,500,900,400);
   C3->SetFillColor(kWhite);
   // Lettura file
  
   tree->GetEntry(0);
   ULong64_t timeinit=Timestamp;
   cout<<" time init: "<<timeinit<<endl;
   

   
   cout<<"Board Channel (pad) Energy (Energy_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);
      
      //if(Energy>0){cout<< Board<<" \t"<<Channel<<" ("<<pad<<")  "<<"\t"<<Energy<<"\t("<<Energy_cal<<")\t"<<Timestamp<<"\t"<<Flags<<endl;}
      if(Timestamp-timeinit<DeltaT){
        if(Board==22646 && Energy>thresh){
           anode->Fill(pad,1,Energy); flag[0]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           row[0]->Fill(pad,Energy);
        }
        if(Board==22645 && Energy>thresh){
           anode->Fill(pad,2,Energy); flag[1]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           row[1]->Fill(pad,Energy);
        }
        if(Board==22644 && Energy>thresh){
           anode->Fill(pad,3,Energy); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           row[2]->Fill(pad,Energy);
        }
        if(Board==22643 && Energy>thresh){
           anode->Fill(pad,4,Energy); flag[3]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           row[3]->Fill(pad,Energy);
        }
        if(Board==22642 && Energy>thresh){
           anode->Fill(pad,5,Energy); flag[4]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           row[4]->Fill(pad,Energy);
        }

        
      }else{
           timeinit=Timestamp;
           cout<<"Timestamp: "<<Timestamp<<endl;
           C1->cd(0);
           anode->Draw("colz");
           //C1->Update();      
           //pad1->GetFrame()->SetFillColor(kGray);  
           axis1->Draw();
           C1->Update();
           C3->cd(0);
           anodeTime->Draw("colz");
           //C1->Update();      
           //pad1->GetFrame()->SetFillColor(kGray);  
           axis1->Draw();
           C3->Update();

           C2->cd(0);
           row[0]->Draw("histo");
           binmax = row[0]->GetMaximumBin(); 
           max = row[0]->GetBinContent(binmax);
           //cout<<binmax<<"  "<<max<<endl;
           row[0]->GetYaxis()->SetRangeUser(0,max*2);
           for(int i=1; i<5; i++){row[i]->Draw("histo same");}
           C2->Update();
           
           cout<<"entry "<<i<<endl;
           cout<<"press any key to continue, q to quit, s to save a plot"<<endl;
           cin>>anykey;
           if(anykey=='q')return 0; // Per uscire dal programma
           if(anykey=='s'){ 		 // Salvi il plot
              C1->Print("c1.pdf");
   	          
        }
        cout<<"---------------"<<endl;
        timeinit=Timestamp;
        anode->Reset("ICES");
        anodeTime->Reset("ICES");
        for(int i=0;i<5; i++){
           row[i]->Reset("ICES");
           flag[i]=0;
        }
       
        
        if(Board==22646 && Energy>thresh){
           anode->Fill(pad,1,Energy); flag[0]=1;
           anodeTime->Fill(pad,1,Timestamp-timeinit +10);
           row[0]->Fill(pad,Energy);
        }
        if(Board==22645 && Energy>thresh){
           anode->Fill(pad,2,Energy); flag[1]=1;
           anodeTime->Fill(pad,2,Timestamp-timeinit +10);
           row[1]->Fill(pad,Energy);
        }
        if(Board==22644 && Energy>thresh){
           anode->Fill(pad,3,Energy); flag[2]=1;
           anodeTime->Fill(pad,3,Timestamp-timeinit +10);
           row[2]->Fill(pad,Energy);
        }
        if(Board==22643 && Energy>thresh){
           anode->Fill(pad,4,Energy); flag[3]=1;
           anodeTime->Fill(pad,4,Timestamp-timeinit +10);
           row[3]->Fill(pad,Energy);
        }
        if(Board==22642 && Energy>thresh){
           anode->Fill(pad,5,Energy); flag[4]=1;
           anodeTime->Fill(pad,5,Timestamp-timeinit +10);
           row[4]->Fill(pad,Energy);
        }
          
      } 
        
   }
      
}
