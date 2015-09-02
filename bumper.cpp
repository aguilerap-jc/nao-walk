/**
 * Copyright (c) 2011 Aldebaran Robotics
 */

#include "bumper.h"

#include "CaminadoNAO.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>

#include <alvision/alimage.h>
#include <alvision/alvideo.h>
#include <alvision/alvisiondefinitions.h>

#include <alproxies/alvideodeviceproxy.h>
#include <alproxies/alrobotpostureproxy.h>
#include <alproxies/almotionproxy.h>

#include <qi/log.hpp>
#include <althread/alcriticalsection.h>

using namespace std;
using namespace cv;
using namespace AL;

bool activacionCiclo=true;
bool comienzaRutinaApagado=false;
int cont=0;

void getUp();

Bumper::Bumper(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name): AL::ALModule(broker, name),
    fCallbackMutex(AL::ALMutex::createALMutex())
{
  setModuleDescription("This module presents how to subscribe to a simple event (here RightBumperPressed) and use a callback method.");

  functionName("onRightBumperPressed", getName(), "Method called when the right bumper is pressed. Makes a LED animation.");
  BIND_METHOD(Bumper::onRightBumperPressed)

  functionName("onLeftBumperPressed", getName(), "Method called when the left bumper is pressed. Makes a LED animation.");
  BIND_METHOD(Bumper::onLeftBumperPressed)
}

Bumper::~Bumper() {
  fMemoryProxy.unsubscribeToEvent("onRightBumperPressed", "Bumper");
  fMemoryProxy2.unsubscribeToEvent("onLeftBumperPressed", "Bumper");
}

