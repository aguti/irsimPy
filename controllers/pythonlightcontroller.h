#ifndef PYTHONLIGHTCONTROLLER_H_
#define PYTHONLIGHTCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "reallightsensor.h"

class CPythonLightController : public CPythonControllerBase
{
public:
    CPythonLightController(const char* pch_name, CEpuck* pc_epuck,
                           const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CRealLightSensor* m_seLight;
};

#endif
