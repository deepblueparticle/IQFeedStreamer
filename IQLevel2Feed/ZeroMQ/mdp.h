/*
Copyright (C) 2018, Juha Hellén

This file is part of Bonsai, a proprietary library
for quantitative trading.

QuantLib is free software: you can redistribute it and/or modify it
under the terms of the QuantLib license.

This program is distributed WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.
*/
#ifndef __MDP_H_INCLUDED__
#define __MDP_H_INCLUDED__

//  This is the version of MDP/Client we implement
#define MDPC_CLIENT         "MDPC01"

//  This is the version of MDP/Worker we implement
#define MDPW_WORKER         "MDPW01"

//  MDP/Server commands, as strings
#define MDPW_READY          "\001"
#define MDPW_REQUEST        "\002"
#define MDPW_REPLY          "\003"
#define MDPW_HEARTBEAT      "\004"
#define MDPW_DISCONNECT     "\005"

// Portfolio commands, as strings
#define WTF_QUANTITY         "\006"
#define WTF_ACTION           "\007"
#define HAI_PLZ_FILL         "\008"
#define HAI_PLZ_PRINT        "\011"
#define HAI_PLZ_PLACE_ORDER  "\012"
#define HAI_PLZ_CANCEL_ORDER "\013"
#define HAI_PLZ_MODIFY_ORDER "\015"
#define LOL_U_HAS_ORDER      "\014"

static char *mdps_commands[] = {
	NULL, (char*)"READY", (char*)"REQUEST", (char*)"REPLY", (char*)"HEARTBEAT", (char*)"DISCONNECT"
};

#endif