#include <cstdio>

#include "../_IBGS.hpp"

#include "package_bgs/PBAS/PixelBasedAdaptiveSegmenter.h"
#include "pyboostcvconverter/pyboostcvconverter.hpp"

namespace bp = boost::python;

class PyPixelBasedAdaptiveSegmenter : public PyIBGS, public PixelBasedAdaptiveSegmenter {
 public:
  PyObject *process(PyObject *py_img_input) {
    cv::Mat output;
    cv::Mat cv_img_input = pbcvt::fromNDArrayToMat(py_img_input);
    PixelBasedAdaptiveSegmenter::process(cv_img_input, output);
    if (output.empty())
      return NULL;
    else
      return pbcvt::fromMatToNDArray(output);
  }
};

BOOST_PYTHON_MODULE (_pbas) {
  bp::class_<PyPixelBasedAdaptiveSegmenter>("PixelBasedAdaptiveSegmenter")
      .def("process", &PyPixelBasedAdaptiveSegmenter::process);
}
