//
//  pspAngularTrajectoryParticle.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#include "pspAngularTrajectoryParticle.h"
#include "pspAngularTrajectorySystem.h"


pspAngularTrajectoryParticle::pspAngularTrajectoryParticle(){
    
}
pspAngularTrajectoryParticle::pspAngularTrajectoryParticle(pspParticleSystem* ms, int ind){
    setup(ms, ind);
    
    
}
pspAngularTrajectoryParticle::~pspAngularTrajectoryParticle(){
    
}

void pspAngularTrajectoryParticle::specificSetup(){
    pts = static_cast<pspAngularTrajectorySystem*>(mySystem)->getPts();
    size = 0.02;
    setNumLoops(0);
    resetTime();
}
void pspAngularTrajectoryParticle::specificUpdate(){
    //cout<<endl<<loopCounter<<" "<<numLoops;
    
    if(pts->size() >= 2){
        if(loopCounter <= numLoops){
            currentTime = Time::getMillisecondCounterHiRes();
            elapsedTime = currentTime - startTime;
            if(elapsedTime - 10. <= (*pts)[ib].time){
                double r, theta, phi, cx, cy, cz;
                r = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].r, (*pts)[ib].r, true);
                //cout<<endl<<ia<<" "<<ib<<" "<<elapsedTime<<" "<<currentTime<<" "<<(*pts)[ib].time<<" "<<(*pts)[ia].x<<" "<<(*pts)[ib].x;
                theta = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].theta, (*pts)[ib].theta, true);
                phi = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].phi, (*pts)[ib].phi, true);
                cx = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cx, (*pts)[ib].cx, true);
                cy = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cy, (*pts)[ib].cy, true);
                cz = interpolate(elapsedTime, 0, (*pts)[ib].time, (*pts)[ia].cz, (*pts)[ib].cz, true);
                
                double x, y, z;
                x = cx + r*sin(theta)*cos(phi);
                y = cy + r*sin(theta)*sin(phi);
                z = cz + r*cos(theta);
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


void pspAngularTrajectoryParticle::setNumLoops(int nl){
    //cout<<endl<<numLoops;
    numLoops = nl;
    loopCounter = 0;
    //resetTime();
}

void pspAngularTrajectoryParticle::resetTime(){
    loopCounter = 0;
    startTime = Time::getMillisecondCounterHiRes();
    currentTime = startTime;
    elapsedTime = 0;
    currentPoint = 0;
    ia = 0;
    ib = 1;
}

void pspAngularTrajectoryParticle::specificDraw(){
    
    if(!pts->empty()){
        for(int i=0; i<(pts->size() - 1); i++){
            int ia = i;
            int ib = i+1;
            int res = 256;
            
            glPushMatrix();
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<(res + 1); j++){
                double r = (*pts)[ia].r + ((*pts)[ib].r - (*pts)[ia].r)*(double)j/(double)res;
                double theta = (*pts)[ia].theta + ((*pts)[ib].theta - (*pts)[ia].theta)*(double)j/(double)res;
                double phi = (*pts)[ia].phi + ((*pts)[ib].phi - (*pts)[ia].phi)*(double)j/(double)res;
                double cx = (*pts)[ia].cx + ((*pts)[ib].cx - (*pts)[ia].cx)*(double)j/(double)res;
                double cy = (*pts)[ia].cy + ((*pts)[ib].cy - (*pts)[ia].cy)*(double)j/(double)res;
                double cz = (*pts)[ia].cz + ((*pts)[ib].cz - (*pts)[ia].cz)*(double)j/(double)res;
                
                double x, y, z;
                x = cx + r*sin(theta)*cos(phi);
                y = cy + r*sin(theta)*sin(phi);
                z = cz + r*cos(theta);
                //cout<<endl<<x<<" "<<y<<" "<<z;
                glVertex3f(x, y, z);
            }
            glEnd();
            glPopMatrix();
            
//            glPushMatrix();
//            glBegin(GL_LINE_STRIP);
//            glVertex3f((*pts)[i].x, (*pts)[i].y, (*pts)[i].z);
//            glVertex3f((*pts)[i + 1].x, (*pts)[i + 1].y, (*pts)[i + 1].z);
//            glEnd();
//            glPopMatrix();
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