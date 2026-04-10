#include <iostream>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[])
{
    //引数が2つでない場合の処理
    if(argc != 3)
    {
        cerr << "引数が2つではありません。\n";
        exit(EXIT_FAILURE);
    }
    //負の数字や小数、数字以外を弾く処理
    int i = 1;
    while(i < 3)
    {
        int j = 0;
        if(argv[i][j] == '-')
        {
            cerr << "負の数字です。\n";
            exit(EXIT_FAILURE);
        }
        while(argv[i][j] !='\0')
        {
            if(argv[i][j] == '.')
            {
                cerr << "値が小数です。\n";
                exit(EXIT_FAILURE);
            }
            else if(argv[i][j]  < '0' || argv[i][j] > '9')
            {
                cerr << "数字以外の文字が含まれています。\n";
                exit(EXIT_FAILURE);
            }
            j++;
        }
        i++;
    }

    //文字列をそれぞれ数値へと変換
    int animal_num = atoi(argv[1]);
    int leg_num = atoi(argv[2]);
    //足の本数が奇数や鶴と亀の合計が多いときはエラー
    if(leg_num % 2 != 0 || (animal_num * 2 > leg_num))
    {
        cerr << "鶴と亀の合計数と足の本数に矛盾があります。\n";
        exit(EXIT_FAILURE);
    }
    //鶴亀算の計算
    int turtle = animal_num;
    int crane = 0;

    while(turtle * 4 + crane * 2 != leg_num)
    {
        turtle--;
        crane++;
    }
    cout << "鶴と亀の合計数は" << animal_num << "匹で、足の合計本数は" << leg_num << "です。\n";
    cout << "このとき鶴は" << crane << "匹で、亀は" << turtle << "匹です。\n";
    return(0);
}
