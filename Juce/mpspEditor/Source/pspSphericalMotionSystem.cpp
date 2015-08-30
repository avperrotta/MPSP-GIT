//
//  pspSphericalMotionSystem.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 28/08/15.
//
//

#include "pspSphericalMotionSystem.h"

//GUI
//=================================================
pspSphericalMotionTrajectorySystemSpecificGUI::pspSphericalMotionTrajectorySystemSpecificGUI(pspParticleSystem* ps){
    mySystem = ps;
    pts = static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->getPts();
    
    panel = new PropertyPanel();
    //panel->setBounds(5, 5, getWidth(), getHeight());
    addAndMakeVisible(panel);
    
    slidersArray = new vector<pspSphericalMotionTrajectoryTimedPosSLider*>();
    
    createWidgets();
}
pspSphericalMotionTrajectorySystemSpecificGUI::~pspSphericalMotionTrajectorySystemSpecificGUI(){
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

void pspSphericalMotionTrajectorySystemSpecificGUI::paint(Graphics& g){
    g.fillAll(Colours::white);
}
void pspSphericalMotionTrajectorySystemSpecificGUI::resized(){
    if(getParentComponent()){
        //cout<<endl<<getParentComponent()->getX()<<" "<<getParentComponent()->getY()<<" "<<getParentComponent()->getWidth()<<" "<<getParentComponent()->getHeight();
        setBounds(getX(), getY(), getWidth(), getParentComponent()->getHeight() - getY() - 15);
        panel->setBounds(5, 5, getWidth() - 10, getHeight() - 10);
    }
}

void pspSphericalMotionTrajectorySystemSpecificGUI::createWidgets(){
    
    if(!panel->isEmpty()){
        panel->clear();
    }
    
    comps.clear();
    comps.add(numPtsSlider = new pspSphericalMotionNumPtsSlider("num pts", mySystem));
    numPtsSlider->setValue(pts->size());
    comps.add(numLoops = new pspSphericalMotionTrajectoryNumLoops("num loops", mySystem));
    panel->addSection("Specific attributes", comps);
    
    comps.clear();
    comps.add(startTheta = new pspSphericalMotionTrajectoryTimedPosSLider("start theta", mySystem, 2, 0, true));
    comps.add(startPhi = new pspSphericalMotionTrajectoryTimedPosSLider("start phi", mySystem, 3, 0, true));
    panel->addSection("Start angles", comps);
    
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
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("r", mySystem, 1, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 0]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("vel theta", mySystem, 7, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 1]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("vel phi", mySystem, 8, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 2]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center x", mySystem, 4, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 3]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center y", mySystem, 5, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 4]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center z", mySystem, 6, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 5]);
        slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("time(ms)", mySystem, 9, i, false));
        ptsGUI.add((*slidersArray)[i*7 + 6]);
        panel->addSection("state: " + String(i+1), ptsGUI);
    }
    
    for(int i=0; i<pts->size(); i++){
        (*slidersArray)[i*7 + 0]->setValue((*pts)[i].r);
        (*slidersArray)[i*7 + 1]->setValue((*pts)[i].vTheta*180./M_PI);
        (*slidersArray)[i*7 + 2]->setValue((*pts)[i].vPhi*180./M_PI);
        (*slidersArray)[i*7 + 3]->setValue((*pts)[i].cx);
        (*slidersArray)[i*7 + 4]->setValue((*pts)[i].cy);
        (*slidersArray)[i*7 + 5]->setValue((*pts)[i].cz);
        (*slidersArray)[i*7 + 6]->setValue((*pts)[i].time);
    }
    
    
}

void pspSphericalMotionTrajectorySystemSpecificGUI::changeNumPts(){
    numPtsSlider->setValue(pts->size());
    int currentNpts = slidersArray->size()/7;
    int np = pts->size();
    
    //cout<<endl<<currentNpts<<" "<<np;
    
    if(np > currentNpts){
        for(int i=currentNpts; i<np; i++){
            ptsGUI.clear();
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("r", mySystem, 1, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 0]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("vel theta", mySystem, 7, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 1]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("vel phi", mySystem, 8, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 2]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center x", mySystem, 4, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 3]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center y", mySystem, 5, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 4]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("center z", mySystem, 6, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 5]);
            slidersArray->push_back(new pspSphericalMotionTrajectoryTimedPosSLider("time(ms)", mySystem, 9, i, false));
            ptsGUI.add((*slidersArray)[i*7 + 6]);
            panel->addSection("state: " + String(i+1), ptsGUI);
        }
    }
    else if(np < currentNpts){
        while((panel->getSectionNames().size() - 2) > np){
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
            (*slidersArray)[i*7 + 1]->setValue((*pts)[i].vTheta*180./M_PI);
            (*slidersArray)[i*7 + 2]->setValue((*pts)[i].vPhi*180./M_PI);
            (*slidersArray)[i*7 + 3]->setValue((*pts)[i].cx);
            (*slidersArray)[i*7 + 4]->setValue((*pts)[i].cy);
            (*slidersArray)[i*7 + 5]->setValue((*pts)[i].cz);
            (*slidersArray)[i*7 + 6]->setValue((*pts)[i].time);
        }
    }
}

