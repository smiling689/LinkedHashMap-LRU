#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP

/**
    矩阵类，对象构造示例如下：
        Matrix < int > a = Matrix < int >(1 ,2 ,3);
        Matrix < int > * p = new Matrix < int >(1 ,2 ,3);
        std :: cout << a << " " << *p << std :: endl ;
    构造1*2，填充的数字都为3的矩阵
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

// 定义一个模板类 Matrix，模板参数为 _Td，表示矩阵中元素的类型
template<typename _Td>
class Matrix {
protected:
    size_t n_rows = 0; // 矩阵的行数
    size_t n_cols = 0; // 矩阵的列数
    std::vector<std::vector<_Td> > data; // 二维向量，用于存储矩阵的数据

    //————————————————————————————————————————————————————————//
    // 内部类 RowProxy，用于代理矩阵的一行，方便对矩阵元素进行访问
    class RowProxy {
        std::vector<_Td> &row; // 引用矩阵中的一行

    public:
        // 构造函数，接收一个向量引用，初始化 row
        RowProxy(std::vector<_Td> &_row) : row(_row) {
        }

        // 重载 [] 运算符，用于访问行中的元素
        _Td &operator[](const size_t &pos) {
            return row[pos];
        }
    };


    //————————————————————————————————————————————————————————//
    // 内部类 ConstRowProxy，用于代理常量矩阵的一行，方便对常量矩阵元素进行访问
    class ConstRowProxy {
        const std::vector<_Td> &row; // 引用常量矩阵中的一行

    public:
        // 构造函数，接收一个常量向量引用，初始化 row
        ConstRowProxy(const std::vector<_Td> &_row) : row(_row) {
        }

        // 重载 [] 运算符，用于访问常量行中的元素，返回常量引用
        const _Td &operator[](const size_t &pos) const {
            return row[pos];
        }
    };

public:
    // 默认构造函数，创建一个空矩阵
    Matrix() {
    };

    // 构造函数，根据指定的行数和列数创建矩阵，元素初始化为默认值
    Matrix(const size_t &_n_rows, const size_t &_n_cols)
        : n_rows(_n_rows), n_cols(_n_cols), data(std::vector<std::vector<_Td> >(n_rows, std::vector<_Td>(n_cols))) {
    }

    // 构造函数，根据指定的行数、列数和填充值创建矩阵
    Matrix(const size_t &_n_rows, const size_t &_n_cols, const _Td &fillValue)
        : n_rows(_n_rows), n_cols(_n_cols),
          data(std::vector<std::vector<_Td> >(n_rows, std::vector<_Td>(n_cols, fillValue))) {
    }

    // 拷贝构造函数，用于创建一个新矩阵，其内容与另一个矩阵相同
    Matrix(const Matrix<_Td> &mat)
        : n_rows(mat.n_rows), n_cols(mat.n_cols), data(mat.data) {
    }

    // 移动构造函数，用于高效地将一个临时矩阵的资源转移到新矩阵中
    Matrix(Matrix<_Td> &&mat) noexcept
        : n_rows(mat.n_rows), n_cols(mat.n_cols), data(mat.data) {
    }

    // 拷贝赋值运算符，用于将一个矩阵的内容复制到另一个矩阵中
    Matrix<_Td> &operator=(const Matrix<_Td> &rhs) {
        this->n_rows = rhs.n_rows;
        this->n_cols = rhs.n_cols;
        this->data = rhs.data;
        return *this;
    }

    // 移动赋值运算符，用于高效地将一个临时矩阵的资源转移到另一个矩阵中
    Matrix<_Td> &operator=(Matrix<_Td> &&rhs) {
        this->n_rows = rhs.n_rows;
        this->n_cols = rhs.n_cols;
        this->data = rhs.data;
        return *this;
    }

    // 获取矩阵行数的常量成员函数，返回行数的常量引用
    inline const size_t &RowSize() const {
        return n_rows;
    }

    // 获取矩阵列数的常量成员函数，返回列数的常量引用
    inline const size_t &ColSize() const {
        return n_cols;
    }

    // 重载 [] 运算符，返回 RowProxy 对象，用于访问矩阵的某一行
    RowProxy operator[](const size_t &Kth) {
        return RowProxy(this->data[Kth]);
    }

    // 重载 [] 运算符，用于常量矩阵，返回 ConstRowProxy 对象，用于访问常量矩阵的某一行
    const ConstRowProxy operator[](const size_t &Kth) const {
        return ConstRowProxy(this->data[Kth]);
    }

    // 默认析构函数
    ~Matrix() = default;
};

/**
 * 两个矩阵相加的运算符重载函数
 */
