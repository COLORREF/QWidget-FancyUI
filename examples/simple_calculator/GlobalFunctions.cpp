#include "GlobalFunctions.h"

QList<int> UniformDist_int(const int minRange, const int maxRange, const int quantity)
{
    assert(quantity >= 1);
    if (quantity < 1)
    {
        throw QException();
    }

    QList<int> RandNum;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniformDist(minRange, maxRange);

    for (int i = 0; i < quantity; i++)
    {
        RandNum.push_back(uniformDist(gen)); // 生成随机整数
    }
    return RandNum;
}
