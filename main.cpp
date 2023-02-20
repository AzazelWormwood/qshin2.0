
#include "headers/gaussianfiles.h"
#include "headers/qchemfiles.h"
#include "headers/spemulate.h"
#include "headers/gsemulate.h"



struct availableCommands {
	string functions[9] = { "summarize", "emulate", "freq-arrange", "help", "quit", "test", "to-alex", "combine", "calc-hrfs"};
	string funchelp[9] = {
	"Outputs a summary of the requested file, consisting of the final geometry and SCF energy.",
	"Takes data from a QChem output file and creates a Gaussian-formatted output file, for the purpose of being read by Dushin.",
	"Arranges the frequencies from the requested file into a column (creates the freq.dat file); this function is a more robust version of \"freq_arrange.f\".",
	"Prints the description of each available function.",
	"Quits the program.",
	"Runs the emulate function on every file in the /tests directory, allowing for troubleshooting the output against the expected output files.",
	"Creates input files for Alex's FGR.exe code from the requested file.",
	"Emulates a Gaussian file like the \"emulate\" function, but allows for taking the geometry and frequencies from separate files.",
	"Creates an HRF file readable by Alex's code by calculating the HRF from the frequency and projection; reads supplem.dat file from Dushin output."
	};
	string flags[28] = {"-s", "-sg", "-sq", "-e", "-ef","-en","-h", "-f","-fg", "-fq","-a", "-ag", "-aq","-ch", "-c", "-i", "-t", "-cg", "-cqg", "-cgq","-b","-bq","-bg","-r","-rq","-rg","-rqg","-rgq"};
};

//class configParser {
//public:
//	void parse(string filename);
//	bool get_linearity() {
//		return this->specify_linearity;
//	}
//private:
//	bool specify_linearity;
//};

class inputHandler {
public:

	void mainLoop();
	void summarize();
	void freq_arrange();
	void emulate();
	void help();
	void test();
	void to_alex();
	void combine();
	void calc_hrfs();
	void parse();
	availableCommands get_commands() {
		return commands;
	}
	void setflags(vector<string> flags) {
		this->flags = flags;
		return;
	}
	void setargc(int argc) {
		this->argc = argc;
		return;
	}
	void setargv(char** argv) {
		this->argv = argv;
	}

private:
	int choice;
	availableCommands commands;
	vector<string> flags;
	int argc;
	char** argv;
};

int main(int argc, char** argv) { //argc is argument count; counts number of command line arguments; argv is the argument values and is an array of pointers to the input flags
	inputHandler interface;
	interface.setargc(argc);
	interface.setargv(argv);

	//command line arguments parser
	if (argc > 1) {
		vector<string> flags;
		for (int i = 1; i < argc; i++) {
			for (int j = 0; j < 28; j++) {
				if (argv[i] == interface.get_commands().flags[j]) {
					flags.push_back(argv[i]);
				}
			}
			for (int j = 0; j < 9; j++) {
				if (argv[i] == "--" + interface.get_commands().functions[j]) {
					flags.push_back(argv[i]);
				}
			}
				
			}
		interface.setflags(flags);
		}
	interface.parse();

	//initialise main loop
	//interface.mainLoop();
	
	
}

void inputHandler::mainLoop() {
	while (true) {
		cout << "Available Commands:" << endl;
		for (string func : this->commands.functions) {
			cout << func << endl;
		}
		cout << "Please enter a command: ";
		string input;
		cin >> input;
		transform(input.begin(), input.end(), input.begin(), ::tolower); //makes input string lowercase
		int choice = 0;
		for (string func : this->commands.functions) {
			if (func == input) {
				break;
			}
			else {
				choice++;
			}
		}

		this->choice = choice;
		if (this->choice >= (sizeof(this->commands.functions) / sizeof(this->commands.functions[0]))) { //if command is not in functions array
			std::cout << "Invalid Command" << endl;
			std::cout << endl;
			return;
		}

		switch (this->choice) {
		case 0: //if user input "summarize"
			this->summarize();
			break;
		case 1: //if user input "emulate"
			this->emulate();
			break;
		case 2: //if user input "freq_arrange"
			this->freq_arrange();
			break;
		case 3: //if user input "help"
			this->help();
			break;
		case 4: //if user input "quit"
			std::cout << "Exiting program..." << endl;
			return;
			break; //not strictly necessary, but follows convention
		case 5: //if user input test--here for an easy command for me to test certain functions with
			this->test();
			break;
		case 6:
			this->to_alex();
			break;
		case 7:
			this->combine();
			break;
		case 8:
			this->calc_hrfs();
			break;
		default:
			break;
		}

	}
}



