/*
Name - Parth Goyal
Entry Number - 2017csb1095
Date of Submission - 10/09/18
Submitted To - Dr. Neeraj Goel
*/

// #include <iostream>
// #include <fstream>
// #include <iostream>
// #include <fstream>
// #include <string.h>
// #include <cstdlib>
// #include <cmath>
// #include <set>
// #include <stack>
// #include <list>
// #include <vector>
// #include <bitset>
// #include <cmath>
// #include <algorithm>
#include <bits/stdc++.h>

using namespace std;

struct Implicant
{
	int* array; //integer array of size in sorted order
	int size;
	int flag1;
	Implicant(int _size) {
	size = _size;
	//allocate size*sizeof(int) memory to array
	array = new int[size];
	}
	Implicant(int _size, Implicant i1, Implicant i2) {
		//create new implicant with size. i1.array and i2.array are merged, sorted and added to array
		this->size = _size;
		array = new int[size];
		int implicant_size = i1.size;
		unsigned int difference = (i2.array[0] - i1.array[0]);
		for (int i = 1; i < implicant_size; i++){
			unsigned int dif = (i2.array[i] - i1.array[i]);
			if(difference != dif){
				flag1 = 0;
				return;
			}
		}
		int i = 0;
		while(difference > pow(2,i)){
			i++;
		}
		if(difference == pow(2,i)){
			flag1 = 1;
			int n = _size/2;
			for(int i = 0; i < n; i++){
				this->array[i] = i1.array[i];
				this->array[i+n] = i2.array[i]; 
			}
			sort(this->array, this->array + 2*n);
		}
		else{
			flag1 = 0;
		}
	}
	void resize(int size){
		this->size = size;
		array = new int[size];
	}
	bool operator == (const Implicant &second){
		if(this->size != second.size){
			return false;
		}
		else{
			for (int i = 0; i < this->size; i++){
				if(this->array[i] != second.array[i]){
					return false;
				}
			}
			return true;
		}
	}
};

struct Bucket 
{
	list <Implicant> implicants;
};

struct BucketStore
{
	stack <Bucket*> store;
};

struct Node
{
	char alpha;
	int index;
	int present;
};

int flag = 0;
void readfromFile();
void POS(char *carrInput, char *carrOutput, int inSize, int outSize, string line);
void SOP(char *carrInput, char *carrOutput, int inSize, int outSize, string line);
void MinSOP(Node *array, int *minterms, int inSize);
int sumBinary(Node *array, int inSize);
void SOP_final(Node *array, int *minterms, int inSize);
bool alreadyPresent(list<Implicant> implicants, Implicant e);
bool CheckSuperset(Bucket prime_implicants, Implicant i1);
void GenerateSOP(bitset<15> *set0, int size, char *carrInput);
void PrintSOP(int array1[], int size, char *carrInput);
void FindingImplicants(list<int> minterms, char *carrInput);

ofstream outMin("Output_QuineM.txt"); 

void PrintSOP(int array1[], int size, char *carrInput)
{
	int length = strlen(carrInput);
	Node node[length];
	for (int i = 0; i < length; i++){
		node[i].alpha = carrInput[i];
		node[i].index = 5-i;
	}
	for (int i = 0; i < length; i++){
		if(array1[i] == -1){
			continue;
		}
		else if(array1[i] == 1){
			outMin << node[i].alpha;
			cout << node[i].alpha;
		}
		else{
			outMin << node[i].alpha << "'";
			cout << node[i].alpha << "'";
		}
	}
}

void GenerateSOP(bitset<15> *set0, int size, char *carrInput)
{
	int length = strlen(carrInput);
	int array1[length] = {0};
	for (int j = 0; j < length; j++){
		int a = set0[0][j];
		for (int i = 1; i < size; i++){
			if(set0[i][j] != a){
				array1[j] = -1;
			}
		}
		if(array1[j] != -1){
			array1[j] = a; 
		}
	}
	PrintSOP(array1, size, carrInput);
}

