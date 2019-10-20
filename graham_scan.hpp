#ifndef _GRAHAM_SCAN_
#define _GRAHAM_SCAN_

#include <vector>
#include <cmath>
#include <unordered_set>
#include <algorithm>

#define MIN_POLYGON_VERTICES 3
#define PI 3.14159265

/*********************** POINT ******************/

/*
 *  struct Point
 *  
 *  members: 
 *    x - the integer x-coordinate
 *    y - the integer y-coordinate
 *  
 *  This struct will be used as a representation of a 2D point
 *  
 */
struct Point
{
    int x;
    int y;

    inline bool operator==(const Point& other) const {
      return this->x == other.x && this->y == other.y;
    }
};

/* 
 *  struct PointHash
 *  
 *  This struct will be used to hash a 2D point. The scan should not contain 
 *  duplicate points so we will need this for the unordered_set.
 */  
struct PointHash
{
  size_t
  operator()(const Point& p) const
  {
    return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
  }
};
 
/*
 *  struct PointComparator
 * 
 *  This struct will be used to compare points. I was not sure how to
 *  get the the unorderd_set to use to operator== in the Point struct.
 */
struct PointComparator
{
  bool
  operator()(const Point& p1, const Point& p2) const
  {
    return p1.x == p2.x && p1.y == p2.y;
  }
};

/*
 *  struct AngleComparator(P0)
 *  
 *  This struct will be used as a
 *  custom sort for the points based on the polar angle
 *  made with the lowest point P0 in pts.
 *  
 *  return
 *      p1 is at a greater angle than p2
 */
struct AngleComparator 
{
    Point P0;
    AngleComparator(const Point& lowest) { P0 = lowest;}
    inline bool operator () (const Point& p1, const Point& p2) {
        int x0 = P0.x, y0 = P0.y;
        int x1 = p1.x, y1 = p1.y;
        int x2 = p2.x, y2 = p2.y;

        float theta1 = atan2((float)y1-y0, (float)x1-x0) * 180 / PI;
        float theta2 = atan2((float)y2-y0, (float)x2-x0) * 180 / PI;

        return theta1 < theta2;
    }
};

/*
 *  class GrahamScan
 *  
 *  members:
 *    pts - a set of points added to by the user
 *    hull - the hull (a list of points) calculated from pts
 *    seen - a set to keep track of unique points
 *  methods:
 *     See graham_scan.cpp for implementation details and comments on methods.
 * 
 *  
 *  The GrahamScan class will hold the points put down by the user
 *  in the GUI and determine the convex hull of that set of points.
 */
class GrahamScan
{
private:
    /* stores the points that are put down by user*/
    std::vector<Point> * pts;
    /* will store the points for the hull */
    std::vector<Point> * hull;

    /* keeps track of the points that have been seen */
    std::unordered_set<Point, PointHash, PointComparator> * seen;

    /* will determine if the last three points in the hull make a left turn */
    bool leftTurn();

public:
    GrahamScan();
    GrahamScan(std::vector<Point> * pts);

    void findHull();
    void addPoint(int x, int y);
    std::vector<Point> & getPoints();
    std::vector<Point> & getHull();

    void clear();
};

#endif /* _GRAHAM_SCAN_ */
