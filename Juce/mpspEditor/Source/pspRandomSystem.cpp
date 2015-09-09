
//
//  pspRandomSystem.cpp
//  psPsa-001
//
//  Created by Andre Perrotta on 02/12/14.
//
//

#include "pspRandomSystem.h"
#include "pspParticleSystemsManager.h"



//Specific GUI component
//=======================================================
pspRandomSystemSpecificGUI::pspRandomSystemSpecificGUI(pspParticleSystem* ps){
    
    mySystem = ps;
    panel = new PropertyPanel();
    panel->setBounds(0, 0, 200, 200);
    createWidgets();
}


pspRandomSystemSpecificGUI::~pspRandomSystemSpecificGUI(){
    comps.clear();
    if(!panel->isEmpty()){
        panel->clear();
    }
    delete panel;
}

void pspRandomSystemSpecificGUI::paint(Graphics& g){
    g.fillAll(Colours::white);
}
void pspRandomSystemSpecificGUI::resized(){
    panel->setBounds(5, 5, panel->getWidth(), panel->getTotalContentHeight());
    setBounds(5, getY(), panel->getWidth() + 10, panel->getTotalContentHeight() + 10);
    
}

void pspRandomSystemSpecificGUI::createWidgets(){
    addAndMakeVisible(panel);
    
    comps.clear();
    comps.add(lxMin = new pspLimitSlider(mySystem, "min limits x", -3., 3., 0.005, 1));
    comps.add(lxMax = new pspLimitSlider(mySystem, "max limits x", -3., 3., 0.005, 2));
    
    comps.add(lyMin = new pspLimitSlider(mySystem, "min limits y", -3., 3., 0.005, 3));
    comps.add(lyMax = new pspLimitSlider(mySystem, "max limits y", -3., 3., 0.005, 4));
    
    comps.add(lzMin = new pspLimitSlider(mySystem, "min limits z", -3., 3., 0.005, 5));
    comps.add(lzMax = new pspLimitSlider(mySystem, "max limits z", -3., 3., 0.005, 6));
    panel->addSection("Specific attributes", comps);
    //comps.clear();
}

void pspRandomSystemSpecificGUI::setSliderValues(double v1, double v2, double v3, double v4, double v5, double v6){
    lxMin->setValue(v1);
    lxMax->setValue(v2);
    lyMin->setValue(v3);
    lyMax->setValue(v4);
    lzMin->setValue(v5);
    lzMax->setValue(v6);
}
//===================================================================


pspRandomSystem::pspRandomSystem(pspParticleSystemsManager* sm, String n, int np){
    bounds = new CubeLimits(limits(-0.7, 0.7), limits(-0.7, 0.7), limits(-0.7, 0.7));
    setup(sm, n, np);
}

pspRandomSystem::~pspRandomSystem(){
    
    delete bounds;
    
}

void pspRandomSystem::specificSetup(){
    
    type = "random";
    
    mySpecificGui = new pspRandomSystemSpecificGUI(this);
    
    lx = bounds->limits_x;
    ly = bounds->limits_y;
    lz = bounds->limits_z;
}

void pspRandomSystem::addParticleSpecific(int np){
    int indexStart = particles->size();
    for(int i=0; i<np; i++){
        particles->push_back(new pspRandomParticle(this, i + indexStart));
        if(bounds != nullptr){
            static_cast<pspRandomParticle*>(particles->back())->setBounds(bounds);
        }
        
    }
}

void pspRandomSystem::specificUpdate(){
    
}

void pspRandomSystem::resetTime(){
    
}

void pspRandomSystem::setBounds(CubeLimits cl){
    bounds->limits_x = cl.limits_x;
    bounds->limits_y = cl.limits_y;
    bounds->limits_z = cl.limits_z;
    
//    bounds->limits_x.min *= systemsManager->getConcertRoom()->getBounds()[0];
//    bounds->limits_x.max *= systemsManager->getConcertRoom()->getBounds()[0];
//    bounds->limits_y.min *= systemsManager->getConcertRoom()->getBounds()[1];
//    bounds->limits_y.max *= systemsManager->getConcertRoom()->getBounds()[1];
//    bounds->limits_z.min *= systemsManager->getConcertRoom()->getBounds()[2];
//    bounds->limits_z.max *= systemsManager->getConcertRoom()->getBounds()[2];
    
    
    lx = bounds->limits_x;
    ly = bounds->limits_y;
    lz = bounds->limits_z;
    
}