bool CheckSuperset(Bucket prime_implicants, Implicant i1)
{
	int size1 = prime_implicants.implicants.size();
	int size2 = i1.size;
	int count = 0;
	for(auto it = prime_implicants.implicants.begin(); it != prime_implicants.implicants.end(); it++ ){
		for(int i = 0; i < size2; i++){
			for(int j = 0; j < it->size; j++){
				if(i1.array[i] == it->array[j]){
					count++;
				}
			}
		}
		if(count == size2){
			return true;
		}
		count = 0;
	}
	return false;
}

bool alreadyPresent(list<Implicant> implicants, Implicant e)
{
	int flag2 = 0;
	int count = 0;
	int n = e.size;
	for(auto it = implicants.begin(); it != implicants.end(); it++){
		for (int i = 0; i < n; i++){
			if((*it).array[i] == e.array[i]){
				count++;
			}
		}
		if (count == n){
			return true;
		}
		else{
			count = 0;
		}
	}
	if(flag2 == 0){
		return false;
	}
	return true;
}

set <int> st;
int num_min = 0;

void FindingImplicants(list<int> minterms, char *carrInput)
{
	int largest_minterm = minterms.back();
	int size_of_bucket = ceil(log2(largest_minterm)) + 1;
	Bucket *pBucket[size_of_bucket];
	const int M = ceil(log2(largest_minterm));
	for(int i = 0; i < size_of_bucket; i++){
		pBucket[i]= new Bucket;
	}
	for (auto i = minterms.begin(); i!= minterms.end(); i++){
		bitset<15> set0(*i);
		int numberof1 = set0.count();
		Implicant e(1);
		e.array[0] = *i;
		pBucket[numberof1]->implicants.push_back(e);
	}
	BucketStore storex;
	for (int i = 0; i < size_of_bucket; i++){
		storex.store.push(pBucket[i]);
	}
	while(1){
		int count = 0;
		Bucket *nBucket[--size_of_bucket];
		for(int i = 0; i < size_of_bucket; i++){
			nBucket[i]= new Bucket;
		}
		
		for (int i = 0; i < size_of_bucket; i++){
			for(auto it = (pBucket[i]->implicants.begin()); it != ((pBucket[i]->implicants).end()); it++ ){
				int size = 2*(*it).size;
				for (auto it2 = (pBucket[i+1]->implicants.begin()); it2 != (pBucket[i+1]->implicants.end()); it2++){
					Implicant e(size, *it, *it2);
					if((e.flag1 == 1) && !(alreadyPresent(nBucket[i]->implicants, e))){
						nBucket[i]->implicants.push_back(e);
						cout << "Array =  "; 
						for (int j = 0; j < e.size ; j++){
							cout << e.array[j] << " ";
						}
						cout << endl;
						count++;
					}
				}
			}
			cout << "--------------------------------------" << nBucket[i]->implicants.size() << endl;
		}
		for (int i = 0; i < size_of_bucket; i++){
			storex.store.push(nBucket[i]);
			pBucket[i] = new Bucket;
			pBucket[i] = nBucket[i];
		}
		if(count <= 1)
			break;
	}


	int sizeofstack = storex.store.size();
	Bucket prime_implicants;
	while(!storex.store.empty()){
		Bucket *e = new Bucket;
		e = storex.store.top();
		storex.store.pop();
		for (auto it = e->implicants.begin(); it != e->implicants.end(); it++){
			if(prime_implicants.implicants.size() == 0){
				prime_implicants.implicants.push_back(*it);
			}
			else{
				if (!CheckSuperset(prime_implicants, *it)){
					prime_implicants.implicants.push_back(*it);
				}
			}
		}
	}
	for (auto it = prime_implicants.implicants.begin(); it != prime_implicants.implicants.end(); it++){
		cout << "Prime Implicant : ";
		for (int i = 0; i < it->size; i++ ){
			cout << it->array[i] << " ";
		}
		cout << endl;
	}

	Bucket ess_prime_implicants;
	Implicant e(1);
	for (auto it1 = minterms.begin(); it1 != minterms.end(); it1++){
		int count = 0;
		for (auto it2 = prime_implicants.implicants.begin(); it2 != prime_implicants.implicants.end(); it2++){
			for(int i = 0; i < it2->size; i++){
				if(it2->array[i] == *it1){
					e.resize(it2->size);
					e = *it2;
					count++;
					break;
				}
			}
		}
		if((count == 1) && (!alreadyPresent(ess_prime_implicants.implicants, e)) ){
			ess_prime_implicants.implicants.push_back(e);
		}
		count = 0;
	}
	for (auto it = ess_prime_implicants.implicants.begin(); it != ess_prime_implicants.implicants.end(); it++){
		cout << "Essential Prime Implicant : ";
		for (int i = 0; i < it->size; i++){
			cout << it->array[i] << " ";
			minterms.remove(it->array[i]);
		}
		cout << endl;
	}
	if(minterms.size() != 0){
			cout << "Left minterms: ";
			for (auto it = minterms.begin(); it != minterms.end(); it++){
				cout << *it << " ";
			}
			cout << endl << endl;
	}

	while(minterms.size() != 0){
		int _size = prime_implicants.implicants.size();
		int terms[_size] = {0};
		int j = 0; 
		for (auto it = prime_implicants.implicants.begin(); it != prime_implicants.implicants.end(); it++){
			int count = 0;
			for (auto it2 = minterms.begin(); it2 != minterms.end(); it2++){
				for (int i = 0; i < it->size; i++){
					if(*it2 == it->array[i]){
						count++;
					}
				}
			}
			terms[j] = count;
			j++;
		}
		int max = terms[0];
		int max_index = 0;
		for (int j =1; j<_size; j++){
			if(terms[j] > max){
				max = terms[j];
				max_index = j;
			}
		}
		std:: list <Implicant> :: iterator it7 = prime_implicants.implicants.begin();
		advance(it7, max_index);
		if(!alreadyPresent(ess_prime_implicants.implicants, *it7)){
			ess_prime_implicants.implicants.push_back(*it7);
			prime_implicants.implicants.remove(*it7);
		}
		for (auto it = ess_prime_implicants.implicants.begin(); it != ess_prime_implicants.implicants.end(); it++){
			cout << "Essential Prime Implicant : ";
			for (int i = 0; i < it->size; i++){
				cout << it->array[i] << " ";
				minterms.remove(it->array[i]);
			}
			cout << endl;
		}
		if(minterms.size() != 0){
			cout << "Left minterms: ";
			for (auto it = minterms.begin(); it != minterms.end(); it++){
				cout << *it << " ";
			}
			cout << endl << endl;
		}
	}
	
	int size2 = ess_prime_implicants.implicants.size();
	int j = 0;
	for(auto it = ess_prime_implicants.implicants.begin(); it != ess_prime_implicants.implicants.end(); it++){
		
		bitset<15> set0[it->size];
		for(int i = 0; i < it->size; i++){
			bitset<15> set1(it->array[i]);
			set0[i] = set1;
		}
		j++;
		GenerateSOP(set0, it->size, carrInput);
		if(j < size2){
			outMin << " + ";
			cout << " + ";
		}
	}
}

