/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * hprose/io/ClassManager.h                               *
 *                                                        *
 * hprose class manager for cpp.                          *
 *                                                        *
 * LastModified: Nov 6, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>

#define HPROSE_PP_CAT(a, b) HPROSE_PP_CAT_I(a, b)
#define HPROSE_PP_CAT_I(a, b) a ## b

#define HPROSE_PP_VARIADIC_SIZE(...) HPROSE_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#define HPROSE_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

#define HPROSE_PP_OVERLOAD(prefix, ...) HPROSE_PP_CAT(prefix, HPROSE_PP_VARIADIC_SIZE(__VA_ARGS__))

#define HPROSE_REGISTER_1(Class) HPROSE_REGISTER_2(Class, #Class)
#define HPROSE_REGISTER_2(Class, Alias)                      \
namespace hprose { namespace io {                            \
                                                             \
template<>                                                   \
std::string getClassCacheData<Class>() {                     \
    std::stringstream stream;                                \
    stream << tags::TagClass;                                \
    util::WriteInt(stream, util::UTF16Length(Alias));        \
    stream << tags::TagQuote << Alias << tags::TagQuote;     \
    stream << tags::TagOpenbrace;                            \
    stream << tags::TagClosebrace;                           \
    return stream.str();                                     \
}                                                            \
                                                             \
inline void encode(const Class &v, Writer &writer) {         \
    writer.writeObject(v);                                   \
}                                                            \
                                                             \
} } // hprose::io

#define HPROSE_REGISTER(...) HPROSE_PP_OVERLOAD(HPROSE_REGISTER_,__VA_ARGS__)(__VA_ARGS__)

namespace hprose {
namespace io {

struct ClassCache {
    std::string data;
};

template<class T>
std::string getClassCacheData();

class ClassManager {
public:
    static ClassManager &SharedInstance() {
        static ClassManager classManager;
        return classManager;
    }

    template<class T>
    const ClassCache &getClassCache() {
        auto type = std::type_index(typeid(T));
        auto iter = cache.find(type);
        if (iter == cache.end()) {
            ClassCache classCache;
            classCache.data = getClassCacheData<T>();
            cache[type] = classCache;
            return cache[type];
        } else {
            return iter->second;
        }
    }

private:
    std::unordered_map<std::type_index, ClassCache> cache;
};

}
} // hprose::io