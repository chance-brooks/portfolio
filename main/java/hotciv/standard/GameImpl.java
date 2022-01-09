package hotciv.standard;

import hotciv.framework.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.lang.Math;

/** Skeleton implementation of HotCiv.
 
   This source code is from the book 
     "Flexible, Reliable Software:
       Using Patterns and Agile Development"
     published 2010 by CRC Press.
   Author: 
     Henrik B Christensen 
     Department of Computer Science
     Aarhus University
   
   Please visit http://www.baerbak.com/ for further information.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

//TODO implement unit attack

public class GameImpl implements Game {


    int red_wins, blue_wins, age, round=0;
    Player currPlayer = Player.RED;
    private Map<Position,Tile> world;
    public List<CityImpl> cities = new ArrayList<>();
    List<UnitImpl> units = new ArrayList<>();
    GameTypeFactory factory;

    //For factories
    AgeStrategy ageStrategy;
    WinStrategy winStrategy;
    WorldLayoutStrategy worldLayoutStrategy;
    UnitActionStrategy unitActionStrategy;
    AttackStrategy attackStrategy;


    public GameImpl(){

    }

    public GameImpl(GameTypeFactory factory){
        this.factory = factory;
        ageStrategy = factory.createAgeStrategy();
        unitActionStrategy = factory.createUnitActionStrategy();
        winStrategy = factory.createWinStrategy();
        worldLayoutStrategy = factory.createWorldLayoutStrategy();
        attackStrategy = factory.createAttackStrategy();

        world = worldLayoutStrategy.defineWorld(world);
        worldLayoutStrategy.configureCities(cities);
        worldLayoutStrategy.configureUnits(this);

        age = ageStrategy.getAge();


    }

    public Tile getTileAt(Position p) {
        return world.get(p);
    }

    public Unit getUnitAt(Position p) {
        for (UnitImpl unit: units)
            if (unit.getUnitPosition().equals(p))
                return unit;
        return null;
    }

    public City getCityAt(Position p) {
        for (CityImpl city : cities)
            if (city.getPosition().equals(p))
                return city;
        return null;
    }

    public Player getPlayerInTurn() {
        return currPlayer;
    }

    public Player getWinner() {
        return winStrategy.getWinner(age, round, cities, red_wins, blue_wins);
    }

    public int getAge() {
        return age;
    }

    public boolean moveUnit(Position from, Position to) {

        if ((Math.abs(from.getRow() - to.getRow()) > 1) || (Math.abs(from.getColumn() - to.getColumn()) > 1)) {
            return false;
        }

        if (getUnitAt(from).getMoveCount() > 0){
            if ((to.getColumn() >= 0 && to.getRow() >= 0 && to.getRow() < GameConstants.WORLDSIZE && to.getColumn() < GameConstants.WORLDSIZE) && (from.getColumn() >= 0 && from.getRow() >= 0 && from.getRow() < GameConstants.WORLDSIZE && from.getColumn() < GameConstants.WORLDSIZE)) {
                if (getUnitAt(to) == null && getTileAt(to).getTypeString() != GameConstants.MOUNTAINS && getTileAt(to).getTypeString() != GameConstants.OCEANS) {
                    ((UnitImpl)getUnitAt(from)).setUnitPosition(to);
                    ((UnitImpl)getUnitAt(to)).setMoveCount(getUnitAt(to).getMoveCount() - 1);
                    return true;
                }
                if (getUnitAt(from).getTypeString() == GameConstants.UFO) {
                    ((UnitImpl)getUnitAt(from)).setUnitPosition(to);
                    ((UnitImpl)getUnitAt(to)).setMoveCount(getUnitAt(to).getMoveCount() - 1);
                    return true;
                }
            }
        }
        return false;
    }

    public void endOfTurn() {

        //implementing behavior of cities at end of turn
        endOfTurn_cityHandler();

        //implementing behavior of players at end of turn
        endOfTurn_playerHandler();

        for (UnitImpl unit : units) {
            if (unit.getOwner().equals(currPlayer)){
                unit.resetMoveCount(unit.getTypeString());
            }
        }
    }

    public void endOfTurn_cityHandler() {
        for (CityImpl city : cities) {
            if (city.getOwner().equals(currPlayer)) {
                city.setTreasury(city.getTreasury() + 6);

                if (city.getProduction().equals(GameConstants.ARCHER) && city.getTreasury() >= 10) {
                    endOfTurn_productionHandler(city, GameConstants.ARCHER, 10);
                } else if (city.getProduction().equals(GameConstants.SETTLER) && city.getTreasury() >= 30) {
                    endOfTurn_productionHandler(city, GameConstants.SETTLER, 30);
                } else if (city.getProduction().equals(GameConstants.LEGION) && city.getTreasury() >= 15) {
                    endOfTurn_productionHandler(city, GameConstants.LEGION, 15);
                } else if (city.getProduction().equals(GameConstants.UFO) && city.getTreasury() >= 60) {
                    endOfTurn_productionHandler(city, GameConstants.UFO, 60);
                }
            }
        }
    }

    //Helper functions for end of turn
    public void endOfTurn_productionHandler(CityImpl currCity, String unitType, int adjustment) {
        //this method is still less than ideal, but this implementation cuts down on repeated code significantly
        //if there isn't a city at this unit, create one
        if (getUnitAt(currCity.getPosition()) == null) {
            createUnit(currCity.getPosition(), unitType, currPlayer);
            currCity.setTreasury(currCity.getTreasury() - adjustment);

        } else {
            //create iteration bounds "around" the city
            int[] rowDelta = new int[]{-1, -1, 0, +1, +1, +1, 0, -1};
            int[] columnDelta = new int[]{0, +1, +1, +1, 0, -1, -1, -1};

            //iterate around the city
            for (int index = 0; index < rowDelta.length; index++) {
                int row = currCity.getPosition().getRow() + rowDelta[index];
                int col = currCity.getPosition().getColumn() + columnDelta[index];
                if (row >= 0 && col >= 0 && row < GameConstants.WORLDSIZE && col < GameConstants.WORLDSIZE) {
                    //create unit if one does not exist
                    if (getUnitAt(new Position(row, col)) == null) {
                        createUnit(new Position(row, col), unitType, currPlayer);
                        currCity.setTreasury(currCity.getTreasury() - adjustment);
                        break;
                    }
                }
            }
        }
    }

    public void endOfTurn_playerHandler() {
        switch (currPlayer) {
            case RED:
                currPlayer = Player.BLUE;
                break;
            case BLUE:
                currPlayer = Player.RED;
                age = ageStrategy.ageWorld();
                round++;
                break;
        }
    }

    public void changeWorkForceFocusInCityAt(Position p, String balance) {
        for (CityImpl city : cities)
            if (city.getPosition().equals(p))
                city.setWorkforceFocus(balance);
    }

    public void changeProductionInCityAt(Position p, String unitType) {
        for (CityImpl city : cities)
            if (city.getPosition().equals(p))
                city.setProduction(unitType);
    }

    public void performUnitActionAt(Position p, GameImpl game) {
        unitActionStrategy.UnitAction(p, units, cities, game);
    }

    public boolean createUnit(Position p, String kindOfUnit, Player ownerOfUnit) {
        if (getUnitAt(p) == null) {
            units.add(new UnitImpl(p, kindOfUnit, ownerOfUnit));
            return true;
        }
        return false;
    }

    public boolean createCity(Position position, Player player, int size, int treasury, String unit, String focus) {
        cities.add(new CityImpl(position, player, size, treasury, unit, focus));
        return true;
    }

    public Integer getUnitsIndex(Position p) {
        for (int count = 0; count < units.size(); count++) {
            if (units.get(count).getUnitPosition().equals(p)) {
                return count;
            }
        }
        return null;
    }

    public boolean attack(Position from, Position to, GameImpl game, boolean debug) {
        boolean result = attackStrategy.attackUnit(from, to, game, debug);
        if (result){
            incrementNumberOfWins(getUnitAt(from).getOwner());
            units.remove(getUnitAt(to));
            moveUnit(from, to);
        }
        else {
            units.remove(getUnitAt(from));
        }
        return result;
    }

    public void incrementNumberOfWins(Player player) {
        if (player.equals(Player.RED)) {
            red_wins++;
        }
        else if (player.equals(Player.BLUE)) {
            blue_wins++;
        }
    }

    public List<CityImpl> getCityList() {return cities;}


    //TODO
    public void addObserver(GameObserver observer) {

    }

    public void setTileFocus(Position position) {

    }

}