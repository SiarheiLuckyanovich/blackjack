#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include <time.h>
#include <windows.h>

using namespace std;
//============================================================================================================
bool IsInteger (string number)
{
    for (size_t i = 0; i < number.length(); ++i)
    {
        if (isdigit(number[i]) == false) { return false; }
    }
    return true;
}
//============================================================================================================
void SetColor(int text, int background)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
ostream &endll(ostream &stream)
{
    stream << endl /*<< "End I can put anything I want here!!!!)))"*/ << endl;
    return stream;
}
//============================================================================================================
class Card
{
public:
enum  rank  { two = 2,three = 3,four = 4,five = 5,six = 6,seven = 7,eight =8,
                      nine =9,ten =10,JACK = 10,QUEEN = 10,KING = 10,ACE = 1};
enum  suit { DIAMONDS = 0, HEARTS = 1, CLUBS = 2, SPADES = 3 };
friend ostream& operator<<(ostream& os, const Card& aCard);
suit c_suit;
rank c_value;
bool c_isFaceUp = 1;
    Card (){}
    Card (suit m_suit , rank value ) : c_suit(m_suit), c_value(value) {}

    void Flip() { c_isFaceUp = !c_isFaceUp; };

    rank GetValue() { return c_value; };
    suit GetSuit() { return c_suit; };
};

//============================================================================================================
class Hand
{
public:
vector <Card*>  m_Cards;

    Hand (){}
    Hand (vector <Card*>  m_Cards) {}

    void Add (Card* pCard)
    {
        m_Cards.push_back(pCard);
    }

    void Clear ()
    {
        m_Cards.clear();
    }

    int GetTotal () const
    {
        int vsum=0;
        for (size_t i=0; i<m_Cards.size(); i++)
        {
            if (vsum < 21)
            {
                if ((static_cast <int> (m_Cards[i] -> c_value)) != 1)
                {
                    vsum += (static_cast <int> (m_Cards[i] -> c_value));
                }
                else if ((vsum + 10)>21)
                    {
                        vsum += (static_cast <int> (m_Cards[i] -> c_value));
                    }
                    else vsum += 11;
            } else vsum += (static_cast <int> (m_Cards[i] -> c_value));
        }
        return  vsum;
    }

    void printHand()
    {
        for (size_t i=0; i<m_Cards.size(); i++)
        {
            SetColor( 9, 0);
            cout << " ' "<< static_cast <char> (m_Cards[i] ->c_suit) << " ' ";
            cout << static_cast <int> (m_Cards[i] ->c_value) << " ' ";
            cout <<  m_Cards[i] -> c_isFaceUp << " ' " << endl;
        }
    }
};
//============================================================================================================
class GenericPlayer : public Hand
{
protected:
    string p_name;
    friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);
public:

    GenericPlayer (const string& name = "");

    virtual bool IsHitting()const = 0; /*{};*/ //- чисто виртуальная функция, возвращает информацию, нужна ли игроку еще одна карта.
    bool IsBusted()
    {
        if (GetTotal()>21)
        return true;
        else return false;
    } //- возвращает bool значение, есть ли у игрока перебор
    void Bust()
    {
        SetColor( 4, 0);
        if (IsBusted())
        cout << "The player " << p_name << " has busted! " << endl;
    } //выводит на экран имя игрока и объявляет, что у него перебор.
    virtual ~GenericPlayer (){}
};
GenericPlayer::GenericPlayer(const string& name ) :
p_name(name)
{}

//============================================================================================================
/*6.3. Реализовать класс Player, который наследует от класса GenericPlayer. У этого класса будет 4 метода:
• virtual bool IsHitting() const - реализация чисто виртуальной функции базового класса.
Метод спрашивает у пользователя,
нужна ли ему еще одна карта и возвращает ответ пользователя в виде true или false.
• void Win() const - выводит на экран имя игрока и сообщение, что он выиграл.
• void Lose() const - выводит на экран имя игрока и сообщение, что он проиграл.
• void Push() const - выводит на экран имя игрока и сообщение, что он сыграл вничью.*/
//============================================================================================================

