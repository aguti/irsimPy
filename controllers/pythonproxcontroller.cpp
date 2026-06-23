
#include <cstdio>
#include "pythonproxcontroller.h"

CPythonProxController::CPythonProxController(const char* pch_name,
                                              CEpuck*     pc_epuck,
                                              const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seProx = (CEpuckProximitySensor*) m_pcEpuck->GetSensor(SENSOR_PROXIMITY);
}

void CPythonProxController::appendSensorData(char* buf, int& len)
{
    double* prox = m_seProx->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seProx->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", prox[i]);
}
