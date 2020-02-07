#include <iostream>
#include <algorithm>
#include "graph.h"
#include <string>
#include <iostream>
#include <chrono>
#include <string>
#include <list>
#pragma GCC optimize("O2")
// #include <ext/hash_map> //gcc ?
// #include <hash_map>
#include <unordered_map>

using namespace std::chrono;
using namespace std;
using namespace __gnu_cxx;

///use list to store neighber
// typedef struct info_s
// {
// int nNumber;

// }info_t;

typedef std::list<int> list_t;
// typedef list< int > list_t;

void print_list(list_t list);
void print_map(unordered_map<int, list_t> map);
list_t add_path_all(unordered_map<int, list_t> map, int curr);       //dfs
list_t add_path_next_next(unordered_map<int, list_t> map, int curr); //bfs
unordered_map<int, list_t> DFS(unordered_map<int, list_t> node_neighber);
unordered_map<int, list_t> BFS(unordered_map<int, list_t> node_neighber);
list_t::iterator is_in_list(list_t list, int n);

int main(int args, char **argv)
{

    std::cout << "Input: ./exe beg csr weight\n";
    if (args != 4)
    {
        std::cout << "Wrong input\n";
        return -1;
    }

    const char *beg_file = argv[1];
    const char *csr_file = argv[2];
    const char *weight_file = argv[3];
    std::cout << "Normal 19!!! ";
    //template <file_vertex_t, file_index_t, file_weight_t
    //new_vertex_t, new_index_t, new_weight_t>
    graph<long, long, /*int*/ long, long, long, /* char*/ long>
        *ginst = new graph<long, long, /*int*/ long, long, long, /*char*/ long>(beg_file, csr_file, weight_file);

    //**
    //You can implement your single threaded graph algorithm here.
    //like BFS, SSSP, PageRank and etc.
    //cout << ginst->vert_count<<"\n";

    // for (int i = 0; i < ginst->vert_count; i++)
    // {
    //     int beg = ginst->beg_pos[i];
    //     int end = ginst->beg_pos[i + 1];
    //     std::cout << i << "'s neighor list: ";
    //     //   std::cout<<i<<"'s outgoing money: ";
    //     for (int j = beg; j < end; j++)
    //     {
    //         std::cout << ginst->csr[j] << " ";
    //         std::cout << "Money:";
    //         std::cout << ginst->weight[j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    std::cout << "\n";
    std::cout << "New Start"
              << "\n";
    // for (int i = 0; i < ginst->vert_count; i++)
    // {
    //     int beg = ginst->beg_pos[i];
    //     int end = ginst->beg_pos[i + 1];
    //     //std::cout<<beg<<"beg "<<"\n";
    //     //std::cout<<end<<"end "<<"\n";
    //     std::cout << i << "Neighber: ";
    //     //   std::cout<<i<<"'s outgoing money: ";
    //     for (int j = beg; j < end; j++)
    //     {
    //         std::cout << ginst->csr[j] << " " << ginst->weight[j] << "   ";
    //     }
    //     std::cout << "\n";
    //     // cout << "Normal at line 160!!! ";
    // }

    std::cout << "Timer start: ";
    int *num_neighber = new int[ginst->vert_count];
    unordered_map<int, list_t> node_neighber; // TO_DO

    auto start2 = high_resolution_clock::now();
    ///////////////////////
    //////////////////////////

    for (int i = 0; i < ginst->vert_count; ++i)
    {
        int beg = ginst->beg_pos[i];
        int end = ginst->beg_pos[i + 1];
        //std::cout<<beg<<"beg "<<"\n";
        //std::cout<<end<<"end "<<"\n";
        num_neighber[i] = end - beg;
        list_t temp;

        std::cout << i << " has num_neighber: " << num_neighber[i] << " -> ";
        //   std::cout<<i<<"'s outgoing money: ";
        for (int j = beg; j < end; ++j)
        {
            // std::cout << ginst->csr[j] << " " << ginst->weight[j] << "   ";
            temp.push_back(ginst->csr[j]);
        }
        node_neighber.emplace(i, temp);
        // std::cout<<"list in map: "<<i <<": ";       print_list(node_neighber[i]);
        std::cout << "neighber of node: " << i << ": ";
        print_list(node_neighber[i]);
    }
    //////////////////////////
    //////////////////////////
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout << "initialization  spent: " << duration2.count() << " us   ";
    //////////////////////////////////////////////////////

    // CSR_T_R below:

    // int Operation_Time = 0;
    int count_Operation = 0;
    int vert_count = ginst->vert_count;
    /////////////////////////////////////////////

    for (int k = 0; k < vert_count - 1; ++k) // the longest path in T_R is N-1 long
    {
        // cout <<"start stage K////////////////////////////////////: "<<k<<endl;
        // //for (unordered_map<int, list_t>::iterator iter_map = node_neighber.begin(); iter_map != node_neighber.end(); ++iter_map)
        // // for (auto  iter_map = node_neighber.begin(); iter_map != node_neighber.end(); ++iter_map)

        //     int curr_nodde = j;
        //     list_t curr_neighber_list = node_neighber[j];
        //     list_t::iterator curr_iteor_list = curr_neighber_list.begin(); // loop of node's neighber iteor_list, (*iter_map).second is the neighber of iter_map

        //     ////////////////////// debug using
        //     cout <<"travel_map, curr_nodde = : "<<curr_nodde<< "'s neithber_num: "<<curr_neighber_list.size()<<" curr_iteor_list: "<<*curr_iteor_list<<" curr_neighber_list.end(): "<<*curr_neighber_list.end()<<" ";
        //     print_list(curr_neighber_list);
        //     /////////////////////

        //     // while (!curr_neighber_list.empty()  &&  *curr_iteor_list < *curr_neighber_list.end())  //loop in current neighber list, curr_iteor_list is the element of current neighber list
        //     for (int i = 0; i < *curr_neighber_list.end()   ; ++i)
        //     {
        //         list_t next_neighber = node_neighber[*curr_iteor_list]; // list of neighber's neighber

        //         cout << "in neighber: "<<*curr_iteor_list <<"  searching next_neighber list: ";print_list(next_neighber);

        //         list_t::iterator next_iteor_list = next_neighber.begin(); //next_iteor_list element in neighber's neighber list
        //         while (!next_neighber.empty()  &&  *next_iteor_list < *next_neighber.end()){ //loop in next neighber lis
        //             cout <<"  start if: "<<*next_iteor_list<<" in curr_neighber_list: "<<curr_nodde<< endl;
        //             if (*is_in_list(curr_neighber_list,*next_iteor_list) != *curr_neighber_list.end())
        //             {
        //                 curr_neighber_list.push_back(*next_iteor_list); // if neighber's neighber not in curr_neighber_list, let it in
        //                 ++count_Operation;
        //             }
        //             ++*next_iteor_list;
        //         }
        //         ++*curr_iteor_list;
        //     }
        //     node_neighber[curr_nodde]=curr_neighber_list;

        // }
        // cout <<"finish stage: "<<k<<endl; print_map(node_neighber);
    }

    // ////////////////////////////////////////////////////////////////// test

    // std::cout<< "test unordered_map + List_t "<<"\n";

    // unordered_map <int, list_t> test_map;
    // for (int i = 0; i < 10; i++)
    // {
    //     list_t temp;
    //     for (int j = i; j < 10; j++)
    //     {
    //         // temp.push_back(j);
    //         if (*is_in_list(temp,j) ==*temp.end()) // if j not in temp, add it in  Attention! must use * to use the value(int)
    //         {
    //            temp.push_back(j);
    //         }
    //     }
    //     std::cout<<"list in temp:  " ;       print_list(temp);

    //     test_map.emplace(i,temp);
    //     if (*is_in_list(temp,3)!=*temp.end())
    //     {
    //         std::cout<<"find 3!  "<< "\n";
    //     }
    //     std::cout<<"list in map:  " ;       print_list(test_map[i]);
    //     std::cout<<"\n"<<"\n";
    // }

    // std::unordered_map <int, list_t> test_map2;
    // list_t a (2,2);
    // print_list(a);
    // test_map2.emplace(0,a);
    // print_list(test_map2[0]);
    // // cout<<test_map2[0].size.size;
    // a.push_back(4);
    // a.push_back(3);
    // a.push_back(4);
    // print_list(a);
    // print_list(test_map2[0]);
    // cout<<" 9 is_in_list:  "<<*is_in_list(a,9);
    // cout<<" 3 is_in_list:  "<<*is_in_list(a,3);
    // cout<<" 2 is_in_list:  "<<*is_in_list(a,2);
    // cout<<" list.end():  "<<*a.end();

    // cout << "step by step\n";
    // auto start=high_resolution_clock::now();auto stop=high_resolution_clock::now();

    // // print_map(node_neighber);

    //  start = high_resolution_clock::now();

    // for (int i = 0; i < node_neighber.size(); ++i)
    // // for (int i = 0; i < 5; ++i)
    // {
    //     node_neighber[i] = add_path_all(node_neighber, i);
    //     node_neighber[i].unique();
    //     // cout<<i<<" -> ";
    //     // print_list(node_neighber[i]);
    //     cout<< "\nfinish: "<<i;
    // }
    //  stop = high_resolution_clock::now();
    //  print_map(node_neighber);
    // auto duration = duration_cast<microseconds>(stop - start);
    // cout << " serial_CSR_T_R spent: " << duration.count() << " us   ";
    // std::cout << "count_Operation: " << count_Operation << "\n";

    // node_neighber = DFS(node_neighber);
    node_neighber = BFS(node_neighber);
    print_map(node_neighber);
    return 0;
}

