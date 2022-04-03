#include "Sipper.h"

#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <exception>

#include <comdef.h>
#include <cstdlib>

#include <windows.h>
#include "crackers.h"
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Sad.lib")

#import  "Analyze.tlb" raw_interfaces_only, raw_native_types, named_guids
#import  "DataAccess.tlb" raw_interfaces_only, raw_native_types, named_guids
#import  "DevAccess.tlb" raw_interfaces_only, raw_native_types, named_guids

using namespace CanberraSequenceAnalyzerLib;
using namespace CanberraDataAccessLib;
using namespace CanberraDeviceAccessLib;

namespace fs = std::filesystem;

/* OBJECTS */

ftpComm::ftpComm() {
	check = 0;
	std::ifstream f;
	std::string s;
	f.open("../ftp.txt");
	std::getline(f, s);
	check = stoi(s);
	f.close();
}

void ftpComm::transfer(std::string path, std::string File1, std::string File2) {
	std::ifstream f;
	std::ofstream log;
	std::string s;
	f.open(path);
	log.open("../log.txt");
	while (std::getline(f, s)) {
		if (s.find("FILE_1") != std::string::npos) {
			s.insert(s.find("FILE_1"), File1);
			s.erase(s.find("FILE_1"), 6);
		}
		if (s.find("FILE_2") != std::string::npos) {
			s.insert(s.find("FILE_2"), File2);
			s.erase(s.find("FILE_2"), 6);
		}
		log << s << std::endl; //std::system(s.c_str());
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	f.close();
	log.close();
}

void ftpComm::get(std::string s, std::string l) {
	transfer("../getFtp.txt", s, l);
}

void ftpComm::put(std::string l, std::string s) {
	transfer("../putFtp.txt", l, s);
}

void ftpComm::take() {
	std::ofstream loc;
	std::ifstream f;
	std::string s;

	while (fs::exists("../stop_loc.txt")) {
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
	loc.open("../stop_loc.txt");
	loc << 1;
	loc.close();

	bool taken = 1;
	get("stop.txt", "");
	f.open("../stop.txt");
	f >> s;
	f.close();
	taken = stoi(s);
	while (taken) {
		fs::remove("../stop.txt");
		std::this_thread::sleep_for(std::chrono::seconds(15));
		get("stop.txt", "stop.txt");

	}
	put("stop_loc.txt", "stop.txt");
}

void ftpComm::release() {
	std::ofstream loc;
	loc.open("../stop_loc.txt");
	loc << 0;
	loc.close();

	put("stop_loc.txt", "stop.txt");
	fs::remove("stop.txt");
	fs::remove("stop_loc.txt");
}

void ftpComm::download(std::string s, std::string l) {
	local = l;
	source = s;
	take();
	get(source, local);
}

void ftpComm::upload() {
	put(local, source);
	std::string s;
	s = "../" + local;
	fs::remove(s);
	release();
}

void transfer(std::string tmp, std::string server) {
	try {
		fs::copy_file(tmp, server, fs::copy_options::overwrite_existing);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		fs::remove(tmp);
	}
	catch (std::exception& e){}
}

std::string getcell(std::string line, size_t col) {
	std::string cell;
	size_t begin = 0;
	size_t end = 0;

	for (size_t i = 0; i < col - 1; i++) {
		begin = line.find(',', begin) + 1;
	}

	if (line.find(',', begin + 1) == std::string::npos) {
		end = line.length();
	}
	else {
		end = line.find(',', begin + 1);
	}

	cell = line.substr(begin, end - begin);
	return cell;
}

database databases;

/* NUCLID AND DATASET FUNCTIONS */

void library::getIsotopes() {
	std::ofstream file;
	ftpComm ftp;
	if (ftp.check) {
		file.open("./library.txt");
	}
	else {
		file.open(databases.server + "/library.txt");
	}

	CoInitialize(nullptr);
	HRESULT hr;
	IDataAccessPtr a;

	std::string lib;
	if (ftp.check) {
		ftp.download("sipper.nlb", "sipper.nlb");
		lib = "../sipper.nlb";
	}
	else {
		lib = databases.server + "/sipper.nlb";
	}
	_variant_t vVar;

	hr = a.CreateInstance(__uuidof(DataAccess));

	hr = a->Open((_bstr_t)lib.c_str(), dReadWrite, 0);
	int i = 1;
	while (S_OK == a->get_Param(CanberraDataAccessLib::CAM_T_NCLNAME, i, 0, &vVar))
	{
		std::wstring wn = vVar.bstrVal;
		std::string sn(wn.begin(), wn.end());
		isotopes.push_back(sn);
		file << sn << std::endl;
		i++;
	}
	hr = a->Close(dNoUpdate);
	file.close();
	if (ftp.check) {
		ftp.upload();
	}
};

void library::readIsotopes() {
	std::ifstream file;
	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get("library.txt", "library.txt");
		ftp.release();
		file.open("../library.txt");
	}
	else {
		file.open(databases.server + "/library.txt");
	}

	std::string line;
	while (std::getline(file, line)) {
		isotopes.push_back(line);
	}
	file.close();
}

library::library() {
		isotopes.clear();
}

void nuclid::getspectral(nuclid Nc, std::string line, int pos) {
	raw = stod(getcell(line, pos));
	background = Nc.raw;
	value = max(0, raw - background);
}

void nuclid::getbackground(nuclid Nc, std::string line, int pos) {
	raw = Nc.raw;
	background = stod(getcell(line, pos));
	value = max(0, raw - background);
}

dataset::dataset() {
	library lib;
	lib.readIsotopes();

	id = 0;
	time = 0.0;
	volume = 0.0;
	background = FALSE;

	for (size_t i = 0; i < lib.isotopes.size(); i++) {
		nuclid nuc;
		nuc.name = lib.isotopes[i];
		nuc.raw = 0.0;
		nuclides.push_back(nuc);
	}
}

void dataset::getDetector() {
	size_t split;
	split = detector.find('/');
	asf = "C:\\GENIE2K\\CTLFILES\\" + detector.substr(split + 1, detector.length());
	detector = detector.substr(0, split);
}

void dataset::input(std::string nameInput, double timeInput, double volumeInput, std::string det) {
	name = nameInput;
	detector = det;
	if (name.back() == 'b') {
		background = TRUE;
	}

	family = name;
	family.pop_back();

	time = timeInput;
	volume = volumeInput;
}

void dataset::analyze() {
	getDetector();
	std::string path = "C:\\GENIE2K\\CAMFILES\\" + name + ".CNF";

	CoInitialize(nullptr);
	HRESULT hr;
	IDataAccessPtr dat;
	ISequenceAnalyzerPtr seq;
	IDeviceAccessPtr det;
	short s = 0;
	DeviceStatus stON, stGO;

	hr = dat.CreateInstance(__uuidof(DataAccess));
	hr = seq.CreateInstance(__uuidof(SequenceAnalyzer));
	hr = det.CreateInstance(__uuidof(DeviceAccess));

	hr = det->Connect((_bstr_t) detector.c_str(), aReadWrite, aSpectralDetector, NULL, aUseSystemSettings);
	hr = det->AcquireStop(aNormalStop);
	hr = det->SpectroscopyAcquireSetup(aCountToLiveTime, time, 0, 0, 0.0, 0.0);
	hr = det->AcquireStart();
	hr = det->get_AnalyzerStatus(&stON);
	std::this_thread::sleep_for(std::chrono::seconds((long) time));
	hr = det->get_AnalyzerStatus(&stGO);
	while (stGO == stON) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		hr = det->get_AnalyzerStatus(&stGO);
	}
	hr = det->Save((_bstr_t) path.c_str(), FALSE);
	hr = det->Disconnect();

	hr = dat->Open((_bstr_t) path.c_str(), dReadWrite, 0);
	hr = seq->Analyze(dat, &s, (_bstr_t) asf.c_str(), VARIANT_FALSE, VARIANT_FALSE, VARIANT_FALSE, VARIANT_FALSE, NULL, NULL);
	hr = dat->Flush();
	int i = 1;
	_variant_t vVar;
	while (i <= nuclides.size() )
	{
		hr = dat->get_Param(CanberraDataAccessLib::CAM_G_NCLWTMEAN, i, 0, &vVar);
		nuclides[i-1].raw = vVar;
		nuclides[i - 1].raw = nuclides[i - 1].raw * 0.037;
		hr = dat->get_Param(CanberraDataAccessLib::CAM_G_NCLMDA, i, 0, &vVar);
		double mda = vVar;
		if (0.037*mda > nuclides[i-1].raw) {
			nuclides[i-1].raw = 0;
		}
		i++;
	}
	hr = dat->Close(dUpdate);

	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.put("C:/GENIE2K/CAMFILES/" + name + ".CNF", "/CNF/" + name + ".CNF");
		ftp.put("C:/GENIE2K/REPFILES/" + name + ".RPT", "/REP/" + name + ".RPT");
		ftp.release();
	}
	else {
		try {
			fs::copy_file("C:/GENIE2K/CAMFILES/" + name + ".CNF", databases.server + "/CNF/" + name + ".CNF",
				fs::copy_options::overwrite_existing);
		}
		catch (std::exception& e) {};
		try {
			fs::copy_file("C:/GENIE2K/REPFILES/" + name + ".RPT", databases.server + "/REP/" + name + ".RPT",
				fs::copy_options::overwrite_existing);
		}
		catch (std::exception& e) {};

	}
}

