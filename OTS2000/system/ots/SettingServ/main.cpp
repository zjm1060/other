#include "stdafx.h"
#include "SettingServ.h"

CSettingServ *g_pServ;

int main( int argc, char ** argv ) 
{
	// create the service-object
	CSettingServ serv;
	g_pServ = &serv;

	// RegisterService() checks the parameterlist for predefined switches
	// (such as -d or -i etc.; see NTService.h for possible switches) and
	// starts the service's functionality.
	// You can use the return value from "RegisterService()"
	// as your exit-code.
	char *argvv[2]={ "SettingServ.exe","-d"};
	int argcc = 2;
	return serv.RegisterService(argcc, argvv);
}
