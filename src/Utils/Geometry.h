#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "Global.h"
#include "Types.h"

namespace Geometry {
  // Considerando ponto e vetor como o mesmo tipo

  Int sign(const Float &t_difference);

  Bool cmp(const Float &t_lhs, const Int &t_op, const Float &t_rhs);

  template <class T>
  Float radiansToDegrees(const T &t_radians) {
    return t_radians * (180.0 / PI);
  }

  template <class T>
  Float degreesToRadians(const T &t_degrees) {
    return (t_degrees * PI) / 180.0;
  }

  template <class T>
  T dot(const cv::Point_<T> &t_p, const cv::Point_<T> &t_q) {
    return (t_p.x * t_q.x) + (t_p.y * t_q.y);
  }

  template <class T>
  T cross(const cv::Point_<T> &t_p, const cv::Point_<T> &t_q) {
    return (t_p.x * t_q.y) - (t_p.y * t_q.x);
  }

  template <class T>
  T distanceSquare(const T &t_x1, const T &t_y1, const T &t_x2, const T &t_y2) {
    return (t_x2 - t_x1) * (t_x2 - t_x1) + (t_y2 - t_y1) * (t_y2 - t_y1);
  }

  template <class T>
  T distanceSquare(const cv::Point_<T> &t_p, const cv::Point_<T> &t_q) {
    return dot(t_p - t_q, t_p - t_q);
  }

  template <class T>
  Float distance(const T &t_x1, const T &t_y1, const T &t_x2, const T &t_y2) {
    return std::sqrt(distanceSquare(t_x1, t_y1, t_x2, t_y2));
  }

  template <class T>
  Float distance(const cv::Point_<T> &t_p, const cv::Point_<T> &t_q) {
    return std::sqrt(distanceSquare(t_p, t_q));
  }

  template <class T>
  Float angle(const cv::Point_<T> &t_p, const cv::Point_<T> &t_q) {
    return std::atan2(cross(t_p, t_q), dot(t_p, t_q));
  }

  template <class T>
  cv::Point_<T> rotateCW90(const cv::Point_<T> &t_p) {
    return cv::Point_<T>(t_p.y, -t_p.x);
  }

  template <class T>
  cv::Point_<T> rotateCCW90(const cv::Point_<T> &t_p) {
    return cv::Point_<T>(-t_p.y, t_p.x);
  }

  // Em radianos
  template <class T>
  cv::Point_<Float> rotateCCW(const cv::Point_<T> &t_p, const Float &t_t) {
    return cv::Point_<Float>(t_p.x * std::cos(t_t) - t_p.y * std::sin(t_t),
                             t_p.x * std::sin(t_t) + t_p.y * std::cos(t_t));
  }

  // Redimensiona o vetor p, no tamanho t
  template <class T>
  cv::Point_<Float> resize(const cv::Point_<T> &t_p, const Float &t_t) {
    return t_p / std::sqrt(dot(t_p, t_p) * t_t);
  }

  // Check if Δ ABC is valid
  template <class T>
  Bool isTriangle(const T &t_a, const T &t_b, const T &t_c) {
    return cmp(t_a + t_b, GREATER, t_c) && cmp(t_a + t_c, GREATER, t_b) && cmp(t_b + t_c, GREATER, t_a);
  }

  // 2 * (Signed) Area Δ ABC
  template <class T>
  T area2(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c) {
    return (t_b.x - t_a.x) * (t_c.y - t_a.y) - (t_c.x - t_a.x) * (t_b.y - t_a.y);
  }

  // (Signed) Area Δ ABC
  Float area(const Point &t_a, const Point &t_b, const Point &t_c);

  // 2 * (Signed) Area of a polygon
  template <class T>
  T area2(const std::vector<cv::Point_<T>> &t_polygon) {
    assert(t_polygon.size() >= 3);
    T ret = 0;
    size_t n = t_polygon.size();
    for (size_t i = 1; i + 1 < n; i++) {
      ret += area2(t_polygon[0], t_polygon[i], t_polygon[i + 1]);
    }
    return ret;
  }

