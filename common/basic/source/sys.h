//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
//!
//! \brief      @ Header file for 
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
#ifndef SYS_H
#define SYS_H

#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include "systypes.h"

PVOID safeMalloc(size_t memsize);

#define SMALLOC(memsize) safeMalloc((size_t)memsize)

/// @def NELEMS(a)
///
/// @brief Computes number of elements of an array.
///
/// @param a An array.
#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

#endif //SYS_H
