#ifndef DATAFORMATS_COMPLORENTZVEC_H
#define DATAFORMATS_COMPLORENTZVEC_H

#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class CompLorentzVec {
public:
  CompLorentzVec() {}

  CompLorentzVec(float px, float py, float pz, float E) {
    px_ = px;
    py_ = py;
    pz_ = pz;
    E_ = E;
  }

  float px() const {return px_; }
  float py() const {return py_; }
  float pz() const {return pz_; }
  float E() const {return E_; }

private:

  float px_ = 0;
  float py_ = 0;
  float pz_ = 0;
  float E_ = 0;

};
#endif  // DATAFORMATS_CompLorentzVec_H
