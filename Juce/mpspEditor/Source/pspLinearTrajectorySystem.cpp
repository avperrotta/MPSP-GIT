//
//  pspLinearTrajectorySystem.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 26/08/15.
//
//

#include "pspLinearTrajectorySystem.h"
//GUI
//=================================================
pspLinearTrajectorySystemSpecificGUI::pspLinearTrajectorySystemSpecificGUI(pspParticleSystem* ps){
    mySystem = ps;
    pts = static_cast<pspLinearTrajectorySystem*>(mySystem)->getPts();
    
    panel = new PropertyPanel();
    //panel->setBounds(5, 5, getWidth(), getHeight());
    addAndMakeVisible(panel);
    
    slidersArray = new vector<pspLinearTrajectoryTimedPosSLider*>();
    
    createWidgets();
}
pspLinearTrajectorySystemSpecificGUI::~pspLinearTrajectorySystemSpecificGUI(){
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

void pspLinearTrajectorySystemSpecificGUI::paint(Graphics& g){
    g.fillAll(Colours::white);
}
void pspLinearTrajectorySystemSpecificGUI::resized(){
    if(getParentComponent()){
        //cout<<endl<<getParentComponent()->getX()<<" "<<getParentComponent()->getY()<<" "<<getParentComponent()->getWidth()<<" "<<getParentComponent()->getHeight();
        setBounds(getX(), getY(), getWidth(), getParentComponent()->getHeight() - getY() - 15);
        panel->setBounds(5, 5, getWidth() - 10, getHeight() - 10);
    }
}

void pspLinearTrajectorySystemSpecificGUI::createWidgets(){
    
    
    
    if(!panel->isEmpty()){
        panel->clear();
    }
    
    comps.clear();
    comps.add(numPtsSlider = new pspLinearNumPtsSlider("num pts", mySystem));
    numPtsSlider->setValue(pts->size());
    comps.add(numLoops = new pspLinearTrajectoryNumLoops("num loops", mySystem));
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
        slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("x", mySystem, 1, i));
        ptsGUI.add((*slidersArray)[i*4 + 0]);
        slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("y", mySystem, 2, i));
        ptsGUI.add((*slidersArray)[i*4 + 1]);
        slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("z", mySystem, 3, i));
        ptsGUI.add((*slidersArray)[i*4 + 2]);
        slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("time(ms)", mySystem, 4, i));
        ptsGUI.add((*slidersArray)[i*4 + 3]);
        panel->addSection("pt: " + String(i+1), ptsGUI);
    }
    
    for(int i=0; i<pts->size(); i++){
        (*slidersArray)[i*4 + 0]->setValue((*pts)[i].x);
        (*slidersArray)[i*4 + 1]->setValue((*pts)[i].y);
        (*slidersArray)[i*4 + 2]->setValue((*pts)[i].z);
        (*slidersArray)[i*4 + 3]->setValue((*pts)[i].time);
    }
    
    
}

void pspLinearTrajectorySystemSpecificGUI::changeNumPts(){
    numPtsSlider->setValue(pts->size());
    int currentNpts = slidersArray->size()/4;
    int np = pts->size();
    
    //cout<<endl<<currentNpts<<" "<<np;
    
    if(np > currentNpts){
        for(int i=currentNpts; i<np; i++){
            ptsGUI.clear();
            slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("x", mySystem, 1, i));
            ptsGUI.add((*slidersArray)[i*4 + 0]);
            slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("y", mySystem, 2, i));
            ptsGUI.add((*slidersArray)[i*4 + 1]);
            slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("z", mySystem, 3, i));
            ptsGUI.add((*slidersArray)[i*4 + 2]);
            slidersArray->push_back(new pspLinearTrajectoryTimedPosSLider("time(ms)", mySystem, 4, i));
            ptsGUI.add((*slidersArray)[i*4 + 3]);
            panel->addSection("pt: " + String(i+1), ptsGUI);
        }
    }
    else if(np < currentNpts){
        while((panel->getSectionNames().size() - 1) > np){
            //cout<<endl<<"removing panel: "<<panel->getSectionNames().size() - 1;
            panel->removeSection(panel->getSectionNames().size() - 1);
            for(int i=1; i<=4; i++){
                //delete (*slidersArray)[slidersArray->size() - i];
                slidersArray->pop_back();
            }
        }
    }
    
    if(!slidersArray->empty() && !pts->empty()){
        for(int i=0; i<pts->size(); i++){
            (*slidersArray)[i*4 + 0]->setValue((*pts)[i].x);
            (*slidersArray)[i*4 + 1]->setValue((*pts)[i].y);
            (*slidersArray)[i*4 + 2]->setValue((*pts)[i].z);
            (*slidersArray)[i*4 + 3]->setValue((*pts)[i].time);
        }
    }
}

