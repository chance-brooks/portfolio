package hotciv.standard;

import hotciv.framework.GameConstants;
import hotciv.framework.Position;
import hotciv.framework.UnitActionStrategy;

import java.util.List;

public class UnitActionStrategyImpl implements UnitActionStrategy {

    String civVersion;

    public UnitActionStrategyImpl(String civVersion) {
        this.civVersion = civVersion;
    }

    public void UnitAction(Position position, List<UnitImpl> units, List<CityImpl> cities, GameImpl game) {

        if(civVersion.equals(GameConstants.GAMMACIV)) {
            for (UnitImpl unit : units) {
                if (unit.getUnitPosition().equals(position)) {
                    if (unit.getTypeString().equals(GameConstants.SETTLER)) {
                        cities.add(new CityImpl(position, unit.getOwner(), 1, 0, GameConstants.SETTLER, GameConstants.productionFocus));
                        units.remove(unit);
                        break;
                    } else if (unit.getTypeString().equals(GameConstants.ARCHER)) {
                        if (unit.getDefensiveStrength() == 3) {
                            unit.setDefensiveStrength(6);
                            unit.setMoveCount(0);
                        }
                        else {
                            unit.setDefensiveStrength(3);
                            unit.setMoveCount(1);
                        }
                    }
                }
            }
        }

        if (civVersion.equals(GameConstants.THETACIV)) {
            for (UnitImpl unit : units) {
                if (unit.getUnitPosition().equals(position)) {
                    if (unit.getTypeString().equals(GameConstants.SETTLER)) {
                        cities.add(new CityImpl(position, unit.getOwner(), 1, 0, GameConstants.SETTLER, GameConstants.productionFocus));
                        units.remove(unit);
                        break;
                    }
                    if (unit.getTypeString().equals(GameConstants.ARCHER)) {
                        if (unit.getDefensiveStrength() == 3) {
                            unit.setDefensiveStrength(6);
                            unit.setMoveCount(0);
                        }
                        else {
                            unit.setDefensiveStrength(3);
                            unit.setMoveCount(1);
                        }
                    }
                    if (unit.getTypeString().equals(GameConstants.UFO)) {
                        if (game.getCityAt(unit.getUnitPosition()) != null && game.getCityAt(unit.getUnitPosition()).getOwner() != unit.getOwner()){
                            ((CityImpl)game.getCityAt(unit.getUnitPosition())).setSize(game.getCityAt(unit.getUnitPosition()).getSize() - 1);
                            if (game.getCityAt(unit.getUnitPosition()).getSize() == 0) {
                                cities.remove(game.getCityAt(unit.getUnitPosition()));
                            }
                        }
                        if (game.getTileAt(unit.getUnitPosition()).getTypeString() == GameConstants.FOREST) {
                            ((TileImpl)game.getTileAt(unit.getUnitPosition())).setTileType(GameConstants.PLAINS);
                        }
                    }
                }
            }
        }

        //TODO: Is there supposed to be anything here?
        if (civVersion.equals(GameConstants.ALPHACIV)) {
            //unit action to be implemented in alphaCIV
        }
    }
}
