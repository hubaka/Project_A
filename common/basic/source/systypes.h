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
#ifndef SYSTYPES_H
#define SYSTYPES_H


/*---------------------------------------------------------------------------
** Typedefs
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @name Base types
*/
/*@{*/
typedef signed char			int8;
typedef signed char         sint8;
typedef unsigned char		uint8;
typedef signed short		int16;
typedef signed short		sint16;
typedef unsigned short		uint16;

typedef signed int			int32;
typedef signed int			sint32;
typedef unsigned int		uint32;
typedef signed long long 	int64;
typedef unsigned long long 	uint64;


/** @name Draft ISO fast names (for compatibility)
*/
/*@{*/
typedef int8				int8_t;
typedef int16				int16_t;
typedef int32				int32_t;
typedef int64               int64_t;
typedef uint8				uint8_t;
typedef uint16				uint16_t;
typedef uint32				uint32_t;
typedef uint64              uint64_t;


/*@}*/

/*!
** \brief	Our own character type
*/
typedef char CHAR;


/*!
** \brief	Our own float type
*/
typedef float FLOAT;

/*!
** \brief	Our own double type
*/
typedef double DOUBLE;

/*!
** \brief	Our void pointer type
*/
typedef void* PVOID;


#ifdef __cplusplus
}
#endif

// Begin MSVC++ Support

#ifdef _UNICODE
#define _tmemcpy  wmemcpy
#define _tmemmove  wmemmove
#define _tmemset  wmemset
#else
#define _tmemcpy  memcpy
#define _tmemmove  memmove
#define _tmemset  memset
#endif

#endif //SYSTYPES_H