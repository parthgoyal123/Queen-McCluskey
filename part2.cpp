#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <set>
#include <stack>
#include <list>
#include <vector>
#include <bitset>
#include <cmath>
#include <algorithm>

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

bool alreadyPresent(list<Implicant> implicants, Implicant e);
bool CheckSuperset(Bucket prime_implicants, Implicant i1);
void GenerateSOP(bitset<3> *set0, int size);
void PrintSOP(int array1[], int size);

void PrintSOP(int array1[], int size)
{
	Node node[3];
	for (int i = 0; i < 3; i++){
		node[i].alpha = 'a' + i;
		node[i].index = 2-i;
	}
	for (int i = 0; i < 3; i++){
		if(array1[i] == -1){
			continue;
		}
		else if(array1[i] == 1){
			cout << node[i].alpha;
		}
		else{
			cout << node[i].alpha << "'";
		}
	}
}

void GenerateSOP(bitset<3> *set0, int size)
{
	int array1[3] = {0};
	for (int j = 0; j < 3; j++){
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
	PrintSOP(array1, size);
	cout << " + ";
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

void FindingImplicants(list<int> minterms)
{
	int largest_minterm = minterms.back();
	int size_of_bucket = ceil(log2(largest_minterm)) + 1;
	Bucket *pBucket[size_of_bucket];
	const int M = ceil(log2(largest_minterm));
	for(int i = 0; i < size_of_bucket; i++){
		pBucket[i]= new Bucket;
	}
	for (auto i = minterms.begin(); i!= minterms.end(); i++){
		bitset<40> set0(*i);
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
	/*for (int i = 0; i < sizeofstack; i++)
	{
		prime_implicants[i] = new Bucket;
	}*/
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
	
	for(auto it = ess_prime_implicants.implicants.begin(); it != ess_prime_implicants.implicants.end(); it++){
		bitset<3> set0[it->size];
		for(int i = 0; i < it->size; i++){
			bitset<3> set1(it->array[i]);
			set0[i] = set1;
		}
		GenerateSOP(set0, it->size);
	}
}

int main()
{
	cout << "Enter all the minterms: (Enter -1 to exit)\n";
	list <int> minterms;
	int a;
	while(1){
		cin >> a;
		if(a == -1){
			break;
		}
		minterms.push_back(a);
	}
	minterms.sort();
	FindingImplicants(minterms);
}