//
// Created by MuYue on 2020/6/22.
//

#ifndef SEATING_PLAN_SP_H
#define SEATING_PLAN_SP_H

#include <vector>
#include <unordered_map>
#include <iostream>

class seatingplan{
public:

    std::vector<std::vector<char>> seats;//座位表（未被訂座的顯示區域ID，否則顯示X）
    std::unordered_map<char,int> prices;//門票撿錢

    seatingplan() {reset();}//每當新建seatingplan類的變量時自動初始化seats和prices

    void menu();
    void show(std::vector<std::vector<char>> &plan);
    void allocate();
    void setzone();
    void setticket();
    bool checkini();
    void setdefault();
    void reset();

    //booking
    void bookmenu();
    void manualbook();
    bool valid(std::string in);
    void autobook();
    void confirm(std::vector<std::vector<char>> &from,std::vector<std::vector<char>> &to);
    bool book_adj(char id,int no,std::vector<char> &zone,std::vector<std::vector<char>> &seats);
};

#endif //SEATING_PLAN_SP_H
