#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define maxengines 100
string engines[maxengines];
int num_p[maxengines];
int num_n[maxengines];
int numengines = 0;

int add_engine(string s) {
	string *ps;
	ps = std::find(engines, engines + maxengines, s);
	if (ps == engines + maxengines) {
		engines[numengines] = s;
		numengines++;
		return numengines - 1;
	}
	else
		return ps - engines;
}

int main(int argc, char* argv[])
{
	string line;
	//ifstream myfile("F:\\p\\c\\cutechess-cli\\randand82182_82180_40_02a - copia.pgn");
	ifstream myfile("F:\\p\\c\\cutechess-cli\\randand82184_82180_20_02a.pgn");

	const string white = "[White \"";
	const string black = "[Black \"";
	const string comment = "{";
	const string endcomment = "}";
	const string endevaluation = "/";
	string s;
#define novalue -9999
	double eval1 = novalue;

	int i, engine1, engine2;
	for (i = 0; i < maxengines; i++) {
		engines[i] = "";
		num_p[i] = 0;
		num_n[i] = 0;
	}

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			//if new game found add possible new players to the list
			if (line.rfind(white, 0) == 0) {
				eval1 = novalue;
				s = line.substr(white.length());
				s = s.substr(0, s.length() - 2);
				engine1 = add_engine(s);
				continue;
			}

			if (line.rfind(black, 0) == 0) {
				eval1 = novalue;
				s = line.substr(black.length());
				s = s.substr(0, s.length() - 2);
				engine2 = add_engine(s);
				continue;
			}

			if (line[0] == '[')
				continue;

			//find evaluations in the line
			do {
				std::size_t commentfound = line.find(comment);
				if (commentfound == std::string::npos)
					break;

				std::size_t endevaluationfound = line.find(endevaluation);
				if (endevaluationfound == std::string::npos)
					break;

				std::size_t endcommentfound = line.find(endcomment);
				if (endcommentfound == std::string::npos)
					break;

				if (endcommentfound < endevaluationfound) {
					//comment without evaluation, ignore
					line = line.substr(endcommentfound + 1);
					continue;
				}

				string saux = line.substr(commentfound + 1);

				if (saux.substr(1, 1) != "M") { //ignore mate value
					double eval = std::stod(saux);
					if (eval1 == novalue)
						eval1 = eval;
					else {
						eval = eval * -1;
						if (eval1 > 0 && eval > 0) {
							num_p[engine1] ++;
							num_n[engine2] ++;
						}
						if (eval1 < 0 && eval < 0) {
							num_n[engine1] ++;
							num_p[engine2] ++;
						}
						eval1 = novalue;
					}
				}
				line = line.substr(endcommentfound + 1);
			} while (true);
		}
		myfile.close();
	}

	for (i = 0; i < numengines; i++){
		cout << engines[i] << " positive: " << num_p[i] << " negative: " << num_n[i] << '\n';
	}

	system("pause");
	return 0;
}

