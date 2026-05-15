class Card {
    id { _id }
    file { _file }

    construct new(id, file) {
        _id = id
        _file = file
    }
}

class Core {
    foreign static f_registerCard(id, file)

    static registerCard(card) {
        f_registerCard(card.id, card.file)
    }
}