#ifndef __STAN__AGRAD__FWD__MATRIX__DETERMINANT_HPP__
#define __STAN__AGRAD__FWD__MATRIX__DETERMINANT_HPP__

#include <vector>
#include <boost/math/tools/promotion.hpp>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/agrad/fwd/fvar.hpp>
#include <stan/agrad/fwd/matrix/typedefs.hpp>
#include <stan/math/matrix/multiply.hpp>
#include <stan/agrad/fwd/matrix/multiply.hpp>
#include <stan/math/matrix/inverse.hpp>
#include <stan/agrad/fwd/matrix/inverse.hpp>

namespace stan {
  namespace agrad {
    
    template<typename T, int R,int C>
    inline 
    fvar<T>
    determinant(const Eigen::Matrix<fvar<T>, R, C>& m) {
      using stan::math::multiply;
      using stan::math::inverse;

      stan::math::validate_square(m, "determinant");
      Eigen::Matrix<T,R,C> m_deriv(m.rows(), m.cols());
      Eigen::Matrix<T,R,C> m_val(m.rows(), m.cols());
      Eigen::Matrix<T,R,C> m_inv(m.rows(), m.cols());
      fvar<T> result;

      for(size_type i = 0; i < m.rows(); i++) {
        for(size_type j = 0; j < m.cols(); j++) {
          m_deriv(i,j) = m(i,j).d_;
          m_val(i,j) = m(i,j).val_;
        }
      }

      m_inv = inverse(m_val);
      m_deriv = multiply(m_inv, m_deriv);

      result.val_ = m_val.determinant();
      result.d_ = result.val_ * m_deriv.trace();

      return result;
    }
  }
}
#endif
