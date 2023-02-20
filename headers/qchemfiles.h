#pragma once
#include "sharedstructures.h"
using namespace std;





class qchemRun {
public:
	int get(string); //get values from qchem file

    //void emulate(string outfile); //output gaussian-formatted file
	void print(string outfile); //print values in memory
	int freq_get(string infile); //gets only frequencies
	void arrange(string outfile); //these functions replicate the behavior of freq_arrange.f in alastor; get frequencies and arrange them in a collumn
	void to_alex(string outfile); //outputs a .xyz file readable by Alex's FGR code
	void basic_dushin(string outfile); //outputs a basic file readable by dushin-2022

	int getcount() { //these get functions allow for accessing member variables outside the class
		return this->count;
	}
	bool is_linear();
	vector<atom> getgeo() {
		return this->geometry;
	}
	double getscf() {
		return this->scf;
	}
	vector<double> getfreq() {
		return this->frequencies;
	}
	vector<nmode> getnmodes() {
		return this->nmodes;
	}
	double getvibtemp() {
		return this->vibtemp;
	}
	double getvibpres() {
		return this->vibpres;
	}
	void setgeo(vector<atom> geo) {
		this->geometry = geo;
		return;
	}
	void setscf(double scf) {
		this->scf = scf;
		return;
	}
	void setcount(int count) {
		this->count = count;
		return;
	}
private:

	double scf; //scf total energy
	vector<atom> geometry; //a list of all the atoms in the input file, each with a symbol and cartesian coordinates
	int count; //amount of atoms
	vector<double> frequencies;
	vector<nmode> nmodes; //normal modes
	double vibtemp; //temperature under vibrational analysis
	double vibpres; //pressure    "                         "
	bool linear; //is the molecule linear or not
	
};


