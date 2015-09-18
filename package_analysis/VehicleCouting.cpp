#include "VehicleCouting.h"

namespace FAV1
{
  IplImage* img_input1 = 0;
  IplImage* img_input2 = 0;
  int roi_x0 = 0;
  int roi_y0 = 0;
  int roi_x1 = 0;
  int roi_y1 = 0;
  int numOfRec = 0;
  int startDraw = 0;
  bool roi_defined = false;
  bool use_roi = true;
  void VehicleCouting_on_mouse(int evt, int x, int y, int flag, void* param)
  {
    if(!use_roi)
      return;
  
    if(evt == CV_EVENT_LBUTTONDOWN)
    {
      if(!startDraw)
      {
        roi_x0 = x;
        roi_y0 = y;
        startDraw = 1;
      }
      else
      {
        roi_x1 = x;
        roi_y1 = y;
        startDraw = 0;
        roi_defined = true;
      }
    }

    if(evt == CV_EVENT_MOUSEMOVE && startDraw)
    {
      //redraw ROI selection
      img_input2 = cvCloneImage(img_input1);
      cvLine(img_input2, cvPoint(roi_x0,roi_y0), cvPoint(x,y), CV_RGB(255,0,255));
      cvShowImage("VehicleCouting", img_input2);
      cvReleaseImage(&img_input2);
    }
  }
}

VehicleCouting::VehicleCouting(): firstTime(true), showOutput(true), key(0), countAB(0), countBA(0), showAB(0)
{
  std::cout << "VehicleCouting()" << std::endl;
}

VehicleCouting::~VehicleCouting()
{
  std::cout << "~VehicleCouting()" << std::endl;
}

void VehicleCouting::setInput(const cv::Mat &i)
{
  //i.copyTo(img_input);
  img_input = i;
}

void VehicleCouting::setTracks(const cvb::CvTracks &t)
{
  tracks = t;
}

