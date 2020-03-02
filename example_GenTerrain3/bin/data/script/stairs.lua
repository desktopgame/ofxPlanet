topBlock = "GrassDirt"
fillBlockDeep = "Stone"
fillBlockShallow = "Dirt"

function start()
    newstruct("Tree",[[
,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
]])
    return "default"
-- return "ignore"
end

function onFixHeight(y)
    if(y >= -0.2 and y < -0.5) then
        y = -0.2
    elseif(y >= -0.5 and y < -0.8) then
        y = -0.5
    elseif(y >= -0.8 and y < 0) then
        y = -0.8
    elseif(y >= 0 and y < 0.3) then
        y = 0
    elseif(y >= 0.3 and y < 0.5) then
        y = 0.3
    elseif(y >= 0.5 and y < 0.6) then
        y = 0.5
    elseif(y >= 0.6 and y < 0.7) then
        y = 0.6
    elseif(y >= 0.7 and y < 0.8) then
        y = 0.7
    elseif(y >= 0.8) then
        y = 0.8
    end
    return y
end

function onGenerateTerrain(x, y, z)
    startY = y
    setblock(x, y, z, topBlock);
    while y > 0 do
        y = y - 1
        if y < (startY - 5) then
            setblock(x, y, z, fillBlockDeep)
        else
            setblock(x, y, z, fillBlockShallow)
        end
    end
end

function onGenerateStructures()
    putblockrange(0, 0, 0, getxsize()-1, math.floor(getysize() / 3) - 5, getzsize()-1, "Water")
    setweightrange(0, 0, 0, getxsize()-1, math.floor(getysize() / 3) - 4, getzsize()-1, 10, "Tree");
    setweightrange(0, getysize()-20, 0, getxsize()-1, getysize()-1, getzsize()-1, 10, "Tree");
    genstruct(
        12,
        1,
        "Tree"
    )
end

function onGenerateCave(x, y, z, noise)
end

function onPostGenerate()
end