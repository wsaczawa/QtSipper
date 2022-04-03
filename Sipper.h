#pragma once
#include <string>
#include <vector>

class ftpComm {
public: 
	bool check;
	std::string local;
	std::string source;

	ftpComm();
	void transfer(std::string path, std::string File1, std::string File2);
	void get(std::string source, std::string local);
	void put(std::string local, std::string source);

	void take();
	void release();

	void download(std::string source, std::string local);
	void upload();
};

class library {
public:
	std::vector<std::string> isotopes;

	void getIsotopes();
	void readIsotopes();

	library();
};

class nuclid {
public:
	std::string name;
	double raw;
	double background;
	double value;
	double sigma;

	void getbackground(nuclid Nc, std::string line, int pos);
	void getspectral(nuclid Nc, std::string line, int pos);
};

class dataset {
public:
	bool background;
	int id;
	double time;
	double volume;
	std::string detector;
	std::string asf;
	std::string name;
	std::string family;
	std::vector<nuclid> nuclides;

	dataset();

	void getDetector();
	void input(std::string name, double time, double volume, std::string det);
	void analyze();
};

class rawCSV {
public:
	std::string path;
	std::vector<std::pair<std::string, std::vector<double>>> vector;

	void write(dataset input);
	void overwrite(dataset input);
	void reset(std::vector<std::string> isotopes);
	bool check(std::string input);
	int count();
	void getVector();
};

class processedCSV {
public:
	std::string path;
	std::vector<std::string> vector;

	void write(dataset input);
	void overwrite(std::vector<dataset> input);
	void reset(std::vector<std::string> isotopes);
	bool check(std::string input);
	int count();
};

class stats {
public:
	int id;
	std::string path;
	double S;
	double Sx;
	double Sy;
	double Sxx;
	double Syy;
	double Sxy;
	double a;
	double b;
	double nMSE;
	double sigma2;

	void update();
	void import();
	void evaluate(dataset set, nuclid Nc);
	void getreg();
	void getsigma();
	void sumnMSE(std::string line, int pos_x, int pos_y);
	double reg(double x);
	double error2(double x, double y);
	int getalarm(double err);
	void reset();
	void reevaluate(std::string line);

};

class database {
public:
	std::string server;
	std::string statsFolder;
	rawCSV spectral;
	rawCSV background;
	processedCSV analyzed;
	std::vector<stats> nuclides;
	bool ftpCheck;

	database();

	void getFtp();
	void getServer();
	bool check(std::string name);
	void evaluate(dataset set);
	void bias();
	void results();
	void reset(std::vector<std::string> isotopes);
	void reevaluate();
};

class data {
public:
	dataset measured;
	std::vector<dataset> analyzed;

	void findBackground(database source);
	void findSpectral(database source);
};

std::string getcell(std::string line, size_t col);

namespace Sipper
{ 
	class resultList {
	public:
		std::vector<std::string> red;
		std::vector<std::string> yellow;
	};

	void Reset();
	void Reevaluate();
	bool Check(std::string name);
	void SippNormal(dataset input);
	void SippOverwrite(dataset input);
	void SippNBL();
	resultList getList();
}