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

void C_plot_phi(int run)
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
      sprintf(fileInName, "../Tracks/tracks_run00%itenta.root", run);
   }else if(run <100){
      sprintf(fileInName, "../Tracks/tracks_run0%itenta.root", run);
   }else{
      sprintf(fileInName, "../Tracks/tracks_run%itenta.root", run);
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
     
   TH1F *histoPhi=new TH1F("","",1000,-160,160);
   histoPhi->SetStats(0);
   histoPhi->GetXaxis()->SetTitle("#phi (deg)");
   histoPhi->GetYaxis()->SetTitle("Counts");
   
   /* phi histogram with 1st threshold on charge: 2000*/
   TH1F* h_phi_geq2000 = new TH1F("","",1000,-160,160);
   h_phi_geq2000->SetStats(0);
   h_phi_geq2000->GetXaxis()->SetTitle("#phi (deg)");
   h_phi_geq2000->GetYaxis()->SetTitle("Counts");
   h_phi_geq2000->SetLineColor(kRed);
   
   /* phi histogram with treshold on rms: all <2.5 */
   TH1F* h_phi_rms = new TH1F("","",1000,-160,160);
   h_phi_rms->SetStats(0);
   h_phi_rms->GetXaxis()->SetTitle("#phi (deg)");
   h_phi_rms->GetYaxis()->SetTitle("Counts");
   h_phi_rms->SetLineColor(kGreen+2);
   
   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeTracks->GetEntry(i);
      histoPhi->Fill(phi_deg);
      
      /*if(cl_charge[0] >2000 && cl_charge[1] >2000 && cl_charge[2] >2000 && cl_charge[3] >2000 && cl_charge[4] >2000){
        h_phi_geq2000->Fill(phi_deg);
        }*/
      if(energySic>2000){
        h_phi_geq2000->Fill(phi_deg);
        }
        
      if(energySic>2000 && cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
        h_phi_rms->Fill(phi_deg);
      }
    }
   
   
   
   /* Visualisation Block*/
   TLine* phi_min = new TLine(-1.75,0,-1.75,135);
   phi_min->SetLineColor(kOrange+2);
   TLine* phi_max = new TLine(1.75,0,1.75,135);
   phi_max->SetLineColor(kOrange+2);
   
   TLine* phi_min1 = new TLine(-1.75,0,-1.75,13380);
   phi_min1->SetLineColor(kOrange+2);
   TLine* phi_max1 = new TLine(1.75,0,1.75,13380);
   phi_max1->SetLineColor(kOrange+2);
   
   TCanvas *c1=new TCanvas("c1","alpha",1600, 100,1000.,600.);
   c1->cd();
   c1->SetLogy();
   c1->SetFillColor(kWhite);
   histoPhi->Draw();
   h_phi_geq2000->Draw("same");
   h_phi_rms->Draw("same");
   TLegend* l = new TLegend();
   l->SetTextSize(0.035);
   l->AddEntry(histoPhi, "Counts(#phi)", "lep");
   l->AddEntry(h_phi_geq2000, "Counts(#phi,E_{SiC}>2000)","lep");
   l->AddEntry(h_phi_rms, "Counts(#phi, E_{SiC}>2000,rms<2.5)","lep");
   l->AddEntry(phi_min, "#phi_{theo}-calc - [-1.75;1.75] (deg)","lep");
   l->Draw("same");
   TLegend* l1 = new TLegend();
   l1->SetTextSize(0.035);
   l1->AddEntry(histoPhi, "Counts(#phi)", "lep");
   TLegend* l2 = new TLegend();
   l2->SetTextSize(0.035);
   l2->AddEntry(h_phi_geq2000, "Counts(#phi,E_{SiC}>2000)","lep");
   TLegend* l3 = new TLegend();
   l3->SetTextSize(0.035);
   l3->AddEntry(h_phi_rms, "Counts(#phi, E_{SiC}>2000,rms<2.5)","lep");
   l3->AddEntry(phi_min, "#phi_{theo}-calc - [-1.75;1.75] (deg)","lep");
   
   TCanvas *c2 = new TCanvas("c2","alpha-division",1600,100,1000,600);
   c2->Divide(2,2);
   c2->cd(1);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   histoPhi->Draw();
   l1->Draw("same");
   c2->cd(2);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   h_phi_geq2000->Draw();
   l2->Draw("same");
   c2->cd(3);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   h_phi_rms->Draw();
   phi_min->Draw("same");
   phi_max->Draw("same");
   l3->Draw("same");
   c2->cd(4);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   histoPhi->Draw();
   h_phi_geq2000->Draw("same");
   h_phi_rms->Draw("same");
   phi_min1->Draw("same");
   phi_max1->Draw("same");
   l->Draw("same");
   
 }
