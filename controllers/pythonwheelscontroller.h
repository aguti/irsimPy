#ifndef PYTHONWHEELSCONTROLLER_H_
#define PYTHONWHEELSCONTROLLER_H_

/******************************************************************************/
/******************************************************************************/

#include "pythoncontrollerbase.h"

/******************************************************************************/
/******************************************************************************/

class CPythonWheelsController : public CPythonControllerBase
{
public:
    CPythonWheelsController(const char* pch_name, CEpuck* pc_epuck,
                            const char* pch_script);

protected:
    void appendSensorData(char* buf, int& len);
};

/******************************************************************************/
/******************************************************************************/

#endif
