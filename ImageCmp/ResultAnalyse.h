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
		using ImgIdxT = std::pair<int, int>;
		ImgIdxT images_ = {0, 0};
		double match_ =  0.;

		Result()
		{}
		Result(ImgIdxT&& images, double match)
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
	void AlgDev::Analyse(ResultT&& result, const Compare& compare, double threshold = 0.)
	{
		using namespace std;

		ResultT sorted_result(forward<ResultT>(result));
		// 1.
		sort(begin(sorted_result), end(sorted_result), compare);
		for (const Result& result : sorted_result)
		{
			cout << result << endl;
		}

		// 2.
		ResultT::const_iterator relevant_end(cbegin(sorted_result));
		if (threshold == 0.)
		{
			auto last_relevant = find_if(rbegin(sorted_result), rend(sorted_result),
				const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));
			if (last_relevant != rend(sorted_result))
			{
				relevant_end = next(last_relevant.base(), 1);
			}
		}
		else
		{
			relevant_end = upper_bound(cbegin(sorted_result), cend(sorted_result), Result(Result::ImgIdxT(0, 0), threshold), compare);
		}

		// 3.
		const size_t selected_count = distance(cbegin(sorted_result), relevant_end);
		const size_t images_count =
			static_cast<size_t>((-1 + sqrt(1 + 4 * 2 * size(sorted_result))) / 2);
		const size_t relevant_count = std::count_if(cbegin(sorted_result), cend(sorted_result),
			const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));
		const size_t selected_relevant_count = count_if(cbegin(sorted_result), relevant_end,
			const_mem_fun_ref_t<bool, const Result>(&Result::isRelevant));

		const double precision = static_cast<double>(selected_relevant_count) / selected_count;
		const double recall = static_cast<double>(selected_relevant_count) / relevant_count;
		wcout << L"Precision: " << precision << endl;
		wcout << L"Recall: " << recall << endl;
		wcout << L"F-measure: " << (2 * precision * recall / (precision + recall)) << endl;
	}
}
