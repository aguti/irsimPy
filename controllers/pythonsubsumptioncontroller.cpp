
/******************* INCLUDES ******************/
/***********************************************/

#include <cstdio>
#include "pythonsubsumptioncontroller.h"

using namespace std;

/******************************************************************************/
/******************************************************************************/

CPythonSubsumptionController::CPythonSubsumptionController(const char* pch_name,
                                                            CEpuck*     pc_epuck,
                                                            const char* pch_script,
                                                            int         n_write_to_file)
    : CPythonIri1Controller(pch_name, pc_epuck, pch_script, n_write_to_file)
{
}

/******************************************************************************/
/******************************************************************************/

/* Message format sent to Python: the IRI1 message followed by
 *
 *   orientation        (robot rotation in radians)
 *   write_to_file      (0 or 1, from the param file)
 */
void CPythonSubsumptionController::appendSensorData(char* buf, int& len)
{
    CPythonIri1Controller::appendSensorData(buf, len);

    len += snprintf(buf + len, 4096 - len, " %.17g %d",
                    m_pcEpuck->GetRotation(), m_nWriteToFile);
}

/******************************************************************************/
/******************************************************************************/
