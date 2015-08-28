//
//  pspSphericalMotionSystem.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#ifndef __mpspEditor__pspSphericalMotionSystem__
#define __mpspEditor__pspSphericalMotionSystem__

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "pspParticleSystem.h"
#include "pspMathUtils.h"
#include "pspSphericalMotionParticle.h"
#include "pspParticleSystemGUI.h"

using namespace std;

class pspParticleSystemsManager;

class pspSphericalMotionTrajectorySystem : public pspParticleSystem{
    
public:
    
    pspSphericalMotionTrajectorySystem(pspParticleSystemsManager* sm, String n, int np);
    ~pspSphericalMotionTrajectorySystem();
    
    void specificSetup();
    void specificUpdate();
    void drawParticles();
    void drawSystem();
    
    void changeNumPts(int np);
    vector<timedSphericalMotionState>* getPts();
    
    void saveXml(File xml);
    void loadXml(File xml);
    
    void setNumLoops(int nl);
    int getNumLoops();
    
    void setStartPoint(int coord, double val);
    timedPosRad* getStartPosition();
    
protected:
    
    timedPosRad* startPosition;
    vector<timedSphericalMotionState>* pts;
    double randomness;
    
    
    void addParticleSpecific(int np);
    
private:
    
    void addPt();
    void removePt();
    int numLoops;
};


//GUI
//===========================================================

class pspSphericalMotionNumPtsSlider : public SliderPropertyComponent{
public:
    pspSphericalMotionNumPtsSlider(const String& propertyName, pspParticleSystem* ps);
    
    ~pspSphericalMotionNumPtsSlider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSphericalMotionNumPtsSlider);
};


class pspSphericalMotionTrajectoryTimedPosSLider : public SliderPropertyComponent{
public:
    pspSphericalMotionTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex, bool isStartPoint_);
    
    ~pspSphericalMotionTrajectoryTimedPosSLider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    void setRange(double min, double max){
        slider.setRange(min, max);
    }
    
protected:
    
    bool isStartPoint;
    int myPtIndex;
    int myCoord;
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSphericalMotionTrajectoryTimedPosSLider);
};


class pspSphericalMotionTrajectoryNumLoops : public SliderPropertyComponent{
public:
    pspSphericalMotionTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps);
    
    ~pspSphericalMotionTrajectoryNumLoops();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSphericalMotionTrajectoryNumLoops);
};


class pspSphericalMotionTrajectorySystemSpecificGUI : public Component{
public:
    
    pspSphericalMotionTrajectorySystemSpecificGUI(pspParticleSystem* ps);
    ~pspSphericalMotionTrajectorySystemSpecificGUI();
    
    void paint(Graphics& g)override;
    void resized()override;
    
    void createWidgets();
    
    void changeNumPts();
    
private:
    
    pspParticleSystem* mySystem;
    PropertyPanel* panel;
    Array<PropertyComponent*> comps;
    Array<PropertyComponent*> ptsGUI;
    vector<timedSphericalMotionState>* pts;
    
    pspSphericalMotionTrajectoryNumLoops* numLoops;
    pspSphericalMotionNumPtsSlider* numPtsSlider;
    pspSphericalMotionTrajectoryTimedPosSLider* startTheta;
    pspSphericalMotionTrajectoryTimedPosSLider* startPhi;
    vector<pspSphericalMotionTrajectoryTimedPosSLider*>* slidersArray;
    vector<pspSphericalMotionTrajectoryTimedPosSLider*>* getSlidersArray();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSphericalMotionTrajectorySystemSpecificGUI);
    
};

#endif /* defined(__mpspEditor__pspSphericalMotionSystem__) */
