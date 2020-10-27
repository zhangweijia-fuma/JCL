
#include <signal.h>
#include "Debug.h"
#include "Timer.h"

#include "EdgeServer.h"

int main(int argc, char *argv[])
{
    LogI("");
    LogI("");
    LogI("");

    // Start system timer
    g_Timer.StartSystemTimer();

    CEdgeServer EdgeServer;
    if ( EdgeServer.InitialSystem() == false )
    {
        LogE("Error: Initial JamesSoft Edge Server failed.");
        return -1;
    }

    EdgeServer.StartSystem();
}
