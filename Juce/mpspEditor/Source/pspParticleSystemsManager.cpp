//
//  pspParticleSystemsManager.cpp
//  psPsa-001
//
//  Created by Andre Perrotta on 22/11/14.
//
//

#include "pspParticleSystemsManager.h"

using namespace std;

pspParticleSystemsManager::pspParticleSystemsManager(){
    setup();
}

pspParticleSystemsManager::~pspParticleSystemsManager(){
    if(pSystems){
        while(!pSystems->empty()){
            delete pSystems->back();
            pSystems->pop_back();
        }
        delete pSystems;
    }
    
    if(pSystemsToDelete){
        clearDeleteQ();
        delete pSystemsToDelete;
    }
    
    
    if(concertRoom){
        delete concertRoom;
    }
    
    
    
    
}

void pspParticleSystemsManager::setup(){
    
    oscSender = new UdpTransmitSocket(IpEndpointName(DEFAULT_OSC_SENDER_IP, DEFAULT_OSC_SENDER_PORT));
    
    
    pSystems = new vector<pspParticleSystem*>();
    pSystemsToDelete = new vector<pspParticleSystem*>();
    concertRoom = new pspConcertRoom(this);
    
    createColorVector();
    
    /*
     for(int i=0; i<1; i++){
     addSystem("generic", 1);
     }
     for(int i=0; i<1; i++){
     addSystem("random", 1);
     }
     */
    
    //cout<<pSystems->size();
    
    transport = 0;
}
void pspParticleSystemsManager::update(){
    if(transport == 1){
        if(pSystems){
            for(int i=0; i<pSystems->size(); i++){
                (*pSystems)[i]->update();
            }
        }
    }
    
    moveSystemToDeleteQ();
    clearDeleteQ();
}
void pspParticleSystemsManager::draw(int flag){
    
    if(flag == 1){
        if(pSystems){
            for(int i=0; i<pSystems->size(); i++){
                (*pSystems)[i]->draw();
            }
        }
    }
    
    if(flag < 3){
        concertRoom->draw();
    }
    
    if(flag == 3){
        
    }
    
    if(numSystemsChanged){
        sendSystemsNames();
    }
    
}


int pspParticleSystemsManager::getNumSystems(){
    if(pSystems != NULL){
        return pSystems->size();
    }
    return 0;
}

int pspParticleSystemsManager::getNumActiveSystems(){
    if(pSystems != NULL){
        int counter = 0;
        for(int i=0; i<pSystems->size(); i++){
            if(!(*pSystems)[i]->getDeleteFlag()){
                counter++;
            }
        }
        return counter;
    }
    return 0;
}

void pspParticleSystemsManager::debug(){
    cout<<endl<<"works !";
}

vector<pspParticleSystem*>* pspParticleSystemsManager::getSystems(){
    if(pSystems != nullptr){
        return pSystems;
    }
    pSystems = new vector<pspParticleSystem*>();
    return pSystems;
    
}

String pspParticleSystemsManager::getSystemName(int s){
    if(pSystems != nullptr){
        if(pSystems->size() > s){
            return (*pSystems)[s]->getName();
        }
    }
    return "nosystemtocall";
}
String pspParticleSystemsManager::getSystemType(int s){
    if(pSystems != nullptr){
        if(pSystems->size() > s){
            return (*pSystems)[s]->getType();
        }
    }
    return "nosystemtocall";
}


void pspParticleSystemsManager::addSystem(juce::String type, int np){
    
    if(pSystems == nullptr){
        pSystems = new vector<pspParticleSystem*>();
    }
    
    int lastIndex = pSystems->size();
    
    if(type == "generic"){
        pSystems->push_back(new pspParticleSystem(this, String(lastIndex + 1), np));
    }
    else if(type == "random"){
        pSystems->push_back(new pspRandomSystem(this, String(lastIndex + 1), np));
    }
    else if(type == "linear"){
        pSystems->push_back(new pspLinearTrajectorySystem(this, String(lastIndex + 1), np));
    }
    else if(type == "angular"){
        pSystems->push_back(new pspAngularTrajectorySystem(this, String(lastIndex + 1), np));
    }
    else if(type == "sphericalMotion"){
        pSystems->push_back(new pspSphericalMotionTrajectorySystem(this, String(lastIndex + 1), np));
    }
    
    pSystems->back()->setColor(colors[colorIndex]);
    pSystems->back()->showGui();
    incrementColorIndex();
    
    numSystemsChanged = true;
}

void pspParticleSystemsManager::deleteSystem(int s){
    if(pSystems != nullptr){
        if(s >= 0 && s < pSystems->size()){
            (*pSystems)[s]->setDeleteFlag(true);
        }
    }
    
    
    
}

void pspParticleSystemsManager::moveSystemToDeleteQ(){
    if(pSystems){
        for(int i=0; i<pSystems->size(); i++){
            if((*pSystems)[i]->getDeleteFlag()){
                pSystemsToDelete->push_back((*pSystems)[i]);
                pSystems->erase(pSystems->begin() + i);
                numSystemsChanged = true;
            }
        }
    }
}

