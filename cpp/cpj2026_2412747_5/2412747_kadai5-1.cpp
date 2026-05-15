#include <iostream>
#include <opencv2/opencv.hpp>

//1問目
void prog1()
{
    //行列を定義
    cv::Mat A = (cv::Mat_ <double>(3, 3) << 1, -3, 5, 3, 2, 4, 6, 4, 3);
    //転置行列を計算
    cv::Mat At = A.t();
    //出力
    std::cout << "A+A\u1D57 = " << A + At << "\n\n";
}

//2問目
void prog2()
{
    //行列を定義
    cv::Mat B = (cv::Mat_ <double>(3, 3) << 3, 2, 1, 1, 3, 2, 2, 1, 3);
    //行列式を計算
    double dl = cv::determinant(B);
    //出力
    std::cout << B << "\nの行列式の演算結果は、" << dl << "です。\n";
}

//3問目
void prog3()
{
    //行列を定義
    cv::Mat C = (cv::Mat_ <double>(2, 2) << 1, 2, 2, 3);
    //逆行列を計算
    cv::Mat C_inv = C.inv();
    //出力
    std::cout << C << "\nの逆行列は、\n" << C_inv << "\nです。\n";
}

int main()
{
    prog1();
    prog2();
    prog3();
    return(0);
}
