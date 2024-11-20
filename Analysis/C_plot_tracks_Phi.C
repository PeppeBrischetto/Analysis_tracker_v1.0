//###################################################################################################
//#   macro that plot tracks from files Tracks
//#   
//#   required as argument the run number and y/n for the presence of a SiC file
//#   theta and phi angles are defined with respect to the z-axis 
//###################################################################################################
//#   created october 2024 from scratch D. Torresi
//#######################################
//#   modified:
//###################################################################################################

#include "../Include/openfiles.h"

void C_plot_tracks_Phi(int run){

//###################################################################
//    VARIABLES

   int qualityFlag=0;
   int flagA=0;
   double totalCharge=0;
   char pippo;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#########################################################
// GRAPHICS

   int xmin=-80;
   int xmax= 118;
   int DeltaX=xmax-xmin;
   int ymin=-10;
   int ymax= 160;
   int DeltaY=ymax-ymin;

   TCanvas *C1=new TCanvas("tracker","tracker",250,160,DeltaX*5,DeltaY*5);
   TH2F *bg=new TH2F("bg","",100,xmin,xmax,100,ymin,ymax); 
   bg->SetStats(0); 
   TLine *edgeLe=new TLine(0,0,0,150);
   TLine *edgeRi=new TLine(108,0,108,150);
   TLine *edgeLo=new TLine(0,0,108,0);
   TLine *edgeUp=new TLine(0,150,108,150);

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
      //for(int j=0; j<5; j++){cout<<cl_x[j]<<"  "; }
      cout<<"|"<<phi<<endl;
      cout<<endl;
      //line->SetParameter(0,interceptT);
      //line->SetParameter(1,slopeT);
      //line->Draw	("SAME");
      qualityFlag=0;
      totalCharge=0;
      
      // reject tracks with first or last pad of the last row hit
      //for (int j=0; j<cl_padMult[4]; j++) {
                   
      //   cout<<a_pads_fired[4][j]<<"\t";
      //   if ((a_pads_fired[4][j] == 1) || (a_pads_fired[4][j] == 2) || (a_pads_fired[4][j] == 58) || (a_pads_fired[4][j] == 57)){
      //      qualityFlag=1;
      //   }
      //}   
      //cout<<endl;
     
      // selection on PadMultiplicity
      //for(int k=0; k<5; k++){
      //   cout<<cl_padMult[k]<<"\t";
      //   if(cl_padMult[k]<4)qualityFlag=1;         
      //}
     
      // selection on total charge on the last row, make the average of the first 4 row ad if the charge of the last 
      // row is smaller or larger of 50% of the average of the first row the track is removed
      //for(int k=0; k<4; k++){
      //   cout<<cl_charge[k]<<"\t";
      //   totalCharge=totalCharge+cl_charge[k];
      //}
      //if(abs(totalCharge/4-cl_charge[4])>(totalCharge/8)) qualityFlag=1;
      //cout<<"\n"<<"# "<<totalCharge/4<<"\t"<<cl_charge[4]<<"\t"<<abs(totalCharge/4-cl_charge[4])<<"\t"<<totalCharge/40<<endl;
      
      //   if(cl_padMult[k]<4)qualityFlag=1;         
      
      //for(int k=0; k<5; k++){
      //   cout<<cl_padMult[k]<<"\t";
         //if(cl_padMult[k]<4)qualityFlag=1;         
      //}
      //cout<<endl;
      //for(int k=0; k<5; k++){
      //   cout<<cl_charge[k]<<"\t";
         //if(cl_padMult[k]<4)qualityFlag=1;         
      
      //}

      
      
      //cout<<" | "<<qualityFlag<<endl;
      
      
      if(qualityFlag==0){   
         runningLine=new TF1("line","[0]+([1]*x)",-100,300);
         runningLine->SetParameter(0,interceptP);
         runningLine->SetParameter(1,slopeP);
         runningLine->Draw("same");
         runningLine->SetLineColor(kGreen+1);
      }else{
         runningLine2=new TF1("line","[0]+([1]*x)",-100,300);
         runningLine2->SetParameter(0,-interceptP/slopeP);
         runningLine2->SetParameter(1,1./slopeP);
         runningLine2->Draw("same");
         runningLine2->SetLineColor(kRed);
      }
      
      line->SetParameter(0,interceptP);
      line->SetParameter(1,slopeP);
      line->Draw("same");
      
      cout<<endl;
      //X1->SetX(0);
      //X1->SetY(interceptT);
      //X1->Draw("SAME");      
      //X2->SetX(interceptT);
      //X2->SetY(0);
      //X2->Draw("SAME");      
          
      C1->Update();
      if(flagA==0){
         cout<<"press anykey to continue"<<endl;
         cin>>pippo;
         if(pippo=='q') break;
      }
      if(pippo=='c') flagA=1;
      
   }


}


