//###################################################################################################
//#   macro that take as input a tracks file, plot the anode map of each event
//#   //and plot the track by means of a fit procedure
//#   
//#   required as argument the run number
//#            
//#		
//#				
//###################################################################################################
//#   created march 2025  by D. Torresi
//#######################################
//# 
//# 
//###################################################################################################



#include "../Include/openfiles.h"


void C_trackFinder_plot(int run)
{

////////////////////////////////////////////////////////////////////
// Dichiarazione variabili

     
   char anykey;
   
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
   
   // Gcut for selection!
   const TString CutFileA = "GCut/cutGa.root";
   const TString CutFileLi = "GCut/cutGli.root";
  
   
// END: Dichiarazione variabili		//////////////////////////////////////

  
 //#################################################################################################
// OpenFile
   openTrackFile(run); 
   
   int entries=tree->GetEntries();
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
   
   // map of the anode filleD with charge
   TH2D *anode=new TH2D("anode","anode",60,-0.5,59.5,11,-0.75,4.75);
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
   
   //crea un nuovo asse per il numero di row
   TGaxis *axis1 = new TGaxis(-0.5,-0.75,-0.5,4.75,-0.75,4.75,10,"+");
   axis1->SetLabelOffset(-0.025);
   axis1->SetTickLength(0.01);

   TCanvas *C1=new TCanvas("c1","c1",1600,50,900,400);
   C1->SetFillColor(kWhite);
   gPad->SetFrameFillColor(17);
   gPad->SetGridy();
   axis1->Draw();

   /*TCanvas *C21=new TCanvas("c21","distr1",50,860,900,400);
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
   C5->cd(0);*/
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
// Opening the GCut files FOR RUN 332

  /* TCutG *cutGli = new TCutG("cutGli",8);
   cutGli->SetVarX("cl_x_mm[0]");
   cutGli->SetVarY("cl_x_mm[1]");
   cutGli->SetPoint(0,13,65);
   cutGli->SetPoint(1,9,42);
   cutGli->SetPoint(2,10,39);
   cutGli->SetPoint(3,25,50);
   cutGli->SetPoint(4,54,94);
   cutGli->SetPoint(5,69,126);
   cutGli->SetPoint(6,36,142);
   cutGli->SetPoint(7,13,65);
   //cutGli->SaveAs(CutFileLi);
   
   TCutG *cutGa = new TCutG("cutGa",8);
   cutGa->SetVarX("cl_x_mm[0]");
   cutGa->SetVarY("cl_x_mm[1]");
   cutGa->SetPoint(0,17,40);
   cutGa->SetPoint(1,8,28);
   cutGa->SetPoint(2,8,9);
   cutGa->SetPoint(3,39,30);
   cutGa->SetPoint(4,182,193);
   cutGa->SetPoint(5,154,214);
   cutGa->SetPoint(6,55,85);
   cutGa->SetPoint(7,17,40); //*/
   

////////////////////////////////////////////////////////////////////
// Opening the files

   cout<<"Board Channel (pad) Charge (Charge_cal) Timestamp Flags"<<endl;
   for(int i=0;i <entries; i++){
      tree->GetEntry(i);   
      cout<<"entry "<<i<<" "<<endl;
      for(int j=0; j<5; j++){  cout<<cl_padMult[j]<<" ";}
      cout<<endl;
  
     // Alcuni eventi sono scritti male e hanno un numero di molteplicità enorme che manda in crash la macro!
     if(cl_padMult[0]<61 && cl_padMult[1]<61 &&cl_padMult[2]<61 && cl_padMult[3]<61 && cl_padMult[4]<61){
      for(int j=0; j<5; j++){
         for(int h=0; h<cl_padMult[j]; h++){
           anode->Fill(pads_fired[j][h],j,pad_charge[j][h]); //flag[0]=1;
           //cout<<pad_charge[j][h]<<" ";
         }  
         //cout<<endl;
      }    
      //cout<<endl;
     }
      //if(cutGa->IsInside(cl_x_mm[0], cl_x_mm[1])){
      //if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1]) && cl_padMult[0]==3 ){
      if(cl_padMult[1]==30){
        
         anode->Draw("colz");
         axis1->Draw();
         C1->Update();
         cout<<"track number: "<<i<<endl;
         cout<<"phi (deg):  "<<  phi_deg<<endl;
         

         cout<<"SiC (deg):  "<<  sic_fired<<endl;
         //for(int l=0; l<5; l++){
         //   cout<<cl_y_mm[l]-cl_y_mm[0]<<"   \t";
         //}
         cout<<"\n";
         
         cout<<"entry "<<i<<endl;
         cout<<"entry Merged "<<entryMF<<endl;
         cout<<"press any key to continue, q to quit, s to save a plot"<<endl;
         cin>>anykey;
         if(anykey=='q')return ; // Per uscire dal programma
      }
           
      anode->Reset();   
        
    }
    
}
