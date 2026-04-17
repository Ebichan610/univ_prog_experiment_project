#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include <vector>
#include <cstdio>
#include <iterator>
using namespace std;

//これは数字列の降順ソート
void file_descending_sort(int **file_nums)
{
    int num_len = size(*file_nums);
    int tmp = 0;
    int i = 0;
    int j = 1;

    while(i < num_len - 1)
    {
        if(*file_nums[i] < *file_nums[i + j])
        {
            tmp = *file_nums[i];
            *file_nums[i] = *file_nums[i + j];
            *file_nums[i + j] = tmp;
            j++;
        }
        else
        {
            i++;
            j = 1;
        }
    }
}

//これは文字列の降順ソート
void file_descending_sort(vector<string> file_contents)
{
    int len = *file_contents.size();
    int i = 0;
    int j = 0;
    int k = 1;
    char *tmp;
    while(i < len - 1)
    {
        j = 0;
        while(*file_contents[i][j] != '\0')
        {
            if(*file_contents[i][j] < *file_contents[i + k][j])
            {
                tmp = *file_contents[i];
                *file_contents[i] = *file_contents[i + k];
                *file_contents[i + k] = tmp;
                j = 0;
                k++;
            }
            else
            {
                j++;
                k = 1;
            }
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 2)
    {
        cerr << "引数の数が1つではありません。\n";
        return(1);
    }
    //ファイルの読み込みとエラー処理
    ifstream fin(argv[1]);
    if(!fin)
    {
        perror("ファイルが開けませんでした。\n");
        return(errno);
    }
    //ファイルに記述されている文字列を読み込み
    vector<string> file_contents;
    string file_string;
    int i = 0;
    while(fin >> file_string)
    {
        file_contents[i] = file_string;
        i++;
    }
    int content_len = file_contents.size();
    //もしファイル内に書かれていたのが数字列ならば
    if(file_contents[0][0] >='0' && file_contents[0][0] <= '9')
    {
        //数字列を数字に変換する
        int file_nums[content_len];
        for(int j = 0; j < content_len; j++)
            file_nums[j] = atoi(file_contents[j]);
        //降順ソート
        file_descending_sort(&file_nums);
        //順に出力していく
        int num_len = sizeof(file_nums) / sizeof(file_nums[0]);
        for(int m = 0; m < num_len; m++)
        {
            cout << file_nums[m];
            if(m != num_len)
                cout << ", ";
            else
                cout << endl;
        }
    }
    //もしファイル内に書かれていたのが文字列ならば
    else
    {
        //降順ソート
        file_descending_sort(&file_contents);
        //順に出力していく
        for(int n = 0; n < content_len; n++)
        {
            cout << content_len[n];
            if(n != content_len)
                cout << ", ";
            else
                cout << endl;
        }
    }
    return(0);
}
