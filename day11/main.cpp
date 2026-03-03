#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <set>

const std::string C_START_PART1 = "you";
const std::string C_START_PART2 = "svr";
const std::string C_FINISH = "out";
const std::string C_FFT = "fft";
const std::string C_DAC = "dac";

struct path_info_str {
	std::string next;
	bool fft, dac;
	std::set<std::string> visited;
};

class AoC2025_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_full_paths_count(const std::string start, const std::string finish);
	int64_t get_full_paths_svr_count();
	int64_t get_full_paths_svr_count_rekurz(const std::string start, const bool fft, const bool dac);
	std::map<std::string, std::set<std::string>> paths_;
	std::map<std::string, int64_t> cache_;
};

bool AoC2025_day11::init(const std::vector<std::string> lines) {
	std::string tmp;
	std::vector<std::string> tokens, tokens2;

	paths_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		tokens = split(lines[i], ":");

		if (tokens.size() != 2) {
			std::cout << "Invalid path format at line " << i + 1 << std::endl;
			return false;
		}

		tokens2 = split(tokens[1], " ");

		if (tokens2.size() == 0) {
			std::cout << "Invalid path format at line " << i + 1 << std::endl;
			return false;
		}

		for (const auto& target : tokens2) {
			paths_[tokens[0]].insert(target);
		}
	}

	return true;
}

int64_t AoC2025_day11::get_full_paths_count(const std::string start, const std::string finish) {
	std::set<std::string> visited;
	std::vector<std::string> to_visit;
	int64_t count = 0;

	to_visit.push_back(start);

	while (!to_visit.empty()) {
		std::string current = to_visit.back();
		to_visit.pop_back();

		if (current == finish) {
			count++;
			continue;
		}

		for (const auto& next : paths_[current]) {
			to_visit.push_back(next);
		}
	}

	return count;
}

int64_t AoC2025_day11::get_full_paths_svr_count() {
	int64_t result;

	cache_.clear();

	result = get_full_paths_svr_count_rekurz(C_START_PART2, false, false);

	return result;
}

int64_t AoC2025_day11::get_full_paths_svr_count_rekurz(const std::string start, const bool fft, const bool dac) {
	int64_t result = 0;

	if (start == C_FINISH) {
		if (fft && dac) {
			return 1;
			//		} else {
			//			return 0;
		}
	}
	const std::string cache_key = start + (fft ? "1" : "0") + (dac ? "1" : "0");

	if (cache_.count(cache_key) != 0) {
		return cache_[cache_key];
	}

	if (paths_.count(start) == 0) {
		return 0;
	}

	for (const auto& next : paths_[start]) {
		result += get_full_paths_svr_count_rekurz(next, fft || next == C_FFT, dac || next == C_DAC);
	}

	cache_[cache_key] = result;

	return result;
}

int32_t AoC2025_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2025_day11::get_aoc_year() {
	return 2025;
}

void AoC2025_day11::tests() {
	int64_t result;

	if (init({"aaa: you hhh", "you: bbb ccc", "bbb: ddd eee", "ccc: ddd eee fff", "ddd: ggg", "eee: out", "fff: out", "ggg: out", "hhh: ccc fff iii",
			  "iii: out"})) {
		result = get_full_paths_count(C_START_PART1, C_FINISH); // 5
	}

	if (init({"svr: aaa bbb", "aaa: fft", "fft: ccc", "bbb: tty", "tty: ccc", "ccc: ddd eee", "ddd: hub", "hub: fff", "eee: dac", "dac: fff", "fff: ggg hhh",
			  "ggg: out", "hhh: out"})) {
		result = get_full_paths_svr_count(); // 2
	}
}

bool AoC2025_day11::part1() {
	int64_t result = 0;

	result = get_full_paths_count(C_START_PART1, C_FINISH);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day11::part2() {
	int64_t result = 0;

	result = get_full_paths_svr_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day11 day11;

	return day11.main_execution();
}
