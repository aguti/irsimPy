
#include <cstdio>
#include "pythonlightcontroller.h"

CPythonLightController::CPythonLightController(const char* pch_name,
                                                CEpuck*     pc_epuck,
                                                const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seLight = (CRealLightSensor*) m_pcEpuck->GetSensor(SENSOR_REAL_LIGHT);
}

void CPythonLightController::appendSensorData(char* buf, int& len)
{
    double* light = m_seLight->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seLight->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", light[i]);
}
