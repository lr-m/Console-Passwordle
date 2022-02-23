#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// Gets a random number less than max
int randomNumber(int max)
{
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0.0, max);
    return dist(mt);
}

// Compares the guess and the word to determine each letters colour
bool submit_guess(string guess, string correct)
{
    static int guess_count = 0;
    guess_count++;

    int correct_count = 0;
    for (int i = 0; i < guess.size(); i++)
    {
        if (correct[i] == guess[i])
        {
            cout << "\x1B[32m" << guess[i] << "\033[0m ";
            correct_count++;
        }
        else if (correct.find(guess[i]) != string::npos)
        {
            cout << "\x1B[33m" << guess[i] << "\033[0m ";
        }
        else
        {
            cout << "\x1B[31m" << guess[i] << "\033[0m ";
        }
    }

    cout << '\n';

    if (correct_count == correct.length())
    {
        cout << "\n\x1B[32m"
                  << "You did it in " << guess_count << " guesses!"
                  << "\033[0m\n\n";
        return true;
    } else if (guess_count == correct.length()){
        cout << "\n\x1B[31m"
                  << "Unlucky!"
                  << "\033[0m\n\n";
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    // Print intro
    cout << "\n\033[1;31mWelcome \033[0m"
         << "\033[1;32mto \033[0m"
         << "\033[1;33mPasswordle\033[0m\n\n";

    int size = 0;

    // Get valid password size
    do
    {
        cout << "Enter password size (3-25): ";
        cin >> size;
    } while (size > 25 || size < 3);

    // Load wordlist
    ifstream file("wordlist.txt");

    vector<string> lines(1000000);
    vector<string> needed_length_lines(1000000);

    int current_option_count = 0;

    while (file.is_open())
    {
        for (auto i = 0; i < lines.size(); ++i)
        {
            getline(file, lines[i]);

            // If matches input length, store in needed length lines vector
            if (lines[i].length() == size)
            {
                needed_length_lines[current_option_count] = lines[i];
                current_option_count++;
            }
        }
        file.close();
    }

    // Get random word
    string word = needed_length_lines[randomNumber(current_option_count - 1)];

    cout << "\n\033[1;31mBegin Guessing!\033[0m\n\n";

    // Iterate until correct word found
    string guess;
    bool correct = false;

    do
    {
        cin >> guess;
        cout << "\e[A";
        if (guess.size() != size)
        {
            cout << "INVALID ENTRY";
            for (int i = 0; i < 100; i++){
                cout << ' ';
            }
            cout << "\n";
            continue;
        }
        correct = submit_guess(guess, word);
    } while (!correct || guess.length() != size);

    cout << "\x1B[33m"
                  << "The password was: " << word
                  << "\033[0m\n\n";
}