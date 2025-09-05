#pragma once

#include <sys/port.h>

extern port_t disc_io_port;
extern port_t disc_control_port;

void disc_select_port(void);
