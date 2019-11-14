#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <cstring>
#include <unistd.h>

using namespace std;


const double eps = 1e-6;

class Point {
public:
    int x = 0;
    int y = 0;
    Point() {}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

static int side(Point start, Point end, Point point) {
    return (end.x - start.x)*(point.y - start.y) - (end.y - start.y)*(point.x - start.x);
}

static vector<Point> order_points (vector<Point> points){

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

static void solve_matrix(double A[8][9]) {
    //Jordan-Gauss

    const int nrows = 8;
    const int ncols = 9;

    cout << "start (must be the same as an input):" << endl;
    for (int i = 0; i < nrows; i++) {
	cout << i << ": ";
        for (int j = 0; j < ncols; j++) {
            cout << setw(4) << A[i][j]  << " ";
        }
        cout << endl;
    }
    cout << "go" << endl;
    cout<< endl;

    for (int lead = 0; lead < nrows; lead++) {
        double d, m;

        int r;
        for (r = lead; r < nrows && fabs(A[r][lead]) < eps; r++);
	cout << "Iteration: lead=" << lead << " r=" << r << endl;
        if (r == nrows) continue;
        // The column is cleared out. There shouldn't be any non-zeroes above
        // in this case since the matrix is not singular.

        if (r != lead) { // Make the right row a leading one
            for (int i = 0; i < ncols; i++) {
                swap(A[lead][i], A[r][i]);
            }
        }

        d = A[lead][lead];
        for (int r = 0; r < nrows; r++) {

            if (fabs(A[r][lead]) < eps) continue;

            m = A[r][lead] / d;

            for (int c = 0; c < ncols; c++) {
                if (r == lead) {
                    A[r][c] /= d;
                } else {
                    A[r][c] -= A[lead][c] * m;
                }
            }
        }
        for (int i = 0; i < nrows; i++) {
	    cout << i << ": ";
            for (int j = 0; j < ncols; j++) {
                cout << setw(4) << A[i][j]  << " ";
            }
            cout << endl;
        }
        cout<< endl;
    }

    for (int i = 0; i < nrows; i++) {
	cout << i << ": ";
        for (int j = 0; j < ncols; j++) {
            cout << setw(4) << A[i][j]  << " ";
        }
        cout << endl;
    }
}

static Point multiply(double matrix[3][3], int x, int y) {
    double x1 = matrix[0][0]*x + matrix[1][0]*y + matrix[2][0];
    double y1 = matrix[0][1]*x + matrix[1][1]*y + matrix[2][1];
    double z1 = matrix[0][2]*x + matrix[1][2]*y + matrix[2][2];
    return Point(int(round(x1/z1)), int(round(y1/z1)));
}

static bool in_bounds(int c, int r, int width, int height) {
    return c >= 0 && c < width && r >= 0 && r < height;
}

static void prepare_transform_equations(
        double transform_equations[8][9],
        const vector<Point>& source,
        const vector<Point>& destination)
{
    for (unsigned i = 0; i < 8; i++) {
        for (unsigned j = 0; j < 9; j++) {
            transform_equations[i][j] = 0;
        }
    }
    for (unsigned i = 0; i < 4; i++) {
        transform_equations[2*i][0] = source[i].x;
        transform_equations[2*i][1] = source[i].y;
        transform_equations[2*i][2] = 1;

        transform_equations[2*i+1][3] = source[i].x;
        transform_equations[2*i+1][4] = source[i].y;
        transform_equations[2*i+1][5] = 1;

        transform_equations[2*i]  [6] = - destination[i].x * source[i].x;
        transform_equations[2*i]  [7] = - destination[i].x * source[i].y;
        transform_equations[2*i+1][6] = - destination[i].y * source[i].x;
        transform_equations[2*i+1][7] = - destination[i].y * source[i].y;

        ////
        transform_equations[2*i]  [8] = destination[i].x;
        transform_equations[2*i+1][8] = destination[i].y;

    }
}

int do_test()
{
    printf("Testing making transform matrix...");
    vector<Point> source, destination;
    source.emplace_back(0, 0);
    source.emplace_back(0, 1);
    source.emplace_back(1, 1);
    source.emplace_back(1, 0);
    destination.emplace_back(0, 0);
    destination.emplace_back(0, 2);
    destination.emplace_back(2, 2);
    destination.emplace_back(2, 0);
    //source = order_points(source);
    double transform_equations[8][9];
    prepare_transform_equations(transform_equations, source, destination);
    solve_matrix(transform_equations);
    double transform_matrix[3][3];
    for (int i = 0; i < 8; i++) {
        transform_matrix[i/3][i%3] = transform_equations[i][8];
    }
    transform_matrix[2][2] = 1;
    cout << "Transform matrix:" << endl;
    for (unsigned row = 0; row <= 2; ++row) {
      for (unsigned col = 0; col <= 2; ++col) {
        cout << transform_matrix[row][col] << ' ';
      }
      cout << endl;
    }
    Point dest;
    dest = multiply(transform_matrix, 0, 0);
    printf("%d %d\n", dest.x, dest.y);
    return 0;
}

int main(int argc, char **argv)
{
    {
        char wd[1024];
        getcwd(wd, sizeof wd);
        cout << "Current directory: " << wd << endl;
    }
    if (argc >= 2 && !strcmp(argv[1], "test")) {
       return do_test();
    }
    const int max_width = 1000;
    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");

    int in_width, in_height;
    in >> in_width >> in_height;
    char in_image[max_width][max_width];

    vector <Point> source;
    for (int i = 0; i < 4; i++) {
        int x, y;
        in >> x >> y;
        source.push_back(Point(x, y));
    }
    source = order_points(source);

    vector <Point> destination;
    int out_width, out_height;
    in >> out_width >> out_height; //rectangle sides
    destination.push_back(Point(0, 0));
    destination.push_back(Point(out_width, 0));
    destination.push_back(Point(out_width, out_height));
    destination.push_back(Point(0, out_height));

    for (int y = 0; y < in_height; y++) {
        for(int x = 0; x < in_width; x++) {
            in.get(in_image[y][x]);
        }
        in.get();
    }


    double transform_equations[8][9];
    prepare_transform_equations(transform_equations, source, destination);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 9; j++) {
            cout << transform_equations[i][j]  << " ";
        }
        cout << endl;
    }
    cout<< endl;


    solve_matrix(transform_equations);

    double transform_matrix[3][3];
    for (int i = 0; i < 8; i++) {
        transform_matrix[i/3][i%3] = transform_equations[i][8];
    }
    transform_matrix[2][2] = 1;

    char out_image[max_width][max_width];
    for (int r = 0; r < out_height; r++) {
        for (int c = 0; c < out_width; c++) {
            out_image[r][c] = ' ';
        }
    }

    for (int r = 0; r < in_height; r++) {
        for(int c = 0; c < in_width; c++) {
            Point dest = multiply(transform_matrix, c, r);
            if (in_bounds(dest.x, dest.y, out_width, out_height)) {
                out_image[dest.y][dest.x] = in_image[r][c];
            }
        }
    }

    for (int r = 0; r < out_height; r++) {
        cout << ":";
        for (int c = 0; c < out_width; c++) {
            cout << out_image[r][c];
            out << out_image[r][c];
        }
        //out << endl;
        cout << ':' << endl;
    }

    in.close();
    out.close();

    return 0;
}
