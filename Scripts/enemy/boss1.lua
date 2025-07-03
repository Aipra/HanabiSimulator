frame = 0
function activate()
    frame = 0
end

function update(dt)
    frame = frame + 1
    speed = 240
    if frame % 30 == 1 then
        local px, py = get_player_pos()
        for i=1, 8 do
            local angle = math.rad(i*45)
            emit_bullet(px, py, math.cos(angle)*speed, math.sin(angle)*speed, 1)
        end
    end
end