#ifndef CAMINADONAO_H
#define CAMINADONAO_H
#include <iostream>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>
#include <alproxies/albehaviormanagerproxy.h>

#include <alcommon/almodule.h>
#include <alcommon/alproxy.h>

#include <math.h>


using namespace std;

class Caminado{
    private :
        //float pi= 3.14159265359;
    public:
    // Sh-> Sheldon   Li->Lisa   Ch->Chicharito Ca->Catarino
    void cShMaxX(float dist,AL::ALMotionProxy motion);
    void cLiMaxX(float dist,AL::ALMotionProxy motion);
    void cCaMaxX(float dist,AL::ALMotionProxy motion);
    void cDerecha(float dist,AL::ALMotionProxy motion);
    void cIzquierda(float dist,AL::ALMotionProxy motion);

    void cDefault(float dist,AL::ALMotionProxy motion);
    void cAtras(float dist,AL::ALMotionProxy motion);

    //Giros. Se utilizara Grados
    void gIzquierda(float grad,AL::ALMotionProxy motion);
    void gDerecha(float grad,AL::ALMotionProxy motion);

    //Rutina Apagado
    void rutinaApagado(AL::ALMotionProxy motion,AL::ALRobotPostureProxy posture);
};
#endif // CAMINADONAO_H


