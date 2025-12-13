#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>
#include <sstream>

const int64_t C_CONNECTIONS_TEST = 10;
const int64_t C_CONNECTIONS_REAL = 1000;

class AoC2025_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_lagest_circuits_size(const int32_t connections_required);
	int64_t get_cable_extension_lenghth();
	void calculate_distances();
	std::vector<coord_3d_str> boxes_;
	std::map<size_t, std::map<size_t, int64_t>> distances_;
	std::map<int64_t, std::set<std::pair<size_t, size_t>>> distance_map_;
};

bool AoC2025_day08::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	int32_t x, y, z;

	boxes_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);
		ss >> x;
		;
		ss.ignore(1);
		ss >> y;
		ss.ignore(1);
		ss >> z;

		boxes_.push_back({x, y, z});
	}

	return true;
}

void AoC2025_day08::calculate_distances() {
	int64_t distance;

	distances_.clear();
	distance_map_.clear();

	for (size_t i = 0; i < boxes_.size() - 1; i++) {
		for (size_t j = i + 1; j < boxes_.size(); j++) {
			distance = boxes_[i].euclidean_square_distance(boxes_[j]);
			distances_[i][j] = distance;
			distance_map_[distance].insert({i, j});
		}
	}
}

int64_t AoC2025_day08::get_lagest_circuits_size(const int32_t connections_required) {
	int64_t result = 0;
	int32_t connections_count = 0, circuit1, circuit2;

	std::vector<std::set<size_t>> circuits;

	circuits.clear();
	calculate_distances();

	for (const auto& [distance, box_pairs] : distance_map_) {
		for (const auto& box_pair : box_pairs) {
			circuit1 = circuit2 = -1;

			for (int64_t i = 0; i < static_cast<int64_t>(circuits.size()); i++) {
				if (circuits[i].count(box_pair.first)) {
					circuit1 = static_cast<int32_t>(i);
					break;
				}
			}

			for (int64_t i = 0; i < static_cast<int64_t>(circuits.size()); i++) {
				if (circuits[i].count(box_pair.second)) {
					circuit2 = static_cast<int32_t>(i);
					break;
				}
			}

			if (circuit1 < 0 && circuit2 < 0) {
				// neither box is in a circuit
				circuits.push_back({box_pair.first, box_pair.second});
			} else if (circuit1 >= 0 && circuit2 >= 0 && circuit1 != circuit2) {
				// both boxes are in different circuits, merge them
				circuits[circuit1].insert(circuits[circuit2].begin(), circuits[circuit2].end());
				circuits.erase(circuits.begin() + circuit2);
			} else if (circuit1 >= 0 && circuit2 >= 0 && circuit1 == circuit2) {
				// both boxes are already in the same circuit
			} else if (circuit1 >= 0) {
				// box1 is in a circuit, box2 is not
				circuits[circuit1].insert(box_pair.second);
			} else if (circuit2 >= 0) {
				// box2 is in a circuit, box1 is not
				circuits[circuit2].insert(box_pair.first);
			}

			connections_count++;
			if (connections_count >= connections_required) {
				break;
			}
		}

		if (connections_count >= connections_required) {
			break;
		}
	}

	std::vector<int32_t> circuit_sizes = {0, 0, 0};

	for (const auto& circuit : circuits) {
		if (static_cast<int32_t>(circuit.size()) > circuit_sizes[0]) {
			circuit_sizes[2] = circuit_sizes[1];
			circuit_sizes[1] = circuit_sizes[0];
			circuit_sizes[0] = static_cast<int32_t>(circuit.size());
		} else if (static_cast<int32_t>(circuit.size()) > circuit_sizes[1]) {
			circuit_sizes[2] = circuit_sizes[1];
			circuit_sizes[1] = static_cast<int32_t>(circuit.size());
		} else if (static_cast<int32_t>(circuit.size()) > circuit_sizes[2]) {
			circuit_sizes[2] = static_cast<int32_t>(circuit.size());
		}
	}

	result = static_cast<int64_t>(circuit_sizes[0]) * static_cast<int64_t>(circuit_sizes[1]) * static_cast<int64_t>(circuit_sizes[2]);

	return result;
}

int64_t AoC2025_day08::get_cable_extension_lenghth() {
	int64_t result = 0;
	int32_t circuit1, circuit2;

	std::vector<std::set<size_t>> circuits;

	circuits.clear();

	for (const auto& [distance, box_pairs] : distance_map_) {
		for (const auto& box_pair : box_pairs) {
			circuit1 = circuit2 = -1;

			for (int64_t i = 0; i < static_cast<int64_t>(circuits.size()); i++) {
				if (circuits[i].count(box_pair.first)) {
					circuit1 = static_cast<int32_t>(i);
					break;
				}
			}

			for (int64_t i = 0; i < static_cast<int64_t>(circuits.size()); i++) {
				if (circuits[i].count(box_pair.second)) {
					circuit2 = static_cast<int32_t>(i);
					break;
				}
			}

			if (circuit1 < 0 && circuit2 < 0) {
				// neither box is in a circuit
				circuits.push_back({box_pair.first, box_pair.second});
			} else if (circuit1 >= 0 && circuit2 >= 0 && circuit1 != circuit2) {
				// both boxes are in different circuits, merge them
				circuits[circuit1].insert(circuits[circuit2].begin(), circuits[circuit2].end());
				circuits.erase(circuits.begin() + circuit2);
			} else if (circuit1 >= 0 && circuit2 >= 0 && circuit1 == circuit2) {
				// both boxes are already in the same circuit
			} else if (circuit1 >= 0) {
				// box1 is in a circuit, box2 is not
				circuits[circuit1].insert(box_pair.second);
			} else if (circuit2 >= 0) {
				// box2 is in a circuit, box1 is not
				circuits[circuit2].insert(box_pair.first);
			}

			if ((circuits.size() == 1) && (circuits[0].size() == boxes_.size())) {
				// all boxes are connected
				return (static_cast<int64_t>(boxes_[box_pair.first].x) * static_cast<int64_t>(boxes_[box_pair.second].x));
			}
		}
	}

	return result;
}

int32_t AoC2025_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2025_day08::get_aoc_year() {
	return 2025;
}

void AoC2025_day08::tests() {
	int64_t result;

	if (init({"162,817,812", "57,618,57",	"906,360,560", "592,479,940", "352,342,300", "466,668,158", "542,29,236",
			  "431,825,988", "739,650,466", "52,470,668",  "216,146,977", "819,987,18",	 "117,168,530", "805,96,715",
			  "346,949,466", "970,615,88",	"941,993,340", "862,61,35",	  "984,92,344",	 "425,690,689"})) {
		result = get_lagest_circuits_size(C_CONNECTIONS_TEST); // 40
		result = get_cable_extension_lenghth();				   // 25272
	}
}

bool AoC2025_day08::part1() {
	int64_t result = 0;

	result = get_lagest_circuits_size(C_CONNECTIONS_REAL);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2025_day08::part2() {
	int64_t result = 0;

	result = get_cable_extension_lenghth();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2025_day08 day08;

	return day08.main_execution();
}