void SOP_final(Node *array, int *minterms, int inSize)
{
	int iFlag = 0;
	int i = 0;
	for(i = 0; i < inSize; i++){
		if(array[i].present == -1){
			iFlag = 1;
		}
	}
	if(iFlag == 0){
		minterms[num_min++] = sumBinary(array, inSize);
		return;
	}
	Node temp[inSize];
	for(i = 0; i < inSize; i++){
		temp[i] = array[i];
	}
	for(i = 0; i < inSize; i++){
		if(array[i].present == -1){
			array[i].present = 0;
			SOP_final(array, minterms, inSize);
			temp[i].present = 1;
			SOP_final(temp, minterms, inSize);
		}
	}
}

int sumBinary(Node *array, int inSize)
{
	int i = 0;
	int sum = 0;
	for (i = 0; i < inSize; i++){
		if(array[i].present == 1){
			sum += pow(2,array[i].index);
		}
	}
	return sum;
}

void MinSOP(Node *array, int *minterms, int inSize)
{
	int i = 0;
	int count = 0;
	for(i = 0 ; i < inSize; i++){
		if(array[i].present == -1){
			count ++;
		}
	}
	
	if(count == 0){
		minterms[num_min] = sumBinary(array, inSize);
		num_min++;
		for (i = 0; i < num_min ; i++){
			st.insert(minterms[i]);
		}	
		return;
	}
	SOP_final(array, minterms, inSize);
	for (i = 0; i < num_min ; i++){
		st.insert(minterms[i]);
	}

}

