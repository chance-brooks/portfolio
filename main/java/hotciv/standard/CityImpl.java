package hotciv.standard;

import hotciv.framework.City;
import hotciv.framework.Player;
import hotciv.framework.Position;

public class CityImpl implements City {

    Player owner;
    int size;
    int treasury;
    String production;
    String workforceFocus;
    Position position;

    public CityImpl (Position cityPosition, Player cityOwner, int citySize, int cityTreasury, String cityProduction, String cityFocus) {
        position = cityPosition;
        owner = cityOwner;
        size = citySize;
        treasury = cityTreasury;
        production = cityProduction;
        workforceFocus = cityFocus;
    }

    public Position getPosition() {
        return position;
    }

    public void setPosition(Position temp) {
        position = temp;
    }


    public Player getOwner() {
        return owner;
    }

    public void setOwner(Player temp) {
        owner = temp;
    }


    public int getSize() {
        return size;
    }

    public void setSize(int temp) {
        size = temp;
    }


    public int getTreasury() {
        return treasury;
    }

    public void setTreasury(int temp) {
        treasury = temp;
    }


    public String getProduction() {
        return production;
    }

    public void setProduction(String temp) {
        production = temp;
    }


    public String getWorkforceFocus() {
        return workforceFocus;
    }

    public void setWorkforceFocus(String temp) {
        workforceFocus = temp;
    }
}