posX = 64
posY = 32
posZ = 64

function start()
    newstruct("Model",[[
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
-- return "default"
    return "ignore"
end

function onFixHeight(y)
    return y
end

function onGenerateTerrain(x, y, z)
end

function onGenerateStructures()
end

function onGenerateCave(x, y, z, noise)
end

function onPostGenerate()
    expandstruct(posX, posY, posZ, "Model")
end