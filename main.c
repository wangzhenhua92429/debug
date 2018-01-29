#include "debug.h"

int main()
{
    int i = 0;

    for(i = 0; i < 100; i++)
        JL_printf(LEVEL_LOG_ERROR, "test%d", i);
}
