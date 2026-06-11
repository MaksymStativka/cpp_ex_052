
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class text_reader
{
public:
    bool read_file(const string& filename, string& output_text)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Failed to open file" << endl;
            return false;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        output_text = buffer.str();
        return true;
    }
};

class text_processor
{
private:
    map<string, int> word_counts;
    vector<pair<string, int>> sorted_words;

    string clean(string text) const
    {
        unordered_set<char> symbols = { ',', '!', '(', ')', '?', '.', ':', ';', '-', '"', '\'', '/' };

        for (char& c : text)
        {
            c = tolower(static_cast<unsigned char>(c));
        }

        erase_if(text, [&symbols](char c)
            {
            return symbols.contains(c);
            });
        return text;
    }

    void sort_words()
    {
        sorted_words.assign(word_counts.begin(), word_counts.end());
        sort(sorted_words.begin(), sorted_words.end(), [](const pair<string, int>& a, const pair<string, int>& b)
            {
            return a.second > b.second;
            });
    }

public:

    void process_text(const string& text)
    {
        string clean_text = clean(text);
        stringstream text_stream(clean_text);
        string word;

        while (text_stream >> word)
        {
            word_counts[word]++;
        }

        sort_words();
    }

    const vector<pair<string, int>>& get_sorted_words() const
    {
        return sorted_words;
    }
};

class result_printer
{
public:
    void print_top_words(const vector<pair<string, int>>& sorted_words, int limit = 10) const
    {
        cout << "Top " << limit << " words" << endl;
        int actual_limit = min(limit, static_cast<int>(sorted_words.size()));

        for (int i = 0; i < actual_limit; ++i)
        {
            cout << i + 1 << ". " << sorted_words[i].first << ": " << sorted_words[i].second << endl;
        }
    }

    void print_unique_word_count(const vector<pair<string, int>>& sorted_words) const
    {
        cout << "Words that appear once" << endl;
        int amount = 0;

        for (const auto& [w, count] : sorted_words)
        {
            if (count == 1)
            {
                amount++;
            }
        }
        cout << amount << endl;
    }
};

int main()
{
    text_reader reader;
    text_processor processor;
    result_printer printer;

    string raw_text;

    if (reader.read_file("file.txt", raw_text))
    {
        processor.process_text(raw_text);

        const auto& results = processor.get_sorted_words();

        printer.print_top_words(results, 10);
        printer.print_unique_word_count(results);
    }

    return 0;
}