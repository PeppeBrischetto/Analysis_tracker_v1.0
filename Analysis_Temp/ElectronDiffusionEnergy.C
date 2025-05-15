#include <iostream>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TMath.h>

void ElectronDiffusionEnergy(double electricField, double pressure, double temperature, double energyLoss, double ionizationEnergy) {
    // Costanti fisiche
    const double kBoltzmann = 1.38e-23; // Costante di Boltzmann in J/K
    const double massElectron = 9.11e-31; // Massa dell'elettrone in kg
    const double referencePressure = 101325; // Pressione di riferimento (Pa)
    const double referenceTemperature = 293.15; // Temperatura di riferimento (K)
    const double timeStep = 1e-6; // Passo temporale (secondi)
    const double driftTime = 1e-3; // Tempo di drift totale (secondi)

    // Calcolo del numero di elettroni
    int nElectrons = static_cast<int>(energyLoss / ionizationEnergy);

    // Calcolo della velocità termica media degli elettroni (Maxwell-Boltzmann)
    double thermalVelocity = sqrt(2 * kBoltzmann * temperature / massElectron);

    // Calcolo della mobilità e del coefficiente di diffusione in base al modello di trasporto
    double mobility = (thermalVelocity / pressure) * (referencePressure / referenceTemperature);
    double diffusionCoefficient = (kBoltzmann * temperature / pressure) * mobility;

    // Istogrammi per la diffusione trasversale e longitudinale
    TH1F *transverseHist = new TH1F("transverseHist", "Diffusione Trasversale;Distanza Trasversale (mm);Conteggio", 100, -5, 5);
    TH1F *longitudinalHist = new TH1F("longitudinalHist", "Diffusione Longitudinale;Distanza Longitudinale (mm);Conteggio", 100, -5, 5);

    TRandom3 randomGen;

    for (int i = 0; i < nElectrons; i++) {
        // Posizioni iniziali
        double transversePosition = 0;
        double longitudinalPosition = 0;

        // Simulazione della diffusione nel tempo
        for (double t = 0; t < driftTime; t += timeStep) {
            double randomStepTransverse = randomGen.Gaus(0, diffusionCoefficient * sqrt(timeStep));
            double randomStepLongitudinal = randomGen.Gaus(0, diffusionCoefficient * sqrt(timeStep));
            double drift = mobility * electricField * timeStep;

            transversePosition += randomStepTransverse;
            longitudinalPosition += drift + randomStepLongitudinal;
        }

        // Riempimento degli istogrammi con le posizioni finali
        transverseHist->Fill(transversePosition);
        longitudinalHist->Fill(longitudinalPosition);
    }

    // Calcolo dei coefficienti di diffusione
    double sigmaTransverse = transverseHist->GetRMS();
    double sigmaLongitudinal = longitudinalHist->GetRMS();
    double transverseDiffusionCoefficient = 0.5 * TMath::Power(sigmaTransverse, 2) / driftTime;
    double longitudinalDiffusionCoefficient = 0.5 * TMath::Power(sigmaLongitudinal, 2) / driftTime;
    double totalDiffusionCoefficient = transverseDiffusionCoefficient + longitudinalDiffusionCoefficient;

    // Visualizzazione dei risultati
    TCanvas *canvas = new TCanvas("canvas", "Simulazione Diffusione Completa", 1200, 600);
    canvas->Divide(2, 1);
    canvas->cd(1);
    transverseHist->Draw();
    canvas->cd(2);
    longitudinalHist->Draw();

    // Stampare i risultati
    std::cout << "Numero di elettroni simulati: " << nElectrons << std::endl;
    std::cout << "Coefficiente di Diffusione Trasversale (D_T): " << transverseDiffusionCoefficient << " mm^2/s" << std::endl;
    std::cout << "Coefficiente di Diffusione