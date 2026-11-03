double euclid(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

using Real = double;
const Real EPS = 1e-10;
const Real PI = acos(-1);

// 誤差を考慮した符号判定 (-1: 負, 0: ゼロ, 1: 正)
int sgn(Real a) { return (a < -EPS) ? -1 : (a > EPS) ? 1 : 0; }
int sgn(Real a, Real b) { return sgn(a - b); }

// 2次元の点 / ベクトル構造体
struct Point {
    Real x, y;
    Point(Real x = 0, Real y = 0) : x(x), y(y) {}

    Point operator-() const { return Point(-x, -y); }
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(Real k) const { return Point(x * k, y * k); }
    Point operator/(Real k) const { return Point(x / k, y / k); }

    bool operator==(const Point& p) const { return sgn(x, p.x) == 0 && sgn(y, p.y) == 0; }
    bool operator<(const Point& p) const { return sgn(x, p.x) != 0 ? sgn(x, p.x) < 0 : sgn(y, p.y) < 0; }
};
using Vector = Point;

// 内積 (dot product): |a||b|cosθ
Real dot(const Vector& a, const Vector& b) { return a.x * b.x + a.y * b.y; }

// 外積 (cross product): |a||b|sinθ (平行なら0)
Real cross(const Vector& a, const Vector& b) { return a.x * b.y - a.y * b.x; }

// ベクトルの長さの2乗
Real norm(const Vector& a) { return a.x * a.x + a.y * a.y; }
// ベクトルの長さ
Real abs(const Vector& a) { return sqrt(norm(a)); }

// 点 a, b, c の位置関係 (ccw)
// 返り値:
//  1: a->b に対して c が反時計回り (Counter Clockwise)
// -1: a->b に対して c が時計回り (Clockwise)
//  2: c-a-b の順で一直線上 (Online Back)
// -2: a-b-c の順で一直線上 (Online Front)
//  0: c が線分 ab 上に存在 (On Segment)
int ccw(const Point& a, Point b, Point c) {
    b = b - a; c = c - a;
    if (cross(b, c) > EPS) return 1;
    if (cross(b, c) < -EPS) return -1;
    if (dot(b, c) < -EPS) return 2;
    if (norm(b) < norm(c)) return -2;
    return 0;
}

// 直線 / 線分構造体 (点 a, b を通る)
struct Line {
    Point a, b;
    Line() = default;
    Line(Point a, Point b) : a(a), b(b) {}
};
using Segment = Line;

// 2直線の平行判定 (外積が0なら平行)
bool is_parallel(const Line& a, const Line& b) {
    return sgn(cross(a.b - a.a, b.b - b.a)) == 0;
}

// 2直線の垂直判定 (内積が0なら垂直)
bool is_orthogonal(const Line& a, const Line& b) {
    return sgn(dot(a.b - a.a, b.b - b.a)) == 0;
}

// 射影: 点 p から直線 l に下ろした垂線の足の座標を返す
Point projection(const Line& l, const Point& p) {
    Vector base = l.b - l.a;
    Real t = dot(p - l.a, base) / norm(base);
    return l.a + base * t;
}

// 2つの線分 s1, s2 が交差しているか判定
bool intersect(const Segment& s1, const Segment& s2) {
    return ccw(s1.a, s1.b, s2.a) * ccw(s1.a, s1.b, s2.b) <= 0 &&
           ccw(s2.a, s2.b, s1.a) * ccw(s2.a, s2.b, s1.b) <= 0;
}

// 2直線の交点座標を計算して返す
Point crosspoint(const Line& l1, const Line& l2) {
    Real d1 = cross(l1.b - l1.a, l2.b - l2.a);
    Real d2 = cross(l1.b - l1.a, l1.b - l2.a);
    if (sgn(d1) == 0 && sgn(d2) == 0) return l2.a; // 一致する場合
    return l2.a + (l2.b - l2.a) * (d2 / d1);
}

/* --- 使用例 ---
Point p1(0, 0), p2(4, 0);
Line l1(p1, p2);
Point p3(2, 2), p4(2, -2);
Line l2(p3, p4);

// 平行・垂直判定
bool para = is_parallel(l1, l2);   // false
bool ortho = is_orthogonal(l1, l2); // true

// 交点取得
Point cp = crosspoint(l1, l2); // (2, 0)

// 点の回転・垂線
Point foot = projection(l1, Point(2, 3)); // (2, 0)
*/
