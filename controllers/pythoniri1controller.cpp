
/******************* INCLUDES ******************/
/***********************************************/

#include <cstdio>
#include "pythoniri1controller.h"

using namespace std;

/******************************************************************************/
/******************************************************************************/

CPythonIri1Controller::CPythonIri1Controller(const char* pch_name,
                                              CEpuck*     pc_epuck,
                                              const char* pch_script,
                                              int         n_write_to_file)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script),
      m_nWriteToFile(n_write_to_file)
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
    m_seEncoder      = (CEncoderSensor*)        m_pcEpuck->GetSensor(SENSOR_ENCODER);
    m_seEncoder->InitEncoderSensor(m_pcEpuck);
    m_seCompass      = (CCompassSensor*)        m_pcEpuck->GetSensor(SENSOR_COMPASS);
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
 *   contact[0..N-1]    (N values, typically 8)
 *   ground[0..2]       (3 values)
 *   groundmemory[0]    (1 value)
 *   battery[0]         (1 value)
 *   bluebattery[0]     (1 value)
 *   redbattery[0]      (1 value)
 *   encoder[0..1]      (2 values)
 *   compass[0]         (1 value)
 *   pos_x pos_y        (robot position)
 */
void CPythonIri1Controller::appendSensorData(char* buf, int& len)
{
    int i;

    double* prox = m_seProx->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seProx->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", prox[i]);

    double* light = m_seLight->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seLight->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", light[i]);

    double* bluelight = m_seBlueLight->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seBlueLight->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", bluelight[i]);

    double* redlight = m_seRedLight->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seRedLight->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", redlight[i]);

    double* contact = m_seContact->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seContact->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", contact[i]);

    double* ground = m_seGround->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seGround->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", ground[i]);

    double* gmem = m_seGroundMemory->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seGroundMemory->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", gmem[i]);

    double* battery = m_seBattery->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seBattery->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", battery[i]);

    double* bluebattery = m_seBlueBattery->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seBlueBattery->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", bluebattery[i]);

    double* redbattery = m_seRedBattery->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seRedBattery->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", redbattery[i]);

    double* encoder = m_seEncoder->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seEncoder->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", encoder[i]);

    double* compass = m_seCompass->GetSensorReading(m_pcEpuck);
    for (i = 0; i < m_seCompass->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", compass[i]);

    /* Robot position (useful for logging on the Python side) */
    len += snprintf(buf + len, 4096 - len, " %f %f",
                    m_pcEpuck->GetPosition().x,
                    m_pcEpuck->GetPosition().y);
}

/******************************************************************************/
/******************************************************************************/

/* Response format from Python (space-separated):
 *
 *   left right sw_light sw_blue sw_red led0 led1 led2 led3 led4 led5 led6 led7
 *
 *   sw_*  : 0=OFF nearest, 1=ON nearest, -1=no action
 *   led_N : LED color (0=BLACK, 1=RED, 2=GREEN, 3=BLUE, 4=YELLOW, 5=WHITE), -1=no change
 */
void CPythonIri1Controller::handleResponse(const char* line)
{
    double left = 0.0, right = 0.0;
    int    sw_light = -1, sw_blue = -1, sw_red = -1;
    int    leds[8]  = {-1, -1, -1, -1, -1, -1, -1, -1};

    sscanf(line, "%lf %lf %d %d %d %d %d %d %d %d %d %d %d",
           &left, &right,
           &sw_light, &sw_blue, &sw_red,
           &leds[0], &leds[1], &leds[2], &leds[3],
           &leds[4], &leds[5], &leds[6], &leds[7]);

    m_acWheels->SetSpeed(left, right);

    if (sw_light >= 0) m_seLight->SwitchNearestLight(sw_light);
    if (sw_blue  >= 0) m_seBlueLight->SwitchNearestLight(sw_blue);
    if (sw_red   >= 0) m_seRedLight->SwitchNearestLight(sw_red);

    /* Find first non-(-1) LED value and check if all are the same */
    int first_color = -1;
    for (int i = 0; i < 8; i++)
        if (leds[i] >= 0) { first_color = leds[i]; break; }

    if (first_color >= 0) {
        bool all_same = true;
        for (int i = 0; i < 8; i++)
            if (leds[i] >= 0 && leds[i] != first_color) { all_same = false; break; }

        if (all_same) {
            /* SetAllColoredLeds calls SetColor() internally — updates visual body color */
            m_pcEpuck->SetAllColoredLeds(first_color);
        } else {
            /* Mixed colors: set individual LEDs, then update body color to LED 0's color */
            for (int i = 0; i < 8; i++)
                if (leds[i] >= 0) m_pcEpuck->SetColoredLed(i, leds[i]);
            m_pcEpuck->SetAllColoredLeds(leds[0] >= 0 ? leds[0] : first_color);
        }
    }
}

/******************************************************************************/
/******************************************************************************/
