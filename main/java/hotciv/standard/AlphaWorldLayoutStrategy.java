package hotciv.standard;

import hotciv.framework.*;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class AlphaWorldLayoutStrategy implements WorldLayoutStrategy {
    public Map<Position,Tile> defineWorld(Map<Position,Tile> world){
        Map<Position,Tile> theWorld = new HashMap<Position,Tile>();
        for(int row = 0; row < GameConstants.WORLDSIZE; row++){
            for(int col = 0; col < GameConstants.WORLDSIZE; col++){
                String type = "error";
                if(row == 1 && col == 0) { type = GameConstants.OCEANS;}
                else if(row == 0 && col ==1) { type = GameConstants.HILLS;}
                else if(row == 2 && col == 2) { type = GameConstants.MOUNTAINS;}
                else { type = GameConstants.PLAINS;}
                Position p = new Position(row,col);
                theWorld.put( p, new TileImpl(type));
            }
        }
        return theWorld;
    }

    public void configureCities(List<CityImpl> cities){
        cities.add(new CityImpl(new Position(1,1), Player.RED, 1, 0, GameConstants.ARCHER, GameConstants.productionFocus));
        cities.add(new CityImpl(new Position(4,1), Player.BLUE, 1, 0, GameConstants.LEGION, GameConstants.productionFocus));
    }

    public void configureUnits(GameImpl game){
        game.createUnit(new Position(2,0), GameConstants.ARCHER, Player.RED);
        game.createUnit(new Position(3,2), GameConstants.LEGION, Player.BLUE);
        game.createUnit(new Position(4,3), GameConstants.SETTLER, Player.RED);
    }

}
