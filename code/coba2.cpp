#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int ch = 256;

// Fungsi untuk mencocokkan pola dengan teks
int match(const string &pattern, const string &text, int i, int pLength, int lineNumber)
{
    for (int j = 0; j < pLength; j++)
    {
        if (pattern[j] != text[i + j])
        {
            return -1;
        }
    }
    cout << "Pattern ditemukan pada indeks " << i << " di baris nomor " << lineNumber << endl;
    return i;
}

// Fungsi untuk menghitung ulang nilai hash
int reHash(int dHash, const string &text, int i, int pLength, int z, int prime, int ch)
{
    int f = dHash - text[i] * z;
    int g = text[i + pLength];
    dHash = (ch * f + g) % prime;
    if (dHash < 0)
    {
        dHash += prime;
    }
    return dHash;
}

// Fungsi untuk memeriksa apakah sebuah bilangan prima
bool isPrime(int num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}
void lcsSearch(const string &pattern)
{
    cout << "\n\n================= HASIL PENCARIAN LCS UNTUK POLA : \"" << pattern << "\" =================\n\n"
         << endl;

    string line;
    int lcsSearchTime = 0;
    int lcsSearchLineCount = 0;

    ofstream lcsOutputFile("output_lcs.txt");
    ifstream lcsInputFile("dokumen.txt");

    while (getline(lcsInputFile, line))
    {
        lcsSearchLineCount++;
        string A = line;
        string B = pattern;

        int m = A.length();
        int n = B.length();

        vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));
        vector<vector<char>> b(m + 1, vector<char>(n + 1, 0));

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                lcsSearchTime++;
                if (A[i - 1] == B[j - 1])
                {
                    c[i][j] = c[i - 1][j - 1] + 1;
                    b[i][j] = 'D'; // diagonal
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

        int similarity = (c[m][n] * 100) / n;
        if (similarity > 30)
        {
            lcsOutputFile << "\nPanjang LCS: " << c[m][n] << endl;
            cout << "Panjang LCS: " << c[m][n] << endl;
            lcsOutputFile << "Kesamaan: " << similarity << "%" << endl;
            cout << "Kesamaan: " << similarity << "%" << endl;
        }

        int index = c[m][n];
        string ans(index, ' ');

        int i = m, j = n;
        while (i > 0 && j > 0)
        {
            if (b[i][j] == 'D')
            {
                ans[--index] = A[i - 1];
                i--;
                j--;
            }
            else if (b[i][j] == 'U')
            {
                i--;
            }
            else
            {
                j--;
            }
        }

        if (similarity > 30)
        {
            lcsOutputFile << "\nLCS: " << ans << " di baris nomor " << lcsSearchLineCount << endl;
            cout << "LCS: " << ans << " di baris nomor " << lcsSearchLineCount << endl;
        }
    }

    cout << "\nWaktu yang dibutuhkan untuk mencari LCS: " << lcsSearchTime << " kali panggilan fungsi" << endl;
    lcsOutputFile << "\nWaktu yang dibutuhkan untuk mencari LCS: " << lcsSearchTime << " kali panggilan fungsi" << endl;
}

// Fungsi untuk mencari pola menggunakan algoritma Rabin-Karp
void rabinKarpSearch(const string &pattern)
{
    cout << "\n\n================= HASIL PENCARIAN RABIN KARP UNTUK POLA : \"" << pattern << "\" =================\n\n"
         << endl;

    string document;
    ofstream rabinKarpOutputFile("output_rabinKarp.txt");
    ifstream rabinKarpInputFile("dokumen.txt");

    int timeCount = 0;
    int lineNumber = 0;

    while (getline(rabinKarpInputFile, document))
    {
        lineNumber++;
        const vector<int> primes = {1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039};
        int prime = primes[rand() % primes.size()];
        int pLength = pattern.length();
        int dLength = document.length();

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

        for (int l = 0; l <= offset; l++)
        {
            if (pHash == dHash)
            {
                timeCount++;
                int matchIndex = match(pattern, document, l, pLength, lineNumber);
                if (matchIndex != -1)
                {
                    rabinKarpOutputFile << "Pattern ditemukan pada indeks " << matchIndex << " di baris nomor " << lineNumber << endl;
                }
            }
            if (l < offset)
            {
                dHash = reHash(dHash, document, l, pLength, z, prime, ch);
            }
        }
    }
}

int main()
{
    string patternLine;
    ifstream patternFile("pattern.txt");

    while (getline(patternFile, patternLine, ' '))
    {
        if (!patternLine.empty())
        {
            lcsSearch(patternLine);
            rabinKarpSearch(patternLine);
        }
    }
    patternFile.close();

    return 0;
}
// patternFile.close();
// patternFile.close();

// return 0;
// }
// while (getline(patternFile, patternLine))
