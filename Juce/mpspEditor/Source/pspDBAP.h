//
//  pspDBAP.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 18/08/15.
//
//

#ifndef __mpspEditor__pspDBAP__
#define __mpspEditor__pspDBAP__

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class pspDBAP{
    
public:
    
    pspDBAP();
    ~pspDBAP();
    
    //1 = linear, 2 = exponential
    int decayProfile;
    
    int getDecayProfile();
    void setDecayProfile(int dp);
    
    vector<double>* linearParams;
    vector<double>* expParams;
    vector<double>* sineParams;
    
    double getParam(int p);
    vector<double>* getParams();
    vector<double>* getLinearParams();
    vector<double>* getExpParams();
    vector<double>* getSineParams();
    
    void setParams(int p, double val);
    void setLinearParams(int p, double val);
    void setExpParams(int p, double val);
    void setSineParams(int p, double val);
    
    double getAmplitude(double x);
    double getLinearAmplitude(double x);
    double getExpAmplitude(double x);
    double getSineAmplitude(double x);
    
    void copyParams(pspDBAP* source);
    
};

#endif /* defined(__mpspEditor__pspDBAP__) */
