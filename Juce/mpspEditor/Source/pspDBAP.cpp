//
//  pspDBAP.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 18/08/15.
//
//

#include "pspDBAP.h"
#include "math.h"

pspDBAP::pspDBAP(){
    linearParams = new vector<double>();
    linearParams->push_back(1.0);
    linearParams->push_back(1.0);
    
    expParams = new vector<double>();
    expParams->push_back(1.0);
    expParams->push_back(1.0);
    expParams->push_back(2.0);
    
    sineParams = new vector<double>();
    sineParams->push_back(1.0);
    sineParams->push_back(1.0);
    
    decayProfile = 1;
}
pspDBAP::~pspDBAP(){
    if(linearParams){
        linearParams->clear();
        delete linearParams;
    }
    
    if(expParams){
        expParams->clear();
        delete expParams;
    }
    
    if(sineParams){
        sineParams->clear();
        delete sineParams;
    }
}

int pspDBAP::getDecayProfile(){
    return decayProfile;
}

void pspDBAP::setDecayProfile(int dp){
    decayProfile = dp;
}

vector<double>* pspDBAP::getParams(){
    switch (decayProfile) {
        case 1:
            return getLinearParams();
            break;
        case 2:
            return getExpParams();
            break;
        case 3:
            return getSineParams();
            break;
            
    }
    return NULL;
}

vector<double>* pspDBAP::getLinearParams(){
    return linearParams;
}

vector<double>* pspDBAP::getExpParams(){
    return expParams;
}

vector<double>* pspDBAP::getSineParams(){
    return sineParams;
}

double pspDBAP::getParam(int p){
    if(decayProfile == 1){
        if(p>0 && p <=2){
            return (*linearParams)[p-1];
        }
    }
    else if(decayProfile == 2){
        if(p>0 && p <=3){
            return (*expParams)[p-1];
        }
    }
    else if(decayProfile == 3){
        if(p>0 && p <=2){
            return (*sineParams)[p-1];
        }
    }
}


void pspDBAP::setLinearParams(int p, double val){
    (*linearParams)[p] = val;
}
void pspDBAP::setExpParams(int p, double val){
    //cout<<endl<<val;
    (*expParams)[p] = val;
}

void pspDBAP::setSineParams(int p, double val){
    if(p == 1){
        if(val == 0.){
            val = 0.0000001;
        }
    }
    (*sineParams)[p] = val;
}

void pspDBAP::setParams(int p,double val){
    if(decayProfile == 1){
        if(p > 0 && p <=2){
            setLinearParams(p-1, val);
        }
    }
    else if(decayProfile == 2){
        if(p>0 && p<=3){
            setExpParams(p-1, val);
        }
    }
    else if(decayProfile == 3){
        if(p>0 && p<=2){
            setSineParams(p-1, val);
        }
    }
}

double pspDBAP::getAmplitude(double x){
    if(decayProfile == 1){
        return getLinearAmplitude(x);
    }
    else if(decayProfile == 2){
        return getExpAmplitude(x);
    }
    else if(decayProfile == 3){
        return getSineAmplitude(x);
    }
    
    return 0.0;
}

double pspDBAP::getLinearAmplitude(double x){
    double y;
    
    y = (*linearParams)[0] - (*linearParams)[1]*x;
    
    if(y < 0.0){
        return 0.0;
    }
    return y;
    
}
double pspDBAP::getExpAmplitude(double x){
    double y;
    
    y = (*expParams)[0] - (*expParams)[1]*pow(x, (*expParams)[2]);
    
    if(y < 0.0){
        return 0.0;
    }
    return y;
}

double pspDBAP::getSineAmplitude(double x){
    double y;
    
    double theta = M_PI_2*x/(*sineParams)[1];
    if(theta > M_PI_2){
        theta = M_PI_2;
    }
    
    y =(*sineParams)[0]*cos(theta);
    
    if(y < 0.0){
        return 0.0;
    }
    return y;
}

void pspDBAP::copyParams(pspDBAP* source){
    decayProfile = source->getDecayProfile();
    
    linearParams->clear();
    for(int i=0; i<source->getLinearParams()->size(); i++){
        linearParams->push_back((*source->getLinearParams())[i]);
    }
    
    expParams->clear();
    for(int i=0; i<source->getExpParams()->size(); i++){
        expParams->push_back((*source->getExpParams())[i]);
    }
    
    sineParams->clear();
    for(int i=0; i<source->getSineParams()->size(); i++){
        sineParams->push_back((*source->getSineParams())[i]);
    }
    
}


