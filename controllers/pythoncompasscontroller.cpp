
#include <cstdio>
#include "pythoncompasscontroller.h"

CPythonCompassController::CPythonCompassController(const char* pch_name,
                                                    CEpuck*     pc_epuck,
                                                    const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seEncoder = (CEncoderSensor*) m_pcEpuck->GetSensor(SENSOR_ENCODER);
    m_seEncoder->InitEncoderSensor(m_pcEpuck);
    m_seCompass = (CCompassSensor*) m_pcEpuck->GetSensor(SENSOR_COMPASS);
}

void CPythonCompassController::appendSensorData(char* buf, int& len)
{
    double* encoder = m_seEncoder->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seEncoder->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", encoder[i]);

    double* compass = m_seCompass->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seCompass->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", compass[i]);
}
