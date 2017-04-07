#include <boost/python.hpp>

#include "package_analysis/VehicleCouting.h"
#include "pyboostcvconverter/pyboostcvconverter.hpp"

namespace bp = boost::python;

class PyVehicleCouting : public VehicleCouting {
 public:
  void setInput(const bp::object &i) {
    VehicleCouting::setInput(pbcvt::fromNDArrayToMat(i.ptr()));
  }
};

BOOST_PYTHON_MODULE (_analysis) {
  bp::class_<PyVehicleCouting>("VehicleCouting")
      .def("setInput", &PyVehicleCouting::setInput)
      .def("setTracks", &PyVehicleCouting::setTracks)
      .def("process", &PyVehicleCouting::process);
}
