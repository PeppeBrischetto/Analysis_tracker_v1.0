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

void C_plot_clchargeE2000RMS2e5(int run)
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
   Double_t somma_R;
   Double_t somma_S;
   Double_t somma_best;
  
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
   TF1* gaus = new TF1("gaus","gaus",0.,70000);
   gaus->SetParameters(0.,0.,0.);  
   vector<TH1F*> h_clcharge_1;
   
   TH1F *totalCharge_rows1 = new TH1F("","",200,0,350000);
   totalCharge_rows1->SetTitle("#Sigma_{row=0}^{4} Histo_{row} - E_{SiC}>2000 #wedge RMS<2.5");
   totalCharge_rows1->GetXaxis()->SetTitle("Total charge (a.u.)");
   totalCharge_rows1->GetYaxis()->SetTitle("Counts");
   
   TH1F *totalCharge_strips1 = new TH1F("","",200,0,350000);
   totalCharge_strips1->SetTitle("#Sigma_{row=5}^{10} Histo_{row} - E_{SiC}>2000 #wedge RMS<2.5");
   totalCharge_strips1->GetXaxis()->SetTitle("Total charge (a.u.)");
   totalCharge_strips1->GetYaxis()->SetTitle("Counts");
   
   TH1F *totalCharge = new TH1F("","",200,0,350000);
   totalCharge->SetTitle("#Sigma_{row=0}^{10} Histo_{row} - E_{SiC}>2000 #wedge RMS<2.5");
   totalCharge->GetXaxis()->SetTitle("Total charge (a.u.)");
   totalCharge->GetYaxis()->SetTitle("Counts");
   
   TH1F *totalCharge_best = new TH1F("","",200,0,350000);
   totalCharge_best->SetTitle("#Sigma_{row=0,row#neq 5,10}^{10} Histo_{row} - E_{SiC}>2000 #wedge RMS<2.5");
   totalCharge_best->GetXaxis()->SetTitle("Total charge (a.u.)");
   totalCharge_best->GetYaxis()->SetTitle("Counts");
   
   /*format vector<TH1F*>* */
   for(Int_t r=0; r<11; r++){
      TString name1;
      name1.Form("Row.%d",r);
      TString title1;
      title1.Form("Row.%d, E_{SiC}>2000 #wedge RMS<2.5",r);
      h_clcharge_1.push_back(new TH1F(name1,title1,200,0,70000));
   }
   
   for(Int_t i=0; i<entries; i++){
      somma_R = 0.;
      somma_S = 0.;
      somma_best = 0.;
      treeTracks->GetEntry(i);
      if(energySic>2000 && cl_x_rms[0] < 2.5 && cl_x_rms[1] < 2.5 && cl_x_rms[3] < 2.5 && cl_x_rms[4] < 2.5){
        for(Int_t j=0; j<11;j++){
           h_clcharge_1.at(j)->Fill(cl_charge[j]);
           h_clcharge_1.at(j)->GetXaxis()->SetTitle("Charge (a.u.)");
           h_clcharge_1.at(j)->GetYaxis()->SetTitle("Counts");
           if(j<5){
             somma_R += cl_charge[j];
             //cout << "somma_R: " << somma_R << endl;
           }
           else{
             somma_S += cl_charge[j];
             //cout << "somma_S: " << somma_S << endl;
           }
           if(j!=5 && j!=10){
             somma_best += cl_charge[j];
           }
        }
      totalCharge_rows1->Fill(somma_R);
      totalCharge_strips1->Fill(somma_S);
      totalCharge->Fill(somma_R + somma_S);
      totalCharge_best->Fill(somma_best);
      }
    }
   
   /* Gaussian fitting for resolution evaluation */
   /*Double_t mean[11] = {0};
   Double_t sigma[11] = {0};
   Double_t resolution[11] = {0};
   for(Int_t t=0;t<11;t++){
      h_clcharge_1.at(t)->Fit("gaus","","",2000,70000);
      mean[t] = gaus->GetParameter(1);
      sigma[t] = gaus->GetParameter(2);
      resolution[t] = (2.35*sigma[t])/(mean[t]);
      
   }
   
   Double_t resTot[4] = {0.};
   totalCharge_rows1->Fit("gaus","","",70000,260000);
   resTot[0] = (2.35 * gaus->GetParameter(2))/(gaus->GetParameter(1));
   totalCharge_strips1->Fit("gaus","","",70000,260000);
   resTot[1] = (2.35 * gaus->GetParameter(2))/(gaus->GetParameter(1));
   totalCharge->Fit("gaus","","",160000,400000);
   resTot[2] = (2.35 * gaus->GetParameter(2))/(gaus->GetParameter(1));
   totalCharge_best->Fit("gaus","","",160000,400000);
   resTot[3] = (2.35 * gaus->GetParameter(2))/(gaus->GetParameter(1));

   for(Int_t r=0;r<11;r++){
      cout << "Resolution Row." << r << ": " << resolution[r] << endl;
   }
   
   cout << "Resolution total charge: " << resTot[0] << "    Resolution total strips: " << resTot[1] << "    Resolution total strips: " << resTot[2] << "    Resolution total strips: " << resTot[3] << endl;*/
   
   /* Visualisation Block */
   TCanvas *c2 = new TCanvas("c2","Row charge distribution",1600, 100,1000.,600.);
   c2->Divide(3,2);
   c2->cd(1);
   h_clcharge_1.at(0)->Draw();
   c2->cd(2);
   h_clcharge_1.at(1)->Draw();
   c2->cd(3);
   h_clcharge_1.at(2)->Draw();
   c2->cd(4);
   h_clcharge_1.at(3)->Draw();
   c2->cd(5);
   h_clcharge_1.at(4)->Draw();
   c2->cd(6);
   totalCharge_rows1->Draw();
   
   TCanvas *c3 = new TCanvas("c3","Row charge distribution",1600, 100,1000.,600.);
   c3->Divide(3,3);
   c3->cd(1);
   h_clcharge_1.at(5)->Draw();
   c3->cd(2);
   h_clcharge_1.at(6)->Draw();
   c3->cd(3);
   h_clcharge_1.at(7)->Draw();
   c3->cd(4);
   h_clcharge_1.at(8)->Draw();
   c3->cd(5);
   h_clcharge_1.at(9)->Draw();
   c3->cd(6);
   h_clcharge_1.at(10)->Draw();
   c3->cd(7);
   totalCharge_strips1->Draw();
   c3->cd(8);
   totalCharge->Draw();
   c3->cd(9);
   totalCharge_best->Draw();
   
   
 }
