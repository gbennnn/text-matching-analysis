#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

int ch = 256;

int match(string pattern, string text, int i, int j, int pLength, int lineNumber)
{
    for (j = 0; j < pLength; j++)
    {
        if (pattern[j] != text[i + j])
        {
            break;
        }
    }

    if (j == pLength)
    {
        cout << "Pattern found at index " << i << " at line number " << lineNumber << "\n"
             << endl;
    }

    return i;
}

int reHash(int dHash, string text, int i, int pLength, int z, int prime)
{
    int f = dHash - text[i] * z;
    int g = text[i + pLength];

    dHash = (ch * (f) + g) % prime;

    if (dHash < 0)
    {
        dHash = dHash + prime;
    }

    return dHash;
}

bool setPrime(int rem)
{
    bool isPrime = true;
    int x = 0;

    for (x = 2; x <= rem / 2; x++)
    {
        if (rem % x == 0)
        {
            isPrime = false;
            break;
        }
    }

    return isPrime;
}

void lcsSearch(string line2)
{
    cout << "\n\n================= LCS SEARCH RESULT FOR PATTERN : \"" << line2 << "\" =================\n\n"
         << endl;

    string line1;
    string A;
    string B;

    int lcsSearchTime = 0;
    int lcsSearchLineCount = 0;
    int position = 0;

    ofstream lcsOutputFile("output_lcs.txt");
    ifstream lcsInputFile("dokumen.txt");

    while (getline(lcsInputFile, line1))
    {
        lcsSearchLineCount++;
        A = line1;
        B = line2;

        int m = A.length(); // length of string A
        int n = B.length(); // length of string B

        int similarity = 0;

        int c[m + 1][n + 1];  // array for storing length of LCS (for matrix)
        char b[m + 1][n + 1]; // array for storing direction of LCS (for matrix)
        c[0][0] = 0;

        for (int i = 1; i <= m; i++)
        {
            c[i][0] = 0;
        }

        for (int j = 1; j <= n; j++)
        {
            c[0][j] = 0;
        }

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                lcsSearchTime++; // count the number of function calls
                if (A[i - 1] == B[j - 1])
                {
                    c[i][j] = c[i - 1][j - 1] + 1;
                    b[i][j] = 'D'; // diagonal
                    position = i;
                }
                else if (c[i - 1][j] >= c[i][j - 1])
                {
                    c[i][j] = c[i - 1][j];
                    b[i][j] = 'U'; // up
                }
                else
                {
                    c[i][j] = c[i][j - 1];
                    b[i][j] = 'L'; // left
                }
            }
        }

        // Printing Longest Common Subsequence
        similarity = (c[m][n] * 100) / n;

        if (similarity > 30)
        {
            lcsOutputFile << "\nLength of Longest Common Subsequence: " << c[m][n] << endl;
            cout << "Length of Longest Common Subsequence: " << c[m][n] << endl;
            lcsOutputFile << "Similarity: " << similarity << "%" << endl;
            cout << "Similarity: " << similarity << "%" << endl;
        }

        int index = c[m][n];

        char ans[index]; // array for storing LCS

        int i = m, j = n;

        while (i > 0 && j > 0)
        {
            if (b[i][j] == 'D') // for every diagonal there is value of LCS
            {
                ans[index - 1] = A[i - 1];
                i--;
                j--;
                index--;
            }
            else if (b[i][j] == 'U')
            {
                i--;
            }
            else // L for Left
            {
                j--;
            }
        }

        if (similarity > 30)
        {
            lcsOutputFile << "\nLongest Common Subsequence: " << ans << " at line number " << lcsSearchLineCount << endl;
            cout << "Longest Common Subsequence: " << ans << " at line number " << lcsSearchLineCount << endl;

            // for (int i = 0; i < c[m][n]; i++)
            // {
            //     lcsOutputFile << ans[i];
            //     cout << ans[i];
            // }
            // lcsOutputFile << endl;
            // cout << endl;
        }
    }

    cout << "\nTime taken to search for LCS: " << lcsSearchTime << " function calls" << endl;
    lcsOutputFile << "\nTime taken to search for LCS: " << lcsSearchTime << " function calls" << endl;
}

void rabinKarpSearch(string pattern)
{
    // string document = "ABAAABCD";
    // string pattern = "ABC";

    string document;
    ofstream rabinKarpOutputFile("output_rabinKarp.txt");
    ifstream rabinKarpInputFile("dokumen.txt");

    int timeCount = 0;
    int lineNumber = 0;

    while (getline(rabinKarpInputFile, document))
    {
        lineNumber++;
        int s = 0;
        int prime = 0;

        cout << "\n\n================= RABIN KARP SEARCH RESULT FOR PATTERN : \"" << pattern << "\" =================\n\n"
             << endl;

        while (true)
        {
            timeCount++;
            s = rand() % 100 + 1985;
            if (setPrime(s))
            {
                prime = s;
                break;
            }
        }

        int pLength = pattern.length();
        int dLength = document.length();

        int j = 0;
        int pHash = 0; // hash value for pattern
        int dHash = 0; // hash value for document

        int z = 1; // hash variable

        for (int i = 1; i <= pLength - 1; i++)
        {
            z = (z * ch) % prime;
            timeCount++;
        }

        for (int j = 0; j < pLength; j++)
        {
            pHash = (ch * pHash + pattern[j]) % prime;
            dHash = (ch * dHash + document[j]) % prime;
            timeCount++;
        }

        int offset = dLength - pLength;
        int l = 0;

        while (l <= offset)
        {
            // to check if the hash values are equal
            if (pHash == dHash)
            {
                // if the hash values come out to be same
                // we can now check each character of both the string and pattern one by one
                timeCount++;
                match(pattern, document, l, j, pLength, lineNumber);
            }

            if (l < offset)
            {
                dHash = reHash(dHash, document, l, pLength, z, prime);
            }

            l++;
        }

        // moving character by character forward in the text input
        if (l < offset)
        {
            // we rehash the values if the pattern is not found
            dHash = reHash(dHash, document, l, pLength, z, prime);
        }
        l++;
    }
}

int main()
{
    char space = ' ';

    string patternLine;
    ifstream patternFile("pattern.txt");

    while (getline(patternFile, patternLine, space))
    {
        lcsSearch(patternLine);
        rabinKarpSearch(patternLine);
    }
    patternFile.close();

    return 0;
}