#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"

class AoC2025_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_fresh_ingredients_count();
	int64_t get_total_ingredients_count();
	std::vector<std::pair<int64_t, int64_t>> ranges_;
	std::vector<int64_t> ids_;
};

bool AoC2025_day05::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens;
	bool ranges = true;

	ids_.clear();
	ranges_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (ranges) {
			if (lines[i].empty()) {
				ranges = false;
				continue;
			}

			tokens = split(lines[i], "-");

			if (tokens.size() != 2) {
				std::cout << "Error: invalid range format at line " << i + 1 << std::endl;
				return false;
			}

			ranges_.push_back(std::make_pair(std::stoll(tokens[0]), std::stoll(tokens[1])));
		} else {
			ids_.push_back(std::stoll((lines[i])));
		}
	}

	return true;
}

int64_t AoC2025_day05::get_fresh_ingredients_count() {
	int64_t result = 0;

	for (size_t i = 0; i < ids_.size(); i++) {
		for (size_t j = 0; j < ranges_.size(); j++) {
			if (ids_[i] >= ranges_[j].first && ids_[i] <= ranges_[j].second) {
				result++;
				break;
			}
		}
	}

	return result;
}

int64_t AoC2025_day05::get_total_ingredients_count() {
	int64_t result = 0;
	bool joined = true;
	std::vector<std::pair<int64_t, int64_t>> new_ranges, ranges = ranges_;
	std::pair<int64_t, int64_t> range;

	do {
		new_ranges.clear();

		while (!ranges.empty()) {
			joined = false;
			range = ranges.back();
			ranges.pop_back();

			for (size_t i = 0; i < new_ranges.size(); i++) {
				if (!(range.second < new_ranges[i].first || range.first > new_ranges[i].second)) {
					new_ranges[i].first = std::min(new_ranges[i].first, range.first);
					new_ranges[i].second = std::max(new_ranges[i].second, range.second);
					joined = true;
					break;
				}
			}

			if (!joined) {
				new_ranges.push_back(range);
			}
		}

		ranges = new_ranges;

	} while (joined);

	for (size_t i = 0; i < ranges.size(); i++) {
		result += (ranges[i].second - ranges[i].first + 1);
	}

	return result;
}

int32_t AoC2025_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2025_day05::get_aoc_year() {
	return 2025;
}

void AoC2025_day05::tests() {
	int64_t result;

	if (init({"3-5", "10-14", "16-20", "12-18", "", "1", "5", "8", "11", "17", "32"})) {
		result = get_fresh_ingredients_count(); // 3
		result = get_total_ingredients_count(); // 14
	}
}

bool AoC2025_day05::part1() {
	int64_t result = 0;

	result = get_fresh_ingredients_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day05::part2() {
	int64_t result = 0;

	result = get_total_ingredients_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day05 day05;

	return day05.main_execution();
}
