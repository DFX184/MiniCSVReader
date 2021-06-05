#ifndef CSVREAD_H_
#define CSVREAD_H_


/********************************
 *
 *  Log: 2021/5/21
 *
 *
 *********************************/

/******************************************
 *
 * In this file we define CSVReader class
 * for your csv file  if its celldata is empty we define its Nan (pandas)
 *
*******************************************/
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<list>
#include<map>
#include<cassert>


class CSVReader {
  private:
    std::size_t RowSize{0};
    std::vector<std::string>ColNames;
    std::map<std::string,std::list<std::string>*>* ColData;
    std::map<std::size_t,std::list<std::string>*>* RowData;
    CSVReader()=default;
  public:


    CSVReader(const std::string& filename);
    template<typename T>
    T* at(const std::size_t& row_index,const std::string& colname);


    std::list<std::string> operator[](const std::string& colname);
    std::map<std::string,std::string>* operator[](const std::size_t & index);

    template<typename Container>
    void AppendCol(const std::string& colname,Container coldata);


    template<typename Container>
    void AppendRow(Container rowdata);

    std::pair<std::size_t,std::size_t> GetShape() {
        return {RowSize,ColNames.size()};
    }

    /****************************************************************
     *
     *    Save file And display csv data
     *    @ params filename
     *
     ***************************************************************/
    void Save(const std::string& filename) const;
    void display() const;
    ~CSVReader() {
        delete RowData;
        delete ColData;
    }

};


template<typename T>

T* CSVReader::at(const std::size_t& row_index,const std::string& colname) {
    if(std::find(ColNames.begin(),ColNames.end(),colname)==ColNames.end()||row_index>RowSize)return nullptr;
    std::string temp_val = (*(this->operator[](row_index)))[colname];
    std::istringstream convert(temp_val);
    T return_val;
    convert>>return_val;
    T* ptr = &return_val;
    return ptr;
}
/****************************************************************
 *
 *    Return the data of the corresponding row through the std::size_t type index,
 *    and you can get the corresponding data by the column name
 *    @ params std::size_t index
 *
 *************************************************************/
std::map<std::string,std::string>* CSVReader::operator[](const std::size_t& index) {
    if(index >= RowSize) return nullptr;
    std::map<std::string,std::string>* result = new std::map<std::string,std::string>();
    std::list<std::string>* data  = RowData->at(index);
    assert(data->size()==ColNames.size());
    auto iter = data->begin();
    for(const auto& name:ColNames) {
        (*result)[name] = *iter++;
    }
    return result;

}
/****************************************************************
 *
 *
 *
 *    Append new Row in data
 *
 *
 * ****************************************************************/

template<typename Container>
void CSVReader::AppendRow(Container rowdata) {
    assert(rowdata.size() == ColNames.size());
    std::list<std::string> *temp = new std::list<std::string>();
    std::for_each(rowdata.begin(), rowdata.end(),[&temp](auto d) {
        temp->push_back(std::to_string(d));
    });
    (*RowData)[RowSize++] = temp;

}
void CSVReader::Save(const std::string& filename) const {
    std::ofstream os(filename,std::ios::out);
    if(!os.is_open()) {
        std::cerr<<"Not open file : "<<filename<<std::endl;
        return;
    }
    for(std::size_t i =0; i<ColNames.size(); i++) {
        if(i==ColNames.size()-1)os<<ColNames[i];
        else {
            os<<ColNames[i]<<",";
        }
    }
    os<<"\n";

    //Save data
    for(std::size_t i =0; i<RowSize; i++) {
        std::list<std::string>* rowdata = (*RowData)[i];
        if(rowdata!=nullptr) {
            std::size_t s =0;
            for(auto it =rowdata->begin(); it!=rowdata->end(); it++) {
                if(s==rowdata->size()-1)os<<*it;
                else {
                    os<<*it<<",";
                }
                s++;
            }
            os<<"\n";
        }
    }
    os.close();
}
/*
*
*   @ params colname , coldata
*   For coldata,if the type of the list is customized by you,
*   it can be safely coerced to std::string
*
*/
template<typename Container>
void CSVReader::AppendCol(const std::string& colname,Container coldata) {
    if(coldata.size()!=RowSize) {
        std::cerr<<"Error : coldata.size() != RowSize!!"<<std::endl;
        return;
    } else {
        ColNames.push_back(colname);
        std::list<std::string>temp;
        std::for_each(coldata.begin(),coldata.end(),[&temp](auto d) {
            temp.push_back(std::to_string(d));
        });
        (*ColData)[colname] = &temp;
        auto it = temp.begin();
        for(std::size_t i =0; i<RowSize; i++) {
            (*RowData)[i]->push_back(*it);
            it++;
        }
    }
}


std::list<std::string> CSVReader::operator[](const std::string& colname) {
    std::list<std::string>result;
    try {
        result = *((*ColData).at(colname));
    } catch(std::out_of_range& e) {}
    return result;
}


void CSVReader::display() const {
    for(const auto & col:ColNames) {
        std::cout<<col<<"\t";
    }
    std::cout<<"\n";
    for(size_t i =0; i<RowSize; i++) {
        std::cout<<i+1<<": ";
        for(const auto& cell:*(*RowData)[i]) {
            std::cout<<cell<<"\t";
        }
        std::cout<<"\n";
    }

}
CSVReader::CSVReader(const std::string& filename) {

    std::ifstream File(filename, std::ios::binary);
    if (!File.is_open()) {
        std::cerr << "File : " <<filename << "Not open!!" << std::endl;
        exit(0);
    }
    std::vector<char>buffer;
    buffer.resize(File.seekg(0, std::ios::end).tellg());
    File.seekg(0, std::ios::beg).read(&buffer[0], static_cast<std::streamsize>(buffer.size()));
    //std::cout<<"end"<<std::endl;
    ColData = new std::map<std::string,std::list<std::string>*>();
    RowData = new std::map<size_t,std::list<std::string>*>();
    auto pos = std::find(buffer.begin(), buffer.end(),'\n');
    if(pos!=buffer.end()) {
        std::string colnames(buffer.begin(), pos-1);
        std::string colname;
        std::cout<<colnames<<std::endl;
        std::istringstream ss(colnames);
        while(std::getline(ss,colname,',')) {
            ColNames.push_back(colname);
            ColData->insert({colname,new std::list<std::string>()});
        }
        /*
          Test code
        */
    }
    pos = pos+1;
    int row=0;
    while(pos!=buffer.end()) {
        auto iter = std::find(pos,buffer.end(),'\n');
        std::istringstream ss(std::string(pos,iter-1));
        std::string celldata;
        int index = 0;
        (*RowData)[row] = new std::list<std::string>();
        while(std::getline(ss,celldata,',')) {
            if(celldata.length()==0) {
                RowData->at(row)->push_back("Nan");
                (*ColData)[ColNames[index]]->push_back("Nan");
            } else {
                RowData->at(row)->push_back(celldata);
                (*ColData)[ColNames[index]]->push_back(celldata);
            }
            index ++;
        }
        pos = iter+1;
        row++;
        RowSize++;
    }
    // std::cout<<ColNames.size()<<std::endl;
    buffer.clear();
}

#endif