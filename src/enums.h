#ifndef KNAPSEN_H
#define KNAPSEN_H

class Knapsen
{
    
public:
    enum TypeOfCards{ GermanSuits, FrenchSuits };
    enum WhoStartGame{ AdminPlayer, OpponentPlayer, RandomPlayer };
    enum GameDifficulty{ Easy, Medium, Hard };
        
    enum GameStatus{ None, LocalMode, ClientMode, ServerMode };
};

#endif //KNAPSEN_H
