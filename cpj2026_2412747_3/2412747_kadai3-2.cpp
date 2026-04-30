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
    //ファイルから文字を読み取っていき単語数を計算
    int count = 0;
    int tmp;

    while(fin >> tmp)
        count++;
    fin.close();
    return(count);
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

    //後ろから範囲を狭めていく(後ろに最大値を集める)
    for(int i = N - 1; i > 0; i--)
    {
        int max = nums[0];
        int max_i = 0;

        //指定範囲内での最大値を探索
        for(int j = 1; j <= i; j++)
        {
            if(max < nums[j])
            {
                max = nums[j];
                max_i = j;
            }
        }

        //値を交換
        ft_swap(&nums[i], &nums[max_i]);
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
        if(j != N - 1)
            cout << ',';
    }
    cout << '\n';
    fin.close();
    return(0);
}
