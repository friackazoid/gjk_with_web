#pragma once

#include <eigen3/Eigen/Core>

bool gjk(std::vector<Eigen::Vector2d> const& rec1,
         std::vector<Eigen::Vector2d> const& rect2, double& dist);
