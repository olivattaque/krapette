/*
 * Copyright (C) 1995 Paul Olav Tvete <paul@troll.no>
 * Copyright (C) 2000-2009 Stephan Kulow <coolo@kde.org>
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

#include "russianbank.h"

#include "dealerinfo.h"
#include "settings.h"
#include "pileutils.h"

#include <QStatusBar>

#include <KToggleAction>
#include <KSelectAction>
#include <KDebug>
#include <KLocalizedString>
#include <KMessageBox>
#include <KgDifficulty>

KrapettePlayer::KrapettePlayer(PatPile *reserve, PatPile *talon, PatPile *waste, bool human, QString name)
{
    m_reserve = reserve;
    m_talon = talon;
    m_waste = waste;
    m_human = human;
    m_name = name;
}
PatPile* KrapettePlayer::reserve() const
{
    return m_reserve;
}
PatPile* KrapettePlayer::talon() const
{
    return m_talon;
}
PatPile* KrapettePlayer::waste() const
{
    return m_waste;
}
bool KrapettePlayer::isHuman() const
{
    return m_human;
}
QString KrapettePlayer::name() const
{
    return m_name;
}
int KrapettePlayer::getTotalCards() const
{
    return m_talon->count() + m_reserve->count() + m_waste->count();
}


Krapette::Krapette( const DealerInfo * di )
  : DealerScene( di )
{
}

void Krapette::initialize()
{
    const qreal dist_x = 1.11;
    const qreal dist_y = 1.11;
    
    setDeckContents(2);
    
    PatPile *wasteJ1 = new PatPile( this, 0, "wasteJ1" );
    wasteJ1->setPileRole( PatPile::Waste );
    wasteJ1->setLayoutPos( 0, 0 );
    wasteJ1->setKeyboardSelectHint( KCardPile::NeverFocus );
    wasteJ1->setKeyboardDropHint( KCardPile::NeverFocus );
    
    PatPile *talonJ1 = new PatPile( this, 1, "talonJ1" );
    talonJ1->setPileRole(PatPile::Stock);
    talonJ1->setLayoutPos(dist_x, 0);
    talonJ1->setZValue( -52 );
    talonJ1->setKeyboardSelectHint( KCardPile::ForceFocusTop );
    talonJ1->setKeyboardDropHint( KCardPile::NeverFocus );
    
    PatPile *reserveJ1 = new PatPile( this, 2, "reserveJ1" );
    reserveJ1->setPileRole( PatPile::Cell );
    reserveJ1->setRightPadding( 1.1 );
    reserveJ1->setLayoutPos( dist_x * 2, 0 );
    reserveJ1->setKeyboardSelectHint( KCardPile::NeverFocus );
    reserveJ1->setKeyboardDropHint( KCardPile::NeverFocus );
    
    m_player1 = new KrapettePlayer(reserveJ1, talonJ1, wasteJ1, true, "player1");
        
    PatPile *wasteJ2 = new PatPile( this, 3, "wasteJ2" );
    wasteJ2->setPileRole( PatPile::Waste );
    wasteJ2->setLayoutPos( dist_x * 5, 0 );
    wasteJ2->setKeyboardSelectHint( KCardPile::NeverFocus );
    wasteJ2->setKeyboardDropHint( KCardPile::NeverFocus );
    
    PatPile *talonJ2 = new PatPile( this, 4, "talonJ2" );
    talonJ2->setPileRole(PatPile::Stock);
    talonJ2->setLayoutPos(dist_x * 6, 0);
    talonJ2->setZValue( -53 );
    talonJ2->setKeyboardSelectHint( KCardPile::ForceFocusTop );
    talonJ2->setKeyboardDropHint( KCardPile::NeverFocus );
    
    PatPile *reserveJ2 = new PatPile( this, 5, "reserveJ2" );
    reserveJ2->setPileRole( PatPile::Cell );
    reserveJ2->setRightPadding( 1.1 );
    reserveJ2->setLayoutPos( dist_x * 7, 0 );
    reserveJ2->setKeyboardSelectHint( KCardPile::NeverFocus );
    reserveJ2->setKeyboardDropHint( KCardPile::NeverFocus );
    
    m_player2 = new KrapettePlayer(reserveJ2, talonJ2, wasteJ2, false, "player2");
    
    for (int i = 0; i < 8; ++i )
    {
        m_target[i] = new PatPile( this, i + 6, QString("target%1").arg(i) );
        m_target[i]->setPileRole(PatPile::Foundation);
        m_target[i]->setLayoutPos(dist_x*(3+(i/4)), dist_y+(i%4)*dist_y);
        m_target[i]->setSpread(0, 0);
        m_target[i]->setKeyboardSelectHint( KCardPile::ForceFocusTop );
        m_target[i]->setKeyboardDropHint( KCardPile::ForceFocusTop );
    }
    for (int i = 0; i < 4; ++i )
    {
        m_play[i] = new PatPile( this, i + 10, QString( "play%1" ).arg( i ));
        m_play[i]->setPileRole(PatPile::Tableau);
        m_play[i]->setLayoutPos(dist_x * 2, dist_y+(i%4)*dist_y);
        m_play[i]->setAutoTurnTop(true);
        m_play[i]->setSpread(-0.5, 0);
        m_play[i]->setLeftPadding( 5 );
        m_play[i]->setWidthPolicy( KCardPile::GrowLeft );
        m_play[i]->setKeyboardSelectHint( KCardPile::AutoFocusDeepestFaceUp );
        m_play[i]->setKeyboardDropHint( KCardPile::AutoFocusTop );
    }
    for (int i = 4; i < 8; ++i )
    {
        m_play[i] = new PatPile( this, i + 14, QString( "play%1" ).arg( i ));
        m_play[i]->setPileRole(PatPile::Tableau);
        m_play[i]->setLayoutPos(dist_x * 5, dist_y+(i%4)*dist_y);
        m_play[i]->setAutoTurnTop(true);
        m_play[i]->setSpread(0.5, 0);
        m_play[i]->setRightPadding( 5 );
        m_play[i]->setWidthPolicy( KCardPile::GrowRight );
        m_play[i]->setKeyboardSelectHint( KCardPile::AutoFocusDeepestFaceUp );
        m_play[i]->setKeyboardDropHint( KCardPile::AutoFocusTop );
    }

    // SETTINGS
    m_compulsoryMovesToggleAction = new KToggleAction(i18n("&Compulsory moves"), this);
    m_compulsoryMovesToggleAction->setText("Compulsory moves");
    connect(m_compulsoryMovesToggleAction, &KToggleAction::triggered, this, &Krapette::toggleCompulsoryMoves);
    m_compulsoryMovesEnabled = Settings::krapetteCompulsoryMoves();
    m_compulsoryMovesToggleAction->setChecked( m_compulsoryMovesEnabled );

    m_movesShortcutsToggleAction = new KToggleAction(i18n("&Moves shortcuts"), this);
    m_movesShortcutsToggleAction->setText("Moves shortcuts");
    connect(m_movesShortcutsToggleAction, &KToggleAction::triggered, this, &Krapette::toggleMovesShortcuts);
    m_movesShortcutsEnabled = Settings::krapetteMovesShortcuts();
    m_movesShortcutsToggleAction->setChecked( m_movesShortcutsEnabled );
    
    m_aiSpeedSelectAction = new KSelectAction(i18n("AI speed"), this );
    m_aiSpeedSelectAction->addAction( i18n("Slow" ));
    m_aiSpeedSelectAction->addAction( i18n("Normal" ));
    m_aiSpeedSelectAction->addAction( i18n("Fast" ));
    connect(m_aiSpeedSelectAction, static_cast<void (KSelectAction::*)(int)>(&KSelectAction::triggered), this, &Krapette::setAISpeed);
    m_aiSpeedSelectAction->setCurrentItem( Settings::krapetteAISpeed() );
    m_aiDurationMove = aiSpeedTab[m_aiSpeedSelectAction->currentItem()];
    m_aiTimeBetweenMoves = aiSpeedTab[m_aiSpeedSelectAction->currentItem()];
    
    m_aiTimer.setSingleShot( true );
    connect(&m_aiTimer, &QTimer::timeout, this, &Krapette::tryMoveAI);
    
    //m_player1StatusLabel = new QLabel(QString(), statusBar());
    //m_player1StatusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    //statusBar()->addWidget( m_player1StatusLabel, 1);
    
    setActions(DealerScene::Hint | DealerScene::Demo | DealerScene::Draw);
    m_difficulty = new KgDifficulty(this);
    m_difficulty->addStandardLevelRange(KgDifficultyLevel::Easy, KgDifficultyLevel::Hard);
}

QList<QAction*> Krapette::configActions() const
{
    return QList<QAction*>() << m_compulsoryMovesToggleAction << m_movesShortcutsToggleAction << m_aiSpeedSelectAction;
}

KgDifficulty* Krapette::getKgDifficulty() const
{
    return m_difficulty;
}

void Krapette::restart( const QList<KCard*> &cards )
{
    m_cardsPlayed = QVector<KCard*>();
    QList<KCard*> cardList = cards;
    PatPile *m_player1Talon = m_player1->talon();
    PatPile *m_player2Talon = m_player2->talon();
    PatPile *m_player1Reserve = m_player1->reserve();
    PatPile *m_player2Reserve = m_player2->reserve();
    
    int bestCardByPlayer1, bestCardByPlayer2 = 2;
    int sumCardsByPlayer1, sumCardsByPlayer2 = 0;
    KCard *cardToDeal;
    // Tabled cards
    for (int i = 0; i < 4; ++i) {
        cardToDeal = cardList.takeLast();
        addCardForDeal( m_play[i], cardToDeal, true, m_player1Talon->pos());
        if (cardToDeal->rank() > bestCardByPlayer1) {
            bestCardByPlayer1 = cardToDeal->rank();
        }
        sumCardsByPlayer1 += cardToDeal->rank();
    }
    for (int i = 4; i < 8; ++i) {
        cardToDeal = cardList.takeLast();
        addCardForDeal( m_play[i], cardToDeal, true, m_player2Talon->pos());
        if (cardToDeal->rank() > bestCardByPlayer2) {
            bestCardByPlayer2 = cardToDeal->rank();
        }
        sumCardsByPlayer2 += cardToDeal->rank();
    }
    
    // Reserve
    for (int i = 0; i < 13; ++i) {
        addCardForDeal( m_player1Reserve, cardList.takeLast(), false, m_player1Talon->pos());
    }
    m_player1Reserve->topCard()->setFaceUp(true);
    for (int i = 0; i < 13; ++i) {;
        addCardForDeal( m_player2Reserve, cardList.takeLast(), false, m_player2Talon->pos());
    }
    m_player2Reserve->topCard()->setFaceUp(true);
    
    for (int i = 0; i < 35; ++i) {;
        KCard * c = cardList.takeFirst();
        c->setPos( m_player1Talon->pos() );
        c->setFaceUp( false );
        m_player1Talon->add( c );
    }
    for (int i = 0; i < 35; ++i) {;
        KCard * c = cardList.takeFirst();
        c->setPos( m_player2Talon->pos() );
        c->setFaceUp( false );
        m_player2Talon->add( c );
    }
    
    // Determine who start
    // 1 : Check highest reserve
    // 2 : Highest tabled card
    // 3 : Highest sum of tabled cards
    // 4 : Random
    const int player1ReserveRank = m_player1Reserve->topCard()->rank();
    const int player2ReserveRank = m_player2Reserve->topCard()->rank();
    if (player1ReserveRank == player2ReserveRank) {
        if (bestCardByPlayer1 != bestCardByPlayer2) {
            m_currentPlayer = (bestCardByPlayer1 > bestCardByPlayer2) ? m_player1 : m_player2;
        } else if (sumCardsByPlayer1 != sumCardsByPlayer2) {
            m_currentPlayer = (sumCardsByPlayer1 > sumCardsByPlayer2) ? m_player1 : m_player2;
        } else {
            m_currentPlayer = (std::rand() % 2 == 0 ? m_player1 : m_player2);
        }
    } else {
        m_currentPlayer = (player1ReserveRank > player2ReserveRank) ? m_player1 : m_player2;
    }
    connect( m_currentPlayer->talon(), &KCardPile::clicked, this, &DealerScene::drawDealRowOrRedeal );

    startDealAnimation();
}

bool Krapette::checkAdd(const PatPile *pile, const QList<KCard*> &oldCards, const QList<KCard*> &newCards) const
{
    // We do this check here instead of in the switch because
    // we have to do it before check compulsory moves
    if (pile->pileRole() == PatPile::Foundation 
        && checkAddSameSuitAscendingFromAce(oldCards, newCards)) {
        return true;
    }
    const int emptyPlayPiles = countEmptyPlayPiles();
    if (checkCompulsoryMoves()
        || (!getActiveReserve()->isEmpty() 
            && getActiveTalon()->topCard()->isFaceUp() 
            && emptyPlayPiles > 0)) {
        return false;
    }
    
    switch (pile->pileRole())
    {
    case PatPile::Tableau: {
        if (m_movesShortcutsEnabled) {
            return (pile->isEmpty() 
                    && Krapette::movesShortcuts[0][newCards.count()] <= emptyPlayPiles)
                    || (!pile->isEmpty() 
                        && Krapette::movesShortcuts[1][newCards.count()] <= emptyPlayPiles
                        && checkAddAlternateColorDescendingFromKing(oldCards, newCards));
        }
        return newCards.count() == 1 
            && (pile->isEmpty() || checkAddAlternateColorDescendingFromKing(oldCards, newCards));

    }
    case PatPile::Foundation:
        // never reached because of a previous same test
        return checkAddSameSuitAscendingFromAce(oldCards, newCards);
    case PatPile::Waste:{
        // Forbid to put more than 1 card
        if (newCards.count() != 1) {
            return false;
        }
        
        if(pile == getActiveWaste()) {
            // Forbid to put a card on our waste if there is empty tables piles
            if(emptyPlayPiles > 0) {
                return false;
            }
            // Forbid to put from our reserve to our waste
            if(newCards.first()->pile() == getActiveReserve()) {
                return false;
            }
            // Forbid to put a tabled card on our waste
            for (int i = 0; i < 8; i++) {
                if (newCards.first()->pile() == m_play[i]) {
                    return false;
                }
            }
            return true;
        } else {
            // Forbid to put card to an opponent empty waste
            if (pile->isEmpty()) {
                return false;
            }
            // Forbid to put another card to opponent waste if our talon is face up
            if (!getActiveTalon()->isEmpty()
                && getActiveTalon()->topCard()->isFaceUp() 
                && newCards.first() != getActiveTalon()->topCard()) {
                return false;
            }
        }
        
        const QList<KCard*> subCards({oldCards.last(), newCards.first()});
        return newCards.count() == 1 
                && pile != getActiveWaste() 
                && (isSameSuitAscending(subCards) || isSameSuitDescending(subCards));
    }
    case PatPile::Cell:{
        const QList<KCard*> subCards({oldCards.last(), newCards.first()});
        return newCards.count() == 1 
                && pile != getActiveReserve()
                && (isSameSuitAscending(subCards) || isSameSuitDescending(subCards));
    }
    case PatPile::Stock:
    default:
        return false;
    }
}

bool Krapette::checkRemove(const PatPile *pile, const QList<KCard*> &cards) const
{
    Q_UNUSED( cards );
    
    switch (pile->pileRole())
    {
    case PatPile::Tableau:
        return getActiveTalon()->isEmpty()
                || (!getActiveTalon()->isEmpty() && !getActiveTalon()->topCard()->isFaceUp());
    case PatPile::Foundation:
        return false;
    case PatPile::Waste:
        return false;
    case PatPile::Cell:
        return pile == getActiveReserve() && !getActiveTalon()->topCard()->isFaceUp();
    case PatPile::Stock:
        return true;
    default:
        return false;
    }
}

void Krapette::cardsMoved( const QList<KCard*> &cards, KCardPile *oldPile, KCardPile *newPile )
{
    DealerScene::cardsMoved( cards, oldPile, newPile );
    
    if (isGameWon() || isGameLost()) {
        return;
    }
    
    m_cardsPlayed.append(cards.last());
    
    const PatPile* oldPatPile = dynamic_cast<PatPile*>(oldPile);
    const PatPile* newPatPile = dynamic_cast<PatPile*>(newPile);
    
    if (oldPatPile->pileRole() == PatPile::Cell && !oldPatPile->isEmpty()) {
        oldPile->topCard()->setFaceUp(true);
    }
    if (newPatPile == getActiveWaste()) {
        changePlayer();
    }
}

void Krapette::animationDone()
{
    DealerScene::animationDone();
    
    checkDrawActionPossible();
    
    startAI();
}

void Krapette::startAI()
{
    if(!m_currentPlayer->isHuman()) {
        m_aiTimer.start( m_aiTimeBetweenMoves );
    }
}

bool Krapette::checkDrawActionPossible() {
    bool possible = m_currentPlayer->isHuman()
                        && ((!getActiveTalon()->isEmpty() 
                                && !getActiveTalon()->topCard()->isFaceUp())
                            || (getActiveTalon()->isEmpty() 
                                && getActiveWaste()->count() > 0));
    possible = (!getActiveReserve()->isEmpty() && countEmptyPlayPiles() > 0) ? false : possible;
    emit newCardsPossible(possible);
    return possible;
}

bool Krapette::newCards()
{
    // Can't pickup if there is compulsory moves to do
    if (checkCompulsoryMoves() 
        || (!getActiveReserve()->isEmpty() && countEmptyPlayPiles() > 0)) {
        return false;
    }

    if (getActiveTalon()->isEmpty()) {
        flipCardsToPile( getActiveWaste()->cards(), getActiveTalon(), DURATION_MOVE );
        emit newCardsPossible(true);
    } else {
        // Can't pickup if talon is already face up
        if (!getActiveTalon()->isEmpty() && getActiveTalon()->topCard()->isFaceUp()) {
            return false;
        }  
        getActiveTalon()->topCard()->setFaceUp(true);
        emit newCardsPossible(false);
        takeState();
    }
    
    startAI();

    return true;
}

bool Krapette::checkCompulsoryMoves() const
{
    if (m_compulsoryMovesEnabled) {
        for (int i = 0; i < 8; i++) {
            if (!getActiveTalon()->isEmpty() 
                && getActiveTalon()->topCard()->isFaceUp() 
                && checkAddCardToFoundation(getActiveTalon(), m_target[i])) {
                    return true;
            }
            if (checkAddCardToFoundation(getActiveReserve(), m_target[i])) {
                return true;
            }
            
            // Check if we can add a card to the foundation from tabled cards
            for (int j = 0; j < 8; j++) {
                if (checkAddCardToFoundation(m_play[j], m_target[i])) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Krapette::checkAddCardToFoundation(PatPile * ownPile, PatPile * destPile) const 
{
    if (!ownPile->isEmpty()) {
        if (ownPile->topCard()->rank() == KCardDeck::Ace
            && destPile->isEmpty()) {
            return true;
        }
        
        const QList<KCard*> cardTry({ownPile->topCard()});
        if (!destPile->isEmpty()
            && checkAddSameSuitAscendingFromAce(destPile->cards(), cardTry)) {
            return true;
        }
    }
    return false;
}

int Krapette::countEmptyPlayPiles() const
{
    int count = 0;
    for (int i = 0; i < 8; i++ ) {
        if (m_play[i]->isEmpty()) {
            count++;
        }
    }
    return count;
}

void Krapette::changePlayer()
{
    m_cardsPlayed = QVector<KCard*>();
    m_currentPlayer->talon()->disconnect();
    m_currentPlayer = (m_currentPlayer == m_player1) ? m_player2 : m_player1;
    connect( m_currentPlayer->talon(), &KCardPile::clicked, this, &DealerScene::drawDealRowOrRedeal );
}

KrapettePlayer* Krapette::getOpponent() const
{
    if (m_currentPlayer == m_player1) {
        return m_player2;
    }
    return m_player1;
}

PatPile* Krapette::getActiveReserve() const
{
    return m_currentPlayer->reserve();
}

PatPile* Krapette::getActiveTalon() const
{
    return m_currentPlayer->talon();
}

PatPile* Krapette::getActiveWaste() const
{
    return m_currentPlayer->waste();
}

void Krapette::setAISpeed()
{
    m_aiDurationMove = aiSpeedTab[m_aiSpeedSelectAction->currentItem()];
    m_aiTimeBetweenMoves = aiSpeedTab[m_aiSpeedSelectAction->currentItem()];
    Settings::setKrapetteAISpeed(m_aiSpeedSelectAction->currentItem());
}

void Krapette::toggleCompulsoryMoves(bool enabled)
{
    m_compulsoryMovesEnabled = enabled;
    Settings::setKrapetteCompulsoryMoves(enabled);
}

void Krapette::toggleMovesShortcuts(bool enabled)
{
    m_movesShortcutsEnabled = enabled;
    Settings::setKrapetteMovesShortcuts(enabled);
}

bool Krapette::isGameWon() const
{
    return getActiveReserve()->isEmpty() 
            && getActiveTalon()->isEmpty()
            && getActiveWaste()->isEmpty();
}

bool Krapette::isGameLost() const
{
    return getOpponent()->reserve()->isEmpty() 
            && getOpponent()->talon()->isEmpty()
            && getOpponent()->waste()->isEmpty();
}

///////////////////////////////////////////////
////////////////// IA /////////////////////////
///////////////////////////////////////////////
void Krapette::moveCardsToPile( const QList<KCard*> & cards, KCardPile * pile, int duration ) 
{
    PatPile* destPile = dynamic_cast<PatPile*>(pile);
    if (destPile->pileRole() == PatPile::Foundation && cards.first()->rank() == KCardDeck::Ace) {
        switch(cards.first()->suit())
        {
        case KCardDeck::Spades:
            pile = m_target[0]->isEmpty() ? m_target[0] : m_target[4];
            break;
        case KCardDeck::Hearts:
            pile = m_target[1]->isEmpty() ? m_target[1] : m_target[5];
            break;
        case KCardDeck::Clubs:
            pile = m_target[2]->isEmpty() ? m_target[2] : m_target[6];
            break;
        case KCardDeck::Diamonds:
            pile = m_target[3]->isEmpty() ? m_target[3] : m_target[7];
            break;
        }
    }
    DealerScene::moveCardsToPile( cards, pile, duration );
}

void Krapette::moveCardToPileByAI(KCard* card, PatPile * pile) 
{
    if (!m_testingMoves) {
        moveCardToPile( card, pile, m_aiDurationMove );
    }
}

void Krapette::tryMoveAI()
{
    if (m_currentPlayer->isHuman()) {
        return;
    }
        emit undoPossible( false );
    emit redoPossible( false );
    // Do compulsory moves first
    if (doCompulsoryMoves()) {
        return;
    }
    
    // Play the talon if the top card is face up
    if (!getActiveTalon()->isEmpty() && getActiveTalon()->topCard()->isFaceUp()) {
        KCard * cardToPlay = getActiveTalon()->topCard();
        if (putCardToPlayerPile(cardToPlay, getOpponent()->reserve())) {
            return;
        }
        if (putCardToPlayerPile(cardToPlay, getOpponent()->waste())) {
            return;
        }
        for (int i = 0; i < 8; i++) {
            if (putCardToTabledPile(cardToPlay, m_play[i])) {
                return;
            }
        }
        // no other choices discard
        moveCardToPileByAI( cardToPlay, getActiveWaste() );
        return;
    }
    
    // Try to put cards to opponent piles
    if (!getActiveReserve()->isEmpty()) {
        KCard * cardToPlay = getActiveReserve()->topCard();
        if (putCardToPlayerPile(cardToPlay, getOpponent()->reserve())) {
            return;
        }
        if (putCardToPlayerPile(cardToPlay, getOpponent()->waste())) {
            return;
        }
    }
    if (putTabledCardToPlayerPile(getOpponent()->reserve())) {
        return;
    }
    if (putTabledCardToPlayerPile(getOpponent()->waste())) {
        return;
    }
    
    // Play the reserve
    if (!getActiveReserve()->isEmpty()) {
        for (int i = 0; i < 8; i++) {
            if (putCardToTabledPile(getActiveReserve()->topCard(), m_play[i])) {
                return;
            }
        }
    }
    
    // Play the tabled
    for (int i = 0; i < 8; i++) {
        if (m_play[i]->isEmpty()) {
            continue;
        }
        if (m_cardsPlayed.last() == m_play[i]->topCard()) {
            kWarning() << "new equal";
            continue;
        }
        if (m_cardsPlayed.at(m_cardsPlayed.size()-3) == m_cardsPlayed.last()
            && m_cardsPlayed.at(m_cardsPlayed.size()-2) == m_play[i]->topCard()) {
            kWarning() << "new 2 move";
            continue;
        }
        for (int j = 0; j < 8; j++) {
            if (!m_play[j]->isEmpty() && m_play[i]->cards() == m_play[j]->cards()) {
                kWarning() << "same cards";
                continue;
            }
            if (m_play[i] != m_play[j] && !isMoveUseless(m_play[i], m_play[j])
                && putCardToTabledPile(m_play[i]->topCard(), m_play[j])) {
                return;
            }
        }
    }
    
    drawDealRowOrRedeal();
}

bool Krapette::tryMoveCardToPile(KCard * card) {
    m_testingMoves = true;
    
    if (putCardToPlayerPile(card, getOpponent()->reserve())) {
        m_testingMoves = false;
        return true;
    }
    if (putCardToPlayerPile(card, getOpponent()->waste())) {
        m_testingMoves = false;
        return true;
    }
    
    for (int i = 0; i < 8; i++) {
        if (!m_play[i]->isEmpty() && card->pile() != m_play[i]
            && putCardToTabledPile(card, m_play[i])) {
            m_testingMoves = false;
            return true;
        }
    }
    
    m_testingMoves = false;
    return false;
}

bool Krapette::isMoveUseless(PatPile * originPile, PatPile * destPile) 
{
    if (originPile->count() == 1 && destPile->isEmpty()) {
        kWarning() << "Move is useless !";
        return true;
    }
    //kWarning() << "originPile->count()" << originPile->count();
    if (originPile->count() > 1 && !tryMoveCardToPile(originPile->at(originPile->count() - 2))) {
        kWarning() << "Future move is useless !";
        return true;
    }
    return false;
}

bool Krapette::doCompulsoryMoves() 
{
     //if (m_compulsoryMovesEnabled) {
        for (int i = 0; i < 8; i++) {
            if (!getActiveTalon()->isEmpty() && getActiveTalon()->topCard()->isFaceUp()
                && checkAddCardToFoundation(getActiveTalon(), m_target[i])) {
                moveCardToPileByAI(getActiveTalon()->topCard(), m_target[i]);
                return true;
            }
            if (checkAddCardToFoundation(getActiveReserve(), m_target[i])) {
                moveCardToPileByAI(getActiveReserve()->topCard(), m_target[i]);
                return true;
            }
            
            // Check if we can add a card to the foundation from tabled cards
            for (int j = 0; j < 8; j++) {
                if (checkAddCardToFoundation(m_play[j], m_target[i])) {
                    moveCardToPileByAI(m_play[j]->topCard(), m_target[i]);
                    return true;
                }
            }
        }
    //}
    return false;
}

bool Krapette::putCardToTabledPile(KCard * card, PatPile * destPile)
{
    if (destPile->isEmpty()) {
        moveCardToPileByAI( card, destPile);
        return true;
    }

    const QList<KCard*> originCards({card});
    if (checkAddAlternateColorDescendingFromKing(destPile->cards(), originCards)) {
        moveCardToPileByAI( card, destPile);
        return true;
    }
    return false;
}

bool Krapette::putCardToPlayerPile(KCard * card, PatPile * opponentPile)
{
    if (!opponentPile->isEmpty()) {
        const QList<KCard*> subCards({card, opponentPile->topCard()});
        if (isSameSuitAscending(subCards) || isSameSuitDescending(subCards)) {
            moveCardToPileByAI( card, opponentPile );
            return true;
        }
    }
    return false;
}

bool Krapette::putTabledCardToPlayerPile(PatPile * opponentPile)
{
    if (!opponentPile->isEmpty()) {
        for(int i = 0; i < 8; i++) {
            if (!m_play[i]->isEmpty() && putCardToPlayerPile(m_play[i]->topCard(), opponentPile)) {
                return true;
            }
        }
    }
    return false;
}

QString Krapette::getGameState() const
{
    return m_currentPlayer->name();
}

void Krapette::setGameState( const QString & state )
{
    if(m_currentPlayer->name() != state) {
        //changePlayer();
        undo();
    }
    checkDrawActionPossible();
}

static class KrapetteDealerInfo : public DealerInfo
{
public:
    KrapetteDealerInfo()
      : DealerInfo(I18N_NOOP("Russian Bank"), RussianBankId )
    {}

    virtual DealerScene *createGame() const
    {
        return new Krapette( this );
    }
} krapetteDealerInfo;