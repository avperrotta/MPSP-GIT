//
//  pspSphericalMotionParticle.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#include "pspSphericalMotionParticle.h"
#include "pspSphericalMotionSystem.h"

pspSphericalMotionTrajectoryParticle::pspSphericalMotionTrajectoryParticle(){
    
}
pspSphericalMotionTrajectoryParticle::pspSphericalMotionTrajectoryParticle(pspParticleSystem* ms, int ind){
    setup(ms, ind);
    
    
}
pspSphericalMotionTrajectoryParticle::~pspSphericalMotionTrajectoryParticle(){
    
}

void pspSphericalMotionTrajectoryParticle::specificSetup(){
    pts = static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->getPts();
    startPosition = static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->getStartPosition();
    size = 0.02;
    setNumLoops(0);
    resetTime();
}
void pspSphericalMotionTrajectoryParticle::specificUpdate(){
    //cout<<endl<<loopCounter<<" "<<numLoops;
    
    if(pts->size() >= 2){
        if(loopCounter <= numLoops){
            currentTime = Time::getMillisecondCounterHiRes();
            elapsedTime = currentTime - startTime;
            if(elapsedTime - 10. <= (*pts)[ib].time){
                
                r = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].r, (*pts)[ib].r, true);
                vTheta = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].vTheta, (*pts)[ib].vTheta, true);
                vPhi = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].vPhi, (*pts)[ib].vPhi, true);
                cx = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cx, (*pts)[ib].cx, true);
                cy = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cy, (*pts)[ib].cy, true);
                cz = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cz, (*pts)[ib].cz, true);
                
            }
            else{
                startTime = Time::getMillisecondCounterHiRes();
                ib++;
                if(ib >= pts->size()){
                    ib = 0;
                    loopCounter++;
                }
                ia++;
                if(ia >= pts->size()){
                    ia = 0;
                }
                
            }
        }
    }
    else if(pts->size() == 1){
        r = (*pts)[0].r;
        cx = (*pts)[0].cx;
        cy = (*pts)[0].cy;
        cz = (*pts)[0].cz;
        vTheta = (*pts)[0].vTheta;
        vPhi = (*pts)[0].vPhi;
    }
    else if(pts->size() == 0){
        r = 0.5;
        cx = 0.;
        cy = 0.;
        cz = 0.;
        vTheta = 0.;
        vPhi = 0.;
    }
    
    
    theta += vTheta;
    phi += vPhi;
    
    x = cx + r*sin(theta)*cos(phi);
    y = cy + r*sin(theta)*sin(phi);
    z = cz + r*cos(theta);
    pos = ofVec3f(x, y, z);
    
    //cout<<endl<<theta<<" "<<phi<<" "<<vTheta<<" "<<vPhi;
    
}


void pspSphericalMotionTrajectoryParticle::setNumLoops(int nl){
    //cout<<endl<<numLoops;
    numLoops = nl;
    loopCounter = 0;
    //resetTime();
}

void pspSphericalMotionTrajectoryParticle::resetTime(){
    loopCounter = 0;
    startTime = Time::getMillisecondCounterHiRes();
    currentTime = startTime;
    elapsedTime = 0;
    currentPoint = 0;
    ia = 0;
    ib = 1;
    
    theta = startPosition->theta;
    phi = startPosition->phi;
}

void pspSphericalMotionTrajectoryParticle::specificDraw(){
    
    if(!pts->empty()){
        glPushMatrix();
        GLUquadricObj* Sphere;
        glTranslatef(pos.x, pos.y, pos.z);
        Sphere = gluNewQuadric();
        gluSphere(Sphere, size, 4, 4);
        gluDeleteQuadric(Sphere);
        glPopMatrix();
        
    }
}