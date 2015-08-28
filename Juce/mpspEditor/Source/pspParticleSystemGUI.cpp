//
//  pspParticleSystemGUI.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 25/08/15.
//
//

#include "pspParticleSystemGUI.h"
#include "pspParticleSystem.h"
#include "pspParticleSystemsManager.h"

//Limit slider
//===========================================
void pspLimitSlider::sliderValueChanged(Slider* slider){
    mySystem->setBounds(limit, slider->getValue());
}
//===========================================


//NumParticlesSlider
//===========================================
pspNumParticlesSlider::pspNumParticlesSlider(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 1, 1){
    
    mySystem = ps;
    
    //slider.setValue(1);
    setValue(mySystem->getNumParticles());
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspNumParticlesSlider::~pspNumParticlesSlider(){
    mySystem = nullptr;
}

void pspNumParticlesSlider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspNumParticlesSlider::sliderValueChanged(Slider* slider){
    mySystem->changeNumParticles(slider->getValue());
}

void pspNumParticlesSlider::sliderDragStarted(Slider*){
    
}

void pspNumParticlesSlider::sliderDragEnded(Slider*){
    
}
//===========================================


//Load Button
//===========================================
void pspPSystemGUILoadButton::buttonClicked(){
    
    FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
    
    if (fc.browseForFileToOpen())
    {
        mySystem->loadXml(fc.getResult());
    }
}
//===========================================


//Save Button
//===========================================
void pspPSystemGUISaveButton::buttonClicked(){
    FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
    
    if (fc.browseForFileToSave(true))
    {
        mySystem->saveXml(fc.getResult());
    }
}
//===========================================

//Generic Component
//===========================================
pspParticleSystemGUIGenericComponent::pspParticleSystemGUIGenericComponent(pspParticleSystem* ps){
    mySystem = ps;
    
    genericPanel = new PropertyPanel();
    genericPanel->setBounds(0, 0, 200, 300);
    setupWidgets();
}
pspParticleSystemGUIGenericComponent::~pspParticleSystemGUIGenericComponent(){
    if(!genericPanel->isEmpty()){
        genericPanel->clear();
    }
    delete genericPanel;
}

void pspParticleSystemGUIGenericComponent::paint(Graphics& g){
    g.fillAll(Colours::white);
}
void pspParticleSystemGUIGenericComponent::resized(){
    if(!genericPanel->isEmpty()){
        genericPanel->setBounds(5, 5, 200, genericPanel->getTotalContentHeight());
        
        //Rectangle<int> myWindowBounds = this->getParentComponent()->getBounds();
        setBounds(5, 5, genericPanel->getWidth() + 10, genericPanel->getTotalContentHeight() + 10);
    }
    else{
        setBounds(0, 0, 100, 100);
    }
    
    //Rectangle<int> myBounds = this->getBounds();
    
    //cout<<endl<<"myBounds = "<<myBounds.getX()<<" "<<myBounds.getY()<<" "<<myBounds.getWidth()<<" "<<myBounds.getHeight();
    //cout<<endl<<"myWindowBounds = "<<myWindowBounds.getX()<<" "<<myWindowBounds.getY()<<" "<<myWindowBounds.getWidth()<<" "<<myWindowBounds.getHeight();
    //    cout<<endl<<"localBounds = "<<getLocalBounds().getX()<<" "<<getLocalBounds().getY()<<" "<<getLocalBounds().getWidth()<<" "<<getLocalBounds().getHeight();
    
}
void pspParticleSystemGUIGenericComponent::setupWidgets(){
    addAndMakeVisible(genericPanel);
    
    if(!genericPanel->isEmpty()){
        genericPanel->clear();
    }
    comps.clear();
    comps.add(new pspPSystemGUILoadButton(" ", mySystem));
    comps.add(new pspPSystemGUISaveButton(" ", mySystem));
    comps.add(npSlider = new pspNumParticlesSlider("num particles", mySystem));
    genericPanel->addSection("generic parameters", comps);
    comps.clear();
}

pspNumParticlesSlider* pspParticleSystemGUIGenericComponent::getNumParticleSlider(){
    return npSlider;
}
//===========================================


//window
//===========================================
pspParticleSystemGUIWindow::pspParticleSystemGUIWindow(pspParticleSystem* ps, const String& name, Colour backgroundColour, int buttonsNeeded) : DocumentWindow(name, backgroundColour, buttonsNeeded){
    
    mySystem = ps;
    
}

pspParticleSystemGUIWindow::~pspParticleSystemGUIWindow(){
    
}


void pspParticleSystemGUIWindow::closeButtonPressed(){
    
    setVisible(false);
}


void pspParticleSystemGUIWindow::paint(Graphics& g){
    g.fillAll(Colour::greyLevel(0.5f));
}


void pspParticleSystemGUIWindow::resized(){
    for(int i=0; i<getNumChildComponents(); i++){
        getChildComponent(i)->setBounds(0, 0, getWidth(), getHeight());
        for(int j=0; j<getChildComponent(i)->getNumChildComponents(); j++){
            getChildComponent(i)->getChildComponent(j)->resized();
        }
    }
}

bool pspParticleSystemGUIWindow::keyPressed(const KeyPress &key){
    mySystem->getSystemsManager()->keyPressed(key);
    return true;
}
//===========================================


//pspParticleSystemGUI class
//===========================================
pspParticleSystemGUI::pspParticleSystemGUI(pspParticleSystem* ps){
    mySystem = ps;
    
    myWindow = new pspParticleSystemGUIWindow(mySystem, mySystem->getFullName() + " GUI", Colour::greyLevel(1.5f), DocumentWindow::allButtons);
    myWindow->setBounds(0, 0, 300, 400);
    myWindow->setResizable(true, true);
    myWindow->setUsingNativeTitleBar(true);
    
    myWindowContent = new Component();
    myWindowContent->setBounds(myWindow->getBounds());
    myWindowContent->setVisible(true);
    
    myGenericContent = new pspParticleSystemGUIGenericComponent(mySystem);
    myGenericContent->setBounds(5, 5, myWindowContent->getWidth(), myWindowContent->getHeight());
    myWindowContent->addAndMakeVisible(myGenericContent);
    
    //myWindow->juce::Component::addChildComponent(myWindowContent);
    myWindow->setContentOwned(myWindowContent, true);
}

pspParticleSystemGUI::~pspParticleSystemGUI(){
    delete myGenericContent;
    delete myWindow;
}

pspParticleSystem* pspParticleSystemGUI::getParticleSystem(){
    return mySystem;
}

void pspParticleSystemGUI::showGui(){
    myWindow->setVisible(true);
}

void pspParticleSystemGUI::setSpecificComponent(Component* c){
    mySpecificContent = c;
    mySpecificContent->setVisible(true);
    myWindowContent->addAndMakeVisible(mySpecificContent);
    mySpecificContent->setBounds(myGenericContent->getX(), myGenericContent->getY() + myGenericContent->getHeight() + 5, myGenericContent->getWidth(), myWindowContent->getHeight() - myGenericContent->getHeight() - 10);
    myWindow->setBounds(100, 100, mySpecificContent->getWidth() + 10, mySpecificContent->getY() + mySpecificContent->getHeight() + 10);
}

Component* pspParticleSystemGUI::getGenericComponent(){
    return myGenericContent;
}