int qchemRun::get(string infile) {
	
		ifstream ifile;
		ifile.open(infile);
		
		
	
		
		if (!ifile.is_open()) {
			cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
			return 1;
		}
		
	
		string num;
		string symbol;
		double x;
		double y;
		double z;
		double scf;
		string line;
		int i = 0;
		vector<double> energies;

		while (ifile.is_open()) {
	
			if (!ifile.is_open()) { //can never be too careful
				break;
			}

			getline(ifile, line);
			

			if (line.find("             Standard Nuclear Orientation (Angstroms)" ) != string::npos) { //locate the coordinates
				vector<atom> atoms; //declare a variable-length list of atoms
				getline(ifile, line);
				getline(ifile, line); //these two extra getlines are necessary to skip two lines past "Standard Nuclear Orientation"
				
				int count = 0;
				while (!ifile.eof()) { //in case something breaks, the program will still stop itself at the end of the file
			


					ifile >> num; //skips the number, we have it already by the list entry
				
					if (num == "----------------------------------------------------------------") { //since we don't need to store this number, it's read as a string. that way, if we reach the end of the input section and the first character of the line is '-', the program knows to stop
						break;
					}
					ifile >> symbol;
					ifile >> x;
					ifile >> y;
					ifile >> z; //get the symbol and coordinates
					atom instance; //declare an instance of an atom
					instance.symbol = symbol;
					instance.coords[0] = x;
					instance.coords[1] = y;
					instance.coords[2] = z; //add the data
					atoms.push_back(instance); //add the instance to the list, and repeat. despite the name "push_back" it appends to the back of the list
					count++;
					this->geometry = atoms; //store the list under the qchemRun object
					this->count = count;
					
				}
				
			}
			else if (line.find(" **  OPTIMIZATION CONVERGED  **") != string::npos) { //locate the coordinates
				vector<atom> atoms; //declare a variable-length list of atoms
				getline(ifile, line);
			
				getline(ifile, line);
			
				getline(ifile, line);
				
				getline(ifile, line);
				
				

				int count = 0;
				while (!ifile.eof()) { //in case something breaks, the program will still stop itself at the end of the file



					ifile >> num; //skips the number, we have it already by the list entry
					
					if (num == "----------------------------------------------------------------"|| num == "Z-matrix") { //since we don't need to store this number, it's read as a string. that way, if we reach the end of the input section and the first character of the line is '-', the program knows to stop
						break;
					}
					ifile >> symbol;
					ifile >> x;
					ifile >> y;
					ifile >> z; //get the symbol and coordinates
					atom instance; //declare an instance of an atom
					instance.symbol = symbol;
					instance.coords[0] = x;
					instance.coords[1] = y;
					instance.coords[2] = z; //add the data
					atoms.push_back(instance); //add the instance to the list, and repeat. despite the name "push_back" it appends to the back of the list
					count++;
					this->geometry = atoms; //store the list under the qchemRun object
					this->count = count;

				}

			}
			else if (line.find("Total energy in the final basis set") != string::npos) { //locate scf energy
				regex target("Total energy in the final basis set =");
				string replace = "";
				string newline = regex_replace(line, target, replace); //delete string, keep only the value
				scf = stod(newline);
				this->scf = scf;
				energies.push_back(scf);
			}
				
			else if (line.find(" Final energy is") != string::npos) { //locate scf energy
				regex target("Final energy is");
				string replace = "";
				string newline = regex_replace(line, target, replace); //delete string, keep only the value
				scf = stod(newline);
				this->scf = scf;
				energies.push_back(scf);
			}
			else if (line.find("  Energy is") != string::npos) { //locate scf energy
				regex target("  Energy is");
				string replace = "";
				string newline = regex_replace(line, target, replace); //delete string, keep only the value
				scf = stod(newline);
				this->scf = scf;
				energies.push_back(scf);
			}
			else if (line.find(" **        VIBRATIONAL FREQUENCIES (CM**-1) AND NORMAL MODES         **") != string::npos) { //get all data related to the normal modes
				vector<nmode> norms;
				string trash;
			
				for (int i = 0; i < 4; i++) {
					getline(ifile, trash);
			
				}
				int nmodes_num;

				if (this->is_linear()) { //calculates expected number of normal modes
					nmodes_num = 3 * this->count - 5;
			
					
				}
				else {
					nmodes_num = 3 * this->count - 6;
				
				}

				

				int blocks = div_ceil(nmodes_num, 3);
		
				int modes_in_final_block = nmodes_num % 3;


				for (int i = 0; i < blocks; i++) { 
					
					if (i == blocks - 1 && modes_in_final_block != 0) {
						break;
					}
					
					ifile >> trash;
					nmode mode_1; //because the data outputs vertically but c++ reads horizontally, the program has to collect data for three normal modes at once. this will result in some ugly code managing a lot of temp data before storing it
					nmode mode_2;
					nmode mode_3;
					
					
					ifile >> mode_1.mode;
				
					ifile >> mode_2.mode;
					ifile >> mode_3.mode;
					ifile >> trash; 
				
					ifile >> mode_1.freq;
				
					ifile >> mode_2.freq;
					ifile >> mode_3.freq;
					ifile >> trash;
					ifile >> trash;
				
					ifile >> mode_1.frc;
		
					ifile >> mode_2.frc;
					ifile >> mode_3.frc;
					ifile >> trash;
					ifile >> trash;
				
					ifile >> mode_1.redmass;

					ifile >> mode_2.redmass;
					ifile >> mode_3.redmass;
					

					for (int i = 0; i < 24 ; i++) { //change back to 24
						ifile >> trash;
						
					}

				
				
				
					
					for (int i = 0; i < this->count; i++) { 
						string instance_symbol;
						atom instance_1; //each instance represents an an atom in one particular mode; they will be stored under seperate nmode objects
						atom instance_2;
						atom instance_3;
						ifile >> instance_symbol >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2];
						
						ifile >> instance_2.coords[0] >> instance_2.coords[1] >> instance_2.coords[2];
						ifile >> instance_3.coords[0] >> instance_3.coords[1] >> instance_3.coords[2];
						instance_1.symbol, instance_2.symbol, instance_3.symbol = instance_symbol;
						mode_1.atoms.push_back(instance_1); //adds atom geometry to nmode object
						mode_2.atoms.push_back(instance_2);
						mode_3.atoms.push_back(instance_3);
					}
					getline(ifile, trash);
					
					getline(ifile, trash); //skip TransDip, two getlines used to eat line buffer
					

					norms.push_back(mode_1);
					norms.push_back(mode_2);
					norms.push_back(mode_3); //adds all normal mode objects, containing frequencies, force constants, mode numbers, and mode geometries to list

				}
				
				ifile >> trash;
				if (modes_in_final_block == !0) {
					
					if (modes_in_final_block == 1) {
						
						nmode mode_1;
				
						ifile >> mode_1.mode;
						
						ifile >> trash;
			
						ifile >> mode_1.freq;
	
						ifile >> trash;
						ifile >> trash;
			
						ifile >> mode_1.frc;
			
						ifile >> trash;
						ifile >> trash;
				
						ifile >> mode_1.redmass;
					
						for (int i = 0; i < 18; i++) { 
							ifile >> trash;

						}
					
						for (int i = 0; i < this->count; i++) {
							string instance_symbol;
							atom instance_1;
							ifile >> instance_symbol >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2];
							instance_1.symbol = instance_symbol;
							mode_1.atoms.push_back(instance_1);
						}
						getline(ifile, trash);
						getline(ifile, trash); //skip TransDip, two getlines used to eat line buffer
						
						norms.push_back(mode_1);
						ifile >> trash;
					}
					else if (modes_in_final_block == 2) {
						
						nmode mode_1;
						nmode mode_2;
		
						ifile >> mode_1.mode;
				
						ifile >> mode_2.mode;
						ifile >> trash;
			
						ifile >> mode_1.freq;
			
						ifile >> mode_2.freq;
						ifile >> trash;
						ifile >> trash;
					
						ifile >> mode_1.frc;
			
						ifile >> mode_2.frc;
						ifile >> trash;
						ifile >> trash;
				
						ifile >> mode_1.redmass;

						ifile >> mode_2.redmass;
						for (int i = 0; i < 21; i++) {
							ifile >> trash;

						}
				
						for (int i = 0; i < this->count; i++) {
							string instance_symbol;
							atom instance_1;
							atom instance_2;
							ifile >> instance_symbol >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2];
							ifile >> instance_2.coords[0] >> instance_2.coords[1] >> instance_2.coords[2];
							instance_1.symbol, instance_2.symbol = instance_symbol;
							mode_1.atoms.push_back(instance_1);
							mode_2.atoms.push_back(instance_2);
						}
						getline(ifile, trash);
						getline(ifile, trash); //skip TransDip, two getlines used to eat line buffer

						norms.push_back(mode_1);
						norms.push_back(mode_2);
						ifile >> trash;
					}
				}


				this->nmodes = norms; //stores everything in norms list in master qchemRun object

				//the queued input stream should now look like this: "THERMODYNAMIC QUANTITIES AT   298.15 K  AND     1.00 ATM"

				
				ifile >> trash;
				ifile >> trash;
				ifile >> trash; //skip to temperature value
				
				
				ifile >> this->vibtemp;
			
				ifile >> trash;
				ifile >> trash; // skip to pressure value
			
				ifile >> this->vibpres;
			
				ifile >> trash;
				getline(ifile, line);
			
			}

			else if (line.find("Zero point vibrational energy:") != string::npos) { //get the lines with format "   Atom    1 Element X  Has Mass   Y (really we just need the mass at this point)
					string trash;
					for (int i = 0; i < this->count; i++) {
						ifile >> trash;
						ifile >> trash;
						ifile >> trash;
						ifile >> trash;
						ifile >> trash;
						ifile >> trash; //skip to mass
						ifile >> this->geometry.at(i).mass;
				
					}

			}
			if (ifile.eof()){
				break;
			}


		
		}

		cout << "Molecule is: ";
		for (int i = 0; i < this->count; i++) {
			cout << this->geometry.at(i).symbol;
		}
		cout << endl;
		cout.precision(18);
		cout << "SCF energy: ";
		for (int i = 0; i < energies.size() - 1; i++) {
			cout << energies.at(i) << " --> ";
		}
			
		cout << energies.at(energies.size()-1) << endl;
		
		if (this->is_linear()) { 
		
			cout << "Molecule is linear" << endl;

		}
		else {

			cout << "Molecule is nonlinear" << endl;
		}
		
		cout << endl;
		ifile.close();

		
	

		return 0;
}


