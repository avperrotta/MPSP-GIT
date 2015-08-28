//
//  pspSpatConfigGUI.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 17/08/15.
//
//
#include "pspParticleSystemsManager.h"
#include "pspSpatConfigGUI.h"
#include "pspSpatConfig.h"
#include "pspConcertRoom.h"
#include "pspDBAP.h"

//widgets

pspDbapGraphView::pspDbapGraphView(pspSpatConfig* msc){
    mySpatConfig = msc;
    dbap = mySpatConfig->getDBAP();
    myCr = mySpatConfig->getConcertRoom();
    setOpaque(true);
}
void pspDbapGraphView::paint(Graphics& g){
    
    g.fillAll(Colours::white);
    g.setOpacity (1.0f);
    g.setColour(Colours::black);
    
    xAxis.setStart(20, this->getHeight() - 30);
    xAxis.setEnd(this->getWidth() - 20,  xAxis.getStartY());
    yAxis.setStart(20, xAxis.getStartY());
    yAxis.setEnd(20, 20);
    
    g.drawArrow(xAxis, 2, 6, 6);
    g.drawArrow(yAxis, 2, 6, 6);
    g.drawText("Amplitude", 10, 5, 100, 10, 1);
    g.drawText("trajectory to speaker dist", xAxis.getStartX(), xAxis.getStartY() + 20, xAxis.getLength(), 10, 20);
    
    double vdx = xAxis.getLength()*0.8;
    vd.setStart(xAxis.getStartX() + vdx, xAxis.getEndY() - 3);
    vd.setEnd(vd.getStartX(), xAxis.getEndY() + 3);
    g.drawLine(vd, 1);
    g.drawText("dmax", vd.getStartX() - 20, vd.getEndY() + 5, 40, 10, 20);
    
    double pdx;
    pdx = vdx*myCr->getPlanarDiagonal()/myCr->getRoomDiagonal();
    pd.setStart(xAxis.getStartX() + pdx, xAxis.getStartY() - 3);
    pd.setEnd(pd.getStartX(), xAxis.getStartY() + 3);
    g.drawLine(pd, 1);
    g.drawText("d", pd.getStartX() - 20, pd.getEndY() + 5, 40, 10, 20);
    
    double may = yAxis.getLength()*0.5;
    ma.setStart(yAxis.getStartX() - 3, yAxis.getStartY() - may);
    ma.setEnd(yAxis.getStartX() + 3, ma.getStartY());
    g.drawLine(ma, 1);
    g.drawText("1", ma.getStartX() - 15, ma.getStartY(), 10, 10, 20);
    
    Path curve;
    double curveStartX, curveStartY;
    
    curveStartX = 0.;
    curveStartY = dbap->getAmplitude(curveStartX);
    curveStartX = xAxis.getStartX() + curveStartX*vdx;
    curveStartY = yAxis.getStartY() - curveStartY*may;
    curve.startNewSubPath(curveStartX, curveStartY);
    curveStartX = 0.;
    curveStartY = dbap->getAmplitude(curveStartX);
    int res = 1000;
    double step = 1.5*myCr->getRoomDiagonal()/(double)res;
    for(int i=0; i<res; i++){
        double x, y;
        x = (double)i*step;
        y = dbap->getAmplitude(x);
        //cout<<endl<<x<<" "<<y;
        curve.lineTo(xAxis.getStartX() + x*vdx, yAxis.getStartY() - y*may);
    }
    
    PathStrokeType stroke (2.);
    g.setColour(Colours::red);
    g.strokePath(curve, stroke);
    //cout<<endl<<myCr->getPlanarDiagonal()<<" "<<myCr->getRoomDiagonal();
    
}
void pspDbapGraphView::resized(){
    
}

void pspSpatCalibSaveButton::buttonClicked(){
    FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
    
    if (fc.browseForFileToSave(true))
    {
        mySpatConfig->saveXml(fc.getResult());
    }
}

void pspSpatCalibLoadButton::buttonClicked(){
    
    FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*.xml", true);
    
    if (fc.browseForFileToOpen())
    {
        mySpatConfig->loadXml(fc.getResult());
    }
    
    //cout<<endl<<"load speakers clicked";
}


pspDbapParameterSlider::pspDbapParameterSlider(const String& propertyName, pspSpatConfig* msc, int p, double min, double max, double inc):SliderPropertyComponent(propertyName, min, max, inc)
{
    mySpatConfig = msc;
    dbap = mySpatConfig->getDBAP();
    parameterIndex = p;
    setValue(dbap->getParam(parameterIndex));
}

void pspDbapParameterSlider::setValue (double newValue)
{
    slider.setValue(newValue);
    //dbap->setParams(parameterIndex, newValue);
}

void pspDbapParameterSlider::sliderValueChanged(Slider* slider){
    dbap->setParams(parameterIndex, slider->getValue());
    mySpatConfig->getConcertRoom()->spatCalibChanged(mySpatConfig);
}

