//
//  pspRoomConfigGUI.cpp
//  psPsa-001
//
//  Created by Andre Perrotta on 01/12/14.
//
//

#include "pspRoomConfigGUI.h"
#include "juce_PropertyPanel.h"
#include "pspSpatConfigGUI.h"

//==========================================
//main GUI
pspRoomConfigGUI::pspRoomConfigGUI(){
    pspRoomConfigGUI(NULL);
}


pspRoomConfigGUI::pspRoomConfigGUI(pspParticleSystemsManager* psm){
    myPsm = psm;
    if(myPsm != NULL){
        myCr = psm->getConcertRoom();
        numSpeakers = myCr->getNumSpeakers();
    }
    
    setup();
    
}

pspRoomConfigGUI::~pspRoomConfigGUI(){
    comps.clear();
}

void pspRoomConfigGUI::setup(){
    
    setOpaque(true);
    setBounds(0, 0, 180, 100);
    //panel.setOpaque(true);
    addAndMakeVisible(panel);
    createWidgets();
    
    
}

void pspRoomConfigGUI::paint (Graphics& g){
    
    g.fillAll(Colour::greyLevel (0.65f));

}

void pspRoomConfigGUI::resized(){
    
    panel.setBounds(getLocalBounds().reduced(4));
}

void pspRoomConfigGUI::createWidgets(){
    
    comps.clear();
    comps.add(new loadSpeakersSetupButton("speakers setup", this));
    comps.add(new saveSpeakersSetupButton("speakers setup", this));
    nss = new numSpeakersSlider("num speakers ", this);
    comps.add(nss);
    panel.addSection("Config", comps);
    
    
    roomDimensions.clear();
    roomDimensions.add(new roomConfigSlider("width", this));
    roomDimensions.add(new roomConfigSlider("length", this));
    roomDimensions.add(new roomConfigSlider("height", this));
    panel.addSection("Room size", roomDimensions);
    
    comps.clear();
    comps.add(new spatMasterCalibrationSetupButton(" ", this));
    panel.addSection("Master Spat setup", comps);
    
    
    speakersPositionAux.clear();
    for(int i=0; i<myCr->getNumSpeakers(); i++){
        speakersPosition.clear();
        speakersPosition.add(new speakerPositionSlider("x", this));
        speakersPosition[0]->setComponentID("xSpeaker:" + String(i+1));
        speakersPosition.add(new speakerPositionSlider("y", this));
        speakersPosition[1]->setComponentID("ySpeaker:" + String(i+1));
        speakersPosition.add(new speakerPositionSlider("z", this));
        speakersPosition[2]->setComponentID("zSpeaker:" + String(i+1));
        speakersPosition.add(new spatCalibrationSetupButton(" ", this, i+1));
        speakersPositionAux.push_back(speakersPosition);
        panel.addSection("Speaker: " + String(i + 1), speakersPosition);
    }
    //cout<<endl<<"panel size = "<<panel.getNumSections();
    
}

pspConcertRoom* pspRoomConfigGUI::getConcertRoom(){
    return myCr;
}

void pspRoomConfigGUI::setNumSpeakers(int n){
    
    int currentNumSpeakers = myCr->getNumSpeakers();
    
    
    if(n > currentNumSpeakers){
        
        for(int i=currentNumSpeakers; i<n; i++){
            speakersPosition.clear();
            speakersPosition.add(new speakerPositionSlider("x", this));
            speakersPosition[0]->setComponentID("xSpeaker:" + String(i+1));
            speakersPosition.add(new speakerPositionSlider("y", this));
            speakersPosition[1]->setComponentID("ySpeaker:" + String(i+1));
            speakersPosition.add(new speakerPositionSlider("z", this));
            speakersPosition[2]->setComponentID("zSpeaker:" + String(i+1));
            speakersPosition.add(new spatCalibrationSetupButton(" ", this, i+1));
            speakersPositionAux.push_back(speakersPosition);
            panel.addSection("Speaker: " + String(i + 1), speakersPosition);
        }
    }
    else if(n < currentNumSpeakers){
        //cout<<endl<<"start: "<<panel.getSectionNames().size()<<" "<<currentNumSpeakers;
        for(int i=currentNumSpeakers; i>n; i--){
            //cout<<endl<<"removing section: "<<(i+1);
            panel.removeSection(i+2);
            speakersPositionAux.pop_back();
            
        }
        //cout<<endl<<"end: "<<panel.getSectionNames().size();
        
    }
    myCr->setNumSpeakers(n);
}


