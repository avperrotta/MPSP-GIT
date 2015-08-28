//
//  pspParticleSystemGUI.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 25/08/15.
//
//

#ifndef __mpspEditor__pspParticleSystemGUI__
#define __mpspEditor__pspParticleSystemGUI__

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include "ofVectorMath.h"
#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;
class pspParticleSystem;
class pspParticleSystemGUI;


class pspLimitSlider : public SliderPropertyComponent
{
public:
    pspLimitSlider (pspParticleSystem* ps, const String& propertyName, double min, double max, double step, int lim)
    : SliderPropertyComponent (propertyName, min, max, step)
    {
        mySystem = ps;
        limit = lim;
    }
    
    ~pspLimitSlider(){
        
    }
    
    void setValue (double newValue) override
    {
        slider.setValue(newValue);
    }
    
    void sliderValueChanged(Slider* slider) override;
    
    Slider* getSlider(){
        return &slider;
    }
    
private:
    
    pspParticleSystem* mySystem;
    int limit;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspLimitSlider)
};


class pspNumParticlesSlider : public SliderPropertyComponent{
public:
    pspNumParticlesSlider(const String& propertyName, pspParticleSystem* ps);
    
    ~pspNumParticlesSlider();
    
    void setValue (double newValue) override;
    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;
    
protected:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspNumParticlesSlider);
};


class pspPSystemGUILoadButton : public ButtonPropertyComponent
{
public:
    pspPSystemGUILoadButton (const String& propertyName, pspParticleSystem* ps):ButtonPropertyComponent(propertyName, true){
        
        mySystem = ps;
        
    }
    
    void buttonClicked() override;
    
    String getButtonText() const override
    {
        return "load";
    }
    
private:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspPSystemGUILoadButton);
};


class pspPSystemGUISaveButton : public ButtonPropertyComponent
{
public:
    pspPSystemGUISaveButton (const String& propertyName, pspParticleSystem* ps):ButtonPropertyComponent(propertyName, true){
        mySystem = ps;
    }
    
    void buttonClicked() override;
    
    String getButtonText() const override
    {
        return "save";
    }
    
private:
    
    pspParticleSystem* mySystem;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspPSystemGUISaveButton);
};


class pspParticleSystemGUIGenericComponent : public Component{
    
public:
    
    pspParticleSystemGUIGenericComponent(pspParticleSystem* ps);
    ~pspParticleSystemGUIGenericComponent();
    
    void paint(Graphics& g) override;
    void resized() override;
    void setupWidgets();
    
    pspNumParticlesSlider* getNumParticleSlider();
    
private:
    
    pspNumParticlesSlider* npSlider;
    
    pspParticleSystem* mySystem;
    PropertyPanel* genericPanel;
    PropertyPanel* specificPanel;
    Array<PropertyComponent*> comps;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspParticleSystemGUIGenericComponent);
};




class pspParticleSystemGUIWindow : public DocumentWindow{
    
public:
    
    pspParticleSystemGUIWindow(pspParticleSystem* ps, const String& name, Colour backgroundColour, int buttonsNeeded);
    ~pspParticleSystemGUIWindow();
    
    void paint(Graphics& g)override;
    void resized()override;
    void closeButtonPressed();
    bool keyPressed(const KeyPress &key);
    
protected:
    
    pspParticleSystem* mySystem;
    void changeListenerCallback(ChangeBroadcaster* source);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspParticleSystemGUIWindow);
    
};



class pspParticleSystemGUI{
public:
    
    pspParticleSystemGUI(pspParticleSystem* ps);
    ~pspParticleSystemGUI();
    
    pspParticleSystem* getParticleSystem();
    
    void showGui();
    void setSpecificComponent(Component* c);
    
    Component* getGenericComponent();
    
private:
    
    pspParticleSystem* mySystem;
    Component::SafePointer<DocumentWindow> myWindow;
    Component* myWindowContent;
    Component* mySpecificContent;
    Component* myGenericContent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspParticleSystemGUI);
};


#endif /* defined(__mpspEditor__pspParticleSystemGUI__) */