void inputHandler::summarize() {
	std::cout << "----------------------------------------------" << endl;
	std::cout << "Would you like to summarize a Gaussian file or a QChem file? (input 'g' or 'q' for Gaussian or QChem respectively, or 'm' to return to the main menu): ";
	char input;

	while (true) {
		std::cin >> input;
		input = tolower(input);
		while (std::cin.get() != '\n'); //clears input stream if user inputs too many characters
		if (input == 'g') {
			
			std::cout << "Please enter the name of the file you would like to summarize (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/summary_" + file;
			gaussRun file_to_summarize;
			
			int error = file_to_summarize.get(infile);
		
			if (error == 0) {
				std::cout << "summary_" << file << " created in /outputfiles directory." << endl;
				file_to_summarize.print(outfile);
			}
			return;
			
		}
		if (input == 'q') {
			std::cout << "Please enter the name of the file you would like to summarize (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/summary_" + file;
			qchemRun file_to_summarize;
			int error = file_to_summarize.get(infile);
			
			if (error == 0) {
				file_to_summarize.print(outfile);
				std::cout << "summary_" << file << " created in /outputfiles directory." << endl;
				
			}
			return;
		}
		if (input == 'm') {
			return;
		}
		std::cout << "Invalid input. Enter 'g' to summarize a Gaussian file, or 'q' to summarize a QChem file. Otherwise, enter 'm' to return to the main menu: ";
		
	}
}

void inputHandler::freq_arrange() {
	std::cout << "----------------------------------------------" << endl;
	std::cout << "Would you like to arrange the frequencies from a Gaussian file or a QChem file? (input 'g' or 'q' for Gaussian or QChem respectively, or 'm' to return to the main menu): ";
	char input;
	while (true) {
		std::cin >> input;
		input = tolower(input);
		while (std::cin.get() != '\n'); //clears input stream if user inputs too many characters
		if (input == 'g') {
			std::cout << "Please enter the name of the file you would like to arrange (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/freq.dat";
			gaussRun freq_file;
			int error = freq_file.freq_get(infile);
			if (error == 0) {
				std::cout << "freq.dat created in /outputfiles directory." << endl;
				freq_file.arrange(outfile);
			}
			return;
		}

		if (input == 'q') {
			std::cout << "Please enter the name of the file you would like to arrange (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/freq.dat";
			qchemRun freq_file;
			int error = freq_file.freq_get(infile);
			if (error == 0) {
				std::cout << "freq.dat created in /outputfiles directory." << endl;
				freq_file.arrange(outfile);
			}
			return;
		}
		if (input == 'm') {
			return;
		}

		std::cout << "Invalid input. Enter 'g' to summarize a Gaussian file, or 'q' to summarize a QChem file. Otherwise, enter 'm' to return to the main menu: ";
	}
}


void inputHandler::emulate() {
	
	std::cout << "Please enter the name of the QChem file you would like to make look like a Gaussian file (make sure the file is in the /inputfiles directory): ";
	string file;
	std::cin >> file;
	string infile;
	std::cout << endl;
	std::cout << "Please enter the desired name of the output file: ";
	string filename;
	std::cin >> filename;
	infile = "inputfiles/" + file;
	string outfile = "outputfiles/" + filename;
	std::cout << "Does this file contain frequencies? (y/n): ";
	string resp;
	std::cin >> resp;
	resp = tolower(resp[0]);
	if (resp[0] == 'y') {
		gs file_to_emulate;


		int error = file_to_emulate.get(infile);
		if (error == 0) {
			std::cout << filename << " created in /outputfiles directory." << endl;
			file_to_emulate.emulate(outfile);
		}
	}
	else{
		sp file_to_emulate;
			
		int error = file_to_emulate.get(infile);
		if (error == 0) {
			std::cout << filename << " created in /outputfiles directory." << endl;
			file_to_emulate.emulate(outfile);
			}

		}
	
		
}

