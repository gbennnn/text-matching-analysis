#include <iostream>
using namespace std;

// Function to find the length of the longest common subsequence implemented using recursion
int lcs(string s1, string s2, int m, int n)
{
    if (m == 0 || n == 0)
    {
        return 0;
    }
    if (s1[m - 1] == s2[n - 1])
    {
        return 1 + lcs(s1, s2, m - 1, n - 1);
    }
    else
    {
        return max(lcs(s1, s2, m - 1, n), lcs(s1, s2, m, n - 1));
    }
}

int lcs_count(string s1, string s2, int m, int n, int &count)
{
    count++;
    if (m == 0 || n == 0)
    {
        return 0;
    }
    if (s1[m - 1] == s2[n - 1])
    {
        return 1 + lcs_count(s1, s2, m - 1, n - 1, count);
    }
    else
    {
        return max(lcs_count(s1, s2, m - 1, n, count), lcs_count(s1, s2, m, n - 1, count));
    }
}

void calculate_time_complexity(string s1, string s2)
{
    int count = 0;
    int m = s1.length();
    int n = s2.length();
    lcs_count(s1, s2, m, n, count);
    cout << "Number of function calls: " << count << endl;
}

#include <chrono>

void calculate_execution_time(string s1, string s2)
{
    auto start = chrono::high_resolution_clock::now();
    int m = s1.length();
    int n = s2.length();
    lcs(s1, s2, m, n);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
}

int main()
{
    string s1 = "AGGTAB";
    string s2 = "GXTXAYB";
    int m = s1.length();
    int n = s2.length();
    cout << "LCS: " << lcs(s1, s2, m, n) << endl;

    calculate_time_complexity(s1, s2);

    cout << endl;

    calculate_execution_time(s1, s2);

        return 0;
}