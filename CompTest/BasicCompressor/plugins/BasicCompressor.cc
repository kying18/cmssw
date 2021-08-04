// -*- C++ -*-
//
// Package:    CompTest/BasicCompressor
// Class:      BasicCompressor
//
/**\class BasicCompressor BasicCompressor.cc CompTest/BasicCompressor/plugins/BasicCompressor.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Yueyang Ying
//         Created:  Mon, 26 Jul 2021 12:13:37 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/Math/interface/libminifloat.h"
// #include "DataFormats/CompLorentzVec/interface/LorentzVec.h"
// #include "DataFormats/CompLorentzVec/interface/CompLorentzVec.h"
#include "DataFormats/CompLorentzVec/interface/ApproxPFJet.h"

// write a producer that read PFJets produced in AOD, then pass it to this method that reduces the 
// resolution and then write it down as PFJet reduced and simply compare the size

//
// class declaration
//

class BasicCompressor : public edm::stream::EDProducer<> {
public:
  explicit BasicCompressor(const edm::ParameterSet&);
  ~BasicCompressor();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  // ----------member data ---------------------------
  edm::InputTag inputPFJets;
  edm::EDGetTokenT<reco::PFJetCollection> jetsToken;
};

BasicCompressor::BasicCompressor(const edm::ParameterSet& iConfig)
{
  //register your products
  inputPFJets = iConfig.getParameter<edm::InputTag>("inputPFJets");
  jetsToken = consumes<reco::PFJetCollection>(inputPFJets);
  produces<std::vector<ApproxPFJet>>();
  produces<reco::PFJetCollection>();
}

BasicCompressor::~BasicCompressor() {

}

double getRandomNumber(double low, double high) {
  return low + rand() / (RAND_MAX / (high - low));
}

void BasicCompressor::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace reco;

  auto resultOrig = std::make_unique<reco::PFJetCollection>();
  auto result = std::make_unique<std::vector<ApproxPFJet>>();
  const auto& jetsCollection = iEvent.get(jetsToken);

  for (const auto& jet : jetsCollection) {
    // do some compressing
    // reco::PFJet* newJet = jet.clone();

    // using new approxPFJet structure
    std::vector<float> p4 = {(float)jet.px(), (float)jet.py(), (float)jet.pz(), (float)jet.energy()};
    std::vector<float> vertex = {(float)jet.vx(), (float)jet.vy(), (float)jet.vz()};
    ApproxPFJet::Specific specificApprox;
    PFJet::Specific specific;

    ApproxPFJet newJet(p4, vertex, specificApprox);
    reco::PFJet newJetOrig(reco::Particle::LorentzVector(jet.px(), jet.py(), jet.pz(), jet.energy()), math::XYZPoint(jet.vx(), jet.vy(), jet.vz()), specific);

    result->push_back(newJet);
    resultOrig->push_back(newJetOrig);
  }

  iEvent.put(std::move(result));
  iEvent.put(std::move(resultOrig));
}

void BasicCompressor::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void BasicCompressor::endStream() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void BasicCompressor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  // desc.setUnknown();
  // descriptions.addDefault(desc);

  desc.add<edm::InputTag>("inputPFJets", edm::InputTag("kt4PFJetsForRho"));

  descriptions.add("BasicCompressor", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BasicCompressor);
