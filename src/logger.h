#pragma once
#ifndef LOGHPP
#define LOGHPP 1

#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <chrono>
#include <fstream>
#define LOG(x, ...)                               \
	std::cout << '['                              \
			  << shorten_path(__FILE__)   \
			  << ':'                              \
			  << __LINE__                         \
			  << ']'                              \
			  << '('                              \
			  << __PRETTY_FUNCTION__              \
			  << ')'                              \
			  << "\t"                             \
			  << format(x, ##__VA_ARGS__) \
			  << std::endl;




inline const std::string shorten_path(const std::string&& path)
{
	size_t pos = path.find_last_of('\\');
	return std::string(path.begin()+pos+1,path.end());
}



template <typename Ele, typename... Args>
std::string format(Ele ele, Args... args)
{
	std::stringstream ss;
	ss << ele;

	if constexpr (sizeof...(args) > 0)
	{
		return ss.str() + format(args...);
	}
	else
	{
		return ss.str();
	}
}

#endif