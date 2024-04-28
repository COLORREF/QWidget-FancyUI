#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
#include <QList>
#include <QException>
#include <random>   //标准库随机数库

QList<int> UniformDist_int(const int minRange, const int maxRange, const int quantity); //全局函数，返回整型随机数的list容器

#endif // GLOBALFUNCTIONS_H
