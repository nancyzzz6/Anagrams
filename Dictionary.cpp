
#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <algorithm>//for sort
using namespace std;

void removeNonLetters(string& s);

class DictionaryImpl
{
private:
    int m_capacity;
    struct BUCKET
    {
        list<string>* wordsls;
    };

    BUCKET* m_buckets;
public:
    DictionaryImpl(int maxBuckets);
    ~DictionaryImpl();
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
    unsigned int hashFunc(string &word) const;
};



DictionaryImpl::DictionaryImpl(int maxBuckets)
{
    m_capacity = maxBuckets;
    m_buckets = new BUCKET[maxBuckets];
    for (int i = 0; i < maxBuckets; i++)
    {
        m_buckets[i].wordsls = new list<string>;
    }
}

DictionaryImpl::~DictionaryImpl()
{
    for (int i = 0; i < m_capacity; i++)
    {
        delete m_buckets[i].wordsls;
    }
    delete[] m_buckets;
}

unsigned int DictionaryImpl::hashFunc(string& word) const
{
    unsigned int hashValue = 0;
    int primeValue = 37;
    string sortedWord = word;
//    sort the word so every anagrams could have the same hash value
    sort(sortedWord.begin(), sortedWord.end());
    for (int i = 0; i < sortedWord.size(); i++)
    {
        hashValue = (hashValue * primeValue) + sortedWord[i] + i;
    }
    hashValue %= m_capacity;
    return hashValue;
}

//insert words by hashing
void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    if (word.empty())
        return;
    unsigned int bucketNum = hashFunc(word);
    m_buckets[bucketNum].wordsls->push_back(word);
}

//
void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
        return;
    
    removeNonLetters(letters);
    if (letters.empty())
        return;
    string sortedLetters = letters;
    sort(sortedLetters.begin(), sortedLetters.end());
    int bucket = hashFunc(sortedLetters);

    const list<string>& words = *(m_buckets[bucket].wordsls);
    for (list<string>::const_iterator it = words.begin(); it != words.end(); ++it)
    {
        const string& word = *it;
        string sortedWord = word;
        sort(sortedWord.begin(), sortedWord.end());
        if (sortedWord == sortedLetters)
            callback(word);
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
