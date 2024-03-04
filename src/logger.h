#pragma once
#ifndef LOGHPP
#define LOGHPP 1

#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <codecvt>
#include <locale>

// 多字节字符集与宽字节字符集转换
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> _logger_wstr_str_converter;
inline const std::wstring strconv(const std::string &str)
{
	return _logger_wstr_str_converter.from_bytes(str);
}
inline const std::string strconv(const std::wstring &str)
{
	return _logger_wstr_str_converter.to_bytes(str);
}

#define LOG(x, ...)                     \
	{                                   \
		std::wcout                      \
			<< '['                      \
			<< shorten_path(__FILE__)   \
			<< ':'                      \
			<< __LINE__                 \
			<< ']'                      \
			<< '('                      \
			<< __FUNCTION__             \
			<< ')';                     \
		std::wcout                      \
			<< '\t'                     \
			<< format(x, ##__VA_ARGS__) \
			<< std::endl;               \
	}
// 从路径获取文件名
inline const std::wstring shorten_path(const std::string &&path)

{
	size_t pos = path.find_last_of('\\');
	return strconv(
		std::string(path.begin() + pos + 1, path.end()));
}

// 展开参数包 && 格式化
template <typename Ele, typename... Args>
constexpr std::wstring format(Ele ele, Args... args)
{
	std::wstringstream ss;
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
