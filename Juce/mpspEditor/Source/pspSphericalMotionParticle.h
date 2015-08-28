//
//  pspSphericalMotionParticle.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#ifndef __mpspEditor__pspSphericalMotionParticle__
#define __mpspEditor__pspSphericalMotionParticle__

#include <stdio.h>

#include <stdio.h>
#include <iostream>
#include "pspParticle.h"
#include "pspMathUtils.h"


class pspSphericalMotionTrajectoryParticle : public pspParticle{
public:
    
    pspSphericalMotionTrajectoryParticle();
    pspSphericalMotionTrajectoryParticle(pspParticleSystem* ms, int ind);
    ~pspSphericalMotionTrajectoryParticle();
    
    void specificSetup();
    void specificUpdate();
    void specificDraw();
    
    void resetTime();
    
    void setNumLoops(int nl);
    
protected:
    
    
private:
    
    double x, y, z;
    double r;
    double theta;
    double phi;
    double cx;
    double cy;
    double cz;
    double vTheta;
    double vPhi;
    double time;
    
    
    
    timedPosRad* startPosition;
    double currentTime;
    double startTime;
    double elapsedTime;
    int currentPoint;
    int ia, ib;
    
    int numLoops;
    int loopCounter;
    vector<timedSphericalMotionState>* pts;
    
};


#endif /* defined(__mpspEditor__pspSphericalMotionParticle__) */
