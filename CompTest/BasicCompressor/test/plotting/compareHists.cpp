#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <dirent.h>
#include <random>
using namespace std;

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "uti.h"
// #include "LabelHelper.h"

// g++ compareHists.cpp $(root-config --cflags --libs) -O2 -o "compareHists.exe"
// ./compareHists.exe -input "../tree/" -output pdfs/compressedFloat -bins 100

double getRandDouble(double minNum, double maxNum, double minBound=0, double maxBound=0) {
    std::uniform_real_distribution<double> unif(minNum,maxNum);
    std::default_random_engine re;
    double randomDouble = unif(re);
    
    if (minBound == 0 && maxBound == 0) {
        while (randomDouble < 0 || randomDouble > 1000) {
            randomDouble = unif(re);
        }
    } else if (randomDouble < minBound) {
        randomDouble += 2*M_PI;
    } else if (randomDouble > maxBound) {
        randomDouble -= 2*M_PI;
    }

    return randomDouble;
}

void addHistToStack(TH1D* h, TLegend* legend, THStack* hStack, int colorIdx, string legendLabel, bool addLegend=false) {
    h->GetYaxis()->SetTitleOffset(1.6);
    h->Sumw2();
    h->Scale(1./h->Integral());
    h->SetMarkerStyle(20);
    h->SetMarkerColor(colorIdx);
    h->SetLineColor(colorIdx);
    h->SetStats(0);

    if (addLegend) legend->AddEntry(h, legendLabel.c_str(), "p"); // we only need to add this once

    hStack->Add(h);
}