vector<pspLinearTrajectoryTimedPosSLider*>* pspLinearTrajectorySystemSpecificGUI::getSlidersArray(){
    return slidersArray;
}

//pspLinearNumPtsSlider
//===========================================
pspLinearNumPtsSlider::pspLinearNumPtsSlider(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    slider.setValue(0);
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspLinearNumPtsSlider::~pspLinearNumPtsSlider(){
    mySystem = nullptr;
}

void pspLinearNumPtsSlider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspLinearNumPtsSlider::sliderValueChanged(Slider* slider){
    static_cast<pspLinearTrajectorySystem*>(mySystem)->changeNumPts(slider->getValue());
}

void pspLinearNumPtsSlider::sliderDragStarted(Slider*){
    
}

void pspLinearNumPtsSlider::sliderDragEnded(Slider*){
    
}
//===========================================

//pspLinearTrajectoryTimedPosSLider
//===========================================
pspLinearTrajectoryTimedPosSLider::pspLinearTrajectoryTimedPosSLider(const String& propertyName, pspParticleSystem* ps, int coord, int ptIndex):SliderPropertyComponent(propertyName, -2., 2., 0.001){
    
    mySystem = ps;
    
    myPtIndex = ptIndex;
    myCoord = coord;
    
    if(coord == 4){
        slider.setRange(50, 3600000);
    }
}

pspLinearTrajectoryTimedPosSLider::~pspLinearTrajectoryTimedPosSLider(){
    mySystem = nullptr;
}

void pspLinearTrajectoryTimedPosSLider::setValue (double newValue){
    slider.setValue(newValue);
}

void pspLinearTrajectoryTimedPosSLider::sliderValueChanged(Slider* slider){
    (*static_cast<pspLinearTrajectorySystem*>(mySystem)->getPts())[myPtIndex].set(myCoord, slider->getValue());
}

void pspLinearTrajectoryTimedPosSLider::sliderDragStarted(Slider*){
    
}

void pspLinearTrajectoryTimedPosSLider::sliderDragEnded(Slider*){
    
}
//===========================================


//num loops slider
//===========================================
pspLinearTrajectoryNumLoops::pspLinearTrajectoryNumLoops(const String& propertyName, pspParticleSystem* ps):SliderPropertyComponent(propertyName, 0, 2000, 1){
    
    mySystem = ps;
    
    //slider.setValue(static_cast<pspLinearTrajectorySystem*>(mySystem)->getNumLoops());
    slider.setSliderStyle(Slider::IncDecButtons);
    slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
    slider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Vertical);
}

pspLinearTrajectoryNumLoops::~pspLinearTrajectoryNumLoops(){
    mySystem = nullptr;
}

void pspLinearTrajectoryNumLoops::setValue (double newValue){
    slider.setValue(newValue);
}

void pspLinearTrajectoryNumLoops::sliderValueChanged(Slider* slider){
    //cout<<endl<<slider->getValue();
    static_cast<pspLinearTrajectorySystem*>(mySystem)->setNumLoops(slider->getValue());
}

void pspLinearTrajectoryNumLoops::sliderDragStarted(Slider*){
    
}

void pspLinearTrajectoryNumLoops::sliderDragEnded(Slider*){
    
}

//===========================================



//=================================================


