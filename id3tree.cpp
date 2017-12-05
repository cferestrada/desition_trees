#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>
using namespace std;

typedef pair<int,map<string, int> > AttributeInnerMap;

struct node {
	struct node* parent;
	std::vector<struct node*> childrens;
	string name;
	int t;
	bool result;

};

double entropy(AttributeInnerMap x) {

	double sum = 0.0;
	for (std::map<string, int>::iterator ite = x.second.begin();ite != x.second.end(); ite++)
		if (ite->second > 0) 
			sum -= (double) ite->second / x.first * log((double) ite->second / x.first);
	return sum;
}

bool splitT(vector<string> index, map<string, vector<string> > in, vector<vector<string> > leafs) {

	AttributeInnerMap x;
	int resX = index.size()-1;
	x.first=leafs.size();

	for (int i = 0 , count = 0; i < in[index[resX]].size(); i++) {
		for (int j = 0; j < leafs.size(); j++){
			if (leafs[j][resX] == in[index[resX]][i]) 
				count++;
		}
		x.second[in[index[resX]][i]] = count;
	}

	if (entropy(x) != 0.0) 
		return true;
	else 
		return false;
}
double gain(AttributeInnerMap Set,vector<AttributeInnerMap> SubSetV) {

	double acum = entropy(Set);
	for(int i = 0; i < SubSetV.size(); i++)
		acum -= ((double)SubSetV[i].first / Set.first) * entropy(SubSetV[i]);
	return acum;
}


int next(vector<string> index, map<string, vector<string> > in, vector<vector<string> > leafs) {

	int temps = leafs.size();
	vector<AttributeInnerMap> y;
	AttributeInnerMap x, aux;
	x.first = temps;
	int ans = index.size()-1;
	double g = -1.0;
	int res = -1;

	for (int i = 0; i < in[index[ans]].size(); i++) {
		int count=0;
		for (int j = 0; j < leafs.size(); j++) 
			if (leafs[j][ans] == in[index[ans]][i]) count++;
		
		x.second[in[index[ans]][i]] = count;
	}

	for (int i = 0; i < index.size()-1; i++) {
		for (int j = 0; j < in[index[i]].size(); j++) {
			int inst = 0;
			for (int k = 0; k < in[index[ans]].size(); k++) {
				int count = 0;
				for (int n = 0; n < leafs.size(); n++) 
					if (leafs[n][ans] == in[index[ans]][k] && leafs[n][i] == in[index[i]][j]) count++;
				aux.second[in[index[ans]][k]] = count;
				inst += count;
			}
			aux.first = inst;
			y.push_back(aux);
			aux.second.clear();
		}
		double gainN = gain(x, y);
		y.clear();
		if(gainN > g) {
			g = gainN;
			res = i;
		}
	}
	return res;
}

void splits(struct node* Node,vector<string> index, map<string, vector<string> > in, vector<vector<string> > leafs, string level) {

	if (splitT(index, in, leafs)) {
		int index_split = next(index, in, leafs);
		vector<string> aux;
		vector<vector<string> > bleafs;
		Node->name = index[index_split];
		Node->t = 0;
		Node->result = false;
		int i;
		for (i = 0; i < in[index[index_split]].size(); i++) {
			struct node* child = new node;
			child->t = 1;
			child->parent = Node;
			child->name = in[index[index_split]][i];
			child->result = false;
			cout << level << Node->name << ": " << child->name << endl;
			Node->childrens.push_back(child);

			aux = index;
			aux.erase(aux.begin()+(index_split));
			for (int j = 0;j < leafs.size(); j++)
				if (leafs[j][index_split] == in[index[index_split]][i]) bleafs.push_back(leafs[j]);

			for (int j = 0; j < bleafs.size() ; j++)
				bleafs[j].erase(bleafs[j].begin()+index_split);

			splits(child, aux, in, bleafs, "  ");
			aux.clear();
			bleafs.clear();
		}
	} else {
		int index_answer = index.size() - 1;
		struct node* child = new node;
		child->t = 2;
		child->parent = Node;
		child->name = leafs[0][index_answer];
		child->result = true;
		cout << level << "  ANSWER: " << child->name << endl;
		Node->childrens.push_back(child);
	}
}




class ID3 {
	private:
		struct node* root;
		string action;
		vector<string> samplesT;
		vector<string> index;
		vector<string> action_values;
		vector<vector<string> > Samples;
		map<string, vector<string> > dictionary;
		
	public:
		ID3(vector<string> samples, vector<string> index_attribute, map<string,vector<string> > dictionary_map): samplesT(samples), index(index_attribute), dictionary(dictionary_map) {
			root = new node;
			root->t = 0;
			root->parent = NULL;
			root->result = false;
			
			for (int i = 0; i < samplesT.size(); i++) {
				for (int j = 0; j < samplesT[i].length(); j++) 
					if (samplesT[i][j] == ',') samplesT[i][j] = ' ';
				stringstream extract(samplesT[i]);
				string aux;
				vector<string> auxiliar;
				while (extract >> aux)
					auxiliar.push_back(aux);
				Samples.push_back(auxiliar);
				auxiliar.clear();
			}
			action = index[index.size()-1];
			for (int i = 0; i < dictionary[action].size(); i++) 
				action_values.push_back(dictionary[action][i]);
		}

	void CreateTree() {splits(root, index, dictionary, Samples, "");}
};

int main(int argc, char *argv[]) {

	string buff, x, proccess,aux;
	char devourer;
	char rm_chars[]="{}";
	std::map<string, vector<string> > in;
	std::vector<string> att;
	std::vector<string> t;
	std::vector<string> vstring;

	while (cin >> devourer) {
		if (devourer == '%') getline(cin, proccess);
		else break;
    }

	getline(cin,proccess);
	getline(cin,proccess);
	getline(cin,buff);
	
	
	while (buff.compare("") != 0) {
		stringstream extract(buff);
		extract >> x;
		if (x == "@attribute") {
			extract >> x;
		    aux = x;
		    att.push_back(aux);
		    buff = "";
		    while (extract >> x) buff += x;
	    	
		    for (int i = 0; i < strlen(rm_chars); i++)
	    		buff.erase (std::remove(buff.begin(), buff.end(), rm_chars[i]), buff.end());
	
			for (int i = 0; i < buff.length(); i++)
				if (buff[i]==',') buff[i]=' ';

			stringstream extract(buff);

			while (extract >> x)
				vstring.push_back(x);
	    
			in[aux]=vstring;

			while (!vstring.empty())
				vstring.pop_back();
			
		}
		getline(cin, buff);
	}

	while (getline(cin, buff)) {
		stringstream extract(buff);
		extract >> x;
		if (x == "@data") {
			while (getline(cin, buff)) {
				if (buff.find('%') != std::string::npos) 	continue;
				else t.push_back(buff);
			}
		}
	}
	
	ID3 tree(t, att, in);
	tree.CreateTree();
}