/* CSV FUNCTIONS */

void rawCSV::write(dataset input) {
	std::ofstream out;
	ftpComm ftp;
	if (ftp.check) {
		ftp.download(path, "tmpRW.csv");
		out.open("../tmpRW.csv", std::ios::app);
	}
	else {
		out.open(path, std::ios::app);
		while (!out.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			out.open(path, std::ios::app);
		}
	}

	out << input.family << "," << input.id << "," << input.name;
	for (size_t i = 0; i < input.nuclides.size(); i++) {
		out << "," << input.nuclides[i].raw;
	}
	out << std::endl;
	out.close();
	if (ftp.check) {
		ftp.upload();
	}
}

void rawCSV::overwrite(dataset input) {
	std::ifstream csv;
	std::ofstream tmp;
	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpRO.csv");
		csv.open("../tmpRO.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path, std::ios::app);
		}
	}

	tmp.open("../tmp.csv");

	std::string line;
	while (std::getline(csv, line)) {
		if (getcell(line, 3) == input.name) {
			tmp << input.family << "," << getcell(line, 2) << "," << input.name;
			for (size_t i = 0; i < input.nuclides.size(); i++) {
				tmp << "," << input.nuclides[i].raw;
			}
			tmp << std::endl;
		}
		else {
			tmp << line << std::endl;
		}
	}

	if (ftp.check) {
		ftp.put("tmp.csv", path);
		ftp.release();
		fs::remove("../tmpRO.csv");
	}
	else {
		transfer("../tmp.csv", path);
	}
}

