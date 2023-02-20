#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>


using namespace std;






struct atom {

	double coords[3]; //cartesian coordinates of atom
	string symbol; //atomic symbol
	double mass;
};


struct nmode { //normal modes

	int mode; //mode number
	double freq; //frequency
	double frc; //force constant
	double redmass; //reduced mass
	double ir = 0; //IR intensities, need to find a way to get these from qchem
	vector<atom> atoms; //geometries


};



struct qsystem {
	vector<atom> atoms;
	int charge;
	int multiplicity;
};

string atosymbols[118] = { "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Rb", "Sr", "Y", "Zr", "Ga", "Ge", "As", "Se", "Br", "Kr", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og" };


int atomic_symbol_to_number(string symbol) { //converts from the atomic symbol to the atomic number

	for (int i = 0; i < 118; i++) {
		if (atosymbols[i] == symbol) {
			return i + 1;
		}
	}
	return 0;
}

int div_ceil(int x, int y) { //integer division by default returns the floor (16/3 = 5). for some indexing purposes, it will help to have access to a version that instead returns the ceiling (16/3 = 6)
	int q = (x / y) + (x % y != 0); //solves the floor and adds 1 if the remainder is nonzero (the second statement is a bool conditional that evaluates to 0 or 1)
	return q;
}

void print_align(ofstream& os, double x) { //allows for printing to file in a way that aligns positive and negative numbers by digit, by adding a pad space before positive numbers. needs tuning

	if (x < 0) {
		os << x;
	}

	else {
		os << " " << x;
	}

	return;
}

struct hrf { //dushin hrfs
	int mode;
	double freq;
	double projection;
	double energy;
	double hrf;
};

class hrf_output {
public:
private:

};

int supplem_to_alex(string infile, string outfile) {
	ifstream ifile;
	ifile.open(infile);

	if (!ifile.is_open()) {
		cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
		return 1;
	}
	std::cout << "Does the file contain symmetries?: ";
	char ans;
	std::cin >> ans;
	char input = tolower(ans);
	while (std::cin.get() != '\n'); //clears input stream if user inputs too many characters
	string line;
	vector<hrf> output;
	while (ifile.is_open()) {
		getline(ifile, line);

		string mode;
		if (line.find(" Mode   Freq   Projn Energy") != string::npos) {
			while (mode != "Reorganization") {
				double freq;
				double proj;
				double energy;
				double huang;

				ifile >> mode;
				if (mode == "Reorganization") {
					break;
				}
				if (input == 'y') {
					string sym;
					ifile >> sym;
				}
				ifile >> freq >> proj >> energy;
				huang = 0.5 * (proj * proj);
				hrf hrf_data;
				hrf_data.mode = stoi(mode);
				hrf_data.energy = energy;
				hrf_data.freq = freq;
				hrf_data.projection = proj;
				hrf_data.hrf = huang;
				output.push_back(hrf_data);
			}
		}
		if (mode == "Reorganization") {
			break;
		}
	}
	ifile.close();
	ofstream ofile;
	ofile.open(outfile);
	for (int i = 0; i < output.size(); i++) {
		ofile.precision(14);
		ofile << output.at(i).freq << "        ";
		ofile.scientific;
		ofile << output.at(i).hrf << endl;
	}
	ofile.close();
	return 0;
}