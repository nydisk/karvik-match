#include "resolver.hpp"

#include "card.hpp"
#include "spdlog/spdlog.h"

Resolver::Resolver() = default;

ResolveResult Resolver::cardClicked(Card* card) {
    if (card1_ == nullptr) {
        card1_ = card;
        return ResolveResult::FirstCard;
    }

    card2_ = card;
    return card1_->id() == card2_->id() ? ResolveResult::Match : ResolveResult::NoMatch;
}