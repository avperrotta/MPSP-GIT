//
//  pspSpatConfig.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 17/08/15.
//
//

#ifndef __mpspEditor__pspSpatConfig__
#define __mpspEditor__pspSpatConfig__

#include <stdio.h>
#include <iostream>
#include "ofVectorMath.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "pspSpatConfigGUI.h"
#include "pspDBAP.h"

using namespace std;
class pspConcertRoom;

class pspSpatConfig {
    
public:
    
    //pspSpatConfig();
    pspSpatConfig(pspConcertRoom* cr, int si);
    ~pspSpatConfig();
    
    void setSpatType(int st);
    
    void copyConfigFrom(int cid);
    
    pspConcertRoom* getConcertRoom();
    int getSpeakerID();
    
    pspDBAP* getDBAP();
    
    //GUI
    void showGui();
    void numberSpeakersChanged();
    void spatCalibChanged(pspSpatConfig* source);
    void roomSizeChanged();
    
    String getName();
    
    void setCopyFromIndex(int sourceIndex);
    int getCopyFromIndex();
    
    
    void createXmlElement(XmlElement* xml);
    bool loadXmlElement(XmlElement* xml);
    void saveXml(File xmlFile);
    bool loadXml(File xmlFile);
    void getAttributesFromElement(XmlElement* xml);
    
    
private:
    
    int speakerID;
    
    pspConcertRoom* myCr;
    
    String name;
    String spatTypeLabel;
    int spatType;
    
    int decayProfile;
    String decayProfileLabel;
    
    pspDBAP* dbap;
    
    pspSpatConfigGUI* myGui;
    
    int copyFromIndex;
    
    
};

#endif /* defined(__mpspEditor__pspSpatConfig__) */
