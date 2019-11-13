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

void solve_matrix(int rank, double A[8][9])
{
    const int nrows = rank;
    const int ncols = rank+1;

    int lead = 0;

    while (lead < nrows) {
        double d, m;

        for (int r = 0; r < nrows; r++) {
            d = A[lead][lead];
            m = A[r][lead] / A[lead][lead];

            for (int c = 0; c < ncols; c++) {
                if (r == lead) {
                    A[r][c] /= d;
                } else {
                    A[r][c] -= A[lead][c] * m;
                }
            }
        }

        lead++;
    }
}

Point multiply(double matrix[3][3], int x, int y) {
    double x1 = matrix[0][0]*x + matrix[1][0]*y + matrix[2][0];
    double y1 = matrix[0][1]*x + matrix[1][1]*y + matrix[2][1];
    double z1 = matrix[0][2]*x + matrix[1][2]*y + matrix[2][2];
    return Point(int(x1/z1), int(y1/z1));
}

bool in_bounds(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int main()
{
    const int n = 4;
    const int max_width = 1000;
    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");

    int w, h;
    in >> w >> h;
    char image[max_width][max_width];

    vector <Point> source;
    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;
        source.push_back(Point(x, y));
    }

    vector <Point> destination;
    int a, b;
    in >> a >> b; //rectangle sides
    destination.push_back(Point(0, 0));
    destination.push_back(Point(0, b));
    destination.push_back(Point(a, 0));
    destination.push_back(Point(a, b));

    for (int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            in.get(image[y][x]);
        }
        in.get();
    }


    double matrix[2*n][2*n+1];
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

    solve_matrix(2*n+1, matrix);

    double transform_matrix[3][3];
    for (int i = 0; i < 8; i++) {
        transform_matrix[i/3][i%3] = matrix[i][8];
    }
    transform_matrix[2][2] = 1;

    char rectangle[max_width][max_width];

    for (int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            Point dest = multiply(transform_matrix, x, y);
            if (in_bounds(dest.x, dest.y, a, b)) {
                rectangle[dest.x][dest.y] = image[x][y];
            }
        }
    }

    for (int y = 0; y < b; y++) {
        for (int x = 0; x < a; x++) {
            out << rectangle[y][x];
        }
        out << endl;
    }

    in.close();
    out.close();

    return 0;
}
