#ifndef PYTHONGROUNDCONTROLLER_H_
#define PYTHONGROUNDCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "groundsensor.h"
#include "groundmemorysensor.h"

class CPythonGroundController : public CPythonControllerBase
{
public:
    CPythonGroundController(const char* pch_name, CEpuck* pc_epuck,
                            const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CGroundSensor*       m_seGround;
    CGroundMemorySensor* m_seGroundMemory;
};

#endif
