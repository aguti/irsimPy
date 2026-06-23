
#include <cstdio>
#include "pythonswitchlightcontroller.h"

CPythonSwitchLightController::CPythonSwitchLightController(const char* pch_name,
                                                            CEpuck*     pc_epuck,
                                                            const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seLight = (CRealLightSensor*) m_pcEpuck->GetSensor(SENSOR_REAL_LIGHT);
}

void CPythonSwitchLightController::appendSensorData(char* buf, int& len)
{
    double* light = m_seLight->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seLight->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", light[i]);
}

void CPythonSwitchLightController::handleResponse(const char* line)
{
    double left = 0.0, right = 0.0;
    int    sw   = -1; /* -1 = no switch command */
    sscanf(line, "%lf %lf %d", &left, &right, &sw);
    m_acWheels->SetSpeed(left, right);
    if (sw >= 0)
        m_seLight->SwitchNearestLight(sw);
}
