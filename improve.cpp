#include <string>
#include <iostream>
#include <windows.h>	 	//for utf-8 windows console output
#include <vector>
using namespace std;

bool testingdueexponential(int tim, int pnt) {
	bool dags = (time(0)-tim >= 2*pnt) && (rand() / (RAND_MAX + 1.))<0.4;
	return(dags);
}

bool testingdueoptimized(int tim, int pnt) { // optimerad
	bool dags = 0;
	if(pnt == 0){
		dags = 1;
	}else if(pnt < 140){
		dags = (time(0)-tim >= 140);
	}else if(pnt < 20000){
		dags = (time(0)-tim >= 30*pnt) || (time(0)-tim > 100000);
	}else if(pnt < 500000){
		dags = (time(0)-tim >= 8*pnt);
	}else{
		dags = (time(0)-tim >= 1.3*pnt) && (rand() / (RAND_MAX + 1.))<0.04;
	}
	return(dags);
}

bool testingduerandom(int tim, int pnt) { // random
	bool dags = (time(0)-tim >= 8*pnt) && (rand() / (RAND_MAX + 1.))<(float(pnt)/float(time(0)-tim)*8+0.01);
	return(dags);
}

int main() {
    SetConsoleOutputCP(65001);
	string in;
	
    //~ vector<string> languages = {"japanska", "kannada", "tyska", "rougon", "franska"};
	//~ vector<string> delimiters = {": ","\t" , ": ", "\t", ": "};
    vector<string> languages = {"schweizertyska"};
	vector<string> delimiters = {": "};
    //~ vector<string> languages = {"ryskaidag", "farsi"};
	//~ vector<string> delimiters = {"\t", ": "};
	int keepplaying = 1;
	bool opposite = 1;
	
	while(keepplaying){
	
	// Read knowledge data
    ifstream outfile("kunskap.txt");
    ofstream outfiletmp("kunskaptmp.txt");
    vector<string> inds;
    vector<int> pnts;
    vector<int> tims;
    string line;
	srand(time(0));
    string alfabet = "abcdefghijklmnopqrstuvwxyz";
    
    while(std::getline(outfile, line)){
	    int tabpos1 = line.find("\t");
	    int tabpos2 = tabpos1 + 1 + line.substr(tabpos1+1).find("\t");
	    string ind = line.substr(0, tabpos1);
	    int pnt = stoi(line.substr(tabpos1+1, tabpos2-tabpos1-1));
	    int tim = stoi(line.substr(tabpos2+1));
	    inds.push_back(ind);
	    pnts.push_back(pnt);
	    tims.push_back(tim);
		outfiletmp << ind << "\t" << pnt << "\t" << tim << endl;
	}
	outfile.close();
	outfiletmp.close();
	remove("kunskap.txt");
	rename("kunskaptmp.txt", "kunskap.txt");
	
	for(unsigned iFile=0; iFile<languages.size(); iFile++){
		if(keepplaying){
	    vector<string> words1;
	    vector<string> words2;
	    vector<int> indices;
	    vector<int> wordpnts;
	    vector<int> wordtims;
	    vector<string> wordinds;
	    string ind;
	    int num = 0;
	    vector <int> ofis;
	    string language = languages[iFile];
		string delimiter = delimiters[iFile];
		// Read language file
	    string langfilename = language + ".txt";
	    string langtmpname = language + "tmp.txt";
		cout << "loading " << langfilename << " ..." << endl;
	    ifstream infile(langfilename);
	    ofstream infiletmp(langtmpname);
	    
	    while(getline(infile, line)){
			if(line.size()>2){
				string nr3 = line.substr(3,1);
				if(nr3=="\t"){
					ind = line.substr(0,3);
					if(find(inds.begin(), inds.end(), ind)==inds.end()){
						outfiletmp << ind << "\t" << 0 << "\t" << time(NULL) << endl;
						inds.push_back(ind);
						pnts.push_back(0);
						tims.push_back(time(NULL));
						cout << "added new translation to kunskap: " << ind << endl;
					}
				}
				else{
					while(find(inds.begin(), inds.end(), ind)!=inds.end()){
						random_shuffle(alfabet.begin(), alfabet.end());
						ind = alfabet.substr(0,3);
					}
					line = ind + "\t" + line;
					outfiletmp << ind << "\t" << 0 << "\t" << time(NULL) << endl;
					inds.push_back(ind);
					pnts.push_back(0);
					tims.push_back(time(NULL));
					cout << "added new translation: " << line << endl;
				}
			    unsigned colonpos = line.substr(4).find(delimiter)+3;
			    if(colonpos == 2){
					cout << "delimiter \"" << delimiter << "\" not found in line " << num << ":  " << line << endl;
				}
			    string word1 = line.substr(4, colonpos-3);
			    string word2 = line.substr(colonpos+delimiter.size()+1);
			    if(opposite){
					string wordtmp = word2;
					word2 = word1;
					word1 = wordtmp;
				}
			    words1.push_back(word1);
			    words2.push_back(word2);
			    int ofi = find(inds.begin(), inds.end(), ind) - inds.begin();
			    ofis.push_back(ofi); // the mapping of rows from words1 etc. to kunskap.txt 
			    wordpnts.push_back(pnts[ofi]);
			    wordtims.push_back(tims[ofi]);
			    wordinds.push_back(inds[ofi]);
			    if(testingdueoptimized(wordtims[num], wordpnts[num])){
				    indices.push_back(num); // rows for entries to test in sprak.txt
				}
			    infiletmp << line << endl;
			    num++;
			}
		}
		infile.close();
		infiletmp.close();
		remove(langfilename.c_str());
		rename(langtmpname.c_str(), langfilename.c_str());
		
		unsigned numwords = indices.size();
		cout << indices.size() << " words require training" << endl;
		if(indices.size()>0){
			cout << "how many words?" << endl;
			getline(cin, in);
			if(in=="-"){in="0";}
			if(in!=""){
				numwords = stoi(in);
				indices.resize(numwords);
			}
		}
		else{
			cout << "quit? (:q) ";
			getline(cin, in);
			if(in==":q" || in=="'"){
				keepplaying=0;
			}
		}
		random_shuffle(indices.begin(), indices.end());
		
	    numwords = indices.size();
		cout << numwords << " words" << endl;
		string answer;
		string result;
	    int starttime = time(0);
	    unsigned wrongs = 0;
	    ofstream datafile;
	    datafile.open("data.txt", ios_base::app);
		for(unsigned int i = 0; i < indices.size(); i++){
			if(keepplaying){
				int wi = indices[i];
				int timepassed = time(0) - wordtims[wi];
				cout << words1[wi] << "\t";
				cout << wordpnts[wi] << "\t";
				cout << timepassed << endl;
				getline(cin, answer);
				cout << words2[wi] << "\t";
				
				if(answer==words2[wi] || answer=="'" || answer==""){
					result = "'";
				}
				else if(answer=="-" || answer=="/"){
					result = "-";
				}
				else if(answer==":q"){
					keepplaying = 0;
					result = "none";
					indices.resize(i);
					numwords = i;
				}
				else{
					cout << "rätt (')? ";
					getline(cin, result);
					if(result == ""){result = "'";}
					if(result == "/"){result = "-";}
				}
				
				if((result=="'" || result=="-") && (language=="franska" || language=="japanska" || language=="tyska" || language=="rougon" || language=="ryskaidag")){
					bool secondchance = (indices.size()>numwords);
					datafile << wordpnts[wi] << "\t" << timepassed << "\t" << result << "\t" << secondchance << "\t" << wordinds[wi] << endl;
				}
				
				if(result == "'"){
					wordpnts[wi] = max(wordpnts[wi], timepassed);
				}
				else if(result == "-"){
					wordpnts[wi] *=0.25;
					indices.push_back(wi);
					wrongs++;
				}
				wordtims[wi] = time(0);
				//~ cout << "pnts " << wordpnts[wi] << endl;
				cout << endl << endl;
			}
		}
		for(unsigned i=0; i<ofis.size(); i++){
			tims[ofis[i]] = wordtims[i];
			pnts[ofis[i]] = wordpnts[i];
		}
		
	    ofstream outfilenew("kunskaptmp.txt");
		for(unsigned i=0; i<inds.size(); i++){
			outfilenew << inds[i] << "\t" << pnts[i] << "\t" << tims[i] << endl;
		}
		outfilenew.close();
		remove("kunskap.txt");
		rename("kunskaptmp.txt", "kunskap.txt");
		
		int timespent = time(0)-starttime;
		cout << "used " << timespent << " s. (" << (float)timespent / (float)numwords << " s per word)" << endl;
		int correct = numwords - wrongs;
		cout << correct << " out of " << numwords << " correct (" << 100*(float)correct / (float)numwords << " %)" << endl << endl << endl;
		}
	}
	}
	
	
}
