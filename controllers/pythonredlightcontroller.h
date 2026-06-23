#ifndef PYTHONREDLIGHTCONTROLLER_H_
#define PYTHONREDLIGHTCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "realredlightsensor.h"

class CPythonRedLightController : public CPythonControllerBase
{
public:
    CPythonRedLightController(const char* pch_name, CEpuck* pc_epuck,
                              const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CRealRedLightSensor* m_seLight;
};

#endif
