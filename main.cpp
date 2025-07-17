#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// -------------------REGEX-------------------

regex review_today_PAT("^review today (\\d+)(\\s*)$");
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

    vector<FlashCard*> take_all_cards()
    {
        vector<FlashCard*> temp;
        temp.swap(flashcards);
        return temp;
    }

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

    int day = 1;
    vector<pair<FlashCard*, Box*>> review_queue;
    int day_of_last_review = 0;

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
            daily_box.add_card(new_card);
            cout << "flashcard added to the daily box" << endl;
            cout << "Q : " << new_card->get_Q() << endl;
            cout << "A : " << new_card->get_A() << endl;
        }

        else if (regex_match(command, match, review_today_PAT))
        {
            if (day != day_of_last_review)
            {
                day_of_last_review = day;
                review_queue.clear();
            }
            // monthly cards
            if (day % 30 == 0)
            {
                vector<FlashCard*> cards = monthly_box.take_all_cards();
                for (FlashCard* card : cards)
                    review_queue.emplace_back(card, &monthly_box);
            }
            // weekly cards
            else if (day % 7 == 0)
            {
                vector<FlashCard*> cards = weekly_box.take_all_cards();
                for (FlashCard* card : cards)
                    review_queue.emplace_back(card, &weekly_box);
            }
            // daily cards
            vector<FlashCard*> cards = daily_box.take_all_cards();
            for (FlashCard* card : cards)
                review_queue.emplace_back(card, &daily_box);

            // if the todays cards are done
            if (review_queue.empty())
            {
                cout << "You've completed today's review!" << endl;
                continue;
            }
            // getting all the cards from that we need
            int number_of_cards_to_review = stoi(match[1]);
            vector<pair<FlashCard*, Box*>> session_cards;
            session_cards.assign(review_queue.begin(), review_queue.begin() + number_of_cards_to_review);
            review_queue.erase(review_queue.begin(), review_queue.begin() + number_of_cards_to_review);

            // showing the Qs
            cout << "Flashcards:" << endl;
            for (int i = 0; i < number_of_cards_to_review; ++i)
            {
                cout << session_cards[i].first->get_Q() << endl;
            }

            // getting answers
            cout << "Your Answers:" << endl;
            for (int i = 0; i < number_of_cards_to_review; i++)
            {
                string user_answer;
                getline(cin, user_answer);

                FlashCard* card = session_cards[i].first;
                Box* box = session_cards[i].second;
                // correct answer
                if (user_answer == card->get_A())
                {
                    cout << "Your answer was correct!" << endl;
                    if (box == &daily_box)
                    {
                        weekly_box.add_card(card);
                    }
                    else if (box == &weekly_box)
                    {
                        monthly_box.add_card(card);
                    }
                    else    // monthly
                    {
                        delete card;
                    }
                }
                // wrong answer
                else
                {
                    cout << "Your answer was incorrect. The correct answer is: " << card->get_A() << endl;
                    if (box == &daily_box)
                    {
                        daily_box.add_card(card);
                    }
                    else if (box == &weekly_box)
                    {
                        daily_box.add_card(card);
                    }
                    else
                    {
                        weekly_box.add_card(card);
                    }
                }
            }
        }

        // invalid command
        else
        {
            cout << "Invalid Command" << endl;
        }
    }
}