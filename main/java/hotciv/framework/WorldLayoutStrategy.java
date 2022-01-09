package hotciv.framework;

import hotciv.standard.CityImpl;
import hotciv.standard.GameImpl;
import hotciv.standard.UnitImpl;

import java.util.List;
import java.util.Map;

public interface WorldLayoutStrategy {
    public Map<Position, Tile> defineWorld(Map<Position,Tile> world);

    public void configureCities(List<CityImpl> cities);

    public void configureUnits(GameImpl game);

}
