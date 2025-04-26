#ifndef SJTU_INTEGER_HPP
#define SJTU_INTEGER_HPP

/**
    整数类Integer，静态变量counter记录变量构造/析构的次数。程序运行结束时应该为0
    对象构造和输出示例如下：
        Integer a = Integer (1);
        Integer * p = new Integer (2);
        std :: cout < < a . val < < " ␣ " < <(p - > val ) < < std :: endl ;
*/
class Integer {
public:
    static int counter;//静态成员，用于记录变量构造和析构的次数，最后应该为0
    int val;//值

    Integer(int val) : val(val) { counter++; }
    ~Integer() { counter--; }

    Integer(const Integer &rhs) {
        val = rhs.val;
        counter++;
    }

    bool operator==(const Integer &rhs) {
        return val == rhs.val;
    }
};

int Integer::counter = 0;

#endif
