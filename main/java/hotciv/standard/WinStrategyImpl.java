package hotciv.standard;

import hotciv.framework.GameConstants;
import hotciv.framework.Player;
import hotciv.framework.WinStrategy;

import java.util.List;

public class WinStrategyImpl implements WinStrategy {
    
    String civVersion;

    public WinStrategyImpl(String civVersion) {this.civVersion = civVersion;}

    public Player getWinner(int age, int round, List<CityImpl> cities, int red_wins, int blue_wins) {

        if (this.civVersion.equals(GameConstants.ALPHACIV))
            if (age >= -3000) return Player.RED;

        if (this.civVersion.equals(GameConstants.BETACIV)) {
            Player owner = cities.get(0).getOwner();
            for (CityImpl city : cities)
                if (!city.getOwner().equals(owner)) return null;
            return owner;
        }

        if (this.civVersion.equals((GameConstants.EPSILONCIV))) {
            if (red_wins == 3){
                return Player.RED;
            }
            else if (blue_wins == 3){
                return Player.BLUE;
            }
        }

        if (this.civVersion.equals(GameConstants.ZETACIV)) {
            if (round <= 20) {
                WinStrategyImpl betaStrategy = new WinStrategyImpl(GameConstants.BETACIV);
                betaStrategy.getWinner(age, round, cities, red_wins, blue_wins);
            }
            else {
                WinStrategyImpl epsilonStrategy = new WinStrategyImpl((GameConstants.EPSILONCIV));
                epsilonStrategy.getWinner(age, round, cities, red_wins, blue_wins);
            }
        }

        return null;
    }
}
