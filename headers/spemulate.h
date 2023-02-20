#pragma once
#include "qchemfiles.h"
#include "gaussianfiles.h"

class sp : public qchemRun {
public:
	void emulate(string outfile) {
		ofstream ofile(outfile);
		ofile << " Entering Gaussian System, Link 0 = g09" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << "#P wB97XD/6-311++G(d,p) freq formcheck" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << endl;
		ofile << endl;

		ofile << " Center     Atomic      Atomic             Coordinates (Angstroms)" << endl; //copy the required headers
		ofile << endl;
		ofile << endl;
		
		for (int i = 0; i < this->getcount(); i++) { //output all the data, formatted to look like a gaussian file
			int atonum = atomic_symbol_to_number(this->getgeo().at(i).symbol);
			if (i + 1 >= 10) {
				ofile << "      " << i + 1;
			}
			else {
				ofile << "       " << i + 1;
			}
			if (atonum < 10) {
				ofile << "         " << atonum << "          0";
			}
			else {
				ofile << "          " << atonum << "          0";
			}
			ofile << fixed;
			ofile.precision(6);
			double coord1 = this->getgeo().at(i).coords[0];
			if (coord1 < 0) {
				ofile << "        " << coord1;
			}
			else {
				ofile << "         " << coord1;
			}
			for (int j = 1; j < 3; j++) {
				ofile << fixed;
				ofile.precision(6);
				double coord = this->getgeo().at(i).coords[j];
				if (coord < 0) {
					ofile << "    " << coord;
				}
				else {
					ofile << "     " << coord;
				}
			}
			ofile << endl;
		}

		ofile.precision(9);
		ofile << " SCF Done: =  " << this->getscf() << endl;
		ofile.close();
	}
};

class gausssp : public gaussRun {
public:
	void emulate(string outfile) {
		ofstream ofile(outfile);
		ofile << " Entering Gaussian System, Link 0 = g09" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << "#P wB97XD/6-311++G(d,p) freq formcheck" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << endl;
		ofile << endl;

		ofile << " Center     Atomic      Atomic             Coordinates (Angstroms)" << endl; //copy the required headers
		ofile << endl;
		ofile << endl;

		for (int i = 0; i < this->getcount(); i++) { //output all the data, formatted to look like a gaussian file
			int atonum = atomic_symbol_to_number(this->getgeo().at(i).symbol);
			if (i + 1 >= 10) {
				ofile << "      " << i + 1;
			}
			else {
				ofile << "       " << i + 1;
			}
			if (atonum < 10) {
				ofile << "         " << atonum << "          0";
			}
			else {
				ofile << "          " << atonum << "          0";
			}
			ofile << fixed;
			ofile.precision(6);
			double coord1 = this->getgeo().at(i).coords[0];
			if (coord1 < 0) {
				ofile << "        " << coord1;
			}
			else {
				ofile << "         " << coord1;
			}
			for (int j = 1; j < 3; j++) {
				ofile << fixed;
				ofile.precision(6);
				double coord = this->getgeo().at(i).coords[j];
				if (coord < 0) {
					ofile << "    " << coord;
				}
				else {
					ofile << "     " << coord;
				}
			}
			ofile << endl;
		}

		ofile.precision(9);
		ofile << " SCF Done: =  " << this->getscf() << endl;
		ofile.close();
	}
};
