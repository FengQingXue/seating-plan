//
// Created by MuYue on 2020/6/28.
//
#include "sp.h"

void seatingplan::menu(){
    int cmd;
    while(1){
        std::cout<<"********** Setup seating plan menu(v0.01) **********\n"
                   "(1) Reset seating plan\n"
                   "(2) Show seating plan\n"
                   "(3) Allocate seats for an event\n"
                   "(4) Set zones for an event\n"
                   "(5) Set ticket prices\n"
                   "(6) Use default seating plan & proceed\n"
                   "(7) Proceed to seat booking\n"
                   "(8) Exit\n"
                   "*********************************************\n"
                   "Please enter the setup option (1 - 8):";
        std::cin>>cmd;
        if(cmd == 8)
            break;
        else
            switch (cmd){
                case 1: reset();break;
                case 2: show(seats);break;
                case 3: allocate();break;
                case 4: setzone();break;
                case 5: setticket();break;
                case 6: setdefault();break;
                case 7: bookmenu();break;
            }
    }
}

void seatingplan::setdefault(){//重置為默認座位表
    prices.clear();
    seats.clear();

    prices['A'] = 100;
    prices['B'] = 200;
    prices['C'] = 300;

    std::vector<char> temp(16,'-');
    seats.push_back(temp);
    for(auto c = 'A';c <= 'C';c++){
        std::vector<char> tmp(16,c);
        for(int i = 0;i < 5;i++)
            seats.push_back(tmp);
    }
}

void seatingplan::reset(){//清空座位表
    prices.clear();
    seats.clear();

    std::vector<char> tmp(16,'-');
    for(int i = 0;i < 16;i++)
        seats.push_back(tmp);
}

void seatingplan::show(std::vector<std::vector<char>> &plan){//展示當前座位表
    for(int i = 1;i <= plan.size();i++){
        if(i < 10)
            std::cout<<" "<<i<<" ";
        else
            std::cout<<i<<" ";
        for(int j = 0;j < plan[i-1].size();j++){
            if(j==4 || j==12)
                std::cout<<"  ";

            std::cout<<plan[i-1][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"   A B C D   E F G H I J K L   M N O P\n";
    std::cout<<std::endl;
    std::cout<<"           ----------------------------"<<std::endl;
    std::cout<<"                       | Stage |"<<std::endl;
    std::cout<<"           ----------------------------"<<std::endl;

    std::cout<<"Prices for each zone:\n";
    if(prices.size() == 0)
        std::cout<<"N/A (Zone is not set)\n";
    else{
        for(auto i : prices){
            std::cout<<"Price for zone "<<i.first<<" : $"<<i.second<<std::endl;
        }
    }
}

bool seatingplan::checkini(){//檢測當前座位表是否未初始化
    for(auto i : seats)
        if(i[0] != '-')
            return 1;
    return 0;
}

void seatingplan::allocate(){//設定可用的區域
    std::cout<<"Assign available seats for the event...\n";
    int s,e;
    std::cout<<"Enter starting row number: ";std::cin>>s;//std::cout<<std::endl;
    std::cout<<"Enter ending row number: ";std::cin>>e;
    for(int i = s-1;i < e;i++){
        for(char &c : seats[i])
            c = 'o';
    }
}

void seatingplan::setzone(){//為可用的區域分配ID
    if(checkini() == 0){
        std::cout<<"Please allocate seats for an event first.\n";
    }else{
        int cnt;
        std::cout<<"Please input the number of zones in the venue: ";
        std::cin>>cnt;
        if(cnt<=0 || cnt>16){
            std::cout<<"Invalid input.\n";
            return;;
        }
        for(int i = 0;i < cnt;i++){
            char name;
            int s,e;
            std::cout<<"Please input the zone ID: ";std::cin>>name;
            std::cout<<"from... ";std::cin>>s;
            std::cout<<"to... ";std::cin>>e;

            prices[name] = -1;//在門票價錢表中加入當前區域的記錄
            for(int i = s-1;i < e;i++)//在座位表中記錄區域的範圍
                for(char &c : seats[i])
                    c = name;
        }
    }
}

void seatingplan::setticket(){
    for(auto &i : prices)//為所有未設置門票價錢的區域設定門票價格，設定後不可改變
        if(i.second == -1){
            int p;
            std::cout<<"Please set the price for zone "<<i.first<<" : ";
            std::cin>>p;
            while(p < 0)
                std::cout<<"Invalid input, try again.\n";
            i.second = p;
        }
}