#include <boost/python.hpp>

#include "package_tracking/cvblob/cvblob.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE (_cvb) {
  bp::class_<CvPoint2D64f>("CvPoint2D64f")
      .def_readwrite("x", &CvPoint2D64f::x)
      .def_readwrite("y", &CvPoint2D64f::y);

  bp::class_<cvb::CvTrack>("CvTrack")
      .def_readwrite("id", &cvb::CvTrack::id,
                     "Track identification number.")
      .def_readwrite("label", &cvb::CvTrack::label,
                     "Label assigned to the blob related to this track.")
      .def_readwrite("minx", &cvb::CvTrack::minx,
                     "X min.")
      .def_readwrite("maxx", &cvb::CvTrack::maxx,
                     "X max.")
      .def_readwrite("miny", &cvb::CvTrack::miny,
                     "Y min.")
      .def_readwrite("maxy", &cvb::CvTrack::maxy,
                     "y max.")
      .def_readwrite("centroid", &cvb::CvTrack::centroid,
                     "Centroid.")
      .def_readwrite("lifetime", &cvb::CvTrack::lifetime,
                     "Indicates how much frames the object has been in scene.")
      .def_readwrite("active", &cvb::CvTrack::active,
                     "Indicates number of frames that has been active from last inactive period.")
      .def_readwrite("inactive", &cvb::CvTrack::inactive,
                     "Indicates number of frames that has been missing.");
  bp::class_<cvb::CvTracks>("CvTracks");
}
