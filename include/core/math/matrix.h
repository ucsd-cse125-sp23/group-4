#pragma once

#include "core/math/vector.h"

template <typename T>
struct TMat4 {
    TMat4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = T(0);
    }
    template <typename T2>
    TMat4(const T2 s) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = T(s);
    }
    template <typename T2>
    TMat4(const T2* arr) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = (T)arr[i * 4 + j];
    }
    template <typename T2>
    TMat4(const TMat4<T2>& m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                data[i][j] = T(m.data[i][j]);
            }
        }
    }

    template <typename T2>
    TMat4(T2 v00, T2 v01, T2 v02, T2 v03,
          T2 v10, T2 v11, T2 v12, T2 v13,
          T2 v20, T2 v21, T2 v22, T2 v23,
          T2 v30, T2 v31, T2 v32, T2 v33) {
        data[0][0] = (T)v00;
        data[0][1] = (T)v01;
        data[0][2] = (T)v02;
        data[0][3] = (T)v03;
        data[1][0] = (T)v10;
        data[1][1] = (T)v11;
        data[1][2] = (T)v12;
        data[1][3] = (T)v13;
        data[2][0] = (T)v20;
        data[2][1] = (T)v21;
        data[2][2] = (T)v22;
        data[2][3] = (T)v23;
        data[3][0] = (T)v30;
        data[3][1] = (T)v31;
        data[3][2] = (T)v32;
        data[3][3] = (T)v33;
    }

    const T& operator()(int i, int j) const {
        return data[i][j];
    }

    T& operator()(int i, int j) {
        return data[i][j];
    }

    static TMat4<T> identity() {
        TMat4<T> m(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        return m;
    }

    T data[4][4];
};
template <typename T>
inline bool operator==(const TMat4<T>& m0, const TMat4<T>& m1) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (m0(i, j) != m1(i, j))
                return false;
    return true;
}
template <typename T>
inline bool operator!=(const TMat4<T>& m0, const TMat4<T>& m1) { return !(m0 == m1); }

template <typename T>
struct TMat3 {
    TMat3() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                data[i][j] = T(0);
    }
    template <typename T2>
    TMat3(const T2 s) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                data[i][j] = T(s);
    }
    template <typename T2>
    TMat3(const T2* arr) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                data[i][j] = (T)arr[i * 3 + j];
    }
    template <typename T2>
    TMat3(const TMat3<T2>& m) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                data[i][j] = T(m.data[i][j]);
            }
        }
    }
    template <typename T2>
    TMat3(const TMat4<T2>& m) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                data[i][j] = T(m.data[i][j]);
            }
        }
    }

    template <typename T2>
    TMat3(T2 v00, T2 v01, T2 v02,
          T2 v10, T2 v11, T2 v12,
          T2 v20, T2 v21, T2 v22) {
        data[0][0] = (T)v00;
        data[0][1] = (T)v01;
        data[0][2] = (T)v02;
        data[1][0] = (T)v10;
        data[1][1] = (T)v11;
        data[1][2] = (T)v12;
        data[2][0] = (T)v20;
        data[2][1] = (T)v21;
        data[2][2] = (T)v22;
    }

    const T& operator()(int i, int j) const {
        return data[i][j];
    }

    T& operator()(int i, int j) {
        return data[i][j];
    }

    static TMat3<T> identity() {
        TMat3<T> m(1, 0, 0,
                   0, 1, 0,
                   0, 0, 1);
        return m;
    }

    T data[3][3];
};
template <typename T>
inline bool operator==(const TMat3<T>& m0, const TMat3<T>& m1) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (m0(i, j) != m1(i,j))
                return false;
    return true;
}
template <typename T>
inline bool operator!=(const TMat3<T>& m0, const TMat3<T>& m1) { return !(m0 == m1); }


