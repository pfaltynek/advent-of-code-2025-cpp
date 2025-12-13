#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <set>
#include <map>

class AoC2025_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_beam_split_count();
	int64_t get_timelines_count();
	void rekursion(size_t row, size_t col, int64_t& count);
	std::vector<std::string> lines_;
	int64_t s_column_;
	std::map<coord_str, int64_t> cache_;
};

bool AoC2025_day07::init(const std::vector<std::string> lines) {

	lines_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (i == 0) {
			if (lines[i].find_first_not_of(".S") != std::string::npos) {
				std::cout << "Error: ivalid input at first line" << std::endl;
				return false;
			}
			s_column_ = lines[i].find("S");
		} else {
			if (lines[i].find_first_not_of(".^") != std::string::npos) {
				std::cout << "Error: ivalid input at first line" << std::endl;
				return false;
			}
		}
		lines_.push_back(lines[i]);
	}

	return true;
}

int64_t AoC2025_day07::get_beam_split_count() {
	int64_t result = 0;
	std::set<size_t> beam_positions, beam_positions_next;

	beam_positions.insert(s_column_);
	beam_positions_next.clear();

	for (size_t i = 1; i < lines_.size(); i++) {
		for (const auto& pos : beam_positions) {
			if (lines_[i][pos] == '^') {
				beam_positions_next.insert(pos - 1);
				beam_positions_next.insert(pos + 1);
				result++;
			} else {
				beam_positions_next.insert(pos);
			}
		}
		beam_positions = beam_positions_next;
		beam_positions_next.clear();
	}

	return result;
}

void AoC2025_day07::rekursion(size_t row, size_t col, int64_t& count) {
	if (row == lines_.size() - 1) {
		count++;
		return;
	}

	if (lines_[row][col] == '^') {
		if (cache_.count({static_cast<int32_t>(col), static_cast<int32_t>(row)}) != 0) {
			count += cache_[{static_cast<int32_t>(col), static_cast<int32_t>(row)}];
		} else {
			int64_t temp_count = 0;
			rekursion(row + 1, col - 1, temp_count);
			rekursion(row + 1, col + 1, temp_count);
			cache_[{static_cast<int32_t>(col), static_cast<int32_t>(row)}] = temp_count;
			count += temp_count;
		}
	} else {
		rekursion(row + 1, col, count);
	}
}

int64_t AoC2025_day07::get_timelines_count() {
	int64_t result = 0;

	cache_.clear();
	rekursion(0, s_column_, result);

	return result;
}

int32_t AoC2025_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2025_day07::get_aoc_year() {
	return 2025;
}

void AoC2025_day07::tests() {
	int64_t result;

	if (init({".......S.......", "...............", ".......^.......", "...............", "......^.^......", "...............", ".....^.^.^.....",
			  "...............", "....^.^...^....", "...............", "...^.^...^.^...", "...............", "..^...^.....^..", "...............",
			  ".^.^.^.^.^...^.", "..............."})) {
		result = get_beam_split_count(); // 21
		result = get_timelines_count();	 // 40
	}
}

bool AoC2025_day07::part1() {
	int64_t result = 0;

	result = get_beam_split_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day07::part2() {
	int64_t result = 0;

	result = get_timelines_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day07 day07;

	return day07.main_execution();
}
