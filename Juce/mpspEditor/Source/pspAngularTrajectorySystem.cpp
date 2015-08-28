//
//  pspAngularTrajectorySystem.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#include "pspAngularTrajectorySystem.h"

//GUI
//=================================================
pspAngularTrajectorySystemSpecificGUI::pspAngularTrajectorySystemSpecificGUI(pspParticleSystem* ps){
    mySystem = ps;
    pts = static_cast<pspAngularTrajectorySystem*>(mySystem)->getPts();
    
    panel = new PropertyPanel();
    //panel->setBounds(5, 5, getWidth(), getHeight());
    addAndMakeVisible(panel);
    
    slidersArray = new vector<pspAngularTrajectoryTimedPosSLider*>();
    
    createWidgets();
}
pspAngularTrajectorySystemSpecificGUI::~pspAngularTrajectorySystemSpecificGUI(){
    comps.clear();
    if(!panel->isEmpty()){
        panel->clear();
    }
    delete panel;
    
    if(slidersArray){
        while(!slidersArray->empty()){
            //delete slidersArray->back();
            slidersArray->pop_back();
        }
        delete slidersArray;
    }
    
}

void pspAngularTrajectorySystemSpecificGUI::paint(Graphics& g){
    g.fillAll(Colours::white);
}
void pspAngularTrajectorySystemSpecificGUI::resized(){
    if(getParentComponent()){
        //cout<<endl<<getParentComponent()->getX()<<" "<<getParentComponent()->getY()<<" "<<getParentComponent()->getWidth()<<" "<<getParentComponent()->getHeight();
        setBounds(getX(), getY(), getWidth(), getParentComponent()->getHeight() - getY() - 15);
        panel->setBounds(5, 5, getWidth() - 10, getHeight() - 10);
    }
}

