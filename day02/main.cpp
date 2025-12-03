#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"

class AoC2025_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_invalid_ids_sum(const bool part2 = false);
	std::vector<std::pair<int64_t, int64_t>> ranges_;
};

bool AoC2025_day02::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens, range;

	ranges_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		tokens = split(lines[i], ",");

		for (size_t j = 0; j < tokens.size(); j++) {
			range = split(tokens[j], "-");
			if (range.size() != 2) {
				std::cout << "Invalid range >" << tokens[j] << "<" << std::endl;
				return false;
			}
			ranges_.push_back({std::stoll(range[0]), std::stoll(range[1])});
		}
	}

	return true;
}

int64_t AoC2025_day02::get_invalid_ids_sum(const bool part2) {
	int64_t result = 0, len, group_cnt;
	std::string id;
	bool invalid;

	for (size_t i = 0; i < ranges_.size(); i++) {
		for (int64_t j = ranges_[i].first; j <= ranges_[i].second; j++) {
			id = std::to_string(j);

			if (part2) {
				len = id.size();

				for (int64_t group_size = 1; group_size <= len / 2; group_size++) {
					if (len % group_size != 0) {
						continue;
					}
					group_cnt = len / group_size;
					
					invalid = true;

					for (int64_t idx = 0; idx < group_size; idx++) {
						for (int64_t group_idx = 1; group_idx < group_cnt; group_idx++) {
							if (id[idx] != id[idx + group_idx * group_size]) {
								invalid = false;
								break;
							}
						}

						if (!invalid) {
							break;
						}
					}
					if (invalid) {
						result += j;
						break;
					}
				}
			} else {
				if (id.size() % 2 != 0) {
					continue;
				}

				invalid = true;

				for (size_t idx = 0; idx < id.size() / 2; idx++) {
					if (id[idx] != id[idx + id.size() / 2]) {
						invalid = false;
						break;
					}
				}

				if (invalid) {
					result += j;
				}
			}
		}
	}

	return result;
}

int32_t AoC2025_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2025_day02::get_aoc_year() {
	return 2025;
}

void AoC2025_day02::tests() {
	int64_t result;

	if (init({"11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,"
			  "2121212118-2121212124"})) {
		result = get_invalid_ids_sum();		// 1227775554
		result = get_invalid_ids_sum(true); // 4174379265
	}
}

bool AoC2025_day02::part1() {
	int64_t result = 0;

	result = get_invalid_ids_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day02::part2() {
	int64_t result = 0;

	result = get_invalid_ids_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day02 day02;

	return day02.main_execution();
}
