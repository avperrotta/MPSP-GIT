//
//  pspSpatConfig.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 17/08/15.
//
//

#include "pspSpatConfig.h"
#include "pspConcertRoom.h"

pspSpatConfig::pspSpatConfig(pspConcertRoom* cr, int si){
    myCr = cr;
    
    speakerID = si;
    
    if(speakerID == 0){
        name = "Master";
    }
    else{
        name = "Speaker:" + String(speakerID);
    }
    
    copyFromIndex = 1;
    
    spatTypeLabel = "DBAP";
    spatType = 1;
    dbap = new pspDBAP();
    
    myGui = new pspSpatConfigGUI(this);
    
}

pspSpatConfig::~pspSpatConfig(){

    delete dbap;
    //delete myGuiWindow;
    delete myGui;
}



void pspSpatConfig::showGui(){
    myGui->showGui();
}

void pspSpatConfig::setSpatType(int st){
    spatType = st;
    switch(spatType){
        case 1:
            spatTypeLabel = "DBAP";
            break;
        case 2:
            spatTypeLabel = "VBAP";
            break;
    }
    //cout<<endl<<spatTypeLabel<<" "<<spatType;
}

void pspSpatConfig::copyConfigFrom(int cid){
    
    if(cid > 0){
        if((cid - 1) != speakerID){
            //cout<<endl<<"link params from: "<<(cid - 1)<<" to: "<<speakerID;
            dbap->copyParams(myCr->getSpatConfig(cid - 1)->getDBAP());
        }
    }
}

pspConcertRoom* pspSpatConfig::getConcertRoom(){
    return myCr;
}

int pspSpatConfig::getSpeakerID(){
    return speakerID;
}



pspDBAP* pspSpatConfig::getDBAP(){
    return dbap;
}

String pspSpatConfig::getName(){
    return name;
}

void pspSpatConfig::spatCalibChanged(pspSpatConfig* source){
    if(source->getSpeakerID() != speakerID){
        if(copyFromIndex == (source->getSpeakerID() + 1)){
            copyConfigFrom(source->getSpeakerID() + 1);
        }
    }
    myGui->setDbapGuiParamsFromModel();
}

void pspSpatConfig::numberSpeakersChanged(){
    if(speakerID > 0){
        myGui->numberSpeakersChanged();
    }
}

void pspSpatConfig::setCopyFromIndex(int sourceIndex){
    copyFromIndex = sourceIndex;
    copyConfigFrom(sourceIndex);
    myGui->setDbapGuiParamsFromModel();
}
int pspSpatConfig::getCopyFromIndex(){
    return copyFromIndex;
}

void pspSpatConfig::createXmlElement(XmlElement* xml){
    
    XmlElement* spatConfig = new XmlElement(name);
    
    spatConfig->setAttribute("copyParametersFrom", copyFromIndex);
    spatConfig->setAttribute("spatAlgorithmChoice", spatType);
    spatConfig->setAttribute("dbapDecayProfile", dbap->getDecayProfile());
    
    for(int i=0; i<dbap->getLinearParams()->size(); i++){
        spatConfig->setAttribute("dbapLinearParams" + String(i), (*dbap->getLinearParams())[i]);
    }
    for(int i=0; i<dbap->getExpParams()->size(); i++){
        spatConfig->setAttribute("dbapExpParams" + String(i), (*dbap->getExpParams())[i]);
    }
    for(int i=0; i<dbap->getSineParams()->size(); i++){
        spatConfig->setAttribute("dbapSineParams" + String(i), (*dbap->getSineParams())[i]);
    }
    
    xml->addChildElement(spatConfig);
}

void pspSpatConfig::saveXml(File xmlFile){
    XmlElement* mainElement = new XmlElement("SpatConfig");
    
    createXmlElement(mainElement);
    
    mainElement->writeToFile(xmlFile, "");
    //speakersElement->deleteAllChildElements();
    mainElement->deleteAllChildElements();
    delete mainElement;
}

bool pspSpatConfig::loadXml(File xmlFile){
    XmlDocument myDocument (xmlFile);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if(!mainElement->hasTagName("SpatConfig")){
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain room config data", "", "OK");
        //delete mainElement;
        return false;
    }
    else{
        if(XmlElement* myConfig = mainElement->getChildElement(0)){
            getAttributesFromElement(myConfig);
        }
    }
    
    delete mainElement;
    return true;
}

bool pspSpatConfig::loadXmlElement(XmlElement* xml){
    if(!xml->hasTagName("SpatConfig")){
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain room config data", "", "OK");
        //delete mainElement;
        return false;
    }
    else{
        if(XmlElement* myConfig = xml->getChildByName(name)){
            getAttributesFromElement(myConfig);
        }
    }
    
    return true;
}

void pspSpatConfig::getAttributesFromElement(XmlElement* xml){
    dbap->setDecayProfile(xml->getIntAttribute("dbapDecayProfile"));
    for(int i=0; i<dbap->getLinearParams()->size(); i++){
        dbap->setLinearParams(i, xml->getDoubleAttribute("dbapLinearParams" + String(i)));
    }
    for(int i=0; i<dbap->getExpParams()->size(); i++){
        dbap->setExpParams(i, xml->getDoubleAttribute("dbapExpParams" + String(i)));
    }
    for(int i=0; i<dbap->getSineParams()->size(); i++){
        dbap->setSineParams(i, xml->getDoubleAttribute("dbapSineParams" + String(i)));
    }
    //cout<<myConfig->getIntAttribute("copyParametersFrom");
    myGui->setFromXml(xml->getIntAttribute("copyParametersFrom"), xml->getIntAttribute("spatAlgorithmChoice"));
    myGui->setDbapGuiParamsFromModel();
}

void pspSpatConfig::roomSizeChanged(){
    myGui->setDbapGuiParamsFromModel();
}



