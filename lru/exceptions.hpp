#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

/**
	用于debug，也可不用。正常的throw应该够用了。
	throw 字符串用法示例
		try {
			throw " have ␣ a ␣ try " ;
		} catch ( const char * c ){
		std :: cout < <c < < std :: endl ;
		}
	以上代码会输出 have a try 并且在throw后立即终止程序。
		throw " have ␣ a ␣ try " ;
	以上代码会出现terminate called after throwing an instance of ’char const*’ 并且程序会停顿几秒后才终止。
*/

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {
    class exception {
    protected:
        const std::string variant = "";
        std::string detail = "";

    public:
        exception(const std::string& var, const std::string& det) : variant(var), detail(det) {}
        exception() = default;

        exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {
        }

        virtual std::string what() {
            return variant + " " + detail;
        }
    };

    class index_out_of_bound : public exception {
    public:
        index_out_of_bound(const std::string& det = "") : exception("Index out of bound", det) {}
        index_out_of_bound(const index_out_of_bound &ec) : exception(ec) {}
    };
    class runtime_error : public exception {
    public:
        runtime_error(const std::string& det = "") : exception("Runtime error", det)  {}
        runtime_error(const runtime_error &ec) : exception(ec) {}
    };
    class invalid_iterator : public exception {
    public:
        invalid_iterator(const std::string& det = "") : exception("Invalid iterator", det) {}
        invalid_iterator(const invalid_iterator &ec) : exception(ec) {}
    };
    class container_is_empty : public exception {
    public:
        container_is_empty(const std::string& det = "") : exception("Container is empty", det) {}
        container_is_empty(const container_is_empty &ec) : exception(ec) {}
    };
}

#endif

/*
*   try {
        throw sjtu::index_out_of_bound("Tried to access an index out of range");
    } catch (const sjtu::exception& e) {
        std::cout << e.what() << std::endl;
    }
*/
