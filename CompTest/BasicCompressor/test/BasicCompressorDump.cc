#include "CompTest/BasicCompressor/test/BasicCompressorDump.h"

BasicCompressorDump::BasicCompressorDump(const edm::ParameterSet& iConfig) {
  inputPFJets = iConfig.getParameter<edm::InputTag>("inputPFJets");
  jetsToken = consumes<reco::PFJetCollection>(inputPFJets);
  inputPFJetsCompressed = iConfig.getParameter<edm::InputTag>("inputPFJetsCompressed");
  compressedJetsToken = consumes<reco::PFJetCollection>(inputPFJetsCompressed);
  
  usesResource("TFileService");

  outTree = fs->make<TTree>("OriginalTree", "OriginalTree");
  outTree->Branch("event", &eventN, "event/i");
  outTree->Branch("energy", &jetEnergy, "energy/D"); // D: Double_t, d: Double32_t, F: Float_t, f: Float16_t
  outTree->Branch("px", &px, "px/D");
  outTree->Branch("py", &py, "py/D");
  outTree->Branch("pz", &pz, "pz/D");

  outTreeCompressed = fs->make<TTree>("CompressedTree", "CompressedTree");
  outTreeCompressed->Branch("event", &eventNCompressed, "event/i");
  outTreeCompressed->Branch("energy", &jetEnergyCompressed, "energy/D"); // D: Double_t, d: Double32_t, F: Float_t, f: Float16_t
  outTreeCompressed->Branch("px", &pxCompressed, "px/D");
  outTreeCompressed->Branch("py", &pyCompressed, "py/D");
  outTreeCompressed->Branch("pz", &pzCompressed, "pz/D");
}

BasicCompressorDump::~BasicCompressorDump() {}

void BasicCompressorDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<reco::PFJetCollection> jetsCollection = iEvent.getHandle(jetsToken);
  edm::Handle<reco::PFJetCollection> compressedJetsCollection = iEvent.getHandle(compressedJetsToken);

  for (const auto& jet : *jetsCollection) {
    eventN = iEvent.id().event();
    jetEnergy = jet.energy();
    px = jet.px();
    py = jet.py();
    pz = jet.pz();
    outTree->Fill();
  }
  for (const auto& jet : *compressedJetsCollection) {
    jetEnergyCompressed = jet.energy();
    pxCompressed = jet.px();
    pyCompressed = jet.py();
    pzCompressed = jet.pz();
    outTreeCompressed->Fill();
  }
}

void BasicCompressorDump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
//   desc.setUnknown();
  desc.add<edm::InputTag>("inputPFJets", edm::InputTag("kt4PFJetsForRho"));
  desc.add<edm::InputTag>("inputPFJetsCompressed", edm::InputTag("BasicCompressor"));
  descriptions.add("BasicCompressorDump", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BasicCompressorDump);