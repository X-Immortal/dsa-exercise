//
// Created by  Apple on 2025/9/23.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace std;

struct Point {
    long long x;
    long long y;
};

double min_distance_simple(vector<Point> &points, int left, int right) {
    double min = -1;
    for (int i = left; i <= right; ++i) {
        for (int j = i + 1; j <= right; ++j) {
            long long dx = points[i].x - points[j].x;
            long long dy = points[i].y - points[j].y;
            double distance = sqrt(dx * dx + dy * dy);
            if (min < 0 || distance < min) {
                min = distance;
            }
        }
    }
    return min;
}

double min_distance(vector<Point> &points, int left, int right) {
    if (right - left <= 3) {
        return min_distance_simple(points, left, right);
    }
    int mid = left + (right - left) / 2;
    double min_left = min_distance(points, left, mid);
    double min_right = min_distance(points, mid + 1, right);
    double min_temp = min(min_left, min_right);
    long long mid_x = points[mid].x;
    vector<Point> mid_points;
    for (int i = mid; i >= left && points[i].x > mid_x - min_temp; --i) {
        mid_points.push_back(points[i]);
    }
    for (int i = mid + 1; i <= right && points[i].x < mid_x + min_temp; ++i) {
        mid_points.push_back(points[i]);
    }
    sort(mid_points.begin(), mid_points.end(), [](const Point &a, const Point &b) {return a.y < b.y;});
    double min_dis = min_temp;
    for (int i = 0; i < mid_points.size(); ++i) {
        for (int j = i + 1; j < mid_points.size() && mid_points[j].y - mid_points[i].y < min_dis; ++j) {
            long long dx = mid_points[i].x - mid_points[j].x;
            long long dy = mid_points[i].y - mid_points[j].y;
            min_dis = min(min_dis, sqrt(dx * dx + dy * dy));
        }
    }
    return min_dis;
}

int main() {
    int n;
    cin >> n;
    vector<Point> points;
    points.reserve(n);
    long long x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        points.emplace_back(Point{x, y});
    }
    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {return a.x < b.x;});
    printf("%.4f\n", min_distance(points, 0, n - 1));
    return 0;
}