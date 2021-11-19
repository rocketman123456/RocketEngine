#include "Geometry/BSpline.h"
#include "Math/Parameter.h"
#include "Math/Vectors.h"

#include <algorithm>

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double u) {

            int32_t m = knots.size();
            int32_t n = cp.size();
            assert((m-n-p-1) == 0);

            int32_t k = std::distance(knots.begin(), std::upper_bound(knots.begin(), knots.end(), u)) - 1;
            std::vector<Eigen::Vector3f> array;
            array.resize(cp.size());
            std::copy(cp.begin(), cp.end(), array.begin());
            for(int32_t r = 1; r <= p; ++r) {
                for (int32_t i = k, j = p; i >= k-p+r; --i, --j) {
                    double alpha = u - knots[i];
                    double dev = (knots[i+p+1-r]-knots[i]);
                    alpha = (std::abs(dev) < EPS) ? alpha / dev : 0;
                    array[j] = (1.0 - alpha) * array[j - 1] + alpha * array[j];
                }
            }
            return array[p];
        }

        void InsertBSplineKnots(
            const std::vector<Eigen::Vector3f>& cp_pld, 
            const std::vector<double>& knots_old, 
            int32_t p, double u, 
            std::vector<Eigen::Vector3f>& cp, 
            std::vector<double>& knots) {

            knots.resize(knots_old.size());
            std::copy(knots_old.begin(), knots_old.end(), knots.begin());
            cp.resize(cp_pld.size());
            std::copy(cp_pld.begin(), cp_pld.end(), cp.begin());

            //查找 u 所在区间
            int k = std::distance(knots.begin(),std::upper_bound(knots.begin(), knots.end(), u))-1;

            std::vector<Eigen::Vector3f> array;
            array.resize(p);
            //i \in [k,k-p+1]
            //j为数组pArray位置
            for (int32_t i = k, j = p-1; i > k-p; --i,--j) {
                double alpha = (u - knots[i]);
                double dev =  (knots[i + p] - knots[i]);
                alpha = (std::abs(dev) < EPS) ? alpha / dev : 0;
                array[j] = cp[i-1] * (1 - alpha) + cp[i] * alpha;
            }
            //将cv [k-p+1,k-1]替换为pArray，并且在cv[k]之前插入 pArray[p-1]
            for (int i = k-p+1, j = 0; i < k; ++i, ++j) {
                cp[i] = array[j];
            }
            cp.insert(cp.begin() + k, array[p-1]);
            //knots 插入u
            knots.insert(knots.begin() + k + 1, u);
        }

        static void SubdividingBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double u, int k,
            std::vector<Eigen::Vector3f>& cp_left, 
            std::vector<Eigen::Vector3f>& cp_right) {
            
            //保存de boor 算法迭代过程中生成的控制点
            cp_left.resize(p+1);
            cp_right.resize(p+1);
        
            //将 P_k-p,...,P_k拷贝到cv_left上面
            std::copy(cp.begin() + k - p, cp.begin() + k + 1, cp_left.begin());
            cp_right[p] = cp_left[p];
        
            //de-boor 迭代p次
            for (int r = 1; r <= p; ++r) {
                //i 从 k 到 k-p+1
                for (int i = k,j = p; i >= k-p+r; --i,--j) {
                    double alpha = u - knots[i];
                    double dev =  (knots[i + p + 1 - r] - knots[i]);
                    alpha = (std::abs(dev) < EPS) ? alpha / dev : 0;
                    cp_left[j] = (1.0 - alpha) * cp_left[j - 1] + alpha * cp_left[j];
                }
                cp_right[p-r] = cp_left[p];
            }
        }

        void SubdividingBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double u, 
            std::vector<Eigen::Vector3f>& cp_left, 
            std::vector<double>& knots_left, 
            std::vector<Eigen::Vector3f>& cp_right, 
            std::vector<double>& knots_right) {

            int32_t m = knots.size();
            int32_t n = cp.size();
            assert((m-n-p-1) == 0);
        
            //查找 u 所在区间
            int k = std::distance(knots.begin(),std::upper_bound(knots.begin(), knots.end(), u))-1;
        
            //保存de boor 算法迭代过程中生成的控制点
            std::vector<Eigen::Vector3f> cv_left, cv_right;
            SubdividingBSpline(cp, knots, p, u, k, cv_left, cv_right);
        
            //曲线在u处打断，生成新的控制点序列 假设原曲线的控制点序列为 P_0,...,P_k-p-1,P_k-p,...,P_k,P_k+1,...,P_n
            //打断后的新控制点序列为：左侧：P_0,....,P_k-p-1,cv_left[...]，右侧 cv_right[...],P_k+1,...,P_n。
            //sub_left
            //knots
            knots_left.resize(k+p+2);
            for (int i = 0; i < k+1; ++i)
                knots_left[i] = knots[i];
            for (int i = 0,j = k+1; i < p+1; ++i,++j)
                knots_left[j] = u;
            //control vertex
            cp_left.resize(k+1);
            for (int i = 0; i < k-p; ++i)
                cp_left[i] = cp[i];
            for (int i = 0,j = k-p; i < p+1; ++i,++j)
                cp_left[j] = cv_left[i];
            
            //sub_right
            //knots
            knots_right.resize(knots.size()-k+p);
            for (int i = 0; i < p+1; i++)
                knots_right[i] =u;
            for (int i = p+1,j = k+1; j < knots.size(); ++j,++i)
                knots_right[i]=knots[j];
            //control_vertex
            cp_right.resize(cp.size()-k+p);
            for (int i = 0; i < p+1; ++i)
                cp_right[i] = cv_right[i];
            for (int i = k+1,j = p+1; i < cp.size(); ++i,++j)
                cp_right[j] = cp[i]; 
        }

        void TesselationBSpline(
            std::vector<Eigen::Vector3f>& cp, 
            std::vector<double>& knots, 
            int32_t p, double tol, int k_s, int k_e,int c_s,int c_e,
            std::vector<Eigen::Vector3f>& points) {
        
            bool stright_enough = true;
            for (int i = c_s + 1; i < c_e; ++i) {
                if (Math::PointLineDistance(cp[i], cp[c_s], cp[c_e]) > tol) {
                    stright_enough = false;
                    break;
                }
            }
        
            if (stright_enough) {
                int c_end = points.empty() ? c_e+1 : c_e;
                points.insert(points.begin(), cp.begin()+c_s, cp.begin()+c_end);
                return;
            }
        
            double u_mid = knots[k_s] + (knots[k_e] - knots[k_s]) / 2.0;
            int k = std::distance(knots.begin(), std::upper_bound(knots.begin(), knots.end(), u_mid)) - 1;
        
            std::vector<Eigen::Vector3f> cv_left, cv_right;
            SubdividingBSpline(cp, knots, p, u_mid, k, cv_left, cv_right);
            
            knots.insert(knots.begin()+k+1, p, u_mid);
            cp.insert(cp.begin() + k, p, Eigen::Vector3f::Zero());
            for (int i = k - p, j = 0; j <p; ++j, ++i)
                cp[i] = cv_left[j];
            for (int i = k, j = 0; j <= p; ++j, ++i)
                cp[i] = cv_right[j];
        
            TesselationBSpline(cp, knots, p, tol, k+1, k_e+p, k, c_e+p , points);
            TesselationBSpline(cp, knots, p, tol, k_s, k+1, c_s, k, points);
        }

        void TesselationBSpline(
            const std::vector<Eigen::Vector3f>& cp, 
            const std::vector<double>& knots, 
            int32_t p, double tol,
            std::vector<Eigen::Vector3f>& points) {
            
            std::vector<Eigen::Vector3f> cv_copy = cp;
	        std::vector<double> knots_copy = knots;
	        TesselationBSpline(cv_copy, knots_copy, p, tol, p, knots.size()-p, 0, cp.size()-1, points);
        }
    }
}