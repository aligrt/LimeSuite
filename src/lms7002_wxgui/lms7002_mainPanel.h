#ifndef __lms7002_mainPanel__
#define __lms7002_mainPanel__

/**
@file
Subclass of mainPanel, which is generated by wxFormBuilder.
*/
#include "lime/LimeSuite.h"
#include "lms7002_wxgui.h"

//// end generated include
namespace lime{
class MCU_BD;
}
class lms7002_pnlMCU_BD_view;
class lms7002_pnlR3_view;

/** Implementing mainPanel */
class lms7002_mainPanel : public mainPanel
{
	protected:
        // Handlers for mainPanel events.
        void OnNewProject( wxCommandEvent& event );
        void OnOpenProject( wxCommandEvent& event );
        void OnSaveProject( wxCommandEvent& event );
        void OnUploadAll( wxCommandEvent& event );
        void OnDownloadAll( wxCommandEvent& event );
        void OnReset( wxCommandEvent& event );
        void OnRegistersTest( wxCommandEvent& event );
        void OnSwitchToChannelA(wxCommandEvent& event);
        void OnSwitchToChannelB(wxCommandEvent& event);
        void Onnotebook_modulesPageChanged( wxNotebookEvent& event );
        void OnResetChip(wxCommandEvent& event);
        void OnLoadDefault(wxCommandEvent& event);
        void OnReadTemperature(wxCommandEvent& event);
        void OnSyncABchecked(wxCommandEvent& event);
        void OnEnableMIMOchecked(wxCommandEvent& event);
        void OnCalibrateInternalADC(wxCommandEvent& event);
        int GetLmsSelection();
        void OnLmsDeviceSelect( wxCommandEvent& event );
	public:
		/** Constructor */
		lms7002_mainPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	//// end generated class members
        void UpdateVisiblePanel();
        ~lms7002_mainPanel();
        void UpdateGUI();
        void Initialize(lms_device_t* pControl);
protected:
        lms7002_pnlMCU_BD_view *mTabMCU;
        lms7002_pnlR3_view *mTabR3;
        lms_device_t* lmsControl;
};

#endif // __lms7002_mainPanel__
