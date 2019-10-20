#include "graham_scan.hpp"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>

/* test 1 data files */
#define PATH_TO_PTS1_DAT "./tests/pts-test1.dat"
#define PATH_TO_PTS1_EXPECTED_DAT "./tests/pts-test1-expected.dat"

/* test 2 data files */
#define PATH_TO_PTS2_DAT "./tests/pts-test2.dat"
#define PATH_TO_PTS2_EXPECTED_DAT "./tests/pts-test2-expected.dat"

/* test 3 data files */
#define PATH_TO_PTS3_DAT "./tests/pts-test3.dat"
#define PATH_TO_PTS3_EXPECTED_DAT "./tests/pts-test3-expected.dat"

void 
readf(std::string fname, std::vector<Point> & pts)
{
    pts.clear();
    std::ifstream input;
    input.open(fname);
    if (!input) { 
        std::cout << "error openning file" << std::endl; 
        return; 
    }
    
    int x, y;
    while (input >> x) {
        input >> y;
        Point p;
        p.x = x;
        p.y = y;
        pts.push_back(p);    
    }
    input.close();
}

void
print_vec(std::vector<Point>& p)
{
    for (Point l : p) {
        std::cout << "(" << l.x << ", " << l.y << "), ";
    }
    std::cout << std::endl;
}

bool
compSort(const Point& p1, const Point& p2)
{
    if (p1.y == p2.y)
        return p1.x < p2.x;
    return p1.y < p2.y;
} 

void 
runScan(std::vector<Point> & pts, std::vector<Point> & hull)
{
    /* there should be nothing in the pts */
    GrahamScan s(&pts);
    /* find the hull and sort it */
    s.findHull();
    hull.clear();
    std::vector<Point> found = s.getHull();
    for (Point p : found) {
        hull.push_back(p);
    }
}

void 
verify(std::vector<Point> & expected, std::vector<Point> & hull)
{
    std::cout << "Recieved: ";
    print_vec(hull);
    std::cout << "------" << std::endl;

    std::cout << "Expected: ";
    print_vec(expected);
    std::cout << "------" << std::endl;

    std::sort(expected.begin(), expected.end(), compSort);
    std::sort(hull.begin(), hull.end(), compSort);

    std::cout << (expected == hull) << std::endl;
}

int 
main()
{
    std::vector<Point> pts;
    std::vector<Point> hull;
    std::vector<Point> expected;

    /* 'small' test set */
    readf(PATH_TO_PTS1_DAT, pts);
    readf(PATH_TO_PTS1_EXPECTED_DAT, expected);

    runScan(pts, hull);
    
    verify(expected, hull);

    /* 'medium' test set */
    readf(PATH_TO_PTS2_DAT, pts);
    readf(PATH_TO_PTS2_EXPECTED_DAT, expected);

    runScan(pts, hull);
    
    verify(expected, hull);
    
    /* 'large' test set */
    readf(PATH_TO_PTS3_DAT, pts);
    readf(PATH_TO_PTS3_EXPECTED_DAT, expected);

    runScan(pts, hull);

    verify(expected, hull);

}