vector<pspSphericalMotionTrajectoryTimedPosSLider*>* pspSphericalMotionTrajectorySystemSpecificGUI::getSlidersArray(){
    return slidersArray;
}

//pspSphericalMotionNumPtsSlider
//===========================================
pspSphericalMotionNumPtsSlider::pspSphericalMotionNumPtsSlider(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    slider.setValue(0);
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspSphericalMotionNumPtsSlider::~pspSphericalMotionNumPtsSlider(){
    mySystem = nullptr;
}

void pspSphericalMotionNumPtsSlider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspSphericalMotionNumPtsSlider::sliderValueChanged(Slider* slider){
    static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->changeNumPts(slider->getValue());
}

void pspSphericalMotionNumPtsSlider::sliderDragStarted(Slider*){
    
}

void pspSphericalMotionNumPtsSlider::sliderDragEnded(Slider*){
    
}
//===========================================

//pspSphericalMotionTrajectoryTimedPosSLider
//===========================================
pspSphericalMotionTrajectoryTimedPosSLider::pspSphericalMotionTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex, bool isStartPoint_):SliderPropertyComponent(propertyName, 0., 1., 0.001)
{
    
    mySystem = ps;
    myPtIndex = ptIndex;
    myCoord = coord;
        isStartPoint = isStartPoint_;
    
    if(coord == 1){
        slider.setRange(0., 1.);
    }
    else if(coord == 2){
        slider.setRange(0., 180.);
    }
    else if(coord == 3){
        slider.setRange(0., 360.);
    }
    else if(coord > 3 && coord < 7){
        slider.setRange(-1., 1.);
    }
    else if(coord > 6 && coord < 9){
        slider.setRange(-20., 20.);
    }
    else if(coord == 9){
        slider.setRange(50, 3600000);
    }
}

pspSphericalMotionTrajectoryTimedPosSLider::~pspSphericalMotionTrajectoryTimedPosSLider(){
    mySystem = nullptr;
}

void pspSphericalMotionTrajectoryTimedPosSLider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspSphericalMotionTrajectoryTimedPosSLider::sliderValueChanged(Slider* slider){
    if(!isStartPoint){
        (*static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->getPts())[myPtIndex].setDeg(myCoord, slider->getValue());
    }
    else{
        static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->setStartPoint(myCoord, slider->getValue());
    }
    
}

void pspSphericalMotionTrajectoryTimedPosSLider::sliderDragStarted(Slider*){
    
}

void pspSphericalMotionTrajectoryTimedPosSLider::sliderDragEnded(Slider*){
    
}
//===========================================


//num loops slider
//===========================================
pspSphericalMotionTrajectoryNumLoops::pspSphericalMotionTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    //slider.setValue(static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->getNumLoops());
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspSphericalMotionTrajectoryNumLoops::~pspSphericalMotionTrajectoryNumLoops(){
    mySystem = nullptr;
}

void pspSphericalMotionTrajectoryNumLoops::setValue (double newValue){
    slider.setValue(newValue);
}

void pspSphericalMotionTrajectoryNumLoops::sliderValueChanged(Slider* slider){
    //cout<<endl<<slider->getValue();
    static_cast<pspSphericalMotionTrajectorySystem*>(mySystem)->setNumLoops(slider->getValue());
}

void pspSphericalMotionTrajectoryNumLoops::sliderDragStarted(Slider*){
    
}

void pspSphericalMotionTrajectoryNumLoops::sliderDragEnded(Slider*){
    
}

//===========================================



//=================================================


//SphericalMotionTrajectorySystem
//=================================================
pspSphericalMotionTrajectorySystem::pspSphericalMotionTrajectorySystem(pspParticleSystemsManager* sm, String n, int np){
    
    pts = new vector<timedSphericalMotionState>();
    pts->push_back(timedSphericalMotionState(0.5, 0., 0., 0., 0., 0., 1., 0., 1000.));
    //pts->push_back(timedSphericalMotionState(0.5, 180., 0., 0., 0., 0., 1., 0., 1000.));
    
    startPosition = new timedPosRad(0.5, 0., 0., 0., 0., 0., 0);
    
    numLoops = 0;
    
    setup(sm, n, np);
    
    
}

