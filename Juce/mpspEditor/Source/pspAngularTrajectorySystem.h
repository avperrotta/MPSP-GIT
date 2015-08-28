//
//  pspAngularTrajectorySystem.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#ifndef __mpspEditor__pspAngularTrajectorySystem__
#define __mpspEditor__pspAngularTrajectorySystem__

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "pspParticleSystem.h"
#include "pspMathUtils.h"
#include "pspAngularTrajectoryParticle.h"
#include "pspParticleSystemGUI.h"

using namespace std;

class pspParticleSystemsManager;

class pspAngularTrajectorySystem : public pspParticleSystem{
    
public:
    
    pspAngularTrajectorySystem(pspParticleSystemsManager* sm, String n, int np);
    ~pspAngularTrajectorySystem();
    
    void specificSetup();
    void specificUpdate();
    void drawParticles();
    void drawSystem();
    
    void changeNumPts(int np);
    vector<timedPosRad>* getPts();
    
    void saveXml(File xml);
    void loadXml(File xml);
    
    void setNumLoops(int nl);
    int getNumLoops();
    
protected:
    
    vector<timedPosRad>* pts;
    double randomness;
    
    
    void addParticleSpecific(int np);
    
private:
    
    void addPt();
    void removePt();
    int numLoops;
};


//GUI
//===========================================================

class pspAngularNumPtsSlider : public SliderPropertyComponent{
public:
    pspAngularNumPtsSlider(const String& propertyName, pspParticleSystem* ps);
    
    ~pspAngularNumPtsSlider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspAngularNumPtsSlider);
};


class pspAngularTrajectoryTimedPosSLider : public SliderPropertyComponent{
public:
    pspAngularTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex);
    
    ~pspAngularTrajectoryTimedPosSLider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    int myPtIndex;
    int myCoord;
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspAngularTrajectoryTimedPosSLider);
};


class pspAngularTrajectoryNumLoops : public SliderPropertyComponent{
public:
    pspAngularTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps);
    
    ~pspAngularTrajectoryNumLoops();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspAngularTrajectoryNumLoops);
};


class pspAngularTrajectorySystemSpecificGUI : public Component{
public:
    
    pspAngularTrajectorySystemSpecificGUI(pspParticleSystem* ps);
    ~pspAngularTrajectorySystemSpecificGUI();
    
    void paint(Graphics& g)override;
    void resized()override;
    
    void createWidgets();
    
    void changeNumPts();
    
private:
    
    pspParticleSystem* mySystem;
    PropertyPanel* panel;
    Array<PropertyComponent*> comps;
    Array<PropertyComponent*> ptsGUI;
    vector<timedPosRad>* pts;
    
    pspAngularTrajectoryNumLoops* numLoops;
    pspAngularNumPtsSlider* numPtsSlider;
    vector<pspAngularTrajectoryTimedPosSLider*>* slidersArray;
    vector<pspAngularTrajectoryTimedPosSLider*>* getSlidersArray();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspAngularTrajectorySystemSpecificGUI);
    
};

#endif /* defined(__mpspEditor__pspAngularTrajectorySystem__) */
