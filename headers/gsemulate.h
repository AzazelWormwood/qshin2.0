#pragma once
#include "qchemfiles.h"
#include "gaussianfiles.h"



class gs : public qchemRun {
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
			
			//preserves alignment
			if (i + 1 >= 10) {
				ofile << "      " << i + 1;
			}
			else {
				ofile << "       " << i + 1;
			}
			if (atonum < 10) {
				ofile << "         " << atonum << "         0";
			}
			else {
				ofile << "          " << atonum << "         0";
			}
			ofile << "    ";
			ofile << fixed;
			ofile.precision(6);
			double coord1 = this->getgeo().at(i).coords[0];
			print_align(ofile, coord1); //not sure why, but the program breaks if i try to include the first coordinate in the loop. TODO investigate

			for (int j = 1; j < 3; j++) {
				ofile << fixed;
				ofile.precision(6);
				double coord = this->getgeo().at(i).coords[j];
			
				ofile << "    ";
				print_align(ofile, coord);
				
			}
			ofile << endl;
		}

		ofile.precision(9);
		ofile << " SCF Done: =  " << this->getscf() << endl;
		ofile << endl;
		ofile << endl;
		ofile << " incident light, reduced masses" << endl; //no idea why dushin cares about this line, but removing it breaks the interpreter, so here it will stay
		ofile << endl;

		//begin the normal modes section

		vector<nmode> nmodes = this->getnmodes();
		int total_nmodes = nmodes.size();
		int blocks = div_ceil(total_nmodes, 3);
		int nmodes_in_final_block = total_nmodes % 3;
		if (nmodes_in_final_block == 0) {
			nmodes_in_final_block = 3;
		}
		int current_block = 0;
		
		for (int i = 0; i < blocks - 1; i++) {

			ofile.precision(4);

			ofile << "                      " << nmodes.at((3 * i)).mode << "                      " << nmodes.at((3 * i) + 1).mode << "                      " << nmodes.at((3 * i) + 2).mode << endl;
			ofile << "                      A                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

			ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
			print_align(ofile, nmodes.at((3 * i)).freq);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * i) + 1).freq);
			ofile << "             ";
			print_align(ofile, nmodes.at((3 * i) + 2).freq);
			ofile << endl;

			ofile << " Red. masses --  ";
			print_align(ofile, nmodes.at((3 * i)).redmass);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).redmass);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * i) + 2).redmass);
			ofile << endl;

			ofile << " Frc consts  --  ";
			print_align(ofile, nmodes.at((3 * i)).frc);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).frc);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * i) + 2).frc);
			ofile << endl;


			ofile << " IR Inten    --  ";
			print_align(ofile, nmodes.at((3 * i)).ir);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).ir);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * i) + 2).ir);
			ofile << endl;

			ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

			for (int a = 0; a < this->getcount(); a++) {
				ofile.precision(2);
				ofile << "     " << a + 1 << "  ";
				if (a + 1 < 10) {
					ofile << " ";
				}
				ofile << nmodes.at((3 * i)).atoms.at(a).symbol << "     ";

				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[2]);
				ofile << endl;
			}
			current_block = i;
		}
				ofile.precision(4);
				switch (nmodes_in_final_block) {
				case 1:

					

					ofile << "                      " << nmodes.at((3 * current_block)).mode << endl;
					ofile << "                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

					ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
					print_align(ofile, nmodes.at((3 * current_block)).freq);
					
					ofile << endl;

					ofile << " Red. masses --  ";
					print_align(ofile, nmodes.at((3 * current_block)).redmass);
				
					ofile << endl;

					ofile << " Frc consts  --  ";
					print_align(ofile, nmodes.at((3 * current_block)).frc);
				
				
					ofile << endl;


					ofile << " IR Inten    --  ";
					print_align(ofile, nmodes.at((3 * current_block)).ir);
					
					ofile << endl;

					ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

					for (int a = 0; a < this->getcount(); a++) {
						ofile.precision(2);
						ofile << "     " << a + 1 << "  ";
						if (a + 1 < 10) {
							ofile << " ";
						}
						ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);

						ofile << endl;
					}
					break;
				case 2:
					ofile << "                      " << nmodes.at((3 * current_block)).mode << "                      " << nmodes.at((3 * current_block) + 1).mode << endl;
					ofile << "                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

					ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
					print_align(ofile, nmodes.at((3 * current_block)).freq);
					ofile << "              ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).freq);
					
					ofile << endl;

					ofile << " Red. masses --  ";
					print_align(ofile, nmodes.at((3 * current_block)).redmass);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).redmass);
					
					ofile << endl;

					ofile << " Frc consts  --  ";
					print_align(ofile, nmodes.at((3 * current_block)).frc);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).frc);
					
					ofile << endl;


					ofile << " IR Inten    --  ";
					print_align(ofile, nmodes.at((3 * current_block)).ir);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).ir);
				
					ofile << endl;

					ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

					for (int a = 0; a < this->getcount(); a++) {
						ofile.precision(2);
						ofile << "     " << a + 1 << "  ";
						if (a + 1 < 10) {
							ofile << " ";
						}
						ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);
						ofile << "   ";

						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[2]);
						ofile << "   ";

						ofile << endl;
					}
					break;
				case 3:
					ofile << "                      " << nmodes.at((3 * current_block)).mode << "                      " << nmodes.at((3 * current_block) + 1).mode << "                      " << nmodes.at((3 * current_block) + 2).mode << endl;
					ofile << "                      A                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

					ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
					print_align(ofile, nmodes.at((3 * current_block)).freq);
					ofile << "              ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).freq);
					ofile << "             ";
					print_align(ofile, nmodes.at((3 * current_block) + 2).freq);
					ofile << endl;

					ofile << " Red. masses --  ";
					print_align(ofile, nmodes.at((3 * current_block)).redmass);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).redmass);
					ofile << "              ";
					print_align(ofile, nmodes.at((3 * current_block) + 2).redmass);
					ofile << endl;

					ofile << " Frc consts  --  ";
					print_align(ofile, nmodes.at((3 * current_block)).frc);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).frc);
					ofile << "               ";
					print_align(ofile, nmodes.at((3 * current_block) + 2).frc);
					ofile << endl;


					ofile << " IR Inten    --  ";
					print_align(ofile, nmodes.at((3 * current_block)).ir);
					ofile << "                ";
					print_align(ofile, nmodes.at((3 * current_block) + 1).ir);
					ofile << "               ";
					print_align(ofile, nmodes.at((3 * current_block) + 2).ir);
					ofile << endl;

					ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

					for (int a = 0; a < this->getcount(); a++) {
						ofile.precision(2);
						ofile << "     " << a + 1 << "  ";
						if (a + 1 < 10) {
							ofile << " ";
						}
						ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);
						ofile << "   ";

						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[2]);
						ofile << "   ";

						print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[0]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[1]);
						ofile << "   ";
						print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[2]);
						ofile << endl;
					}
					break;
					
				}

		

		ofile << endl;
		
		ofile.precision(6);
		
		ofile << " Temperature   " << this->getvibtemp() << " Kelvin.  Pressure   " << this->getvibpres() << " Atm." << endl;
		
		for (int i = 0; i < this->getcount(); i++) {
			if (i + 1 > 9) {
				ofile << " Atom    " << i + 1;
			}
			else {
				ofile << " Atom     " << i + 1;
			}
			ofile << " has atomic number  " << atomic_symbol_to_number(this->getgeo().at(i).symbol) << " and mass  " << this->getgeo().at(i).mass << endl;

		}
		ofile.close();
	}
};

