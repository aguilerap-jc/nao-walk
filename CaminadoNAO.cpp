#include "CaminadoNAO.h"

void Caminado::cShMaxX(float dist,AL::ALMotionProxy motion){
    //Falta Modificar Parametros
    //Manda los parametros de X, Frecuencia y Height al maximo
    //Recibe como parametros de entrada dist de tipo float, motion de tipo ALMotionProxy

     AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.08),AL::ALValue::array("MaxStepY",0.14)
                                                  ,AL::ALValue::array("MaxStepTheta",0.4),AL::ALValue::array("MaxStepFrequency",1)
                                                  ,AL::ALValue::array("StepHeight",0.04),AL::ALValue::array("TorsoWx",0.0)
                                                  ,AL::ALValue::array("TorsoWy",0));

     cout<<"Caminado Max Solo X"<<endl;
     motion.setSmartStiffnessEnabled(true);
     motion.moveTo(dist,0,0,maxStep);
}

void Caminado::cLiMaxX(float dist,AL::ALMotionProxy motion){
    //Falta Modificar Parametros
    //Manda los parametros de X, Frecuencia y Height al maximo
    //Recibe como parametros de entrada dist de tipo float, motion de tipo ALMotionProxy
     AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.08),AL::ALValue::array("MaxStepY",0.14)
                                ,AL::ALValue::array("MaxStepTheta",0.37),AL::ALValue::array("MaxStepFrequency",1)
                                ,AL::ALValue::array("StepHeight",0.04),AL::ALValue::array("TorsoWx",0.0)
                                ,AL::ALValue::array("TorsoWy",0));

     cout<<"Caminado Max Solo X"<<endl;
     motion.setSmartStiffnessEnabled(true);
     motion.moveTo(dist,0,0,maxStep);
}

void Caminado::cCaMaxX(float dist,AL::ALMotionProxy motion){
    //Falta Modificar Parametros
    //Manda los parametros de X, Frecuencia y Height al maximo
    //Recibe como parametros de entrada dist de tipo float, motion de tipo ALMotionProxy
     AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.08),AL::ALValue::array("MaxStepY",0.08)
                                ,AL::ALValue::array("MaxStepTheta",0.4),AL::ALValue::array("MaxStepFrequency",1)
                                ,AL::ALValue::array("StepHeight",0.04),AL::ALValue::array("TorsoWx",0.0)
                                ,AL::ALValue::array("TorsoWy",0));

     cout<<"Caminado Max Solo X"<<endl;
     motion.setSmartStiffnessEnabled(true);
     motion.moveTo(dist,0,0,maxStep);
}


void Caminado::cDerecha(float dist,AL::ALMotionProxy motion){
    //Manda los parametros de X, Frecuencia y Height al maximo
    //Recibe como parametros de entrada dist de tipo float, motion de tipo ALMotionProxy
     AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.08),AL::ALValue::array("MaxStepY",0.101)
                                ,AL::ALValue::array("MaxStepTheta",-0.523599),AL::ALValue::array("MaxStepFrequency",1)
                                ,AL::ALValue::array("StepHeight",0.4),AL::ALValue::array("TorsoWx",0.0)
                                ,AL::ALValue::array("TorsoWy",0));

     cout<<"Caminado Max Solo X"<<endl;
     motion.setSmartStiffnessEnabled(true);
     motion.moveTo(dist,0,0,maxStep);
}

void Caminado::cIzquierda(float dist,AL::ALMotionProxy motion){
    //Manda los parametros de X, Frecuencia y Height al maximo
    //Recibe como parametros de entrada dist de tipo float, motion de tipo ALMotionProxy
     AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.08),AL::ALValue::array("MaxStepY",0.101)
                                ,AL::ALValue::array("MaxStepTheta",0.523599),AL::ALValue::array("MaxStepFrequency",1)
                                ,AL::ALValue::array("StepHeight",0.4),AL::ALValue::array("TorsoWx",0.0)
                                ,AL::ALValue::array("TorsoWy",0));

     cout<<"Caminado Max Solo X"<<endl;
     motion.setSmartStiffnessEnabled(true);
     motion.moveTo(dist,0,0,maxStep);
}

void Caminado::cDefault(float dist,AL::ALMotionProxy motion){
    cout<<"Caminado Default MoveTo"<<endl;
    motion.moveTo(dist,0,0,motion.getFootGaitConfig("Default"));
}

void Caminado::cAtras(float dist,AL::ALMotionProxy motion){
    AL::ALValue maxStep     = AL::ALValue::array(AL::ALValue::array("MaxStepX",0.04),AL::ALValue::array("MaxStepY",0.101)
                                ,AL::ALValue::array("MaxStepTheta",0.001),AL::ALValue::array("MaxStepFrequency",1)
                                ,AL::ALValue::array("StepHeight",0.04),AL::ALValue::array("TorsoWx",0.0)
                                ,AL::ALValue::array("TorsoWy",-0.122173));

    cout<<"Caminado Hacia Atras"<<endl;
    motion.setSmartStiffnessEnabled(true);
    dist = dist * -1;
    cout<<dist<<endl;
    motion.moveTo(dist,0,0,maxStep);
}

//Giros del Robot Parametros de giro en grados
void Caminado::gIzquierda(float grad, AL::ALMotionProxy motion){
    float rads = ((grad*M_PI)/180);
    motion.moveTo(0,0,rads);
}

void Caminado::gDerecha(float grad, AL::ALMotionProxy motion){
    float rads = ((grad*M_PI)/180);
    motion.moveTo(0,0,-rads);
}

void Caminado::rutinaApagado(AL::ALMotionProxy motion,AL::ALRobotPostureProxy posture){
    motion.stopMove();
    posture.goToPosture("Crouch",0.8);
    motion.setStiffnesses("Body",0);
    AL::ALBehaviorManagerProxy behaviors;
    behaviors.stopAllBehaviors();


}
