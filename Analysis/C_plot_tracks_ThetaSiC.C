//###################################################################################################
//#   macro that plot tracks from files Tracks if there is a SiC hit
//#   
//#   required as argument the run number and y/n for the presence of a SiC file
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created october 2024 from scratch D. Torresi
//#######################################
//#   modified:
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_tracks_ThetaSiC(int run){

//###################################################################
//    VARIABLES

   int qualityFlag=0;
   int flagA=0;
   double totalCharge=0;
   char pippo;
   int trackSicCounter=0;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#########################################################
// GRAPHICS

   int xmin=-10;
   int xmax= 310;
   int DeltaX=xmax-xmin;
   int ymin=-80;
   int ymax= 118;
   int DeltaY=ymax-ymin;

   TCanvas *C1=new TCanvas("tracker","tracker",250,160,DeltaX*5,DeltaY*5);
   TH2F *bg=new TH2F("bg","",100,xmin,xmax,100,ymin,ymax); 
   bg->SetStats(0); 
   TLine *edgeLe=new TLine(0,0,0,108);
   TLine *edgeRi=new TLine(300,0,300,108);
   TLine *edgeLo=new TLine(0,0,300,0);
   TLine *edgeUp=new TLine(0,108,300,108);

   bg->Draw();
   edgeLe->SetLineColor(kViolet);
   edgeLe->Draw("SAME"	);
   edgeRi->SetLineColor(kViolet);
   edgeRi->Draw("SAME"	);
   edgeLo->SetLineColor(kViolet);
   edgeLo->Draw("SAME"	);
   edgeUp->SetLineColor(kViolet);
   edgeUp->Draw("SAME"	); 
   
   TF1 *runningLine;		// in order to keep all the line in the same plot   
   TF1 *runningLine2;
           
   TF1 *line = new TF1("line","[0]+([1]*x)",-100,300);
   line->SetLineColor(kBlue);
   TF1 *line2 = new TF1("line2","[0]+([1]*x)",-100,300);
   line2->SetLineColor(kViolet);  
   
   for(int i=0; i<entries;i++){
      tree->GetEntry(i);
      printf("\rentry %i/%i.", i+1, entries);
      fflush(stdout);
      flagA=1;
      
      //for(int j=0; j<5; j++){cout<<cl_x[j]<<"  "; }
      //cout<<"theta| "<<theta_deg<<endl;
      //cout<<"phi|   "<<phi_deg<<endl;
      //cout<<endl;
      //line->SetParameter(0,interceptT);
      //line->SetParameter(1,slopeT);
      //line->Draw	("SAME");
      qualityFlag=0;
      totalCharge=0;
   
     for(int j=0; j<5; j++){
     
     if( cl_x_rms[j]>5 ){qualityFlag=1;}   
     
 }
      if( sic_fired && qualityFlag==0){      
         runningLine=new TF1("line","[0]+([1]*x)",-100,300);
         runningLine->SetParameter(0,-interceptT/slopeT);
         runningLine->SetParameter(1,1./slopeT);
         runningLine->Draw("same");
         runningLine->SetLineColor(kGreen+1);
         trackSicCounter++;
         //flagA=0;
      }
            
      line->SetParameter(0,-interceptT/slopeT);
      line->SetParameter(1,1./slopeT);
      //line->Draw("same");
     
          
      C1->Update();
      if(flagA==0){
         cout<<"entry is: "<<i<<endl;
         cout<<"press anykey to continue"<<endl;
         cin>>pippo;
         if(pippo=='q') break;
      }
      if(pippo=='c') flagA=1;
      cout<<endl;
      printf(" \033[41;37m Track with SiC %i.\033[0m ", trackSicCounter);
      cout<<endl;
      cout<<endl;
      //fflush(stdout);
      //runningLine->Reset();
   }

}


