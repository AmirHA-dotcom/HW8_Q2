#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// -------------------REGEX-------------------


// -------------------DATA-------------------

class FlashCard
{
private:
    string question;
    string answer;
public:
    FlashCard(string q, string a) : question(q), answer(a) {}
    string get_Q () { return question; }
    string get_A () { return answer; }
};

// -------------------INT MAIN-------------------

int main()
{
    vector<FlashCard*> flashcards;
    string command;
    while (true)
    {
        getline(cin, command);

        // end program
        if (command == "END")
        {
            for (FlashCard* flashCard : flashcards)
                delete flashCard;
            flashcards.clear();
            cout << "ENDED!!!" << endl;
            return 0;
        }

        // add flash card
        else if (command == "add flashcard")
        {
            string question, answer;
            getline(cin, question);
            getline(cin, answer);
            flashcards.push_back(new FlashCard(question, answer));
            cout << "flashcard added to the daily box" << endl;
            cout << "Q : " << flashcards.back()->get_Q() << endl;
            cout << "A : " << flashcards.back()->get_A() << endl;
        }

        // invalid command
        else
        {
            cout << "Invalid Command" << endl;
        }
    }
}