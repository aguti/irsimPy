#ifndef PYTHONENCODERCONTROLLER_H_
#define PYTHONENCODERCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "encodersensor.h"

class CPythonEncoderController : public CPythonControllerBase
{
public:
    CPythonEncoderController(const char* pch_name, CEpuck* pc_epuck,
                             const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CEncoderSensor* m_seEncoder;
};

#endif
