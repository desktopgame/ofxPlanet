topBlock = "GrassDirt"
fillBlockDeep = "Stone"
fillBlockShallow = "Dirt"
iBaseline = 0

function start()
    newstruct("House",[[
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
,StoneBottomSlab,StoneBottomSlab,StoneBottomSlab,StoneBottomSlab,,,,,,

Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
Wood,SmoothWood,LDoorBottom,RDoorBottom,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
,,,,,,,,,,

Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
Wood,SmoothWood,LDoorTop,RDoorTop,SmoothWood,SmoothWood,Glass,Glass,Glass,SmoothWood,Wood
,,,,,,,,,,

Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Glass,Glass,Glass,SmoothWood,Wood
,,,,,,,,,,

Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
SmoothWood,,,,,,,,,,SmoothWood
Wood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,SmoothWood,Wood
,,,,,,,,,,

Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone
,,,,,,,,,,

Stone,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,StoneBackSlab,Stone
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
StoneRightSlab,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,Stone,StoneLeftSlab
Stone,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,StoneFrontSlab,Stone
,,,,,,,,,,
]])
    return "default"
-- return "ignore"
end

function onFixHeight(y)
    if(y < iBaseline) then
        return iBaseline;
    end
    return y;
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
    genstruct(
        80,
        1,
        "House"
    )
end

function onGenerateCave(x, y, z, noise)
end

function onPostGenerate()
end