void rawCSV::getVector() {
	vector.clear();
	std::ifstream csv;
	std::string line;

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpV.csv");
		ftp.release();
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path, std::ios::app);
		}
	}

	std::getline(csv, line);
	while (std::getline(csv, line)) {
		std::vector<double> val;
		int i = 4;
		while (i <= std::count(line.begin(), line.end(), ',')+1) {
			try {
				val.push_back(stod(getcell(line, i)));
			}
			catch (std::exception& e) {
				val.push_back(0);
			}
			i++;
		}
		vector.push_back(std::make_pair(getcell(line, 1), val));
	}
}

void rawCSV::reset(std::vector<std::string> isotopes) {
	{
		std::ofstream tmp;
		tmp.open("../tmp_raw.csv");
		tmp << "Family,ID,Name";
		for (size_t i = 0; i < isotopes.size(); i++) {
			tmp << "," << isotopes[i];
		}
		tmp << std::endl;
		tmp.close();

		ftpComm ftp;
		if (ftp.check) {
			ftp.take();
			ftp.put("../tmp_raw.csv", path);
			ftp.release();
		}
		else {
			transfer("../tmp_raw.csv", path);
		}
	}
}

bool rawCSV::check(std::string input) {
	std::ifstream csv;

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpRCh.csv");
		ftp.release();
		csv.open("../tmpRCh.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path);
		}
	}

	std::string line;
	while (std::getline(csv, line)) {
		if (getcell(line, 3) == input) {
			return true;
		}
	}
	return false;
}

