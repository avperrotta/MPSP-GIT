//
//  pspSpatConfigGUI.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 17/08/15.
//
//

#ifndef __mpspEditor__pspSpatConfigGUI__
#define __mpspEditor__pspSpatConfigGUI__

#include <stdio.h>
#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"
#include "pspWidgets.h"
#include "ofVectorMath.h"

using namespace std;
class pspSpatConfig;
class pspDBAP;
class pspConcertRoom;

class pspDbapGraphView : public Component{
    
public:
    
    pspDbapGraphView(pspSpatConfig* msc);
    void paint(Graphics& g) override;
    void resized() override;
    
    
private:
    
    Line<float> xAxis;
    Line<float> yAxis;
    Line<float> vd;
    Line<float> pd;
    Line<float> ma;
    ofVec2f roomDiagonal;
    ofVec2f planarDiagonal;
    
    
    pspSpatConfig* mySpatConfig;
    pspDBAP* dbap;
    pspConcertRoom* myCr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspDbapGraphView);
};

class pspDbapParameterSlider : public SliderPropertyComponent{
public:
    pspDbapParameterSlider(const String& propertyName, pspSpatConfig* msc, int p, double min, double max, double inc);
   
    void setValue (double newValue) override;
        
    void sliderValueChanged(Slider* slider) override;
    
    void sliderDragStarted(Slider*) override{
        
    }
    
    void sliderDragEnded(Slider*) override{
        
    }
    
    void updateValue();
    
private:
    
    int parameterIndex;
    pspSpatConfig* mySpatConfig;
    pspDBAP* dbap;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspDbapParameterSlider);
};



class pspDBAPdecayProfileMenu : public PropertyComponent,
public ComboBoxListener
{
public:
    
    pspDBAPdecayProfileMenu(pspSpatConfig* msc):PropertyComponent("DBAP Amplitude Decay Profile"){
        mySpatConfig = msc;
        
        mySpatConfig = msc;
        menuItems.clear();
        menuItems.add("linear: y=A - Bx");
        menuItems.add("exponential: y=A - B(x^C)");
        menuItems.add("sinusoidal: y=A*cos((PI/2)*x/B)");
        addAndMakeVisible(comboBox);
        comboBox.addListener(this);
        populateMenu(menuItems, 1);
    }
    
    
    void setItem(int it){
        comboBox.setSelectedId(it);
    }
    
    void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/) override;
    
    void populateMenu(StringArray mi, int defaultSel){
        comboBox.clear();
        for(int i=0; i<mi.size(); i++){
            comboBox.addItem(mi[i], i+1);
        }
        
        setItem(defaultSel);
    }
    
    void refresh(){
        
    }
    
    void updateChoice();
    
private:
    
    StringArray menuItems;
    pspSpatConfig* mySpatConfig;
    ComboBox comboBox;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspDBAPdecayProfileMenu);
};



class pspSpatCopyFromMenu : public PropertyComponent,
public ComboBoxListener
{
    
public:
    pspSpatCopyFromMenu(pspSpatConfig* msc);
    
    void setItem(int it){
        comboBox.setSelectedId(it);
    }
    
    void comboBoxChanged (ComboBox*) override;
    
    void populateMenu(StringArray mi, int defaultSel){
        comboBox.clear();
        for(int i=0; i<mi.size(); i++){
            comboBox.addItem(mi[i], i+1);
        }
        setItem(defaultSel);
    }
    
    void refresh(){
        
    }
    
    int getSelectedItem(){
        return comboBox.getSelectedId();
    }
    
private:
    
    StringArray menuItems;
    pspSpatConfig* mySpatConfig;
    ComboBox comboBox;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatCopyFromMenu);
    
};



