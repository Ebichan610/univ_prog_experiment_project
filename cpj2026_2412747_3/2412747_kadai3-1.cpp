#include <iostream>
#include <cstdio>

using namespace std;

//引数が数字のみなのかを判定する関数
static bool is_valid_arg(char *str)
{
    while(isspace(*str))
        str++;
    if(*str == '+')
        str++;
    if(*str < '0' || *str > '9')
        return(false);
    while(*str >= '0' && *str <= '9')
        str++;
    if(*str != '\0')
        return(false);
    return(true);
}

//素数かどうかを判定する関数
static bool is_prime(int num)
{
    //0と1は素数判定に関係がないので排除
    if(num <= 1)
        return(false);
    if(num == 2)
        return(true);
    //2だけは例外で処理
    if(num % 2 == 0)
        return(false);
    //3からは順番に奇数を試す
    int i = 3;
    //iがnumの平方根よりも大きい時点で処理を終了
    while(i * i <= num)
    {
        if(num % i == 0)
            return(false);
        i += 2;
    }
    return(true);
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc !=2)
    {
        cerr << "引数を1つにして実行してください\n";
        return(1);
    }
    //数値が妥当かの判断
    if(is_valid_arg(argv[1]) == false)
    {
        cerr << "数値の入力が不正です。2以上の整数を入力してください\n";
        return(1);
    }
    //数字をint型に変換
    int N = atoi(argv[1]);
    //0と1は数学的におかしいので排除
    if(N == 0 || N == 1)
    {
        cerr << "0と1は素数ではありません。2以上の整数を入力してください\n";
        return(1);
    }
    //課題の指定通りstring型の配列を用意
    string nums[N + 1];
    //各要素に1を入れて初期化
    for(int i = 0; i <= N; i++)
        nums[i] = "1";
    //素数判定プログラム
    for(int j = 0; j <= N; j++)
    {
        if(is_prime(j) == false)
            nums[j] = "0";
    }
    //素数だったものを出力
    for(int k = 0; k <= N; k++)
    {
        //1であるインデックスなら出力
        if(nums[k] == "1")
        {
            cout << k;
            if(k != N)
                cout << ',';
        }
    }
    cout << '\n';
    return(0);
}