using mat3f = TMat3<float>;
using mat4f = TMat4<float>;
template <typename T>
inline TVector4<T> operator*(const TMat4<T>& m, const TVector4<T>& v) {
    return TVector4<T>(m(0, 0) * v[0] + m(0, 1) * v[1] + m(0, 2) * v[2] + m(0, 3) * v[3],
                    m(1, 0) * v[0] + m(1, 1) * v[1] + m(1, 2) * v[2] + m(1, 3) * v[3],
                    m(2, 0) * v[0] + m(2, 1) * v[1] + m(2, 2) * v[2] + m(2, 3) * v[3],
                    m(3, 0) * v[0] + m(3, 1) * v[1] + m(3, 2) * v[2] + m(3, 3) * v[3]);
}
template <typename T>
inline TVector3<T> operator*(const TMat3<T>& m, const TVector3<T>& v) {
    return TVector3<T>(m(0, 0) * v[0] + m(0, 1) * v[1] + m(0, 2) * v[2],
                    m(1, 0) * v[0] + m(1, 1) * v[1] + m(1, 2) * v[2],
                    m(2, 0) * v[0] + m(2, 1) * v[1] + m(2, 2) * v[2]);
}


template <typename T>
inline TMat4<T> operator+(const TMat4<T>& m0, const TMat4<T>& m1) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) + m1(i, j);
    return ret;
}
template <typename T>
inline TMat4<T> operator-(const TMat4<T>& m0, const TMat4<T>& m1) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) - m1(i, j);
    return ret;
}
template <typename T>
inline TMat4<T>& operator+=(const TMat4<T>& m0, const TMat4<T>& m1) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m0(i, j) += + m1(i, j);
    return m0;
}
template <typename T>
inline TMat4<T>& operator-=(const TMat4<T>& m0, const TMat4<T>& m1) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m0(i, j) -= m1(i, j);
    return m0;
}

template <typename T>
inline TMat4<T> operator*(const TMat4<T>& m0, const TMat4<T>& m1) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret(i, j) = T(0);
            for (int k = 0; k < 4; k++) {
                ret(i, j) += m0(i, k) * m1(k, j);
            }
        }
    }
    return ret;
}
template <typename T>
inline TMat4<T>& operator*=(TMat4<T>& m0, const TMat4<T>& m1) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m0(i, j) *= m1(i, j);
    return m0;
}
template <typename T>
inline TMat4<T> eMul(const TMat4<T>& m0, const TMat4<T>& m1) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) * m1(i, j);
    return ret;
}
template <typename T>
inline TMat4<T> eDiv(const TMat4<T>& m0, const TMat4<T>& m1) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) / m1(i, j);
    return ret;
}

template <typename T>
inline TMat4<T> operator+(const TMat4<T>& m0, const T s) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) + s;
    return ret;
}
template <typename T>
inline TMat4<T> operator-(const TMat4<T>& m0, const T s) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) - s;
    return ret;
}
template <typename T>
inline TMat4<T> operator*(const TMat4<T>& m0, const T s) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = m0(i, j) * s;
    return ret;
}
template <typename T>
inline TMat4<T>& operator*=(TMat4<T>& m0, const T s) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m0(i, j) *= s;
    return m0;
}

template <typename T>
inline TMat4<T> operator+(const T s, const TMat4<T>& m0) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = s + m0(i, j);
    return ret;
}
template <typename T>
inline TMat4<T> operator-(const T s, const TMat4<T>& m0) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = s - m0(i, j);
    return ret;
}
template <typename T>
inline TMat4<T> operator*(const T s, const TMat4<T>& m0) {
    TMat4<T> ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret(i, j) = s * m0(i, j);
    return ret;
}

