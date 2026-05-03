#include <iostream>
using namespace std;

//行列クラス
class Matrix
{
    public:
        //行列のサイズ
        int width;
        int height;
        //コンストラクタ
        Matrix(int width, int height, int* elem);
        //コピーコンストラクタ
        Matrix(const Matrix& other);
        //デストラクタ
        ~Matrix();
        //行列演算用関数
        Matrix add(const Matrix& other) const;
        Matrix sub(const Matrix& other) const;
        Matrix mul(const Matrix& other) const;
        int    det() const;
        //出力用関数
        void   print() const;
    private:
        int **data;
        //余因子展開用関数
        Matrix getCofactor(int row, int col) const;
};

//コンストラクタ
Matrix::Matrix(int w, int h, int* elem)
    :width(w),
    height(h),
    //nullで初期化
    data(nullptr)
{
    try
    {
        data = new int* [height];
        //new失敗時のエラー処理対策
        for(int i = 0; i < height; i++)
            data[i] = nullptr;
        //各インデックスでメモリ確保
        for(int i = 0; i < height; i++)
        {
            data[i] = new int [width];
            for(int j = 0; j < width; j++)
                //行列(二次元配列)の値を無理矢理1次元配列で持っているので
                data[i][j] = elem[i * width + j];
        }
    }
    //メモリ確保失敗時の例外を拾った場合
    catch(bad_alloc& e)
    {
        //確保済みのメモリを解放
        if(data != nullptr)
        {
            //null埋めしているので安全
            for(int i = 0; i < height; i++)
                delete[] data[i];
            delete[] data;
        }
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        //呼び出し元に投げる
        throw;
    }
}

//コピーコンストラクタ(内容はコンストラクタとほぼ同様)
Matrix::Matrix(const Matrix& other)
    :width(other.width),
    height(other.height),
    data(nullptr)
{
    try
    {
        data = new int* [height];
        for(int i = 0; i < height; i++)
            data[i] = nullptr;
        for(int i = 0; i < height; i++)
        {
            data[i] = new int [width];
            for(int j = 0; j < width; j++)
                data[i][j] = other.data[i][j];
        }
    }
    catch(bad_alloc& e)
    {
        if(data != nullptr)
        {
            for(int i = 0; i < height; i++)
                delete[] data[i];
            delete[] data;
        }
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        throw;
    }
}

//デコンストラクタ(オブジェクトのメモリを解放)
Matrix::~Matrix()
{
    for(int i = 0; i < height; i++)
        delete[] data[i];
    delete[] data;
}

//行列の和をとる関数
Matrix Matrix::add(const Matrix& other) const
{
    //行列のサイズが不一致の時は例外を投げる
    if(width != other.width || height != other.height)
        throw invalid_argument("行列のサイズが合わないので計算できません\n");
    //ここでもtry-catch文でメモリエラー対策
    int *elem = nullptr;
    try
    {
        elem = new int[width * height];
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                //和の計算
                elem[i * width + j] = data[i][j] + other.data[i][j];
        //定義した要素で新たにクラスをインスタンス化
        Matrix result(width, height, elem);
        //不要になったので解放
        delete[] elem;
        return(result);
    }
    catch(bad_alloc& e)
    {
        delete[] elem;
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        throw;
    }
}

//行列の差をとる関数
Matrix Matrix::sub(const Matrix& other) const
{
    if(width != other.width || height != other.height)
        throw invalid_argument("行列のサイズが合わないので計算できません\n");
    int *elem = nullptr;
    //和と同様に処理
    try
    {
        elem = new int[width * height];
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                //差の計算
                elem[i * width + j] = data[i][j] - other.data[i][j];
        Matrix result(width, height, elem);
        delete[] elem;
        return(result);
    }
    catch(bad_alloc& e)
    {
        delete[] elem;
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        throw;
    }
}

//行列の積を計算する関数
Matrix Matrix::mul(const Matrix& other) const
{
    if(width != other.height)
        throw invalid_argument("行列のサイズが合わないので計算できません\n");
    int *elem = nullptr;
    try
    {
        elem = new int[height * other.width]();
        for(int i = 0; i < height; i++)
            for(int j = 0; j < other.width; j++)
                for(int k = 0; k < width; k++)
                    //積の計算
                    elem[i * other.width + j] += data[i][k] * other.data[k][j];
        //積の結果の行列のサイズはこのようになる
        Matrix result(other.width, height, elem);
        delete[] elem;
        return(result);
    }
    catch(bad_alloc& e)
    {
        delete[] elem;
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        throw;
    }
}

//余因子展開用の関数
Matrix Matrix::getCofactor(int row, int col) const
{
    int *elem = nullptr;
    try
    {
        //余因子なのでサイズは1つ小さくなる
        elem = new int[(width - 1) * (height - 1)];
        int idx = 0;
        for(int i = 0; i < height; i++)
        {
            if(i == row)
                continue;
            for(int j = 0; j < width; j++)
            {
                if(j == col)
                    continue;
                elem[idx] = data[i][j];
                idx++;
            }
        }
        Matrix result(width - 1, height - 1, elem);
        delete[] elem;
        return(result);
    }
    catch(bad_alloc& e)
    {
        delete[] elem;
        cerr << "メモリの確保に失敗しました:" << e.what() << '\n';
        throw;
    }
}

//行列式計算関数
int Matrix::det() const
{
    if(width != height)
        throw invalid_argument("正方行列でないので計算できません\n");
    if(height  == 1)
        return(data[0][0]);
    int result = 0;
    for(int j = 0; j < width; j++)
    {
        //三項演算子を用いて正負の符号を場合わけ
        int sign = (j % 2 == 0) ? 1 : -1;
        //行列式の計算
        //余因子展開の部分は再帰関数のような仕組みになっている
        result += sign * data[0][j] * getCofactor(0, j).det();
    }
    return(result);
}

//行列を出力する関数
void Matrix::print() const
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            cout << data[i][j] << "\t";
        cout << "\n";
    }
}

int main()
{
    //行列の値をint配列で定義
    int e1[] = {1, 2,
                3, 4};
    int e2[] = {5, 6,
                7, 8};
    //インスタンス化
    Matrix M1(2, 2, e1);
    Matrix M2(2, 2, e2);
    //出力部分(可読性のために各工程毎に一行に記述)
    cout << "M1:\n";        M1.print();
    cout << "M2:\n";        M2.print();
    cout << "M1 + M2:\n";  M1.add(M2).print();
    cout << "M1 - M2:\n";  M1.sub(M2).print();
    cout << "M1 * M2:\n";  M1.mul(M2).print();
    cout << "det(M1) = " << M1.det() << "\n";
    cout << "det(M2) = " << M2.det() << "\n";
    return(0);
}
