#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// -------------------REGEX-------------------

regex review_today_PAT("^review today (\\d+)(\\s*)$");
regex get_report_in_period_PAT(R"(^get report (\d+) (\d+)(\s*)$)");
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

    void add_card_to_front(FlashCard* card)
    {
        flashcards.insert(flashcards.begin(), card);
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
    vector<pair<int, int>> report; // correct, wrong and one based
    int mastered_cards_count = 0;

    int streak = 0;
    bool reviewed_this_day = false;

    string command;
    while (getline(cin, command))
    {
        // add flash card
        if (command == "add flashcard")
        {
            string question, answer;
            getline(cin, question);
            getline(cin, answer);
            FlashCard* new_card = new FlashCard(question, answer);
            daily_box.add_card(new_card);
            cout << "flashcard added to the daily box" << endl;
//            cout << "Q : " << new_card->get_Q() << endl;
//            cout << "A : " << new_card->get_A() << endl;
        }

        // review cards
        else if (regex_match(command, match, review_today_PAT))
        {
            if (day != day_of_last_review)
            {
                day_of_last_review = day;
                review_queue.clear();
                // monthly cards
                if (day % 30 == 0)
                {
                    vector<FlashCard*> cards = monthly_box.take_all_cards();
                    for (FlashCard* card : cards)
                        review_queue.emplace_back(card, &monthly_box);
                }
                    // weekly cards
                if (day % 7 == 0)
                {
                    vector<FlashCard*> cards = weekly_box.take_all_cards();
                    for (FlashCard* card : cards)
                        review_queue.emplace_back(card, &weekly_box);
                }
                // daily cards
                vector<FlashCard*> cards = daily_box.take_all_cards();
                for (FlashCard* card : cards)
                    review_queue.emplace_back(card, &daily_box);
            }

            if (day >= report.size())
            {
                report.resize(day + 1, {0, 0});
            }
            // if the todays cards are done
            if (review_queue.empty())
            {
                cout << "You've completed today's review" << endl;
                continue;
            }
            // getting all the cards from that we need
            int number_of_cards_to_review = stoi(match[1]);
            vector<pair<FlashCard*, Box*>> session_cards;
            session_cards.assign(review_queue.begin(), review_queue.begin() + number_of_cards_to_review);
            review_queue.erase(review_queue.begin(), review_queue.begin() + number_of_cards_to_review);
            reviewed_this_day = true;

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
                    report[day].first++;
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
                        mastered_cards_count++;
                    }
                }
                // wrong answer
                else
                {
                    cout << "Your answer was incorrect. The correct answer is: " << card->get_A() << endl;
                    report[day].second++;
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

        // get report in a period
        else if (regex_match(command, match, get_report_in_period_PAT))
        {
            int day1 = stoi(match[1]), day2 = stoi(match[2]);
            if (day1 == day2)
            {
                if (day1 >= report.size())
                {
                    cout << "Day: " << day1 << endl;
                    cout << "Correct Answers: 0" << endl;
                    cout << "Incorrect Answers: 0" << endl;
                    cout << "Total: 0" << endl;
                } else {
                    cout << "Day: " << day1 << endl;
                    cout << "Correct Answers: " << report[day1].first << endl;
                    cout << "Incorrect Answers: " << report[day1].second << endl;
                    cout << "Total: " << report[day1].first + report[day1].second << endl;
                }
            }
            else
            {
                int correct = 0, incorrect = 0;
                for (int i = day1; i <= day2; i++)
                {
                    if (i < report.size())
                    {
                        correct += report[i].first;
                        incorrect += report[i].second;
                    }
                }

                cout << "Day: " << day1 << " to " << day2 << endl;
                cout << "Correct Answers: " << correct << endl;
                cout << "Incorrect Answers: " << incorrect << endl;
                cout << "Total: " << correct + incorrect << endl;
            }
        }

        // get progress report
        else if (command == "get progress report")
        {
            int total_participant = 0;
            for (auto& d : report)
            {
                if (d.first + d.second > 0)
                    total_participant++;
            }
            cout << "Challenge Progress Report:" << endl;
            cout << "Day of the Challenge: " << day << endl;
            cout << "Streak: " << streak << endl;
            cout << "Total Days Participated: " << total_participant << endl;
            cout << "Mastered Flashcards: " << mastered_cards_count << endl;
        }

        // start a new day
            // start a new day
        else if (command == "next day")
        {
            for (const auto& item : review_queue)
            {
                FlashCard* card = item.first;
                Box* source_box = item.second;

                if (source_box == &monthly_box) {
                    weekly_box.add_card_to_front(card);
                } else if (source_box == &weekly_box) {
                    daily_box.add_card_to_front(card);
                } else { // from daily_box
                    daily_box.add_card_to_front(card);
                }
            }
            review_queue.clear();

            if (reviewed_this_day)
            {
                streak++;
            }
            else
            {
                streak = 0;
                if (day % 30 == 0)
                {
                    vector<FlashCard*> cards = monthly_box.take_all_cards();
                    for (FlashCard* card : cards)
                    {
                        weekly_box.add_card_to_front(card);
                    }
                }
                if (day % 7 == 0)
                {
                    vector<FlashCard*> cards = weekly_box.take_all_cards();
                    for (FlashCard* card : cards)
                    {
                        daily_box.add_card_to_front(card);
                    }
                }
            }

            reviewed_this_day = false;
            day++;

            cout << "--------------------------------------------------" << endl;
            cout << "It is day " << day << " of your journey." << endl;
            cout << "Your current streak is: " << streak << endl;
        }

        // get streak
        else if (command == "streak")
            cout << "Your current streak is: " << streak << endl;

        // invalid command
        else
        {
            cout << "Invalid Command" << endl;
        }
    }
}