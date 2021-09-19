local plugin = {}

function plugin:on_start()
    print('plugin start')
end

function plugin:on_stop()
    print('plugin stop')
end

function plugin:on_update()
    if input.mouse_released(input.MOUSE_LEFT) then
        context.request_stop()
    end
end

return plugin