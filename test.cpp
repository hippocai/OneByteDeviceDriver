#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#define SIZE 5
using namespace std;
void write_one_mb();
int main(){
	for(int i=0;i<SIZE;++i){
		write_one_mb();
	}
}

void write_one_mb(){
	srand((unsigned)time(NULL)); 
	for(int i=0;i<(1024*1024-3);++i){
		cout<<"A";
	}
	cout<<"1MB";
}
