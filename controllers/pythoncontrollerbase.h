#ifndef PYTHONCONTROLLERBASE_H_
#define PYTHONCONTROLLERBASE_H_

/******************************************************************************/
/******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "controller.h"
#include "wheelsactuator.h"

/******************************************************************************/
/******************************************************************************/

class CPythonControllerBase : public CController
{
public:
    CPythonControllerBase(const char* pch_name, CEpuck* pc_epuck,
                          const char* pch_script);
    virtual ~CPythonControllerBase();

    void SimulationStep(unsigned n_step_number, double f_time,
                        double f_step_interval);

    static void ReadInitPose(const char* pch_script,
                             double& x, double& y, double& theta);

    /* Reads ARENA_CELLS_X/Y, ARENA_WIDTH/HEIGHT and ARENA_MAP from the
       script. Returns the map as a flat string (caller deletes it). */
    static char* ReadArenaConfig(const char* pch_script,
                                 int& n_cells_x, int& n_cells_y,
                                 double& f_width, double& f_height);

protected:
    /* Subclass appends sensor values to buf (already prefixed with
       "step time interval"). len must be updated accordingly. */
    virtual void appendSensorData(char* buf, int& len) = 0;

    /* Subclass parses the response line and drives actuators.
       Default implementation reads "left right" and calls SetSpeed. */
    virtual void handleResponse(const char* line);

    CWheelsActuator* m_acWheels;
    CEpuck*          m_pcEpuck;

private:
    pid_t m_pid;
    int   m_fdWrite;
    FILE* m_pfRead;
    char  m_lineBuf[512];
};

/******************************************************************************/
/******************************************************************************/

#endif