void print_list(list_t list)
{
    list_t::iterator iteor = list.begin();
    while (iteor != list.end())
    {
        cout << *iteor++ << " ";
    }
    cout << endl;
}
list_t::iterator is_in_list(list_t list, int n)
{
    list_t::iterator iteor = list.begin();
    while (iteor != list.end())
    {
        if (*iteor == n)
        {
            return iteor;
        }
        *iteor++;
    }
    return iteor;
}
bool is_in_list2(list_t list, int n)
{
    list_t::iterator iter;
    iter = std::find(list.begin(), list.end(), n);
    if (iter != list.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print_map(unordered_map<int, list_t> map)
{
    //unordered_map<int, list_t>::iterator iteor = map.begin();
    for (int i = 0; i < map.size(); ++i)
    {
        cout << i << "-> ";
        print_list(map[i]);
    }
    // while (iteor != map.end()){
    //     cout<<(*iteor).first<<"-> ";
    //     print_list((*iteor).second);
    //     iteor++;
    // }
    cout << "\n";
}

list_t add_path_next_next(unordered_map<int, list_t> map, int curr)
{
    list_t need_update;
    list_t neighber_list = map[curr];
    //print_list(neighber_list);
    //int op =0;
    //sleep(2);

    for (std::list<int>::iterator it = neighber_list.begin(); it != neighber_list.end(); ++it)
    {                                //loop in curr's neighber list
        list_t next_list = map[*it]; //neighber's neighber
        //cout<<"\n  neighbor: " <<*it<<"  ";
        //sleep(2);print_list(next_list);
        for (std::list<int>::iterator it2 = next_list.begin(); it2 != next_list.end(); ++it2)
        { // loop in neighber's neighber list
            //cout<<"next next node: " <<*it2;

            if (*it2 != curr && !is_in_list2(need_update, *it2) && !is_in_list2(neighber_list, *it2)) // if next next node != curr && next next node not in neighber_list
            {
                need_update.push_back(*it2);
                //cout<<"added!";
            }
        } //cout<<"\n";
    }
    neighber_list.merge(need_update);
    need_update.clear();

    return neighber_list; // new neighber list which add Secondary path
}

list_t add_path_all(unordered_map<int, list_t> map, int curr)
{
    list_t need_update;
    list_t neighber_list = map[curr];
    //print_list(neighber_list);
    //int op =0;
    //sleep(2);

    for (std::list<int>::iterator it = neighber_list.begin(); it != neighber_list.end(); ++it)
    {                                //loop in curr's neighber list
        list_t next_list = map[*it]; //neighber's neighber
        //cout<<"\n  neighbor: " <<*it<<"  ";
        //sleep(2);print_list(next_list);
        for (std::list<int>::iterator it2 = next_list.begin(); it2 != next_list.end(); ++it2)
        { // loop in neighber's neighber list
            // cout<<"next next node: " <<*it2;

            if (*it2 != curr && *is_in_list(need_update, *it2) == *need_update.end() && *is_in_list(neighber_list, *it2) == *neighber_list.end()) // if next next node != curr && next next node not in neighber_list
            {
                // cout<<"added!: "<<*it2<<" in need_update: "; print_list(need_update);

                need_update.push_back(*it2);
            }
        } //cout<<"\n";
        neighber_list.merge(need_update);
        need_update.clear();
    }

    return neighber_list; // new neighber list which add Secondary path
}

unordered_map<int, list_t> DFS(unordered_map<int, list_t> node_neighber)
{
    int count_Operation;
    cout << "step by step\n";
    for (int i = 0; i < node_neighber.size(); ++i)
    // for (int i = 0; i < 5; ++i)
    {
        node_neighber[i] = add_path_all(node_neighber, i);
        node_neighber[i].unique();
        // cout<<i<<" -> ";
        // print_list(node_neighber[i]);
        cout << "\nfinish: " << i;
        // ++count_Operation;
        // std::cout << "count_Operation: " << count_Operation << "\n";
    }

    return node_neighber;
}

unordered_map<int, list_t> BFS(unordered_map<int, list_t> node_neighber)
{
    for (int k = 0; k < node_neighber.size() - 1; k++)
    {
        for (int i = 0; i < node_neighber.size(); i++)
        {
            if (node_neighber.empty())
            {
                break;
            }
            {
                node_neighber[i] = add_path_next_next(node_neighber, i);
                node_neighber[i].unique();
            }
        }
        cout << "\nfinish K : " << k;
    }
    return node_neighber;
}