#ifndef PYTHONCONTACTCONTROLLER_H_
#define PYTHONCONTACTCONTROLLER_H_

#include "pythoncontrollerbase.h"
#include "contactsensor.h"

class CPythonContactController : public CPythonControllerBase
{
public:
    CPythonContactController(const char* pch_name, CEpuck* pc_epuck,
                             const char* pch_script);
protected:
    void appendSensorData(char* buf, int& len);
private:
    CContactSensor* m_seContact;
};

#endif
