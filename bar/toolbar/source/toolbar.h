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
#ifndef TOOL_BAR_H
#define TOOL_BAR_H
//---------------------------------------------------------------------------


namespace bar
{
	class ToolBar {
	public:
		ToolBar(void);
		~ToolBar(void);
		
		HWND createToolBar(HWND hWnd);
		HWND getToolBarInstance(void);


	private:
		HWND	m_hToolbar;

	};

} //namespace bar

#endif //TOOL_BAR_H