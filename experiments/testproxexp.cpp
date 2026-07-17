
/******************* INCLUDES ******************/
/***********************************************/

/******************** General ******************/

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <sys/time.h>

/******************** Simulator ****************/
#include <vector>
#include "simmath.h"
#include "testproxexp.h"
#include "programmedarena.h"

/******************** Sensors ******************/
#include "epuckproximitysensor.h"

/******************** Actuators ****************/
#include "wheelsactuator.h"

/******************** Controllers **************/
#include "pythonproxcontroller.h"


using namespace std;

/*Create Arena */
static const char* pchHeightMap = 
"%%%%%%%%%%%%%%%%%%%%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"
"%##################%"    
"%##################%"
"%##################%"
"%##################%"
"%%%%%%%%%%%%%%%%%%%%";


extern gsl_rng* rng;
extern long int rngSeed;

/*******************************************************************************/
/*******************************************************************************/

CTestProxExp::CTestProxExp(const char* pch_name, const char* paramsFile) :
	CExperiment(pch_name, COLLISION_MODEL_SIMPLE, COLLISION_HANDLER_POSITION)
{

	/* If there is not a parameter file input get default values*/
	if (paramsFile == NULL )
	{
		m_nRobotsNumber = 1;
		SetNumberOfEpucks(m_nRobotsNumber);
	}
	/* Else, extract info from the file */
	/* (NOTE: STILL NOT IMPLEMENTED */
	else{
		/* I SHOULD WORK ON THIS */
		m_nRobotsNumber = 1;
		SetNumberOfEpucks(m_nRobotsNumber);
	}
}

/******************************************************************************/
/******************************************************************************/

CArena* CTestProxExp::CreateArena()
{
	/* Create Arena */
	CArena* pcArena = NULL;
	pcArena = new CProgrammedArena("CProgrammedArena", 20, 20, 3.0, 3.0);
	((CProgrammedArena*)pcArena)->SetHeightPixelsFromChars(pchHeightMap, ' ', '#', '%');

	return pcArena;
}

/******************************************************************************/
/******************************************************************************/

void CTestProxExp::AddActuators(CEpuck* pc_epuck)
{
	/* Create and Add Wheels */
	char pchTemp[128];
	sprintf(pchTemp, "actuators_%s", pc_epuck->GetName());
	CActuator* pcActuator = NULL;
	pcActuator = new CWheelsActuator(pchTemp, pc_epuck);
	pc_epuck->AddActuator(pcActuator);
}

/******************************************************************************/
/******************************************************************************/

void CTestProxExp::AddSensors(CEpuck* pc_epuck)
{

	/* Create and add Proximity Sensor */
	CSensor* pcProxSensor = NULL;
	pcProxSensor = new CEpuckProximitySensor(252);
	pc_epuck->AddSensor(pcProxSensor);

}

/******************************************************************************/
/******************************************************************************/

void CTestProxExp::SetController(CEpuck* pc_epuck)
{
	/* Create and add test light controller */
	char pchTemp[128];
	sprintf(pchTemp, "testprox");
	CController* pcController = new CPythonProxController(pchTemp, pc_epuck, "python/prox_controller.py");
	pc_epuck->SetControllerType( CONTROLLER_TEST_PROX );
	pc_epuck->SetController(pcController);
}

/******************************************************************************/
/******************************************************************************/

void CTestProxExp::CreateAndAddEpucks(CSimulator* pc_simulator)
{
	double px, py, ptheta;
	CPythonControllerBase::ReadInitPose("python/prox_controller.py", px, py, ptheta);
	char label[100];
	for (int i = 0; i < m_nRobotsNumber; i++)
	{
		sprintf(label, "epuck%.4d", i);
		CEpuck* pcEpuck = CreateEpuck(label, px, py, ptheta);
		pc_simulator->AddEpuck(pcEpuck);
	}
}


/******************************************************************************/
/******************************************************************************/

