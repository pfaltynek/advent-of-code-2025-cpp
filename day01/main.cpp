#include "./../common/aoc.hpp"
#include <sstream>

const int64_t C_DIAL_SIZE = 100;

class AoC2025_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_password(const bool part2 = false);
	std::vector<int32_t> rotations_;
};

bool AoC2025_day01::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	char direction;
	int32_t value, sign;

	rotations_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		ss >> direction;
		if (direction == 'L') {
			sign = -1;
		} else if (direction == 'R') {
			sign = 1;
		} else {
			std::cout << "Invalid direction in line " << i + 1 << std::endl;
			return false;
		}

		ss >> value;
		rotations_.push_back(sign * value);
	}

	return true;
}

int64_t AoC2025_day01::get_password(const bool part2) {
	int64_t value = 50, result = 0;

	for (size_t i = 0; i < rotations_.size(); i++) {
		if (part2) {

			if (rotations_[i] >= 0) {
				value += rotations_[i];
				result += (value / C_DIAL_SIZE);
				value = value % C_DIAL_SIZE;
			} else {
				int64_t rest = std::abs(rotations_[i]) % C_DIAL_SIZE;
				int64_t full = std::abs(rotations_[i]) / C_DIAL_SIZE;

				if ((rest >= value) && (value > 0)) {
					result++;
				}
				result += full;
				value -= rest;
				value = (value + C_DIAL_SIZE) % C_DIAL_SIZE;
			}
		} else {
			value += rotations_[i] + C_DIAL_SIZE;
			value %= C_DIAL_SIZE;

			if (value == 0) {
				result++;
			}
		}
	}

	return result;
}

int32_t AoC2025_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2025_day01::get_aoc_year() {
	return 2025;
}

void AoC2025_day01::tests() {
	uint64_t result;

	if (init({"L68", "L30", "R48", "L5", "R60", "L55", "L1", "L99", "R14", "L82"})) {
		result = get_password();	 // 3
		result = get_password(true); // 6
	}
}

bool AoC2025_day01::part1() {
	int64_t result = 0;

	result = get_password();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day01::part2() {
	int64_t result = 0;

	result = get_password(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day01 day01;

	return day01.main_execution();
}