void pspAngularTrajectorySystemSpecificGUI::createWidgets(){
    
    
    
    if(!panel->isEmpty()){
        panel->clear();
    }
    
    comps.clear();
    comps.add(numPtsSlider = new pspAngularNumPtsSlider("num pts", mySystem));
    numPtsSlider->setValue(pts->size());
    comps.add(numLoops = new pspAngularTrajectoryNumLoops("num loops", mySystem));
    panel->addSection("Specific attributes", comps);
    
    //cout<<endl<<"panel size = "<<panel->getSectionNames().size();
    
    if(!slidersArray->empty()){
        while(!slidersArray->empty()){
            //delete slidersArray->back();
            slidersArray->pop_back();
        }
        slidersArray->clear();
    }
    
    for(int i=0; i<pts->size(); i++){
        //cout<<endl<<"entrei";
        ptsGUI.clear();
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("r", mySystem, 1, i));
        ptsGUI.add((*slidersArray)[i*7 + 0]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("theta", mySystem, 2, i));
        ptsGUI.add((*slidersArray)[i*7 + 1]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("phi", mySystem, 3, i));
        ptsGUI.add((*slidersArray)[i*7 + 2]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center x", mySystem, 4, i));
        ptsGUI.add((*slidersArray)[i*7 + 3]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center y", mySystem, 5, i));
        ptsGUI.add((*slidersArray)[i*7 + 4]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center z", mySystem, 6, i));
        ptsGUI.add((*slidersArray)[i*7 + 5]);
        slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("time(ms)", mySystem, 7, i));
        ptsGUI.add((*slidersArray)[i*7 + 6]);
        panel->addSection("pt: " + String(i+1), ptsGUI);
    }
    
    for(int i=0; i<pts->size(); i++){
        (*slidersArray)[i*7 + 0]->setValue((*pts)[i].r);
        (*slidersArray)[i*7 + 1]->setValue((*pts)[i].theta*180./M_PI);
        (*slidersArray)[i*7 + 2]->setValue((*pts)[i].phi*180./M_PI);
        (*slidersArray)[i*7 + 3]->setValue((*pts)[i].cx);
        (*slidersArray)[i*7 + 4]->setValue((*pts)[i].cy);
        (*slidersArray)[i*7 + 5]->setValue((*pts)[i].cz);
        (*slidersArray)[i*7 + 6]->setValue((*pts)[i].time);
    }
    
    
}

void pspAngularTrajectorySystemSpecificGUI::changeNumPts(){
    numPtsSlider->setValue(pts->size());
    int currentNpts = slidersArray->size()/7;
    int np = pts->size();
    
    //cout<<endl<<currentNpts<<" "<<np;
    
    if(np > currentNpts){
        for(int i=currentNpts; i<np; i++){
            ptsGUI.clear();
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("r", mySystem, 1, i));
            ptsGUI.add((*slidersArray)[i*7 + 0]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("theta", mySystem, 2, i));
            ptsGUI.add((*slidersArray)[i*7 + 1]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("phi", mySystem, 3, i));
            ptsGUI.add((*slidersArray)[i*7 + 2]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center x", mySystem, 4, i));
            ptsGUI.add((*slidersArray)[i*7 + 3]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center y", mySystem, 5, i));
            ptsGUI.add((*slidersArray)[i*7 + 4]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("center z", mySystem, 6, i));
            ptsGUI.add((*slidersArray)[i*7 + 5]);
            slidersArray->push_back(new pspAngularTrajectoryTimedPosSLider("time(ms)", mySystem, 7, i));
            ptsGUI.add((*slidersArray)[i*7 + 6]);
            panel->addSection("pt: " + String(i+1), ptsGUI);
        }
    }
    else if(np < currentNpts){
        while((panel->getSectionNames().size() - 1) > np){
            //cout<<endl<<"removing panel: "<<panel->getSectionNames().size() - 1;
            panel->removeSection(panel->getSectionNames().size() - 1);
            for(int i=1; i<=7; i++){
                //delete (*slidersArray)[slidersArray->size() - i];
                slidersArray->pop_back();
            }
        }
    }
    
    if(!slidersArray->empty() && !pts->empty()){
        for(int i=0; i<pts->size(); i++){
            (*slidersArray)[i*7 + 0]->setValue((*pts)[i].r);
            (*slidersArray)[i*7 + 1]->setValue((*pts)[i].theta*180./M_PI);
            (*slidersArray)[i*7 + 2]->setValue((*pts)[i].phi*180./M_PI);
            (*slidersArray)[i*7 + 3]->setValue((*pts)[i].cx);
            (*slidersArray)[i*7 + 4]->setValue((*pts)[i].cy);
            (*slidersArray)[i*7 + 5]->setValue((*pts)[i].cz);
            (*slidersArray)[i*7 + 6]->setValue((*pts)[i].time);
        }
    }
}

vector<pspAngularTrajectoryTimedPosSLider*>* pspAngularTrajectorySystemSpecificGUI::getSlidersArray(){
    return slidersArray;
}

//pspAngularNumPtsSlider
//===========================================
pspAngularNumPtsSlider::pspAngularNumPtsSlider(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    slider.setValue(0);
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspAngularNumPtsSlider::~pspAngularNumPtsSlider(){
    mySystem = nullptr;
}

void pspAngularNumPtsSlider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspAngularNumPtsSlider::sliderValueChanged(Slider* slider){
    static_cast<pspAngularTrajectorySystem*>(mySystem)->changeNumPts(slider->getValue());
}

void pspAngularNumPtsSlider::sliderDragStarted(Slider*){
    
}

void pspAngularNumPtsSlider::sliderDragEnded(Slider*){
    
}
//===========================================

//pspAngularTrajectoryTimedPosSLider
//===========================================
pspAngularTrajectoryTimedPosSLider::pspAngularTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex):SliderPropertyComponent(propertyName, 0., 1., 0.001){
    
    mySystem = ps;
    
    myPtIndex = ptIndex;
    myCoord = coord;
    
    if(coord == 1){
        slider.setRange(0., 1.);
    }
    else if(coord == 2){
        slider.setRange(-10*180., 10*180.);
    }
    else if(coord == 3){
        slider.setRange(-10*360., 10*360.);
    }
    else if(coord > 3 && coord < 7){
        slider.setRange(-1., 1.);
    }
    else if(coord == 7){
        slider.setRange(50, 3600000);
    }
}

pspAngularTrajectoryTimedPosSLider::~pspAngularTrajectoryTimedPosSLider(){
    mySystem = nullptr;
}

void pspAngularTrajectoryTimedPosSLider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspAngularTrajectoryTimedPosSLider::sliderValueChanged(Slider* slider){
    (*static_cast<pspAngularTrajectorySystem*>(mySystem)->getPts())[myPtIndex].setDeg(myCoord, slider->getValue());
}

void pspAngularTrajectoryTimedPosSLider::sliderDragStarted(Slider*){
    
}

void pspAngularTrajectoryTimedPosSLider::sliderDragEnded(Slider*){
    
}
//===========================================


//num loops slider
//===========================================
pspAngularTrajectoryNumLoops::pspAngularTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    //slider.setValue(static_cast<pspAngularTrajectorySystem*>(mySystem)->getNumLoops());
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspAngularTrajectoryNumLoops::~pspAngularTrajectoryNumLoops(){
    mySystem = nullptr;
}

void pspAngularTrajectoryNumLoops::setValue (double newValue){
    slider.setValue(newValue);
}

void pspAngularTrajectoryNumLoops::sliderValueChanged(Slider* slider){
    //cout<<endl<<slider->getValue();
    static_cast<pspAngularTrajectorySystem*>(mySystem)->setNumLoops(slider->getValue());
}

void pspAngularTrajectoryNumLoops::sliderDragStarted(Slider*){
    
}

void pspAngularTrajectoryNumLoops::sliderDragEnded(Slider*){
    
}

//===========================================



//=================================================


//AngularTrajectorySystem
//=================================================
pspAngularTrajectorySystem::pspAngularTrajectorySystem(pspParticleSystemsManager* sm, String n, int np){
    
    pts = new vector<timedPosRad>();
    pts->push_back(timedPosRad(0.5, 0., 0., 0., 0., 0., 1000.));
    pts->push_back(timedPosRad(0.5, 180., 0., 0., 0., 0., 1000.));
    
    numLoops = 0;
    
    setup(sm, n, np);
    
    
}

pspAngularTrajectorySystem::~pspAngularTrajectorySystem(){
    
    pts->clear();
    delete  pts;
    
}

void pspAngularTrajectorySystem::specificSetup(){
    
    type = "Angular";
    
    mySpecificGui = new pspAngularTrajectorySystemSpecificGUI(this);
    
    numLoops = 0;
}

void pspAngularTrajectorySystem::addParticleSpecific(int np){
    int indexStart = particles->size();
    for(int i=0; i<np; i++){
        particles->push_back(new pspAngularTrajectoryParticle(this, i + indexStart));
    }
}

void pspAngularTrajectorySystem::specificUpdate(){
    
}

void pspAngularTrajectorySystem::changeNumPts(int np){
    if(np > pts->size()){
        for(int i=pts->size(); i<np; i++){
            pts->push_back(timedPosRad(0.5, 0., 0., 0., 0., 0., 1000));
        }
    }
    else if(np < pts->size()){
        while(np < pts->size()){
            pts->pop_back();
        }
    }
    static_cast<pspAngularTrajectorySystemSpecificGUI*>(mySpecificGui)->changeNumPts();
    
}

void pspAngularTrajectorySystem::addPt(){
    pts->push_back(timedPosRad(0.5, 0., 0., 0., 0., 0., 1000));
}
void pspAngularTrajectorySystem::removePt(){
    pts->pop_back();
}

vector<timedPosRad>* pspAngularTrajectorySystem::getPts(){
    return pts;
}

void pspAngularTrajectorySystem::setNumLoops(int nl){
    numLoops = nl;
    for(int i=0; i<particles->size(); i++){
        static_cast<pspAngularTrajectoryParticle*>((*particles)[i])->setNumLoops(nl);
    }
}
int pspAngularTrajectorySystem::getNumLoops(){
    return numLoops;
}

void pspAngularTrajectorySystem::drawParticles(){
    pspParticleSystem::drawParticles();
}

void pspAngularTrajectorySystem::drawSystem(){
    
}

void pspAngularTrajectorySystem::saveXml(File xml){
    XmlElement* mainElement = new XmlElement("AngularTrajectory");
    
    XmlElement* params = new XmlElement("parameterValues");
    params->setAttribute("numParticles", (int)particles->size());
    params->setAttribute("numLoops", numLoops);
    XmlElement* points = new XmlElement("trajectoryPoints");
    for(int i=0; i<pts->size(); i++){
        XmlElement* ptElement = new XmlElement("timedPt");
        ptElement->setAttribute("pt", i+1);
        ptElement->setAttribute("r", (*pts)[i].r);
        ptElement->setAttribute("theta", (*pts)[i].theta);
        ptElement->setAttribute("phi", (*pts)[i].phi);
        ptElement->setAttribute("cx", (*pts)[i].cx);
        ptElement->setAttribute("cy", (*pts)[i].cy);
        ptElement->setAttribute("cz", (*pts)[i].cz);
        ptElement->setAttribute("time", (*pts)[i].time);
        points->addChildElement(ptElement);
    }
    params->addChildElement(points);
    mainElement->addChildElement(params);
    mainElement->writeToFile(xml, "");
    mainElement->deleteAllChildElements();
    delete mainElement;
}
void pspAngularTrajectorySystem::loadXml(File xml){
    XmlDocument myDocument (xml);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "error loading AngularTrajectory xml file !", "", "OK");
        delete mainElement;
        return;
    }
    else{
        if(!mainElement->hasTagName("AngularTrajectory")){
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain AngularTrajectory data", "", "OK");
            delete mainElement;
            return;
        }
        else{
            XmlElement* params = mainElement->getChildByName("parameterValues");
            if(params){
                int np = params->getIntAttribute("numParticles");
                changeNumParticles(np);
                static_cast<pspParticleSystemGUIGenericComponent*>(myGui->getGenericComponent())->getNumParticleSlider()->setValue(np);
                int nloops = params->getIntAttribute("numLoops");
                
                XmlElement* ptsElement = params->getChildByName("trajectoryPoints");
                if(ptsElement != nullptr){
                    int npts = ptsElement->getNumChildElements();
                    if(npts > 0){
                        pts->clear();
                        for(int i=0; i<npts; i++){
                            double r, theta, phi, cx, cy, cz, time;
                            r = ptsElement->getChildElement(i)->getDoubleAttribute("r");
                            theta = ptsElement->getChildElement(i)->getDoubleAttribute("theta");
                            phi = ptsElement->getChildElement(i)->getDoubleAttribute("phi");
                            cx = ptsElement->getChildElement(i)->getDoubleAttribute("cx");
                            cy = ptsElement->getChildElement(i)->getDoubleAttribute("cy");
                            cz = ptsElement->getChildElement(i)->getDoubleAttribute("cz");
                            time = ptsElement->getChildElement(i)->getDoubleAttribute("time");
                            pts->push_back(timedPosRad(r, theta, phi, cx, cy, cz, time));
                        }
                        static_cast<pspAngularTrajectorySystemSpecificGUI*>(mySpecificGui)->createWidgets();
                    }
                    
                }
            }
        }
    }
    
    
    
    delete mainElement;
}
//=================================================
