#include "brewparams.h"

BrewParams::BrewParams()
{
    this->brewer=0;
    this->ncups=0;
    this->strength=2;
    this->grindMSperS = 1000;
    this->grindMSperC = 2000; //base suggested grind time
    this->strengthScale = 1.;
}

void BrewParams::setgrindMS() {
    int basems = this->grindMSperC -1*(2-this->strength)*this->grindMSperC*0.25;
    this->grindTime = basems*(this->ncups+1);
}

void BrewParams::setSendData() {
    this->sendData[0] = this->brewer;
    this->sendData[1] = this->ncups;
    this->sendData[2] = this->steepTime;
}
