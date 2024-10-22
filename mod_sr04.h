
#include "mod_user_defs.h"
#include "mod_libs.h"

#include "mod_library/mod_core.h"
#include "mod_library/mod_timerA.h"


void sr04Init(void);
void sr04GetCm(uint16_t* pBuffer);
void sr04GetUs(uint16_t* pBuffer);
void sr04Print(uint16_t* pBuffer);