int rawCSV::count() {
	int i = 0;
	std::ifstream csv;

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpRC.csv");
		ftp.release();
		csv.open("../tmpRC.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path);
		}
	}

	std::string line;
	std::getline(csv, line);
	while (std::getline(csv, line)) {
		i = max(i, stoi(getcell(line, 2)));
	}
	csv.close();
	return i;
}

void processedCSV::write(dataset input) {
	std::ofstream out;

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.download(path, "tmpPW.csv");
		out.open("../tmpPW.csv");
	}
	else {
		out.open(path, std::ios::app);
		while (!out.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			out.open(path, std::ios::app);
		}
	}

	out << input.family << "," << input.id << "," << input.name;
	for (size_t i = 0; i < input.nuclides.size(); i++) {
		out << "," << input.nuclides[i].raw << "," << input.nuclides[i].background << "," << input.nuclides[i].value;
	}
	out << ",XXX" << std::endl;
	out.close();
	if (ftp.check) {
		ftp.upload();
	}
}

void processedCSV::overwrite(std::vector<dataset> input) {
	std::ifstream csv;
	std::ofstream tmp;
	std::string line;

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpPO.csv");
		csv.open("../tmpPO.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path);
		}
	}
	
	tmp.open("../tmp.csv");

	while (std::getline(csv, line)) {
		bool found = false;
		for (size_t i = 0; i < input.size(); i++) {
			if (input[i].name == getcell(line, 3)) {
				found = true;
				tmp << input[i].family << "," << getcell(line, 2) << "," << input[i].name;
				for (size_t j = 0; j < input[i].nuclides.size(); j++) {
					tmp << "," << input[i].nuclides[j].raw 
						<< "," << input[i].nuclides[j].background 
						<< "," << input[i].nuclides[j].value;
				}
				tmp << ",XXX" << std::endl;
			}
		}
		if(!found) 
			tmp << line << std::endl;
	}

	csv.close();
	tmp.close();

	if (ftp.check) {
		ftp.put("tmp.csv", path);
		ftp.release();
		fs::remove("../tmpPO.csv");
	}
	else {
		transfer("../tmp.csv", path);
	}
}

void processedCSV::reset(std::vector<std::string> isotopes) {
	std::ofstream tmp;
	tmp.open("../tmp_proc.csv");
	tmp << "Family,ID,Name";
	for (size_t i = 0; i < isotopes.size(); i++) {
		tmp << "," << isotopes[i] << ".r," << isotopes[i] << ".b," << isotopes[i] << ".v";
	}
	tmp << ",XXX";
	for (size_t i = 0; i < isotopes.size(); i++) {
		tmp << "," << isotopes[i] << ".sigma," << isotopes[i] << ".alarm";
	}
	tmp << std::endl;
	tmp.close();

	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.put("tmp_proc.csv", path);
		ftp.release();
		fs::remove("../tmp_proc.csv");
	}
	else {
		transfer("../tmp_proc.csv", path);
	}
}

bool processedCSV::check(std::string input) {
	std::ifstream csv;
	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpPCh.csv");
		ftp.release();
		csv.open("../tmpPCh.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path);
		}
	}

	std::string line;
	while (std::getline(csv, line)) {
		if (getcell(line, 3) == input) {
			return true;
		}
	}
	return false;
}

int processedCSV::count() {
	int i = 0;
	std::ifstream csv;
	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpPC.csv");
		ftp.release();
		csv.open("../tmpPC.csv");
	}
	else {
		csv.open(path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(path);
		}
	}

	std::string line;
	std::getline(csv, line);
	while (std::getline(csv, line)) {
		i = max(i, stoi(getcell(line, 2)));
	}
	csv.close();
	return i;
}

/* STATS FUNCTIONS */

void stats::update() {
	std::ofstream txt;
	ftpComm ftp;
	if (ftp.check) {
		ftp.download(path, "tmpST.txt");
		txt.open("../tmpST.txt");
	}
	else {
		txt.open(path);
		while (!txt.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			txt.open(path);
		}
	}

	txt << S << std::endl;
	txt << Sx << std::endl;
	txt << Sy << std::endl;
	txt << Sxx << std::endl;
	txt << Syy << std::endl;
	txt << Sxy << std::endl;
	txt.close();
	if (ftp.check) {
		ftp.upload();
	}
}

