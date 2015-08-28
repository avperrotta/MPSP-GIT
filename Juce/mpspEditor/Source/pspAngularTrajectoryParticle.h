//
//  pspAngularTrajectoryParticle.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#ifndef __mpspEditor__pspAngularTrajectoryParticle__
#define __mpspEditor__pspAngularTrajectoryParticle__

#include <stdio.h>

#include <stdio.h>
#include <iostream>
#include "pspParticle.h"
#include "pspMathUtils.h"


class pspAngularTrajectoryParticle : public pspParticle{
public:
    
    pspAngularTrajectoryParticle();
    pspAngularTrajectoryParticle(pspParticleSystem* ms, int ind);
    ~pspAngularTrajectoryParticle();
    
    void specificSetup();
    void specificUpdate();
    void specificDraw();
    
    void resetTime();
    
    void setNumLoops(int nl);
    
protected:
    
    
private:
    
    double currentTime;
    double startTime;
    double elapsedTime;
    int currentPoint;
    int ia, ib;
    
    int numLoops;
    int loopCounter;
    vector<timedPosRad>* pts;
    
};


#endif /* defined(__mpspEditor__pspAngularTrajectoryParticle__) */
