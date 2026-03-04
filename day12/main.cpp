#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

const size_t C_SHAPE_WIDTH = 3;
const size_t C_SHAPE_HEIGHT = 3;

class AoC2025_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_regions_fitted_count();
	std::vector<std::vector<std::string>> shapes_;
	std::vector<std::vector<int32_t>> fields_;
};

bool AoC2025_day12::init(const std::vector<std::string> lines) {
	bool shapes = true;
	bool shape_idx = false;
	std::stringstream ss;
	std::string tmp;
	int32_t value, w, l;
	std::vector<std::string> shape_lines;
	std::vector<int32_t> field_line;
	char delimiter;

	fields_.clear();
	shapes_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (shapes) {
			if (lines[i].empty()) {
				shape_idx = false;

				if (shape_lines.size() != C_SHAPE_HEIGHT) {
					std::cout << "Invalid shape definition at line " << i + 1 << std::endl;
					return false;
				}

				shapes_.push_back(shape_lines);
				shape_lines.clear();
				continue;
			}

			if (!shape_idx) {
				ss.str(lines[i]);
				ss >> value;

				if (lines[i].back() == ':') {
					shape_idx = true;

					if (value != static_cast<int32_t>(shapes_.size())) {
						std::cout << "Invalid shape index at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					ss >> delimiter;

					if (delimiter == 'x') {
						ss.clear();
						shapes = false;
					} else {
						std::cout << "Shape index expected at line " << i + 1 << std::endl;
						return false;
					}
				}
			} else {
				if (lines[i].size() != C_SHAPE_WIDTH) {
					std::cout << "Invalid shape line length at line " << i + 1 << std::endl;
					return false;
				}

				shape_lines.push_back({lines[i]});
			}
		}

		if (!shapes) {
			ss.clear();
			ss.str(lines[i]);
			ss >> w >> delimiter >> l;

			if (delimiter != 'x') {
				std::cout << "Invalid field dimensions at line " << i + 1 << std::endl;
				return false;
			}

			field_line.clear();
			field_line.push_back(w);
			field_line.push_back(l);

			ss.ignore(2);
			while (!ss.eof()) {
				ss >> value;
				field_line.push_back(value);
			}

			if (field_line.size() != (shapes_.size() + 2)) {
				std::cout << "Invalid field definition at line " << i + 1 << std::endl;
				return false;
			}

			fields_.push_back(field_line);
		}
	}

	return true;
}

int64_t AoC2025_day12::get_regions_fitted_count() {
	int64_t result = 0;
	size_t size, count1, count2, count3;
	std::vector<size_t> shape_sizes;

	shape_sizes.clear();

	for (const auto& shape : shapes_) {
		size = 0;

		for (const auto& line : shape) {
			size += std::count(line.begin(), line.end(), '#');
		}

		shape_sizes.push_back(size);
	}

	for (const auto& field : fields_) {
		size = field[0] * field[1];

		count1 = 0;
		count2 = 0;
		for (size_t i = 0; i < shape_sizes.size(); i++) {
			count1 += field[i + 2] * shape_sizes[i];
			count2 += field[i + 2];
		}

		count3 = (field[0] / C_SHAPE_WIDTH) * (field[1] / C_SHAPE_HEIGHT);

		if (count3 >= count2){
			if (size >= count1) {
				result++;
			}
		} 

	}

	return result;
}

int32_t AoC2025_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2025_day12::get_aoc_year() {
	return 2025;
}

void AoC2025_day12::tests() {
	if (init({"0:",
			  "###",
			  "##.",
			  "##.",
			  "",
			  "1:",
			  "###",
			  "##.",
			  ".##",
			  "",
			  "2:",
			  ".##",
			  "###",
			  "##.",
			  "",
			  "3:",
			  "##.",
			  "###",
			  "##.",
			  "",
			  "4:",
			  "###",
			  "#..",
			  "###",
			  "",
			  "5:",
			  "###",
			  ".#.",
			  "###",
			  "",
			  "4x4: 0 0 0 0 2 0",
			  "12x5: 1 0 1 0 2 2",
			  "12x5: 1 0 1 0 3 2"})) {
		// no tests
	}
}

bool AoC2025_day12::part1() {
	int64_t result = 0;

	result = get_regions_fitted_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day12::part2() {
	int64_t result = 0;

	result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day12 day12;

	return day12.main_execution();
}
