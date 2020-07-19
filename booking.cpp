#include <sstream>
#include "sp.h"

void seatingplan::bookmenu(){
    while(1){
        int cmd;
        std::cout<<"************* Seat booking menu *************\n"
                   "(1) Show seating plan\n"
                   "(2) Manual booking - choose seat manually\n"
                   "(3) Automatic booking - choose seat by system\n"
                   "(4) Exit\n"
                   "*********************************************\n";
        std::cout<<"Please enter the booking option (1 - 4):";
        std::cin>>cmd;
        if(cmd == 4)
            break;
        else{
            switch(cmd){
                case 1: show(seats);break;
                case 2: manualbook();break;
                case 3: autobook();break;
            }
        }
    }
}

void seatingplan::manualbook(){//手動訂座
    int no,total=0;
    std::vector<std::vector<char>> order = seats;
    std::unordered_map<char,int> booked;
    std::cout<<"How many seats do you want to book? ";
    std::cin>>no;
    show(seats);

    for(int i = 1;i <= no;i++){//逐個逐個輸入座位的位置
        std::cout<<"Ticket "<<i<<": Input the seat number, e.g., 10A or 9B:";
        std::string seat;
        std::cin>>seat;
        while(!valid(seat)){//檢測位置是否有效
            std::cout<<"Invalid input, please try again."<<std::endl;
            std::cin>>seat;
        }

        int row,col;
        if(seat.size() == 2){
            row = seat[0] - '0' - 1;
            col = seat[1] - 'A';
        }else{
            std::string rowno;
            rowno += seat[0];
            rowno += seat[1];
            row = stoi(rowno) - 1;
            col = seat[2] - 'A';
        }
        booked[seats[row][col]]++;
        order[row][col] = 'X';
    }
    show(order);

    for(auto i : booked){//計算門票總價錢
        int price4thiszone = i.second * prices[i.first];
        std::cout<<"Number of tickets in zone "<<i.first<<" "<<prices[i.first]<<" x " <<i.second<<" = $"<<price4thiszone<<std::endl;
        total += price4thiszone;
    }
    std::cout<<"Total price: $"<<total<<std::endl;

    confirm(order,seats);
}

bool seatingplan::valid(std::string in){
    //std::cout<<in.size()<<"\n";
    if(in.size() == 2){
        if(isdigit(in[0]) && in[1]>=65 && in[1]<=80)
            return true;
    }else{
        if(in.size() == 3){
            if(isdigit(in[0]) && isdigit(in[1]) && in[2]>=65 && in[2]<=80)
                return true;
        }
    }
    return false;
}

void seatingplan::autobook(){//由電腦根據每個區域所需的門票數量訂座，位置隨緣
    std::vector<std::vector<char>> order = seats;
    std::unordered_map<char,int> bookrec;//儲存每個區域想要的座位數量
    char wantadjs;
    std::cout<<"Do you want adjacent seats only (Y/N)?:";
    std::cin>>wantadjs;

    for(auto i : prices){//record numbers of seats want in each zone
        std::cout<<"Number of tickets for zone "<<i.first<<": ";
        std::cin>>bookrec[i.first];
    }
    if(bookrec.empty())
        return;//zero seats need to be booked

    std::unordered_map<char,std::vector<std::pair<int,char>>> bookedseats;
    if(wantadjs=='n' || wantadjs=='N'){

        std::unordered_map<char,int> copy = bookrec;
        for(int i = 1;i < 16;i++){
            for (int j = 0; j < 16; ++j) {
                if(copy[seats[i][j]]>0){
                    bookedseats[seats[i][j]].push_back({i,j+'A'});
                    order[i][j] = 'X';
                    copy[seats[i][j]]--;
                }
            }
        }
    }else{
        for(auto i : bookrec){
            std::vector<std::pair<int,int>> tmpseats;
            if(!book_adj(i.first,i.second,zone,order,tmpseats)){
                bookrec.erase(i.first);
                break;
            }
            for(auto j : tmpseats)
                bookedseats[i.first].push_back(j);
        }
    }

    /*for(auto i : bookedseats){//show booked seats
        std::cout<<"Tickets for zone "<<i.first<<" :";
        for(auto j : i.second)
            std::cout<<" "<<j.first<<j.second;
        std::cout<<std::endl;
    }*/

    show(order);

    int total = 0;
    for(auto i : bookrec){
        int price4thiszone = i.second * prices[i.first];
        std::cout<<"Number of tickets in zone "<<i.first<<" "<<prices[i.first]<<" x " <<i.second<<" = $"<<price4thiszone<<std::endl;
        total += price4thiszone;
    }
    std::cout<<"Total price: $"<<total<<std::endl;
    confirm(order,seats);
}

void seatingplan::confirm(std::vector<std::vector<char>> &from,std::vector<std::vector<char>> &to){
    char choice;
    std::cout<<"Confirm the booking (Y/N)?:";
    std::cin>>choice;
    if(choice=='Y' || choice=='y')
        to = from;
}

bool seatingplan::book_adj(char id,int no,std::vector<char> &zone,std::vector<std::vector<char>> &seats,std::vector<std::pair<int,int>> &ret){//逐个逐个区域地自动订座
    bool succ = 0;
    std::vector<std::pair<int,int>> tmpseats;
    for(int i = 1;i < 16;i++){
        if(zone[i]==id && succ==0){//找到对应区域
            //std::cout<<zone[i]<<" "<<seats[i][0]<<std::endl;
            tmpseats.clear();
            for(int j = 0;j < 16;j++){//todo 目前不能连续订16个座位
                //std::cout<<i<<" "<<j<<std::endl;
                if(tmpseats.size()<no && seats[i][j]==id){//寻找连续且可用的座位
                    tmpseats.emplace_back(i,j);
                }else if(tmpseats.size() == no){
                    succ = 1;
                    break;
                }else{
                    tmpseats.clear();
                }
            }
        }
    }
    //std::cout<<"booked "<<tmpseats.size()<<std::endl;
    if(succ){//只有在有足够的连续空位下才会将订座纪录记录
        std::cout<<"Tickets for zone "<<id<<" ";//std::endl
        for(auto i : tmpseats){
            seats[i.first][i.second] = 'x';
            char a = i.second + 'A';
            std::cout<<i.first<<a<<" ";
        }
        std::cout<<std::endl;
        ret = tmpseats;
    }

    return succ;
}