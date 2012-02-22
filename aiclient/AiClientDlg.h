/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * Dialog Box class header. Change BaseBot to your Bot class type, to get your Bot to run
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

// AiClientDlg.h : header file
//

#if !defined(AFX_AICLIENTDLG_H__A808B939_F4EA_11D6_ADA7_000000000000__INCLUDED_)
#define AFX_AICLIENTDLG_H__A808B939_F4EA_11D6_ADA7_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AiClientTypes.h"
#include "Bot_Type.h"

/////////////////////////////////////////////////////////////////////////////
// CAiClientDlg dialog

class CAiClientDlg : public CDialog
{
// Construction
public:
	CAiClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAiClientDlg)
	enum { IDD = IDD_AICLIENT_DIALOG };
	CButton	m_cancel_ctrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAiClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAiClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG

	afx_msg void on_tcpip_message( void );

	DECLARE_MESSAGE_MAP()

public:
	void set_command_line( CString command_line );

	void end_dialog( void );

private:
	bool extract_parameters( COMMAND_LINE_PARAMETERS &parameters );

	CString m_command_line;

	// Made static to force it onto the heap. There is only ever one CAiClientDlg object
	static BOT_TYPE m_the_bot;
};

// Define the windows message id for a TCP/IP message
#define WM_TCPIP_MESSAGE                 0x4149

// Define the handler for the ipc message	
#define ON_WM_TCPIP_MESSAGE() \
	{ WM_TCPIP_MESSAGE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&on_tcpip_message },

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AICLIENTDLG_H__A808B939_F4EA_11D6_ADA7_000000000000__INCLUDED_)
