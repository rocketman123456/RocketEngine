#include "Geometry/BezierCurve.h"

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBezierCurve(const std::vector<Eigen::Vector3f>& cp, float t) {
            std::vector<Eigen::Vector3f> cp_copy;
            cp_copy.resize(cp.size());
            std::copy(cp.begin(), cp.end(), cp_copy.begin());
            for(int32_t i = 0; i < cp.size() - 1; ++i) {
                for(int32_t j = 0; j < cp.size() - i - 1; ++j) {
                    cp_copy[j] = (1.0 - t) * cp_copy[j] + t * cp_copy[j + 1];
                }
            }
            return cp_copy[0];
        }

        Eigen::Vector3f CalculateBezierCurveReCursive(const std::vector<Eigen::Vector3f>& cp, float t) {
            if(cp.size()==2) {
                return (1-t)*cp[0] + t*cp[1];
            }
            std::vector<Eigen::Vector3f> vec;
            for(int i = 0; i < cp.size()-1; i++) {
                vec.push_back((1-t)*cp[i] + t*cp[1+i]);
            }
            return CalculateBezierCurveReCursive(vec, t);
        }

        void SubdividingBezierCurve(const std::vector<Eigen::Vector3f>& cp, double u, std::vector<Eigen::Vector3f>& cp_left, std::vector<Eigen::Vector3f>& cp_right) {
            cp_left.resize(cp.size());
            cp_right.resize(cp.size());

            int p = cp.size()-1;
            //将 cv 拷贝到 cv_right,因为每次迭代计算的点数依次少1，
            //因此可以直接用cv_right记录，全部算完后就是所有的尾点集合
            std::copy(cp.begin(), cp.end(), cp_right.begin());
            cp_left[0] = cp_right[0];
        
            //p次迭代
            for (int i = 0; i<p; ++i) {
                for (int j = 0; j < p-i; ++j)
                    cp_right[j] = (1.0 - u) * cp_right[j] + u * cp_right[j+1];
                //左侧控制点为每次迭代的首点
                cp_left[i + 1] = cp_right[0];
            }
        }

        void UpgradeBezierCurve(const std::vector<Eigen::Vector3f>& cp, std::vector<Eigen::Vector3f>& cp_new) {
            int n = cp.size();
            cp_new.resize(n+1);
        
            cp_new.front() = cp.front();
            cp_new.back() = cp.back();
        
            for (int i=1; i<n; ++i) {
                double ratio = ((double)i)/n;
                cp_new[i] = ratio * cp[i-1] + (1.0-ratio) * cp[i];
            }
        }
    }
}
