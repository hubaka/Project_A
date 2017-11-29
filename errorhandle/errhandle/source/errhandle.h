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

//---------------------------------------------------------------------------
#ifndef errhandle_h
#define errhandle_h
//---------------------------------------------------------------------------

namespace errhandle
{
	class ErrHandle {
	public:
		ErrHandle(void);

		~ErrHandle(void);

		void 
		getErrorInfo(
			LPTSTR lpszFunction
		);
	};

} //namespace errhandle

#endif //errhandle_h