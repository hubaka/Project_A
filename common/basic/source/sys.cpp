//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
//!
//! \brief      @ Source file for 
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include "sys.h"


//---------------------------------------------------------------------------------------------------
//! \brief		Safe malloc
//!
//! \param[in]	size	-	The desired allocation size.
//!
//! \return		A pointer to the new allocation.
//!
PVOID safeMalloc(size_t memsize)
{
    PVOID data;

    data = malloc(memsize);
    if (data == NULL)
    {
        fprintf(stderr, "\nOut of memory.\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return data;
}