int main(int argc, char *argv[])
{
    SetThesisStyle();

    CommandLine CL(argc, argv);

    vector<string> InputFileFolders = CL.GetStringVector("input");
    string Output = CL.Get("output");
    int bins = CL.GetInt("bins");

    PdfFileHelper PdfFile(Output + "_" + to_string(bins) + "bins.pdf");

    vector<string> lines;
    lines.push_back("CMSSW Producer/Analyzer Results on AOD data:");
    lines.push_back("");
    lines.push_back("file:/eos/cms/store/group/phys_heavyions/chenyi/Samples/");
    lines.push_back("store/himc/HINPbPbAutumn18DR/");
    lines.push_back("DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/AODSIM/");
    lines.push_back("mva98_103X_upgrade2018_realistic_HI_v11-v1/40000/");
    
    PdfFile.AddTextPage(lines, 0.1, 0.9, 0.03);

    THStack* hEnergyStack = new THStack("hEnergyStack","Jet Energy;Jet Energy;1/N d energy");
    THStack* hPxStack = new THStack("hPxStack","Px;Px;1/N dPx");
    THStack* hPyStack = new THStack("hPyStack","Py;Py;1/N dPy");
    THStack* hPzStack = new THStack("hPzStack","Pz;Pz;1/N dPz");

    TLegend* legend = new TLegend(0.657,0.8,0.857,0.9);

    TH1D* hOriginal[4];
    TH1D* hPerturbed[4];

    bool original = true;
    
    // for(string folder : InputFileFolders)
    for (int folderIdx=0; folderIdx < InputFileFolders.size(); folderIdx++)
    {
        string folder = InputFileFolders[folderIdx];
        struct dirent *de; // to get files in path
        DIR *dr = opendir(folder.c_str()); 
        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            cout << "Could not open directory: " << folder << endl;
            return 0;
        }
        
        TH1D* HJetEnergy = new TH1D("HJetEnergy", "Jet Energy;Energy;1/N dEnergy", bins, 0, 400);
        TH1D* HJetPx = new TH1D("HJetPx", "Jet Px;Px;1/N dPx", bins, -100, 150);
        TH1D* HJetPy = new TH1D("HJetPy", "Jet Py;Py;1/N dPy", bins, -400, 400);
        TH1D* HJetPz = new TH1D("HJetPz", "Jet Pz;Pz;1/N dPz", bins, -160, 160);
        
        int fileCount = 0; // REMOVE
        while ((de = readdir(dr)) != NULL) {
            if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ) continue;
            if (fileCount >= 5) continue; // REMOVE
            string fileName = de->d_name;
            cout << fileName << endl;
            TFile File((folder + "/" + fileName).c_str());

            TTree *Tree = (TTree *)File.Get("BasicCompressorDump/OriginalTree");
            if (!original) {
                Tree = (TTree *)File.Get("BasicCompressorDump/CompressedTree");
            }

            if(Tree == nullptr)
            {
                File.Close();
                continue;
            }

            Double_t energy;
            Double_t px;
            Double_t py;
            Double_t pz;

            Tree->SetBranchAddress("energy", &energy);
            Tree->SetBranchAddress("px", &px);
            Tree->SetBranchAddress("py", &py);
            Tree->SetBranchAddress("pz", &pz);

            int EntryCount = Tree->GetEntries();

            cout << "Entries: " << EntryCount << endl;
            for(int iE = 0; iE < EntryCount; iE++)
            {
                Tree->GetEntry(iE);
                HJetEnergy->Fill(energy);
                HJetPx->Fill(px);
                HJetPy->Fill(py);
                HJetPz->Fill(pz);
            }
            File.Close();
        }

        addHistToStack(HJetEnergy, legend, hEnergyStack, folderIdx+1+((bool)(!original)), (folderIdx == 0 && original ? "Original" : "Perturbed"), true);
        
        addHistToStack(HJetPx, legend, hPxStack, folderIdx+1+((bool)(!original)), "");
        addHistToStack(HJetPy, legend, hPyStack, folderIdx+1+((bool)(!original)), "");
        addHistToStack(HJetPz, legend, hPzStack, folderIdx+1+((bool)(!original)), "");
        

        if (folderIdx == 0 && original) {
            hOriginal[0] = HJetEnergy;
            hOriginal[1] = HJetPx;
            hOriginal[2] = HJetPy;
            hOriginal[3] = HJetPz;
        } else {
            hPerturbed[0] = HJetEnergy;
            hPerturbed[1] = HJetPx;
            hPerturbed[2] = HJetPy;
            hPerturbed[3] = HJetPz;
        }

        // if only one input folder, then go back and perturb it
        if (InputFileFolders.size() == 1 && original) {
            folderIdx -= 1;
            original = false;
        }
   }

    // THStack* hStacks[1] = {hEnergyStack};
    THStack* hStacks[4] = {hEnergyStack, hPxStack, hPyStack, hPzStack};
    // THStack* hStacks[3] = {hPTStack, hEtaStack, hPhiStack};
    // THStack* hStacks[4] = {hNStack, hPTStack, hNLeadingStack, hPTTrackStack};
    for (int i = 0; i < 4; i ++ ){
        TCanvas* c = new TCanvas("c","",1200,900);

        hStacks[i]->Draw("nostackb");

        // KS test
        double KSProb = hOriginal[i]->KolmogorovTest(hPerturbed[i]);
        cout << i << ": " << KSProb << endl;

        // draw text
        TLatex *text = new TLatex();
        text->SetTextSize(0.025);
        string latexText = "KS test prob: " + to_string(KSProb);
        // string latexText = "#splitline{p_{T}^{track} > 1GeV, p_T^{jet} > 0GeV}{"+dataLabel+"}";
        // text->DrawLatexNDC(0.33, 0.15, latexText.c_str());
        text->DrawLatexNDC(0.6, 0.75, latexText.c_str());

        legend->SetTextSize(0.028);
        legend->SetFillColor(0);
        legend->SetBorderSize(1);
        legend->Draw("");
        if (i == 0) c->SetLogy();
        // c->SetLogy();
        c->Update();
        // c->SaveAs((TString) ("test.jpg"));
        PdfFile.AddCanvas(c);
    }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