void stats::import() {
	std::ifstream txt;
	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(path, "tmpSTi.txt");
		ftp.release();
		txt.open("../tmpSTi.txt");
	}
	else {
		txt.open(path);
		while (!txt.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			txt.open(path);
		}
	}

	txt >> S;
	txt >> Sx;
	txt >> Sy;
	txt >> Sxx;
	txt >> Syy;
	txt >> Sxy;
	txt.close();
}

void stats::evaluate(dataset set, nuclid Nc) {
	import();
	S = S + 1;
	Sx = Sx + set.id;
	Sy = Sy + Nc.value;
	Sxx = Sxx + (double)set.id * (double)set.id;
	Syy = Syy + Nc.value * Nc.value;
	Sxy = Sxy + Nc.value * (double)set.id;
	if (S * Sxx - Sx * Sx != 0) {
		a = (S * Sxy - Sx * Sy) / (S * Sxx - Sx * Sx);
		b = (Sxx * Sy - Sx * Sxy) / (S * Sxx - Sx * Sx);
	}
	update();
}

void stats::getreg() {
	a = (S * Sxy - Sx * Sy) / (S * Sxx - Sx * Sx);
	b = (Sxx * Sy - Sx * Sxy) / (S * Sxx - Sx * Sx);
}

void stats::getsigma() {
	sigma2 = nMSE / S;
}

void stats::sumnMSE(std::string line, int pos_x, int pos_y) {
	nMSE = nMSE + error2(stod(getcell(line, pos_x)), stod(getcell(line, pos_y)));
}

double stats::reg(double x) {
	return a * x + b;
}

double stats::error2(double x, double y) {
	return (y - reg(x)) * (y - reg(x));
}

int stats::getalarm(double err) {
	if (err > sqrt(sigma2)) {
		if (err > 3 * sqrt(sigma2)) return 2;
		else return 1;
	}
	else return 0;
}

void stats::reset() {
	std::ofstream tmp;
	tmp.open("../tmp.txt");
	tmp << 0 << std::endl;
	tmp << 0 << std::endl;
	tmp << 0 << std::endl;
	tmp << 0 << std::endl;
	tmp << 0 << std::endl;
	tmp << 0 << std::endl;
	tmp.close();

	transfer("../tmp.txt", path);
}

void stats::reevaluate(std::string line) {
	if (stod(getcell(line, 6 + id * 3)) > 0) {
		S = S + 1;
		Sx = Sx + stod(getcell(line, 2));
		Sy = Sy + stod(getcell(line, 6 + id * 3));
		Sxx = Sxx + stod(getcell(line, 2)) * stod(getcell(line, 2));
		Syy = Syy + stod(getcell(line, 6 + id * 3)) * stod(getcell(line, 6 + id * 3));
		Sxy = Sxy + stod(getcell(line, 6 + id * 3)) * stod(getcell(line, 2));
	}
}

/* DATA AND DATABASE FUNCTIONS */

void data::findBackground(database source) {

	std::ifstream csv;
	std::string line;
	analyzed;
	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(source.background.path, "tmpBf.csv");
		ftp.release();
		csv.open("../tmpBf.csv");
	}
	else {
		csv.open(source.background.path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(source.background.path);
		}
	}

	while (std::getline(csv, line)) {
		if (line.substr(0, line.find(",")) == measured.family) {
			dataset tmp;
			tmp.family = measured.family;
			tmp.id = measured.id;
			tmp.name = measured.name;
			for (size_t i = 0; i < tmp.nuclides.size(); i++) {
				tmp.nuclides[i].getbackground(measured.nuclides[i], line, 4 + i);
			}
			analyzed.push_back(tmp);
		}
	}
	csv.close();
}

