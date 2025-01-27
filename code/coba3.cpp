#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>         // Untuk menghitung waktu eksekusi
#include <sys/resource.h> // Untuk menghitung penggunaan memori
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

// Fungsi untuk menghitung penggunaan memori saat ini
size_t getMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memori dalam kilobyte
}

void evaluatePerformance(const string &algorithm, int detected, int totalPatterns, double execTime, size_t memoryUsage)
{
    double accuracy = (double(detected) / totalPatterns) * 100;
    cout << "\n==================== EVALUASI KINERJA: " << algorithm << " ====================\n";
    cout << "Akurasi: " << accuracy << "%\n";
    cout << "Waktu Eksekusi: " << execTime << " detik\n";
    cout << "Penggunaan Memori: " << memoryUsage << " KB\n";
    cout << "==========================================================\n";
}

void lcsSearch(const string &pattern, int &detected, int &totalPatterns)
{
    string line;
    int lcsSearchLineCount = 0;

    ifstream lcsInputFile("dokumen.txt");

    // Mulai penghitungan waktu eksekusi
    auto startTime = chrono::high_resolution_clock::now();
    size_t memoryBefore = getMemoryUsage();

    while (getline(lcsInputFile, line))
    {
        lcsSearchLineCount++;
        string A = line;
        string B = pattern;

        int m = A.length();
        int n = B.length();

        vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (A[i - 1] == B[j - 1])
                {
                    c[i][j] = c[i - 1][j - 1] + 1;
                }
                else
                {
                    c[i][j] = max(c[i - 1][j], c[i][j - 1]);
                }
            }
        }

        int similarity = (c[m][n] * 100) / n;
        if (similarity > 30)
        {
            detected++;
        }
    }

    totalPatterns++;

    // Hitung waktu eksekusi dan penggunaan memori
    auto endTime = chrono::high_resolution_clock::now();
    double execTime = chrono::duration<double>(endTime - startTime).count();
    size_t memoryAfter = getMemoryUsage();

    evaluatePerformance("LCS", detected, totalPatterns, execTime, memoryAfter - memoryBefore);
}

void rabinKarpSearch(const string &pattern, int &detected, int &totalPatterns)
{
    string document;
    ifstream rabinKarpInputFile("dokumen.txt");

    // Mulai penghitungan waktu eksekusi
    auto startTime = chrono::high_resolution_clock::now();
    size_t memoryBefore = getMemoryUsage();

    while (getline(rabinKarpInputFile, document))
    {
        const vector<int> primes = {1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039};
        int prime = primes[rand() % primes.size()];
        int pLength = pattern.length();
        int dLength = document.length();

        int pHash = 0;
        int dHash = 0;

        int z = 1;

        for (int i = 1; i <= pLength - 1; i++)
        {
            z = (z * ch) % prime;
        }

        for (int j = 0; j < pLength; j++)
        {
            pHash = (ch * pHash + pattern[j]) % prime;
            dHash = (ch * dHash + document[j]) % prime;
        }

        int offset = dLength - pLength;

        for (int l = 0; l <= offset; l++)
        {
            if (pHash == dHash)
            {
                int matchIndex = match(pattern, document, l, pLength, l);
                if (matchIndex != -1)
                {
                    detected++;
                }
            }
            if (l < offset)
            {
                dHash = reHash(dHash, document, l, pLength, z, prime, ch);
            }
        }
    }

    totalPatterns++;

    // Hitung waktu eksekusi dan penggunaan memori
    auto endTime = chrono::high_resolution_clock::now();
    double execTime = chrono::duration<double>(endTime - startTime).count();
    size_t memoryAfter = getMemoryUsage();

    evaluatePerformance("Rabin-Karp", detected, totalPatterns, execTime, memoryAfter - memoryBefore);
}

int main()
{
    string patternLine;
    ifstream patternFile("pattern.txt");

    int lcsDetected = 0, rkDetected = 0;
    int totalPatterns = 0;

    while (getline(patternFile, patternLine, ' '))
    {
        if (!patternLine.empty())
        {
            lcsSearch(patternLine, lcsDetected, totalPatterns);
            rabinKarpSearch(patternLine, rkDetected, totalPatterns);
        }
    }
    patternFile.close();

    return 0;
}
