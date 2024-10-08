//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created 7 Jun 2024 by A. Pitronaci, based on C_plot_phi.C available in the present directory
//#######################################
//#   updated: 10 June 2024, extended for no-segmeneted rows, i.e. 5,6,7,8,9,10 (strips)
//# 
//###################################################################################################

void C_plot_phi_v2(int run)
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

   Int_t cl_padMult0,cl_padMult1,cl_padMult2,cl_padMult3,cl_padMult4;
   Int_t a_pads_fired0[100],a_pads_fired1[100],a_pads_fired2[100],a_pads_fired3[100],a_pads_fired4[100]; 
  
   Int_t flagLastPad=0;
  
   int sicCounts=0;		// counter that return the tracks with a SiC
  
   /* Reading block variables */  
   char fileInName[50];
   if(run<10){
      sprintf(fileInName, "../Tracks/tracks_run00%i.root", run);
   }else if(run <100){
      sprintf(fileInName, "../Tracks/tracks_run0%i.root", run);
   }else{
      sprintf(fileInName, "../Tracks/tracks_run%i.root", run);
   } 
   TFile *fileIn = new TFile(fileInName);
   TTree *treeTracks = (TTree*)fileIn->Get("Data_R");
   
   /* Setting tree-Branches */
   treeTracks->SetBranchAddress("cl_x", &cl_x);
   treeTracks->SetBranchAddress("cl_x_mm", &cl_x_mm);
   treeTracks->SetBranchAddress("cl_y", &cl_y);
   treeTracks->SetBranchAddress("cl_y_mm", &cl_y_mm);
   treeTracks->SetBranchAddress("cl_x_rms", &cl_x_rms);
   //treeTracks->SetBranchAddress("cl_padMult", &cl_padMult);
   treeTracks->SetBranchAddress("cl_padMult0",&cl_padMult0);
   treeTracks->SetBranchAddress("cl_padMult1",&cl_padMult1);
   treeTracks->SetBranchAddress("cl_padMult2",&cl_padMult2);
   treeTracks->SetBranchAddress("cl_padMult3",&cl_padMult3);
   treeTracks->SetBranchAddress("cl_padMult4",&cl_padMult4);
   treeTracks->SetBranchAddress("pads_fired0",a_pads_fired0);
   treeTracks->SetBranchAddress("pads_fired1",a_pads_fired1);
   treeTracks->SetBranchAddress("pads_fired2",a_pads_fired2);
   treeTracks->SetBranchAddress("pads_fired3",a_pads_fired3);
   treeTracks->SetBranchAddress("pads_fired4",a_pads_fired4);   
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
     
   TH1F *histophi=new TH1F("","",250,-50,80);
   histophi->SetStats(0);
   histophi->GetXaxis()->SetTitle("#phi (deg)");
   histophi->GetYaxis()->SetTitle("Counts");
   histophi->GetYaxis()->SetRangeUser(0.1,5000.);
   histophi->SetLineColor(kBlue-3);
   histophi->SetLineWidth(2);
   
   /* phi histogram with 1st threshold on charge: 2000*/
   TH1F* h_phi_geq2000 = new TH1F("","",250,-50,80);
   h_phi_geq2000->SetStats(0);
   h_phi_geq2000->GetXaxis()->SetTitle("#phi (deg)");
   h_phi_geq2000->GetYaxis()->SetTitle("Counts");
   h_phi_geq2000->SetLineColor(kRed);
   h_phi_geq2000->SetLineWidth(2);   
   
   /* phi histogram with treshold on rms: all <2.5 */
   TH1F* h_phi_rms = new TH1F("","",1000,-50,80);
   h_phi_rms->SetStats(0);
   h_phi_rms->GetXaxis()->SetTitle("#phi (deg)");
   h_phi_rms->GetYaxis()->SetTitle("Counts");
   h_phi_rms->SetLineColor(kGreen+2);
   

   for(int i=0; i<entries; i++){
   //for(int i=0; i<50; i++){
      treeTracks->GetEntry(i);
      histophi->Fill(phi_deg);
      if (cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[2] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5) {
          for (int j=0; j<cl_padMult4; j++) {
              if ( (a_pads_fired4[j] == 0) || (a_pads_fired4[j] == 1) || (a_pads_fired4[j] == 2))
                 flagLastPad = 1;       
          }
         
          if (flagLastPad != 1)
             histophi->Fill(phi_deg);

          flagLastPad=0;

           if(cl_charge[0] >2000 && cl_charge[1] >2000 && cl_charge[2] >2000 && cl_charge[3] >2000 && cl_charge[4] >2000){
             h_phi_geq2000->Fill(phi_deg);
           }
           if(energySic>2000){
             h_phi_geq2000->Fill(phi_deg);
             sicCounts++;
           }
        
           if(energySic>2000 && cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
             h_phi_rms->Fill(phi_deg);
             sicCounts++;
           }
      }
   }
   
   
   
   /* Visualisation Block*/
   TLine* phi_min = new TLine(-1.75,0,-1.75,5000);
   phi_min->SetLineColor(kOrange+2);
   phi_min->SetLineWidth(2);
   phi_min->SetLineStyle(9);
   TLine* phi_max = new TLine(1.75,0,1.75,5000);
   phi_max->SetLineColor(kOrange+2);
   phi_max->SetLineWidth(2);   
   phi_max->SetLineStyle(9);
   
   TArrow *arrow = new TArrow(-9.1,2800,-2.1,2800,0.02,"|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(2);
   arrow->SetAngle(40);
   
   TArrow *arrow2 = new TArrow(1.9,2800,8.9,2800,0.02,"<|");
   arrow2->SetFillColor(1);
   arrow2->SetFillStyle(1001);
   arrow2->SetLineWidth(2);
   arrow2->SetAngle(40);
   
   TLatex *latex = new TLatex(-36., 2450, "SiC expected #phi range");
   latex->SetTextSize(0.032);
   
   TLine* phi_min1 = new TLine(-41.2,0,-41.2,6500);
   phi_min1->SetLineColor(kOrange+2);
   TLine* phi_max1 = new TLine(-38.788,0,-38.788,6500);
   phi_max1->SetLineColor(kOrange+2);
   
   TCanvas *c1=new TCanvas("c1","alpha",1000, 900);
   c1->cd();
   c1->SetLogy();
   c1->SetFillStyle(0);
   histophi->Draw();
   h_phi_geq2000->Draw("same");
   h_phi_rms->Draw("same");
   phi_min->Draw("same");
   phi_max->Draw("same");
   arrow->Draw("same");
   arrow2->Draw("same");
   latex->Draw("same");
   
   TLegend* l = new TLegend(0.68,0.68,0.92,0.78);
   l->SetTextSize(0.033);
   l->SetLineWidth(0);
   l->AddEntry(histophi, "Total", "lp");
   l->AddEntry(h_phi_geq2000, "SiC coincidence","lp");
   l->AddEntry(h_phi_rms, "Counts(#phi, E_{SiC}>2000,rms<2.5)","lep");
   l->AddEntry(phi_min, "#phi_{calc} [-38.8;-41.2] (deg)","lep");
   l->Draw("same");
   TLegend* l1 = new TLegend();
   l1->SetTextSize(0.035);
   l1->AddEntry(histophi, "Counts(#phi)", "lep");
   TLegend* l2 = new TLegend();
   l2->SetTextSize(0.035);
   l2->AddEntry(h_phi_geq2000, "Counts(#phi,E_{SiC}>2000)","lep");
   TLegend* l3 = new TLegend();
   l3->SetTextSize(0.035);
   l3->AddEntry(h_phi_rms, "Counts(#phi, E_{SiC}>2000,rms<2.5)","lep");
   l3->AddEntry(phi_min, "#phi_{calc} [-38.8;-41.2] (deg)","lep");
   
   TCanvas *c2 = new TCanvas("c2","alpha-division",1600,100,1000,600);
   c2->Divide(2,2);
   c2->cd(1);
   c2->SetLogy();
   c2->SetFillColor(kWhite);
   histophi->Draw();
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
   histophi->Draw();
   h_phi_geq2000->Draw("same");
   h_phi_rms->Draw("same");
   phi_min1->Draw("same");
   phi_max1->Draw("same");
   l->Draw("same");
   cout<<sicCounts<<endl;
   
 }