template <typename T>
TMat4<T> inverse(const TMat4<T>& m) {
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    TMat4<T> inv;

    inv(0, 0) = m(1, 1) * m(2, 2) * m(3, 3) -
        m(1, 1) * m(2, 3) * m(3, 2) -
        m(2, 1) * m(1, 2) * m(3, 3) +
        m(2, 1) * m(1, 3) * m(3, 2) +
        m(3, 1) * m(1, 2) * m(2, 3) -
        m(3, 1) * m(1, 3) * m(2, 2);

    inv(1, 0) = -m(1, 0) * m(2, 2) * m(3, 3) +
        m(1, 0) * m(2, 3) * m(3, 2) +
        m(2, 0) * m(1, 2) * m(3, 3) -
        m(2, 0) * m(1, 3) * m(3, 2) -
        m(3, 0) * m(1, 2) * m(2, 3) +
        m(3, 0) * m(1, 3) * m(2, 2);

    inv(2, 0) = m(1, 0) * m(2, 1) * m(3, 3) -
        m(1, 0) * m(2, 3) * m(3, 1) -
        m(2, 0) * m(1, 1) * m(3, 3) +
        m(2, 0) * m(1, 3) * m(3, 1) +
        m(3, 0) * m(1, 1) * m(2, 3) -
        m(3, 0) * m(1, 3) * m(2, 1);

    inv(3, 0) = -m(1, 0) * m(2, 1) * m(3, 2) +
        m(1, 0) * m(2, 2) * m(3, 1) +
        m(2, 0) * m(1, 1) * m(3, 2) -
        m(2, 0) * m(1, 2) * m(3, 1) -
        m(3, 0) * m(1, 1) * m(2, 2) +
        m(3, 0) * m(1, 2) * m(2, 1);

    inv(0, 1) = -m(0, 1) * m(2, 2) * m(3, 3) +
        m(0, 1) * m(2, 3) * m(3, 2) +
        m(2, 1) * m(0, 2) * m(3, 3) -
        m(2, 1) * m(0, 3) * m(3, 2) -
        m(3, 1) * m(0, 2) * m(2, 3) +
        m(3, 1) * m(0, 3) * m(2, 2);

    inv(1, 1) = m(0, 0) * m(2, 2) * m(3, 3) -
        m(0, 0) * m(2, 3) * m(3, 2) -
        m(2, 0) * m(0, 2) * m(3, 3) +
        m(2, 0) * m(0, 3) * m(3, 2) +
        m(3, 0) * m(0, 2) * m(2, 3) -
        m(3, 0) * m(0, 3) * m(2, 2);

    inv(2, 1) = -m(0, 0) * m(2, 1) * m(3, 3) +
        m(0, 0) * m(2, 3) * m(3, 1) +
        m(2, 0) * m(0, 1) * m(3, 3) -
        m(2, 0) * m(0, 3) * m(3, 1) -
        m(3, 0) * m(0, 1) * m(2, 3) +
        m(3, 0) * m(0, 3) * m(2, 1);

    inv(3, 1) = m(0, 0) * m(2, 1) * m(3, 2) -
        m(0, 0) * m(2, 2) * m(3, 1) -
        m(2, 0) * m(0, 1) * m(3, 2) +
        m(2, 0) * m(0, 2) * m(3, 1) +
        m(3, 0) * m(0, 1) * m(2, 2) -
        m(3, 0) * m(0, 2) * m(2, 1);

    inv(0, 2) = m(0, 1) * m(1, 2) * m(3, 3) -
        m(0, 1) * m(1, 3) * m(3, 2) -
        m(1, 1) * m(0, 2) * m(3, 3) +
        m(1, 1) * m(0, 3) * m(3, 2) +
        m(3, 1) * m(0, 2) * m(1, 3) -
        m(3, 1) * m(0, 3) * m(1, 2);

    inv(1, 2) = -m(0, 0) * m(1, 2) * m(3, 3) +
        m(0, 0) * m(1, 3) * m(3, 2) +
        m(1, 0) * m(0, 2) * m(3, 3) -
        m(1, 0) * m(0, 3) * m(3, 2) -
        m(3, 0) * m(0, 2) * m(1, 3) +
        m(3, 0) * m(0, 3) * m(1, 2);

    inv(2, 2) = m(0, 0) * m(1, 1) * m(3, 3) -
        m(0, 0) * m(1, 3) * m(3, 1) -
        m(1, 0) * m(0, 1) * m(3, 3) +
        m(1, 0) * m(0, 3) * m(3, 1) +
        m(3, 0) * m(0, 1) * m(1, 3) -
        m(3, 0) * m(0, 3) * m(1, 1);

    inv(3, 2) = -m(0, 0) * m(1, 1) * m(3, 2) +
        m(0, 0) * m(1, 2) * m(3, 1) +
        m(1, 0) * m(0, 1) * m(3, 2) -
        m(1, 0) * m(0, 2) * m(3, 1) -
        m(3, 0) * m(0, 1) * m(1, 2) +
        m(3, 0) * m(0, 2) * m(1, 1);

    inv(0, 3) = -m(0, 1) * m(1, 2) * m(2, 3) +
        m(0, 1) * m(1, 3) * m(2, 2) +
        m(1, 1) * m(0, 2) * m(2, 3) -
        m(1, 1) * m(0, 3) * m(2, 2) -
        m(2, 1) * m(0, 2) * m(1, 3) +
        m(2, 1) * m(0, 3) * m(1, 2);

    inv(1, 3) = m(0, 0) * m(1, 2) * m(2, 3) -
        m(0, 0) * m(1, 3) * m(2, 2) -
        m(1, 0) * m(0, 2) * m(2, 3) +
        m(1, 0) * m(0, 3) * m(2, 2) +
        m(2, 0) * m(0, 2) * m(1, 3) -
        m(2, 0) * m(0, 3) * m(1, 2);

    inv(2, 3) = -m(0, 0) * m(1, 1) * m(2, 3) +
        m(0, 0) * m(1, 3) * m(2, 1) +
        m(1, 0) * m(0, 1) * m(2, 3) -
        m(1, 0) * m(0, 3) * m(2, 1) -
        m(2, 0) * m(0, 1) * m(1, 3) +
        m(2, 0) * m(0, 3) * m(1, 1);

    inv(3, 3) = m(0, 0) * m(1, 1) * m(2, 2) -
        m(0, 0) * m(1, 2) * m(2, 1) -
        m(1, 0) * m(0, 1) * m(2, 2) +
        m(1, 0) * m(0, 2) * m(2, 1) +
        m(2, 0) * m(0, 1) * m(1, 2) -
        m(2, 0) * m(0, 2) * m(1, 1);

    T det = m(0, 0) * inv(0, 0) +
        m(0, 1) * inv(1, 0) +
        m(0, 2) * inv(2, 0) +
        m(0, 3) * inv(3, 0);

    if (det == 0) {
        return TMat4<T>{};
    }

    T inv_det = 1.0 / det;

    inv *= inv_det;

    return inv;
}
template <typename T>
TMat4<T> transpose(const TMat4<T>& m) {
    return TMat4(m(0, 0), m(1, 0), m(2, 0), m(3, 0),
                 m(0, 1), m(1, 1), m(2, 1), m(3, 1),
                 m(0, 2), m(1, 2), m(2, 2), m(3, 2),
                 m(0, 3), m(1, 3), m(2, 3), m(3, 3));
}




