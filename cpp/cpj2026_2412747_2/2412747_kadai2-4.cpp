#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include <vector>
#include <cstdio>
#include <iterator>
using namespace std;

//これは数字列の降順ソート
static void file_descending_sort(vector<int> &nums)
{
    int num_len = nums.size();
    //ソートアルゴリズムはバブルソート
    for(int i = 0; i < num_len - 1; i++)
    {
        for(int j = 0; j < num_len - i - 1; j++)
        {
            if(nums[j] < nums[j + 1])
                swap(nums[j], nums[j + 1]);
        }
    }
}

//これは文字列の降順ソート
static void file_descending_sort(vector<string> &strs)
{
    int len = strs.size();
    for(int i = 0; i < len - 1; i++)
    {
        for(int j = 0; j < len - i - 1; j++)
        {
            if(strs[j] < strs[j + 1])
                swap(strs[j], strs[j + 1]);
        }
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
        perror("ファイルが開けませんでした。");
        return(errno);
    }
    //ファイルに記述されている文字列を読み込み
    vector<string> file_contents;
    string file_string;
    while(fin >> file_string)
        file_contents.push_back(file_string);
    int content_len = file_contents.size();
    //もしファイル内に書かれていたのが数字列ならば
    if(file_contents[0][0] >='0' && file_contents[0][0] <= '9')
    {
        //数字列を数字に変換する
        vector<int> file_nums;
        for(int i = 0; i < content_len; i++)
            file_nums.push_back(stoi(file_contents[i]));
        //降順ソート
        file_descending_sort(file_nums);
        //順に出力していく
        for(int m = 0; m < content_len; m++)
        {
            cout << file_nums[m];
            if(m != content_len - 1)
                cout << ", ";
            else
                cout << endl;
        }
    }
    //もしファイル内に書かれていたのが文字列ならば
    else
    {
        //降順ソート
        file_descending_sort(file_contents);
        //順に出力していく
        for(int n = 0; n < content_len; n++)
        {
            cout << file_contents[n];
            if(n != content_len - 1)
                cout << ", ";
            else
                cout << endl;
        }
    }
    return(0);
}
