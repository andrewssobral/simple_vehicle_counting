Vehicle Detection, Tracking and Counting
========================================

Last page update: **19/10/2016**

Last version: **1.0.0** (see Release Notes for more info)

Hi everyone,

There are several ways to perform vehicle detection, tracking and counting.
Here is a step-by-step of a simplest way to do this: 

1. First, you will need to detect the moving objects. An easy way to do vehicle detection is by using a Background Subtraction (BS) algorithm. You can try to use a background subtraction library like [BGSLibrary](https://github.com/andrewssobral/bgslibrary#bgslibrary).
2. For vehicle tracking, you will need to use a tracking algorithm. A simplest way to do this is by using a blob tracker algorithm (see [cvBlob](https://code.google.com/p/cvblob/) or [OpenCVBlobsLib](http://opencvblobslib.github.io/opencvblobslib/)). So, send the foreground mask to **cvBlob** or **OpenCVBlobsLib**. For example, the **cvBlob** library provide some methods to get the **centroid**, the **track** and the **ID** of the moving objects. You can also set to draw a **bounding box**, the **centroid** and the **angle** of the tracked object.
3. And then, check if the **centroid** of the moving object has crossed a **region of interest** (i.e. virtual line) in your video.
4. Voilà! enjoy it :)

<p align="center"><img src="https://sites.google.com/site/andrewssobral/vehicle_counting_screen.png" /></p>

For Windows users
-----------------
* There is a Visual Studio 2013 template project in the **vs2013/** folder. Open it in the Visual Studio IDE and select [Release]-[Win32] mode.
* The include files for the OpenCV 2.4.10 are provided in the **include/** folder, and the related static libraries are provided in the **lib/x86/vc12** folder.

For Linux users
-----------------
* For Linux and Mac users, a Makefile is provided to compile the source code.
* * Requirements: OpenCV 2.4.x (it only works with this version).
* * Check out the latest project source code and compile it:
``` 
~/git clone https://github.com/andrewssobral/simple_vehicle_counting.git
~/cd simple_vehicle_counting
~/simple_vehicle_counting/cd build
~/simple_vehicle_counting/build/ cmake ..
~/simple_vehicle_counting/build/ make
```
* * Run demo:
```
~/simple_vehicle_counting/run_simple_vehicle_counting.sh
```

Docker image
----------------------------------------
* Docker image is available at:
* * **Ubuntu 16.04 + VNC + OpenCV 2.4.13 + Python 2.7 + Vehicle Detection, Tracking and Counting**
https://hub.docker.com/r/andrewssobral/vehicle_detection_tracking_counting/

Example code
------------
```C++
#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "package_bgs/PBAS/PixelBasedAdaptiveSegmenter.h"
#include "package_tracking/BlobTracking.h"
#include "package_analysis/VehicleCouting.h"

int main(int argc, char **argv)
{
  /* Open video file */
  CvCapture *capture = 0;
  capture = cvCaptureFromAVI("dataset/video.avi");
  if(!capture){
    std::cerr << "Cannot open video!" << std::endl;
    return 1;
  }
  
  /* Background Subtraction Algorithm */
  IBGS *bgs;
  bgs = new PixelBasedAdaptiveSegmenter;
  
  /* Blob Tracking Algorithm */
  cv::Mat img_blob;
  BlobTracking* blobTracking;
  blobTracking = new BlobTracking;

  /* Vehicle Counting Algorithm */
  VehicleCouting* vehicleCouting;
  vehicleCouting = new VehicleCouting;

  std::cout << "Press 'q' to quit..." << std::endl;
  int key = 0;
  IplImage *frame;
  while(key != 'q')
  {
    frame = cvQueryFrame(capture);
    if(!frame) break;

    cv::Mat img_input(frame);
    cv::imshow("Input", img_input);

    // bgs->process(...) internally process and show the foreground mask image
    cv::Mat img_mask;
    bgs->process(img_input, img_mask);
    
    if(!img_mask.empty())
    {
      // Perform blob tracking
      blobTracking->process(img_input, img_mask, img_blob);

      // Perform vehicle counting
      vehicleCouting->setInput(img_blob);
      vehicleCouting->setTracks(blobTracking->getTracks());
      vehicleCouting->process();
    }

    key = cvWaitKey(1);
  }

  delete vehicleCouting;
  delete blobTracking;
  delete bgs;

  cvDestroyAllWindows();
  cvReleaseCapture(&capture);
  
  return 0;
}
```

Release Notes:
--------------
* Version 1.0.0:
First version.
