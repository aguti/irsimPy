#ifndef PYTHONIRI1CONTROLLER_H_
#define PYTHONIRI1CONTROLLER_H_

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
#include "realredlightsensor.h"
#include "encodersensor.h"
#include "compasssensor.h"

/******************************************************************************/
/******************************************************************************/

class CPythonIri1Controller : public CPythonControllerBase
{
public:
    CPythonIri1Controller(const char* pch_name, CEpuck* pc_epuck,
                          const char* pch_script, int n_write_to_file);

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
    CEncoderSensor*        m_seEncoder;
    CCompassSensor*        m_seCompass;

    int m_nWriteToFile;
};

/******************************************************************************/
/******************************************************************************/

#endif
