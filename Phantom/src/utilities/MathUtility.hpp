#include "stdafx.h"
#pragma once

/**
 * @file MathUtility.hpp
 * @brief Mathematical utility <br/> 数学工具
 * @author 赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef MATH_UTILITY_HPP
#define MATH_UTILITY_HPP

/**
 * @struct Math
 * @brief Mathematical function class. <br/> 数学函数类
 * @author 赵启轩 <QixuanZhao@outlook.com>
 */
struct MathUtility {
	static std::uniform_real_distribution<float> randomFloats;
	static std::default_random_engine generator;

	/**
	 * @brief returns elementwise maximum vector <br/> 返回逐元素比较的最大值
	 * @param a vector a <br/> 向量 a
	 * @param b vector b <br/> 向量 b
	 */
	template<typename T, unsigned int length, glm::qualifier Q>
	inline static vec<length, T, Q> emax(const vec<length, T, Q>& a, const vec<length, T, Q>& b) {
		vec<length, T, Q> result;
		for (int i = 0; i < length; i++)
			result[i] = glm::max(a[i], b[i]);

		return result;
	}

	/**
	 * @brief returns elementwise minimum vector <br/> 返回逐元素比较的最小值
	 * @param a vector a <br/> 向量 a
	 * @param b vector b <br/> 向量 b
	 */
	template<typename T, unsigned int length, glm::qualifier Q>
	inline static vec<length, T, Q> emin(const vec<length, T, Q>& a, const vec<length, T, Q>& b) {
		vec<length, T, Q> result;
		for (int i = 0; i < length; i++)
			result[i] = glm::min(a[i], b[i]);

		return result;
	}

	template<typename T>
	inline static T max(std::initializer_list<T> list) {
		if (list.size() == 0) return 0;

		T max;
		bool first = true;
		for (const T& element: list) {
			if (first) {
				first = false;
				max = element;
			}
			else {
				max = glm::max(element, max);
			}
		}

		return max;
	}

	template<typename T>
	inline static T min(std::initializer_list<T> list) {
		if (list.size() == 0) return 0;

		T min;
		bool first = true;
		for (const T& element : list) {
			if (first) {
				first = false;
				min = element;
			}
			else {
				min = glm::max(element, min);
			}
		}

		return min;
	}

	inline static float random() {
		return randomFloats(generator);
	}

	inline static float random(float from, float to) {
		return randomFloats(generator) * (to - from) + from;
	}

};

std::uniform_real_distribution<float> MathUtility::randomFloats(0.0, 1.0);
std::default_random_engine generator;

#endif