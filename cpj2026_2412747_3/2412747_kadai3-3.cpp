#include <iostream>
using namespace std;

//uint8内の数字なのかを判定する関数
static bool is_in_uint8(char *str)
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

int main(int argc, char*argv[])
{
    //引数エラー処理
    if(argc < 2)
    {
        cerr << "引数を入力してください\n";
        return(1);
    }
    //引数に入力されたものがuint8に収まっているものなのかを判定
    for(int i = 0; i < argc; i++)
    {
        if(is_in_uint8(argv[i]) == false)
        {
            cerr << "有効な数字を入力してください\n";
            return(1);
        }
    }
    //little endianかどうかを順番に検索
    for(int j = 0; j < argc; j++)
    {

    }
    return(0);
}