class pspSpatTypeMenu : public PropertyComponent,
public ComboBoxListener
{
    
public:
    
    pspSpatTypeMenu(pspSpatConfig* msc):PropertyComponent("Spat Type")
    {
        mySpatConfig = msc;
        menuItems.clear();
        menuItems.add("DBAP");
        addAndMakeVisible(comboBox);
        comboBox.addListener(this);
        populateMenu(menuItems, 1);
        
    }
    void setItem(int it){
        comboBox.setSelectedId(it);
    }
    
    void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/) override;
    
    void populateMenu(StringArray mi, int defaultSel){
        comboBox.clear();
        for(int i=0; i<mi.size(); i++){
            comboBox.addItem(mi[i], i+1);
        }
        
        setItem(defaultSel);
    }
    
    void refresh(){
        
    }
    
    
private:
    
    StringArray menuItems;
    pspSpatConfig* mySpatConfig;
    ComboBox comboBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatTypeMenu);
    
    
};

class pspSpatCalibLoadButton : public ButtonPropertyComponent
{
public:
    pspSpatCalibLoadButton (const String& propertyName, pspSpatConfig* msc):ButtonPropertyComponent(propertyName, true){
        
        mySpatConfig = msc;
        
    }
    
    void buttonClicked() override;
    
    String getButtonText() const override
    {
        return "load";
    }
    
private:
    
    pspSpatConfig* mySpatConfig;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatCalibLoadButton);
};


class pspSpatCalibSaveButton : public ButtonPropertyComponent
{
public:
    pspSpatCalibSaveButton (const String& propertyName, pspSpatConfig* msc):ButtonPropertyComponent(propertyName, true){
        mySpatConfig = msc;
    }
    
    void buttonClicked() override;
    
    String getButtonText() const override
    {
        return "save";
    }
    
private:
    
    pspSpatConfig* mySpatConfig;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatCalibSaveButton);
};




class pspSpatConfigGUIComponents : public Component{
public:
    pspSpatConfigGUIComponents(pspSpatConfig* msc);
    ~pspSpatConfigGUIComponents();
    
    void paint(Graphics& g) override;
    void resized() override;
    void setupWidgets();
    
    void numberSpeakersChanged();
    void setDbapGuiParamsFromModel();
    void updateDbapGraph();
    int getCopyFromMenuState();
    void setFromXml(int cpfm, int algchoice);
    
protected:
    
    pspSpatConfig* mySpatConfig;
    PropertyPanel* panel;
    Array<PropertyComponent*> comps;
    PropertyComponent* copyFromMenu;
    bool useCopyFromMenu;
    PropertyComponent* spatTypeMenu;
    PropertyComponent* dbapDecayMenu;
    PropertyComponent* dbapParamSliderA;
    PropertyComponent* dbapParamSliderB;
    PropertyComponent* dbapParamSliderC;
    Component* dbapGraph;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatConfigGUIComponents);
};


class pspSpatConfig;
class pspSpatConfigGUIWindow : public DocumentWindow{
    
public:
    
    pspSpatConfigGUIWindow(pspSpatConfig* msc, const String& name, Colour backgroundColour, int buttonsNeeded);
    ~pspSpatConfigGUIWindow();
    
    void paint(Graphics& g)override;
    void resized()override;
    void closeButtonPressed();
    bool keyPressed(const KeyPress &key);
    
protected:
    
    pspSpatConfig* mySpatConfig;
    void changeListenerCallback(ChangeBroadcaster* source);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatConfigGUIWindow);
    
};


class pspSpatConfigGUI {
    
public:
    
    pspSpatConfigGUI(pspSpatConfig* msc);
    ~pspSpatConfigGUI();
    
    void showGui();
    
    void numberSpeakersChanged();
    void setDbapGuiParamsFromModel();
    void setFromXml(int cpfm, int algchoice);
    
private:
    
    Component::SafePointer<DocumentWindow> myWindow;
    Component* myWindowContent;
    
    
    pspSpatConfig* mySpatConfig;
    pspConcertRoom* myCr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspSpatConfigGUI);
};








#endif /* defined(__mpspEditor__pspSpatConfigGUI__) */
