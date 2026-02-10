#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/ranges.hpp"
#include <map>
#include <set>
#include <sstream>

class AoC2025_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t find_largest_rectangle();
	int64_t find_largest_red_green_rectangle();
	bool is_outer(const coord_str& coord);
	bool is_valid_rectangle(const coord_str& c1, const coord_str& c2);
	int64_t get_rectangle_size(const coord_str& c1, const coord_str& c2);
	std::vector<coord_str> corners_;
	coord_str min_coord_, max_coord_;
	std::map<int32_t, std::set<coord_str>> outer_h_, outer_v_;
};

bool AoC2025_day09::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	int32_t x, y;

	corners_.clear();

	min_coord_ = coord_str{INT32_MAX, INT32_MAX};
	max_coord_ = coord_str{INT32_MIN, INT32_MIN};

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);
		ss >> x;
		ss.ignore(1);
		ss >> y;

		corners_.push_back(coord_str{x, y});

		if (x < min_coord_.x) {
			min_coord_.x = x;
		}
		if (y < min_coord_.y) {
			min_coord_.y = y;
		}
		if (x > max_coord_.x) {
			max_coord_.x = x;
		}
		if (y > max_coord_.y) {
			max_coord_.y = y;
		}
	}

	return true;
}

int64_t AoC2025_day09::find_largest_rectangle() {
	int64_t result = 0, size;

	for (size_t i = 0; i < corners_.size() - 1; i++) {
		for (size_t j = i + 1; j < corners_.size(); j++) {
			size = get_rectangle_size(corners_[j], corners_[i]);
			if (size > result) {
				result = size;
			}
		}
	}

	return result;
}

int64_t AoC2025_day09::get_rectangle_size(const coord_str& c1, const coord_str& c2) {

	return (abs(static_cast<int64_t>(c1.x) - static_cast<int64_t>(c2.x)) + 1) * (abs(static_cast<int64_t>(c1.y) - static_cast<int64_t>(c2.y)) + 1);
}

bool AoC2025_day09::is_outer(const coord_str& coord) {
	if (outer_h_.count(coord.y) == 0) {
		return false;
	}

	for (const auto& range : outer_h_[coord.y]) {
		if ((coord.x >= range.x) && (coord.x <= range.y)) {
			return true;
		}
	}

	return false;
}

bool AoC2025_day09::is_valid_rectangle(const coord_str& c1, const coord_str& c2) {

	if (outer_h_.count(c1.y)) {
		for (const auto& range : outer_h_[c1.y]) {
			if (ranges_intersection(c1.x, c2.x, range.x, range.y)) {
				return false;
			}
		}
	}

	if (outer_h_.count(c2.y)) {
		for (const auto& range : outer_h_[c2.y]) {
			if (ranges_intersection(c1.x, c2.x, range.x, range.y)) {
				return false;
			}
		}
	}

	if (outer_v_.count(c1.x)) {
		for (const auto& range : outer_v_[c1.x]) {
			if (ranges_intersection(c1.y, c2.y, range.x, range.y)) {
				return false;
			}
		}
	}

	if (outer_v_.count(c2.x)) {
		for (const auto& range : outer_v_[c2.x]) {
			if (ranges_intersection(c1.y, c2.y, range.x, range.y)) {
				return false;
			}
		}
	}

	return true;
}

int64_t AoC2025_day09::find_largest_red_green_rectangle() {
	std::map<int32_t, coord_str> hedges, vedges;
	int64_t result = 0, size;
	coord_str prev = corners_.back();

	// data visualisation gave me hint that the area looks like circle with one hole from left side
	// making look the area like pac-man, its mouth ends in points:
	// const coord_str upper(94987, 50332);
	// const coord_str lower(94987, 48421);
	// one of these coordinates will be corner of result area

	for (const auto& corner : corners_) {
		if (corner.x == prev.x) {
			vedges[corner.x] = coord_str{std::min(corner.y, prev.y), std::max(corner.y, prev.y)};
		} else if (corner.y == prev.y) {
			hedges[corner.y] = coord_str{std::min(corner.x, prev.x), std::max(corner.x, prev.x)};
		}
		prev = corner;
	}

	coord_str ul, lr;
	ul = coord_str{min_coord_.x - 1, min_coord_.y - 1};
	lr = coord_str{max_coord_.x + 1, max_coord_.y + 1};
	outer_h_.clear();
	outer_v_.clear();
	int32_t curr_x = ul.x, curr_y = ul.y;

	for (int32_t y = ul.y; y <= lr.y; y++) {
		bool out = true;
		curr_x = ul.x;

		for (const auto& vedge : vedges) {
			if ((vedge.second.x < y) && (vedge.second.y > y)) { // vedge crosses y
				if (out) {
					outer_h_[y].emplace(coord_str{curr_x, vedge.first - 1});
				} else {
					curr_x = vedge.first + 1;
				}
				out = !out;
			} else if ((vedge.second.x == y) || (vedge.second.y == y)) { // vedge starts or ends at y
				if (out) {
					outer_h_[y].emplace(coord_str{curr_x, vedge.first - 1});
					out = false;
				} else {
					if (is_outer(coord_str{vedge.first + 1, y - 1})) {
						curr_x = vedge.first + 1;
						out = true;
					} else {
						out = false;
					}
				}
			}
		}

		if (out) {
			outer_h_[y].emplace(coord_str{curr_x, lr.x});
		}
	}

	for (int32_t x = ul.x; x <= lr.x; x++) {
		bool out = true;
		curr_y = ul.y;

		for (const auto& hedge : hedges) {
			if ((hedge.second.x < x) && (hedge.second.y > x)) { // hedge crosses x
				if (out) {
					outer_v_[x].emplace(coord_str{curr_y, hedge.first - 1});
				} else {
					curr_y = hedge.first + 1;
				}
				out = !out;
			} else if ((hedge.second.x == x) || (hedge.second.y == x)) { // hedge starts or ends at x
				if (out) {
					outer_v_[x].emplace(coord_str{curr_y, hedge.first - 1});
					out = false;
				} else {
					if (is_outer(coord_str{hedge.first + 1, x - 1})) {
						curr_y = hedge.first + 1;
						out = true;
					} else {
						out = false;
					}
				}
			}
		}

		if (out) {
			outer_v_[x].emplace(coord_str{curr_y, lr.y});
		}
	}

	for (size_t i = 0; i < corners_.size() - 1; i++) {
		for (size_t j = i + 1; j < corners_.size(); j++) {
			size = (abs(static_cast<int64_t>(corners_[j].x) - static_cast<int64_t>(corners_[i].x)) + 1) *
				   (abs(static_cast<int64_t>(corners_[j].y) - static_cast<int64_t>(corners_[i].y)) + 1);
			if (size > result) {
				if (is_valid_rectangle(corners_[i], corners_[j])) {
					result = size;
				}
			}
		}
	}

	return result;
}

int32_t AoC2025_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2025_day09::get_aoc_year() {
	return 2025;
}

void AoC2025_day09::tests() {
	int64_t result;

	if (init({"7,1", "11,1", "11,7", "9,7", "9,5", "2,5", "2,3", "7,3"})) {
		result = find_largest_rectangle();			 // 50
		result = find_largest_red_green_rectangle(); // 24
	}
}

bool AoC2025_day09::part1() {
	int64_t result = 0;

	result = find_largest_rectangle();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day09::part2() {
	int64_t result = 0;

	result = find_largest_red_green_rectangle();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day09 day09;

	return day09.main_execution();
}