void POS(char *carrInput, char *carrOutput, int inSize, int outSize, string line)
{
	int i = 0;
	int j = 0;
	int terms = 0;
	while(line[i] != '\0'){
		if(line[i] == ')'){
			terms++;
		}
		i++;
	}
	i = 3;
	Node array[terms][inSize];
	int minterms[1000];
	for(j = 0; j < 1000; j++){
		minterms[j] = -1;
	}
	j = 0;
	while(line[i] != '\0'){
		if((line[i] == '(') || (line[i] == ')')){
			
			i++;
			continue;
		}
		while(line[i] != ')'){
			int k = 0;
			while(k < inSize){
				if((line[i] == ' ') || (line[i] == '+')){
					i++;
					continue;
				}
				if((line[i] == carrInput[k]) && (line[i+1] != '\'')){
					array[j][k].alpha = carrInput[k];
					array[j][k].index = k;
					array[j][k].present = 0;
					k++;
					i++;
				}
				else if((line[i] == carrInput[k]) && (line[i+1] == '\'')){
					array[j][k].alpha = carrInput[k];
					array[j][k].index = k;
					array[j][k].present = 1;
					k++;
					i = i + 2;
				}
				else if(line[i] != carrInput[k]){
					array[j][k].alpha = carrInput[k];
					array[j][k].index = k;
					array[j][k].present = -1;
					k++;
				}
			}
			MinSOP(array[j], minterms, inSize);
			j++;
		}

	}
}

void SOP(char *carrInput, char *carrOutput, int inSize, int outSize, string line)
{
	int j = 0;
	int terms = 0;
	while(line[j] != '\0'){
		if(line[j] == '+'){
			terms++;
		}
		j++;
	}
	terms = terms + 1;
	int i = 3;
	j = 0;
	Node array[terms][inSize];
	int f = pow(2, inSize);
	int minterms[1000];
	for(j = 0; j < 1000; j++){
		minterms[j] = -1;
	}
	j = 0;

	while(line[i] != '\0'){
		if((line[i] == ' ') || (line[i] == '+')){
			i++;
			continue;
		}
		else{
			while((line[i] != ' ') && (line[i] != '+') && (line[i] != '\0')){
				int k = 0;
				while(k < inSize){
					if((line[i] == carrInput[k]) && (line[i+1] != '\'')){
						array[j][k].alpha = carrInput[k];
						array[j][k].index = k;
						array[j][k].present = 1;
						k++;
						i++;
					}
					else if((line[i] == carrInput[k]) && (line[i+1] == '\'')){
						array[j][k].alpha = carrInput[k];
						array[j][k].index = k;
						array[j][k].present = 0;
						k++;
						i = i + 2;
					}
					else if(line[i] != carrInput[k]){
						array[j][k].alpha = carrInput[k];
						array[j][k].index = k;
						array[j][k].present = -1;
						k++;
					}
				}
				MinSOP(array[j], minterms, inSize);
				j++;
			}
		}
	}
}

