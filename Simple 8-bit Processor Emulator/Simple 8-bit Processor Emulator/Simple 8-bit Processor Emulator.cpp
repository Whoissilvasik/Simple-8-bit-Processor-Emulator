#include <iostream>

using namespace std;

bool NAND(bool a, bool b)
{
    if (a == 0 && b == 0)
        return 1;
    if (a == 1 && b == 0)
        return 1;
    if (a == 0 && b == 1)
        return 1;
    if (a == 1 && b == 1)
        return 0;
}
bool NOT(bool a)
{
    return NAND(a, a);
}
bool AND(bool a, bool b)
{
    a = NAND(NAND(a, a), NAND(a, a));
    b = NAND(NAND(b, b), NAND(b, b));
    return NAND(NAND(a, b), NAND(a, b));
}
bool OR(bool a, bool b)
{
    a = NAND(a, a);
    b = NAND(b, b);
    return NAND(a, b);
}
bool XOR(bool a, bool b)
{
    bool temp = NAND(a, b);
    a = NAND(a, temp);
    b = NAND(temp, b);
    return NAND(a, b);
}
bool NOR(bool a, bool b)
{
    a = NAND(a, a);
    b = NAND(b, b);
    return NAND(NAND(a, b), NAND(a, b));
}
bool negative_B(bool b)
{
    b = NOT(b);
    return b;
}
void halfadder(bool a, bool b, bool& carry_n, bool& sum)
{
    sum = XOR(a, b);
    carry_n = AND(a, b);
}
void fulladder(bool a, bool b, bool& carry1, bool& carry2, bool& carry_in, bool& sum1, bool& sum)
{
    halfadder(a, b, carry1, sum1);
    halfadder(sum1, carry_in, carry2, sum);
    carry_in = OR(carry1, carry2);
}
void decrease(bool(&b)[8])
{
    for (int i = 7; i > -1; i--)
    {
        b[i] = NOT(b[i]);
        if (b[i] == 0)
            break;
    }
}
void shift(bool(&b)[8], bool direction)
{
    bool B[8] = { 0 };
    B[0] = AND(b[1], NOT(direction));
    B[1] = OR(AND(b[0], direction), AND(b[2], NOT(direction)));
    B[2] = OR(AND(b[1], direction), AND(b[3], NOT(direction)));
    B[3] = OR(AND(b[2], direction), AND(b[4], NOT(direction)));
    B[4] = OR(AND(b[3], direction), AND(b[5], NOT(direction)));
    B[5] = OR(AND(b[4], direction), AND(b[6], NOT(direction)));
    B[6] = OR(AND(b[5], direction), AND(b[7], NOT(direction)));
    B[7] = AND(b[6], direction);

    for (int i = 0; i < 8; i++)
    {
        b[i] = B[i];
    }
}
bool palyginimas(bool a[8], bool b[8])
{
    return OR(OR(OR(XOR(a[0], b[0]), XOR(a[1], b[1])), OR(XOR(a[2], b[2]), XOR(a[3], b[3]))), OR(OR(XOR(a[4], b[4]), XOR(a[5], b[5])), OR(XOR(a[6], b[6]), XOR(a[7], b[7]))));
}
void daugyba(bool(&a)[8], bool(&b)[8], bool(&s)[16], bool carry_in)
{
    bool carry1, sum1, carry2, sum2;
    int j = 0;
    bool allzero = 0;
    while (true)
    {
        if (b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0 && b[4] == 0 && b[5] == 0 && b[6] == 0 && b[7] == 0)
        {
            allzero = true;
        }
        if (allzero == true)
        {
            break;
        }
        if (b[7] != 0)
            for (int i = 7; i > -1; i--)
            {
                fulladder(s[i + (8 - j)], a[i - j], carry1, carry2, carry_in, sum1, sum2);
                s[i + (8 - j)] = sum2;
            }
        shift(b, 1);
        shift(a, 0);
        j++;
    }
}

int main()
{
    bool A[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    bool B[8] = { 0, 0, 0, 1, 0, 0, 0, 0 };
    bool Sig_a, Sig_b, Sig_c, ENA = 1, ENB = 1, carry_in = 0;

    cin >> Sig_a >> Sig_b >> Sig_c;

    for (int i = 0; i < 8; i++)
    {
        A[i] = AND(A[i], ENA);
        B[i] = AND(B[i], ENB);
    }

    if (AND(NOR(Sig_a, Sig_b), NOT(Sig_c)) == 1)
    {
        for (int i = 0; i < 8; i++)
        {
            cout << negative_B(B[i]);
        }
        cout << endl;
    }
    // 000 B -> !B 
    if (AND(NOR(Sig_a, Sig_b), Sig_c) == 1)
    {
        bool carry1, sum1, carry2, sum[8];

        for (int i = 7; i > -1; i--)
        {
            fulladder(A[i], B[i], carry1, carry2, carry_in, sum1, sum[i]);
        }
        for (int i = 0; i < 8; i++)
        {
            cout << sum[i];
        }
    }
    // 001 A+B
    if (AND(NOR(Sig_a, Sig_c), Sig_b) == 1)
    {
        bool rez[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        daugyba(A, B, rez, carry_in);

        for (int i = 0; i < 16; i++)
        {
            cout << rez[i];
        }
    }
    // 010 A*B
    if (AND(NOT(Sig_a), AND(Sig_b, Sig_c)) == 1)
    {
        shift(B, 1);

        for (int i = 0; i < 8; i++)
        {
            cout << B[i];
        }
    }
    // 011 B>>1 
    if (AND(Sig_a, AND(NOT(Sig_b), NOT(Sig_c))) == 1)
    {
        decrease(B);

        for (int i = 0; i < 8; i++)
        {
            cout << B[i];
        }
    }
    // 100 B-1 2
    if (AND(AND(Sig_a, Sig_c), NOT(Sig_b)) == 1)
    {
        for (int i = 0; i < 8; i++)
            B[i] = AND(B[i], 0);
        bool f = palyginimas(A, B);
        if (NOT(f) == 0)
        {
            cout << "True";
        }
        else
        {
            cout << " False ";
        }

        cout << endl << "F=" << NOT(f);
    }
    // 101 A!=0 
    return 0;
}