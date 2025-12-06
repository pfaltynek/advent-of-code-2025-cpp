#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <set>

class AoC2025_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_accessible_rolls_count();
	int64_t get_removable_rolls_count();
	std::set<coord_str> map_;
};

bool AoC2025_day04::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens, range;
	size_t size;

	map_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (!i) {
			size = lines[i].size();
		} else {
			if (lines[i].size() != size) {
				std::cout << "Error: inconsistent line sizes in input at line " << i + 1 << std::endl;
				return false;
			}
		}

		for (size_t j = 0; j < lines[i].size(); j++) {
			if (lines[i][j] == '@') {
				map_.insert({static_cast<int32_t>(j), static_cast<int32_t>(i)});
			}
		}
	}

	return true;
}

int64_t AoC2025_day04::get_accessible_rolls_count() {
	int64_t result = 0, cnt;
	std::set<coord_str> adjacent = {coord_step_east,	   coord_step_west,		  coord_step_north,		 coord_step_south,
									coord_step_north_east, coord_step_north_west, coord_step_south_east, coord_step_south_west};

	for (auto& c : map_) {
		cnt = 0;

		for (const auto& a : adjacent) {
			coord_str check = c;
			check = check + a;

			if (map_.count(check)) {
				cnt++;
			}
		}

		if (cnt < 4) {
			result++;
		}
	}

	return result;
}

int64_t AoC2025_day04::get_removable_rolls_count() {
	int64_t result = 0, cnt;
	std::set<coord_str> adjacent = {coord_step_east,	   coord_step_west,		  coord_step_north,		 coord_step_south,
									coord_step_north_east, coord_step_north_west, coord_step_south_east, coord_step_south_west};
	std::set<coord_str> map = map_, tmp;

	do {
		tmp.clear();
		for (auto& c : map) {
			cnt = 0;

			for (const auto& a : adjacent) {
				coord_str check = c;
				check = check + a;

				if (map.count(check)) {
					cnt++;
				}
			}

			if (cnt < 4) {
				tmp.emplace(c);
			}
		}
		result += tmp.size();

		for(const auto& c : tmp){
			map.erase(c);
		}
	} while (!tmp.empty() > 0);

	return result;
}

int32_t AoC2025_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2025_day04::get_aoc_year() {
	return 2025;
}

void AoC2025_day04::tests() {
	int64_t result;

	if (init({"..@@.@@@@.", "@@@.@.@.@@", "@@@@@.@.@@", "@.@@@@..@.", "@@.@@@@.@@", ".@@@@@@@.@", ".@.@.@.@@@", "@.@@@.@@@@", ".@@@@@@@@.", "@.@.@@@.@."})) {
		result = get_accessible_rolls_count(); // 13
		result = get_removable_rolls_count();  // 43
	}
}

bool AoC2025_day04::part1() {
	int64_t result = 0;

	result = get_accessible_rolls_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day04::part2() {
	int64_t result = 0;

	result = get_removable_rolls_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day04 day04;

	return day04.main_execution();
}
