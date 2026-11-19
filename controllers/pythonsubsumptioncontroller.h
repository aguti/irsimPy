#ifndef PYTHONSUBSUMPTIONCONTROLLER_H_
#define PYTHONSUBSUMPTIONCONTROLLER_H_

/******************************************************************************/
/******************************************************************************/

#include "pythoniri1controller.h"

/******************************************************************************/
/******************************************************************************/

/* Python bridge for the subsumption experiments (22 and 23).
 * Same messages as IRI1 (all sensors in, wheels/lights/LEDs out), plus the
 * robot orientation and the WRITE TO FILE flag of the param file, so the
 * Python controller can write the same outputFiles/ as the C++ one. */
class CPythonSubsumptionController : public CPythonIri1Controller
{
public:
    CPythonSubsumptionController(const char* pch_name, CEpuck* pc_epuck,
                                 const char* pch_script, int n_write_to_file);

protected:
    void appendSensorData(char* buf, int& len);
};

/******************************************************************************/
/******************************************************************************/

#endif