void qchemRun::print(string outfile) {
	ofstream ofile;
	ofile.open(outfile, ios_base::app);
	ofile.precision(18);
	for (int i = 0; i < this->count; i++) {
		ofile << this->geometry.at(i).symbol << " " << this->geometry.at(i).coords[0] << " " << this->geometry.at(i).coords[1] << " " << this->geometry.at(i).coords[2] << endl;
	} //for each atom output the number, symbol, and coordinates
	  //this->geometry.at(i).thing means for the qchemRun object (which calls the function), check its list of atoms (geometry) for the value at the index i. since this value is a struct, we check the struct for either the symbol or the coordinates, which are in their own list.
	ofile << "SCF: " << this->scf << endl; //output the scf energy
	ofile << "---------------------------------------------------" << endl;
	
	ofile.close();
	return;
}

void qchemRun::to_alex(string outfile) {
	vector<atom> geo = this->geometry;
	
	ofstream ofile;
	ofile.open(outfile);
	ofile << int(geo.size()) << endl;
	for (int i = 0; i < geo.size(); i++) {
		string symbol = geo.at(i).symbol;
		if (symbol.size() < 2) {
			ofile << symbol << "    ";
		}
		else {
			ofile << symbol << "   ";
		}


		ofile << fixed;
		ofile.precision(18);
		double coord1 = geo.at(i).coords[0];
		if (coord1 < 0) {
			ofile << "   " << coord1;
		}
		else {
			ofile << "    " << coord1;
		}
		for (int j = 1; j < 3; j++) {
			ofile << fixed;
			ofile.precision(18);
			double coord = geo.at(i).coords[j];
			if (coord < 0) {
				ofile << "   " << coord;
			}
			else {
				ofile << "    " << coord;
			}

		}

		ofile << endl;
	}




	return;

}

