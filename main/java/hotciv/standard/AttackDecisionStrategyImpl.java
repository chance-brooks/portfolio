package hotciv.standard;

import hotciv.framework.AttackDecisionStrategy;

import java.util.Random;

public class AttackDecisionStrategyImpl implements AttackDecisionStrategy {
    public int calculateFinalStrength(int strength, boolean debug){
        if (debug) {
            return strength;
        }
        else{
            Random random = new Random();
            int multiplier = random.nextInt(5) + 1;
            return strength * multiplier;
        }
    }
}
