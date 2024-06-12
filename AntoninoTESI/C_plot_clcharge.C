//###################################################################################################
//#   plot charge spectra of a single SiC
//#      
//#   required as argument the run number
//#
//###################################################################################################
//#   created june 2024 by A. Pitronaci, based on C_plot_theta.C available in the present directory
//#######################################
//#   updated: june 2024
//# 
//###################################################################################################

void C_plot_clcharge(int run)
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
   Double_t somma_R = 0.;
   Double_t somma_S = 0.;
  
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
     
   vector<TH1F*> h_clcharge;
   
   TH1F *totalCharge_rows = new TH1F("","",1000,0,70000);
   totalCharge_rows->SetTitle("#Sigma_{row=0}^{4} Histo_{row}");
   totalCharge_rows->GetXaxis()->SetTitle("Total charge");
   totalCharge_rows->GetYaxis()->SetTitle("Counts");
   
   TH1F *totalCharge_strips = new TH1F("","",1000,0,70000);
   totalCharge_strips->SetTitle("#Sigma_{row=5}^{10} Histo_{row}");
   totalCharge_strips->GetXaxis()->SetTitle("Total charge");
   totalCharge_strips->GetYaxis()->SetTitle("Counts");
   
   /*format vector<TH1F*>* */
   for(Int_t r=0; r<11; r++){
      TString name;
      name.Form("Row.%d",r);
      TString title;
      title.Form("Row.%d",r);
      h_clcharge.push_back(new TH1F(name,title,1000,0,70000));
   }
   
   for(Int_t i=0; i<entries; i++){
      somma_R = 0.;
      somma_S = 0.;
      treeTracks->GetEntry(i);
      for(Int_t j=0; j<11;j++){
         h_clcharge.at(j)->Fill(cl_charge[j]);
         if(j<5){
           somma_R += cl_charge[j];
           //cout << "somma_R: " << somma_R << endl;
           totalCharge_rows->Fill(somma_R);
         }
         else{
           somma_S += cl_charge[j];
           //cout << "somma_S: " << somma_S << endl;
           totalCharge_strips->Fill(somma_S);
         }
      }
      somma_R = 0.;
      somma_S = 0.;
   }
   
   /* Visualisation Block*/
   TCanvas *c = new TCanvas("c","Row charge distribution",1600, 100,1000.,600.);
   c->Divide(3,2);
   c->cd(1);
   h_clcharge.at(0)->Draw();
   c->cd(2);
   h_clcharge.at(1)->Draw();
   c->cd(3);
   h_clcharge.at(2)->Draw();
   c->cd(4);
   h_clcharge.at(3)->Draw();
   c->cd(5);
   h_clcharge.at(4)->Draw();
   c->cd(6);
   totalCharge_rows->Draw();
   
   TCanvas *c1 = new TCanvas("c1","Row charge distribution",1600, 100,1000.,600.);
   c1->Divide(3,3);
   c1->cd(1);
   h_clcharge.at(5)->Draw();
   c1->cd(2);
   h_clcharge.at(6)->Draw();
   c1->cd(3);
   h_clcharge.at(7)->Draw();
   c1->cd(4);
   h_clcharge.at(8)->Draw();
   c1->cd(5);
   h_clcharge.at(9)->Draw();
   c1->cd(6);
   h_clcharge.at(10)->Draw();
   c1->cd(7);
   totalCharge_strips->Draw();
   
 }
