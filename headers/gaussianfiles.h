#pragma once
#include "sharedstructures.h"

struct abesubgroup { //Abelian subgroup
	string group;
	int operations;
};



class gaussRun {
public:
	int get(string infile);
	void print(string outfile);
	int freq_get(string infile);
	void arrange(string outfile);
	void to_alex(string infile, string outfile);
	bool is_linear();
	void basic_dushin(string outfile);

	int getcount() { //these get functions allow for accessing member variables outside the class
		return this->geometry.size();
	}

	string getscf() {
		return (this->totenergy);
	}
	vector<double> getfreq() {
		return this->frequencies;
	}

	vector<atom> getgeo() {
		return this->geometry;
	}
	
	void setgeo(vector<atom> geo) {
		this->geometry = geo;
		return;
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

	void setscf(double scf) {
		this->totenergy = scf;
		return;
	}
	void setcount(int count) {
		this->count = count;
		return;
	}

private:
	qsystem input; //user input geometry
	int dof; //degrees of freedom
	abesubgroup largestabe; //Largest Abelian subgroup
	vector<atom> geometry; //output geometry
	string totenergy; //total energy. using string to avoid trying to parse the line for now, it will be messy
	vector<double> frequencies;
	int count; //amount of atoms
	vector<nmode> nmodes; //normal modes
	bool linear;
	double vibtemp;
	double vibpres;

};



int gaussRun::get(string infile) {
	ifstream ifile;
	
	ifile.open(infile);
	if (!ifile.is_open()) {
		cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
		return 1;
	}
	vector<atom> geometry;
	while (ifile.is_open()) {
		string line;
		getline(ifile, line); //goes down the file line by line. since no output is given until the end, will only give the newest information about the system (e.g., if it calculated geometry more than once, the program only shows the final calculation)

		if (line.find(" Symbolic Z-matrix:") != string::npos) { //gets user input, to sanity check
			cout << "Getting user input..." << endl;
			string trash;
			ifile >> trash;
			ifile >> trash; //skips "Charge ="
			int charge;
			
			ifile >> charge;
			

			ifile >> trash;
			ifile >> trash; //skips "Multiplicity ="
			int multiplicity;
			ifile >> multiplicity;
			
			input.charge = charge;
			input.multiplicity = multiplicity;
			string instance;
			double coord;
			ifile >> instance;
			
			while (instance != "NAtoms=" && instance != "Add" && instance.find("Grad") == string::npos && instance != "\n" && instance != "Stoichiometry") { //stops at the end of the coordinate data
				atom ainstance;
				
				ainstance.symbol = instance;

				ifile >> coord;
				ainstance.coords[0] = coord;
				ifile >> coord;
				ainstance.coords[1] = coord;
				ifile >> coord;
				ainstance.coords[2] = coord; //gets symbol and coords
				this->input.atoms.push_back(ainstance);
				ifile >> instance;
			}
			this->count = input.atoms.size();
		}
		if (line.find("Standard orientation:") != string::npos) { //gets calculated geometry
			cout << "Getting calculated geometry..." << endl;
			string trash;
			getline(ifile, trash);
			getline(ifile, trash);
			getline(ifile, trash);
			getline(ifile, trash); //skips header



			int instance;
			int atonum;
			int atotype;
			double coord;

			for (int i = 0; i < this->input.atoms.size(); i++) { //for every atom in the input, get an output atom. assumes no atoms have been gained or lost
				atom ainstance;

				ifile >> instance;
				ifile >> atonum;

				//TODO: examine why it returned As where Br was expected
				ainstance.symbol = atosymbols[atonum - 1]; //gets symbol from atomic number

				ifile >> atotype;


				ifile >> coord;
				ainstance.coords[0] = coord;
				ifile >> coord;
				ainstance.coords[1] = coord;
				ifile >> coord;
				ainstance.coords[2] = coord; //gets symbol and coords
				geometry.push_back(ainstance);

			}
		
		this->geometry = geometry;
		
		}
		if (line.find(" Framework group") != string::npos) { //gets degrees of freedom by anticipating the correct line
			cout << "Getting degrees of freedom..." << endl;
			string trash;
			ifile >> trash;
			ifile >> trash;
			ifile >> trash; //skips label
			int dof;
			ifile >> dof;
			this->dof = dof; //saves degrees of freedom
		}
		if (line.find(" Deg.of freedom") != string::npos) { //same method for largest Abelian subgroup
			cout << "Getting largest Abelian subgroup..." << endl;
			string trash;
			ifile >> trash;
			ifile >> trash;
			ifile >> trash; //skips label
			string group;
			int operations;
			ifile >> group;
			ifile >> trash;
			ifile >> operations; //gets group and operations, skips NOp (not sure what that is). a new variable can be added to the abesubgroup struct if that information is desired
			this->largestabe.group = group;
			this->largestabe.operations = operations;
			

		}
		
		

		if (line.find(" SCF Done:") != string::npos) { //same method for SCF total energy
			cout << "Getting SCF energy..." << endl;
			this->totenergy = line;
		}
		if (ifile.eof()) {
			break;
		}
		else if (line.find(" and normal coordinates:") != string::npos) { //get all data related to the normal modes
			vector<nmode> norms;
			string trash;
			;
			//for (int i = 0; i < 2; i++) {
				//getline(ifile, trash);
				//cout << trash << endl;

			//}
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
				string trash;
				if (i == blocks - 1 && modes_in_final_block != 0) {
					break;
				}

				
				
				nmode mode_1; //because the data outputs vertically but c++ reads horizontally, the program has to collect data for three normal modes at once. this will result in some ugly code managing a lot of temp data before storing it
				nmode mode_2;
				nmode mode_3;


				ifile >> mode_1.mode;
				
				ifile >> mode_2.mode;
				ifile >> mode_3.mode;
				
				ifile >> trash;
				ifile >> trash;
                                ifile >> trash;
                                ifile >> trash;
                                ifile >> trash;		 //skip symmetries and go to freq value
				
				ifile >> mode_1.freq;
				ifile >> mode_2.freq;
				ifile >> mode_3.freq;
				
				ifile >> trash;
				ifile >> trash;
				ifile >> trash; //skip to redmass value
				
				ifile >> mode_1.redmass;

				ifile >> mode_2.redmass;
				ifile >> mode_3.redmass;
				ifile >> trash >> trash >> trash; //skip to force constant value

				ifile >> mode_1.frc;

				ifile >> mode_2.frc;
				ifile >> mode_3.frc;
				
				for (int j = 0; j < 17; j++) { //skip IR intensities and go to mode geometries
					ifile >> trash;
					

				}
				
				for (int j = 0; j < this->count; j++) {
					string instance_symbol;
					int atonum;
					atom instance_1; //each instance represents an an atom in one particular mode; they will be stored under seperate nmode objects
					atom instance_2;
					atom instance_3;
					ifile >> trash >> atonum >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2]; //gaussian prints atomic index that needs skipped while qchem does not
					
					ifile >> instance_2.coords[0] >> instance_2.coords[1] >> instance_2.coords[2];
					ifile >> instance_3.coords[0] >> instance_3.coords[1] >> instance_3.coords[2];
					instance_symbol = atosymbols[atonum+1];
					instance_1.symbol, instance_2.symbol, instance_3.symbol = instance_symbol;
					mode_1.atoms.push_back(instance_1); //adds atom geometry to nmode object
					mode_2.atoms.push_back(instance_2);
					mode_3.atoms.push_back(instance_3);
				}
				getline(ifile, trash); //eat line buffer and clear input stream at end of geometry loop

				norms.push_back(mode_1);
				norms.push_back(mode_2);
				norms.push_back(mode_3); //adds all normal mode objects, containing frequencies, force constants, mode numbers, and mode geometries to list

			}

			ifile >> trash;
			if (modes_in_final_block == !0) {

				if (modes_in_final_block == 1) {

					nmode mode_1;

					ifile >> mode_1.mode;

					ifile >> trash >> trash >> trash;

					ifile >> mode_1.freq;

					ifile >> trash;
					ifile >> trash;
					ifile >> trash;

					ifile >> mode_1.redmass;
					ifile >> trash >> trash >> trash;

					ifile >> mode_1.frc;

					for (int i = 0; i < 9; i++) {
						ifile >> trash;

					}

					for (int i = 0; i < this->count; i++) {
						string instance_symbol;
						atom instance_1;
						ifile >> trash >> instance_symbol >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2];
						instance_1.symbol = instance_symbol;
						mode_1.atoms.push_back(instance_1);
					}
					
					getline(ifile, trash); //eat line buffer

					norms.push_back(mode_1);
					ifile >> trash;
				}
				else if (modes_in_final_block == 2) {

					nmode mode_1;
					nmode mode_2;

					ifile >> mode_1.mode >> mode_2.mode;

					ifile >> trash >> trash >> trash;

					ifile >> mode_1.freq >> mode_2.freq;

					ifile >> trash;
					ifile >> trash;
					ifile >> trash;

					ifile >> mode_1.redmass >> mode_2.redmass;
					ifile >> trash >> trash >> trash;

					ifile >> mode_1.frc >> mode_2.frc;

					for (int i = 0; i < 13; i++) {
						ifile >> trash;

					}

					for (int i = 0; i < this->count; i++) {
						string instance_symbol;
						atom instance_1;
						atom instance_2;
						ifile >> trash >> instance_symbol >> instance_1.coords[0] >> instance_1.coords[1] >> instance_1.coords[2];
						ifile >> trash >> instance_2.coords[0] >> instance_2.coords[1] >> instance_2.coords[2];
						instance_1.symbol, instance_2.symbol = instance_symbol;
						mode_1.atoms.push_back(instance_1);
						mode_2.atoms.push_back(instance_2);
					}
					
					getline(ifile, trash); //eat line buffer

					norms.push_back(mode_1);
					norms.push_back(mode_2);
					ifile >> trash;
				}
			}