class Player : public GenericPlayer
{
public:

    Player(const string& name = ""){p_name = name;}
    virtual ~Player(){};
    // показывает, хочет ли игрок продолжать брать карты
    virtual bool IsHitting() const;
    // объявляет, что игрок победил
    void Win() const;
    // объявляет, что игрок проиграл
    void Lose() const;
    // объявляет ничью
    void Push() const;
};
bool Player::IsHitting() const
{
    SetColor( 11, 0);
    cout << " Player " << p_name << ", do you want a hit? (Y/N): ";
    char response;
    cin >> response;
    SetColor( 9, 0);
    return (response == 'y' || response == 'Y');
}
void Player::Win() const
{SetColor( 13, 0);
cout << " Player " << p_name << " wins.\n";
}
void Player::Lose() const
{SetColor( 4, 0);
cout << " Player " << p_name << " loses.\n";
}
void Player::Push() const
{SetColor( 12, 0);
cout << " Player " << p_name << " pushes.\n";
}

//============================================================================================================
/*6.4. Реализовать класс House, который представляет дилера.
Этот класс наследует от класса GenericPlayer. У него есть 2 метода:
• virtual bool IsHitting() const - метод указывает, нужна ли дилеру еще одна карта.
Если у дилера не больше 16 очков, то он берет еще одну карту.
• void FlipFirstCard() - метод переворачивает первую карту дилера.*/
//============================================================================================================

class House : public GenericPlayer
{
public:
    House(const string& name = "House"){p_name = name;}
    virtual ~House(){};
    // показывает, хочет ли дилер продолжать брать карты
    virtual bool IsHitting() const;
    // переворачивает первую карту
    void FlipFirstCard();
};
bool House::IsHitting() const
{
    return (GetTotal() <= 16);
}
void House::FlipFirstCard()
{
    if (!(m_Cards.empty()))
    {
        m_Cards[0]->Flip();
    }
    else
    {
        cout << "No card to flip!\n";
    }
}


//============================================================================================================
/*6.5. Написать перегрузку оператора вывода для класса Card.
Если карта перевернута рубашкой вверх (мы ее не видим),
вывести ХХ, если мы ее видим, вывести масть и номинал карты.
Также для класса GenericPlayer написать перегрузку оператора вывода,
который должен отображать имя игрока и его карты, а также общую сумму очков его карт.*/
//============================================================================================================
ostream& operator<<(ostream& os, const Card& aCard)
{
const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
const string SUITS[] = { "Diamonds", "Hearts", "Clubs", "Spades" };
    if ( aCard.c_isFaceUp )
    {
        os << "|" << RANKS[static_cast <int>(aCard.c_value)] << " "
        << SUITS[static_cast <int> (aCard.c_suit)]<< "|";
    }
    else {os << " XX ";}
return os;
}
//============================================================================================================
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << "Player " << aGenericPlayer.p_name << ":   ";
    vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();
        pCard != aGenericPlayer.m_Cards.end(); ++pCard)
        {
            os << *(*pCard) << "   ";
        }
        if (aGenericPlayer.GetTotal() != 0)
        {
            cout << "Hand value = (" << aGenericPlayer.GetTotal() << ")" ;
        }
    }
    else
    {
        os << "<empty>" << endll;
    }
    return os;
}

//============================================================================================================
/*3. Создать класс Deck, который наследует от класса Hand и представляет собой колоду карт.
Класс Deck имеет 4 метода:
● vold Populate() - Создает стандартную колоду из 52 карт, вызывается из конструктора.
● void Shuffle() - Метод, который тасует карты, можно использовать функцию из
алгоритмов STL random_shuffle
● vold Deal (Hand& aHand) - метод, который раздает в руку одну карту
● void AddltionalCards (GenericPlayer& aGenerlcPlayer) - раздает игроку
дополнительные карты до тех пор, пока он может и хочет их получать
Обратите внимание на применение полиморфизма. В каких методах применяется этот
принцип ООП?*/
//============================================================================================================

