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

#include "pspConcertRoom.h"
#include <OpenGL/OpenGL.h>
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#include "pspParticleSystemsManager.h"

pspConcertRoom::pspConcertRoom(pspParticleSystemsManager* psm){
    myPsm = psm;
    setup();
}

pspConcertRoom::~pspConcertRoom(){
    if(speakers){
        while(!speakers->empty()){
            delete speakers->back();
            speakers->pop_back();
        }
        speakers->clear();
        delete speakers;
    }
    
    if(reverbGain){
        
        while(!reverbGain->empty()){
            delete reverbGain->back();
            reverbGain->pop_back();
        }
        
        reverbGain->clear();
        delete reverbGain;
    }
    
    if(reverbSize){
        
        while(!reverbSize->empty()){
            delete reverbSize->back();
            reverbSize->pop_back();
        }
        
        reverbSize->clear();
        delete reverbSize;
    }

    
    if(roomBounds){
        delete[] roomBounds;
    }
    
    if(soundLimits){
        delete[] soundLimits;
    }
    
    if(masterSpatConfig){
        delete masterSpatConfig;
    }
}

void pspConcertRoom::setup(){
    createColorsVec();
    currentColorIndex = 0;
    
    roomBounds = new double[3];
    soundLimits = new double[3];
    
    roomBounds[0] = 1.;
    roomBounds[1] = 1.;
    roomBounds[2] = 1.;
    
    roomWidth = roomBounds[0];
    roomHeight = roomBounds[1];
    roomLength = roomBounds[2];
    
    roomDiagonal = sqrt(roomBounds[0]*roomBounds[0] + roomBounds[1]*roomBounds[1] + roomBounds[2]*roomBounds[2]);
    planarDiagonal = sqrt(roomBounds[0]*roomBounds[0] + roomBounds[2]*roomBounds[2]);
    
    soundLimits[0] = 2.*SOUND_LIMIT*roomDiagonal/sqrt(2.) + roomBounds[0];
    soundLimits[1] = roomBounds[1];
    soundLimits[2] = 2.*SOUND_LIMIT*roomDiagonal/sqrt(2.) + roomBounds[2];
    
    speakers = new vector<Speaker*>();
    reverbGain = new vector<ofVec2f*>();
    reverbSize = new vector<ofVec2f*>();
    
    speakers->push_back(new Speaker(this, 1, -0.5, 0., -0.5));
    speakers->push_back(new Speaker(this, 2, 0.5, 0., -0.5));
    //speakers->push_back(new Speaker(this, 3, -0.5, 0., 0.5));
    numSpeakers = 2;
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->scalePosition(getBounds());
    }
    
    for(int i=0; i<10; i++){
        reverbGain->push_back(new ofVec2f(i, -120.));
        reverbSize->push_back(new ofVec2f(i, 0.));
    }
    
    masterSpatConfig = new pspSpatConfig(this, 0);
    
    //refreshSpatCalibGUIs();
}


void pspConcertRoom::setBounds(ofVec3f b){
    
    roomBounds[0] = b.x;
    roomBounds[1] = b.y;
    roomBounds[2] = b.z;
    
    roomWidth = roomBounds[0];
    roomHeight = roomBounds[1];
    roomLength = roomBounds[2];
    
    roomDiagonal = sqrt(roomBounds[0]*roomBounds[0] + roomBounds[1]*roomBounds[1] + roomBounds[2]*roomBounds[2]);
    planarDiagonal = sqrt(roomBounds[0]*roomBounds[0] + roomBounds[2]*roomBounds[2]);
    
    soundLimits[0] = 2.*SOUND_LIMIT*roomDiagonal/sqrt(2.) + roomBounds[0];
    soundLimits[1] = roomBounds[1];
    soundLimits[2] = 2.*SOUND_LIMIT*roomDiagonal/sqrt(2.) + roomBounds[2];
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->scalePosition(getBounds());
    }
    
}

void pspConcertRoom::setBounds(int c, double val){
    if(c == 1){
        setBounds(ofVec3f(val, roomBounds[1], roomBounds[2]));
    }
    else if(c == 2){
        setBounds(ofVec3f(roomBounds[0], val, roomBounds[2]));
    }
    else if(c == 3){
        setBounds(ofVec3f(roomBounds[0], roomBounds[1], val));
    }
    
    roomSizeChanged();
}


double* pspConcertRoom::getBounds(){
    if(roomBounds){
        return roomBounds;
    }
    return NULL;
}

double pspConcertRoom::getRoomDiagonal(){
    return roomDiagonal;
}
double pspConcertRoom::getPlanarDiagonal(){
    return planarDiagonal;
}