void pspDbapParameterSlider::updateValue(){
    setValue(dbap->getParam(parameterIndex));
}


pspSpatCopyFromMenu::pspSpatCopyFromMenu(pspSpatConfig* msc):juce::PropertyComponent("use parameters from"){
    mySpatConfig = msc;
    
    menuItems.clear();
    menuItems.add("Master");
    
    for(int i=0; i<msc->getConcertRoom()->getNumSpeakers(); i++){
        menuItems.add("speaker " + String(i + 1));
    }
    
    
    addAndMakeVisible(comboBox);
    comboBox.addListener(this);
    populateMenu(menuItems, 1);
}

void pspSpatCopyFromMenu::comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/){
    mySpatConfig->setCopyFromIndex(comboBox.getSelectedId());
    //mySpatConfig->refreshGUIContent();
}

void pspSpatTypeMenu::comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/){
    mySpatConfig->setSpatType(comboBox.getSelectedId());
}

void pspDBAPdecayProfileMenu::comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/){
    mySpatConfig->getDBAP()->setDecayProfile(comboBox.getSelectedId());
    mySpatConfig->getConcertRoom()->spatCalibChanged(mySpatConfig);
}

void pspDBAPdecayProfileMenu::updateChoice(){
    comboBox.setSelectedId(mySpatConfig->getDBAP()->getDecayProfile());
}

//window
//===========================================
pspSpatConfigGUIWindow::pspSpatConfigGUIWindow(pspSpatConfig* msc, const String& name, Colour backgroundColour, int buttonsNeeded) : DocumentWindow(name, backgroundColour, buttonsNeeded){

    mySpatConfig = msc;

}

pspSpatConfigGUIWindow::~pspSpatConfigGUIWindow(){
    
}


void pspSpatConfigGUIWindow::closeButtonPressed(){
    
    setVisible(false);
}


void pspSpatConfigGUIWindow::paint(Graphics& g){
    g.fillAll(Colour::greyLevel(0.5f));
}


void pspSpatConfigGUIWindow::resized(){
    for(int i=0; i<getNumChildComponents(); i++){
        getChildComponent(i)->resized();
    }
}

bool pspSpatConfigGUIWindow::keyPressed(const KeyPress &key){
    mySpatConfig->getConcertRoom()->getSystemsManager()->keyPressed(key);
    return true;
}


//===========================================

//component
//===========================================


pspSpatConfigGUIComponents::pspSpatConfigGUIComponents(pspSpatConfig* msc){
    mySpatConfig = msc;
    
    panel = new PropertyPanel();
    
    dbapGraph = new pspDbapGraphView(mySpatConfig);
    
    useCopyFromMenu = false;
    if(mySpatConfig->getSpeakerID() > 0){
        useCopyFromMenu = true;
    }
    
    setupWidgets();
}

pspSpatConfigGUIComponents::~pspSpatConfigGUIComponents(){
    if(!panel->isEmpty()){
        panel->clear();
    }
    delete panel;
    delete dbapGraph;
}


void pspSpatConfigGUIComponents::paint(Graphics& g){
    //g.fillAll (Colours::white);
    //g.setOpacity (1.0f);
}
void pspSpatConfigGUIComponents::resized(){
    
    Rectangle<int> myWindowBounds = this->getParentComponent()->getBounds();
    setBounds(0, 0, myWindowBounds.getWidth(), myWindowBounds.getHeight());
    
    Rectangle<int> myBounds = this->getBounds();
    
    //cout<<endl<<"myBounds = "<<myBounds.getX()<<" "<<myBounds.getY()<<" "<<myBounds.getWidth()<<" "<<myBounds.getHeight();
    //cout<<endl<<"myWindowBounds = "<<myWindowBounds.getX()<<" "<<myWindowBounds.getY()<<" "<<myWindowBounds.getWidth()<<" "<<myWindowBounds.getHeight();
    //    cout<<endl<<"localBounds = "<<getLocalBounds().getX()<<" "<<getLocalBounds().getY()<<" "<<getLocalBounds().getWidth()<<" "<<getLocalBounds().getHeight();
    
    panel->setBounds(5, 5, 300, panel->getTotalContentHeight());
    dbapGraph->setBounds(panel->getX() + panel->getWidth() + 5, 5, getLocalBounds().getWidth() - panel->getWidth() - 15, getLocalBounds().getHeight() - 15);
}

