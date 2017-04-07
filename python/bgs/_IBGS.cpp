#include "_IBGS.hpp"

namespace bp = boost::python;

class PyIBGSWrap : public PyIBGS, public bp::wrapper<PyIBGS> {
 public:
  PyObject *process(PyObject *) {
    return this->get_override("process")();
  }
};

BOOST_PYTHON_MODULE (_bgs) {
  bp::class_<PyIBGSWrap, boost::noncopyable>("IBGS")
      .def("process", bp::pure_virtual(&PyIBGS::process));
}
