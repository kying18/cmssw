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

// edm event size 
// recoPFJets_BasicCompressor__compressJets. 62072 42656
// recoPFJets_kt4PFJetsForRho__RECO. 60560 41250
// without putting result in event:
// recoPFJets_BasicCompressor__compressJets. 35560 35560


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
#include "DataFormats/CompLorentzVec/interface/LorentzVec.h"
#include "DataFormats/CompLorentzVec/interface/CompLorentzVec.h"

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

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  edm::InputTag inputPFJets;
  edm::EDGetTokenT<reco::PFJetCollection> jetsToken;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
BasicCompressor::BasicCompressor(const edm::ParameterSet& iConfig)
{
  //register your products
  inputPFJets = iConfig.getParameter<edm::InputTag>("inputPFJets");
  jetsToken = consumes<reco::PFJetCollection>(inputPFJets);
  // produces<reco::PFJetCollection>();
  produces<std::vector<LorentzVec>>();
  produces<std::vector<CompLorentzVec>>();

  // TODO figure out what exactly the proper data type is.. det set vec?? vector?
  // clusterToken = consumes<edmNew::DetSetVector<SiStripApproximateCluster>>(inputApproxClusters);
  // produces<edmNew::DetSetVector<SiStripApproximateCluster>>();
}

BasicCompressor::~BasicCompressor() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called to produce the data  ------------
double getRandomNumber(double low, double high) {
  return low + rand() / (RAND_MAX / (high - low));
}

void BasicCompressor::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace reco;

  // auto result = std::make_unique<reco::PFJetCollection>();
  auto result = std::make_unique<std::vector<LorentzVec>>();
  auto resultComp = std::make_unique<std::vector<CompLorentzVec>>();
  const auto& jetsCollection = iEvent.get(jetsToken);

  for (const auto& jet : jetsCollection) {
    // do some compressing
    // reco::PFJet* newJet = jet.clone();

    // double shift = rand() / RAND_MAX; // random number between 0 and 1

    // todo
    // create another data format storing lorentz vector
    // take an object
    // instad of jet, create object called kylie lorentz vector, only store 4 quantities
    // inside producer, store both modified and unmodified vector
    // create 2 data formats and produce 2 objects v0 and v1, and put with two diff versions
    // LorentzVec l = LorentzVec(jet.px(), jet.py(), jet.pz(), jet.energy());
    // CompLorentzVec cl = CompLorentzVec((float)jet.px(), (float)jet.py(), (float)jet.pz(), (float)jet.energy());
    LorentzVec l = LorentzVec(3, 3, 3, 3);
    CompLorentzVec cl = CompLorentzVec(3, 3, 3, 3);

    result->push_back(l);
    resultComp->push_back(cl);

    // approxPFJet --> PFJet
    // todo try to rewrite new data format approxPFJet

    // algo based on supervised binning
    // float -> bin it to be a short
    // difference respect to standard binning, bins get adjusted in size by themself to give best resolution
    // using algo, maybe drop down entropy
    

    // // compress the 4-vector and set it here
    // float px = MiniFloatConverter::reduceMantissaToNbitsRounding<1>(jet.px());
    // float py = MiniFloatConverter::reduceMantissaToNbitsRounding<1>(jet.py());
    // float pz = MiniFloatConverter::reduceMantissaToNbitsRounding<1>(jet.pz());
    // float energy = MiniFloatConverter::reduceMantissaToNbitsRounding<1>(jet.energy());

    // // uint16_t px = MiniFloatConverter::float32to16round((float)jet.px());
    // // uint16_t py = MiniFloatConverter::float32to16round((float)jet.py());
    // // uint16_t pz = MiniFloatConverter::float32to16round((float)jet.pz());
    // // uint16_t energy = MiniFloatConverter::float32to16round((float)jet.energy());

    // // float32to16round

    // // todo cast to uint8 and then convert back to see what happens

    // // double px = getRandomNumber(jet.px() * 0.75, jet.px() * 1.5);
    // // double py = getRandomNumber(jet.py() * 0.75, jet.py() * 1.5);
    // // double pz = getRandomNumber(jet.pz() * 0.75, jet.pz() * 1.5);
    // // double energy = getRandomNumber(jet.energy() * 0.75, jet.energy() * 1.5);

    
    // // newJet->setP4(math::XYZTLorentzVectorF(px, py, pz, energy));
    // newJet->setP4(reco::Particle::LorentzVector(px, py, pz, energy)); 
    // // newJet->setP4(math::XYZTLorentzVector(px, py, pz, energy)); 
  
    // // can make another LorentzVector and make it float or uint instead of double
    // // can check out package with jet, compile it, take LorentzVector and change double with float ("tricking the system")
    // // do the analyzer first

    // // std::cout << "New jet px: " << jet.px() << ", px after shift: " << newJet->px() << std::endl;

    // result->push_back(*newJet);

    // // std::cout << "px after shift: " << (*result)[result->size()-1].px() << std::endl;
  }

  iEvent.put(std::move(result));
  iEvent.put(std::move(resultComp));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void BasicCompressor::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void BasicCompressor::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
BasicCompressor::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
BasicCompressor::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
BasicCompressor::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
BasicCompressor::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

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