class gaussgs : public gaussRun {
public:
	void emulate(string outfile) {
		ofstream ofile(outfile);
		ofile << " Entering Gaussian System, Link 0 = g09" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << "# PwB97XD/6-311++G(d,p) freq formcheck" << endl;
		ofile << "-------------------------------------" << endl;
		ofile << endl;
		ofile << endl;

		ofile << " Center     Atomic      Atomic             Coordinates (Angstroms)" << endl; //copy the required headers
		ofile << endl;
		ofile << endl;

		for (int i = 0; i < this->getcount(); i++) { //output all the data, formatted to look like a gaussian file
			int atonum = atomic_symbol_to_number(this->getgeo().at(i).symbol);

			//preserves alignment
			if (i + 1 >= 10) {
				ofile << "      " << i + 1;
			}
			else {
				ofile << "       " << i + 1;
			}
			if (atonum < 10) {
				ofile << "         " << atonum << "         0";
			}
			else {
				ofile << "          " << atonum << "         0";
			}
			ofile << "    ";
			ofile << fixed;
			ofile.precision(6);
			double coord1 = this->getgeo().at(i).coords[0];
			print_align(ofile, coord1); //not sure why, but the program breaks if i try to include the first coordinate in the loop. TODO investigate

			for (int j = 1; j < 3; j++) {
				ofile << fixed;
				ofile.precision(6);
				double coord = this->getgeo().at(i).coords[j];

				ofile << "    ";
				print_align(ofile, coord);

			}
			ofile << endl;
		}

		ofile.precision(9);
		ofile << " SCF Done: =  " << this->getscf() << endl;
		ofile << endl;
		ofile << endl;
		ofile << " incident light, reduced masses" << endl; //no idea why dushin cares about this line, but removing it breaks the interpreter, so here it will stay
		ofile << endl;

		//begin the normal modes section

		vector<nmode> nmodes = this->getnmodes();
		int total_nmodes = nmodes.size();
		int blocks = div_ceil(total_nmodes, 3);
		int nmodes_in_final_block = total_nmodes % 3;
		if (nmodes_in_final_block == 0) {
			nmodes_in_final_block = 3;
		}
		int current_block = 0;

		for (int i = 0; i < blocks - 1; i++) {

			ofile.precision(4);

			ofile << "                      " << nmodes.at((3 * i)).mode << "                      " << nmodes.at((3 * i) + 1).mode << "                      " << nmodes.at((3 * i) + 2).mode << endl;
			ofile << "                      A                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

			ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
			print_align(ofile, nmodes.at((3 * i)).freq);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * i) + 1).freq);
			ofile << "             ";
			print_align(ofile, nmodes.at((3 * i) + 2).freq);
			ofile << endl;

			ofile << " Red. masses --  ";
			print_align(ofile, nmodes.at((3 * i)).redmass);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).redmass);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * i) + 2).redmass);
			ofile << endl;

			ofile << " Frc consts  --  ";
			print_align(ofile, nmodes.at((3 * i)).frc);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).frc);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * i) + 2).frc);
			ofile << endl;


			ofile << " IR Inten    --  ";
			print_align(ofile, nmodes.at((3 * i)).ir);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * i) + 1).ir);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * i) + 2).ir);
			ofile << endl;

			ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

			for (int a = 0; a < this->getcount(); a++) {
				ofile.precision(2);
				ofile << "     " << a + 1 << "  ";
				if (a + 1 < 10) {
					ofile << " ";
				}
				ofile << nmodes.at((3 * i)).atoms.at(a).symbol << "     ";

				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i)).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 1).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * i) + 2).atoms.at(a).coords[2]);
				ofile << endl;
			}
			current_block = i;
		}
		ofile.precision(4);
		switch (nmodes_in_final_block) {
		case 1:



			ofile << "                      " << nmodes.at((3 * current_block)).mode << endl;
			ofile << "                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

			ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
			print_align(ofile, nmodes.at((3 * current_block)).freq);

			ofile << endl;

			ofile << " Red. masses --  ";
			print_align(ofile, nmodes.at((3 * current_block)).redmass);

			ofile << endl;

			ofile << " Frc consts  --  ";
			print_align(ofile, nmodes.at((3 * current_block)).frc);


			ofile << endl;


			ofile << " IR Inten    --  ";
			print_align(ofile, nmodes.at((3 * current_block)).ir);

			ofile << endl;

			ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

			for (int a = 0; a < this->getcount(); a++) {
				ofile.precision(2);
				ofile << "     " << a + 1 << "  ";
				if (a + 1 < 10) {
					ofile << " ";
				}
				ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);

				ofile << endl;
			}
			break;
		case 2:
			ofile << "                      " << nmodes.at((3 * current_block)).mode << "                      " << nmodes.at((3 * current_block) + 1).mode << endl;
			ofile << "                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

			ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
			print_align(ofile, nmodes.at((3 * current_block)).freq);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).freq);

			ofile << endl;

			ofile << " Red. masses --  ";
			print_align(ofile, nmodes.at((3 * current_block)).redmass);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).redmass);

			ofile << endl;

			ofile << " Frc consts  --  ";
			print_align(ofile, nmodes.at((3 * current_block)).frc);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).frc);

			ofile << endl;


			ofile << " IR Inten    --  ";
			print_align(ofile, nmodes.at((3 * current_block)).ir);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).ir);

			ofile << endl;

			ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

			for (int a = 0; a < this->getcount(); a++) {
				ofile.precision(2);
				ofile << "     " << a + 1 << "  ";
				if (a + 1 < 10) {
					ofile << " ";
				}
				ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[2]);
				ofile << "   ";

				ofile << endl;
			}
			break;
		case 3:
			ofile << "                      " << nmodes.at((3 * current_block)).mode << "                      " << nmodes.at((3 * current_block) + 1).mode << "                      " << nmodes.at((3 * current_block) + 2).mode << endl;
			ofile << "                      A                      A                      A" << endl; //dushin doesn't care about symmetry but it does need dummy values here

			ofile << " Frequencies --  "; //these blocks are organised like this instead of like the above in order to play nicely with the operator overload
			print_align(ofile, nmodes.at((3 * current_block)).freq);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).freq);
			ofile << "             ";
			print_align(ofile, nmodes.at((3 * current_block) + 2).freq);
			ofile << endl;

			ofile << " Red. masses --  ";
			print_align(ofile, nmodes.at((3 * current_block)).redmass);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).redmass);
			ofile << "              ";
			print_align(ofile, nmodes.at((3 * current_block) + 2).redmass);
			ofile << endl;

			ofile << " Frc consts  --  ";
			print_align(ofile, nmodes.at((3 * current_block)).frc);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).frc);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * current_block) + 2).frc);
			ofile << endl;


			ofile << " IR Inten    --  ";
			print_align(ofile, nmodes.at((3 * current_block)).ir);
			ofile << "                ";
			print_align(ofile, nmodes.at((3 * current_block) + 1).ir);
			ofile << "               ";
			print_align(ofile, nmodes.at((3 * current_block) + 2).ir);
			ofile << endl;

			ofile << "  Atom  AN      X      Y      Z        X      Y      Z        X      Y      Z" << endl;

			for (int a = 0; a < this->getcount(); a++) {
				ofile.precision(2);
				ofile << "     " << a + 1 << "  ";
				if (a + 1 < 10) {
					ofile << " ";
				}
				ofile << nmodes.at((3 * current_block)).atoms.at(a).symbol << "     ";

				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block)).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 1).atoms.at(a).coords[2]);
				ofile << "   ";

				print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[0]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[1]);
				ofile << "   ";
				print_align(ofile, nmodes.at((3 * current_block) + 2).atoms.at(a).coords[2]);
				ofile << endl;
			}
			break;

		}



		ofile << endl;

		ofile.precision(6);

		ofile << " Temperature   " << this->getvibtemp() << " Kelvin.  Pressure   " << this->getvibpres() << " Atm." << endl;

		for (int i = 0; i < this->getcount(); i++) {
			if (i + 1 > 9) {
				ofile << " Atom    " << i + 1;
			}
			else {
				ofile << " Atom     " << i + 1;
			}
			ofile << " has atomic number  " << atomic_symbol_to_number(this->getgeo().at(i).symbol) << " and mass  " << this->getgeo().at(i).mass << endl;

		}
		ofile.close();
	}
};

