#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>

int main() {
  const int k_point_nums       = 128;
  const int k_max_iter         = 50;
  const double k_iter_stop_thr = 0.0001;

  std::srand(time(NULL));
  Eigen::Vector3d tmp = Eigen::Vector3d::Random();
  Eigen::AngleAxis<double> angle_axis(tmp.array().abs().sum(), tmp.normalized());
  Eigen::Matrix<double, 3, k_point_nums> in  = Eigen::Matrix<double, 3, k_point_nums>::Random();
  Eigen::Matrix<double, 3, k_point_nums> out = angle_axis.toRotationMatrix() * in + Eigen::Matrix<double, 3, k_point_nums>::Random() * 0.1;
  std::cout << "rotation:\n"
            << angle_axis.toRotationMatrix() << "\n";

  Eigen::AngleAxis<double> estimate_angle_axis(0, Eigen::Vector3d::Zero());
  {
    double err = (estimate_angle_axis.toRotationMatrix() * in - out).matrix().squaredNorm() / k_point_nums;
    printf("init err = %lf\n", err);
  }

  double u = 1;
  double v = 2;

  // (Jt * J + uI) * hgm = -Jt * f
  for(int i = 0; i < k_max_iter; ++i) {
    // cal f
    Eigen::Matrix<double, 3, k_point_nums> cur = estimate_angle_axis.toRotationMatrix() * in;
    Eigen::Matrix<double, 3 * k_point_nums, 1> f;
    for(int j = 0; j < k_point_nums; ++j) {
      f(3 * j + 0, 0) = cur(0, j) - out(0, j);
      f(3 * j + 1, 0) = cur(1, j) - out(1, j);
      f(3 * j + 2, 0) = cur(2, j) - out(2, j);
    }

    // cal J
    // J = [ 0   z  -y
    //      -z   0   x
    //       y  -x   0 ]
    Eigen::Matrix<double, 3 * k_point_nums, 3> J;
    for(int j = 0; j < k_point_nums; ++j) {
      J(3 * j + 0, 0) = 0;
      J(3 * j + 0, 1) = cur(2, j);
      J(3 * j + 0, 2) = -cur(1, j);
      J(3 * j + 1, 0) = -cur(2, j);
      J(3 * j + 1, 1) = 0;
      J(3 * j + 1, 2) = cur(0, j);
      J(3 * j + 2, 0) = cur(1, j);
      J(3 * j + 2, 1) = -cur(0, j);
      J(3 * j + 2, 2) = 0;
    }

    // cal hgm
    Eigen::Matrix<double, 3, 3> JtJ = J.transpose() * J;
    if(i == 0) {
      u = std::max(JtJ(0, 0), std::max(JtJ(1, 1), JtJ(2, 2)));
    }
    JtJ                               = JtJ + u * Eigen::Matrix<double, 3, 3>::Identity();
    Eigen::Matrix<double, 3, 1> g_opp = -J.transpose() * f;
    Eigen::Vector3d hlm               = JtJ.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(g_opp);

    // cal new angle_axis
    double theta = hlm.norm();
    Eigen::AngleAxis<double> deta(theta, hlm.normalized());
    Eigen::AngleAxis<double> new_angle_axis;
    new_angle_axis = deta * estimate_angle_axis;

    if(theta < k_iter_stop_thr * estimate_angle_axis.angle()) break;

    // cal rho
    double actual = (cur - out).matrix().squaredNorm() -
                    (new_angle_axis.toRotationMatrix() * in - out).matrix().squaredNorm();
    double estimate = hlm.transpose() * (u * hlm + g_opp);
    double rho      = actual / estimate;

    // update angle_axis
    if(rho > 0) {
      estimate_angle_axis = new_angle_axis;

      u = u * std::max(1. / 3., 1 - std::pow(2 * rho - 1, 3));
      v = 2;
    } else {
      u = u * v;
      v = v * 2;
    }

    double err = (estimate_angle_axis.toRotationMatrix() * in - out).matrix().squaredNorm() / k_point_nums;
    printf("iter %d err = %lf\n", i, err);
  }

  {
    double err = (estimate_angle_axis.toRotationMatrix() * in - out).matrix().squaredNorm() / k_point_nums;
    printf("final err = %lf\n", err);
  }
  std::cout << "estimate rotation:\n"
            << estimate_angle_axis.toRotationMatrix() << "\n";

  return 0;
}
