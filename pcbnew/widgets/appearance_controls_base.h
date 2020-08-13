///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/scrolwin.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/collpane.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gbsizer.h>
#include <wx/textctrl.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/choice.h>
#include <wx/statbox.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class APPEARANCE_CONTROLS_BASE
///////////////////////////////////////////////////////////////////////////////
class APPEARANCE_CONTROLS_BASE : public wxPanel
{
	private:

	protected:
		wxBoxSizer* m_sizerOuter;
		wxNotebook* m_notebook;
		wxPanel* m_panelLayers;
		wxBoxSizer* m_panelLayersSizer;
		wxScrolledWindow* m_windowLayers;
		wxBoxSizer* m_layers_outer_sizer;
		wxCollapsiblePane* m_paneLayerDisplay;
		wxStaticText* m_staticText13;
		wxRadioButton* m_rbHighContrastNormal;
		wxRadioButton* m_rbHighContrastDim;
		wxRadioButton* m_rbHighContrastOff;
		wxStaticLine* m_staticline5;
		wxCheckBox* m_cbFlipBoard;
		wxPanel* m_panelObjects;
		wxBoxSizer* m_objectsPanelSizer;
		wxScrolledWindow* m_windowObjects;
		wxGridBagSizer* m_objectsSizer;
		wxPanel* m_panelNetsAndClasses;
		wxSplitterWindow* m_netsTabSplitter;
		wxPanel* m_panelNets;
		wxStaticText* m_staticText141;
		wxTextCtrl* m_txtNetFilter;
		wxBitmapButton* m_btnNetInspector;
		wxScrolledWindow* m_netsScrolledWindow;
		wxBoxSizer* m_netsOuterSizer;
		wxPanel* m_panelNetclasses;
		wxStaticText* m_staticText14;
		wxBitmapButton* m_btnConfigureNetClasses;
		wxScrolledWindow* m_netclassScrolledWindow;
		wxBoxSizer* m_netclassOuterSizer;
		wxCollapsiblePane* m_paneNetDisplay;
		wxStaticText* m_staticText131;
		wxRadioButton* m_rbNetColorOff;
		wxRadioButton* m_rbNetColorRatsnest;
		wxRadioButton* m_rbNetColorAll;
		wxChoice* m_cbLayerPresets;
		wxBitmapButton* m_btnDeletePreset;

		// Virtual event handlers, overide them in your derived class
		virtual void OnNotebookPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnLayerDisplayPaneChanged( wxCollapsiblePaneEvent& event ) { event.Skip(); }
		virtual void OnFlipBoardChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNetDisplayPaneChanged( wxCollapsiblePaneEvent& event ) { event.Skip(); }
		virtual void OnLayerPresetChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnDeleteLayerPreset( wxCommandEvent& event ) { event.Skip(); }


	public:

		APPEARANCE_CONTROLS_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,762 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~APPEARANCE_CONTROLS_BASE();

		void m_netsTabSplitterOnIdle( wxIdleEvent& )
		{
			m_netsTabSplitter->SetSashPosition( 300 );
			m_netsTabSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( APPEARANCE_CONTROLS_BASE::m_netsTabSplitterOnIdle ), NULL, this );
		}

};
