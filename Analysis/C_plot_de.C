//################################################################################################################################################

//Macro that reads the information from a third level root file and plots the cumulative charge from each cluster of each row, the cumulative charge measured by the strips and the sum of the two (rows+strip)//

//To run simply open a root session and type: .x C_plot_de.C(run number)//

//Created in December 2024 by O. Sgouros//

//################################################################################################################################################



#include "../Include/openfiles.h"


void C_plot_de(int run)
{

cout<<""<<endl;
cout<<""<<endl;



//#################################################################################################
//OpenFile
   openTrackFile(run);

//#################################################################################################

  double binmax;
  double xmax;
  double DeRows;
  double DeStrip;


// Declaring Histos 

   TH1F *de_rows = new TH1F("de_rows","",20000,0.0,500000.0);      //Histogram with the charge measured by the 5 rows//
   TH1F *de_rows_SiC = new TH1F("de_rows","",20000,0.0,500000.0);      //Histogram with the charge measured by the 5 rows//
   TH1F *de_strips = new TH1F("de_strips","",20000,0.0,500000.0);  //Histogram with the charge measured by the 4 strips//
   TH1F *de_tot = new TH1F("de_tot","",20000,0.0,500000.0);        //Histogram with the sum (charge_rows+charge_strips) //
   
   de_rows->GetXaxis()->SetTitle("Charge rows (a.u.)");
   de_rows->GetXaxis()->SetTitleOffset(1.05);
   de_rows->GetXaxis()->SetTitleSize(0.040);
   de_rows->GetXaxis()->SetTitleFont(62);
   de_rows->GetXaxis()->SetLabelFont(62);
   de_rows->GetXaxis()->SetLabelOffset(0.008);
   de_rows->GetXaxis()->SetLabelSize(0.030);
   de_rows->GetYaxis()->SetNdivisions(505);
   de_rows->GetXaxis()->CenterTitle();

   de_rows->GetYaxis()->SetTitle("N (Counts)");
   de_rows->GetYaxis()->SetTitleOffset(1.10);
   de_rows->GetYaxis()->SetTitleSize(0.040);
   de_rows->GetYaxis()->SetTitleFont(62);
   de_rows->GetYaxis()->SetLabelFont(62);
   de_rows->GetYaxis()->SetLabelOffset(0.008);
   de_rows->GetYaxis()->SetLabelSize(0.030);
   de_rows->GetYaxis()->SetNdivisions(505);
   de_rows->GetYaxis()->CenterTitle();
   
   de_strips->GetXaxis()->SetTitle("Charge strips (a.u.)");
   de_strips->GetXaxis()->SetTitleOffset(1.05);
   de_strips->GetXaxis()->SetTitleSize(0.040);
   de_strips->GetXaxis()->SetTitleFont(62);
   de_strips->GetXaxis()->SetLabelFont(62);
   de_strips->GetXaxis()->SetLabelOffset(0.008);
   de_strips->GetXaxis()->SetLabelSize(0.030);
   de_strips->GetYaxis()->SetNdivisions(505);
   de_strips->GetXaxis()->CenterTitle();

   de_strips->GetYaxis()->SetTitle("N (Counts)");
   de_strips->GetYaxis()->SetTitleOffset(1.10);
   de_strips->GetYaxis()->SetTitleSize(0.040);
   de_strips->GetYaxis()->SetTitleFont(62);
   de_strips->GetYaxis()->SetLabelFont(62);
   de_strips->GetYaxis()->SetLabelOffset(0.008);
   de_strips->GetYaxis()->SetLabelSize(0.030);
   de_strips->GetYaxis()->SetNdivisions(505);
   de_strips->GetYaxis()->CenterTitle();
   
   
   de_tot->GetXaxis()->SetTitle("Charge Rows+strips (a.u.)");
   de_tot->GetXaxis()->SetTitleOffset(1.05);
   de_tot->GetXaxis()->SetTitleSize(0.040);
   de_tot->GetXaxis()->SetTitleFont(62);
   de_tot->GetXaxis()->SetLabelFont(62);
   de_tot->GetXaxis()->SetLabelOffset(0.008);
   de_tot->GetXaxis()->SetLabelSize(0.030);
   de_tot->GetYaxis()->SetNdivisions(505);
   de_tot->GetXaxis()->CenterTitle();

   de_tot->GetYaxis()->SetTitle("N (Counts)");
   de_tot->GetYaxis()->SetTitleOffset(1.10);
   de_tot->GetYaxis()->SetTitleSize(0.040);
   de_tot->GetYaxis()->SetTitleFont(62);
   de_tot->GetYaxis()->SetLabelFont(62);
   de_tot->GetYaxis()->SetLabelOffset(0.008);
   de_tot->GetYaxis()->SetLabelSize(0.030);
   de_tot->GetYaxis()->SetNdivisions(505);
   de_tot->GetYaxis()->CenterTitle();

   TCanvas *c1=new TCanvas("c1","",800,500,1000,800);
   TCanvas *c2=new TCanvas("c2","",800,500,1000,800);
   TCanvas *c3=new TCanvas("c3","",800,500,1000,800);
        
   
      
   for(int i=0; i<entries; i++){
      tree->GetEntry(i);
      DeRows=cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4];
      //DeRows=cl_charge[1];
      de_rows->Fill(DeRows,1);
      if(sic_fired)de_rows_SiC->Fill(DeRows,1);
      de_strips->Fill(cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10],1);
      de_tot->Fill(cl_charge[0]+cl_charge[1]+cl_charge[2]+cl_charge[3]+cl_charge[4]+cl_charge[5]+cl_charge[6]+cl_charge[7]+cl_charge[8]+cl_charge[9]+cl_charge[10],1);
   
 }
    
     

   c1->cd();
   de_rows->SetLineColor(kBlack);
   de_rows->Draw();
   de_rows_SiC->SetLineColor(kRed);
   //de_rows_SiC->Draw("same");  
   binmax = de_rows->GetMaximumBin(); 
   xmax = de_rows->GetXaxis()->GetBinCenter(binmax);
   //de_rows->GetXaxis()->SetRangeUser(xmax-500,xmax+500);


   c2->cd();
   de_strips->SetLineColor(2);
   de_strips->Draw();
   
   binmax = de_strips->GetMaximumBin(); 
   xmax = de_strips->GetXaxis()->GetBinCenter(binmax);
   //de_strips->GetXaxis()->SetRangeUser(xmax-500,xmax+500);

     
   c3->cd();
   de_tot->SetLineColor(4);
   de_tot->Draw();
   
   binmax = de_tot->GetMaximumBin(); 
   xmax = de_tot->GetXaxis()->GetBinCenter(binmax);
   //de_tot->GetXaxis()->SetRangeUser(xmax-500,xmax+500);


   
}

