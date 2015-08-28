/*
 Copyright (c) 2014, André V. Perrotta
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the developer nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY ANDRÉ V. PERROTTA ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL ANDRÉ V. PERROTTA BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 This file is part of psPlayground.
 
 psPlayground is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 psPlayground is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with psPlayground.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __psPlayground__Speaker__
#define __psPlayground__Speaker__

#include <iostream>
#include "ofVectorMath.h"
#include "pspMathUtils.h"
#include "pspSpatConfig.h"

class pspConcertRoom;

class Speaker{
public:
    
    Speaker(pspConcertRoom* cr);
    Speaker(pspConcertRoom* cr, int n, double x_, double y_, double z_);
    Speaker(pspConcertRoom* cr, int n, ofVec3f pos_);
    ~Speaker();
    
    void setup(pspConcertRoom* cr);
    void setup(pspConcertRoom* cr, int n, double x_, double y_, double z_);
    void setup(pspConcertRoom* cr, int n, ofVec3f pos_);
    void update();
    void draw();
    
    void setPosition(double x_, double y_, double z_);
    void setPosition(ofVec3f pos_);
    void setPosition(int c, double val);
    
    void scalePosition(double* rs);
    
    double x, y, z;
    double r, theta, phi;
    
    bool active;
    
    ofVec3f pos;
    ofVec3f scaledPos;
    
    int num;
    
    double width;
    double height;
    
    void setSize(double w, double h);
    
    void setSpeakerCalibration(int sp);
    vector<ofVec2f*>* speakerCalibration;
    vector<ofVec2f*>* getSpeakerCalibration();
    
    pspSpatConfig* spatConfig;
    pspSpatConfig* getSpatConfig();
    void showSpatCalibration();
    pspConcertRoom* myCr;
    
    void setupSpatConfig();
    
    double getAmp(ofVec3f trajectoryPos);
};


#endif /* defined(__concertSpat__Speaker__) */
