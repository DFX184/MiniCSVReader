#include<iostream>
#include"CSVReader.hpp"

int main(){

    CSVReader csv("test.csv");
    auto data = csv["studytime"];
    // for(const auto& d:data){
    //     std::cout<<d<<std::endl;
    // }
    auto shape = csv.GetShape();
    std::cout<<"("<<shape.first<<","<<shape.second<<")"<<std::endl;
    for(int j =0;j<1000;j++)
    {
    auto shape1 = csv.GetShape();
    std::cout<<"("<<shape1.first<<","<<shape1.second<<")"<<std::endl;
    std::vector<int> row,col;
    for(int i =0;i<(int)shape1.second;i++)
    {
        row.push_back(i);
    }
    
    csv.AppendRow(row);
    for(int i =0;i<(int)shape1.first+1;i++){
        col.push_back(i);
    }
    std::string colname = std::string("test_") + std::to_string(j);
   // csv.display();
    csv.AppendCol(colname,col);
    
    }
    std::cout<<"test end"<<std::endl;
    auto shape1 = csv.GetShape();
    std::cout<<"("<<shape1.first<<","<<shape1.second<<")"<<std::endl;
    csv.Save("out.csv");
 
    return 0;
}