void data::findSpectral(database source) {

	std::ifstream csv;
	std::ofstream copy;
	std::string line;
	analyzed;
	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(source.spectral.path, "tmpSf.csv");
		csv.open("../tmpSf.csv");
	}
	else {
		csv.open(source.spectral.path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(source.spectral.path);
		}
	}

	copy.open("../copy.csv");
	while (std::getline(csv, line)) {
		if (line.substr(0, line.find(",")) == measured.family) {
			dataset tmp;
			tmp.family = measured.family;
			tmp.id = stoi(getcell(line, 2));
			tmp.name = getcell(line, 3);
			for (size_t i = 0; i < tmp.nuclides.size(); i++) {
				tmp.nuclides[i].getspectral(measured.nuclides[i], line, 4 + i);
			}
			analyzed.push_back(tmp);
		}
		else {
			copy << line << std::endl;
		}
	}
	csv.close();
	copy.close();
	
	if (ftp.check) {
		ftp.put("copy.csv", source.spectral.path);
		ftp.release();
		fs::remove("../copy.csv");
	}
	else {
		transfer("../copy.csv", source.spectral.path);
	}

}

void database::getServer() {
	std::ifstream file;
	file.open("../server.txt");
	while (!file.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		file.open("../server.txt");
	}
	server.clear();
	std::getline(file, server);
	file.close();
}

database::database() {
	getServer();
	statsFolder = server + "/stats/";
	library lib;
	lib.readIsotopes();
		for (size_t i = 0; i < lib.isotopes.size(); i++) {
			stats nuclide;
			nuclide.id = i;
			nuclide.path = statsFolder + lib.isotopes[i] + ".txt";
			nuclide.nMSE = 0.0;
			nuclides.push_back(nuclide);
		}

		spectral.path = server + "/spectral.csv";
		background.path = server + "/background.csv";
		analyzed.path = server + "/analyzed.csv";
}

bool database::check(std::string input) {
	return !(!analyzed.check(input) * !spectral.check(input) * !background.check(input));
}

void database::evaluate(dataset set) {
	for (size_t i = 0; i < nuclides.size(); i++) {
		nuclides[i].evaluate(set, set.nuclides[i]);
	}
}

void database::bias() {
	std::ifstream csv;
	std::string line;
	ftpComm ftp;

	for (size_t i = 0; i < nuclides.size(); i++) {
		nuclides[i].nMSE = 0;
	}

	if (ftp.check) {
		ftp.take();
		ftp.get(analyzed.path, "tmpB.csv");
		ftp.release();
		csv.open("../tmpB.csv");
	}
	else {
		csv.open(analyzed.path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(analyzed.path);
		}
	}


	std::getline(csv, line);
	while (std::getline(csv, line)) {
		for (size_t i = 0; i < nuclides.size(); i++) {
			if (stod(getcell(line, 6 + i * 3)) > 0) {
				nuclides[i].sumnMSE(line, 2, 6 + 3 * i);
			}
		}
	}
	csv.close();
	for (size_t i = 0; i < nuclides.size(); i++) {
		nuclides[i].getsigma();
	}
}

void database::results() {
	std::ifstream oldcsv;
	std::ofstream newcsv;
	std::string line;
	double error = 0.0;
	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(analyzed.path, "tmpR.csv");
		oldcsv.open("../tmpR.csv");
	}
	else {
		oldcsv.open(analyzed.path);
		while (!oldcsv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			oldcsv.open(analyzed.path);
		}
	}

	newcsv.open("../tmp.csv");
	std::getline(oldcsv, line);
	newcsv << line << std::endl;
	while (std::getline(oldcsv, line)) {
		newcsv << line.substr(0, line.find("XXX")) << "XXX";
		for (size_t i = 0; i < nuclides.size(); i++) {
			if (stod(getcell(line, 6 + 3 * i)) > 0) {
				error = sqrt(nuclides[i].error2(stod(getcell(line, 2)), stod(getcell(line, 6 + 3 * i))));
				newcsv << "," << error << "," << nuclides[i].getalarm(error);
			}
			else {
				newcsv << ",0,0";
			}
		}
		newcsv << std::endl;
	}
	oldcsv.close();
	newcsv.close();

	if (ftp.check) {
		ftp.put("tmp.csv", analyzed.path);
		ftp.release();
		fs::remove("../tmp.csv");
	}
	else {
		transfer("../tmp.csv", analyzed.path);
	}

}

