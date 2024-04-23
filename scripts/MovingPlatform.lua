-- Sample Script: MovingPlatform.lua
-- This script moves the entity back and forth along the X axis

local speed = 1

local moveForward = true
local frames = 0
local fps = 60
local secondsToMove = 3

local transform = nil
local position = nil

function Start()
    transform = AttachedEntity:GetCTransform()
    position = transform.Position
end

function Update()
    frames = frames + 1

    if frames % (fps * secondsToMove) == 0 then
        moveForward = not moveForward
    end

    if moveForward then
        position.X = position.X + speed
    else
        position.X = position.X - speed
    end
end