//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created 7 Jun 2024 by A. Pitronaci, based on C_plot_theta.C available in the present directory
//#######################################
//#   updated: 10 June 2024, extended for no-segmeneted rows, i.e. 5,6,7,8,9,10 (strips)
//# 
//###################################################################################################

void C_plot_theta(int run)
{
   /* Main variables referred to input-tree */
   Double_t cl_x[5] = {0.};
   Double_t cl_x_mm[5] = {0.};
   Double_t cl_y[5] = {0.};
   Double_t cl_y_mm[5] = {0.};
   Double_t cl_x_rms[5] = {0.};
   Double_t cl_charge[11] = {0.};
   Int_t cl_padMult[5] = {0};
   Double_t phi = 0.;
   Double_t theta = 0.;
   Double_t phi_deg = 0.;
   Double_t theta_deg = 0.;
   Int_t sic_fired = 0;
   Double_t sic_charge = 0.;
   Double_t energySic = 0.;
  
   /* Reading block variables */  
   char fileInName[50];
   if(run<10){
      sprintf(fileInName, "../../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileInName, "../../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileInName, "../../Tracks/tracks_run%i.root", run);
   } 
   TFile *fileIn = new TFile(fileInName);
   TTree *treeTracks = (TTree*)fileIn->Get("Data_R");
   
   /* Setting tree-Branches */
   treeTracks->SetBranchAddress("cl_x", &cl_x);
   treeTracks->SetBranchAddress("cl_x_mm", &cl_x_mm);
   treeTracks->SetBranchAddress("cl_y", &cl_y);
   treeTracks->SetBranchAddress("cl_y_mm", &cl_y_mm);
   treeTracks->SetBranchAddress("cl_x_rms", &cl_x_rms);
   treeTracks->SetBranchAddress("cl_padMult", &cl_padMult);
   treeTracks->SetBranchAddress("cl_charge", &cl_charge);
   treeTracks->SetBranchAddress("phi", &phi);
   treeTracks->SetBranchAddress("theta", &theta);
   treeTracks->SetBranchAddress("theta_deg",&theta_deg);
   treeTracks->SetBranchAddress("phi_deg",&phi_deg);
   treeTracks->SetBranchAddress("sic_fired",&sic_fired);
   treeTracks->SetBranchAddress("energySic",&energySic);
   
   /* Start reading block */
   int entries=treeTracks->GetEntries();
   cout<<"Entries tracks file "<< entries <<endl;
     
   TH1F *histoTheta=new TH1F("","",1000,-80,50);
   histoTheta->SetStats(0);
   histoTheta->GetXaxis()->SetTitle("#theta (deg)");
   histoTheta->GetYaxis()->SetTitle("Counts");
   histoTheta->SetLineWidth(2);
   
   /* Theta histogram with 1st threshold on charge: 2000*/
   TH1F* h_theta_geq2000 = new TH1F("","",1000,-80,50);
   h_theta_geq2000->SetStats(0);
   h_theta_geq2000->GetXaxis()->SetTitle("#theta (deg)");
   h_theta_geq2000->GetYaxis()->SetTitle("Counts");
   h_theta_geq2000->SetLineColor(kRed);
   h_theta_geq2000->SetLineWidth(2);
   
   /* Theta histogram with treshold on rms: all <2.5 */
   TH1F* h_theta_rms = new TH1F("","",1000,-80,50);
   h_theta_rms->SetStats(0);
   h_theta_rms->GetXaxis()->SetTitle("#theta (deg)");
   h_theta_rms->GetYaxis()->SetTitle("Counts");
   h_theta_rms->SetLineColor(kGreen+2);
   h_theta_rms->SetLineWidth(2);
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeTracks->GetEntry(i);
      histoTheta->Fill(theta_deg);
      
      /*if(cl_charge[0] >2000 && cl_charge[1] >2000 && cl_charge[2] >2000 && cl_charge[3] >2000 && cl_charge[4] >2000){
        h_theta_geq2000->Fill(theta_deg);
        }*/
      if(energySic>2000){
        h_theta_geq2000->Fill(theta_deg);
        }
        
      if(energySic>2000 && cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
        h_theta_rms->Fill(theta_deg);
      }
    }
   
   
   
   /* Visualisation Block*/
   TLine* theta_min = new TLine(-41.2,0,-41.2,100);
   theta_min->SetLineColor(kOrange+2);
   TLine* theta_max = new TLine(-38.788,0,-38.788,100);
   theta_max->SetLineColor(kOrange+2);
   theta_min->SetLineWidth(2);
   theta_max->SetLineWidth(2);
   
   TLine* theta_min1 = new TLine(-41.2,0,-41.2,6500);
   theta_min1->SetLineColor(kOrange+2);
   TLine* theta_max1 = new TLine(-38.788,0,-38.788,6500);
   theta_max1->SetLineColor(kOrange+2);
   
   TCanvas *c1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   c1->cd();
   c1->SetLogy();
   c1->SetFillColor(kWhite);
   histoTheta->Draw();
   h_theta_geq2000->Draw("same");
   h_theta_rms->Draw("same");
   theta_min->Draw("same");
   theta_max->Draw("same");
   TLegend* l = new TLegend();
   l->SetTextSize(0.035);
   l->AddEntry(histoTheta, "Counts(#theta)", "lp");
   l->AddEntry(h_theta_geq2000, "Counts(#theta,E_{SiC}>2000)","lp");
   l->AddEntry(h_theta_rms, "Counts(#theta, E_{SiC}>2000,rms<2.5)","lp");
   l->AddEntry(theta_min, "#theta_{calc} [-38.8;-41.2] (deg)","l");
   l->Draw("same");
   TLegend* l1 = new TLegend();
   l1->SetTextSize(0.035);
   l1->AddEntry(histoTheta, "Counts(#theta)", "lp");
   TLegend* l2 = new TLegend();
   l2->SetTextSize(0.035);
   l2->AddEntry(h_theta_geq2000, "Counts(#theta,E_{SiC}>2000)","lp");
   TLegend* l3 = new TLegend();
   l3->SetTextSize(0.035);
   l3->AddEntry(h_theta_rms, "Counts(#theta, E_{SiC}>2000,rms<2.5)","lp");
   l3->AddEntry(theta_min, "#theta_{calc} [-38.8;-41.2] (deg)","lp");
   
   /*TCanvas *c2 = new TCanvas("c2","alpha-division",1600,100,1000,600);
   c2->Divide(2,2);
   c2->cd(1);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   histoTheta->Draw();
   l1->Draw("same");
   c2->cd(2);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   h_theta_geq2000->Draw();
   l2->Draw("same");
   c2->cd(3);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   h_theta_rms->Draw();
   theta_min->Draw("same");
   theta_max->Draw("same");
   l3->Draw("same");
   c2->cd(4);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   histoTheta->Draw();
   h_theta_geq2000->Draw("same");
   h_theta_rms->Draw("same");
   theta_min1->Draw("same");
   theta_max1->Draw("same");
   l->Draw("same");*/
   
 }
