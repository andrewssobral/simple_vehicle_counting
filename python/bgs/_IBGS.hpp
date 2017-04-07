#ifndef SIMPLE_VEHICLE_COUNTING_IBGS_HPP
#define SIMPLE_VEHICLE_COUNTING_IBGS_HPP

#include "package_bgs/IBGS.h"

#include <boost/python.hpp>

class PyIBGS {
 public:
  virtual PyObject *process(PyObject *) = 0;
};

#endif //SIMPLE_VEHICLE_COUNTING_IBGS_HPP
