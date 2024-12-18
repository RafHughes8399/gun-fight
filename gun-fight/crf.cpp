//
//#include "frame.h"
//#include <wx/wx.h>
//
///** enum ids {
//	BUTTON_ID = 2
//};
// *
// * STATIC EVENT HANDLING.
// * begin the event table with the handler class and its base class
// * identify an event for the button, passing the id and what should happen
//	end the event table
//wxBEGIN_EVENT_TABLE(frame, wxFrame)
//	EVT_BUTTON(BUTTON_ID, frame::on_button_press)
//wxEND_EVENT_TABLE()
//*/
//frame::frame(const wxString& title)
//	: wxFrame(nullptr, wxID_ANY, title) { // wxFrame takes the parent frame, an int id and title, position and size can be specified too
//
//	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
//	wxButton* button = new wxButton(panel, wxID_ANY, "button", wxPoint(150, 50), wxSize(100, 35));
//
//	// bind the button to any event, specify the event type, then a pointer to the event method
//	// then a pointer to the object that handles the event
//	button->Bind(wxEVT_BUTTON, &frame::on_button_press, this); // add the binding
//
//	panel->Bind(wxEVT_LEFT_DOWN, &frame::on_mouse_event, this);
//	panel->Bind(wxEVT_CHAR_HOOK, &frame::on_key_event, this);
//	CreateStatusBar();
//	/**
//	 * bind can also be called using frame or panel due to event propogation and button being a child
//	 * of panel and frame. The event handling propogates up the class heirarchy. If there are multiple buttons
//	 * then they would both have the same event behaviour when using frame or panel
//	 *
//	 */
//
//	 //wxStaticText* static_text = new wxStaticText(panel, wxID_ANY, "static text, unchangeable", wxPoint(120, 150)); // default size makes entire text visible
//	 //wxColor xanthous = wxColor(252, 191, 21);
//	 //static_text->SetBackgroundColour(xanthous);
//
//};
//
//// describes what will happen when the button is pressed, but needs to be bound to the button event handler first
//
//void frame::on_button_press(wxCommandEvent& event) {
//	wxLogStatus("pressed button");
//}
//
//void frame::on_mouse_event(wxMouseEvent& event) {
//	auto pos = event.GetPosition();
//	auto str = wxString::Format(
//		"mouse event at x: %d, y: %d", pos.x, pos.y
//	);
//	wxLogStatus(str);
//}
//
//void frame::on_key_event(wxKeyEvent& event) {
//	if (event.GetUnicodeKey() == 'A') {
//		wxLogStatus("pressed A");
//
//	}
//	wxChar key = event.GetUnicodeKey();
//	if (key == WXK_NONE) {
//		auto key_code = event.GetKeyCode();
//		wxLogStatus("key event %d", key_code);
//	}
//	else {
//		wxLogStatus("key pressed %c", key);
//	}
//}
//// need the ability to display it, usually this happens in main. In a GUI application, it happens in a winMain function. In wxWidgets, 
//// this happens in a separate class app
//
//
