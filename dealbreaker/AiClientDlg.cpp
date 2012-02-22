/**
 * Diplomacy AI Client - Part of the DAIDE project.
 *
 * Main Dialog Box. Just kicks off the Bot and passes the TCP/IP messages to it
 *
 * (C) David Norman 2002 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

// AiClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AiClient.h"
#include "AiClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAiClientDlg dialog

CAiClientDlg::CAiClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAiClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAiClientDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAiClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAiClientDlg)
	DDX_Control(pDX, IDCANCEL, m_cancel_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAiClientDlg, CDialog)
	//{{AFX_MSG_MAP(CAiClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_WM_TCPIP_MESSAGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAiClientDlg message handlers

// Declare static member variables
BOT_TYPE CAiClientDlg::m_the_bot;

BOOL CAiClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	COMMAND_LINE_PARAMETERS parameters;
	bool bot_initialised_ok;

	extract_parameters( parameters );

	bot_initialised_ok = m_the_bot.initialise( parameters, this );

	if ( bot_initialised_ok == false )
	{
		AfxMessageBox("Couldn't initialise Bot");

		OnCancel();
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAiClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAiClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAiClientDlg::set_command_line( CString command_line )
{
	m_command_line = command_line;
}

bool CAiClientDlg::extract_parameters( COMMAND_LINE_PARAMETERS &parameters )
{
	int search_start;					// Position to start searching command line
	int param_start;					// Start of the next parameter
	int param_end;						// End of the next parameter
	char param_token;					// The token specifying the parameter type
	CString parameter;					// The parameter
	bool extracted_ok = true;			// Whether the parameters were OK
	bool param_processed;				// Whether a parameter has been processed by the Bot
	CString bot_specific_params;		// Parameters specific to this Bot

	parameters.ip_specified = false;
	parameters.name_specified = false;
	parameters.port_specified = false;
	parameters.log_level_specified = false;
	parameters.reconnection_specified = false;

	// Strip the program name off the command line
	if ( m_command_line[ 0 ] == '"' )
	{
		// Program name is in quotes.
		param_start = m_command_line.Find( '"', 1 );

		if ( param_start != -1 )
		{
			m_command_line = m_command_line.Mid( param_start + 1 );
		}
	}
	else
	{
		// Program name is not quoted, so is terminated by a space
		param_start = m_command_line.Find( ' ' );

		if ( param_start != -1 )
		{
			m_command_line = m_command_line.Mid( param_start );
		}
	}

	param_start = m_command_line.Find('-', 0 );

	while ( param_start != -1 )
	{
		param_token = m_command_line[ param_start + 1 ];

		param_end = m_command_line.Find(" ", param_start );

		if ( param_end == -1 )
		{
			parameter = m_command_line.Mid( param_start + 2 );

			search_start = m_command_line.GetLength();
		}
		else
		{
			parameter = m_command_line.Left( param_end ).Mid( param_start + 2 );

			search_start = param_end;
		}

		switch ( param_token )
		{
			case 'S':
			case 's':
			{
				parameters.name_specified = true;
				parameters.server_name = parameter;
				
				break;
			}

			case 'I':
			case 'i':
			{
				parameters.ip_specified = true;
				parameters.server_name = parameter;

				break;
			}

			case 'P':
			case 'p':
			{
				parameters.port_specified = true;
				parameters.port_number = atoi( parameter );
				
				break;
			}

			case 'L':
			case 'l':
			{
				parameters.log_level_specified = true;
				parameters.log_level = atoi( parameter );
				
				break;
			}

			case 'R':
			case 'r':
			{
				if ( parameter[3] == ':' )
				{
					parameters.reconnection_specified = true;
					parameters.reconnect_power = parameter.Left( 3 );
					parameters.reconnect_power.MakeUpper();
					parameters.reconnect_passcode = atoi( parameter.Mid( 4 ) );
				}
				else
				{
					AfxMessageBox("-r should be followed by 'POW:passcode'\n"
								  "POW should be three characters");
				}

				break;
			}

			case 'M':
			case 'm':
			{
				ShowWindow( SW_MINIMIZE );

				break;
			}

			case 'H':
			case 'h':
			{
				ShowWindow( SW_HIDE );

				break;
			}

			default:
			{
				param_processed = m_the_bot.process_command_line_parameter( param_token, parameter );

				if ( param_processed == false )
				{
					bot_specific_params = m_the_bot.report_command_line_parameters();

					AfxMessageBox("Usage : AiClient [-sServerName|-iIPAddress] [-pPortNumber] "
								  "[-lLogLevel] [-rPOW:passcode] [-m] [-h] " + bot_specific_params );

					extracted_ok = false;
				}
			}
		}

		param_start = m_command_line.Find('-', search_start );
	}

	if ( ( parameters.ip_specified ) && ( parameters.name_specified ) )
	{
		AfxMessageBox("You must not specify Server name and IP address");

		extracted_ok = false;
	}

	return extracted_ok;
}

void CAiClientDlg::on_tcpip_message( void )
{
	m_the_bot.process_incoming_messages();
}

void CAiClientDlg::end_dialog( void )
{
	OnCancel();
}

