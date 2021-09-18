local plugin = {}

function plugin:on_start()

end

function plugin:on_stop()

end

function plugin:on_update()
    self.x = self.x + 1
end

return plugin