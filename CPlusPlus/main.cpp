// NOTE:
// Write production quality code
// We prefer clarity over performance (though if you can achieve both - great!)
// A working solution is preferred (assert in main() should succeed)
// Solutions using Modern C++ are preferred

// Reverse each word in the input string.
// The order of the words will be unchanged.
// A word is made up of letters and/or numbers.
// Other characters (spaces, punctuation) will not be reversed.

#include <string>
#include <assert.h>
#include <list>
#include <algorithm>
#include "ctype.h"

struct part
{
    part(bool word, const std::string& str)
    {
        isWord = word;
        string = str;
    }

    bool isWord;
    std::string string;
};

std::string reverse_words(const std::string &str)
{
    std::list<part*> substrList;
    std::string substr = "";

    bool extractingWord = false;
    bool extractingOther = false;

    for (auto c : str)
    {
        if (isalnum(c) != 0)
        {
            if (extractingOther)
            {
                // end of non-alnum character
                extractingOther = false;
                substrList.push_back(new part(false, substr));
                substr.clear();
            }

            extractingWord = true;
            substr.append(1, c);
        }
        else
        {
            if (extractingWord)
            {
                // end of alnum character
                extractingWord = false;
                substrList.push_back(new part(true, substr));
                substr.clear();
            }

            extractingOther = true;
            substr.append(1, c);
        }
    }

    std::string reversed;

    for (auto substr : substrList)
    {
        std::string str = substr->string;
        if (substr->isWord)
        {
            std::reverse(str.begin(), str.end());
        }
        reversed.append(str);

        delete substr;
    }

    if (!substr.empty() && isalnum(substr[0]) != 0)
    {
        std::reverse(substr.begin(), substr.end());
    }

    reversed.append(substr);

    return reversed;

}

int main()
{
    std::string test_str = "String;   2be reversed...";
    assert(reverse_words(test_str) == "gnirtS;   eb2 desrever...");
    return 0;
}