class Deck : public Hand
{
public:
    Deck();
    virtual ~Deck();
    // создает стандартную колоду из 52 карт
    void Populate();
    // тасует карты
    void Shuffle();
    // раздает одну карту в руку
    void Deal(Hand& aHand);
    // дает дополнительные карты игроку
    void AdditionalCards(GenericPlayer& aGenericPlayer);
};
Deck::Deck()
{
    m_Cards.reserve(52);
    Populate();
}
Deck::~Deck(){}

void Deck::Populate()
{
    Clear();
    // создает стандартную колоду
    for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
    {
        for (int r = Card::ACE; r <= Card::KING; ++r)
        {
            Add(new Card( static_cast<Card::suit>(s), static_cast<Card::rank>(r) ));
        }
    }
}

void Deck::Shuffle()
{
    random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand)
{
    if (!m_Cards.empty())
    {
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    }
    else
    {
        cout << "Out of cards. Unable to deal.";
    }
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer)
{
    cout << endl;
    // продолжает раздавать карты до тех пор, пока у игрока не случается
    // перебор или пока он хочет взять еще одну карту
    while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting())
    {
        Deal(aGenericPlayer);
        cout << aGenericPlayer << endl;
        if (aGenericPlayer.IsBusted())
        {
            aGenericPlayer.Bust();
        }
    }
}


//============================================================================================================
/*7.4. Реализовать класс Game, который представляет собой основной процесс игры. У этого
класса будет 3 поля:
● колода карт
● рука дилера
● вектор игроков.
Конструктор класса принимает в качестве параметра вектор имен игроков и создает объекты
самих игроков. В конструкторе создается колода карт и затем перемешивается.
Также класс имеет один метод play(). В этом методе раздаются каждому игроку по две
стартовые карты, а первая карта дилера прячется. Далее выводится на экран информация о
картах каждого игра, в т.ч. и для дилера. Затем раздаются игрокам дополнительные карты.
Потом показывается первая карта дилера и дилер набирает карты, если ему надо. После
этого выводится сообщение, кто победил, а кто проиграл. В конце руки всех игроков
очищаются.*/
//============================================================================================================

class Game
{
public:
    Game(const vector<string>& names);
    ~Game();
// проводит игру в Blackjack
    void Play();
private:
    Deck m_Deck;
    House m_House;
    vector<Player> m_Players;
};
// Конструктор этого класса принимает ссылку на вектор строк, представляющих
// имена игроков-людей. Конструктор создает объект класса Player для каждогоимени
Game::Game(const vector<string>& names)
{
// создает вектор игроков из вектора с именами
    vector<string>::const_iterator pName;
    for (pName = names.begin(); pName != names.end(); ++pName)
    {
        m_Players.push_back(Player(*pName));
    }
// запускает генератор случайных чисел
    srand(static_cast<unsigned int>(time(0)));
    m_Deck.Populate();
    m_Deck.Shuffle();
}
Game::~Game(){}

void Game::Play()
{
    // раздает каждому по две стартовые карты
    vector<Player>::iterator pPlayer;
    for (int i = 0; i < 2; ++i)
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            m_Deck.Deal(*pPlayer);
        }
        m_Deck.Deal(m_House);
    }
// прячет первую карту дилера
    m_House.FlipFirstCard();
// открывает руки всех игроков
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        cout << *pPlayer << endl;
    }
    cout << m_House << endl;
// раздает игрокам дополнительные карты
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        m_Deck.AdditionalCards(*pPlayer);
    }
// показывает первую карту дилера
    m_House.FlipFirstCard();
    cout << endl << m_House;
