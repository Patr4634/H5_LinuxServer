#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void AddLetters(const string &word, vector<char> letters, vector<string> *candidates)
{
    for (int i = 0; i < letters.size(); ++i)
        for (int j = 0; j < word.length() + 1; ++j)
            candidates->push_back(string(word).insert(j, string(1, letters[i])));
}

void RemoveLetters(const string &word, vector<string> *candidates)
{
    for (int i = 0; i < word.length(); ++i)
        candidates->push_back(string(word).erase(i, 1));
}

void ReplaceLetters(const string &word, vector<char> letters, vector<string> *candidates)
{
    for (int i = 0; i < letters.size(); ++i)
        for (int j = 0; j < word.length(); ++j)
            candidates->push_back(string(word).replace(j, 1, string(1, letters[i])));
}

void SwapLetters(const string &word, vector<string> *candidates)
{
    for (int i = 0; i < word.length() - 1; ++i)
        candidates->push_back(string(word).replace(i, 2, string(1, word[i + 1]).append(string(1, word[i]))));
}

void Squared(vector<char> letters, vector<string> *candidates)
{
    string tempWord;

    for (int i = 0; i < candidates->size(); i++)
    {
        tempWord = candidates->at(i);
        AddLetters(tempWord, letters, candidates);
        RemoveLetters(tempWord, candidates);
        ReplaceLetters(tempWord, letters, candidates);
        SwapLetters(tempWord, candidates);
    }
}