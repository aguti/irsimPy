
#include <cstdio>
#include "pythonbatterycontroller.h"

CPythonBatteryController::CPythonBatteryController(const char* pch_name,
                                                    CEpuck*     pc_epuck,
                                                    const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seBattery = (CBatterySensor*) m_pcEpuck->GetSensor(SENSOR_BATTERY);
}

void CPythonBatteryController::appendSensorData(char* buf, int& len)
{
    double* battery = m_seBattery->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seBattery->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", battery[i]);
}
