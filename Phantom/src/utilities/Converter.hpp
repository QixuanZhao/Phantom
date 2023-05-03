#pragma once

#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include "stdafx.h"
#include <locale>
#include <codecvt>

class Converter {
public:
	inline static wstring convert(const string& str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	inline static string convert(const wstring& str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(str);
	}
};

#endif
