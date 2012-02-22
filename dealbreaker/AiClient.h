/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * Main header file. Automatically generated.
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

// AiClient.h : main header file for the AICLIENT application
//

#if !defined(AFX_AICLIENT_H__A808B937_F4EA_11D6_ADA7_000000000000__INCLUDED_)
#define AFX_AICLIENT_H__A808B937_F4EA_11D6_ADA7_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAiClientApp:
// See AiClient.cpp for the implementation of this class
//

class CAiClientApp : public CWinApp
{
public:
	CAiClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAiClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAiClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AICLIENT_H__A808B937_F4EA_11D6_ADA7_000000000000__INCLUDED_)
