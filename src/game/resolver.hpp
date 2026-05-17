#pragma once

class Card;

enum class ResolveResult {
    FirstCard,
    NoMatch,
    Match
};

class Resolver {
    Card* card1_ = nullptr;
    Card* card2_ = nullptr;
public:
    Resolver();

    ResolveResult cardClicked(Card* card);
};