void database::reset(std::vector<std::string> isotopes) {
	spectral.reset(isotopes);
	background.reset(isotopes);
	analyzed.reset(isotopes);
	nuclides.clear();
	fs::remove_all(statsFolder);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	fs::create_directory(statsFolder);
	fs::remove_all(databases.server + "/REP/");
	fs::remove_all(databases.server + "/CNF/");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	fs::create_directory(databases.server + "/REP/");
	fs::create_directory(databases.server + "/CNF/");
	for (size_t i = 0; i < isotopes.size(); i++) {
		stats nuc;
		nuc.path = statsFolder + isotopes[i] + ".txt";
		nuc.id = i;
		nuclides.push_back(nuc);
		nuclides[i].reset();
	}
	std::ofstream out;
	out.open(server + "/id.txt");
	out << 0;
	out.close();
}

void database::reevaluate() {
	for (int i = 0; i < nuclides.size(); i++) {
		nuclides[i].reset();
		nuclides[i].import();
	}
	
	std::string line;

	background.getVector();

	std::ifstream out;
	std::ofstream tmp;
	std::ofstream in;

	out.open(analyzed.path);
	while (!out.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		out.open(analyzed.path);
	}
	in.open(spectral.path, std::ios_base::app);
	while (!in.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		in.open(spectral.path, std::ios::app);
	}
	tmp.open("../tmp.csv");

	std::getline(out, line);
	tmp << line << std::endl;
	while (std::getline(out, line)) {
		bool found = false;
		size_t it = 0;
		while(it < background.vector.size() && !background.vector.empty()) {
			if (background.vector[it].first == getcell(line, 1)) {
				found = true;
				tmp << getcell(line, 1) << "," << getcell(line, 2) << "," << getcell(line, 3);
				for (size_t nuc = 0; nuc < background.vector[it].second.size(); nuc++) {
					tmp << "," << getcell(line, 4 + 3 * nuc) << "," << background.vector[it].second[nuc] << ","
						<< max(0, stod(getcell(line, 4 + 3 * nuc)) - background.vector[it].second[nuc]);
				}
				tmp << ",XXX" << std::endl;
				break;
			}
			it++;
		}
		if (!found) {
			in << getcell(line, 1) << "," << getcell(line, 2) << "," << getcell(line, 3);
			int cell = 4;
			for(size_t i = 0; i < nuclides.size(); i++) {
				in << "," << getcell(line, cell + 3*i);
			}
			in << std::endl;
		}
	}

	out.close();
	in.close();
	tmp.close();

	transfer("../tmp.csv", analyzed.path);

	out.open(spectral.path);
	while (!out.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		out.open(spectral.path);
	}
	in.open(analyzed.path, std::ios_base::app);
	while (!in.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		in.open(analyzed.path, std::ios_base::app);
	}
	tmp.open("../tmp.csv");
	std::getline(out, line);
	tmp << line << std::endl;
	while (std::getline(out, line)) {
		bool found = false;
		for (size_t i = 0; i < background.vector.size() && !background.vector.empty(); i++) {
			if (background.vector[i].first == getcell(line, 1)) {
				found = true;
				in << getcell(line, 1) << "," << getcell(line, 2) << "," << getcell(line, 3);
				for (size_t n = 0; n < background.vector[i].second.size(); n++) {
					in << "," << getcell(line, n + 4) << "," << background.vector[i].second[n];
					in << "," << max(0, stod(getcell(line, n + 4)) - background.vector[i].second[n]);
				}
				in << ",XXX" << std::endl;
				break;
			}
		}
		if (!found) {
			tmp << line << std::endl;
		}
	}
	out.close();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	in.close();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	tmp.close();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	transfer("../tmp.csv", spectral.path);

	int i = 0;
	i = max(spectral.count(), max(background.count(), analyzed.count()));

	std::ofstream id;
	id.open(server + "/id.txt");
	while (!id.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(30));
		id.open(server + "/id.txt");
	}
	id << i;
	id.close();

	out.open(analyzed.path);
	std::getline(out, line);
	while (std::getline(out, line)) {
		for (size_t i = 0; i < nuclides.size(); i++) {
			nuclides[i].reevaluate(line);
		}
	}
	out.close();
	for (size_t i = 0; i < nuclides.size(); i++) {
		nuclides[i].getreg();
		nuclides[i].update();
	}
	bias();
	results();
}

