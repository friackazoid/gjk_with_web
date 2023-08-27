#include "gjk.hpp"

#include <eigen3/Eigen/Core>

struct Simplex {
    std::vector<Eigen::Vector2d> points;
};

bool containsOrigin(Simplex& simplex, Eigen::Vector2d& direction) {
    if (simplex.points.size() == 3) {
        // For triangles, use barycentric coordinates to check if the origin is inside the triangle
        Eigen::Vector2d a = simplex.points[2];
        Eigen::Vector2d b = simplex.points[1];
        Eigen::Vector2d c = simplex.points[0];

        Eigen::Vector2d AB = b - a;
        Eigen::Vector2d AC = c - a;

        Eigen::Vector2d AO = -a; // Negative of point A, where A is the last added point

        double dotAB = AB.dot(AO);
        double dotAC = AC.dot(AO);

        if (dotAB <= 0.0 && dotAC <= 0.0) {
            return true; // Origin lies inside the triangle
        }

        Eigen::Vector2d ABPerp(-AB[1], AB[0]);
        Eigen::Vector2d ACPerp(-AC[1], AC[0]);

        if (dotAB > 0.0 && ABPerp.dot(AO) <= 0.0) {
            simplex.points.erase(simplex.points.begin()); // Remove C
            direction = ABPerp; // Set direction towards AB
            return false;
        }

        if (dotAC > 0.0 && ACPerp.dot(AO) <= 0.0) {
            simplex.points.erase(simplex.points.begin() + 1); // Remove B
            direction = ACPerp; // Set direction towards AC
            return false;
        }

        // Origin is above the triangle, so we keep the simplex unchanged and the direction as it is
        return true;
    }

    // For line segments, check if the origin is on the "negative" side of the line
    Eigen::Vector2d a = simplex.points[1];
    Eigen::Vector2d b = simplex.points[0];

    Eigen::Vector2d AB = b - a;
    Eigen::Vector2d AO = -a; // Negative of point A, where A is the last added point

    if (AB.dot(AO) <= 0.0) {
        direction = AB; // Set direction towards AB
    } else {
        simplex.points.erase(simplex.points.begin()); // Remove B
        direction = -AO; // Set direction towards A
    }

    return false;
}

Eigen::Vector2d support(std::vector<Eigen::Vector2d> const& shape,
                        Eigen::Vector2d const& direction) {
    double max = -std::numeric_limits<double>::max();
    Eigen::Vector2d maxPoint;

    for (auto const& point : shape) {
        double projection = point.dot(direction);

        if (projection > max) {
            max = projection;
            maxPoint = point;
        }
    }

    return maxPoint;
}

bool gjk(std::vector<Eigen::Vector2d> const& rect1,
         std::vector<Eigen::Vector2d> const& rect2, double& dist) {

    Eigen::Vector2d initialDirection(1.0, 0.0);
    Simplex simplex;
    simplex.points.push_back(support(rect1, initialDirection) - support(rect2, -initialDirection));
    Eigen::Vector2d direction = -simplex.points[0];

    while (true) {
        simplex.points.push_back(support(rect1, direction) - support(rect2, -direction));

        if (simplex.points.back().dot(direction) < 0) {
            return false; // No collision
        } else {
            if (containsOrigin(simplex, direction)) {
                return true; // Collision
            }
        }
    }
}