//void qchemRun::emulate(string outfile) {
//	ofstream ofile(outfile);
//	ofile << " Entering Gaussian System, Link 0 = g09" << endl;
//	ofile << endl;
//	ofile << endl;
//	ofile << endl;
//	
//	ofile << " Center     Atomic      Atomic             Coordinates (Angstroms)" << endl; //copy the required headers
//	ofile << endl;
//	ofile << endl;
//
//	for (int i = 0; i < this->count; i++) { //output all the data, formatted to look like a gaussian file
//		int atonum = atomic_symbol_to_number(this->geometry.at(i).symbol);
//		if (i + 1 >= 10) {
//			ofile << "      " << i + 1;
//		}
//		else {
//			ofile << "       " << i + 1;
//		}
//		if (atonum < 10) {
//			ofile << "         " << atonum << "          0";
//		}
//		else {
//			ofile << "          " << atonum << "          0";
//		}
//		ofile << fixed;
//		ofile.precision(6);
//		double coord1 = this->geometry.at(i).coords[0];
//		if (coord1 < 0) {
//			ofile << "        " << coord1;
//		}
//		else {
//			ofile << "         " << coord1;
//		}
//		for (int j = 1; j < 3; j++) {
//			ofile << fixed;
//			ofile.precision(6);
//			double coord = this->geometry.at(i).coords[j];
//			if (coord < 0) {
//				ofile << "    " << coord;
//			}
//			else {
//				ofile << "     " << coord;
//			}
//		}
//		ofile << endl;
//	}
//	
//	ofile.precision(9);
//	ofile << " SCF Done: =  " << this->scf << endl;
//	ofile.close();
//
//}

int qchemRun::freq_get(string infile) {
	ifstream ifile(infile);
	if (!ifile.is_open()) {
		cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
		return 1;
	}
	string line;
	while (getline(ifile, line)) {



	
		if (line.find("Mode:") != string::npos) { //checks if "Frequency:" is in the next line; if not it ignores it
			string trash;
			ifile >> trash;
			for (int i = 0; i < 3; i++) {
				double freq;
				ifile >> freq;
				this->frequencies.push_back(freq);
			}
			
		}
		if (ifile.eof()) {
			break;
		}
	}

	ifile.close();
	return 0;
}

void qchemRun::arrange(string outfile) {
	ofstream ofile(outfile);
	ofile << this->frequencies.size() << endl;
	for (int i = 0; i < this->frequencies.size(); i++) {
		ofile << this->frequencies.at(i) << endl;
	}
	return;
}

bool qchemRun::is_linear() {
	vector<atom> geometry = this->geometry;
	bool x_0 = false;
	bool y_0 = false;
	bool z_0 = false;
	for (int i = 0; i < 3; i++) {
		bool nonzero = false;
		for (int j = 0; j < this->count; j++) { //this loops over first the X coordinates of every atom, then the Y coordinates, and then Z. for each set, it checks until it finds a nonzero coordinate (within 10^-5 units of precision). if it finds one, it breaks.
			                                    //if it confirms all the coordinates of that list are zero, it sets the variable to true. if not, the variable stays false.

	
			if (abs(geometry.at(j).coords[i]) > 0.000009) {
				nonzero = true;
				break;
			}
			
		}
		if (nonzero) {
			continue;
		}
		switch (i) {
		case 0:

			x_0 = true;
			break;
		case 1:

			y_0 = true;
			break;
		case 2:

			z_0 = true;
			break;
		}
	}

	bool x_and_y = x_0 && y_0;  //for a molecule to be linear, it must be possible to describe it with only one set of coordinates. therefore, it must be the case that two of the coordinates are 0 across the entire molecule.
	bool x_and_z = x_0 && z_0;  //this sets variables for each combination of possibilities
	bool y_and_z = y_0 && z_0;
	this->linear = x_and_y || x_and_z || y_and_z;
	return x_and_y||x_and_z||y_and_z; //if any two of the coordinate lists are all zeroes, it returns true, aka "the molecule is linear". if none are, it returns false, aka "the molecule is nonlinear".
}

void qchemRun::basic_dushin(string outfile) {
	ofstream ofile;
	ofile.open(outfile);

	ofile << "E(DFT) = " << this->scf << endl;
	for (int i = 0; i < this->count; i++) {
		ofile << atomic_symbol_to_number(this->geometry.at(i).symbol) << " " << this->geometry.at(i).coords[0] << " " << this->geometry.at(i).coords[1] << " " << this->geometry.at(i).coords[2] << endl;
	}
	ofile.close();
}
