#ifndef PYTHONBRAITENBERGCONTROLLER_H_
#define PYTHONBRAITENBERGCONTROLLER_H_

/******************************************************************************/
/******************************************************************************/

#include "pythoncontrollerbase.h"
#include "epuckproximitysensor.h"
#include "reallightsensor.h"
#include "realbluelightsensor.h"
#include "realredlightsensor.h"
#include "contactsensor.h"
#include "groundsensor.h"
#include "groundmemorysensor.h"
#include "batterysensor.h"
#include "bluebatterysensor.h"
#include "redbatterysensor.h"

/******************************************************************************/
/******************************************************************************/

/* Python bridge for the Braitenberg experiment (20).
 * Sends the sensors of the experiment (no encoder or compass) and the robot
 * pose. The Python controller answers with the wheel outputs in [0, 1]
 * (0.5 = stop), which are applied with SetOutput as in the C++ controller. */
class CPythonBraitenbergController : public CPythonControllerBase
{
public:
    CPythonBraitenbergController(const char* pch_name, CEpuck* pc_epuck,
                                 const char* pch_script);

protected:
    void appendSensorData(char* buf, int& len);
    void handleResponse(const char* line);

private:
    CEpuckProximitySensor* m_seProx;
    CRealLightSensor*      m_seLight;
    CRealBlueLightSensor*  m_seBlueLight;
    CRealRedLightSensor*   m_seRedLight;
    CContactSensor*        m_seContact;
    CGroundSensor*         m_seGround;
    CGroundMemorySensor*   m_seGroundMemory;
    CBatterySensor*        m_seBattery;
    CBlueBatterySensor*    m_seBlueBattery;
    CRedBatterySensor*     m_seRedBattery;
};

/******************************************************************************/
/******************************************************************************/

#endif