  // (Signed) Area of a polygon
  Float area(const Polygon &t_polygon);

  // Project point C onto line through A and B, assuming A != B
  Point projectPointLine(const Point &t_a, const Point &t_b, const Point &t_c);

  // Reflect point C onto line through A and B, assuming A != B
  Point reflectPointLine(const Point &t_a, const Point &t_b, const Point &t_c);

  // Project point C onto line segment through A and B
  Point projectPointSegment(const Point &t_a, const Point &t_b, const Point &t_c);

  // Compute distance from C to segment between A and B
  Float distancePointSegment(const Point &t_a, const Point &t_b, const Point &t_c);

  // Determines if C is between A and B
  template <class T>
  Bool between(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c) {
    if (t_a == t_b) {
      return (t_a == t_c);
    }
    return cmp(cross(t_a - t_c, t_b - t_c), EQUAL, 0) && not cmp(dot(t_a - t_c, t_b - t_c), GREATER, 0);
  }

  // Determines if A, B and C are collinear
  template <class T>
  Bool collinear(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c) {
    return cmp(area2(t_a, t_b, t_c), EQUAL, 0);
  }

  // Determine if lines from A to B and C to D are parallel
  template <class T>
  Bool linesParallel(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c, const cv::Point_<T> &t_d) {
    return cmp(cross(t_b - t_a, t_c - t_d), EQUAL, 0);
  }

  // Determine if lines from a to b and c to d are collinear;
  template <class T>
  Bool linesCollinear(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c, const cv::Point_<T> &t_d) {
    if (!linesParallel(t_a, t_b, t_c, t_d)) {
      return false;
    }
    if (!cmp(std::abs(cross(t_a - t_b, t_a - t_c)), EQUAL, 0) or !cmp(abs(cross(t_c - t_d, t_c - t_a)), EQUAL, 0)) {
      return false;
    }
    return true;
  }

  // Determine if line segment from A to B intersects with line segment from C to D
  template <class T>
  Bool segmentsIntersect(const cv::Point_<T> &t_a, const cv::Point_<T> &t_b, const cv::Point_<T> &t_c, const cv::Point_<T> &t_d) {
    if (between(t_a, t_b, t_c) || between(t_a, t_b, t_d) || between(t_c, t_d, t_a) || between(t_c, t_d, t_b)) {
      return true;
    }
    if (collinear(t_a, t_b, t_c) || collinear(t_a, t_b, t_d) || collinear(t_c, t_d, t_a) || collinear(t_c, t_d, t_b)) {
      return false;
    }
    if (cmp(cross(t_d - t_a, t_b - t_a) * cross(t_c - t_a, t_b - t_a), GREATER, 0)) {
      return false;
    }
    if (cmp(cross(t_a - t_c, t_d - t_c) * cross(t_b - t_c, t_d - t_c), GREATER, 0)) {
      return false;
    }
    return true;
  }

  /*
   * Compute intersection of line passing through A and B
   * with line passing through C and D, assuming that unique
   * intersection exists
   */
  Point computeLineIntersection(Point t_a, Point t_b, Point t_c, Point t_d);

  /*
   * Compute intersection of segment passing through A and B
   * with segment passing through C and D, assuming that unique
   * intersection exists
   */
  Point computeSegmentIntersection(const Point &t_a, const Point &t_b, const Point &t_c, const Point &t_d);

  // Compute circle passing through 3 given points
  Point computeCircleCenter(Point t_a, Point t_b, Point t_c);
  /*
   * Compute the arc degree of circle with radius r1
   * intersected with circle with radius r2 and with
   * distance between them equals to d
   */
  Float circleCircleIntersectionAngle(const Float &t_r1, const Float &t_r2, const Float &t_d);

  // Compute intersection of circle centered at a with radius r with circle centered at b with radius R
  Points circleCircleIntersection(const Point &t_p, const Float &t_r1, const Point &t_q, const Float t_r2);
}

#endif // GEOMETRY_H
