#pragma once

namespace SpecFunc {
// * The standard normal CDF, for one random variable.
//* This is the erfc() routine only, adapted by the
//* transform stdnormal_cdf(u)=(erfc(-u/sqrt(2))/2;
// erfc(u) = 2*stdnormal_cdf(-u*sqrt(2))
// erf(u) = 1 - erfc(u) = 1 - 2*stdnormal_cdf(-u*sqrt(2))
    double stdnormal_cdf(double u);
// * The inverse standard normal distribution.
    double stdnormal_inv(double p);

}; //SpecFunc