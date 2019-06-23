#include <vector>
#include <cmath>

template<typename t>
t l2_norm(std::vector<t> const& u, std::vector<t> const& v) {
    t temp, accum = 0.0;
    for (int i = 0; i < u.size(); i++) {
      temp = u[i] - v[i];
      accum += temp * temp;
    }
    return sqrt(accum);
}

template<typename t>
t l1_norm(std::vector<t> const& u, std::vector<t> const& v) {
    t temp, accum = 0.0;
    for (int i = 0; i < u.size(); i++) {
      accum += std::abs(u[i] - v[i]);
    }
    return (accum);
}