pspSphericalMotionTrajectorySystem::~pspSphericalMotionTrajectorySystem(){
    
    pts->clear();
    delete  pts;
    delete startPosition;
    
}

void pspSphericalMotionTrajectorySystem::specificSetup(){
    
    type = "sphericalMotion";
    
    mySpecificGui = new pspSphericalMotionTrajectorySystemSpecificGUI(this);
    
    numLoops = 0;
}

void pspSphericalMotionTrajectorySystem::addParticleSpecific(int np){
    int indexStart = particles->size();
    for(int i=0; i<np; i++){
        particles->push_back(new pspSphericalMotionTrajectoryParticle(this, i + indexStart));
    }
}

void pspSphericalMotionTrajectorySystem::specificUpdate(){
    
}

void pspSphericalMotionTrajectorySystem::changeNumPts(int np){
    if(np > pts->size()){
        for(int i=pts->size(); i<np; i++){
            pts->push_back(timedSphericalMotionState(0.5, 180., 0., 0., 0., 0., 1., 0., 1000.));
        }
    }
    else if(np < pts->size()){
        while(np < pts->size()){
            pts->pop_back();
        }
    }
    static_cast<pspSphericalMotionTrajectorySystemSpecificGUI*>(mySpecificGui)->changeNumPts();
    
}

void pspSphericalMotionTrajectorySystem::addPt(){
    pts->push_back(timedSphericalMotionState(0.5, 180., 0., 0., 0., 0., 1., 0., 1000.));
}
void pspSphericalMotionTrajectorySystem::removePt(){
    pts->pop_back();
}

vector<timedSphericalMotionState>* pspSphericalMotionTrajectorySystem::getPts(){
    return pts;
}

void pspSphericalMotionTrajectorySystem::setNumLoops(int nl){
    numLoops = nl;
    for(int i=0; i<particles->size(); i++){
        static_cast<pspSphericalMotionTrajectoryParticle*>((*particles)[i])->setNumLoops(nl);
    }
}

timedPosRad* pspSphericalMotionTrajectorySystem::getStartPosition(){
    return startPosition;
}

int pspSphericalMotionTrajectorySystem::getNumLoops(){
    return numLoops;
}

void pspSphericalMotionTrajectorySystem::setStartPoint(int coord, double val){
    startPosition->setDeg(coord, val);
}

void pspSphericalMotionTrajectorySystem::drawParticles(){
    pspParticleSystem::drawParticles();
}

void pspSphericalMotionTrajectorySystem::drawSystem(){
    
}

void pspSphericalMotionTrajectorySystem::saveXml(File xml){
    XmlElement* mainElement = new XmlElement("SphericalMotionTrajectory");
    
    XmlElement* params = new XmlElement("parameterValues");
    params->setAttribute("numParticles", (int)particles->size());
    params->setAttribute("numLoops", numLoops);
    XmlElement* points = new XmlElement("trajectoryPoints");
    for(int i=0; i<pts->size(); i++){
        XmlElement* ptElement = new XmlElement("timedPt");
        ptElement->setAttribute("pt", i+1);
        ptElement->setAttribute("r", (*pts)[i].r);
        ptElement->setAttribute("vTheta", (*pts)[i].vTheta);
        ptElement->setAttribute("vPhi", (*pts)[i].vPhi);
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
void pspSphericalMotionTrajectorySystem::loadXml(File xml){
    XmlDocument myDocument (xml);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "error loading SphericalMotionTrajectory xml file !", "", "OK");
        delete mainElement;
        return;
    }
    else{
        if(!mainElement->hasTagName("SphericalMotionTrajectory")){
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain SphericalMotionTrajectory data", "", "OK");
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
                            double r, vTheta, vPhi, cx, cy, cz, time;
                            r = ptsElement->getChildElement(i)->getDoubleAttribute("r");
                            vTheta = ptsElement->getChildElement(i)->getDoubleAttribute("vTheta");
                            vPhi = ptsElement->getChildElement(i)->getDoubleAttribute("vPhi");
                            cx = ptsElement->getChildElement(i)->getDoubleAttribute("cx");
                            cy = ptsElement->getChildElement(i)->getDoubleAttribute("cy");
                            cz = ptsElement->getChildElement(i)->getDoubleAttribute("cz");
                            time = ptsElement->getChildElement(i)->getDoubleAttribute("time");
                            pts->push_back(timedSphericalMotionState(r, 0., 0., cx, cy, cz, vTheta, vPhi, time));
                        }
                        static_cast<pspSphericalMotionTrajectorySystemSpecificGUI*>(mySpecificGui)->createWidgets();
                    }
                    
                }
            }
        }
    }
    
    
    
    delete mainElement;
}
//=================================================