template<typename _Td>
Matrix<_Td> operator+(const Matrix<_Td> &a, const Matrix<_Td> &b) {
    // 检查两个矩阵的行数和列数是否相同，如果不同则抛出异常
    if (a.RowSize() != b.RowSize() || a.ColSize() != b.ColSize()) {
        throw std::invalid_argument("different matrics\'s sizes");
    }
    // 创建一个新矩阵，用于存储相加的结果
    Matrix<_Td> c(a.RowSize(), a.ColSize());
    // 遍历矩阵的每一个元素，将对应位置的元素相加
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

// 两个矩阵相减的运算符重载函数
template<typename _Td>
Matrix<_Td> operator-(const Matrix<_Td> &a, const Matrix<_Td> &b) {
    // 检查两个矩阵的行数和列数是否相同，如果不同则抛出异常
    if (a.RowSize() != b.RowSize() || a.ColSize() != b.ColSize()) {
        throw std::invalid_argument("different matrics\'s sizes");
    }
    // 创建一个新矩阵，用于存储相减的结果
    Matrix<_Td> c(a.RowSize(), a.ColSize());
    // 遍历矩阵的每一个元素，将对应位置的元素相减
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

// 判断两个矩阵是否相等的运算符重载函数
template<typename _Td>
bool operator==(const Matrix<_Td> &a, const Matrix<_Td> &b) {
    // 检查两个矩阵的行数和列数是否相同，如果不同则返回 false
    if (a.RowSize() != b.RowSize() || a.ColSize() != b.ColSize()) {
        return false;
    }
    // 遍历矩阵的每一个元素，检查对应位置的元素是否相等
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            if (a[i][j] != b[i][j])
                return false;
        }
    }
    return true;
}

// 矩阵取负的运算符重载函数
template<typename _Td>
Matrix<_Td> operator-(const Matrix<_Td> &mat) {
    // 创建一个新矩阵，用于存储取负的结果
    Matrix<_Td> result(mat.RowSize(), mat.ColSize());
    // 遍历矩阵的每一个元素，将其取负
    for (size_t i = 0; i < mat.RowSize(); ++i) {
        for (size_t j = 0; j < mat.ColSize(); ++j) {
            result[i][j] = -mat[i][j];
        }
    }
    return result;
}

// 移动语义的矩阵取负的运算符重载函数
template<typename _Td>
Matrix<_Td> operator-(Matrix<_Td> &&mat) {
    // 遍历矩阵的每一个元素，将其取负
    for (size_t i = 0; i < mat.RowSize(); ++i) {
        for (size_t j = 0; j < mat.ColSize(); ++j) {
            mat[i][j] = -mat[i][j];
        }
    }
    return mat;
}

/**
 * 两个矩阵相乘的运算符重载函数
 */
template<typename _Td>
Matrix<_Td> operator*(const Matrix<_Td> &a, const Matrix<_Td> &b) {
    // 检查第一个矩阵的列数是否等于第二个矩阵的行数，如果不同则抛出异常
    if (a.ColSize() != b.RowSize()) {
        throw std::invalid_argument("different matrics\'s sizes");
    }
    // 创建一个新矩阵，用于存储相乘的结果
    Matrix<_Td> c(a.RowSize(), b.ColSize(), 0);
    // 矩阵乘法的核心逻辑，使用三重循环计算结果矩阵的每一个元素
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < b.ColSize(); ++j) {
            for (size_t k = 0; k < a.ColSize(); ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

/**
 * 矩阵与数相乘的运算符重载函数
 */
template<typename _Td>
Matrix<_Td> operator*(const Matrix<_Td> &a, const _Td &b) {
    // 创建一个新矩阵，用于存储相乘的结果
    Matrix<_Td> c(a.RowSize(), a.ColSize());
    // 遍历矩阵的每一个元素，将其与数相乘
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            c[i][j] = a[i][j] * b;
        }
    }
    return c;
}

