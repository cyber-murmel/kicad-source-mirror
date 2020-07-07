///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "panel_pcbnew_display_origin_base.h"

///////////////////////////////////////////////////////////////////////////

PANEL_PCBNEW_DISPLAY_ORIGIN_BASE::PANEL_PCBNEW_DISPLAY_ORIGIN_BASE( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bPanelSizer;
	bPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bMargins;
	bMargins = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bLeftSizer;
	bLeftSizer = new wxBoxSizer( wxVERTICAL );

	wxString m_DisplayOriginChoices[] = { _("Page Origin"), _("Drill/Place Origin"), _("Grid Origin") };
	int m_DisplayOriginNChoices = sizeof( m_DisplayOriginChoices ) / sizeof( wxString );
	m_DisplayOrigin = new wxRadioBox( this, wxID_ANY, _("Display Origin"), wxDefaultPosition, wxDefaultSize, m_DisplayOriginNChoices, m_DisplayOriginChoices, 1, wxRA_SPECIFY_COLS );
	m_DisplayOrigin->SetSelection( 0 );
	m_DisplayOrigin->SetToolTip( _("Select which origin is used for X,Y coordinate display.") );

	bLeftSizer->Add( m_DisplayOrigin, 0, wxALL|wxEXPAND, 5 );


	bMargins->Add( bLeftSizer, 1, wxEXPAND|wxRIGHT, 5 );

	wxBoxSizer* bRightSizer;
	bRightSizer = new wxBoxSizer( wxVERTICAL );

	wxString m_XAxisDirectionChoices[] = { _("Increases Right"), _("Increases Left") };
	int m_XAxisDirectionNChoices = sizeof( m_XAxisDirectionChoices ) / sizeof( wxString );
	m_XAxisDirection = new wxRadioBox( this, wxID_ANY, _("X Axis"), wxDefaultPosition, wxDefaultSize, m_XAxisDirectionNChoices, m_XAxisDirectionChoices, 1, wxRA_SPECIFY_COLS );
	m_XAxisDirection->SetSelection( 0 );
	m_XAxisDirection->SetToolTip( _("Select which the direction on the screen in which the X axis increases.") );

	bRightSizer->Add( m_XAxisDirection, 0, wxALL|wxEXPAND, 5 );

	wxString m_YAxisDirectionChoices[] = { _("Increases Up"), _("Increases Down") };
	int m_YAxisDirectionNChoices = sizeof( m_YAxisDirectionChoices ) / sizeof( wxString );
	m_YAxisDirection = new wxRadioBox( this, wxID_ANY, _("Y Axis"), wxDefaultPosition, wxDefaultSize, m_YAxisDirectionNChoices, m_YAxisDirectionChoices, 1, wxRA_SPECIFY_COLS );
	m_YAxisDirection->SetSelection( 0 );
	m_YAxisDirection->SetToolTip( _("Select which the direction on the screen in which the Y axis increases.") );

	bRightSizer->Add( m_YAxisDirection, 0, wxALL|wxEXPAND, 5 );


	bMargins->Add( bRightSizer, 1, wxEXPAND|wxRIGHT, 5 );


	bPanelSizer->Add( bMargins, 1, wxRIGHT, 5 );


	this->SetSizer( bPanelSizer );
	this->Layout();
}

PANEL_PCBNEW_DISPLAY_ORIGIN_BASE::~PANEL_PCBNEW_DISPLAY_ORIGIN_BASE()
{
}
