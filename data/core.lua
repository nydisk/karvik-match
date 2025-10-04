-- core.lua
Core = Core or {}

Core.data = Core.data or {}

---@param list table[]
---@return nil
function Core:register_data(list)
    for _, entry in ipairs(list) do
        self.data[entry.id] = entry
    end
end