void inputHandler::help() {
	std::cout << "What function would you like help with (type all for all functions): ";
	string input;
	std::cin >> input;
	std::cout << endl;
	transform(input.begin(), input.end(), input.begin(), ::tolower); //makes input string lowercase
	if (input == "all") {
		for (int i = 0; i<(sizeof(this->commands.functions) / sizeof(this->commands.functions[0])); i++){
			std::cout << this->commands.functions[i] << ": " << this->commands.funchelp[i] << endl; 
			std::cout << endl;
		}
		return;
	}
	int choice = 0;
	for (string func : this->commands.functions) {
		if (func == input) {
			break;
		}
		else {
			choice++;
		}
	}
	this->choice = choice;
	if (this->choice >= (sizeof(this->commands.functions) / sizeof(this->commands.functions[0]))) { //if command is not in functions array
		std::cout << "Invalid Command" << endl;
		std::cout << endl;
		return;
	}
	std::cout << this->commands.funchelp[choice] << endl;
	std::cout << endl;
	return;
}

void inputHandler::test() {
	
	gs test;
	test.get("tests/inputs/diacetylene.out");
	test.emulate("tests/test-outputs/diacetylene.out");

	gs test1;
	test1.get("tests/inputs/n2_freq.out");
	test1.emulate("tests/test-outputs/n2_freq.out");

	sp test2;
	test2.get("tests/inputs/excited_difluorodiphenyl.out");
	test2.emulate("tests/test-outputs/excited_difluorodiphenyl.out");

	gs test3;
	test3.get("tests/inputs/difluorodiphenyl.out");
	test3.emulate("tests/test-outputs/difluorodiphenyl.out");

	gs test4;
	test4.get("tests/inputs/c60.out");
	test4.emulate("tests/test-outputs/c60.out");

	gs test5;
	test5.get("tests/inputs/co2.out");
	test5.emulate("tests/test-outputs/co2.out");

	sp test6;
	test6.get("tests/inputs/n2.out");
	test6.emulate("tests/test-outputs/n2.out");

    sp test7;
	test7.get("tests/inputs/water.out");
	test7.emulate("tests/test-outputs/water.out");

	sp test8;
	test8.get("tests/inputs/fe@c60.out");
	test8.emulate("tests/test-outputs/fe@c60.out");

	//supplem_to_alex("inputfiles/supplem.dat", "outputfiles/hrf.dat");

	//gaussRun roshan;
	//roshan.get("inputfiles/sp.log");
	//vector<atom> geo = roshan.getgeo();
	//double scf = 0;
	//int count = geo.size();

	//sp roshan_convert;
	//roshan_convert.setgeo(geo);
	//roshan_convert.setscf(scf);
	//roshan_convert.setcount(count);
	//roshan_convert.emulate("outputfiles/c.log");

	return;
}

void inputHandler::to_alex() {
	std::cout << "----------------------------------------------" << endl;
	std::cout << "Would you like to make input files for Alex's FGR code from a Gaussian file or a QChem file? (input 'g' or 'q' for Gaussian or QChem respectively, or 'm' to return to the main menu): ";
	char input;
	while (true) {
		std::cin >> input;
		input = tolower(input);
		while (std::cin.get() != '\n'); //clears input stream if user inputs too many characters
		if (input == 'm') {
			return;
		}
		if (input == 'g') {
			std::cout << "Please enter the name of the source file (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			std::cout << "Please enter the desired name of the output file: ";
			string filename;
			std::cin >> filename;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/" + filename;
			gaussRun run;
			run.to_alex(infile, outfile);
			std::cout << outfile << " created in /outputfiles directory." << endl;


			return;
		}

		if (input == 'q') {
			std::cout << "Please enter the name of the source file (make sure the file is in the /inputfiles directory): ";
			string file;
			std::cin >> file;
			string infile;
			std::cout << "Please enter the desired name of the output file: ";
			string filename;
			std::cin >> filename;
			infile = "inputfiles/" + file;
			string outfile = "outputfiles/" + filename;
			qchemRun run;
			run.get(infile);
			run.to_alex(outfile);
			std::cout << outfile << " created in /outputfiles directory." << endl;


			return;
		}
	}
}

