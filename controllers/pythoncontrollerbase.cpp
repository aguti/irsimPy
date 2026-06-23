
/******************* INCLUDES ******************/
/***********************************************/

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <iostream>

#include "pythoncontrollerbase.h"

using namespace std;

/******************************************************************************/
/******************************************************************************/

CPythonControllerBase::CPythonControllerBase(const char* pch_name,
                                              CEpuck*     pc_epuck,
                                              const char* pch_script)
    : CController(pch_name, pc_epuck),
      m_pid(-1), m_fdWrite(-1), m_pfRead(NULL)
{
    m_pcEpuck  = pc_epuck;
    m_acWheels = (CWheelsActuator*) m_pcEpuck->GetActuator(ACTUATOR_WHEELS);

    int pipe_to[2], pipe_from[2];
    if (pipe(pipe_to) < 0 || pipe(pipe_from) < 0) {
        perror("pipe");
        return;
    }

    m_pid = fork();
    if (m_pid < 0) {
        perror("fork");
        return;
    }

    if (m_pid == 0) {
        /* Child: redirect stdin/stdout and exec python */
        dup2(pipe_to[0],   STDIN_FILENO);
        dup2(pipe_from[1], STDOUT_FILENO);
        close(pipe_to[1]);
        close(pipe_from[0]);
        execl("/usr/bin/python3", "python3", pch_script, NULL);
        perror("execl");
        _exit(1);
    }

    /* Parent: keep write-end of pipe_to, read-end of pipe_from */
    close(pipe_to[0]);
    close(pipe_from[1]);
    m_fdWrite = pipe_to[1];
    m_pfRead  = fdopen(pipe_from[0], "r");
}

/******************************************************************************/
/******************************************************************************/

CPythonControllerBase::~CPythonControllerBase()
{
    if (m_fdWrite >= 0) close(m_fdWrite);
    if (m_pfRead)       fclose(m_pfRead);
    if (m_pid > 0)      kill(m_pid, SIGTERM);
}

/******************************************************************************/
/******************************************************************************/

void CPythonControllerBase::SimulationStep(unsigned n_step_number,
                                            double   f_time,
                                            double   f_step_interval)
{
    if (m_fdWrite < 0 || !m_pfRead) return;

    char buf[4096];
    int  len = snprintf(buf, sizeof(buf), "%u %f %f",
                        n_step_number, f_time, f_step_interval);

    appendSensorData(buf, len);

    buf[len++] = '\n';
    write(m_fdWrite, buf, len);

    if (!fgets(m_lineBuf, sizeof(m_lineBuf), m_pfRead)) {
        fprintf(stderr, "[PythonController] read error\n");
        return;
    }
    handleResponse(m_lineBuf);
}

/******************************************************************************/
/******************************************************************************/

void CPythonControllerBase::handleResponse(const char* line)
{
    double left = 0.0, right = 0.0;
    sscanf(line, "%lf %lf", &left, &right);
    m_acWheels->SetSpeed(left, right);
}

/******************************************************************************/
/******************************************************************************/
