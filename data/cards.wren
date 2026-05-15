import "core" for Core, Card

var cards = {
    "alien1":       Card.new("alien1", "alien1.png"),
    "alien2":       Card.new("alien2", "alien2.png"),
    "computer-car": Card.new("computer-car", "computer-car.png"),
    "cuh":          Card.new("cuh", "cuh.png"),
    "karvik1":      Card.new("karvik1", "karvik1.png"),
    "karvik2":      Card.new("karvik2", "karvik2.png"),
    "karvik3":      Card.new("karvik3", "karvik3.png"),
    "karvik4":      Card.new("karvik4", "karvik4.png"),
    "melon":        Card.new("melon", "melon.png")
}

for (card in cards.values) {
    Core.registerCard(card)
}