local plugin = {}

function plugin:on_start()

end

function plugin:on_stop()
    
end

function plugin:on_update()
    if Input.keyboard_pressed(Input.KEYBOARD_F2) then
        Context.request_stop()
    end
end

return plugin