//
//  pspLinearTrajectoryParticle.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 26/08/15.
//
//

#include "pspLinearTrajectoryParticle.h"
#include "pspLinearTrajectorySystem.h"


pspLinearTrajectoryParticle::pspLinearTrajectoryParticle(){
    
}
pspLinearTrajectoryParticle::pspLinearTrajectoryParticle(pspParticleSystem* ms, int ind){
    setup(ms, ind);
    
    
}
pspLinearTrajectoryParticle::~pspLinearTrajectoryParticle(){
    
}

void pspLinearTrajectoryParticle::specificSetup(){
    pts = static_cast<pspLinearTrajectorySystem*>(mySystem)->getPts();
    size = 0.02;
    setNumLoops(0);
    resetTime();
}
void pspLinearTrajectoryParticle::specificUpdate(){
    //cout<<endl<<loopCounter<<" "<<numLoops;
    
    if(pts->size() >= 2){
        if(loopCounter <= numLoops){
            currentTime = Time::getMillisecondCounterHiRes();
            elapsedTime = currentTime - startTime;
            if(elapsedTime - 10. <= (*pts)[ib].time){
                double x, y, z;
                x = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].x, (*pts)[ib].x, true);
                //cout<<endl<<ia<<" "<<ib<<" "<<elapsedTime<<" "<<currentTime<<" "<<(*pts)[ib].time<<" "<<(*pts)[ia].x<<" "<<(*pts)[ib].x;
                y = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].y, (*pts)[ib].y, true);
                z = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].z, (*pts)[ib].z, true);
                pos = ofVec3f(x, y, z);
                //cout<<endl<<pos.x<<" "<<pos.y<<" "<<pos.z;
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
}


void pspLinearTrajectoryParticle::setNumLoops(int nl){
    //cout<<endl<<numLoops;
    numLoops = nl;
    loopCounter = 0;
    //resetTime();
}

void pspLinearTrajectoryParticle::resetTime(){
    loopCounter = 0;
    startTime = Time::getMillisecondCounterHiRes();
    currentTime = startTime;
    elapsedTime = 0;
    currentPoint = 0;
    ia = 0;
    ib = 1;
}

void pspLinearTrajectoryParticle::specificDraw(){
    
    if(!pts->empty()){
        for(int i=0; i<(pts->size() - 1); i++){
            glPushMatrix();
            glBegin(GL_LINE_STRIP);
            glVertex3f((*pts)[i].x, (*pts)[i].y, (*pts)[i].z);
            glVertex3f((*pts)[i + 1].x, (*pts)[i + 1].y, (*pts)[i + 1].z);
            glEnd();
            glPopMatrix();
        }
        
        for(int i=0; i<pts->size(); i++){
            glPushMatrix();
            GLUquadricObj* Sphere;
            glTranslatef((*pts)[i].x, (*pts)[i].y, (*pts)[i].z);
            Sphere = gluNewQuadric();
            gluSphere(Sphere, size, 4, 4);
            gluDeleteQuadric(Sphere);
            glPopMatrix();
        }
        glPushMatrix();
        GLUquadricObj* Sphere;
        glTranslatef(pos.x, pos.y, pos.z);
        Sphere = gluNewQuadric();
        gluSphere(Sphere, size, 4, 4);
        gluDeleteQuadric(Sphere);
        glPopMatrix();
        
    }
}