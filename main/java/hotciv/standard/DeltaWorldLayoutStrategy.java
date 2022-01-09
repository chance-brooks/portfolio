package hotciv.standard;

import hotciv.framework.*;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DeltaWorldLayoutStrategy implements WorldLayoutStrategy {
    public Map<Position,Tile> defineWorld(Map<Position,Tile> world) {
        // Basically we use a 'data driven' approach - code the
        // layout in a simple semi-visual representation, and
        // convert it to the actual Game representation.
        String[] layout =
                new String[]{
                        "...ooMooooo.....",
                        "..ohhoooofffoo..",
                        ".oooooMooo...oo.",
                        ".ooMMMoooo..oooo",
                        "...ofooohhoooo..",
                        ".ofoofooooohhoo.",
                        "...ooo..........",
                        ".ooooo.ooohooM..",
                        ".ooooo.oohooof..",
                        "offfoooo.offoooo",
                        "oooooooo...ooooo",
                        ".ooMMMoooo......",
                        "..ooooooffoooo..",
                        "....ooooooooo...",
                        "..ooohhoo.......",
                        ".....ooooooooo..",
                };
        // Conversion...
        Map<Position, Tile> theWorld = new HashMap<Position, Tile>();
        String line;
        for (int r = 0; r < GameConstants.WORLDSIZE; r++) {
            line = layout[r];
            for (int c = 0; c < GameConstants.WORLDSIZE; c++) {
                char tileChar = line.charAt(c);
                String type = "error";
                if (tileChar == '.') {
                    type = GameConstants.OCEANS;
                }
                if (tileChar == 'o') {
                    type = GameConstants.PLAINS;
                }
                if (tileChar == 'M') {
                    type = GameConstants.MOUNTAINS;
                }
                if (tileChar == 'f') {
                    type = GameConstants.FOREST;
                }
                if (tileChar == 'h') {
                    type = GameConstants.HILLS;
                }
                Position p = new Position(r, c);
                theWorld.put(p, new TileImpl(type));
            }
        }
        return theWorld;
    }

    public void configureCities(List<CityImpl> cities){
        cities.add(new CityImpl(new Position(1,1), Player.RED, 1, 0, GameConstants.ARCHER, GameConstants.productionFocus));
        cities.add(new CityImpl(new Position(4,1), Player.BLUE, 1, 0, GameConstants.LEGION, GameConstants.productionFocus));
        cities.add(new CityImpl(new Position(8,12), Player.RED, 1, 0, GameConstants.ARCHER, GameConstants.productionFocus));
        cities.add(new CityImpl(new Position(4,5), Player.BLUE, 1, 0, GameConstants.LEGION, GameConstants.productionFocus));

    }

    public void configureUnits(GameImpl game){
        game.createUnit(new Position(2,0), GameConstants.ARCHER, Player.RED);
        game.createUnit(new Position(3,2), GameConstants.LEGION, Player.BLUE);
        game.createUnit(new Position(4,3), GameConstants.SETTLER, Player.RED);
    }

}