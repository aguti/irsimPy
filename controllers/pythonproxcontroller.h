#ifndef PYTHONPROXCONTROLLER_H_
#define PYTHONPROXCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "epuckproximitysensor.h"

class CPythonProxController : public CPythonControllerBase
{
public:
    CPythonProxController(const char* pch_name, CEpuck* pc_epuck,
                          const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CEpuckProximitySensor* m_seProx;
};

#endif