void pspParticleSystemsManager::clearDeleteQ(){
    if(pSystemsToDelete){
        while(!pSystemsToDelete->empty()){
            delete pSystemsToDelete->back();
            pSystemsToDelete->pop_back();
        }
        //delete pSystemsToDelete;
    }
    
    
    
    
}

pspParticleSystem* pspParticleSystemsManager::getSystem(int s){
    if(pSystems != nullptr){
        if(pSystems->size() > s){
            return (*pSystems)[s];
        }
        return (*pSystems->begin());
    }
    pSystems = new vector<pspParticleSystem*>();
    return (*pSystems->begin());
}

pspConcertRoom* pspParticleSystemsManager::getConcertRoom(){
    return concertRoom;
}

int pspParticleSystemsManager::getNumSpeakers(){
    if(concertRoom != nullptr){
        return concertRoom->getNumSpeakers();
    }
    return 0;
}


void pspParticleSystemsManager::createColorVector(){
    colorIndex = 0;
    
    colors.push_back(Colours::tomato);
    colors.push_back(Colours::forestgreen);
    colors.push_back(Colours::coral);
    colors.push_back(Colours::cornflowerblue);
    colors.push_back(Colours::darkred);
    colors.push_back(Colours::firebrick);
    colors.push_back(Colours::khaki);
    colors.push_back(Colours::lemonchiffon);
    colors.push_back(Colours::lightsalmon);
    colors.push_back(Colours::lightyellow);
    colors.push_back(Colours::maroon);
    colors.push_back(Colours::darkseagreen);
    colors.push_back(Colours::darkblue);
    colors.push_back(Colours::lavenderblush);
    colors.push_back(Colours::mediumaquamarine);
    colors.push_back(Colours::plum);
    colors.push_back(Colours::springgreen);
    colors.push_back(Colours::sienna);
    colors.push_back(Colours::seashell);
    colors.push_back(Colours::steelblue);
    colors.push_back(Colours::aliceblue);
}

void pspParticleSystemsManager::incrementColorIndex(){
    colorIndex++;
    if(colorIndex >= colors.size()){
        colorIndex = 0;
    }
}

void pspParticleSystemsManager::showPsystemGui(int ps){
    if(pSystems != nullptr){
        if(ps >= 0 && ps < pSystems->size()){
            (*pSystems)[ps]->showGui();
        }
    }
}

UdpTransmitSocket* pspParticleSystemsManager::getOscSender(){
    if(oscSender != nullptr){
        return oscSender;
    }
    
    return new UdpTransmitSocket(IpEndpointName(DEFAULT_OSC_SENDER_IP, DEFAULT_OSC_SENDER_PORT));
}

void pspParticleSystemsManager::sendSystemsNames(){
    if(pSystems){
        if(pSystems->size() > 0){
            char buffer[OSC_MSG_BUFFER_SIZE * pSystems->size()];
            osc::OutboundPacketStream p(buffer, OSC_MSG_BUFFER_SIZE * pSystems->size());
            p << osc::BeginBundleImmediate;
            juce::String path = "/mpsp";
            p << osc::BeginMessage(path.getCharPointer());
            p << "pSystems";
            for(int i=0; i<pSystems->size(); i++){
                p << (*pSystems)[i]->getFullName().getCharPointer();
            }
            p << osc::EndMessage;
            p << osc::EndBundle;
            oscSender->Send(p.Data(), p.Size());
        }
        else {
            char buffer[OSC_MSG_BUFFER_SIZE];
            osc::OutboundPacketStream p(buffer, OSC_MSG_BUFFER_SIZE);
            p << osc::BeginBundleImmediate;
            juce::String path = "/mpsp";
            p << osc::BeginMessage(path.getCharPointer());
            p << "pSystems";
            p << "empty";
            p << osc::EndMessage;
            p << osc::EndBundle;
            oscSender->Send(p.Data(), p.Size());
        }
    }
    numSystemsChanged = false;
    
}

void pspParticleSystemsManager::sendTransportOSC(int t){
    char buffer[OSC_MSG_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_MSG_BUFFER_SIZE);
    p << osc::BeginBundleImmediate;
    juce::String path = "/mpsp";
    p << osc::BeginMessage(path.getCharPointer());
    p << "transport";
    if(t == 0){
        p << "stop";
    }
    else if(t == 1){
        p << "play";
    }
    p << osc::EndMessage;
    p << osc::EndBundle;
    oscSender->Send(p.Data(), p.Size());
}


void pspParticleSystemsManager::keyPressed(const juce::KeyPress &key){
    //cout<<" "<<key.getKeyCode();
    if(key.getKeyCode() == 32){
        if(transport == 1){
            transport = 0;
        }
        else{
            resetTime();
            transport = 1;
        }
        sendTransportOSC(transport);
    }
}

void pspParticleSystemsManager::resetTime(){
    for(int i=0; i<pSystems->size(); i++){
        (*pSystems)[i]->resetTime();
    }
}

void pspParticleSystemsManager::setTransport(String t){
    if(t == "play"){
        transport = 1;
    }
    else if(t == "stop"){
        transport = 0;
    }
}