/* MAIN FUNCTIONS */

void Sipper::Reset() {
	library lib;
	lib.getIsotopes();
	databases.reset(lib.isotopes);
}

void Sipper::Reevaluate() {
	databases.reevaluate();
}

bool Sipper::Check(std::string name) {
	return databases.check(name);
}

void Sipper::SippNormal(dataset input) {
	data sample;
	sample.measured = input;

	std::ifstream in;
	ftpComm ftp;
	
	if (ftp.check) {
		ftp.download(databases.server + "/id.txt", "tmpID.txt");
		in.open("../tmpID.txt");
	}
	else {
		in.open(databases.server + "/id.txt");
		while (!in.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			in.open(databases.server + "/id.txt");
		}
	}

	in >> sample.measured.id;
	in.close();
	sample.measured.id++;
	std::ofstream out;
	if (ftp.check) {
		out.open("../tmpID.txt");
	}
	else {
		out.open(databases.server + "/id.txt");
		while (!out.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			out.open(databases.server + "/id.txt");
		}
	}
	out << sample.measured.id;
	out.close();
	if (ftp.check) {
		ftp.upload();
	}

	if (sample.measured.background) {
		databases.background.write(sample.measured);
		sample.findSpectral(databases);
	}
	else {
		sample.findBackground(databases);
		if (sample.analyzed.empty()) {
			databases.spectral.write(sample.measured);
		}
	}
	if (sample.analyzed.size() != 0) {
		for (size_t i = 0; i < sample.analyzed.size(); i++) {
			databases.analyzed.write(sample.analyzed[i]);
			for (size_t n = 0; n < databases.nuclides.size(); n++) {
				databases.nuclides[n].evaluate(sample.analyzed[i], sample.analyzed[i].nuclides[n]);
			}
		}
		databases.bias();
		databases.results();
	}
}

void Sipper::SippOverwrite(dataset input) {
	data sample;
	sample.measured = input;

	if (sample.measured.background) {
		databases.background.overwrite(sample.measured);
	}
	else {
		sample.findBackground(databases);
		if (sample.analyzed.empty()) {
			databases.spectral.overwrite(sample.measured);
		}
		else {
			databases.analyzed.overwrite(sample.analyzed);
		}
	}

	databases.reevaluate();
}

void Sipper::SippNBL() {
	ftpComm ftp;
	if (ftp.check) {
		ftp.take();
		ftp.get(databases.server + "/SIPPER.NLB", "C:/GENIE2K/CAMFILES/SIPPER.NLB");
		ftp.release();
	}
	else {
		try {
			fs::copy_file(databases.server + "/SIPPER.NLB", "C:/GENIE2K/CAMFILES/SIPPER.NLB", fs::copy_options::overwrite_existing);
		}
		catch (std::exception& e) {}
	}
}

Sipper::resultList Sipper::getList() {
	std::ifstream csv;
	std::string line;
	resultList result;

	library lib;
	lib.readIsotopes();

	int pos = 3 + 3 * lib.isotopes.size() + 3;

	ftpComm ftp;

	if (ftp.check) {
		ftp.take();
		ftp.get(databases.analyzed.path, "tmpL.csv");
		ftp.release();
		csv.open("../tmpL.csv");
	}
	else {
		csv.open(databases.analyzed.path);
		while (!csv.is_open()) {
			std::this_thread::sleep_for(std::chrono::seconds(30));
			csv.open(databases.analyzed.path);
		}
	}

	std::getline(csv, line);
	while (std::getline(csv, line)) {
		int color = 0;
		for (size_t i = 0; i < lib.isotopes.size(); i++) {
			if (color < stoi(getcell(line, pos + i * 2)))
				color = stoi(getcell(line, pos + i * 2));
		}

		if (color == 2) {
			result.red.push_back(getcell(line, 3));
		}
		else {
			if (color == 1)
				result.yellow.push_back(getcell(line, 3));
		}
	}

	return result;
}
