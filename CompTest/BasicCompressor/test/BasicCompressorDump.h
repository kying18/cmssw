
#ifndef CompTest_BasicCompressorDump_h
#define CompTest_BasicCompressorDump_h

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/Math/interface/libminifloat.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include <memory>
#include <iostream>

//ROOT inclusion
#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TMath.h"
#include "TList.h"
#include "TString.h"

class BasicCompressorDump : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit BasicCompressorDump(const edm::ParameterSet& );
  ~BasicCompressorDump() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void analyze(const edm::Event& , const edm::EventSetup& ) override;

  edm::InputTag inputPFJets;
  edm::EDGetTokenT<reco::PFJetCollection> jetsToken;
  edm::InputTag inputPFJetsCompressed;
  edm::EDGetTokenT<reco::PFJetCollection> compressedJetsToken;

  TTree* outTree;
  TTree* outTreeCompressed;
  edm::Service<TFileService> fs;

  edm::EventNumber_t eventN;
  Double_t jetEnergy;
  Double_t px;
  Double_t py;
  Double_t pz;
  
  edm::EventNumber_t eventNCompressed;
  Double_t jetEnergyCompressed;
  Double_t pxCompressed;
  Double_t pyCompressed;
  Double_t pzCompressed;
};
#endif
