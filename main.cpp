#pragma once
#include "14956.h"
#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────────
// 1. Number: wraps a single int
// ─────────────────────────────────────────────
class Number : public Term
{
    int val;

public:
    Number(int v) : val(v) {}

    bool isNumber() const override { return true; }
    int asInt() const override { return val; }
    void print() const override { cout << val; }
    Term *clone() const override { return makeNumber(val); }
};

// ─────────────────────────────────────────────
// 2. Variable: wraps a string name
// ─────────────────────────────────────────────
class Variable : public Term
{
    string name;

public:
    Variable(const string &n) : name(n) {}

    string asStr() const override { return name; }
    void print() const override { cout << name; }
    Term *clone() const override { return makeVariable(name); }
};

// ─────────────────────────────────────────────
// 3. FuncCall: stores func name + owned child array
// ─────────────────────────────────────────────
class FuncCall : public Term
{
    string func;
    Term **children; // heap-allocated; we own these pointers
    int n;

public:
    // Constructor: args[] may be deleted right after this returns,
    // so allocate our own array and copy the pointers in.
    FuncCall(const string &func, Term **args, int n)
        : func(func), n(n)
    {
        children = new Term *[n];
        for (int i = 0; i < n; i++)
            children[i] = args[i];
    }

    // Destructor: we own the children — delete each, then the array.
    ~FuncCall() override
    {
        for (int i = 0; i < n; i++)
            delete children[i];
        delete[] children;
    }

    bool isFuncCall() const override { return true; }
    string asStr() const override { return func; }
    int arity() const override { return n; }
    Term *arg(int i) const override { return children[i]; }

    // print as:  func(arg0,arg1,...)
    void print() const override
    {
        cout << func << "(";
        for (int i = 0; i < n; i++)
        {
            if (i > 0) cout << ",";
            children[i]->print();
        }
        cout << ")";
    }

    // clone: deep-copy every child, build new FuncCall, delete temp array
    Term *clone() const override
    {
        Term **tmp = new Term *[n];
        for (int i = 0; i < n; i++)
            tmp[i] = children[i]->clone();
        Term *result = makeFuncCall(func, tmp, n);
        delete[] tmp; // makeFuncCall already copied the pointers internally
        return result;
    }
};

// ─────────────────────────────────────────────
// Factory functions
// ─────────────────────────────────────────────
Term *makeNumber(int v) { return new Number(v); }
Term *makeVariable(const string &name) { return new Variable(name); }
Term *makeFuncCall(const string &func, Term **args, int n) { return new FuncCall(func, args, n); }

// ─────────────────────────────────────────────
// evaluate: recursively simplify the expression
// ─────────────────────────────────────────────
Term *evaluate(const Term *t)
{
    // Base cases: number or variable → just clone
    if (!t->isFuncCall())
        return t->clone();

    int n = t->arity();

    // Recursively evaluate every child
    Term **evaled = new Term *[n];
    for (int i = 0; i < n; i++)
        evaled[i] = evaluate(t->arg(i));

    // Check if every evaluated child is a number
    bool allNums = true;
    for (int i = 0; i < n; i++)
        if (!evaled[i]->isNumber()) { allNums = false; break; }

    if (allNums)
    {
        // Fold into a single number
        string fname = t->asStr();
        int acc = (fname == "add") ? 0 : 1;
        for (int i = 0; i < n; i++)
        {
            if (fname == "add") acc += evaled[i]->asInt();
            else                acc *= evaled[i]->asInt();
        }

        // We produced heap-allocated children — delete them before returning
        for (int i = 0; i < n; i++)
            delete evaled[i];
        delete[] evaled;

        return makeNumber(acc);
    }

    // Not all numbers → return a new FuncCall that owns the evaluated children.
    // makeFuncCall copies the pointers internally, so we only delete the temp array.
    Term *result = makeFuncCall(t->asStr(), evaled, n);
    delete[] evaled; // delete temp array only, NOT the Term*s (FuncCall owns them now)
    return result;
}