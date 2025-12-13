#include "./../common/aoc.hpp"

class AoC2025_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_homework_grand_total();
	int64_t get_homework_grand_total_r2l();
	std::vector<std::vector<std::string>> numbers_;
	std::vector<std::string> ops_;
};

bool AoC2025_day06::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens;
	int64_t size = -1, last_empty_column = -1;
	std::string line;

	ops_.clear();
	numbers_.clear();

	size = lines[0].size();

	for (size_t i = 0; i < lines.size() - 1; i++) {
		numbers_.push_back(std::vector<std::string>());

		if (lines[i].size() != static_cast<size_t>(size)) {
			std::cout << "Error: inconsistent line size at line " << i + 1 << std::endl;
			return false;
		}
	}

	size++;
	for (int64_t j = 0; j < size; j++) {
		bool empty_column = true;

		for (size_t i = 0; i < lines.size(); i++) {
			line = lines[i] + " ";

			if (line[j] != ' ') {
				empty_column = false;
				break;
			}
		}

		if (empty_column) {
			for (size_t k = 0; k < lines.size() - 1; k++) {
				numbers_[k].push_back(lines[k].substr(last_empty_column + 1, j - last_empty_column - 1));
			}

			ops_.push_back(lines.back().substr(last_empty_column + 1, j - last_empty_column - 1));

			last_empty_column = static_cast<int64_t>(j);
		}
	}

	return true;
}

int64_t AoC2025_day06::get_homework_grand_total() {
	int64_t result = 0, subresult;
	bool add;

	for (size_t i = 0; i < numbers_[0].size(); i++) {
		if (ops_[i].find("+") != std::string::npos) {
			add = true;
			subresult = 0;
		} else {
			add = false;
			subresult = 1;
		}

		for (size_t j = 0; j < numbers_.size(); j++) {
			if (add) {
				subresult += std::stoll(numbers_[j][i]);
			} else {
				subresult *= std::stoll(numbers_[j][i]);
			}
		}

		result += subresult;
	}

	return result;
}

int64_t AoC2025_day06::get_homework_grand_total_r2l() {
	int64_t result = 0, subresult, check;
	std::vector<std::string> tmp;
	std::string num;
	bool add;

	for (size_t c = 0; c < numbers_[0].size(); c++) {
		if (ops_[c].find("+") != std::string::npos) {
			add = true;
			subresult = 0;
		} else {
			add = false;
			subresult = 1;
		}

		tmp.clear();

		for (size_t j = 0; j < numbers_[0][c].size(); j++) {
			num = "";
			for (size_t i = 0; i < numbers_.size(); i++) {
				num += numbers_[i][c][j];
			}

			tmp.push_back(num);
		}

		for (size_t i = 0; i < tmp.size(); i++) {
			if (add) {
				subresult += std::stoll(tmp[i]);
			} else {
				subresult *= std::stoll(tmp[i]);
			}
		}

		check = result;
		result += subresult;
		if (result < check) {
			std::cout << "Warning: integer overflow detected in column " << c + 1 << " when adding " << subresult << " to " << check << std::endl;
		}
	}

	return result;
}

int32_t AoC2025_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2025_day06::get_aoc_year() {
	return 2025;
}

void AoC2025_day06::tests() {
	int64_t result;

	if (init({"123 328  51 64 ", " 45 64  387 23 ", "  6 98  215 314", "*   +   *   +  "})) {
		result = get_homework_grand_total();	 // 4277556
		result = get_homework_grand_total_r2l(); // 3263827
	}
}

bool AoC2025_day06::part1() {
	int64_t result = 0;

	result = get_homework_grand_total();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day06::part2() {
	int64_t result = 0;

	result = get_homework_grand_total_r2l();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day06 day06;

	return day06.main_execution();
}