double* pspConcertRoom::getSoundLimits(){
    if(soundLimits){
        return soundLimits;
    }
    return NULL;
}


vector<Speaker*>* pspConcertRoom::getSpeakers(){
    return speakers;
}

std::string pspConcertRoom::getFilesPath(){
    return filesPath;
}

void pspConcertRoom::setFilesPath(std::string fp){
    filesPath = fp;
}


void pspConcertRoom::setNumSpeakers(int n){
    int ns = n;
    if(ns > numSpeakers){
        for(int i=0; i<(ns - numSpeakers); i++){
            speakers->push_back(new Speaker(this, i+1, 0., 0., 0.));
        }
        numSpeakers = speakers->size();
        //return;
    }
    else if(ns < numSpeakers && ns > 0){
        while(speakers->size() > ns){
            delete speakers->back();
            speakers->pop_back();
        }
        numSpeakers = speakers->size();
        //return;
    }
    
    numberSpeakersChanged();
}

int pspConcertRoom::getNumSpeakers(){
    return speakers->size();
}

void pspConcertRoom::setSpeakerPosition(int sp, ofVec3f pos){
    if(speakers->size() > 0){
        if(sp >= 1 && sp <= numSpeakers){
            (*speakers)[sp - 1]->setPosition(pos.x, pos.y, pos.z);
            (*speakers)[sp - 1]->scalePosition(getBounds());
        }
    }
}

void pspConcertRoom::setSpeakerPosition(int sp, int c, float val){
    if(speakers->size() > 0){
        if(sp >= 1 && sp <= numSpeakers){
            (*speakers)[sp - 1]->setPosition(c, val);
            (*speakers)[sp - 1]->scalePosition(getBounds());
        }
    }

}

void pspConcertRoom::setSpeakerCalibration(int sp){
    
    
}

void pspConcertRoom::setReverbGain(int sp){
    
}

vector<ofVec2f*>* pspConcertRoom::getReverbGain(){
    return reverbGain;
}

void pspConcertRoom::setReverbSize(int sp){
    
}

vector<ofVec2f*>* pspConcertRoom::getReverbSize(){
    return reverbSize;
}

pspSpatConfig* pspConcertRoom::getMasterSpatConfig(){
    return masterSpatConfig;
}

void pspConcertRoom::draw(){
    
    
    //draw speakers;
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->draw();
    }
    
    glLineWidth(1.);
    glPushMatrix();
    glColor4f(0.7, 0.7, 0.7, 0.8);
    glBegin(GL_LINES);
    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
        glVertex3f(i, -0.5*roomBounds[1] - 0.01, 2.5); glVertex3f(i, -0.5*roomBounds[1] - 0.01, -2.5);
        glVertex3f(2.5, -0.5*roomBounds[1] - 0.01, i); glVertex3f(-2.5, -0.5*roomBounds[1] - 0.01, i);
    }
    glEnd();
    glPopMatrix();
    
//    glLineWidth(1.);
//    glPushMatrix();
//    glColor4f(0.7, 0.7, 0.7, 1.);
//    glBegin(GL_QUADS);
//    glVertex3f(-2.*roomBounds[0], -0.5*roomBounds[1] - 0.001, 2.*roomBounds[2]);
//    glVertex3f(2.*roomBounds[0], -0.5*roomBounds[1] - 0.001, 2.*roomBounds[2]);
//    glVertex3f(2.*roomBounds[0], -0.5*roomBounds[1] - 0.001, -2.*roomBounds[2]);
//    glVertex3f(-2.*roomBounds[0], -0.5*roomBounds[1] - 0.001, -2.*roomBounds[2]);
//    glEnd();
//    glPopMatrix();
    
    glPushMatrix();
    glLineWidth(1.5);
    glColor3f(0., 0., 0.);
    
    //floor
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    
    //ceiling
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    
    //walls
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], 0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], 0.5*roomBounds[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(-0.5*roomBounds[0], 0.5*roomBounds[1], -0.5*roomBounds[2]);
    glVertex3f(-0.5*roomBounds[0], -0.5*roomBounds[1], -0.5*roomBounds[2]);
    glEnd();
    
    
    glPopMatrix();
    
    /*
    glPushMatrix();
    glLineWidth(2.);
    glColor3f(1., 1., 0.);
    glBegin(GL_LINES);
    glVertex3f(-0.5*soundLimits[0], -0.5*soundLimits[1], 0.5*soundLimits[2]);
    glVertex3f(0.5*soundLimits[0], -0.5*soundLimits[1], 0.5*soundLimits[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*soundLimits[0], -0.5*soundLimits[1], 0.5*soundLimits[2]);
    glVertex3f(0.5*soundLimits[0], -0.5*soundLimits[1], -0.5*soundLimits[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5*soundLimits[0], -0.5*soundLimits[1], -0.5*soundLimits[2]);
    glVertex3f(-0.5*soundLimits[0], -0.5*soundLimits[1], -0.5*soundLimits[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.5*soundLimits[0], -0.5*soundLimits[1], -0.5*soundLimits[2]);
    glVertex3f(-0.5*soundLimits[0], -0.5*soundLimits[1], 0.5*soundLimits[2]);
    glEnd();
    glPopMatrix();
    */
    
}


