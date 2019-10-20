#include "wx/wx.h"
#include "wx/utils.h"
#include "wx/sizer.h"
#include "visualization.hpp"
#include "graham_scan.hpp"

/********************* APP ***********************/
IMPLEMENT_APP(App)

bool 
App::OnInit()
{
    wxBoxSizer * szr = new wxBoxSizer(wxVERTICAL);
    this->frame = new wxFrame((wxFrame *)NULL, -1, wxT(APP_NAME), wxPoint(50,50), wxSize(800,600));

    this->drawPane = new BasicDrawingPane((wxFrame *) this->frame);
    szr->Add(this->drawPane, 1, wxEXPAND);

    this->frame->SetSizer(szr);
    this->frame->SetAutoLayout(true);

    this->frame->Show();
    return true;
}
/**************************************************/

/************* BasicDrawingPane Methods ***********/

BasicDrawingPane::BasicDrawingPane(wxFrame * parent) : 
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER) 
{
    g = new GrahamScan;
}

/*
 *  void mouseDown(e)
 *  
 *  The mouseDown event will capture LEFT clicks of the mouse 
 *  when the mouse is over the pane. This point is added to the 
 *  set of points maintained by the graham scan.
 *  
 *  return 
 *      None
 */
void
BasicDrawingPane::mouseDown(wxMouseEvent & e)
{
    /* get the point that the mouse clicked */
    const wxPoint& pt = wxGetMousePosition();
    
    /* 
     *  get the x, y position of the mouse click relative to this 
     *  window.
     */
    int xPos = pt.x - this->GetScreenPosition().x;
    int yPos = pt.y - this->GetScreenPosition().y;

    g->addPoint(xPos, yPos);

    /* repaint the canvas */
    paintNow();
}

/*
 *  void keyPressed(e)
 *  
 *  The keyPressed event will capture key presses designated by the macros
 *  in visualization.hpp. Currectly if the user hits the `r' key, then the
 *  pane will be reset. If the user hits the `h' key then the hull will be 
 *  calculated for the set of points maintained by g. Otherwise, nothing
 *  occurs.
 *  
 *  return 
 *      None
 */
void
BasicDrawingPane::keyPressed(wxKeyEvent & e)
{
    const wxChar c = e.GetUnicodeKey();
    switch (c) {
        case RESET_KEY:
            /* pts and hull will be empty so clear re-render */
            {
                g->clear();
                wxClientDC dc(this);
                dc.Clear();
            }
            break;
        case RUN_KEY:
            g->findHull();
            /* hull points will not be empty so re-render */
            break;
        default:
            /* all other key presses are not handled */
            return;
    }
    /* repaint the canvas since state has changed */
    paintNow(); 
}

/*
 *  void paintEvent(e)
 *  
 *  This event is fired whenever, painting happens on the pane.
 *  There is no need to explicitly handle this event, other than rendering
 *  the current context. This is called whenever something needs to be
 *  rendered.
 * 
 *  return 
 *      None
 */
void
BasicDrawingPane::paintEvent(wxPaintEvent & e)
{
    wxPaintDC dc(this);
    render (dc);
}

/*
 *  void paintNow()
 *  
 *  This method is not an event; rather, it is called whenever the state
 *  needs to be updated. For example, if the user clicks a key we force a
 *  re-render of the screen.
 *  
 *  return 
 *      None
 */
void 
BasicDrawingPane::paintNow()
{
    wxClientDC dc(this);
    render (dc);
}

/*
 *  void render(dc)
 *  
 *  This method will render the points in the scan and the hull
 *  if there are any points in the hull set.
 * 
 *  return
 *      None
 */
void 
BasicDrawingPane::render(wxDC&  dc)
{
    std::vector<Point> & pts = g->getPoints();   
    int N = pts.size();

    /* grey filling */
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    /* blue outline for the pen */
    dc.SetPen( wxPen(BLUE, POINT_OUTLINE_THICKNESS) );
    /* draw the points if there are any */
    for (int i = 0; i < N; ++i) {
        Point p = pts[i];
        dc.DrawCircle( wxPoint(p.x, p.y), POINT_RADIUS );
    }

    /* there will only be hull points if user hit key for finding them */
    std::vector<Point>& hull = g->getHull();
    N = hull.size();

    /* red line for connecting hull points */
    dc.SetPen( wxPen(RED, HULL_OUTLINE_THICKNESS) );
    /* draw lines connecting the hull */
    for (int i = 0; i < N-1; ++i) {
        Point p1 = hull[i];
        Point p2 = hull[i+1];

        dc.DrawLine(p1.x, p1.y, p2.x, p2.y);
    }

    if (!hull.empty()) { /* connect the last two points */
        Point first = hull[0];
        Point last = hull[N-1];

        dc.DrawLine(first.x, first.y, last.x, last.y);
    }
}
/**************************************************/
