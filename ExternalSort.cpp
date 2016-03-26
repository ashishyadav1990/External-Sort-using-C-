/**
@Author : Ashish Yadav
**/


#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<map>
#include<string>
#include<sstream>
#include<limits.h>

using namespace std;

int calculateNumS(string str){
    int count_s=0;
    for(int i=0;str[i]!='\0';i++){
        if(str[i]=='s' || str[i]=='S'){
            count_s++;
        }
    }
    return count_s;
}

int main(int argc, char** argv){
    if(argc<2) {
        cout<<"Enter 2 arguments for input and output files respectively, and third optional argument for available memory (in KBytes)";
        exit(1);
    }
    vector<string> listStrings;
    map<int, vector<string> > listMap;
    vector<string> files;
    long int file_size;
    long int n;
    long int k;
    int i=1;
    int memory;
    char *fn;
    FILE *f, *ft;
    int size_of_file;
    fn = argv[1];
    if(argc==3) {
        memory = atoi(argv[3]);
    } else {
        memory = 0;
    }
    f=fopen(fn,"rb");
    if(f==NULL){
        cout<<"Cannot open the input file\n";
        exit(1);
    }

    /*********** 20 Kb of memory is reserved for various local variables int the program ************/
    memory=memory-20;

    fseek(f,0,SEEK_END);
    file_size=ftell(f);
    if(memory!=0) {
      if(((file_size/1000)%memory)==0){
        n=((file_size/1000)/memory);
      }
      else{
        n=((file_size/1000)/memory)+1;
      }

    }
    else {
        n=100;
    }
    k=file_size/n;
    rewind(f);

    /*******************************************************************/
    /*********** Create temporary partitioned Files ********************/
    /*******************************************************************/

    ifstream infile(fn);
    string line;
    vector<string> fileNameRemove;
    long int max_size=0;
    bool flagLine = false;
    while(infile.good()){
        if(max_size>= k){
            string fileName=string(fn);
            stringstream ss;
            ss << i;
            fileName+=ss.str();
            files.push_back(fileName);
            const char* conv_fileName = fileName.c_str();
            fileNameRemove.push_back(conv_fileName);
            FILE *fp = fopen(conv_fileName,"w");
            if(fp==NULL){
                cout<<"Current working directory does not have write permissions\n";
                exit(1);
            }
            for(map<int,vector<string> >::iterator it=listMap.begin();it!=listMap.end();it++){
                for(vector<string>::iterator it_v=listMap[it->first].begin();it_v!=listMap[it->first].end();it_v++){
                    fprintf(fp, "%s\n",(*it_v).c_str());
                }
            }
            fclose(fp);
            listMap.clear();
            if(flagLine){
                flagLine=false;
                int count_s=calculateNumS(line);
                if(listMap.find(count_s)==listMap.end()){
                    vector<string> temp;
                    temp.push_back(line);
                    listMap[count_s]=temp;
                }
                else{
                    vector<string> temp = listMap[count_s];
                    temp.push_back(line);
                    listMap[count_s] = temp;
                }
                max_size=line.size();
                i++;
                continue;
            }
            max_size=0;
            i++;
            continue;
        }
        else {
            getline(infile,line);
        }
        int count_s=calculateNumS(line);
        int line_size=(line.length()+1);
        if(max_size+line_size >= k) {
            max_size+=line_size;
            flagLine = true;
            continue;
        }
        max_size+=line_size;
        if(listMap.find(count_s)==listMap.end()){
            vector<string> temp;
            temp.push_back(line);
            listMap[count_s]=temp;
        }
        else{
            vector<string> temp = listMap[count_s];
            temp.push_back(line);
            listMap[count_s] = temp;
        }
    }
    string fileName=string(fn);
    stringstream ss;
    ss << i;
    fileName+=ss.str();
    files.push_back(fileName);
    const char* conv_fileName = fileName.c_str();
    fileNameRemove.push_back(fileName);
    FILE *fp = fopen(conv_fileName,"w");
    if(fp==NULL){
        cout<<"Current working directory does not have write permissions\n";
        exit(1);
    }
    for(map<int,vector<string> >::iterator it=listMap.begin();it!=listMap.end();it++){
        for(vector<string>::iterator it_v=listMap[it->first].begin();it_v!=listMap[it->first].end();it_v++){
            fprintf(fp, "%s\n",(*it_v).c_str());
        }
    }
    listMap.clear();
    max_size=0;
    fclose(fp);





    /*******************************************************************/
    /********* Merge Partitioned Files in a single output file *********/
    /*******************************************************************/


    map<int,vector<map<int,string> > > finalMap;
    int numFiles=files.size();
    ifstream fileObjects[numFiles];
    FILE *fout = fopen(argv[2],"w");
    if(fp==NULL){
        cout<<"Cannot open/create the output file\n";
        exit(1);
    }
    for(int i=0;i<numFiles;i++){
        fileObjects[i].open(files[i].c_str());
    }
    int index=-1;
    int lastCount = -1;
    while(!files.empty()){
        int minS=INT_MAX;
        int minIndex=0;
        string line;
        int flagMapMemory=0;
        long int max_size=0;
        if(index==-1){
            int j=0;
            while(j<numFiles && getline(fileObjects[j++],line)){
                vector<map<int,string> > tempVec;
                map<int, string> tempMap;
                int count_s=calculateNumS(line);
                tempMap[j]=line;
                tempVec.push_back(tempMap);
                if(finalMap.find(count_s)==finalMap.end()){
                    finalMap[count_s]=tempVec;
                }
                else{
                    vector<map<int, string> > tmp;
                    tmp = finalMap[count_s];
                    tmp.push_back(tempMap);
                    finalMap[count_s]=tmp;
                }
            }
        }
        else{
            if(index <= numFiles && getline(fileObjects[index-1],line)){
                vector<map<int,string> > tempVec;
                int count_s=calculateNumS(line);
                map<int,string> tempMap;
                tempMap[index]=line;
                tempVec.push_back(tempMap);
                if(finalMap.find(count_s)==finalMap.end()){
                    finalMap[count_s]=tempVec;
                }
                else{
                    vector<map<int, string> > tmp;
                    tmp = finalMap[count_s];
                    if(count_s==lastCount) {
                       tmp.insert(tmp.begin(),tempMap);
                    } else {
                        tmp.push_back(tempMap);
                    }
                    finalMap[count_s]=tmp;
                }
            }
            else{
                files.erase(files.begin()+index-1);
            }
        }
        if(!finalMap.empty()) {
            map<int,vector<map<int,string> > >::iterator it=finalMap.begin();
            vector<map<int, string> > tVec = finalMap[it->first];
            vector<map<int, string> >::iterator it_vec=tVec.begin();
            map<int,string> tMap = *it_vec;
            map<int,string>::iterator it2=tMap.begin();
            index=it2->first;
            lastCount = it->first;
            fprintf(fout, "%s\n",(it2->second).c_str());
            tVec.erase(tVec.begin());
            if(tVec.empty()){
                finalMap.erase(finalMap.begin());
            } else {
                finalMap[it->first] = tVec;
            }
        }
    }
    while(!finalMap.empty()){
        map<int,vector<map<int,string> > >::iterator it=finalMap.begin();
        vector<map<int, string> > tVec = finalMap[it->first];
        vector<map<int, string> >::iterator it_vec=tVec.begin();
        map<int,string> tMap = *it_vec;
        map<int,string>::iterator it2=tMap.begin();
        fprintf(fout, "%s\n",(it2->second).c_str());
        tVec.erase(tVec.begin());
        if(tVec.empty()){
            finalMap.erase(finalMap.begin());
        } else {
            finalMap[it->first] = tVec;
        }
    }
    max_size=0;
    for(int i=0;i<numFiles;i++){
        fileObjects[i].close();
        int y=remove(fileNameRemove[i].c_str());
    }
}