template <typename T>
inline TMat3<T> operator+(const TMat3<T>& m0, const TMat3<T>& m1) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) + m1(i, j);
    return ret;
}
template <typename T>
inline TMat3<T> operator-(const TMat3<T>& m0, const TMat3<T>& m1) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) - m1(i, j);
    return ret;
}
template <typename T>
inline TMat3<T>& operator+=(const TMat3<T>& m0, const TMat3<T>& m1) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m0(i, j) += +m1(i, j);
    return m0;
}
template <typename T>
inline TMat3<T>& operator-=(const TMat3<T>& m0, const TMat3<T>& m1) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m0(i, j) -= m1(i, j);
    return m0;
}

template <typename T>
inline TMat3<T> operator*(const TMat3<T>& m0, const TMat3<T>& m1) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ret(i, j) = T(0);
            for (int k = 0; k < 4; k++) {
                ret(i, j) += m0(i, k) * m1(k, j);
            }
        }
    }
    return ret;
}
template <typename T>
inline TMat3<T>& operator*=(TMat3<T>& m0, const TMat3<T>& m1) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m0(i, j) *= m1(i, j);
    return m0;
}
template <typename T>
inline TMat3<T> eMul(const TMat3<T>& m0, const TMat3<T>& m1) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) * m1(i, j);
    return ret;
}
template <typename T>
inline TMat3<T> eDiv(const TMat3<T>& m0, const TMat3<T>& m1) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) / m1(i, j);
    return ret;
}