void inputHandler::combine() {
	std::cout << "Please enter the file from which to pull the frequencies: ";
	string freq_file;
	std::cin >> freq_file;
	string freq_infile = "inputfiles/" + freq_file;
	std::cout << "Please enter the file from which to pull the geometry: ";
	string geo_file;
	std::cin >> geo_file;
	string geo_infile = "inputfiles/" + geo_file;
	string outfile = "outputfiles/gs.log";
	gs freqfile;

	int error = freqfile.get(freq_infile);
	
	gs geofile;
	error = geofile.get(geo_infile);
	freqfile.setgeo(geofile.getgeo());
	if (error == 0) {
		std::cout << outfile << " created in /outputfiles directory." << endl;
		freqfile.emulate(outfile);
	}
	return;
}

void inputHandler::calc_hrfs() {
	std::cout << "Please enter the name of the source file (make sure the file is in the /inputfiles directory): ";
	string file;
	std::cin >> file;
	string infile;
	std::cout << "Please enter the desired name of the output file: ";
	string filename;
	std::cin >> filename;
	infile = "inputfiles/" + file;
	string outfile = "outputfiles/" + filename;
	int error = supplem_to_alex(infile, outfile);
	if (error == 0) {
		std::cout << filename << " created in /outputfiles directory." << endl;
	}
	return;
}