VehiclePosition VehicleCouting::getVehiclePosition(const CvPoint2D64f centroid)
{
  VehiclePosition vehiclePosition = VP_NONE;

  if(laneOrientation == LO_HORIZONTAL)
  {
    if(centroid.x < FAV1::roi_x0)
    {
      cv::putText(img_input, "STATE: A", cv::Point(10,img_h/2), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      vehiclePosition = VP_A;
    }
    
    if(centroid.x > FAV1::roi_x0)
    {
      cv::putText(img_input, "STATE: B", cv::Point(10,img_h/2), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      vehiclePosition = VP_B;
    }
  }

  if(laneOrientation == LO_VERTICAL)
  {
    if(centroid.y > FAV1::roi_y0)
    {
      cv::putText(img_input, "STATE: A", cv::Point(10,img_h/2), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      vehiclePosition = VP_A;
    }
    
    if(centroid.y < FAV1::roi_y0)
    {
      cv::putText(img_input, "STATE: B", cv::Point(10,img_h/2), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      vehiclePosition = VP_B;
    }
  }

  return vehiclePosition;
}

void VehicleCouting::process()
{
  if(img_input.empty())
    return;

  img_w = img_input.size().width;
  img_h = img_input.size().height;

  loadConfig();

  //--------------------------------------------------------------------------

  if(FAV1::use_roi == true && FAV1::roi_defined == false && firstTime == true)
  {
    do
    {
      cv::putText(img_input, "Please, set the counting line", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,255));
      cv::imshow("VehicleCouting", img_input);
      FAV1::img_input1 = new IplImage(img_input);
      cvSetMouseCallback("VehicleCouting", FAV1::VehicleCouting_on_mouse, NULL);
      key = cvWaitKey(0);
      delete FAV1::img_input1;

      if(FAV1::roi_defined)
      {
        std::cout << "Counting line defined (" << FAV1::roi_x0 << "," << FAV1::roi_y0 << "," << FAV1::roi_x1 << "," << FAV1::roi_y1 << ")" << std::endl;
        break;
      }
      else
        std::cout << "Counting line undefined!" << std::endl;
    }while(1);
  }

  if(FAV1::use_roi == true && FAV1::roi_defined == true)
    cv::line(img_input, cv::Point(FAV1::roi_x0,FAV1::roi_y0), cv::Point(FAV1::roi_x1,FAV1::roi_y1), cv::Scalar(0,0,255));
  
  bool ROI_OK = false;
  
  if(FAV1::use_roi == true && FAV1::roi_defined == true)
    ROI_OK = true;

  if(ROI_OK)
  {
    laneOrientation = LO_NONE;

    if(abs(FAV1::roi_x0 - FAV1::roi_x1) < abs(FAV1::roi_y0 - FAV1::roi_y1))
    {
      if(!firstTime)
        cv::putText(img_input, "HORIZONTAL", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      laneOrientation = LO_HORIZONTAL;

      cv::putText(img_input, "(A)", cv::Point(FAV1::roi_x0-32,FAV1::roi_y0), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      cv::putText(img_input, "(B)", cv::Point(FAV1::roi_x0+12,FAV1::roi_y0), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
    }

    if(abs(FAV1::roi_x0 - FAV1::roi_x1) > abs(FAV1::roi_y0 - FAV1::roi_y1))
    {
      if(!firstTime)
        cv::putText(img_input, "VERTICAL", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      laneOrientation = LO_VERTICAL;

      cv::putText(img_input, "(A)", cv::Point(FAV1::roi_x0,FAV1::roi_y0+22), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
      cv::putText(img_input, "(B)", cv::Point(FAV1::roi_x0,FAV1::roi_y0-12), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
    }
  }

  //--------------------------------------------------------------------------

  for(std::map<cvb::CvID,cvb::CvTrack*>::iterator it = tracks.begin() ; it != tracks.end(); it++)
  {
    //std::cout << (*it).first << " => " << (*it).second << std::endl;
    cvb::CvID id = (*it).first;
    cvb::CvTrack* track = (*it).second;

    CvPoint2D64f centroid = track->centroid;
    /*
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "0)id:" << id << " (" << centroid.x << "," << centroid.y << ")" << std::endl;
    std::cout << "track->active:" << track->active << std::endl;
    std::cout << "track->inactive:" << track->inactive << std::endl;
    std::cout << "track->lifetime:" << track->lifetime << std::endl;
    */
    
    //----------------------------------------------------------------------------

    if(track->inactive == 0)
    {
      if(positions.count(id) > 0)
      {
        std::map<cvb::CvID, VehiclePosition>::iterator it2 = positions.find(id);
        VehiclePosition old_position = it2->second;

        VehiclePosition current_position = getVehiclePosition(centroid);

        if(current_position != old_position)
        {
          if(old_position == VP_A && current_position == VP_B)
            countAB++;

          if(old_position == VP_B && current_position == VP_A)
            countBA++;

          positions.erase(positions.find(id));
        }
      }
      else
      {
        VehiclePosition vehiclePosition = getVehiclePosition(centroid);

        if(vehiclePosition != VP_NONE)
          positions.insert(std::pair<cvb::CvID, VehiclePosition>(id,vehiclePosition));
      }
    }
    else
    {
      if(positions.count(id) > 0)
        positions.erase(positions.find(id));
    }

    //----------------------------------------------------------------------------

    if(points.count(id) > 0)
    {
      std::map<cvb::CvID, std::vector<CvPoint2D64f>>::iterator it2 = points.find(id);
      std::vector<CvPoint2D64f> centroids = it2->second;
      
      std::vector<CvPoint2D64f> centroids2;
      if(track->inactive == 0 && centroids.size() < 30)
      {
        centroids2.push_back(centroid);
      
        for(std::vector<CvPoint2D64f>::iterator it3 = centroids.begin() ; it3 != centroids.end(); it3++)
        {
          centroids2.push_back(*it3);
        }

        for(std::vector<CvPoint2D64f>::iterator it3 = centroids2.begin() ; it3 != centroids2.end(); it3++)
        {
          cv::circle(img_input, cv::Point((*it3).x,(*it3).y), 3, cv::Scalar(255,255,255), -1);
        }
      
        points.erase(it2);
        points.insert(std::pair<cvb::CvID, std::vector<CvPoint2D64f>>(id,centroids2));
      }
      else
      {
        points.erase(it2);
      }
    }
    else
    {
      if(track->inactive == 0)
      {
        std::vector<CvPoint2D64f> centroids;
        centroids.push_back(centroid);
        points.insert(std::pair<cvb::CvID, std::vector<CvPoint2D64f>>(id,centroids));
      }
    }

    //cv::waitKey(0);
  }
  
  //--------------------------------------------------------------------------

  std::string countABstr = "A->B: " + std::to_string(countAB);
  std::string countBAstr = "B->A: " + std::to_string(countBA);

  if(showAB == 0)
  {
    cv::putText(img_input, countABstr, cv::Point(10, img_h - 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
    cv::putText(img_input, countBAstr, cv::Point(10, img_h - 8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
  }
  
  if(showAB == 1)
    cv::putText(img_input, countABstr, cv::Point(10, img_h - 8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
  
  if(showAB == 2)
    cv::putText(img_input, countBAstr, cv::Point(10, img_h - 8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));

  if(showOutput)
    cv::imshow("VehicleCouting", img_input);

  if(firstTime)
    saveConfig();

  firstTime = false;
}

void VehicleCouting::saveConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("config/VehicleCouting.xml", 0, CV_STORAGE_WRITE);

  cvWriteInt(fs, "showOutput", showOutput);
  cvWriteInt(fs, "showAB", showAB);
  
  cvWriteInt(fs, "fav1_use_roi", FAV1::use_roi);
  cvWriteInt(fs, "fav1_roi_defined", FAV1::roi_defined);
  cvWriteInt(fs, "fav1_roi_x0", FAV1::roi_x0);
  cvWriteInt(fs, "fav1_roi_y0", FAV1::roi_y0);
  cvWriteInt(fs, "fav1_roi_x1", FAV1::roi_x1);
  cvWriteInt(fs, "fav1_roi_y1", FAV1::roi_y1);
  
  cvReleaseFileStorage(&fs);
}

void VehicleCouting::loadConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("config/VehicleCouting.xml", 0, CV_STORAGE_READ);

  showOutput = cvReadIntByName(fs, 0, "showOutput", true);
  showAB = cvReadIntByName(fs, 0, "showAB", 1);
  
  FAV1::use_roi = cvReadIntByName(fs, 0, "fav1_use_roi", true);
  FAV1::roi_defined = cvReadIntByName(fs, 0, "fav1_roi_defined", false);
  FAV1::roi_x0 = cvReadIntByName(fs, 0, "fav1_roi_x0", 0);
  FAV1::roi_y0 = cvReadIntByName(fs, 0, "fav1_roi_y0", 0);
  FAV1::roi_x1 = cvReadIntByName(fs, 0, "fav1_roi_x1", 0);
  FAV1::roi_y1 = cvReadIntByName(fs, 0, "fav1_roi_y1", 0);
  
  cvReleaseFileStorage(&fs);
}
