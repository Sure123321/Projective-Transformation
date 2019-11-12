#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Point {
public:
    int x;
    int y;
    Point() {
        this->x = 0;
        this->y = 0;
    }
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

int side(Point start, Point end, Point point) {
    return (end.x - start.x)*(point.y - start.y) - (end.y - start.y)*(point.x - start.x);
}

vector<Point> order_points (vector<Point> points){

    Point corner = points[0];
    Point p1 = points[1];
    Point p2 = points[2];
    Point p3 = points[3];

    Point opposite, leftside, rightside;
    if (side(corner, p1, p2) * side(corner, p1, p3) < 0) {
        opposite = p1;
        if (side(corner, p1, p2) < 0) {
            leftside = p2;
            rightside = p3;
        }
        else {
            leftside = p3;
            rightside = p2;
        }
    }
    else if (side(corner, p2, p1) * side(corner, p2, p3) < 0) {
        opposite = p2;
        if (side(corner, p2, p1) < 0) {
            leftside = p1;
            rightside = p3;
        }
        else {
            leftside = p3;
            rightside = p1;
        }
    }
    else if (side(corner, p3, p1) * side(corner, p3, p2) < 0) {
        opposite = p3;
        if (side(corner, p3, p1) < 0) {
            leftside = p1;
            rightside = p2;
        }
        else {
            leftside = p2;
            rightside = p1;
        }
    }
    else {
        throw("Computation error: right points order not found");
    }

    vector<Point> answer;
    answer.push_back(corner);
    answer.push_back(leftside);
    answer.push_back(opposite);
    answer.push_back(rightside);
    return answer;
}

int main()
{
    const int n = 4;
    ifstream in;
    in.open("input.txt");
    vector <Point> source;
    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;
        source.push_back(Point(x, y));
    }
    //source = order_points(source);
    // Suppose that the first point is the upper-right corner.
    // We need to order the remaining three so the go in the clockwise order.
    // Otherwise we can flip or distort the image.
    // There is still ambiguity, but nevermind that.

    vector <Point> destination;
    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;
        destination.push_back(Point(x, y));
    }

    int matrix[2*n][2*n+1];
    for (int i = 0; i < 2*n; i++) {
        for (int j = 0; j < 2*n; j++) {
            matrix[i][j] = 0;
        }
    }
    for (unsigned i = 0; i < n; i++) {
        matrix[2*i][0] = source[i].x;
        matrix[2*i][1] = source[i].y;
        matrix[2*i][2] = 1;

        matrix[2*i+1][3] = source[i].x;
        matrix[2*i+1][4] = source[i].y;
        matrix[2*i+1][5] = 1;

        matrix[2*i]  [6] = - destination[i].x * source[i].x;
        matrix[2*i]  [7] = - destination[i].x * source[i].y;
        matrix[2*i+1][6] = - destination[i].y * source[i].x;
        matrix[2*i+1][7] = - destination[i].y * source[i].y;

        ////
        matrix[2*i]  [8] = destination[i].x;
        matrix[2*i+1][8] = destination[i].y;

    }

    //solve the matrix
    for

    return 0;
}
