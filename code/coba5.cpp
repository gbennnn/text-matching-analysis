#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <iomanip>
#include <sys/resource.h>

using namespace std;
using namespace std::chrono;

const int ch = 256;

// Function to match pattern with text
int match(const string &pattern, const string &text, int i, int pLength, int lineNumber)
{
    for (int j = 0; j < pLength; j++)
    {
        if (pattern[j] != text[i + j])
        {
            return -1;
        }
    }
    cout << "Pattern \"" << pattern << "\" ditemukan pada indeks " << i << " di baris nomor " << lineNumber << endl;
    return i;
}

// Function to recalculate hash value
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

// Function to check if a number is prime
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

// Rabin-Karp Algorithm
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
        int dHash = 0; // hash value for text

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
                    rabinKarpOutputFile << "Pattern \"" << pattern << "\" ditemukan pada indeks " << matchIndex << " di baris nomor " << lineNumber << endl;
                }
            }
            if (l < offset)
            {
                dHash = reHash(dHash, document, l, pLength, z, prime, ch);
            }
        }
    }
}

// Longest Common Subsequence (LCS) Algorithm
void lcsSearch(const string &pattern)
{
    cout << "\n\n================= HASIL PENCARIAN LCS UNTUK PATTERN : \"" << pattern << "\" =================\n\n"
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
            lcsOutputFile << "\nPanjang Pattern\t: " << c[m][n] << endl;
            cout << "Panjang Pattern\t: " << c[m][n] << endl;
            lcsOutputFile << "Similarity: " << similarity << "%" << endl;
            cout << "Similarity\t: " << similarity << "%" << endl;
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
            lcsOutputFile << "\nLCS: \"" << ans << "\" di baris nomor " << lcsSearchLineCount << endl;
            cout << "LCS\t\t: \"" << ans << "\" di baris nomor " << lcsSearchLineCount << endl;
            cout << endl;
        }
    }

    cout << "\nWaktu yang dibutuhkan untuk mencari pattern [\"" << pattern << "\"] adalah " << lcsSearchTime << " kali panggilan fungsi" << endl;
    lcsOutputFile << "\nWaktu yang dibutuhkan untuk mencari LCS: " << lcsSearchTime << " kali panggilan fungsi" << endl;
}

// Function to calculate execution time
template <typename Func>
long long measureExecutionTime(Func &&func)
{
    auto start = high_resolution_clock::now();
    func();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

// Function to estimate memory usage (approximate)
size_t estimateMemoryUsage()
{
    // Get current RSS (Resident Set Size) in bytes
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss * 1024; // Convert from kilobytes to bytes
}

// Function to calculate accuracy (for Rabin-Karp)
double calculateRabinKarpAccuracy(const string &pattern, const vector<string> &actualMatches)
{
    // Assuming actualMatches contains the correct indices of all occurrences of the pattern

    vector<int> rabinKarpMatches;
    rabinKarpSearch(pattern); // This function modifies rabinKarpMatches internally
    // ... (Implement logic to extract matched indices from rabinKarpOutputFile or modify rabinKarpSearch() to populate rabinKarpMatches) ...

    int truePositives = 0;
    for (int match : rabinKarpMatches)
    {
        if (find(actualMatches.begin(), actualMatches.end(), to_string(match)) != actualMatches.end())
        {
            truePositives++;
        }
    }

    if (rabinKarpMatches.empty())
    {
        return actualMatches.empty() ? 1.0 : 0.0; // Handle cases with no matches
    }

    return static_cast<double>(truePositives) / rabinKarpMatches.size();
}

int main()
{
    string patternLine;
    ifstream patternFile("pattern.txt");

    while (getline(patternFile, patternLine, ' '))
    {
        if (!patternLine.empty())
        {

            // Measure LCS execution time and memory usage
            long long lcsTime = measureExecutionTime([&]
                                                     { lcsSearch(patternLine); });
            size_t lcsMemory = estimateMemoryUsage();

            // Measure Rabin-Karp execution time and memory usage
            long long rabinKarpTime = measureExecutionTime([&]
                                                           { rabinKarpSearch(patternLine); });
            size_t rabinKarpMemory = estimateMemoryUsage();

            // Calculate Rabin-Karp accuracy
            // (Assuming you have a way to obtain actualMatches from an external source)
            vector<string> actualMatches;
            // ... (Populate actualMatches with the correct indices) ...
            double rabinKarpAccuracy = calculateRabinKarpAccuracy(patternLine, actualMatches);

            // Display results
            cout << "\nPattern: " << patternLine << endl;
            cout << "LCS:" << endl;
            cout << "\tExecution Time: " << lcsTime << " microseconds" << endl;
            cout << "\tMemory Usage: " << lcsMemory << " bytes" << endl;
            cout << "Rabin-Karp:" << endl;
            cout << "\tExecution Time: " << rabinKarpTime << " microseconds" << endl;
            cout << "\tMemory Usage: " << rabinKarpMemory << " bytes" << endl;
            cout << "\tAccuracy: " << fixed << setprecision(2) << rabinKarpAccuracy * 100 << "%" << endl;
        }
    }

    patternFile.close();

    return 0;
}