			this->nmodes = norms; //stores everything in norms list in master gaussRun object

			//the queued input stream should now look like this: "- Thermochemistry -"
			


			ifile >> trash;
			ifile >> trash;
			ifile >> trash;
			ifile >> trash; 
			ifile >> trash; //skip to temperature value


			ifile >> this->vibtemp;

			ifile >> trash;
			ifile >> trash; // skip to pressure value

			ifile >> this->vibpres;

			ifile >> trash;
			getline(ifile, line); //eat line buffer
			for (int i = 0; i < this->count; i++) {
				ifile >> trash;
				ifile >> trash;
				ifile >> trash;
				ifile >> trash;
				ifile >> trash;
				ifile >> trash;
				ifile >> trash; //skip to mass
				ifile >> trash;
				ifile >> this->geometry.at(i).mass;

			}


		}
	}
	cout << "Molecule is: ";
	for (int i = 0; i < this->count; i++) {
		cout << this->geometry.at(i).symbol;
	}
	cout << endl;
	cout.precision(18);
	cout << "SCF energy: " << this->totenergy << endl;

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

void gaussRun::print(string outfile) {
	ofstream ofile(outfile);

	ofile << "Input:" << endl;
	ofile << "Charge: " << this->input.charge << "   Multiplicity: " << this->input.multiplicity << endl;
	ofile << "-------------------------------------------------------" << endl;
	for (int i = 0; i < this->input.atoms.size(); i++) {
		string symbol = this->input.atoms.at(i).symbol;
		if (symbol.size() < 2) {
			ofile << symbol << "    ";
		}
		else {
			ofile << symbol << "   ";
		}


		ofile << fixed;
		ofile.precision(18);
		double coord1 = this->input.atoms.at(i).coords[0];
		if (coord1 < 0) {
			ofile << "   " << coord1;
		}
		else {
			ofile << "    " << coord1;
		}
		for (int j = 1; j < 3; j++) {
			ofile << fixed;
			ofile.precision(18);
			double coord = this->input.atoms.at(i).coords[j];
			if (coord < 0) {
				ofile << "   " << coord;
			}
			else {
				ofile << "    " << coord;
			}

		}

		ofile << endl;
	}
	ofile << "-------------------------------------------------------" << endl;
	ofile << "Degrees of freedom: " << this->dof << endl;
	ofile << "Largest Abelian Subgroup: " << this->largestabe.group << endl;
	ofile << "Subgroup operations: " << this->largestabe.operations << endl;
	ofile << this->totenergy << endl;
	ofile << "Calculated Geometry:" << endl;
	ofile << "-------------------------------------------------------" << endl;
	for (int i = 0; i < this->input.atoms.size(); i++) {
		string symbol = this->geometry.at(i).symbol;
		if (symbol.size() < 2) {
			ofile << symbol << "    ";
		}
		else {
			ofile << symbol << "   ";
		}


		ofile << fixed;
		ofile.precision(18);
		double coord1 = this->geometry.at(i).coords[0];
		if (coord1 < 0) {
			ofile << "   " << coord1;
		}
		else {
			ofile << "    " << coord1;
		}
		for (int j = 1; j < 3; j++) {
			ofile << fixed;
			ofile.precision(18);
			double coord = this->geometry.at(i).coords[j];
			if (coord < 0) {
				ofile << "   " << coord;
			}
			else {
				ofile << "    " << coord;
			}

		}

		ofile << endl;
	}
	ofile << "-------------------------------------------------------" << endl;
	return;
}

