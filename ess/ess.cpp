// ess.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include<cmath>
#include <algorithm>
#include <numeric>



struct Bin
{ 
	double mins[6];
	double maxs[6];
	double averages[6];
	double val;
	double relerr;
};

std::istream & operator>>(std::istream& str, Bin& bin)
{
	//zmin,zmax, thetamin,thetamax, lambdamin,lambdamax, timemin,timemax, wxmin,wxmax, wzmin,wzmax, val,relerr
	std::string temp;
	int i = 0;

	std::getline(str, temp, ',');
	if (temp.empty()) {
		return str;
	}


	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;
	

	i++;
	std::getline(str, temp, ',');
	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;

	i++;
	std::getline(str, temp, ',');
	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;

	i++;
	std::getline(str, temp, ',');
	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;

	i++;
	std::getline(str, temp, ',');
	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;

	i++;
	std::getline(str, temp, ',');
	bin.mins[i] = std::stod(temp);
	std::getline(str, temp, ',');
	bin.maxs[i] = std::stod(temp);
	bin.averages[i] = (bin.mins[i] + bin.maxs[i]) / 2;

	std::getline(str, temp, ',');
	bin.val = std::stod(temp);
	std::getline(str, temp);
	bin.relerr = std::stod(temp);
	return str;
}

std::ostream& operator<<(std::ostream& os, Bin& bin) {

	std::string base = "[";

	for (int i = 0; i < 6; i++) {
		base += std::to_string(bin.averages[i]);
		base += ",";
	}

	return os << base << "] Val: " << bin.val << " \n";
}

class Predicter
{
public:
	void init(std::string path) 
	{
		std::ifstream file(path);

		Bin b;
		//CSVRow row;
		std::string headers;
		std::getline(file, headers);
		while (file >> b)
		{
			rows.push_back(b);
		}
	}
	//zmin,zmax, thetamin,thetamax, lambdamin,lambdamax, timemin,timemax, wxmin,wxmax, wzmin,wzmax, val,relerr
	double predict(std::vector<double> parameters) {

		std::vector<std::pair<double, Bin>> bins_with_distances;

		if (parameters.size() != 6) {
			std::cout << "Could not predict since not exactly 6 params were given";
			return -1;
		}

		// Calculate distances
		for (int i = 0; i < rows.size(); i++) {

			double temp_dist = calculateDistance(rows[i], parameters);
			bins_with_distances.push_back(std::make_pair(temp_dist, rows[i]));
		};

		std::sort(bins_with_distances.begin(), bins_with_distances.end(), [](auto &left, auto &right) {
			return left.first < right.first;
		});

		// here I use 4 entries and take the last one, substract all and then divide by the first one (should be the largest), the last one will be 0 so we have 3 results
		const int n_bins = 4;
		double selected_distances[n_bins] = { 0 };
		double first_distance = bins_with_distances[n_bins].first - bins_with_distances[0].first;
		double dist_sum = 0;
		double weight_sum = 0;

		for (std::size_t i = 0; i < n_bins + 1; i++) {
			// Calculate relative dist to nth element
			double relative_dist = bins_with_distances[n_bins].first - bins_with_distances[i].first;
			// Divide by first distance to value lower distance
			double weight = relative_dist / first_distance;
			// Calculate weighted sum
			dist_sum += weight * bins_with_distances[i].second.val;
			weight_sum += weight;
			std::cout << bins_with_distances[i].second.val << "\n";
		}

		// Return divided sum
		return dist_sum / weight_sum;
	}
private:
	std::vector<Bin> rows;

	double calculateDistance(Bin b, std::vector<double> v2) {

		double dist = 0;

		for (int i = 0; i < v2.size(); i++) {
			dist += pow(b.averages[i] - v2[i], 2.0);
		}

		return sqrt(dist);
	}
};


int main()
{
	Predicter p;
	p.init("../W6-reduced.csv");
	std::cout << "INIT DONE\n";
	std::vector<double> d = { 10,80,50,0,0,0 };
	std::cout << p.predict(d) << "\n";
}