void inputHandler::parse() {
	for (string flag : this->flags) {
		if (flag == "-sq" || flag == "--summarize"|| flag == "-s") { //summarize qchem input file
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			string filename = argv[pos + 1];
			qchemRun file_to_summarize;
			int error = file_to_summarize.get(filename);
			string outfile = "summary_" + filename;
			if (error == 0) {
				file_to_summarize.print(outfile);
				std::cout << outfile << " created." << endl;

			}
		}
		if (flag == "-sg") { //summarize gaussian input file
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			string filename = argv[pos + 1];
			gaussRun file_to_summarize;
			int error = file_to_summarize.get(filename);
			string outfile = "summary_" + filename;
			if (error == 0) {
				file_to_summarize.print(outfile);
				std::cout << outfile << " created." << endl;

			}
		}
		if (flag == "-e" || flag == "--emulate" || flag == "-ef") { //emulate frequency gaussian file into output file
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gs file_to_emulate;

			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "gs.log";
			}
			else {
				outfile = argv[pos+2];
			}

			int error = file_to_emulate.get(infile);
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				file_to_emulate.emulate(outfile);
			}
		}
		if (flag == "-en") { //emulate non-frequency gaussian files
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			sp file_to_emulate;

			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "sp.log";
			}
			else {
				outfile = argv[pos + 2];
			}

			int error = file_to_emulate.get(infile);
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				file_to_emulate.emulate(outfile);
			}
		}
		if (flag == "-h" || flag == "--help") { //print help options
			std::cout << "Qshin is a multipurpose program for the manipulation of Q-chem output files, with the primary goal of using Q-Chem files in Dushin (Reimers, J. R. J. Chem. Phys. 2001, 115, 91039109.)" << endl;
			std::cout << "Written by Elijah Holder in 2022" << endl;
			std::cout << "-s, -sq, --summarize: " << "Outputs a summary of the requested Q-Chem file, consisting of the final geometry and SCF energy." << endl;
			std::cout << "-sg: " << "As above, but for Gaussian files." << endl;
			std::cout << "-e, -ef, --emulate: " << "Takes data from a QChem frequency job and creates a Gaussian-formatted output file, for the purpose of being read by Dushin." << endl;
			std::cout << "-en: " << "As above, but for non-frequency jobs" << endl;
			std::cout << "-f, -fq, --freq-arrange: " << "Arranges the frequencies from the requested Q-Chem file into a column (creates the freq.dat file); this function is a more robust version of \"freq_arrange.f\"." << endl;
			std::cout << "-fg: " << "As above, but for Gaussian files." << endl;
			std::cout << "-a, -aq, --to-alex: " << "Creates input files for Alex's FGR.exe code from the requested Q-Chem file." << endl;
			std::cout << "-ag: " << "As above, but for Gaussian files." << endl;
			std::cout << "-c, --combine: " << "Emulates a Gaussian file like the \"emulate\" function, but allows for taking the geometry and frequencies from separate Q-Chem files." << endl;
			std::cout << "-cg, -cqg, -cgq: " << "As above, but for Gaussian files, a Q-Chem frequency file + a Gaussian geometry file, and a Gaussian frequency file + a Q-Chem geometry file respectively." << endl;
			std::cout << "-ch, --calc-hrfs: " << "Creates an HRF file readable by Alex's code by calculating the HRF from the frequency and projection; reads supplem.dat file from Dushin output." << endl;
			std::cout << "-t: " << "Runs the emulate function on every file in the /tests directory, allowing for troubleshooting the output against the expected output files." << endl;
			std::cout << "-i: " << "Opens an interactive UI; allows for prompted command rather than command-line execution." << endl;
			std::cout << "-b, -bq: " << "Creates a basic dushin-readable file." << endl;
			std::cout << "-bg: " << "As above, but for Gaussian files." << endl;
			std::cout << "-r, -rq: " << "Calculates the RMSD of two Q-Chem structures." << endl;
			std::cout << "-rg, -rqg, -rgq: " << "As above, but for Gaussian files, a Q-Chem frequency file + a Gaussian geometry file, and a Gaussian frequency file + a Q-Chem geometry file respectively." << endl;

		}
		if (flag == "-fq" || flag == "--freq-arrange" || flag == "-f") { //arrange qchem frequencies
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			qchemRun freq_file;
			string infile = argv[pos + 1];
			int error = freq_file.freq_get(infile);
			if (error == 0) {
				std::cout << "freq.dat created." << endl;
				freq_file.arrange("freq.dat");
			}
		}
		if (flag == "-fg") { //arrange gaussain frequencies
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			
			gaussRun freq_file;
			string infile = argv[pos + 1];
			int error = freq_file.freq_get(infile);
			if (error == 0) {
				std::cout << "freq.dat created." << endl;
				freq_file.arrange("freq.dat");
			}
		}
		if (flag == "-a" || flag == "--to-alex"|| flag == "-aq") { //generate alex input files from qchem
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "alex.xyz";
			}
			else {
				outfile = argv[pos + 2];
			}
			qchemRun run;
			run.get(infile);
			run.to_alex(outfile);
			std::cout << outfile << " created." << endl;
		}
		if (flag == "-ag") { //generate alex input files from gaussian
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "alex.xyz";
			}
			else {
				outfile = argv[pos + 2];
			}
			gaussRun run;
			run.to_alex(infile,outfile);
			std::cout << outfile << " created." << endl;
		}
		if (flag == "-ch" || flag == "--calc-hrfs") { //calculate hrfs from projections
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "hrf.dat";
			}
			else {
				outfile = argv[pos + 2];
			}
			int error = supplem_to_alex(infile, outfile);
			if (error == 0) {
				std::cout << outfile << " created." << endl;
			}
		}
		if (flag == "-i") { //initialise UI-based interaction
			this->mainLoop();
		}
		if (flag == "-c" || flag == "--combine") { //emulate gaussian file by combining two files for freqs and geometry
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gs freqfile;
			string freq_infile = argv[pos + 1];
			string geo_infile = argv[pos + 2];

			int error = freqfile.get(freq_infile);

			qchemRun geofile;
			error = geofile.get(geo_infile);
			freqfile.setgeo(geofile.getgeo());
			string outfile;
			if (argc <= pos + 3) {
				outfile = "gs.log";
			}
			else {
				outfile = argv[pos + 3];
			}
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				freqfile.emulate(outfile);
			}
		}
		if (flag == "-cg") { //emulate gaussian file by combining two files for freqs and geometry
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gaussgs freqfile;
			string freq_infile = argv[pos + 1];
			string geo_infile = argv[pos + 2];

			int error = freqfile.get(freq_infile);

			gaussRun geofile;
			error = geofile.get(geo_infile);
			freqfile.setgeo(geofile.getgeo());
			string outfile;
			if (argc <= pos + 3) {
				outfile = "gs.log";
			}
			else {
				outfile = argv[pos + 3];
			}
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				freqfile.emulate(outfile);
			}
		}

		if (flag == "-cqg") { //emulate gaussian file by combining two files for freqs and geometry
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gs freqfile;
			string freq_infile = argv[pos + 1];
			string geo_infile = argv[pos + 2];

			int error = freqfile.get(freq_infile);

			gaussRun geofile;
			error = geofile.get(geo_infile);
			freqfile.setgeo(geofile.getgeo());
			string outfile;
			if (argc <= pos + 3) {
				outfile = "gs.log";
			}
			else {
				outfile = argv[pos + 3];
			}
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				freqfile.emulate(outfile);
			}
		}

		if (flag == "-cgq") { //emulate gaussian file by combining two files for freqs and geometry
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gaussgs freqfile;
			string freq_infile = argv[pos + 1];
			string geo_infile = argv[pos + 2];

			int error = freqfile.get(freq_infile);

			qchemRun geofile;
			error = geofile.get(geo_infile);
			freqfile.setgeo(geofile.getgeo());
			string outfile;
			if (argc <= pos + 3) {
				outfile = "gs.log";
			}
			else {
				outfile = argv[pos + 3];
			}
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				freqfile.emulate(outfile);
			}
		}
		
		if (flag == "-t" || flag == "--test") { //run tests
			this->test();
		}

		if (flag == "-b" || flag == "-bq") {
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			qchemRun file;

			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "gs.coord";
			}
			else {
				outfile = argv[pos + 2];
			}

			int error = file.get(infile);
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				file.basic_dushin(outfile);
			}
		}

		if (flag == "-bg") {
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gaussRun file;

			string infile = argv[pos + 1];
			string outfile;
			if (argc <= pos + 2) {
				outfile = "gs.coord";
			}
			else {
				outfile = argv[pos + 2];
			}

			int error = file.get(infile);
			if (error == 0) {
				std::cout << outfile << " created." << endl;
				file.basic_dushin(outfile);
			}
		}

		if (flag == "-r" || flag == "-rq") { //calculate RMSD
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			qchemRun struct1;
			string file1 = argv[pos + 1];
			string file2 = argv[pos + 2];

			int error1 = struct1.get(file1);

			qchemRun struct2;
			int error2 = struct2.get(file2);
			
			if (error1 == 0 && error2 == 0) {
				if (struct1.getcount() != struct2.getcount()) {
					std::cout << "Error: structures do not have the same number of atoms" << endl;
					return;
				}
				int count = struct1.getcount();
				vector<atom> geo1 = struct1.getgeo();
				vector<atom> geo2 = struct2.getgeo();
				double sum = 0;
				for (int i = 0; i < count; i++) {
					double xdiff = geo1.at(i).coords[0] - geo2.at(i).coords[0];
					double ydiff = geo1.at(i).coords[1] - geo2.at(i).coords[1];
					double zdiff = geo1.at(i).coords[2] - geo2.at(i).coords[2];
					double distsquare = pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2);
					sum += distsquare;
				}
				double msd = sum / count;
				double rmsd = sqrt(msd);
				std::cout << "RMSD: " << rmsd << endl;
			}
		}
		if (flag == "-rg") { //calculate RMSD
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gaussRun struct1;
			string file1 = argv[pos + 1];
			string file2 = argv[pos + 2];

			int error1 = struct1.get(file1);

			gaussRun struct2;
			int error2 = struct2.get(file2);

			if (error1 == 0 && error2 == 0) {
				if (struct1.getcount() != struct2.getcount()) {
					std::cout << "Error: structures do not have the same number of atoms" << endl;
					return;
				}
				int count = struct1.getcount();
				vector<atom> geo1 = struct1.getgeo();
				vector<atom> geo2 = struct2.getgeo();
				double sum = 0;
				for (int i = 0; i < count; i++) {
					double xdiff = geo1.at(i).coords[0] - geo2.at(i).coords[0];
					double ydiff = geo1.at(i).coords[1] - geo2.at(i).coords[1];
					double zdiff = geo1.at(i).coords[2] - geo2.at(i).coords[2];
					double distsquare = pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2);
					sum += distsquare;
				}
				double msd = sum / count;
				double rmsd = sqrt(msd);
				std::cout << "RMSD: " << rmsd << endl;
			}
		}
		if (flag == "-rqg") { //calculate RMSD
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			qchemRun struct1;
			string file1 = argv[pos + 1];
			string file2 = argv[pos + 2];

			int error1 = struct1.get(file1);

			gaussRun struct2;
			int error2 = struct2.get(file2);

			if (error1 == 0 && error2 == 0) {
				if (struct1.getcount() != struct2.getcount()) {
					std::cout << "Error: structures do not have the same number of atoms" << endl;
					return;
				}
				int count = struct1.getcount();
				vector<atom> geo1 = struct1.getgeo();
				vector<atom> geo2 = struct2.getgeo();
				double sum = 0;
				for (int i = 0; i < count; i++) {
					double xdiff = geo1.at(i).coords[0] - geo2.at(i).coords[0];
					double ydiff = geo1.at(i).coords[1] - geo2.at(i).coords[1];
					double zdiff = geo1.at(i).coords[2] - geo2.at(i).coords[2];
					double distsquare = pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2);
					sum += distsquare;
				}
				double msd = sum / count;
				double rmsd = sqrt(msd);
				std::cout << "RMSD: " << rmsd << endl;
			}
		}
		if (flag == "-rgq") { //calculate RMSD
			int pos; //position of argument in argv
			for (int i = 0; i < this->argc; i++) {
				if (this->argv[i] == flag) {
					pos = i;
					break;
				}
			}
			gaussRun struct1;
			string file1 = argv[pos + 1];
			string file2 = argv[pos + 2];

			int error1 = struct1.get(file1);

			qchemRun struct2;
			int error2 = struct2.get(file2);

			if (error1 == 0 && error2 == 0) {
				if (struct1.getcount() != struct2.getcount()) {
					std::cout << "Error: structures do not have the same number of atoms" << endl;
					return;
				}
				int count = struct1.getcount();
				vector<atom> geo1 = struct1.getgeo();
				vector<atom> geo2 = struct2.getgeo();
				double sum = 0;
				for (int i = 0; i < count; i++) {
					double xdiff = geo1.at(i).coords[0] - geo2.at(i).coords[0];
					double ydiff = geo1.at(i).coords[1] - geo2.at(i).coords[1];
					double zdiff = geo1.at(i).coords[2] - geo2.at(i).coords[2];
					double distsquare = pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2);
					sum += distsquare;
				}
				double msd = sum / count;
				double rmsd = sqrt(msd);
				std::cout << "RMSD: " << rmsd << endl;
			}
		}
	}
	return;
}


//void configParser::parse(string filename) {
//	string configs[] = {"set_linearity"};
//	string current;
//	string setting;
//	string trash;
//	ifstream ifile;
//	ifile.open(filename);
//	while (ifile.is_open()) {
//		ifile >> current;
//		transform(current.begin(), current.end(), current.begin(), ::tolower); //makes current lowercase
//		if (current.find("*") != string::npos || current.find("#") != string::npos) {
//			continue;
//		}
//		int i = 0;
//		for (string match : configs) {
//			
//			if (current == match) {
//				ifile >> trash;
//				ifile >> setting;
//				transform(setting.begin(), setting.end(), setting.begin(), ::tolower); //makes setting lowercase
//				switch (i) {
//				case 0:
//					if (setting == "true") {
//						this->specify_linearity = true;
//					}
//					else {
//						this->specify_linearity = false;
//					}
//					break;
//				}
//				
//
//
//			}
//			if (current == "END") {
//				return;
//			}
//		}
//		}
//	return;
//}
