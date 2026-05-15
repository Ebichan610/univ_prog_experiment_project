#include <iostream>
#include <cstdint>
using namespace std;

static void check_endian()
{
    int num = 1;

    if(*(uint8_t *)&num == 1)
        cout << "リトルエンディアンです。\n";
    else
        cout << "ビッグエンディアンです。\n";
}

int main()
{
    check_endian();
    return(0);
}
