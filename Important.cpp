#include <iostream>
#include "graph.h"
#include <string>

using namespace std;

typedef struct
{
    //struct for matrix
    int row, col;
    //use to allocate MEM dynamicly
    float **matrix;
} Matrix;
typedef struct
{
    int row, col;
    string **matrix;
} Matrix_string;

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
    for (int i = 0; i < ginst->vert_count; i++)
    {
        int beg = ginst->beg_pos[i];
        int end = ginst->beg_pos[i + 1];
        std::cout << i << "'s neighor list: ";
        //   std::cout<<i<<"'s outgoing money: ";
        for (int j = beg; j < end; j++)
        {
            std::cout << ginst->csr[j] << " ";
            std::cout << "Money:";
            std::cout << ginst->weight[j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::cout << "New Start"
              << "\n";
    for (int i = 0; i < ginst->vert_count; i++)
    {
        int beg = ginst->beg_pos[i];
        int end = ginst->beg_pos[i + 1];
        //std::cout<<beg<<"beg "<<"\n";
        //std::cout<<end<<"end "<<"\n";
        std::cout << i << "Neighber: ";
        //   std::cout<<i<<"'s outgoing money: ";
        for (int j = beg; j < end; j++)
        {
            std::cout << ginst->csr[j] << " " << ginst->weight[j] << "   ";
        }
        std::cout << "\n";
        // cout << "Normal at line 160!!! ";
    }
    // std::cout << "Normal  61!!! ";
    //start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////////////////////////
    int row = 10, col = 10; // col and row
    int INF = 9999;

    cout << "Normal 65!!! ";
    typedef struct
    {
        //struct for matrix
    int row, col;
    //use to allocate MEM dynamicly
        float **matrix;
    } Matrix;
    cout << "Normal 73!!! ";
    typedef struct
    {
        int row, col;
        string **matrix;
    } Matrix_string;

    Matrix m;         //store value of path
    Matrix_string m2; //store path
    std::cout << "Normal 81!!! "
              << "\n";

    float **enterMatrix;
    string **enterMatrix2;
    enterMatrix = (float **)malloc(row * sizeof(float *));   // value of path
    enterMatrix2 = (string **)malloc(row * sizeof(float *)); //store path
    std::cout << "Normal 103!!! "
              << "\n";
    for (int i = 0; i < row; i++) //put in to memory  //change size to *10
    {
        enterMatrix[i] = (float *)malloc(col *10* sizeof(float));
        enterMatrix2[i] = (string *)malloc(col *10* sizeof(string)); // change sizeof(float) to string
    }
    std::cout << "Normal 109!!! "
              << "\n";

    for (int i = 0; i < row; i++) //set default value
    {
        for (int j = 0; j < col; j++)
        {
            enterMatrix[i][j] = INF;  //For path value ,default is 99999
            enterMatrix2[i][j] = " "; //for path, default is " "
            if (i==j)
            {
                enterMatrix[i][j]=0;
            }
            
        }
    }
    std::cout << "Normal 122!!! " //<<enterMatrix[9][9]<< enterMatrix2[9][9]
              << "\n";
     for (int i = 0; i < ginst->vert_count; i++) // i: scr  ginst->csr[j]: dist   ginst->weight[j]: weight
    {
        int beg = ginst->beg_pos[i];
        int end = ginst->beg_pos[i + 1];

        for (int j = beg; j < end; j++)
        {
            enterMatrix[i][ginst->csr[j]] = ginst->weight[j];                                                          //Matrix[scr][dist] = path weight
            enterMatrix2[i][ginst->csr[j]] = enterMatrix2[i][ginst->csr[j]] + to_string(i) + to_string(ginst->csr[j]); //Matrix[scr][dist] = path
        }
    }

    std::cout << "Normal 136!!! "<<"\n";

    m.col = col;
    m.row = row;
    m.matrix = enterMatrix;
    m2.matrix = enterMatrix2;

    for (int i = 0; i < m.row; i++) //print Matrix
        {
            for (int j = 0; j < m.col; j++)
            {
                std::cout << m2.matrix[i][j] << " ";//path
                std::cout << m.matrix[i][j] << " ";//value
                
            }
            std::cout << endl;
        }

        for (int i = 0; i < m.row; i++) //print Matrix
        {
            for (int j = 0; j < m.col; j++)
            {
                if ( m.matrix[i][j] != INF && m.matrix[i][j] !=0)
                {
                    for (int k = i+1; k < m.col; k++){ // extend the path of
                        if ( m.matrix[j][k] > m.matrix[i][j] + m.matrix[i][k]){ // if new path is shorter, refersh
                            int temp =m.matrix[i][j] + m.matrix[i][k];
                            m.matrix[j][k] = temp;
                            m.matrix[k][j] = temp;
                            //m2.matrix[j][k] += m2.matrix[j][i]+m2.matrix[i][k];
                            // m2.matrix[j][k] += m2.matrix[j][i]+m2.matrix[i][k];
                            // m2.matrix[k][j] += m2.matrix[k][i]+m2.matrix[i][j];
                             m2.matrix[j][k] = m2.matrix[j][i]+m2.matrix[i][k];
                             m2.matrix[k][j] = m2.matrix[k][i]+m2.matrix[i][j];
                        }

                    }
                }
                
                
            }
            std::cout << endl;
        }
        std::cout << "NEW PATH of 1-9: "<< m.matrix[0][9] <<"\n";
        std::cout << "NEW PATH weight of 1-9: "<< m2.matrix[0][9] <<"\n";
        //     for (int i = 0; i < m.row; i++) //print Matrix
        // {
        //     for (int j = 0; j < m.col; j++)
        //     {
        //         std::cout << m2.matrix[i][j] << " ";//path
        //         std::cout << m.matrix[i][j] << " ";//value
                
        //     }
        //     std::cout << endl;
        // }

    return 0;
}
