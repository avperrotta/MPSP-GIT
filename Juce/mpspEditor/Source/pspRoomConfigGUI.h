//
//  pspRoomConfigGUI.h
//  psPsa-001
//
//  Created by Andre Perrotta on 01/12/14.
//
//

#ifndef __psPsa_001__pspRoomConfigGUI__
#define __psPsa_001__pspRoomConfigGUI__
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

#include "pspParticleSystemsManager.h"
#include "pspConcertRoom.h"



class pspSpatConfigGUIWindow;
class numSpeakersSlider;

class pspRoomConfigGUI : public Component{
    
public:
    
    pspRoomConfigGUI(),
    pspRoomConfigGUI(pspParticleSystemsManager* psm);
    ~pspRoomConfigGUI();
    
    void setup();
    void paint (Graphics& g) override;
    void resized() override;
    
    pspConcertRoom* getConcertRoom();
    void setNumSpeakers(int n);
    void setSpeakerPosition(Slider* s);
    void setRoomSize(Slider* s);
    
    
    bool loadXmlRoomConfig(File xmlFile);
    bool saveXmlRoomConfig(File xmlFile);
    
    void showMasterSpatCalibration();
    void showSpatCalibration(int sp);
    
private:
    
    pspParticleSystemsManager* myPsm;
    pspConcertRoom* myCr;
    PropertyPanel panel;
    numSpeakersSlider* nss;
    Array<PropertyComponent*> comps;
    Array<PropertyComponent*> roomDimensions;
    Array<PropertyComponent*> speakersPosition;
    vector<Array<PropertyComponent*> > speakersPositionAux;
    
    int numSpeakers;
    
    void createWidgets();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspRoomConfigGUI);
    
};



class numSpeakersSlider : public SliderPropertyComponent{
public:
    numSpeakersSlider(const String& propertyName, pspRoomConfigGUI* mg):SliderPropertyComponent(propertyName, 1, 2000, 1)
    {
        
        myPsprc = mg;
        
        setValue(myPsprc->getConcertRoom()->getNumSpeakers());
        slider.setSliderStyle(Slider::IncDecButtons);
        slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
        slider.setIncDecButtonsMode (Slider::incDecButtonsDraggable_Vertical);
    }
    
    void setValue (double newValue) override
    {
        slider.setValue (newValue);
        //cout<<endl<<"slider value = "<<newValue;
    }
    
    void sliderValueChanged(Slider* slider) override{
        myPsprc->setNumSpeakers(slider->getValue());
    }
    
    void sliderDragStarted(Slider*) override{
        
    }
    
    void sliderDragEnded(Slider*) override{
        
    }
    
private:
    
    pspRoomConfigGUI* myPsprc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (numSpeakersSlider)
};

class roomConfigSlider : public SliderPropertyComponent{
public:
    roomConfigSlider(const String& propertyName, pspRoomConfigGUI* mg):SliderPropertyComponent(propertyName, 0., 1., 0.01)
    {
        
        myPsprc = mg;
        
        setValue(1.);
    }
    
    void setValue (double newValue) override
    {
        slider.setValue (newValue);
    }
    
    void sliderValueChanged(Slider* slider) override{
        myPsprc->setRoomSize(slider);
    }
    
    void sliderDragStarted(Slider*) override{
        
    }
    
    void sliderDragEnded(Slider*) override{
        
    }
    
    
private:
    
    pspRoomConfigGUI* myPsprc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (roomConfigSlider);
};

class speakerPositionSlider : public SliderPropertyComponent{
public:
    speakerPositionSlider(const String& propertyName, pspRoomConfigGUI* mg):SliderPropertyComponent(propertyName, -0.5, 0.5, 0.005)
    {
        
        myPsprc = mg;
        
        setValue(0.);
    }
    
    speakerPositionSlider(const String& propertyName, pspRoomConfigGUI* mg, double val):SliderPropertyComponent(propertyName, -0.5, 0.5, 0.005)
    {
        
        myPsprc = mg;
        
        setValue(val);
    }
    
    
    void setValue (double newValue) override
    {
        slider.setValue (newValue);
    }
    
    void sliderValueChanged(Slider* slider) override{
        myPsprc->setSpeakerPosition(slider);
    }
    
    void sliderDragStarted(Slider*) override{
        
    }
    
    void sliderDragEnded(Slider*) override{
        
    }
    
private:
    
    pspRoomConfigGUI* myPsprc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (speakerPositionSlider);
};



class loadSpeakersSetupButton : public ButtonPropertyComponent
{
public:
    loadSpeakersSetupButton (const String& propertyName, pspRoomConfigGUI* mg):ButtonPropertyComponent(propertyName, true){
        
        myPsprc = mg;
        
    }
    
    void buttonClicked() override{
        
        FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
        
        if (fc.browseForFileToOpen())
        {
            myPsprc->loadXmlRoomConfig(fc.getResult());
        }
        
        //cout<<endl<<"load speakers clicked";
    }
    
    String getButtonText() const override
    {
        return "load";
    }
    
private:
    
    pspRoomConfigGUI* myPsprc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (loadSpeakersSetupButton);
};

class saveSpeakersSetupButton : public ButtonPropertyComponent
{
public:
    saveSpeakersSetupButton (const String& propertyName, pspRoomConfigGUI* mg):ButtonPropertyComponent(propertyName, true){
        myPsprc = mg;
    }
    
    void buttonClicked() override{
        FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
        
        if (fc.browseForFileToSave(true))
        {
            myPsprc->saveXmlRoomConfig(fc.getResult());
        }
    }
    
    String getButtonText() const override
    {
        return "save";
    }
    
private:
    
    pspRoomConfigGUI* myPsprc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (saveSpeakersSetupButton);
};

class spatMasterCalibrationSetupButton : public ButtonPropertyComponent
{
public:
    spatMasterCalibrationSetupButton (const String& propertyName, pspRoomConfigGUI* cw):ButtonPropertyComponent(propertyName, true){
        myGui = cw;
    }
    
    void buttonClicked() override{
        myGui->showMasterSpatCalibration();
        
    }
    
    String getButtonText() const override
    {
        return "calibrate";
    }
    
private:
    
    pspRoomConfigGUI* myGui;
    String myLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatMasterCalibrationSetupButton);
};

class spatCalibrationSetupButton : public ButtonPropertyComponent
{
public:
    spatCalibrationSetupButton (const String& propertyName, pspRoomConfigGUI* cw, int sp):ButtonPropertyComponent(propertyName, true){
        myGui = cw;
        speaker = sp;
    }
    
    void buttonClicked() override{
        myGui->showSpatCalibration(speaker);
    }
    
    String getButtonText() const override
    {
        return "calibrate";
    }
    
private:
    
    pspRoomConfigGUI* myGui;
    String myLabel;
    int speaker;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatCalibrationSetupButton);
};


#endif /* defined(__psPsa_001__pspRoomConfigGUI__) */
