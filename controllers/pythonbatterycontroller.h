#ifndef PYTHONBATTERYCONTROLLER_H_
#define PYTHONBATTERYCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "batterysensor.h"

class CPythonBatteryController : public CPythonControllerBase
{
public:
    CPythonBatteryController(const char* pch_name, CEpuck* pc_epuck,
                             const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CBatterySensor* m_seBattery;
};

#endif
