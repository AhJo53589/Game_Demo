#include "Console.h"

namespace pool
{
	void addConsole()
	{
#ifdef _DEBUG
		AllocConsole(); 

		freopen("CONIN$","r",stdin);
		freopen("CONOUT$","w",stdout);
		freopen("CONOUT$","w",stderr);
#endif
	}
} 