int gaussRun::freq_get(string infile) {
	ifstream ifile(infile);
	if (!ifile.is_open()) {
		cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
		return 1;
	}
	string line;
	while (getline(ifile, line)) {
		
	

		if (line.find("A                      A                      A") != string::npos) { //checks if "Frequencies --" is in the next line; if not it ignores it
			string trash;
			ifile >> trash;
			for (int i = 0; i < 3; i++) {
				double freq;
				ifile >> freq;
				this->frequencies.push_back(freq);
			}


		}
		//if (line.find("Frequency:") != string::npos) { //checks if "Frequency:" is in the line; if not it ignores it
		//	regex target("Frequency:");
		//	string replace = "";
		//	string newline = regex_replace(line, target, replace); //deletes "Frequency:" and outputs the line to a temporary file
		//	ofile << newline << endl;
		
		if (ifile.eof()) {
			break;
		}
	}

	ifile.close();
	return 0;
}

void gaussRun::arrange(string outfile) {
	ofstream ofile(outfile);
	for (int i = 0; i < this->frequencies.size(); i++) {
		ofile << this->frequencies.at(i) << endl;
	}

}

void gaussRun::to_alex(string infile, string outfile){
	vector<atom> geo;
	ifstream ifile;
	ifile.open(infile);


	if (!ifile.is_open()) {
		cout << "Problem opening file (check that the name is correct and it is in the correct directory)." << endl;
		return;
	}
	int instance;
	int atonum;
	int atotype;
	double coord;
	for (int i = 0; i < 66; i++) { //for every atom in the input, get an output atom. assumes no atoms have been gained or lost
		atom ainstance;

		ifile >> instance;
		cout << "instance: " << instance << endl;;
		ifile >> atonum;
		cout << "atonum: " << atonum << endl;

		//TODO: examine why it returned As where Br was expected
		ainstance.symbol = atosymbols[atonum - 1]; //gets symbol from atomic number

		ifile >> atotype;
		cout << atotype;

		ifile >> coord;
		cout << coord;
		ainstance.coords[0] = coord;
		ifile >> coord;
		ainstance.coords[1] = coord;
		ifile >> coord;
		ainstance.coords[2] = coord; //gets symbol and coords
		geo.push_back(ainstance);

	}


	ofstream ofile;
	ofile.open(outfile);
	ofile << geo.size() << endl;
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

bool gaussRun::is_linear() {
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
	return x_and_y || x_and_z || y_and_z; //if any two of the coordinate lists are all zeroes, it returns true, aka "the molecule is linear". if none are, it returns false, aka "the molecule is nonlinear".
}

void gaussRun::basic_dushin(string outfile) {
	ofstream ofile;
	ofile.open(outfile);

	ofile << "E(DFT) = " << this->totenergy << endl;
	for (int i = 0; i < this->count; i++) {
		ofile << atomic_symbol_to_number(this->geometry.at(i).symbol) << " " << this->geometry.at(i).coords[0] << " " << this->geometry.at(i).coords[1] << " " << this->geometry.at(i).coords[2] << endl;
	}
	ofile.close();
}
