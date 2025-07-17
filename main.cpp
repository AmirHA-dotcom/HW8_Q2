#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// -------------------REGEX-------------------

regex review_today_PAT("^review today (\\d+)(\\s*)$)");
smatch match;

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

class Box
{
protected:
    vector<FlashCard*> flashcards;
public:
    void add_card(FlashCard* card)
    {
        flashcards.push_back(card);
    }
    vector<FlashCard*> get_flashcards() { return flashcards; }
    virtual ~Box()
    {
        for (FlashCard* card : flashcards)
        {
            delete card;
        }
        flashcards.clear();
    }
};

class Daily : public Box
{

};

class Weekly : public Box
{

};

class Monthly : public Box
{

};

// -------------------INT MAIN-------------------

int main()
{
    Daily daily_box;
    Weekly weekly_box;
    Monthly monthly_box;

    string command;
    while (true)
    {
        getline(cin, command);

        // end program
        if (command == "END")
        {
            cout << "ENDED!!!" << endl;
            return 0;
        }

        // add flash card
        else if (command == "add flashcard")
        {
            string question, answer;
            getline(cin, question);
            getline(cin, answer);
            FlashCard* new_card = new FlashCard(question, answer);
            daily_box.get_flashcards().push_back(new_card);
            cout << "flashcard added to the daily box" << endl;
            cout << "Q : " << new_card->get_Q() << endl;
            cout << "A : " << new_card->get_A() << endl;
        }

        else if (regex_match(command, match, review_today_PAT))
        {
            cout << "Flashcards:" << endl;

        }

        // invalid command
        else
        {
            cout << "Invalid Command" << endl;
        }
    }
}