#ifndef _LOGS_H_
#define _LOGS_H_

#include <dbgPort.h>

#define LOG(x) dbgu_print_ascii(x); dbgu_print_ascii("\r\n")
#define LOG_hex(x) dbgu_print_hex8(x); dbgu_print_ascii("\r\n")

#endif