void Bumper::init() {
  try {
    /** Create a proxy to ALMemory.
    */
    fMemoryProxy = AL::ALMemoryProxy(getParentBroker());
    fMemoryProxy2= AL::ALMemoryProxy(getParentBroker());

    fState = fMemoryProxy.getData("RightBumperPressed");
    fState2= fMemoryProxy2.getData("LeftBumperPressed");
    /** Subscribe to event LeftBumperPressed
    * Arguments:
    * - name of the event
    * - name of the module to be called for the callback
    * - name of the bound method to be called on event
    */
    fMemoryProxy.subscribeToEvent("RightBumperPressed", "Bumper",
                                   "onRightBumperPressed");
    fMemoryProxy2.subscribeToEvent("LeftBumperPressed", "Bumper",
                                   "onLeftBumperPressed");

  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void Bumper::onRightBumperPressed() {
  qiLogInfo("module.example") << "Executing callback method on right bumper event" << std::endl;
  /**
  * As long as this is defined, the code is thread-safe.
  */
  AL::ALCriticalSection section(fCallbackMutex);

  /**
  * Check that the bumper is pressed.
  */
  fState =  fMemoryProxy.getData("RightBumperPressed");

  if (fState  > 0.5f) {
    return;
  }
  try {
    fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
    fTtsProxy.say("Right bumper pressed");
    walkWithBumper();

  }
  catch (const AL::ALError& e) {
    qiLogError("module.example") << e.what() << std::endl;
  }
}

void Bumper::onLeftBumperPressed(){

    qiLogInfo("module.example") << "Executing callback method on left bumper event" << std::endl;
    /**
    * As long as this is defined, the code is thread-safe.
    */
    //AL::ALCriticalSection section(fCallbackMutex);
    /**
    * Check that the bumper is pressed.
    */

    fState2=  fMemoryProxy2.getData("LeftBumperPressed");
    cout<<"BumperIzq"<<endl;
    if (fState2 >0.5f){
      //return;
    }
    try {

      fTtsProxy = AL::ALTextToSpeechProxy(getParentBroker());
      //fTtsProxy.say("Left bumper pressed");
      cont++;
      cout<<"Cont ="<<cont;
      if(cont>=20){
      activacionCiclo=false;
      if(comienzaRutinaApagado){
        Caminado run;
        AL::ALMotionProxy motion;
        AL::ALRobotPostureProxy posture;
        run.rutinaApagado(motion,posture);
        cout<<"TerminoRutinaApagado"<<endl;
        //cout<<"AL::ALModule::exit();"<<endl;
                }
            }
        }
            catch (const AL::ALError& e) {
      qiLogError("module.example") << e.what() << std::endl;
    }
}

int getOrientation(vector<Point> &pts, Mat &img);

bool Bumper::actualizaActivacionCiclo(){
    if (activacionCiclo){activacionCiclo=true;}
    else {activacionCiclo=false;}
    return activacionCiclo;
}

void Bumper::walkWithBumper(){

    Mat src;
    Mat src_gray;
    int thresh = 100;
    RNG rng(12345);
    //Stand NAO
    AL::ALRobotPostureProxy posture;
    AL::ALMotionProxy motion;
    AL::ALVideoDeviceProxy camProxy;
    if(activacionCiclo){
    posture.goToPosture("StandInit", 0.5);
    }
    /// Load source image and convert it to gray
    VideoCapture cap(1);
    bool DEBUG = false;
    bool NAO = true;
    int area;
    int length;
    char key = 'a';
    int contFrames = 0;
    vector<vector<Point> > contoursClean;
    const std::string clientName = camProxy.subscribe("test", AL::kQVGA, AL::kBGRColorSpace, 30);
    float distancia = 0.65;
    //Sheldon 0.85
    //Academico 0.5
    float correccion = 18;
    float correccion2 = 25;
    int anguloDeMeta = 40;

    Caminado run;
    while(activacionCiclo) {
        if ( NAO ) {
            //Get image from NAO
            camProxy.setActiveCamera(1); //conect to bottom camera
            camProxy.setResolution("test", 1);
            /** Create an cv::Mat header to wrap into an opencv image.*/
            Mat imgHeader = cv::Mat(cv::Size(320, 240), CV_8UC3);

            /** Create a OpenCV window to display the images. */
            //cv::namedWindow("images");

            ALValue img = camProxy.getImageRemote(clientName);

            /** Access the image buffer (6th field) and assign it to the opencv image
            * container. */
            imgHeader.data = (uchar*) img[6].GetBinary();
            /** Tells to ALVideoDevice that it can give back the image buffer to the
            * driver. Optional after a getImageRemote but MANDATORY after a getImageLocal.*/
            camProxy.releaseImage(clientName);
            /** Display the iplImage on screen.*/
            src = imgHeader.clone();
            //imshow("src", src);
            //waitKey(50);
        } else {
        cap >> src;
        }
        if(contFrames == 5) {
            contFrames = 0;
            /// Convert image to gray and blur it
            //imshow("srcBGray", src);
            //waitKey(50);
            cvtColor( src, src_gray, CV_BGR2GRAY );
            blur( src_gray, src_gray, Size(3,3) );
            //imshow("blur", src_gray);
            //imshow( "src", src );

            Mat canny_output;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            /// Detect edges using canny
            Canny( src_gray, canny_output, thresh, thresh*2, 3 );
            /// Find contours
            findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

            /// Get the moments
              vector<Moments> mu(contours.size() );
              for( int i = 0; i < contours.size(); i++ )
                 { mu[i] = moments( contours[i], false ); }

              ///  Get the mass centers:
              vector<Point2f> mc( contours.size() );
              for( int i = 0; i < contours.size(); i++ ) {
                  mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
              }

              //Eliminate contours without Area
              contoursClean.clear();
              int indMax = 0;
              int lengthMax = 0;
              Point2f punto;
              for(int i = 0; i < contours.size(); i++) {
                 area = mu[i].m00;
                 length = arcLength(contours[i], true);
                 punto = mc[i];
                 if(DEBUG){
                     cout << "Area " << i << " = " << area << endl;
                     cout << "Length " << i << " = " << length << endl;
                     cout << "mass Center " << i << " = " << punto.x << "," << punto.y << endl;
                     cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                 }
                 if(area != 0 && length > 200 && punto.x > 0 && punto.y > 0) {
                     contoursClean.push_back(contours.at(i));
                 }
              }
              if(contoursClean.size() != 0) {
              //Get moments and mass for new vector
              vector<Moments> muClean(contoursClean.size() );
              for( int i = 0; i < contoursClean.size(); i++ )
                 { muClean[i] = moments( contoursClean[i], false ); }

              ///  Get the mass centers:
              vector<Point2f> mcClean( contoursClean.size() );
              for( int i = 0; i < contoursClean.size(); i++ ) {
                  mcClean[i] = Point2f( muClean[i].m10/muClean[i].m00 , muClean[i].m01/muClean[i].m00 );
              }

              for(int i = 0; i < contoursClean.size(); i++) {
                  punto = mcClean[i];
                  length = arcLength(contoursClean[i], true);
                  if(DEBUG){
                      cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                      cout << "Area " << i << " = " << area << endl;
                      cout << "Length " << i << " = " << length << endl;
                      cout << "mass Center " << i << " = " << punto.x << "," << punto.y << endl;
                      cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                  }
              }
                 //Encontrar el mas largo
                 for(int i = 0; i < contoursClean.size(); i++) {
                  length = arcLength(contoursClean[i], true);
                  lengthMax = arcLength(contoursClean[indMax], true);
                     if(i > 0) {
                      if(length  > lengthMax) {
                          indMax = i;
                      }
                  } else {
                      indMax = 0;
                  }
              }
              /// Draw contours
              Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
              cout << "*************************************" << endl;
              cout << "# de lineas encontradas = " << contoursClean.size() << endl;
              //for( int i = 0; i< contoursClean.size(); i++ ) {
              Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
              drawContours( drawing, contoursClean, indMax, color, 2, 8, hierarchy, 0, Point() );
              //circle( drawing, mcClean[indMax], 4, color, 5, 8, 0 );
              //Orientation
              int orientation = getOrientation(contoursClean[indMax], drawing);
              cout << "orientation degrees = " << orientation <<endl;
              //}

              Point2f puntoMax;
              if(contoursClean.size() != 0) {
                  puntoMax = mcClean[indMax];
                  lengthMax = arcLength(contoursClean[indMax], true);
                  if(DEBUG) {
                      cout << "El mas grande es el " << indMax << " con " << lengthMax << endl;
                      cout << "Y esta en la posicion x = " << puntoMax.x << " y = " << puntoMax.y << endl;
                      cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
                  }
                  int umbral = 0;
                  if(puntoMax.x > ((drawing.cols/2) + umbral) && orientation <= 90 && orientation >= anguloDeMeta)  { //linea correcta
                      cout << "Corregir a la Izquierda" << endl;
                      run.gIzquierda(correccion, motion);
                      run.cShMaxX(distancia,motion);
                  } else if(puntoMax.x < ((drawing.cols/2) - umbral) && orientation > 90) { //linea correcta
                      cout << "Corregir a la Derecha" << endl;
                      run.gDerecha(correccion, motion);
                      run.cShMaxX(distancia,motion);
                  } else if(puntoMax.x > ((drawing.cols/2) + umbral) && orientation >= 90)  { //linea incorrecta
                      cout << "Corregir a la Derecha" << endl;
                      run.gDerecha(correccion2, motion);
                      run.cShMaxX(distancia,motion);
                  } else if(puntoMax.x < ((drawing.cols/2) - umbral) && orientation < 90 && orientation >= anguloDeMeta) { //linea incorrecta
                      cout << "Corregir a la Izquierda" << endl;
                      run.gIzquierda(correccion2, motion);
                      run.cShMaxX(distancia,motion);
                  } else {
                          cout << "Sigue Derecho11111" << endl;
                          run.cShMaxX(distancia,motion);
                  }
              } else {
                  cout << "No hay lineas detectadas" << endl;
                  cout << "Sigue Derecho222222" << endl;
                  run.cShMaxX(distancia,motion);

              }
              } else {
                  cout << "No hay lineas detectadas" << endl;
                  cout << "Sigue Derecho3333" << endl;
                  run.cShMaxX(distancia,motion);
              }
        }
          contFrames++;
  }
camProxy.unsubscribe(clientName);
    activacionCiclo=actualizaActivacionCiclo();
if(activacionCiclo==false){
comienzaRutinaApagado=true;}
getUp();
}

int getOrientation(vector<Point> &pts, Mat &img)
{
  //Construct a buffer used by the pca analysis
  Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
  for (int i = 0; i < data_pts.rows; ++i)
  {
      data_pts.at<double>(i, 0) = pts[i].x;
      data_pts.at<double>(i, 1) = pts[i].y;
  }

  //Perform PCA analysis
  PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

  //Store the position of the object
  Point pos = Point(pca_analysis.mean.at<double>(0, 0),
                    pca_analysis.mean.at<double>(0, 1));

  //Store the eigenvalues and eigenvectors
  vector<Point2d> eigen_vecs(2);
  vector<double> eigen_val(2);
  for (int i = 0; i < 2; ++i)
  {
      eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                              pca_analysis.eigenvectors.at<double>(i, 1));

      eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);
  }

  return ((atan2(eigen_vecs[0].y, eigen_vecs[0].x))*180)/M_PI;
}

void getUp(){
    AL::ALRobotPostureProxy posture;
    string postura;

    postura= posture.getPostureFamily();

    if ((postura == "LyingBelly")||(postura == "LyingBack")||(postura == "Back"))
    {
    posture.goToPosture("Stand",1);
    }
}