// раздает дилеру дополнительные карты
    m_Deck.AdditionalCards(m_House);
    if (m_House.IsBusted())
    {
    // все, кто остался в игре, побеждают
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            if (!(pPlayer->IsBusted()))
            {
                pPlayer->Win();
            }
        }
    }
    else
    {
    // сравнивает суммы очков всех оставшихся игроков с суммой очковдилера
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            if (!(pPlayer->IsBusted()))
            {
                if (pPlayer->GetTotal() > m_House.GetTotal())
                {
                    pPlayer->Win();
                }
                else if (pPlayer->GetTotal() < m_House.GetTotal())
                {
                    pPlayer->Lose();
                }
                else
                {
                    pPlayer->Push();
                }
            }
        }
    }
// очищает руки всех игроков
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        pPlayer->Clear();
    }
    m_House.Clear();
}
//============================================================================================================
/*7.5. Написать функцию main() к игре Блекджек. В этой функции вводятся имена игроков.
Создается объект класса Game и запускается игровой процесс. Предусмотреть возможность
повторной игры.*/
//============================================================================================================

int main()
{
//============================================================================================================
SetColor( 8, 0);
system("cls");
    cout << endl;
    cout << endl;
    SetColor( 13, 0);
    cout << "                ---====<<";
    SetColor( 12, 0);
    cout << "The Blackjack game is begin!";
    SetColor( 13, 0);
    cout << " >>===---";
    cout << endl;
    SetColor( 14, 0);
    cout << "    ======================================================================" ;
    cout << endll;
SetColor( 8, 0);
string playerNum;
int iNum;
char answ;

do
{
SetColor( 8, 0);
system("cls");
    cout << endl;
    cout << endl;
    SetColor( 13, 0);
    cout << "                ---====<<";
    SetColor( 12, 0);
    cout << "The Blackjack game is begin!";
    SetColor( 13, 0);
    cout << " >>===---";
    cout << endl;
    SetColor( 14, 0);
    cout << "    ======================================================================" ;
    cout << endll;
SetColor( 8, 0);

    do
    {
        cout << "Enter the number of players (1-7): " ;
        cin >> playerNum ;
SetColor( 8, 0);
system("cls");
    cout << endl;
    cout << endl;
    SetColor( 13, 0);
    cout << "                ---====<<";
    SetColor( 12, 0);
    cout << "The Blackjack game is begin!";
    SetColor( 13, 0);
    cout << " >>===---";
    cout << endl;
    SetColor( 14, 0);
    cout << "    ======================================================================" ;
    cout << endll;
SetColor( 8, 0);
        if (IsInteger(playerNum) == false)
        {
            cerr << "Oops, you entered an invalid integer!" << endl;
        }
        else if ( (iNum = stoi(playerNum)) <= 0 || iNum > 7 )
        {
            cerr << "Oops, you entered an invalid number!" << endl;
        }
        else if ( iNum  < 2)
        {
            cout << "The game for " << iNum << " player." << endl;
        }
        else { cout << "The game for " << iNum << " players." << endl; }
    } while (IsInteger(playerNum) == false || iNum <= 0 || iNum > 7);

    int iNum = stoi(playerNum);
    vector<string> names;
    string name;
    for (int i = 0; i < iNum ; ++i)
    {
        SetColor( 7, 0);
        cout << "Enter player name: ";
        cin >> name;
        names.push_back(name);
    }
    cout << endl;
SetColor( 8, 0);
system("cls");
    cout << endl;
    cout << endl;
    SetColor( 13, 0);
    cout << "                ---====<<";
    SetColor( 12, 0);
    cout << "The Blackjack game is begin!";
    SetColor( 13, 0);
    cout << " >>===---";
    cout << endl;
    SetColor( 14, 0);
    cout << "    ======================================================================" ;
    cout << endll;
SetColor( 8, 0);
    SetColor( 9, 0);
    Game aGame(names);
    aGame.Play();

    SetColor( 10, 0);
    cout << "            Do you wanna play again? (say yes): ";
    cin >> answ;
} while (answ == 'y' || answ == 'Y');

cout << endl;
//============================================================================================================
return 0;
}
