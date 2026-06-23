
#include <cstdio>
#include "pythongroundcontroller.h"

CPythonGroundController::CPythonGroundController(const char* pch_name,
                                                  CEpuck*     pc_epuck,
                                                  const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seGround       = (CGroundSensor*)       m_pcEpuck->GetSensor(SENSOR_GROUND);
    m_seGroundMemory = (CGroundMemorySensor*) m_pcEpuck->GetSensor(SENSOR_GROUND_MEMORY);
}

void CPythonGroundController::appendSensorData(char* buf, int& len)
{
    double* ground = m_seGround->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seGround->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", ground[i]);

    double* gmem = m_seGroundMemory->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seGroundMemory->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", gmem[i]);
}
