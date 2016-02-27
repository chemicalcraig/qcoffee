#ifndef BREWPARAMS_H
#define BREWPARAMS_H

class BrewParams
{
public:
    int brewer,ncups,strength;
    float setTempC;
    int grindGperMS, grindMSperS;
    float strengthScale;
    int grindTime; //grind time in ms
    int grindMSperC;
    int sendData[3];
    int steepTime;
    void setSendData();
    void setgrindMS();
    
    BrewParams();
};

#endif // BREWPARAMS_H