template <typename T>
inline TMat3<T> operator+(const TMat3<T>& m0, const T s) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) + s;
    return ret;
}
template <typename T>
inline TMat3<T> operator-(const TMat3<T>& m0, const T s) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) - s;
    return ret;
}
template <typename T>
inline TMat3<T> operator*(const TMat3<T>& m0, const T s) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = m0(i, j) * s;
    return ret;
}
template <typename T>
inline TMat3<T>& operator*=(TMat3<T>& m0, const T s) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m0(i, j) *= s;
    return m0;
}

template <typename T>
inline TMat3<T> operator+(const T s, const TMat3<T>& m0) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = s + m0(i, j);
    return ret;
}
template <typename T>
inline TMat3<T> operator-(const T s, const TMat3<T>& m0) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = s - m0(i, j);
    return ret;
}
template <typename T>
inline TMat3<T> operator*(const T s, const TMat3<T>& m0) {
    TMat3<T> ret;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ret(i, j) = s * m0(i, j);
    return ret;
}

template <typename T>
TMat3<T> inverse(const TMat3<T>& m) {
    TMat3<T> inv;
    inv(0, 0) = m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1);
    inv(0, 1) =-m(0, 1) * m(2, 2) + m(0, 2) * m(2, 1);
    inv(0, 2) = m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1);
    inv(1, 0) =-m(1, 0) * m(2, 2) + m(1, 2) * m(2, 0);
    inv(1, 1) = m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0);
    inv(1, 2) =-m(0, 0) * m(1, 2) + m(0, 2) * m(1, 0);
    inv(2, 0) = m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0);
    inv(2, 1) =-m(0, 0) * m(2, 1) + m(0, 1) * m(2, 0);
    inv(2, 2) = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);

    T det = m(0, 0) * inv(0, 0) +
            m(0, 1) * inv(1, 0) +
            m(0, 2) * inv(2, 0);

    if (det == 0) {
        return TMat3<T>{};
    }

    T inv_det = T(1.0) / det;

    inv *= inv_det;

    return inv;
}
template <typename T>
TMat3<T> transpose(const TMat3<T>& m) {
    return TMat3(m(0, 0), m(1, 0), m(2, 0),
                 m(0, 1), m(1, 1), m(2, 1),
                 m(0, 2), m(1, 2), m(2, 2));
}



template <typename T>
TMat4<T> translate(TVector3<T> pos) {
    return TMat4<T>(T(1), T(0), T(0), pos[0],
                    T(0), T(1), T(0), pos[1],
                    T(0), T(0), T(1), pos[2],
                    T(0), T(0), T(0), T(1));
}
template <typename T>
TMat4<T> rotate(TVector3<T> rollpitchyaw) {
    T ys = sin(rollpitchyaw[0]);
    T yc = cos(rollpitchyaw[0]);
    T ps = sin(rollpitchyaw[1]);
    T pc = cos(rollpitchyaw[1]);
    T rs = sin(rollpitchyaw[2]);
    T rc = cos(rollpitchyaw[2]);
    return TMat4<T>(yc*pc, yc*ps*rs-ys*rc, yc*ps*rc+ys*rs, T(0),
                    ys*pc, ys*ps*rs+yc*rc, ys*ps*rc-yc*rs, T(0),
                    -ps, pc*rs, pc*rc, T(0),
                    T(0), T(0), T(0), T(1));
}
template <typename T>
TMat4<T> scale(TVector3<T> s) {
    return TMat4<T>(s[0], T(0), T(0), T(0),
                T(0), s[1], T(0), T(0),
                T(0), T(0), s[2], T(0),
                T(0), T(0), T(0), T(1));
}