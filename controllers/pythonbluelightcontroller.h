#ifndef PYTHONBLUELIGHTCONTROLLER_H_
#define PYTHONBLUELIGHTCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "realbluelightsensor.h"

class CPythonBlueLightController : public CPythonControllerBase
{
public:
    CPythonBlueLightController(const char* pch_name, CEpuck* pc_epuck,
                               const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CRealBlueLightSensor* m_seLight;
};

#endif
