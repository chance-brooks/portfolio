package hotciv.framework;

import hotciv.standard.CityImpl;
import hotciv.standard.GameImpl;
import hotciv.standard.UnitImpl;

import java.util.Dictionary;
import java.util.List;

public interface UnitActionStrategy {

    public void UnitAction(Position position, List<UnitImpl> units, List<CityImpl> cities, GameImpl game);

}
