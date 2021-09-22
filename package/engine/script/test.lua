local plugin = {}

function plugin:on_start()
    local v = Vec3()
    print(v.x)
end

function plugin:on_stop()
    print('plugin stop')
end

function plugin:on_update()
    if Input.mouse_released(Input.MOUSE_LEFT) then
        Context.request_stop()
    end
end

return plugin