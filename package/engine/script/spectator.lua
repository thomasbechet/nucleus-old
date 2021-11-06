require('engine/script/utils')

local plugin = {
    -- constants
    NORMAL_SPEED         = 10.0 / 1000.0,
    SLOW_SPEED           = 2.0 / 1000.0,
    FAST_SPEED           = 100.0 / 1000.0,
    ROLL_SPEED           = 90.0 / 1000,
    ROTATION_SENSIBILITY = 0.1,
    ZOOM_SPEED           = 5.0,
    
    -- camera variables
    transform = Transform(),
    free_mode = false,
    yaw       = 0.0,
    pitch     = 0.0,
    fov       = 80.0
}
 
function plugin:on_start()
    -- set initial position
    self.transform.translation = Vec3(0, 0, 0.5)
end

function plugin:on_update()
    -- check cursor mode
    if Input.get_cursor_mode() ~= Input.CURSOR_MODE_DISABLE then 
        return 
    end

    -- get delta time
    local delta_time = Context.get_delta_time()

    -- update camera mode
    if Input.keyboard_pressed(Input.KEYBOARD_C) then
        self.free_mode = not self.free_mode
    end

    -- camera translation
    local direction = Vec3()
    if Input.keyboard_is_pressed(Input.KEYBOARD_W) then
        direction = direction + self.transform:forward()
    end
    if Input.keyboard_is_pressed(Input.KEYBOARD_S) then
        direction = direction + self.transform:backward()
    end
    if Input.keyboard_is_pressed(Input.KEYBOARD_A) then
        direction = direction + self.transform:left()
    end
    if Input.keyboard_is_pressed(Input.KEYBOARD_D) then
        direction = direction + self.transform:right()
    end
    if self.free_mode then
        if Input.keyboard_is_pressed(Input.KEYBOARD_X) then
            direction = direction + self.transform:up()
        end
        if Input.keyboard_is_pressed(Input.KEYBOARD_Z) then
            direction = direction + self.transform:down()
        end
    else
        if Input.keyboard_is_pressed(Input.KEYBOARD_X) then
            direction = direction + Vec3.UP
        end
        if Input.keyboard_is_pressed(Input.KEYBOARD_Z) then
            direction = direction + Vec3.DOWN
        end
    end
    Vec3.normalize(direction)

    -- camera speed
    local speed = self.NORMAL_SPEED
    if Input.keyboard_is_pressed(Input.KEYBOARD_LSHIFT) then speed = self.FAST_SPEED end
    if Input.keyboard_is_pressed(Input.KEYBOARD_LCONTROL) then speed = self.SLOW_SPEED end

    -- apply translation
    self.transform.translation = self.transform.translation + direction * delta_time * speed

    -- camera fov
    local sx, sy = Input.get_mouse_scroll()
    if sy ~= 0.0 then
        if sy > 0.0 then
            self.fov = self.fov - self.ZOOM_SPEED
        else
            self.fov = self.fov + self.ZOOM_SPEED
        end
        self.fov = math.max(0.1, math.min(179.9, self.fov))
    end

    -- get mouse motion
    local mx, my = Input.get_mouse_motion()

    -- apply rotation
    if self.free_mode then
        if mx ~= 0 then
            self.transform.rotation = self.transform.rotation * Quat(-math.rad(mx) * self.ROTATION_SENSIBILITY, Vec3.UP)
        end
        if my ~= 0 then
            self.transform.rotation = self.transform.rotation * Quat(math.rad(my) * self.ROTATION_SENSIBILITY, Vec3.LEFT)
        end
        if Input.keyboard_is_pressed(Input.KEYBOARD_Q) then
            self.transform.rotation = self.transform.rotation * Quat(-math.rad(self.ROLL_SPEED) * delta_time, Vec3.FORWARD)
        end
        if Input.keyboard_is_pressed(Input.KEYBOARD_E) then
            self.transform.rotation = self.transform.rotation * Quat(math.rad(self.ROLL_SPEED) * delta_time, Vec3.FORWARD)
        end
    else
        if mx ~= 0 then
            self.yaw = self.yaw + mx * self.ROTATION_SENSIBILITY
        end
        if my ~= 0 then
            self.pitch = self.pitch + my * self.ROTATION_SENSIBILITY
        end
        
        if self.pitch < -90.0 then self.pitch = -90.0 end
        if self.pitch > 90.0 then self.pitch = 90.0 end

        self.transform.rotation = Quat(-math.rad(self.yaw), Vec3.UP)
        self.transform.rotation = self.transform.rotation * Quat(math.rad(self.pitch), Vec3.LEFT)
    end

    -- update camera
    local forward = self.transform:forward()
    local up      = self.transform:up()

    Renderer.camera_set_view(self.transform.translation, forward, up)
    Renderer.camera_set_fov(math.rad(self.fov))
end

return plugin