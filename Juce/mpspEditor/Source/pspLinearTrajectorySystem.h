//
//  pspLinearTrajectorySystem.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 26/08/15.
//
//

#ifndef __mpspEditor__pspLinearTrajectorySystem__
#define __mpspEditor__pspLinearTrajectorySystem__

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "pspParticleSystem.h"
#include "pspMathUtils.h"
#include "pspLinearTrajectoryParticle.h"
#include "pspParticleSystemGUI.h"

using namespace std;

class pspParticleSystemsManager;

class pspLinearTrajectorySystem : public pspParticleSystem{
    
public:
    
    pspLinearTrajectorySystem(pspParticleSystemsManager* sm, String n, int np);
    ~pspLinearTrajectorySystem();
    
    void specificSetup();
    void specificUpdate();
    void drawParticles();
    void drawSystem();
    
    void changeNumPts(int np);
    vector<timedPos>* getPts();
    
    void saveXml(File xml);
    void loadXml(File xml);
    
    void setNumLoops(int nl);
    int getNumLoops();
    
protected:
    
    vector<timedPos>* pts;
    double randomness;
    
    
    void addParticleSpecific(int np);

private:
    
    void addPt();
    void removePt();
    int numLoops;
};


//GUI
//===========================================================

class pspLinearNumPtsSlider : public SliderPropertyComponent{
public:
    pspLinearNumPtsSlider(const String& propertyName, pspParticleSystem* ps);
    
    ~pspLinearNumPtsSlider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspLinearNumPtsSlider);
};


class pspLinearTrajectoryTimedPosSLider : public SliderPropertyComponent{
public:
    pspLinearTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex);
    
    ~pspLinearTrajectoryTimedPosSLider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    int myPtIndex;
    int myCoord;
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspLinearTrajectoryTimedPosSLider);
};


class pspLinearTrajectoryNumLoops : public SliderPropertyComponent{
public:
    pspLinearTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps);
    
    ~pspLinearTrajectoryNumLoops();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspLinearTrajectoryNumLoops);
};


class pspLinearTrajectorySystemSpecificGUI : public Component{
public:
    
    pspLinearTrajectorySystemSpecificGUI(pspParticleSystem* ps);
    ~pspLinearTrajectorySystemSpecificGUI();
    
    void paint(Graphics& g)override;
    void resized()override;
    
    void createWidgets();
    
    void changeNumPts();
    
private:
    
    pspParticleSystem* mySystem;
    PropertyPanel* panel;
    Array<PropertyComponent*> comps;
    Array<PropertyComponent*> ptsGUI;
    vector<timedPos>* pts;
    
    pspLinearTrajectoryNumLoops* numLoops;
    pspLinearNumPtsSlider* numPtsSlider;
    vector<pspLinearTrajectoryTimedPosSLider*>* slidersArray;
    vector<pspLinearTrajectoryTimedPosSLider*>* getSlidersArray();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspLinearTrajectorySystemSpecificGUI);
    
};





#endif /* defined(__mpspEditor__pspLinearTrajectorySystem__) */
