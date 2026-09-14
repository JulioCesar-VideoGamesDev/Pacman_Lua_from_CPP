-- config.lua

-- for ConfigManager if we are using it
powerUpScore            = 5000
powerUpDuration         = 5.0
powerUpSpeedMultiplier  = 2.0
bronzeMedalPoints       = 500
coinPoints              = 50

--------------------------------------------------------------------

function getPowerUpColor(health)
    if health >= 1.5 then
        return {r = 255, g = 0,   b = 0}    -- rojo
    elseif health >= 1.0 then
        return {r = 255, g = 165, b = 0}    -- naranja
    elseif health >= 0.5 then
        return {r = 0,   g = 255, b = 0}    -- verde
    else
        return {r = 0,   g = 0,   b = 255}  -- azul
    end
end

--------------------------------------------------------------------

-- base functions of the library
setPacmanSpeedMultiplier(1.0)
setPacmanColor(255, 255, 0)
setPowerUpTime(5)

--------------------------------------------------------------------

-- class Pacman
pacman = Pacman()
pacman:setSpeedMultiplier(1.0)
pacman:setColor(255, 255, 0)
pacman:setPowerUpTime(5)
pacman:setMaxLives(1.5)