// 数与矩阵相乘的运算符重载函数
template<typename _Td>
Matrix<_Td> operator*(const _Td &b, const Matrix<_Td> &a) {
    // 创建一个新矩阵，用于存储相乘的结果
    Matrix<_Td> c(a.RowSize(), a.ColSize());
    // 遍历矩阵的每一个元素，将其与数相乘
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            c[i][j] = a[i][j] * b;
        }
    }
    return c;
}

// 矩阵除以数的运算符重载函数
template<typename _Td>
Matrix<_Td> operator/(const Matrix<_Td> &a, const double &b) {
    // 创建一个新矩阵，用于存储相除的结果
    Matrix<_Td> c(a.RowSize(), a.ColSize());
    // 遍历矩阵的每一个元素，将其除以数
    for (size_t i = 0; i < a.RowSize(); ++i) {
        for (size_t j = 0; j < a.ColSize(); ++j) {
            c[i][j] = a[i][j] / b;
        }
    }
    return c;
}

// 矩阵转置函数
template<typename _Td>
Matrix<_Td> Transpose(const Matrix<_Td> &a) {
    // 创建一个新矩阵，行数和列数与原矩阵相反
    Matrix<_Td> res(a.ColSize(), a.RowSize());
    // 遍历原矩阵，将元素转置到新矩阵中
    for (size_t i = 0; i < a.ColSize(); ++i) {
        for (size_t j = 0; j < a.RowSize(); ++j) {
            res[i][j] = a[j][i];
        }
    }
    return res;
}

// 矩阵输出运算符重载函数
template<typename _Td>
std::ostream &operator<<(std::ostream &stream, const Matrix<_Td> &mat) {
    // 保存原有的输出格式标志
    std::ostream::fmtflags oldFlags = stream.flags();
    // 设置输出精度为 8 位
    stream.precision(8);
    // 设置输出格式为固定小数位和右对齐
    stream.setf(std::ios::fixed | std::ios::right);

    // 输出换行符
    stream << '\n';
    // 遍历矩阵的每一个元素，按格式输出
    for (size_t i = 0; i < mat.RowSize(); ++i) {
        for (size_t j = 0; j < mat.ColSize(); ++j) {
            stream << std::setw(15) << mat[i][j];
        }
        stream << '\n';
    }

    // 恢复原有的输出格式标志
    stream.flags(oldFlags);
    return stream;
}

// 生成单位矩阵的函数
template<typename _Td>
Matrix<_Td> I(const size_t &n) {
    // 创建一个 n x n 的矩阵，元素初始化为 0
    Matrix<_Td> res(n, n, 0);
    // 将矩阵的主对角线元素设置为 1
    for (size_t i = 0; i < n; ++i) {
        res[i][i] = static_cast<_Td>(1);
    }
    return res;
}

// 矩阵幂运算函数
template<typename _Td>
Matrix<_Td> Pow(Matrix<_Td> A, size_t &b) {
    // 检查矩阵是否为方阵，如果不是则抛出异常
    if (A.RowSize() != A.ColSize()) {
        throw std::invalid_argument("The row size and column size are different.");
    }
    // 创建一个单位矩阵，用于存储结果
    Matrix<_Td> result = I<_Td>(A.ColSize());
    // 使用快速幂算法计算矩阵的幂
    while (b > 0) {
        if (b & static_cast<size_t>(1)) {
            result = result * A;
        }
        A = A * A;
        b = b >> static_cast<size_t>(1);
    }
    return result;
}

#endif