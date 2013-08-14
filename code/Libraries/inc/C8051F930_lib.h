//-----------------------------------------------------------------------------
// C8051F930_lib.h
//-----------------------------------------------------------------------------
// Copyright (C) 2009 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Driver for the 'F93x power management function.
//
// Target:         C8051F93x-C8051F90x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    -23 JUL 2009
//
#ifndef C8051F930_LIB_H
#define C8051F930_LIB_H
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Interrupt.h"
#include "C8051f93x_IOPort.h"
#include "C8051f93x_SysClk.h"
#include "C8051f93x_Timer01.h"
#include "C8051f93x_Timer23.h"
#include "C8051f93x_UART0.h"
#include "C8051f93x_ADC.H"
#include "smartclock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality

 
//-----------------------------------------------------------------------------
// Global Configuration Options
//-----------------------------------------------------------------------------

//#define SYSCLK  20000000			   // deleted by scf

#define SMARTCLOCK_ENABLED   1

#endif