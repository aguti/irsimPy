
#include <cstdio>
#include "pythoncontactcontroller.h"

CPythonContactController::CPythonContactController(const char* pch_name,
                                                    CEpuck*     pc_epuck,
                                                    const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seContact = (CContactSensor*) m_pcEpuck->GetSensor(SENSOR_CONTACT);
}

void CPythonContactController::appendSensorData(char* buf, int& len)
{
    double* contact = m_seContact->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seContact->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", contact[i]);
}
