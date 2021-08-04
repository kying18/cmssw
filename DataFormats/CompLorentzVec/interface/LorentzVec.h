#ifndef DATAFORMATS_LORENTZVEC_H
#define DATAFORMATS_LORENTZVEC_H

#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class LorentzVec {
public:
  LorentzVec() {}

  LorentzVec(double px, double py, double pz, double E) {
    px_ = px;
    py_ = py;
    pz_ = pz;
    E_ = E;
  }

  double px() const {return px_; }
  double py() const {return py_; }
  double pz() const {return pz_; }
  double E() const {return E_; }

private:

  double px_ = 0;
  double py_ = 0;
  double pz_ = 0;
  double E_ = 0;

};
#endif  // DATAFORMATS_LorentzVec_H