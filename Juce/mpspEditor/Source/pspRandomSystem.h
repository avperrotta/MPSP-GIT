//
//  pspRandomSystem.h
//  psPsa-001
//
//  Created by Andre Perrotta on 02/12/14.
//
//

#ifndef __psPsa_001__pspRandomSystem__
#define __psPsa_001__pspRandomSystem__
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "pspParticleSystem.h"
#include "pspMathUtils.h"
#include "pspRandomParticle.h"
#include "pspParticleSystemGUI.h"

using namespace std;

class pspParticleSystemsManager;

class pspRandomSystem : public pspParticleSystem{
    
public:
    
    pspRandomSystem(pspParticleSystemsManager* sm, String n, int np);
    ~pspRandomSystem();
    
    void specificSetup();
    void specificUpdate();
    void drawParticles();
    void drawSystem();
    
    void resetTime();
    void setBounds(CubeLimits cl) override;
    void setBounds(int l, double val) override;
    CubeLimits* getBounds();
    
    void saveXml(File xml);
    void loadXml(File xml);
    
protected:
    
    CubeLimits* bounds;
    limits lx, ly, lz;
    
    void addParticleSpecific(int np);
    
};


class pspRandomSystemSpecificGUI : public Component{
public:
    
    pspRandomSystemSpecificGUI(pspParticleSystem* ps);
    ~pspRandomSystemSpecificGUI();
    
    void paint(Graphics& g)override;
    void resized()override;
    
    void createWidgets();
    
    void setSliderValues(double v1, double v2, double v3, double v4, double v5, double v6);
    
private:
    
    pspLimitSlider* lxMin;
    pspLimitSlider* lxMax;
    pspLimitSlider* lyMin;
    pspLimitSlider* lyMax;
    pspLimitSlider* lzMin;
    pspLimitSlider* lzMax;
    
    pspParticleSystem* mySystem;
    PropertyPanel* panel;
    Array<PropertyComponent*> comps;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspRandomSystemSpecificGUI);
    
};




#endif /* defined(__psPsa_001__pspRandomSystem__) */
