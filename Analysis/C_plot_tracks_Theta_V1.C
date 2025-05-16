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

void C_plot_tracks_Theta_V1(int run){

//###################################################################
//    VARIABLES

   int qualityFlag=0;
   int flagA=0;
   double totalCharge=0;
   char pippo;

//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################
// Graphyical cut definition

   TCutG *cutGli = new TCutG("cutGli",8);
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
   cutGli->SetPoint(8,13,65);
   
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
   cutGa->SetPoint(7,17,40);

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
      //for(int j=0; j<5; j++){cout<<cl_x[j]<<"  "; }
      cout<<"theta| "<<theta_deg<<endl;
      cout<<"phi|   "<<phi_deg<<endl;
      cout<<endl;
                 
      if(cutGli->IsInside(cl_x_mm[0], cl_x_mm[1])){   
         runningLine=new TF1("line","[0]+([1]*x)",-100,300);
         runningLine->SetParameter(0,-interceptT/slopeT);
         runningLine->SetParameter(1,1./slopeT);
         runningLine->Draw("same");
         runningLine->SetLineColor(kRed);
      }else{
         runningLine2=new TF1("line","[0]+([1]*x)",-100,300);
         runningLine2->SetParameter(0,-interceptT/slopeT);
         runningLine2->SetParameter(1,1./slopeT);
         runningLine2->Draw("same");
         runningLine2->SetLineColor(kGreen+1);
      }
      
      line->SetParameter(0,-interceptT/slopeT);
      line->SetParameter(1,1./slopeT);
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


