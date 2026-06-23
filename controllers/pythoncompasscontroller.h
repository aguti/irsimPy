#ifndef PYTHONCOMPASSCONTROLLER_H_
#define PYTHONCOMPASSCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "encodersensor.h"
#include "compasssensor.h"

class CPythonCompassController : public CPythonControllerBase
{
public:
    CPythonCompassController(const char* pch_name, CEpuck* pc_epuck,
                             const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CEncoderSensor* m_seEncoder;
    CCompassSensor* m_seCompass;
};

#endif