void pspRandomSystem::setBounds(int l, double val){
    switch(l){
        case 1:
            bounds->limits_x.min = val;
            break;
        case 2:
            bounds->limits_x.max = val;
            break;
        case 3:
            bounds->limits_y.min = val;
            break;
        case 4:
            bounds->limits_y.max = val;
            break;
        case 5:
            bounds->limits_z.min = val;
            break;
        case 6:
            bounds->limits_z.max = val;
            break;
    }
    
    bounds->limits_x.min *= systemsManager->getConcertRoom()->getBounds()[0];
    bounds->limits_x.max *= systemsManager->getConcertRoom()->getBounds()[0];
    bounds->limits_y.min *= systemsManager->getConcertRoom()->getBounds()[1];
    bounds->limits_y.max *= systemsManager->getConcertRoom()->getBounds()[1];
    bounds->limits_z.min *= systemsManager->getConcertRoom()->getBounds()[2];
    bounds->limits_z.max *= systemsManager->getConcertRoom()->getBounds()[2];
    
    lx = bounds->limits_x;
    ly = bounds->limits_y;
    lz = bounds->limits_z;
}

CubeLimits* pspRandomSystem::getBounds(){
    return bounds;
}


void pspRandomSystem::drawParticles(){
    pspParticleSystem::drawParticles();
}



void pspRandomSystem::drawSystem(){
    glPushMatrix();
    //floor
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.min, lz.max);
    glVertex3f(lx.max, ly.min, lz.max);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.min, lz.max);
    glVertex3f(lx.max, ly.min, lz.min);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.min, lz.min);
    glVertex3f(lx.min, ly.min, lz.min);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.min, lz.min);
    glVertex3f(lx.min, ly.min, lz.max);
    glEnd();
    
    //ceiling
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.max, lz.max);
    glVertex3f(lx.max, ly.max, lz.max);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.max, lz.max);
    glVertex3f(lx.max, ly.max, lz.min);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.max, lz.min);
    glVertex3f(lx.min, ly.max, lz.min);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.max, lz.min);
    glVertex3f(lx.min, ly.max, lz.max);
    glEnd();
    
    //walls
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.max, lz.max);
    glVertex3f(lx.min, ly.min, lz.max);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.max, lz.max);
    glVertex3f(lx.max, ly.min, lz.max);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(lx.max, ly.max, lz.min);
    glVertex3f(lx.max, ly.min, lz.min);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(lx.min, ly.max, lz.min);
    glVertex3f(lx.min, ly.min, lz.min);
    glEnd();
    
    glPopMatrix();
}

void pspRandomSystem::saveXml(juce::File xmlFile){
    XmlElement* mainElement = new XmlElement("RandomTrajectory");
    
    XmlElement* params = new XmlElement("parameterValues");
    params->setAttribute("numParticles", (int)particles->size());
    params->setAttribute("lxMin", (double)lx.min);
    params->setAttribute("lxMax", (double)lx.max);
    params->setAttribute("lyMin", (double)ly.min);
    params->setAttribute("lyMax", (double)ly.max);
    params->setAttribute("lzMin", (double)lz.min);
    params->setAttribute("lzMax", (double)lz.max);
    mainElement->addChildElement(params);
    
    mainElement->writeToFile(xmlFile, "");
    mainElement->deleteAllChildElements();
    delete mainElement;
}

void pspRandomSystem::loadXml(juce::File xmlFile){
    XmlDocument myDocument (xmlFile);
    XmlElement* mainElement = myDocument.getDocumentElement();
    if (mainElement == nullptr)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "error loading randomTrajectory xml file !", "", "OK");
        delete mainElement;
        return;
    }
    else{
        if(!mainElement->hasTagName("RandomTrajectory")){
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "xml file doesn't randomTrajectory data", "", "OK");
            delete mainElement;
            return;
        }
        else{
            XmlElement* params = mainElement->getChildByName("parameterValues");
            if(params){
                int np = params->getIntAttribute("numParticles");
                changeNumParticles(np);
                static_cast<pspParticleSystemGUIGenericComponent*>(myGui->getGenericComponent())->getNumParticleSlider()->setValue(np);
                
                setBounds(1, params->getDoubleAttribute("lxMin"));
                setBounds(2, params->getDoubleAttribute("lxMax"));
                setBounds(3, params->getDoubleAttribute("lyMin"));
                setBounds(4, params->getDoubleAttribute("lyMax"));
                setBounds(5, params->getDoubleAttribute("lzMin"));
                setBounds(6, params->getDoubleAttribute("lzMax"));
                
                static_cast<pspRandomSystemSpecificGUI*>(mySpecificGui)->setSliderValues(params->getDoubleAttribute("lxMin"),
                                                                                         params->getDoubleAttribute("lxMax"),
                                                                                         params->getDoubleAttribute("lyMin"),
                                                                                         params->getDoubleAttribute("lxMax"),
                                                                                         params->getDoubleAttribute("lzMin"),
                                                                                         params->getDoubleAttribute("lzMax"));
            }
        }
    }
    
        
    
    delete mainElement;
}








