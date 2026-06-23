#ifndef PYTHONSWITCHLIGHTCONTROLLER_H_
#define PYTHONSWITCHLIGHTCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "reallightsensor.h"

class CPythonSwitchLightController : public CPythonControllerBase
{
public:
    CPythonSwitchLightController(const char* pch_name, CEpuck* pc_epuck,
                                 const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
    void handleResponse(const char* line);
private:
    CRealLightSensor* m_seLight;
};

#endif
