
/******************* INCLUDES ******************/
/***********************************************/

#include <cstdio>
#include <cstring>
#include <cstdlib>
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
      m_pchScript(pch_script), m_pid(-1), m_fdWrite(-1), m_pfRead(NULL)
{
    m_pcEpuck  = pc_epuck;
    m_acWheels = (CWheelsActuator*) m_pcEpuck->GetActuator(ACTUATOR_WHEELS);

    /* If Python dies, writing to its pipe must fail instead of killing the
       simulator, so SimulationStep can report the error */
    signal(SIGPIPE, SIG_IGN);

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
        /* The Python process has died: its error is already on the terminal */
        fprintf(stderr, "[PythonController] %s stopped. Fix the Python error shown above.\n",
                m_pchScript);
        exit(1);
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

void CPythonControllerBase::ReadInitPose(const char* pch_script,
                                          double& x, double& y, double& theta)
{
    x = y = theta = 0.0;
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "python3 -c \"g={}; exec(open('%s').read(),g); "
        "print(g.get('INIT_X',0.0), g.get('INIT_Y',0.0), g.get('INIT_THETA',0.0))\" < /dev/null",
        pch_script);
    FILE* f = popen(cmd, "r");
    if (!f) return;
    fscanf(f, "%lf %lf %lf", &x, &y, &theta);
    pclose(f);
}

/******************************************************************************/
/******************************************************************************/

char* CPythonControllerBase::ReadArenaConfig(const char* pch_script,
                                              int& n_cells_x, int& n_cells_y,
                                              double& f_width, double& f_height)
{
    n_cells_x = n_cells_y = 20;
    f_width = f_height = 3.0;

    /* Write a temporary Python reader script (one per process, so several
       simulators can start at the same time) */
    char tmpPath[64];
    snprintf(tmpPath, sizeof(tmpPath), "/tmp/irsim_arena_reader_%d.py", (int) getpid());
    FILE* fw = fopen(tmpPath, "w");
    if (!fw) { fprintf(stderr, "[PythonController] Cannot write arena reader\n"); return NULL; }

    fprintf(fw,
        "g={}\n"
        "exec(open(\"%s\").read(), g)\n"
        "print(int(g.get(\"ARENA_CELLS_X\", 20)))\n"
        "print(int(g.get(\"ARENA_CELLS_Y\", 20)))\n"
        "print(float(g.get(\"ARENA_WIDTH\",  3.0)))\n"
        "print(float(g.get(\"ARENA_HEIGHT\", 3.0)))\n"
        "rows = g.get(\"ARENA_MAP\", [])\n"
        "print(len(rows))\n"
        "for r in reversed(rows):\n"
        "    print(r)\n",
        pch_script);
    fclose(fw);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "python3 %s < /dev/null", tmpPath);
    FILE* f = popen(cmd, "r");
    if (!f) { fprintf(stderr, "[PythonController] Cannot run arena reader\n"); return NULL; }

    char line[512];

    /* No output means the script failed: its error is already on the terminal */
    if (!fgets(line, sizeof(line), f)) {
        pclose(f);
        unlink(tmpPath);
        fprintf(stderr, "[PythonController] Cannot read the arena from %s. "
                        "Fix the Python error shown above.\n", pch_script);
        exit(1);
    }

    /* Arena dimensions */
    n_cells_x = atoi(line);
    fgets(line, sizeof(line), f); n_cells_y = atoi(line);
    fgets(line, sizeof(line), f); f_width = atof(line);
    fgets(line, sizeof(line), f); f_height = atof(line);

    /* Map rows — concatenate into one flat string */
    fgets(line, sizeof(line), f); int nRows = atoi(line);
    char* pchMap = new char[nRows * n_cells_x + 1];
    pchMap[0] = '\0';
    char rowBuf[256];
    for (int i = 0; i < nRows; i++) {
        fgets(rowBuf, sizeof(rowBuf), f);
        int len = strlen(rowBuf);
        while (len > 0 && (rowBuf[len-1] == '\n' || rowBuf[len-1] == '\r')) rowBuf[--len] = '\0';
        strncat(pchMap, rowBuf, n_cells_x);
    }

    pclose(f);
    unlink(tmpPath);
    return pchMap;
}

/******************************************************************************/
/******************************************************************************/
