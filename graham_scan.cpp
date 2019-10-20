#include "graham_scan.hpp"


/* initialize the list of points */
GrahamScan::GrahamScan()
{
    this->pts = new std::vector<Point>;
    this->hull = new std::vector<Point>;
    this->seen = new std::unordered_set<Point, PointHash, PointComparator>;
}

/* 
 *  This constructor is primarily used for unit testing.
 *  
 *  Note: I did try using the Catch2 framework, but I continued
 *  to get bad_alloc errors. I am unsure why this was happenning.
 * 
 *  I then ran tests manually, and there were no errors.
 */
GrahamScan::GrahamScan(std::vector<Point> * pts)
{
    this->pts = pts;
    this->hull = new std::vector<Point>;
    this->seen = new std::unordered_set<Point, PointHash, PointComparator>;
}

/* 
 *  void findHull()
 *  
 * 
 *  Performs a Graham Scan on the set of points
 *  `pts'. This is done by first sorting the points
 *  based on y-coordinate. Ties are broken by considering
 *  the x coordinate. 
 * 
 *  Once the "lowest" point is found, we can
 *  sort the remaining points based on angle made with 
 *  the vertical line going through the lowest point.
 * 
 *  return
 *      None
 */
void
GrahamScan::findHull()
{
    /* ensure that there are points to consider */
    if (pts->empty()) return;

    int N = pts->size();
    if (N < MIN_POLYGON_VERTICES) return;

    /* find the lowest point in the hull */
    Point lowest = (*pts)[0];
    int idx = 0;
    for (int i = 1; i < N; ++i) {
        Point curr = (*pts)[i];
        if (curr.y < lowest.y) {
            lowest = curr;
            idx = i;
        } else if (
            fabs(curr.y-lowest.y) < std::numeric_limits<float>::epsilon()
        ) {
            if (curr.x < lowest.x) {
                lowest = curr;
                idx = i;
            }
        }
    }
    
    /* the lowest point is in the hull */
    hull->push_back(lowest);
    /* swap the lowest for the 0th entry */
    Point tmp = (*pts)[0];
    (*pts)[0] = (*pts)[idx];
    (*pts)[idx] = tmp;

    /* sort the remaining points according to polar angle with lowest*/
    std::sort(pts->begin() + 1, pts->end(), AngleComparator((*hull)[0]));
    
    /* we can be sure that the next point will be in the hull */
    Point nextLowest = (*pts)[1];
    hull->push_back(nextLowest);

    for (int i = 2; i < N; ++i) {
        /* as far as we know the i-th point is in the hull */
        Point curr = (*pts)[i];
        hull->push_back(curr);
        while (!leftTurn()) {
            /* if a right turn, the second to last element in hull */
            int j = hull->size() - 1;
            (*hull)[j-1] = (*hull)[j];
            /* get rid of duplicate */
            hull->pop_back();
        }
    } 

}

/*
 *  void addPoint(x, y)
 *  
 *  addPoint will place the point with 
 *  x-coordinate: x and y-coordinate: y
 *  into the collection of points.
 * 
 *  return
 *      None
 */
void
GrahamScan::addPoint(int x, int y)
{
    Point newPoint;
    newPoint.x = x;
    newPoint.y = y;
    if (seen->count(newPoint) == 0) {
        pts->push_back(newPoint);
        seen->insert(newPoint);
    }
}

/*
 *  bool leftTurn()
 *  
 *  This method will determine if the last 3 points in 
 *  the hull form a left turn. If so, true is returned. False 
 *  otherwise. 
 * 
 *  We can check if we have a left turn by considering
 *  the cross product of the last three points in the hull A, B, C
 *  if AB x AC is positive, then we have made a left turn. Otherwise,
 *  we have made a right turn.
 *  
 *  return
 *      boolean 
 */
bool
GrahamScan::leftTurn()
{
    int i = hull->size()-1;
    Point c = (*hull)[i];
    Point b = (*hull)[i-1];
    Point a = (*hull)[i-2];

    /* we only need the z-component of cross product */

    int ab_x = b.x - a.x;
    int ab_y = b.y - a.y;
    
    int ac_x = c.x - a.x;
    int ac_y = c.y - a.y;

    int z_comp = ab_x * ac_y - ab_y * ac_x;

    return z_comp > 0;
}

/*
 *  vector<Point> & getPoints()
 * 
 *  A getter for the points of the graham scan
 *  
 *  return
 *      reference to vector of points
 */
std::vector<Point> &
GrahamScan::getPoints()
{
    return *pts;
}

/*
 *  vector<Point> & getHull()
 * 
 *  A getter for the hull of the graham scan
 *  
 *  return
 *      reference to vector of points
 */
std::vector<Point> &
GrahamScan::getHull()
{
    return *hull;
}

/*
 *  void clear()
 *  
 *  Method to clear the points and the hull
 *  
 *  return 
 *      None
 */
void
GrahamScan::clear()
{
    pts->clear();
    hull->clear();
}
