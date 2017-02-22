#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>
#include <numeric>

namespace AlgDev
{
	struct Result
	{
		std::pair<int, int> images_ = {0, 0};
		double match_ =  0.;

		Result()
		{}
		Result(std::pair<int, int>&& images, double match)
			: images_(std::move(images))
			, match_(match)
		{}

		bool isRelevant() const
		{
			return images_.second == images_.first ||
				images_.first % 2 == 0 && images_.second - images_.first == 1;
		}

		bool operator < (const Result& right) const
		{
			return match_ < right.match_;
		}
		bool operator > (const Result& right) const
		{
			return match_ > right.match_;
		}

		friend std::ostream& operator << (std::ostream& strm, const Result& result)
		{
			strm << result.images_.first << ' ' << result.images_.second << ' ' << result.match_;
			if (result.isRelevant())
			{
				strm << " *";
			}
			return strm;
		}
	};

	// 1. sort result by match
	// 2. find last (with worst match value) pair that should be matched
	// 3. considering all pairs from top to that pair as 'matched', calculate precision, recall and F
	// result is passed by UR, because we need a copy. EMC++Item41
	template <class ResultT, class Compare>
	void AlgDev::Analyse(ResultT&& result, Compare&& compare)
	{
		ResultT sorted_result(std::forward<ResultT>(result));
		// 1.
		std::sort(std::begin(sorted_result), std::end(sorted_result), std::move(compare));
		for (const Result& result : sorted_result)
		{
			std::cout << result << std::endl;
		}

		// 2.
		auto last_relevant = std::find_if(std::rbegin(sorted_result), std::rend(sorted_result),
			std::const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant)).base();

		// 3.
		const size_t selected_count = std::distance(std::begin(sorted_result), last_relevant) + 1;
		const size_t images_count =
			static_cast<size_t>((-1 + std::sqrt(1 + 4 * 2 * std::size(sorted_result))) / 2);
		const size_t relevant_count = std::count_if(std::begin(sorted_result), std::end(sorted_result),
			std::const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));
		const size_t selected_relevant_count = std::count_if(std::begin(sorted_result), last_relevant,
			std::const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));

		const double precision = static_cast<double>(selected_relevant_count) / selected_count;
		const double recall = static_cast<double>(selected_relevant_count) / relevant_count;
		std::wcout << L"Precision: " << precision << std::endl;
		std::wcout << L"Recall: " << recall << std::endl;
		std::wcout << L"F-measure: " << (2 * precision * recall / (precision + recall)) << std::endl;
	}
}
