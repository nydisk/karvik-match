#include "resolver.hpp"

#include "card.hpp"
#include "spdlog/spdlog.h"

Resolver::Resolver() = default;

void Resolver::cardClicked(Card* card) {
    ResolveResult res;
    if (card1_ == nullptr) {
        card1_ = card;
        res = ResolveResult::FirstCard;
    } else {
        card2_ = card;
        res = card1_->id() == card2_->id() ? ResolveResult::Match : ResolveResult::NoMatch;
    }

    card->flip();

    switch (res) {
        case ResolveResult::FirstCard:

            break;
        case ResolveResult::NoMatch:

            break;
        case ResolveResult::Match:

            break;
    }

    if (res != ResolveResult::FirstCard) {
        card1_ = nullptr;
        card2_ = nullptr;
    }
}