void pspSpatConfigGUIComponents::setupWidgets(){
    
    addAndMakeVisible(panel);
    addChildComponent(dbapGraph);
    
    if(!panel->isEmpty()){
        panel->clear();
    }
    comps.clear();
    
    comps.add(new pspSpatCalibLoadButton(" ", mySpatConfig));
    comps.add(new pspSpatCalibSaveButton(" ", mySpatConfig));
    
    if(useCopyFromMenu){
        comps.add(new pspSpatCopyFromMenu(mySpatConfig));
        copyFromMenu = comps.getLast();
    }
    comps.add(new pspSpatTypeMenu(mySpatConfig));
    spatTypeMenu = comps.getLast();
    panel->addSection("Spatialization parameters", comps);
    comps.clear();
    
    comps.add(new pspDBAPdecayProfileMenu(mySpatConfig));
    dbapDecayMenu = comps.getLast();
    comps.add(new pspDbapParameterSlider("A", mySpatConfig, 1, 0., 1.5, 0.001));
    dbapParamSliderA = comps.getLast();
    comps.add(new pspDbapParameterSlider("B", mySpatConfig, 2, 0., 5., 0.001));
    dbapParamSliderB = comps.getLast();
    comps.add(new pspDbapParameterSlider("C", mySpatConfig, 3, 0., 10., 0.001));
    dbapParamSliderC = comps.getLast();
    panel->addSection("DBAP parameters", comps);
    comps.clear();
    dbapGraph->setVisible(true);
}

void pspSpatConfigGUIComponents::numberSpeakersChanged(){
    if(useCopyFromMenu){
        StringArray items;
        items.clear();
        items.add("Master");
        for(int i=0; i<mySpatConfig->getConcertRoom()->getNumSpeakers(); i++){
            items.add("speaker " + String(i + 1));
        }
        int speakerIndex = static_cast<pspSpatCopyFromMenu*>(copyFromMenu)->getSelectedItem();
//        if((speakerIndex - 1) > mySpatConfig->getConcertRoom()->getNumSpeakers()){
//            speakerIndex = 1;
//        }
        
        static_cast<pspSpatCopyFromMenu*>(copyFromMenu)->populateMenu(items, speakerIndex);
        //mySpatConfig->copyConfigFrom(static_cast<pspSpatCopyFromMenu*>(copyFromMenu)->getSelectedItem());
    }
}

void pspSpatConfigGUIComponents::setDbapGuiParamsFromModel(){
    static_cast<pspDbapParameterSlider*>(dbapParamSliderA)->updateValue();
    static_cast<pspDbapParameterSlider*>(dbapParamSliderB)->updateValue();
    static_cast<pspDbapParameterSlider*>(dbapParamSliderC)->updateValue();
    static_cast<pspDBAPdecayProfileMenu*>(dbapDecayMenu)->updateChoice();
    
    updateDbapGraph();
}

void pspSpatConfigGUIComponents::updateDbapGraph(){
    if(dbapGraph){
        if(dbapGraph->isVisible()){
            dbapGraph->repaint();
        }
        
    }
}

int pspSpatConfigGUIComponents::getCopyFromMenuState(){
    return static_cast<pspSpatCopyFromMenu*>(copyFromMenu)->getSelectedItem();
}

void pspSpatConfigGUIComponents::setFromXml(int cpfm, int algchoice){
    if(useCopyFromMenu){
        static_cast<pspSpatCopyFromMenu*>(copyFromMenu)->setItem(cpfm);
    }
    
    static_cast<pspSpatTypeMenu*>(spatTypeMenu)->setItem(algchoice);
}


//SpatConfigGUI
//==============================
pspSpatConfigGUI::pspSpatConfigGUI(pspSpatConfig* msc){
    mySpatConfig = msc;
    myCr = mySpatConfig->getConcertRoom();
    
    myWindow = new pspSpatConfigGUIWindow(mySpatConfig, mySpatConfig->getName() + " Spat Calibration", Colour::greyLevel(1.5f), DocumentWindow::allButtons);
    myWindow->setBounds(0, 0, 640, 320);
    myWindow->setResizable(true, true);
    myWindow->setUsingNativeTitleBar(true);
    
    myWindowContent = new pspSpatConfigGUIComponents(mySpatConfig);
    myWindow->juce::Component::addChildComponent(myWindowContent);
    myWindow->setContentOwned(myWindowContent, false);
}

pspSpatConfigGUI::~pspSpatConfigGUI(){
    //myWindow->deleteAllChildren();
    delete myWindow;
}

void pspSpatConfigGUI::showGui(){
    myWindow->setVisible(true);
}

void pspSpatConfigGUI::numberSpeakersChanged(){
    static_cast<pspSpatConfigGUIComponents*>(myWindowContent)->numberSpeakersChanged();
}

void pspSpatConfigGUI::setDbapGuiParamsFromModel(){
    static_cast<pspSpatConfigGUIComponents*>(myWindowContent)->setDbapGuiParamsFromModel();
}

void pspSpatConfigGUI::setFromXml(int cpfm, int algchoice){
    static_cast<pspSpatConfigGUIComponents*>(myWindowContent)->setFromXml(cpfm, algchoice);
}

//==============================












