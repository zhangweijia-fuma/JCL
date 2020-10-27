
#include <signal.h>
#include "Debug.h"
#include "Timer.h"

#include "RSUManager.h"

int main(int argc, char *argv[])
{
    LogI("");
    LogI("");
    LogI("");

    // Start system timer
    g_Timer.StartSystemTimer();

    CRSUManager RSUManager;
    if ( RSUManager.InitialSystem() == false )
    {
        LogE("Error: Initial JamesSoft Cloud Server failed.");
        return -1;
    }

    RSUManager.StartSystem();
}
