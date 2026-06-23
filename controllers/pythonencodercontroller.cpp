
#include <cstdio>
#include "pythonencodercontroller.h"

CPythonEncoderController::CPythonEncoderController(const char* pch_name,
                                                    CEpuck*     pc_epuck,
                                                    const char* pch_script)
    : CPythonControllerBase(pch_name, pc_epuck, pch_script)
{
    m_seEncoder = (CEncoderSensor*) m_pcEpuck->GetSensor(SENSOR_ENCODER);
    m_seEncoder->InitEncoderSensor(m_pcEpuck);
}

void CPythonEncoderController::appendSensorData(char* buf, int& len)
{
    double* encoder = m_seEncoder->GetSensorReading(m_pcEpuck);
    for (int i = 0; i < m_seEncoder->GetNumberOfInputs(); i++)
        len += snprintf(buf + len, 4096 - len, " %f", encoder[i]);

    /* Also send the real position so Python can use it for odometry */
    len += snprintf(buf + len, 4096 - len, " %f %f",
                    m_pcEpuck->GetPosition().x,
                    m_pcEpuck->GetPosition().y);
}
