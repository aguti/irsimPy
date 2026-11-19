
/******************* INCLUDES ******************/
/***********************************************/

#include <cstdio>
#include "pythonbraitenbergcontroller.h"

using namespace std;

/******************************************************************************/
/******************************************************************************/

CPythonBraitenbergController::CPythonBraitenbergController(const char* pch_name,
                                                            CEpuck*     pc_epuck,
                                                            const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seProx         = (CEpuckProximitySensor*) m_pcEpuck->GetSensor(SENSOR_PROXIMITY);
    m_seLight        = (CRealLightSensor*)      m_pcEpuck->GetSensor(SENSOR_REAL_LIGHT);
    m_seBlueLight    = (CRealBlueLightSensor*)  m_pcEpuck->GetSensor(SENSOR_REAL_BLUE_LIGHT);
    m_seRedLight     = (CRealRedLightSensor*)   m_pcEpuck->GetSensor(SENSOR_REAL_RED_LIGHT);
    m_seContact      = (CContactSensor*)        m_pcEpuck->GetSensor(SENSOR_CONTACT);
    m_seGround       = (CGroundSensor*)         m_pcEpuck->GetSensor(SENSOR_GROUND);
    m_seGroundMemory = (CGroundMemorySensor*)   m_pcEpuck->GetSensor(SENSOR_GROUND_MEMORY);
    m_seBattery      = (CBatterySensor*)        m_pcEpuck->GetSensor(SENSOR_BATTERY);
    m_seBlueBattery  = (CBlueBatterySensor*)    m_pcEpuck->GetSensor(SENSOR_BLUE_BATTERY);
    m_seRedBattery   = (CRedBatterySensor*)     m_pcEpuck->GetSensor(SENSOR_RED_BATTERY);
}

/******************************************************************************/
/******************************************************************************/

/* Message format sent to Python (all space-separated on one line):
 *
 *   step time interval
 *   prox[0..7]         (8 values)
 *   light[0..7]        (8 values)
 *   bluelight[0..7]    (8 values)
 *   redlight[0..7]     (8 values)
 *   contact[0..7]      (8 values)
 *   ground[0..2]       (3 values)
 *   groundmemory[0]    (1 value)
 *   battery[0]         (1 value)
 *   bluebattery[0]     (1 value)
 *   redbattery[0]      (1 value)
 *   pos_x pos_y        (robot position)
 *   orientation        (robot rotation in radians)
 */
static void appendValues(char* buf, int& len, const double* values, int n)
{
    for (int i = 0; i < n; i++)
        len += snprintf(buf + len, 4096 - len, " %.17g", values[i]);
}

void CPythonBraitenbergController::appendSensorData(char* buf, int& len)
{
    appendValues(buf, len, m_seProx->GetSensorReading(m_pcEpuck),         m_seProx->GetNumberOfInputs());
    appendValues(buf, len, m_seLight->GetSensorReading(m_pcEpuck),        m_seLight->GetNumberOfInputs());
    appendValues(buf, len, m_seBlueLight->GetSensorReading(m_pcEpuck),    m_seBlueLight->GetNumberOfInputs());
    appendValues(buf, len, m_seRedLight->GetSensorReading(m_pcEpuck),     m_seRedLight->GetNumberOfInputs());
    appendValues(buf, len, m_seContact->GetSensorReading(m_pcEpuck),      m_seContact->GetNumberOfInputs());
    appendValues(buf, len, m_seGround->GetSensorReading(m_pcEpuck),       m_seGround->GetNumberOfInputs());
    appendValues(buf, len, m_seGroundMemory->GetSensorReading(m_pcEpuck), m_seGroundMemory->GetNumberOfInputs());
    appendValues(buf, len, m_seBattery->GetSensorReading(m_pcEpuck),      m_seBattery->GetNumberOfInputs());
    appendValues(buf, len, m_seBlueBattery->GetSensorReading(m_pcEpuck),  m_seBlueBattery->GetNumberOfInputs());
    appendValues(buf, len, m_seRedBattery->GetSensorReading(m_pcEpuck),   m_seRedBattery->GetNumberOfInputs());

    len += snprintf(buf + len, 4096 - len, " %.17g %.17g %.17g",
                    m_pcEpuck->GetPosition().x,
                    m_pcEpuck->GetPosition().y,
                    m_pcEpuck->GetRotation());
}

/******************************************************************************/
/******************************************************************************/

/* Response format from Python:
 *
 *   left_output right_output     (each in [0, 1], 0.5 = stop)
 */
void CPythonBraitenbergController::handleResponse(const char* line)
{
    double left = 0.5, right = 0.5;
    sscanf(line, "%lf %lf", &left, &right);
    m_acWheels->SetOutput(0, left);
    m_acWheels->SetOutput(1, right);
}

/******************************************************************************/
/******************************************************************************/
