
#include "mod_user_defs.h"
#include "mod_libs.h"
#include "mod_library/mod_core.h"
#include "mod_library/mod_lora.h"

#define GW_ADDRESS  0x04
#define LOCAL_ADD   0x03


uint8_t radioInit(uint32_t varFreq,uint16_t varChannel);
uint8_t radioGet(uint8_t* bufIn,uint8_t rfLocalAddress);
uint8_t radioPut(uint8_t* bufIn, uint8_t varSize,uint8_t rfLocalAddress,uint8_t rfDestAddress);

