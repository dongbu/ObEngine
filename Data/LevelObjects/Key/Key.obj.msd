Key:
    @Animator
        path:"Sprites/LevelObjects/Key"
    @Collider
        position:"relative"
        solid:False
        click:True
        ?polygonPoints(str):
            "0,0"
            "64,0"
            "64,64"
            "0,64"
    @LevelSprite
        posX:600
        posY:200
        rotation:0
        scale:1.0
        layer:1
        z-depth:21
    @Script
        priority:1
        ?scriptList(str):
            "Data/LevelObjects/Key/Key.lua"