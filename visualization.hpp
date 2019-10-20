#include "wx/wx.h"
#include "wx/utils.h"
#include "wx/sizer.h"
#include "graham_scan.hpp"


#ifndef _VISUALIZATION_HPP_
/***************** MACROS ******************/
#define DEBUG 0
#define APP_NAME "2D Convex Hull via Graham Scan"

#define POINT_OUTLINE_THICKNESS 2
#define POINT_RADIUS 5
#define BLUE wxColor(0, 0, 255)

#define HULL_OUTLINE_THICKNESS 1
#define RED wxColor(255, 0, 0)

/* `h' key to run the scan on the set of points */
#define RUN_KEY 72 

/* `r' key to reset to screen */
#define RESET_KEY 82
/********************************************/


/*************** CLASSES & ENUM *************/

/*
 *  class BasicDrawingPane
 *   
 *  members:
 *      g - the GrahamScan that can be performed on the set of points
 *  
 *  methods:
 *      See app.cpp for implementation details and comments
 * 
 *  This class represents the window that will be drawn on 
 *  for this visualization. 
 */
class BasicDrawingPane: public wxPanel
{
private:
    GrahamScan * g;

public: 
    BasicDrawingPane(wxFrame * parent);

    void paintEvent(wxPaintEvent & event);
    void paintNow();

    void render(wxDC & dc);

    void mouseDown(wxMouseEvent & e);
    void keyPressed(wxKeyEvent & e);

    /* other events that can be handled (also implement in app.cpp)*/
    /*
    void mouseMoved(wxMouseEvent & e);
    void mouseWheelMoved(wxMouseEvent & e);
    void mouseReleased(wxMouseEvent & e);
    void rightClick(wxMouseEvent & e);
    void mouseLeftWindow(wxMouseEvent & e);
    void keyReleased(wxKeyEvent & e);
    */

    DECLARE_EVENT_TABLE()
};

/*
 *  class App
 *  
 *  members:
 *      frame - the frame that the app will be showing
 *      drawPane - the pane that will hold our drawings
 * 
 *  methods:
 *      See app.cpp for implementation details and comments
 */
class App : public wxApp
{
    virtual bool OnInit();

    wxFrame * frame;

    BasicDrawingPane * drawPane;
};
/*********************************************/

/***************** EVENT TABLE ********************/
/* event table for panel */
BEGIN_EVENT_TABLE(BasicDrawingPane, wxPanel)
    /* other events to handle */
    /*
    EVT_MOTION(BasicDrawingPane::mouseMoved)
    EVT_LEFT_UP(BasicDrawingPane::mouseReleased)
    EVT_RIGHT_DOWN(BasicDrawingPane::rightClick)
    EVT_LEAVE_WINDOW(BasicDrawingPane::mouseLeftWindow)
    EVT_KEY_UP(BasicDrawingPane::keyReleased)
    EVT_MOUSEWHEEL(BasicDrawingPane::mouseWheelMoved)
    */

    /* paint event */
    EVT_PAINT(BasicDrawingPane::paintEvent)
    /* key down event */
    EVT_KEY_DOWN(BasicDrawingPane::keyPressed)
    /* for pressing the mouse down */
    EVT_LEFT_DOWN(BasicDrawingPane::mouseDown)
END_EVENT_TABLE()
/**************************************************/
#endif /* _VISUALIZATION_HPP_ */
