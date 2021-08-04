#ifndef DATAFORMATS_APPROXPFJET_H
#define DATAFORMATS_APPROXPFJET_H

#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class ApproxPFJet {
public:
  struct Specific {
    Specific()
        : mChargedHadronEnergy(0),
          mNeutralHadronEnergy(0),
          mPhotonEnergy(0),
          mElectronEnergy(0),
          mMuonEnergy(0),
          mHFHadronEnergy(0),
          mHFEMEnergy(0),

          mChargedHadronMultiplicity(0),
          mNeutralHadronMultiplicity(0),
          mPhotonMultiplicity(0),
          mElectronMultiplicity(0),
          mMuonMultiplicity(0),
          mHFHadronMultiplicity(0),
          mHFEMMultiplicity(0),

          mChargedEmEnergy(0),
          mChargedMuEnergy(0),
          mNeutralEmEnergy(0),

          mChargedMultiplicity(0),
          mNeutralMultiplicity(0),

          mHOEnergy(0) {}
    float mChargedHadronEnergy;
    float mNeutralHadronEnergy;
    float mPhotonEnergy;
    float mElectronEnergy;
    float mMuonEnergy;
    float mHFHadronEnergy;
    float mHFEMEnergy;

    int mChargedHadronMultiplicity;
    int mNeutralHadronMultiplicity;
    int mPhotonMultiplicity;
    int mElectronMultiplicity;
    int mMuonMultiplicity;
    int mHFHadronMultiplicity;
    int mHFEMMultiplicity;

    //old (deprecated) data members
    //kept only for backwards compatibility:
    float mChargedEmEnergy;
    float mChargedMuEnergy;
    float mNeutralEmEnergy;
    int mChargedMultiplicity;
    int mNeutralMultiplicity;

    float mHOEnergy;
  };

  ApproxPFJet() {}

  // TODO worry about constituents lol
  ApproxPFJet(std::vector<float> p4, std::vector<float> vertex, Specific& specific) {
    p4_ = p4;
    vertex_ = vertex;
    m_specific = specific;
  }

  float px() const {return p4_[0]; }
  float py() const {return p4_[1]; }
  float pz() const {return p4_[2]; }
  float E() const {return p4_[3]; }

  float vx() const {return vertex_[0];}
  float vy() const {return vertex_[1];}
  float vz() const {return vertex_[2];}

private:
  std::vector<float> p4_;  // px, py, pz, energy, corresponds to math::XYZTLorentzVector LorentzVector;
  std::vector<float> vertex_;  // x, y, z, corresponds to math::XYZPoint Point;
  Specific m_specific;
};
#endif  // DATAFORMATS_ApproxPFJet_H