void pspRoomConfigGUI::setSpeakerPosition(Slider* s){
    
    int speakerIndex;
    String aux;
    aux = s->getParentComponent()->getParentComponent()->getName();
    speakerIndex = aux.substring(aux.lastIndexOf(":") + 2).getIntValue();
    
    int coor;
    
    if(s->getParentComponent()->getName() == "x"){
        coor = 1;
    }
    else if(s->getParentComponent()->getName() == "y"){
        coor = 2;
    }
    else if(s->getParentComponent()->getName() == "z"){
        coor = 3;
    }
    
    myCr->setSpeakerPosition(speakerIndex, coor, s->getValue());
}

void pspRoomConfigGUI::setRoomSize(Slider* s){
    
    int coor;
    
    if(s->getParentComponent()->getName() == "width"){
        coor = 1;
    }
    else if(s->getParentComponent()->getName() == "height"){
        coor = 2;
    }
    else if(s->getParentComponent()->getName() == "length"){
        coor = 3;
    }
    
    myCr->setBounds(coor, s->getValue());
}

bool pspRoomConfigGUI::loadXmlRoomConfig(File xmlFile){
    XmlDocument myDocument (xmlFile);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "error loading room config xml file !", "", "OK");
        delete mainElement;
        return false;
    }
    else{
        if(!mainElement->hasTagName("RoomConfig")){
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain room config data", "", "OK");
            delete mainElement;
            return false;
        }
        else{
            XmlElement* roomSize = mainElement->getChildByName("roomSize");
            if(roomSize){
                static_cast<roomConfigSlider*>(roomDimensions[0])->setValue(roomSize->getDoubleAttribute("width"));
                static_cast<roomConfigSlider*>(roomDimensions[1])->setValue(roomSize->getDoubleAttribute("length"));
                static_cast<roomConfigSlider*>(roomDimensions[2])->setValue(roomSize->getDoubleAttribute("height"));
            }
            
            XmlElement* speakers = mainElement->getChildByName("speakers");
            if(speakers != nullptr){
                int ns = speakers->getNumChildElements();
                if(ns > 0){
                    setNumSpeakers(ns);
                    nss->setValue(ns);
                    for(int i=0; i<ns; i++){
                        static_cast<speakerPositionSlider*>(speakersPositionAux[i][0])->setValue(speakers->getChildElement(i)->getDoubleAttribute("x"));
                        static_cast<speakerPositionSlider*>(speakersPositionAux[i][1])->setValue(speakers->getChildElement(i)->getDoubleAttribute("y"));
                        static_cast<speakerPositionSlider*>(speakersPositionAux[i][2])->setValue(speakers->getChildElement(i)->getDoubleAttribute("z"));
                    }
                }
                
            }
            
            
            //delete speakers;
        }
        
    }
    if(XmlElement* spatConfig = mainElement->getChildByName("SpatConfig")){
        myCr->loadSpatConfigFromXmlElement(spatConfig);
    }
    
    
    delete mainElement;
    return true;
}


bool pspRoomConfigGUI::saveXmlRoomConfig(File xmlFile){
    
    XmlElement* mainElement = new XmlElement("RoomConfig");
    
    XmlElement* roomSizeElement = new XmlElement("roomSize");
    roomSizeElement->setAttribute("width", static_cast<roomConfigSlider*>(roomDimensions[0])->getValue());
    roomSizeElement->setAttribute("length", static_cast<roomConfigSlider*>(roomDimensions[1])->getValue());
    roomSizeElement->setAttribute("height", static_cast<roomConfigSlider*>(roomDimensions[2])->getValue());
    mainElement->addChildElement(roomSizeElement);
    
    
    XmlElement* speakersElement = new XmlElement("speakers");
    
    for(int i=0; i<speakersPositionAux.size(); i++){
        XmlElement* speakerPositionElement = new XmlElement("speaker");
        speakerPositionElement->setAttribute("number", i+1);
        speakerPositionElement->setAttribute("x", static_cast<speakerPositionSlider*>(speakersPositionAux[i][0])->getValue());
        speakerPositionElement->setAttribute("y", static_cast<speakerPositionSlider*>(speakersPositionAux[i][1])->getValue());
        speakerPositionElement->setAttribute("z", static_cast<speakerPositionSlider*>(speakersPositionAux[i][2])->getValue());
        speakersElement->addChildElement(speakerPositionElement);
    }
    mainElement->addChildElement(speakersElement);
    
    myCr->createXmlElement(mainElement);
    
    mainElement->writeToFile(xmlFile, "");
    
    //speakersElement->deleteAllChildElements();
    mainElement->deleteAllChildElements();
    delete mainElement;
    
}


void pspRoomConfigGUI::showMasterSpatCalibration(){
    myCr->getMasterSpatConfig()->showGui();
}

void pspRoomConfigGUI::showSpatCalibration(int sp){
    myCr->showSpeakerSpatCalibration(sp);
}

//=============================================
