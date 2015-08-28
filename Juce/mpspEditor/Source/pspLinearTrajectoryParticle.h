//
//  pspLinearTrajectoryParticle.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 26/08/15.
//
//

#ifndef __mpspEditor__pspLinearTrajectoryParticle__
#define __mpspEditor__pspLinearTrajectoryParticle__

#include <stdio.h>
#include <iostream>
#include "pspParticle.h"
#include "pspMathUtils.h"


class pspLinearTrajectoryParticle : public pspParticle{
public:
    
    pspLinearTrajectoryParticle();
    pspLinearTrajectoryParticle(pspParticleSystem* ms, int ind);
    ~pspLinearTrajectoryParticle();
    
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
    vector<timedPos>* pts;

};


#endif /* defined(__mpspEditor__pspLinearTrajectoryParticle__) */