void pspConcertRoom::createColorsVec(){
    
    colorsVec.clear();
    
    colorsVec.push_back(ofVec4f(220, 20, 60, 255));
    colorsVec.push_back(ofVec4f(255, 110, 180, 255));
    colorsVec.push_back(ofVec4f(72, 118, 255, 255));
    colorsVec.push_back(ofVec4f(0, 238, 118, 255));
    colorsVec.push_back(ofVec4f(255, 236, 139, 255));
    colorsVec.push_back(ofVec4f(255, 153, 18, 255));
    colorsVec.push_back(ofVec4f(50, 205, 50, 255));
    colorsVec.push_back(ofVec4f(0, 255, 255, 255));
    colorsVec.push_back(ofVec4f(99, 184, 255, 255));
    colorsVec.push_back(ofVec4f(238, 0, 238, 255));
    colorsVec.push_back(ofVec4f(255, 127, 80, 255));
    colorsVec.push_back(ofVec4f(255, 0, 255, 255));
    colorsVec.push_back(ofVec4f(255, 255, 255, 255));
    colorsVec.push_back(ofVec4f(113, 198, 113, 255));
    colorsVec.push_back(ofVec4f(198, 113, 113, 255));
    colorsVec.push_back(ofVec4f(113, 113, 198, 255));
    colorsVec.push_back(ofVec4f(152, 251, 152, 255));
    colorsVec.push_back(ofVec4f(70, 130, 180, 255));
    colorsVec.push_back(ofVec4f(192, 255, 62, 255));
    colorsVec.push_back(ofVec4f(171, 130, 255, 255));
    
    for(int i=0; i<colorsVec.size(); i++){
        colorsVec[i] = colorsVec[i]/255.;
    }
    
}

ofVec4f pspConcertRoom::getNextColor(){
    currentColorIndex++;
    if(currentColorIndex >= colorsVec.size()){
        currentColorIndex = 0;
    }
    return colorsVec[currentColorIndex];
}


void pspConcertRoom::numberSpeakersChanged(){
    
    masterSpatConfig->numberSpeakersChanged();
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->getSpatConfig()->numberSpeakersChanged();
    }
    
    sendNumSpeakers();
}

void pspConcertRoom::roomSizeChanged(){
    masterSpatConfig->roomSizeChanged();
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->getSpatConfig()->roomSizeChanged();
    }
}

void pspConcertRoom::showSpeakerSpatCalibration(int sp){
    if(sp > 0 && sp <= numSpeakers){
        (*speakers)[sp - 1]->showSpatCalibration();
    }
}


pspSpatConfig* pspConcertRoom::getSpatConfig(int index){
    if(index > 0 && index <= speakers->size()){
        return (*speakers)[index - 1]->getSpatConfig();
    }
    return getMasterSpatConfig();
}



void pspConcertRoom::spatCalibChanged(pspSpatConfig* source){
    masterSpatConfig->spatCalibChanged(source);
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->getSpatConfig()->spatCalibChanged(source);
    }
}

void pspConcertRoom::createXmlElement(XmlElement* xml){
    XmlElement* spatCalib = new XmlElement("SpatConfig");
    
    masterSpatConfig->createXmlElement(spatCalib);
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->getSpatConfig()->createXmlElement(spatCalib);
    }
    
    xml->addChildElement(spatCalib);
}

void pspConcertRoom::loadSpatConfigFromXmlElement(XmlElement* xml){
    masterSpatConfig->loadXmlElement(xml);
    
    for(int i=0; i<speakers->size(); i++){
        (*speakers)[i]->getSpatConfig()->loadXmlElement(xml);
    }
}


void pspConcertRoom::sendNumSpeakers(){
    char buffer[OSC_MSG_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_MSG_BUFFER_SIZE);
    p << osc::BeginBundleImmediate;
    juce::String path = "/mpsp";
    p << osc::BeginMessage(path.getCharPointer());
    p << "numSpeakers";
    p << (int)speakers->size();
    p << osc::EndMessage;
    p << osc::EndBundle;
    myPsm->getOscSender()->Send(p.Data(), p.Size());
}

pspParticleSystemsManager* pspConcertRoom::getSystemsManager(){
    return myPsm;
}

