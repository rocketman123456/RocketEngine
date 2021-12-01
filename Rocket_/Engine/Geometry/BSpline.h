#pragma once
#include "Memory/MemoryDefine.h"

#include <Eigen/Eigen>
#include <vector>

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double u);
        void InsertBSplineKnots(
            const std::vector<Eigen::Vector3f>& cp_pld, 
            const std::vector<double>& knots_old, 
            int32_t p, double u, 
            std::vector<Eigen::Vector3f>& cp, 
            std::vector<double>& knots);
        void SubdividingBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double u, 
            std::vector<Eigen::Vector3f>& cp_left, 
            std::vector<double>& knots_left, 
            std::vector<Eigen::Vector3f>& cp_right, 
            std::vector<double>& knots_right);
        void TesselationBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double tol,
            std::vector<Eigen::Vector3f>& points);
    } // namespace Geometry
}