void readfromFile(char *carrInput, char *carrOutput, int *inSize, int *outSize)
{
	ifstream infile;
	ofstream outfile("Output.txt");
	infile.open("Input.txt");
	if(!infile){
		cerr << "Cannot locate the Input file\n";
		exit(1);
		return;
	}
	string line;
	int inIndex = 0;
	int outIndex = 0;
	int iFlag = 0;
	int j = 0;

	while (!infile.eof()){
		getline(infile, line);
		//outfile << line << endl;
		int i = 0;
		iFlag = 0;
		while(line[i] != '\0'){
			if((line[i++] == '.') && (line[i++] == 'i') && (line[i++] == 'n') && (line[i++] == 'p') && (line[i++] == 'u') && (line[i++] == 't')){
				while(line[i] != '\0'){
					if(line[i] == ' '){
						i++;
						continue;
					}
					carrInput[inIndex++] = line[i++];
					carrInput = (char*)realloc(carrInput, (inIndex+1)*sizeof(char));
					carrInput[inIndex] = '\0';
					*inSize = inIndex;
					iFlag = 1;
				}
				cout << carrInput << " " << "\tNumber of Variables: "<<*inSize << endl << endl;	
			}
			if(iFlag != 1){
				i = 0;
				if((line[i++] == '.') && (line[i++] == 'o') && (line[i++] == 'u') && (line[i++] == 't') && (line[i++] == 'p') && (line[i++] == 'u') && (line[i++] == 't')){
					while(line[i] != '\0'){
						if(line[i] == ' '  || line[i] == ','){
							i++;
							continue;
						}
						carrOutput[outIndex++] = line[i++];
						carrOutput = (char*)realloc(carrOutput, (outIndex+1)*sizeof(char));
						carrOutput[outIndex] = '\0';
						*outSize = outIndex;
						iFlag = 2;
					}
					cout << carrOutput << " " << "\tNumber of outputs to give: "<<*outSize << endl << endl;
					cout << "-----------------All the minterms of the Expressions in the input file are in Output.txt file-------------------\n\n";
					cout << "-----------------All the Minumum Expressions are in Output_QuineM.txt file -------------------------------------\n\n";
				}
			}
			if(iFlag != 1 && iFlag != 2){
				i = 0;
				if((line[i++] == '.') && (line[i++] == carrOutput[j]) && (line[++i] != '(')){
					SOP(carrInput, carrOutput, *inSize, *outSize, line);
					list <int> minterms;
					outfile << carrOutput[j] << " = "<< " m( ";
					for(auto i = st.begin(); i!= st.end(); i++){
						outfile << *i << " ";
						minterms.push_back(*i);
					}
					outfile << ")\n";
					outMin << carrOutput[j] << " =  ";
					cout << carrOutput[j] << " = ";
					//outMin << "Min SOP form of the expression :   " << line << "  :\n-------------->>>>   ";
					FindingImplicants(minterms, carrInput);
					outMin << endl;
					cout << endl;
					st.erase(st.begin(), st.end());
					num_min = 0;
					j++;
					break;
				}
				i = 0;
				if((line[i++] == '.') && (line[i++] == carrOutput[j]) && (line[++i] == '(')){
					POS(carrInput, carrOutput, *inSize, *outSize, line);
					list <int> minterms;
					outfile << carrOutput[j] << " = "<< " m( ";
					int k = pow(2, *inSize);
					int array[k];
					for(k = 0; k < pow(2, *inSize); k++){
						array[k] = k;
					}
					for(auto i = st.begin(); i!= st.end(); i++){
						array[*i] = -1;
					}	
					for(k = 0; k < pow(2, *inSize); k++){
						if(array[k] != -1){
							outfile << array[k] << " ";
							minterms.push_back(array[k]);
						}
					}
					outfile << ")\n";
					outMin << carrOutput[j] << " =  ";
					cout << carrOutput[j] << " =  ";
					//outMin << "Min SOP form of the expression :   " << line << "  : \n-------------->>>>   ";
					FindingImplicants(minterms, carrInput);
					outMin << endl;
					cout << endl;
					st.erase(st.begin(), st.end());
					num_min = 0;
					j++;
					break;
				}
			}
		}
	}
	return;
}

int main()
{
	cout << "\n-------------------------Min-Terms for SOP and POS and also their Minimum Expressions(Using Quine McClukey)--------------------------\n\n";
	char *carrInput = (char*)malloc(sizeof(char));
	int inSize = 0;
	char *carrOutput = (char*)malloc(sizeof(char));
	int outSize = 0;

	readfromFile(carrInput, carrOutput, &inSize, &outSize);
	
	return 0;
}