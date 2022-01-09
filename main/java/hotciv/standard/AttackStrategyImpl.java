package hotciv.standard;

import hotciv.framework.*;

import java.util.Random;

import static java.util.Objects.isNull;

public class AttackStrategyImpl implements AttackStrategy {
    String civVersion;

    public AttackStrategyImpl (String civVersion) {
        this.civVersion = civVersion;
    }

    public boolean attackUnit(Position from, Position to, GameImpl game, boolean debug){

        if(civVersion.equals(GameConstants.EPSILONCIV)) {
            int defender_strength = (game.getUnitAt(to).getDefensiveStrength() + getSupportingUnits(to, game)) * getTerrainBoost(to, game);
            int attacker_strength = (((UnitImpl)game.getUnitAt(from)).getAttackingStrength() + getSupportingUnits(from, game)) * getTerrainBoost(from, game);

            AttackDecisionStrategyImpl decisionStrategy = new AttackDecisionStrategyImpl();
            int finalAttackingStrength = decisionStrategy.calculateFinalStrength(attacker_strength, debug);
            int finalDefensiveStrength = decisionStrategy.calculateFinalStrength(defender_strength, debug);

            if (finalAttackingStrength > finalDefensiveStrength){
                return true;
            }
            else{
                return false;
            }
        }

        if (civVersion.equals(GameConstants.ALPHACIV)) {
            return true;
        }

        return false;
    }

    private int getSupportingUnits(Position p, GameImpl game) {
        //create iteration bounds "around" the city
        int[] rowDelta = new int[]{-1, -1, 0, +1, +1, +1, 0, -1};
        int[] columnDelta = new int[]{0, +1, +1, +1, 0, -1, -1, -1};

        //iterate around the unit
        int supporting_units = 0;
        for (int index = 0; index < rowDelta.length; index++) {
            int row = p.getRow() + rowDelta[index];
            int col = p.getColumn() + columnDelta[index];
            if (row >= 0 && col >= 0 && row < GameConstants.WORLDSIZE && col < GameConstants.WORLDSIZE) {
                //find number of supporting units
                Position adjacent_p = new Position(row, col);
                if(!isNull(game.getUnitAt(adjacent_p))){
                    if (game.getUnitAt(p).getOwner().equals(game.getUnitAt(adjacent_p).getOwner())) {
                        supporting_units = supporting_units + 1;
                    }
                }
            }
        }

        return supporting_units;
    }

    private int getTerrainBoost(Position p, GameImpl game) {
        int boost = 1;

        if (game.getTileAt(p).getTypeString().equals(GameConstants.HILLS))
            boost = 2;
        if (game.getTileAt(p).getTypeString().equals(GameConstants.FOREST))
            boost = 2;
        if (game.getCityAt(p) != null)
            boost = 3;

        return boost;
    }

}
