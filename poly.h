#ifndef POLY5_H
#define POLY5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定義圖片大小與多項式最大項數
#define IMG_WIDTH 800
#define IMG_HEIGHT 600
#define MAX_TERMS 100

// 單一項的結構：係數 (coef) 與指數 (exp)
typedef struct 
{
    double coef;
    double exp;
} Term;

// 多項式結構
typedef struct 
{
    Term terms[MAX_TERMS];
    int n; // 目前有多少項
} Poly;

// 初始化多項式
static void initPoly(Poly *p) 
	{ p->n = 0; }

// 新增一項到多項式中
static void addTerm(Poly *p, double coef, double exp) 
{
    if (p->n < MAX_TERMS) 
	{
        p->terms[p->n].coef = coef;
        p->terms[p->n].exp = exp;
        p->n++;
    }
}

// 計算多項式在 x 的值：f(x)
static double evaluate(Poly p, double x) 
{
    double sum = 0;
    for (int i = 0; i < p.n; i++) sum += p.terms[i].coef * pow(x, p.terms[i].exp);
    return sum;
}

// 微分：回傳一個新的多項式 f'(x)
static Poly derivative(Poly p) 
{
    Poly d; initPoly(&d);
    for (int i = 0; i < p.n; i++) 
        if (p.terms[i].exp != 0) // 常數項微分為 0，忽略
            addTerm(&d, p.terms[i].coef * p.terms[i].exp, p.terms[i].exp - 1);
    return d;
}

// 積分：回傳一個新的多項式 ∫f(x)dx
static Poly integral(Poly p, double C) 
{
    Poly in; initPoly(&in);
    for (int i = 0; i < p.n; i++) 
        addTerm(&in, p.terms[i].coef / (p.terms[i].exp + 1), p.terms[i].exp + 1);
    if (C != 0) addTerm(&in, C, 0); // 加入積分常數
    return in;
}

// 在終端機印出多項式字串
static void printPoly(Poly p) 
{
    if (p.n == 0) 
	{ printf("0\n"); return; }
    for (int i = 0; i < p.n; i++) 
	{
        double c = p.terms[i].coef; double e = p.terms[i].exp;
        if (i > 0 && c > 0) printf(" + "); else if (c < 0) printf(" - ");
        if (e == 0) printf("%.2f", fabs(c)); // 常數項
        else if (e == 1) printf("%.2fx", fabs(c)); // 一次項
        else printf("%.2fx^%.2f", fabs(c), e); // 高次項
    }
    printf("\n");
}

// 尋找特殊點 (order=1 找極值, order=2 找反曲點)
static Poly findPoints(Poly p, int order) 
{
    Poly res; initPoly(&res);
    Poly target = (order == 1) ? derivative(p) : derivative(derivative(p));
    
    //如果目標函數完全是 0，直接回傳空結果
    if (target.n == 0) return res; 
    int isZero = 1;
    for(int k=0; k<target.n; k++) 
	{
        if(fabs(target.terms[k].coef) > 0.000001) isZero = 0;
    }
    if(isZero) return res;

    for (double x = -10.0; x <= 10.0; x += 0.0001) 
	{
        if (fabs(evaluate(target, x)) < 0.005) 
		{
            if (res.n == 0 || fabs(res.terms[res.n-1].coef - x) > 0.5) 
			{
                double val = (fabs(x) < 0.005) ? 0.0 : x;
                addTerm(&res, val, 0);
            }
        }
    }
    return res;
}
//sakana


