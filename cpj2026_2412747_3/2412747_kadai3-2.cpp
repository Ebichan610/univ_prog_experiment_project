#include <iostream>
#include <fstream>
#include <cerrno>
#include <climits>
using namespace std;

static int calc_num_of_nums_in_file(const char *filename)
{
    //ファイルをオープン
    ifstream fin(filename);
    if(!fin)
    {
        perror("ファイルが開けませんでした");
        exit(errno);
    }
    //ファイルから文字を読み取っていき空白を探索
    char word;
    int num_of_nums = 1;
    while(fin >> noskipws >> word)
    {
        if(word == ' ')
            num_of_nums++;
    }
    fin.close();
    return(num_of_nums);
}

//値を交換するswap関数の自作関数
static void ft_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//昇順ソート関数
static void ascending_sort(int* nums, int N)
{
    int max = nums[0];
    int max_i = 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(max < nums[j])
            {
                max = nums[j];
                max_i = j;
            }
        }
        swap(nums[i], nums[max_i]);
        max = nums[0];
        max_i = 0;
    }
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 2)
    {
        cerr << "引数を1つにして入力してください\n";
        return(1);
    }
    //まずはファイルにある整数の数を計算
    int N = calc_num_of_nums_in_file(argv[1]);
    //一応例外処理
    if(N <= 0)
    {
        cerr << "ファイル内に整数が存在しません\n";
        return(1);
    }
    //ソート元の整数配列を用意
    int nums[N];
    //ファイルから順番に整数を読み込んでいく
    ifstream fin(argv[1]);
    if(!fin)
    {
        perror("ファイルが開けませんでした");
        return(errno);
    }
    for(int i = 0; i < N; i++)
        fin >> nums[i];
    //昇順ソートを実行
    ascending_sort(nums, N);
    //出力
    for(int j = 0; j < N; j++)
    {
        cout << nums[j];
        if(j != N)
            cout << ',';
    }
    cout << '\n';
    return(0);
}
