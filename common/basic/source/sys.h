//!
//! $URL: $
//!
//!
//! \file       sys.h
//! \author     Anand.Kathiresan
//! \date       2017
//!
//! \brief      @ Header file for common macro definitons
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

//---------------------------------------------------------------------------------------------------
//! \brief		Safe malloc
//!
//! \param[in]	size	-	The desired allocation size.
//!
//! \return		A pointer to the new allocation.
//!
#define SMALLOC(memsize) safeMalloc((size_t)memsize)

//---------------------------------------------------------------------------
//! @def NELEMS(a)
//!
//! @brief Computes number of elements of an array.
//!
//! @param a An array.
#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

#endif //SYS_H
