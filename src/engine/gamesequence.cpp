#include "gamesequence.h"

GameSequence::GameSequence()
{
	mPlayers = 0;
	
	mCurrentPlayer = 0;
}

void GameSequence::setWhoStartGame( Knapsen::WhoStartGame whoStartGame )
{
	if( whoStartGame == Knapsen::RandomPlayer ){
		
		if( qrand()%2 == 0 ){
			whoStartGame = Knapsen::AdminPlayer;
		}else{
			whoStartGame = Knapsen::OpponentPlayer;
		}
		
	}
	
	if( whoStartGame == Knapsen::AdminPlayer ){
		//Becouse the first player always the admin
		mCurrentPlayer = 0;
	}else{ //whoStartGame == Knapsen::OpponentPlayer
		//Becouse the second player always the opponent
		mCurrentPlayer = 1;
	}
}

void GameSequence::setCurrentPlayer( Player* player )
{
	for( int i = 0; i < mPlayers->size(); ++i ){
		if( player->getName() == mPlayers->at( i )->getName() ){
			mCurrentPlayer = i;
		}
	}
}

Player* GameSequence::getNextPlayer()
{
	int nextPlayer = mCurrentPlayer;
	
	if( nextPlayer == 1 ){
		nextPlayer = 0;
	}else{ //nextPlayer == 0
		nextPlayer = 1;
	}
	
	return mPlayers->at( nextPlayer );
}

bool GameSequence::isRoundOver()
{
	//If a player have equal or more then 66 tricks, will have win the round
	for( int i = 0; i < mPlayers->size(); ++i ){
		if( mPlayers->at( i )->getTricks() >= 66 ){
			return true;
		}
	}
	
	//If a player have not card in hand, then win the round who win the last turn
	if( mPlayers->at( mCurrentPlayer )->getNumberOfCardsInHandNow()  == 0 ){
		return true;
	}
	
	return false;
}

bool GameSequence::isGameOver()
{
	for( int i = 0; i < mPlayers->size(); ++i ){
		if( mPlayers->at( i )->getScores() >= 7 ){
			return true;
		}
	}
	
	return false;
}
