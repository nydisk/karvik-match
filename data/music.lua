--[[ 
    It is possible to use any of the following values for the playsIn table:
    splash, menu, game, settings
--]]


local music = {
	{
        type = "bgm",
        id = "katamari",
        file = "bgm/katamari.ogg",
        author = "Fearofdark",
        title = "Rolling Down The Street, In My Katamari",
        playsIn = { "game" }
    }
}

Core:register_data(music)