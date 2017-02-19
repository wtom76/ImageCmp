#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>

namespace AlgDev
{
	struct Result
	{
		std::pair<int, int> images_ = {0, 0};
		double match_ =  0.;

		Result(std::pair<int, int>&& images, double match)
			: images_(std::move(images))
			, match_(match)
		{}

		bool isRelevant() const
		{
			if (images_.first % 2 != 0)
			{
				return false;
			}
			const auto distance = images_.second - images_.first;
			return (distance == 0 || distance == 1);
		}
	};

	// 1. sort result by match
	// 2. find last (with worst match value) pair that should be matched
	// 3. considering all pairs from top to that pair as 'matched', calculate precision, recall and F
	// true positives === relevant count in our case
	// result is passed by UR, because we need a copy. EMC++Item41
	template <class ResultT>
	void AlgDev::Analyse(ResultT&& result)
	{
		ResultT sorted_result(std::forward<ResultT>(result));
		// 1.
		std::sort(std::begin(sorted_result), std::end(sorted_result),
			[](const Result& left, const Result& right)
		{
			return left.match_ > right.match_;
		});

		// 2.
		auto last_relevant = std::find_if(std::rbegin(sorted_result), std::rend(sorted_result),
			std::const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));

		// 3.
		const auto selected_count = std::distance(last_relevant, std::rend(sorted_result)) + 1;
		const decltype(selected_count) images_count =
			static_cast<decltype(selected_count)>((-1 + std::sqrt(1 + 4 * 2 * std::size(sorted_result))) / 2);
		const decltype(selected_count) relevant_count = images_count * static_cast<decltype(selected_count)>(1.5); // pairs and same: 0-1 and 0-0, 1-1

		const double precision = static_cast<double>(relevant_count) / selected_count;
		const double recall = 1.;
		std::wcout << L"Precision: " << precision << std::endl;
		std::wcout << L"Recall: " << recall << std::endl;
		std::wcout << L"F-measure: " << (2 * precision * recall / (precision + recall)) << std::endl;
	}
}
