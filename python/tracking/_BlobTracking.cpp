#include <boost/python.hpp>
#include "pyboostcvconverter/pyboostcvconverter.hpp"
#include "package_tracking/BlobTracking.h"

namespace bp = boost::python;

class PyBlobTracking : public BlobTracking {
 public:
  bp::object process(const bp::object &img_input, const bp::object &img_mask) {
    cv::Mat output;
    BlobTracking::process(pbcvt::fromNDArrayToMat(img_input.ptr()),
                          pbcvt::fromNDArrayToMat(img_mask.ptr()),
                          output);
    return bp::object(bp::handle<>(pbcvt::fromMatToNDArray(output)));
  }

  const bp::dict getPyTracks() {
    cvb::CvTracks cv_tracks = BlobTracking::getTracks();

    bp::dict py_tracks;
    for (auto entry : cv_tracks) {
      cvb::CvTrack *cv_track = entry.second;

      bp::object py_track = bp::class_<cvb::CvTrack>("CvTrack")
          .def_readonly("id", &cvb::CvTrack::id,
                        "Track identification number.")
          .def_readonly("label", &cvb::CvTrack::label,
                        "Label assigned to the blob related to this track.")
          .def_readonly("minx", &cvb::CvTrack::minx,
                        "X min.")
          .def_readonly("maxx", &cvb::CvTrack::maxx,
                        "X max.")
          .def_readonly("miny", &cvb::CvTrack::miny,
                        "Y min.")
          .def_readonly("maxy", &cvb::CvTrack::maxy,
                        "y max.")
          .def_readonly("centroid", &cvb::CvTrack::centroid,
                        "Centroid.")
          .def_readonly("lifetime", &cvb::CvTrack::lifetime,
                        "Indicates how much frames the object has been in scene.")
          .def_readonly("active", &cvb::CvTrack::active,
                        "Indicates number of frames that has been active from last inactive period.")
          .def_readonly("inactive", &cvb::CvTrack::inactive,
                        "Indicates number of frames that has been missing.")();

      py_track.attr("id") = cv_track->id;
      py_track.attr("label") = cv_track->label;
      py_track.attr("minx") = cv_track->minx;
      py_track.attr("maxx") = cv_track->maxx;
      py_track.attr("miny") = cv_track->miny;
      py_track.attr("maxy") = cv_track->maxy;
      py_track.attr("centroid") = bp::class_<CvPoint2D64f>("CvPoint2D64f")
          .def_readonly("x", &CvPoint2D64f::x)
          .def_readonly("y", &CvPoint2D64f::y)();
      py_track.attr("centroid").attr("x") = cv_track->centroid.x;
      py_track.attr("centroid").attr("y") = cv_track->centroid.y;
      py_track.attr("lifetime") = cv_track->lifetime;
      py_track.attr("active") = cv_track->active;
      py_track.attr("inactive") = cv_track->inactive;

      py_tracks[entry.first] = py_track;
    }
  }
};

BOOST_PYTHON_MODULE (_tracking) {
  bp::class_<PyBlobTracking>("BlobTracking")
      .def("process", &PyBlobTracking::process)
      .def("getPyTracks", &PyBlobTracking::getPyTracks)
      .def("getTracks", &PyBlobTracking::getTracks);
}
