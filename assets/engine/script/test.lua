local plugin = {}

function plugin:on_start()
    
end

function plugin:on_stop()
    
end

function plugin:on_update()
    if Input.keyboard_pressed(Input.KEYBOARD_U) then
        width, height = Window.get_size()
        Renderer.viewport_set_size(width, height)
    elseif Input.keyboard_pressed(Input.KEYBOARD_I) then
        width, height = Window.get_size()
        Renderer.viewport_set_size(width // 2, height // 2)
    elseif Input.keyboard_pressed(Input.KEYBOARD_O) then
        width, height = Window.get_size()
        Renderer.viewport_set_size(width // 4, height // 4)
    end
end

return plugin