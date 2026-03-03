#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <set>

struct machine_str {
	uint32_t lights;
	std::vector<std::pair<uint32_t, std::set<uint32_t>>> buttons;
	std::vector<uint32_t> joltage;
	std::vector<uint32_t> max;
	std::vector<std::vector<uint32_t>> btn_hits;
};

class AoC2025_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_fewest_buttons_presses_sum();
	int64_t get_fewest_buttons_presses_joltage_sum();
	int64_t find_button_presses_to_lights(const uint32_t lights, const std::vector<std::pair<uint32_t, std::set<uint32_t>>>& buttons,
										  std::vector<std::vector<uint32_t>>& button_hits);
	int64_t get_button_presses_for_joltages(std::vector<uint32_t> joltages, const std::vector<std::pair<uint32_t, std::set<uint32_t>>>& buttons);
	bool is_joltages_cleared(const std::vector<uint32_t>& joltages);
	std::vector<machine_str> machines_;
	std::map<std::vector<uint32_t>, int64_t> cache_;
};

bool AoC2025_day10::init(const std::vector<std::string> lines) {
	std::string tmp;
	std::vector<std::string> tokens, tokens2;
	machine_str machine;
	int32_t button;
	uint32_t buttons, bit;

	machines_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		tokens = split(lines[i], " ");

		if (tokens.size() < 3) {
			std::cout << "Invalid machine manual format" << std::endl;
			return false;
		}

		if ((tokens.back().front() != '{') || (tokens.back().back() != '}')) {
			std::cout << "Invalid joltage requirements format" << std::endl;
			return false;
		}

		tokens2 = split(tokens.back().substr(1, tokens.back().size() - 2), ",");
		machine.joltage.clear();
		for (const auto& token : tokens2) {
			button = std::stoi(token);
			machine.joltage.push_back(button);
		}

		tokens.pop_back();

		if ((tokens.front().front() != '[') || (tokens.front().back() != ']')) {
			std::cout << "Invalid lights format" << std::endl;
			return false;
		}

		machine.lights = 0;
		bit = 1;

		for (size_t j = 1; j < tokens.front().size() - 1; j++) {
			if (tokens.front()[j] == '#') {
				machine.lights |= bit;
			}

			bit <<= 1;
		}

		machine.buttons.clear();

		for (size_t j = 1; j < tokens.size(); j++) {
			if ((tokens[j].front() != '(') || (tokens[j].back() != ')')) {
				std::cout << "Invalid button format" << std::endl;
				return false;
			}

			tokens2 = split(tokens[j].substr(1, tokens[j].size() - 2), ",");
			buttons = 0;
			std::set<uint32_t> btns;

			for (const auto& token : tokens2) {
				button = std::stoi(token);

				if ((button < 0) || (button > 31)) {
					std::cout << "Invalid button number" << std::endl;
					return false;
				}
				buttons |= 1 << button;
				btns.insert(button);
			}

			machine.buttons.push_back({buttons, btns});
		}

		machines_.push_back(machine);
	}

	return true;
}

int64_t AoC2025_day10::find_button_presses_to_lights(const uint32_t lights, const std::vector<std::pair<uint32_t, std::set<uint32_t>>>& buttons,
													 std::vector<std::vector<uint32_t>>& button_hits) {
	int64_t buttons_presses_sum_result = INT64_MAX;
	uint32_t lights_new, pos, btn_idx;
	std::vector<uint32_t> btn_hits;

	uint32_t max = 1U << buttons.size();

	button_hits.clear();

	for (uint32_t i = 0; i < max; i++) {
		uint32_t mask = i;
		lights_new = 0;
		pos = 1U;
		btn_idx = 0;
		btn_hits.clear();

		while (mask >= pos) {
			if (mask & pos) {
				lights_new ^= buttons[btn_idx].first;
				btn_hits.push_back(btn_idx);
			}
			pos <<= 1;
			btn_idx++;
		}

		if (lights_new == lights) {
			button_hits.push_back(btn_hits);

			if (btn_hits.size() < static_cast<size_t>(buttons_presses_sum_result)) {
				buttons_presses_sum_result = btn_hits.size();
			}
		}
	}

	return buttons_presses_sum_result;
}

int64_t AoC2025_day10::get_fewest_buttons_presses_sum() {
	int64_t result = 0, machine_result;
	std::vector<std::vector<uint32_t>> button_hits;

	for (const auto& machine : machines_) {
		machine_result = find_button_presses_to_lights(machine.lights, machine.buttons, button_hits);

		result += machine_result;
	}

	return result;
}

bool AoC2025_day10::is_joltages_cleared(const std::vector<uint32_t>& joltages) {
	for (const auto& joltage : joltages) {
		if (joltage) {
			return false;
		}
	}

	return true;
}

/* Very good suggestion from Reddit: joltages should be split to even and odd parts. For odd part can be used part 1 to get steps,
   for even part we can divide all joltages by 2 and repeat the process. And using cache can significantly improve the final speed

   https://old.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/
   */
int64_t AoC2025_day10::get_button_presses_for_joltages(std::vector<uint32_t> joltages, const std::vector<std::pair<uint32_t, std::set<uint32_t>>>& buttons) {
	uint32_t pattern = 0;
	int64_t subresult = INT32_MAX, result = INT32_MAX;
	std::vector<uint32_t> joltages_new = joltages;
	std::vector<std::vector<uint32_t>> button_hits;

	if (cache_.count(joltages)) {
		return cache_[joltages];
	}

	for (size_t i = 0; i < joltages.size(); i++) {
		if (joltages[i] % 2) {
			pattern |= 1U << i;
		}
	}

	find_button_presses_to_lights(pattern, buttons, button_hits);

	for (const auto& btn_hits : button_hits) {
		joltages_new = joltages;
		subresult = INT32_MAX;

		bool underflow = false;
		for (const auto& btn_idx : btn_hits) {
			for (const auto& light_idx : buttons[btn_idx].second) {
				if (joltages_new[light_idx] == 0) {
					underflow = true;
					break;
				} else {
					joltages_new[light_idx]--;
				}
			}

			if (underflow) {
				break;
			}
		}

		if (underflow) {
			continue;
		}

		for (auto& joltage : joltages_new) {
			joltage /= 2;
		}
		subresult = btn_hits.size();
		if (!is_joltages_cleared(joltages_new)) {
			subresult += 2 * get_button_presses_for_joltages(joltages_new, buttons);
		}

		result = std::min(result, subresult);
	}

	cache_[joltages] = result;

	return result;
}

int64_t AoC2025_day10::get_fewest_buttons_presses_joltage_sum() {
	int64_t result = 0, machine_result;
	std::vector<uint32_t> joltages;

	for (auto& machine : machines_) {
		machine_result = INT64_MAX;

		cache_.clear();

		machine_result = get_button_presses_for_joltages(machine.joltage, machine.buttons);

		result += machine_result;
	}

	return result;
}

int32_t AoC2025_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2025_day10::get_aoc_year() {
	return 2025;
}

void AoC2025_day10::tests() {
	int64_t result;

	if (init({"[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}", "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}",
			  "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"})) {
		result = get_fewest_buttons_presses_sum();		   // 7
		result = get_fewest_buttons_presses_joltage_sum(); // 33
	}
}

bool AoC2025_day10::part1() {
	int64_t result = 0;

	result = get_fewest_buttons_presses_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day10::part2() {
	int64_t result = 0;

	result = get_fewest_buttons_presses_joltage_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day10 day10;

	return day10.main_execution();
}
