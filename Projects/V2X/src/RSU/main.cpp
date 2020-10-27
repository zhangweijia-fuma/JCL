
#include <signal.h>
#include "Debug.h"
#include "Timer.h"

#include "RSUModule.h"

#include "V2XPrimitive.h"

int main(int argc, char *argv[])
{
    LogI("");
    LogI("");
    LogI("");


    CRSUModule RSUModule;
    if ( RSUModule.InitialSystem() == false )
    {
        LogE("Initial JamesSoft RSU failed.");
        return -1;
    }

    RSUModule.StartSystem();
    return 0;
}
