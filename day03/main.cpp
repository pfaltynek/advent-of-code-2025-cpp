#include "./../common/aoc.hpp"

const int64_t C_PART1_BATTERY_COUNT = 2;
const int64_t C_PART2_BATTERY_COUNT = 12;

class AoC2025_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_best_joltage(const bool part2, const std::string& bank);
	int64_t get_total_output_joltage(const bool part2 = false);
	std::vector<std::string> banks_;
};

bool AoC2025_day03::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens, range;

	banks_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (!lines[i].empty()) {
			if (lines[i].find_first_not_of("0123456789") == std::string::npos) {
				banks_.push_back(lines[i]);
			} else {
				std::cout << "Invalid bank at line: " << i + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

int64_t AoC2025_day03::get_best_joltage(const bool part2, const std::string& bank) {
	int64_t best_joltage = 0, size_needed = part2 ? C_PART2_BATTERY_COUNT : C_PART1_BATTERY_COUNT, size_max;
	std::vector<int64_t> ref;
	std::string bat = "";

	ref.clear();
	for (int64_t i = 0; i < size_needed; i++) {
		ref.push_back(i);
		bat += bank[i];
	}

	for (int64_t i = 1; i < static_cast<int64_t>(bank.size()); i++) {
		size_max = size_needed;

		if (size_max + i > static_cast<int64_t>(bank.size())) {
			size_max -= i + size_max - static_cast<int64_t>(bank.size());
		}

		int64_t from = size_needed - size_max;
		for (int64_t j = from; j < size_needed; j++) {
			if ((bank[i] > bat[j]) && (i > ref[j])) {
				for (int64_t k = 0; k < static_cast<int64_t>(bat.size()) - j; k++) {
					bat[j + k] = bank[i + k];
					ref[j + k] = i + k;
				}
				break;
			}
		}
	}

	best_joltage = std::stoll(bat);

	return best_joltage;
}

int64_t AoC2025_day03::get_total_output_joltage(const bool part2) {
	int64_t result = 0;

	for (size_t i = 0; i < banks_.size(); i++) {
		result += get_best_joltage(part2, banks_[i]);
	}

	return result;
}

int32_t AoC2025_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2025_day03::get_aoc_year() {
	return 2025;
}

void AoC2025_day03::tests() {
	int64_t result;

	if (init({"987654321111111", "811111111111119", "234234234234278", "818181911112111"})) {
		result = get_total_output_joltage();	 // 357
		result = get_total_output_joltage(true); // 3121910778619
	}
}

bool AoC2025_day03::part1() {
	int64_t result = 0;

	result = get_total_output_joltage();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day03::part2() {
	int64_t result = 0;

	result = get_total_output_joltage(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day03 day03;

	return day03.main_execution();
}
