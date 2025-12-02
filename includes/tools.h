#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <sstream>
#include <regex>
#include <iomanip>


#define NULL_STR		""

typedef unsigned int	uint;
typedef unsigned char	uchar;

template <int N> inline float truncateN(float x) {
  constexpr float factor = [] {
    float f = 1.f;
    for (int i = 0; i < N; ++i)
      f *= 10.f;
    return f;
  }();
  int i = static_cast<int>(x * factor);
  return i / factor;
}

inline std::string to_string3(float x)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << x;
    return ss.str();
}

class Tools {
private:
	Tools();
	~Tools();
		
public:
	// Strings tools
	template <typename T> static T fromString(const std::string &str) {
		T value;
		std::istringstream stream(str);
		stream >> value;
		return value;
	}
	
	template <typename T> static std::string toString(const T &value) {
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}
	
	static std::string toLowerString(std::string str);
	static std::string toUpperString(std::string str);
	
	static bool isNumber(const std::string &str);

	template<int N>
	inline float truncateN(float x) {
		constexpr float factor = []{
			float f = 1.f;
			for (int i = 0; i < N; ++i) f *= 10.f;
			return f;
		}();
		int i = static_cast<int>(x * factor);
		return i / factor;
	}
};

#endif // TOOLS_H