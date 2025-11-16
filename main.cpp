#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

struct Point {
    double x;
    double y;
};

struct Circle {
    double x;
    double y;
    double r;

    bool Contains(Point& p) const {
        double dx = (x - p.x);
        double dy = (y - p.y);
        return r * r >= dx * dx + dy * dy;
    }
};

struct Rect {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    std::string type;

    [[nodiscard]]
    double Area() const {
        return (xmax - xmin) * (ymax - ymin);
    }
};


std::mt19937_64 rng(12);

double estimateArea(const Rect& rect,
                    const std::vector<Circle>& circles,
                    size_t N) {
    std::uniform_real_distribution<double> distX(rect.xmin, rect.xmax), distY(rect.ymin, rect.ymax);



    size_t count = 0;

    for (size_t i = 0; i < N; ++i) {
        double x = distX(rng);
        double y = distY(rng);

        Point p{x, y};

        bool insideAll = true;
        for (const auto &c : circles) {
            if (!c.Contains(p)) {
                insideAll = false;
                break;
            }
        }

        if (insideAll) {
            ++count;
        }
    }

    return rect.Area() * static_cast<double>(count) / static_cast<double>(N);
}

int main() {
#ifdef HOME
    freopen("input.txt", "r", stdin);
#endif
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Circle> circles{
            Circle {1, 1, 1},
            Circle {1.5, 2, std::sqrt(5) / 2},
            Circle {2, 1.5, std::sqrt(5) / 2}
    };

    const double exactArea = 0.25 * std::numbers::pi + 1.25 * std::asin(0.8) - 1;


//    Rect bound{xmin, xmax, ymin, ymax};
    std::ofstream file("results.csv");
    file << "rect,N,exactArea,estArea,relError,absError\n";

    int startN = 100;

    int endN = 100000;
    int step = 500;

    std::vector<Rect> rects {
            {0, 3.5, 0, 3.5, "Wider"},
            {0.7, 2.05, 0.7, 2.2, "Accurate"}
    };
    for (const auto &rect : rects) {
        for (int curN = startN; curN <= endN; curN += step) {
            double res = estimateArea(
                    rect,
                    circles,
                    curN
            );

            double absError = std::abs(exactArea - res);
            double relError = absError / exactArea;
            file << rect.type << ','
                 << curN << ','
                 << exactArea << ','
                 << res << ','
                 << relError << ','
                 << absError << '\n';
        }
    }
    file.close();
}