//LinearTrajectorySystem
//=================================================
pspLinearTrajectorySystem::pspLinearTrajectorySystem(pspParticleSystemsManager* sm, String n, int np){
    
    pts = new vector<timedPos>();
    pts->push_back(timedPos(-0.5, 0., 0.5, 1000.));
    pts->push_back(timedPos(0.5, 0., 0.5, 1000.));
    
    numLoops = 0;
    
    setup(sm, n, np);
    
    
}

pspLinearTrajectorySystem::~pspLinearTrajectorySystem(){
    
    pts->clear();
    delete  pts;
    
}

void pspLinearTrajectorySystem::specificSetup(){
    
    type = "linear";
    
    mySpecificGui = new pspLinearTrajectorySystemSpecificGUI(this);
    
    numLoops = 0;
}

void pspLinearTrajectorySystem::addParticleSpecific(int np){
    int indexStart = particles->size();
    for(int i=0; i<np; i++){
        particles->push_back(new pspLinearTrajectoryParticle(this, i + indexStart));
    }
}

void pspLinearTrajectorySystem::specificUpdate(){
    
}

void pspLinearTrajectorySystem::changeNumPts(int np){
    if(np > pts->size()){
        for(int i=pts->size(); i<np; i++){
            pts->push_back(timedPos(0., 0., 0., 1000));
        }
    }
    else if(np < pts->size()){
        while(np < pts->size()){
            pts->pop_back();
        }
    }
    static_cast<pspLinearTrajectorySystemSpecificGUI*>(mySpecificGui)->changeNumPts();
    
}

void pspLinearTrajectorySystem::addPt(){
    pts->push_back(timedPos(0., 0., 0., 1000));
}
void pspLinearTrajectorySystem::removePt(){
    pts->pop_back();
}

vector<timedPos>* pspLinearTrajectorySystem::getPts(){
    return pts;
}

void pspLinearTrajectorySystem::setNumLoops(int nl){
    numLoops = nl;
    for(int i=0; i<particles->size(); i++){
        static_cast<pspLinearTrajectoryParticle*>((*particles)[i])->setNumLoops(nl);
    }
}
int pspLinearTrajectorySystem::getNumLoops(){
    return numLoops;
}

void pspLinearTrajectorySystem::drawParticles(){
    pspParticleSystem::drawParticles();
}

void pspLinearTrajectorySystem::drawSystem(){
    
}

void pspLinearTrajectorySystem::saveXml(File xml){
    XmlElement* mainElement = new XmlElement("linearTrajectory");
    
    XmlElement* params = new XmlElement("parameterValues");
    params->setAttribute("numParticles", (int)particles->size());
    params->setAttribute("numLoops", numLoops);
    XmlElement* points = new XmlElement("trajectoryPoints");
    for(int i=0; i<pts->size(); i++){
        XmlElement* ptElement = new XmlElement("timedPt");
        ptElement->setAttribute("pt", i+1);
        ptElement->setAttribute("x", (*pts)[i].x);
        ptElement->setAttribute("y", (*pts)[i].y);
        ptElement->setAttribute("z", (*pts)[i].z);
        ptElement->setAttribute("time", (*pts)[i].time);
        points->addChildElement(ptElement);
    }
    params->addChildElement(points);
    mainElement->addChildElement(params);
    mainElement->writeToFile(xml, "");
    mainElement->deleteAllChildElements();
    delete mainElement;
}
void pspLinearTrajectorySystem::loadXml(File xml){
    XmlDocument myDocument (xml);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "error loading linearTrajectory xml file !", "", "OK");
        delete mainElement;
        return;
    }
    else{
        if(!mainElement->hasTagName("linearTrajectory")){
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't contain linearTrajectory data", "", "OK");
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
                            double x, y, z, time;
                            x = ptsElement->getChildElement(i)->getDoubleAttribute("x");
                            y = ptsElement->getChildElement(i)->getDoubleAttribute("y");
                            z = ptsElement->getChildElement(i)->getDoubleAttribute("z");
                            time = ptsElement->getChildElement(i)->getDoubleAttribute("time");
                            pts->push_back(timedPos(x, y, z, time));
                        }
                        static_cast<pspLinearTrajectorySystemSpecificGUI*>(mySpecificGui)->createWidgets();
                    }
                    
                }
            }
        }
    }
    
    
    
    delete mainElement;
}
//=================================================






