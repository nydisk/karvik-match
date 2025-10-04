-- core.lua
Core = Core or {}

Core.data = Core.data or {}

---@param list table[]
---@return nil
function Core:register_data(list)
    for _, entry in ipairs(list) do
        if entry.id then
            self.data[entry.id] = entry
        else
            print("lua @ core.lua => warning: skipping data entry with missing id '"..entry.."'")
        end
    end
end