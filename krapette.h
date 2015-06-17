/*
 * Copyright (C) 1995 Paul Olav Tvete <paul@troll.no>
 * Copyright (C) 2000-2009 Stephan Kulow <coolo@kde.org>
 * Copyright (C) 2010 Parker Coates <coates@kde.org>
 *
 * License of original code:
 * -------------------------------------------------------------------------
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation.
 *
 *   This file is provided AS IS with no warranties of any kind.  The author
 *   shall have no liability with respect to the infringement of copyrights,
 *   trade secrets or any patents by this file or any part thereof.  In no
 *   event will the author be liable for any lost revenue or profits or
 *   other special, indirect and consequential damages.
 * -------------------------------------------------------------------------
 *
 * License of modifications/additions made after 2009-01-01:
 * -------------------------------------------------------------------------
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------
 */

#ifndef KRAPETTE_H
#define KRAPETTE_H

#include "dealer.h"

class QLabel;

class KToggleAction;
class KSelectAction;

class KrapettePlayer;

class Krapette : public DealerScene
{
    Q_OBJECT

public:
    Krapette(const DealerInfo *di);
    virtual void initialize();
    QList<QAction*> configActions() const;
    bool isGameWon() const;
    bool isGameLost() const;

    enum AISpeed {
        AI_SLOW = 500,
        AI_NORMAL = 300,
        AI_FAST = 100
    };
    
public slots:
    virtual bool checkAdd(const PatPile *pile, const QList<KCard*> &oldCards, const QList<KCard*> &newCards) const;
    virtual bool checkRemove(const PatPile *pile, const QList<KCard*> &cards) const;
    virtual void cardsMoved(const QList<KCard*> &cards, KCardPile *oldPile, KCardPile *newPile);
    virtual void restart(const QList<KCard*> &cards);
    
protected:
    virtual void cardsDroppedOnPile( const QList<KCard*> & cards, KCardPile * pile );
    
protected slots:
    virtual void animationDone();
    virtual bool newCards();
    
private:
    const int movesShortcuts[2][12] = { {0,1,2,3,3,4,5,5,5,5,6,6},{0,0,1,2,2,3,3,3,3,4,4,4} };
    const int aiSpeedTab[3] = {Krapette::AI_SLOW, Krapette::AI_NORMAL, Krapette::AI_FAST};

    void toggleCompulsoryMoves(bool enabled);
    void toggleMovesShortcuts(bool enabled);
    void setCrapetteRule();
    void setAISpeed();
    KSelectAction *m_player1By;
    KSelectAction *m_player2By;
    KSelectAction *m_crapetteRuleSelectAction;
    KSelectAction *m_aiSpeedSelectAction;
    KToggleAction *m_compulsoryMovesToggleAction;
    KToggleAction *m_movesShortcutsToggleAction;
    bool m_movesShortcutsEnabled;
    bool m_compulsoryMovesEnabled;
    int m_crapetteRule;
    int m_aiDurationMove;
    int m_aiTimeBetweenMoves;
    QLabel *m_player1StatusLabel;

    void changePlayer();
    void player1Changed();
    void player2Changed();
    KrapettePlayer* getOpponent() const;
    PatPile* getActiveCrapette() const;
    PatPile* getActiveWaste() const;
    PatPile* getActiveTalon() const;
    int countEmptyPlayPiles() const;
    void moveCardsToPileCustom(QList<KCard*> cards, PatPile* pile, int duration);
    void moveCardToPileByAI(KCard *card, PatPile *pile);
    bool tryMoveCardToPile(KCard *card);
    bool checkCompulsoryMoves() const;
    bool doCompulsoryMoves();
    bool isMoveUseless(PatPile *originPile, PatPile *destPile);
    bool checkAddCardToFoundation(PatPile *ownPile, PatPile *destPile) const;
    bool putCardToTabledPile(KCard *card, PatPile *destPile);
    bool putCardToPlayerPile(KCard *card, PatPile *opponentPile);
    bool putTabledCardToPlayerPile(PatPile *opponentPile);
    void startAI();
    void tryMoveAI();
    
    QTimer m_aiTimer;
    QList<KCard*> m_cardsPlayed;
    bool m_testingMoves;
    KrapettePlayer *m_currentPlayer;
    KrapettePlayer *m_player1;
    KrapettePlayer *m_player2;
    PatPile *m_play[8];
    PatPile *m_target[8];
};


class KrapettePlayer
{
public:
    KrapettePlayer( PatPile *crapette, PatPile *talon, PatPile *waste, bool human );
    
    PatPile* crapette() const;
    PatPile* talon() const;
    PatPile* waste() const;
    bool isHuman() const;
    void toggleControl();
    int getTotalCards() const;
    
private:
    PatPile *m_crapette;
    PatPile *m_talon;
    PatPile *m_waste;
    bool m_human;
};

#endif
