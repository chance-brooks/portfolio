package hotciv.framework;

import hotciv.standard.CityImpl;

import java.util.List;

public interface WinStrategy {
    public Player getWinner(int age, int round, List<CityImpl> cities, int red_wins, int blue_wins);
}
