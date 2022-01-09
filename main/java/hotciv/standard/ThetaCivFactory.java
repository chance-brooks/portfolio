package hotciv.standard;

import hotciv.framework.*;

public class ThetaCivFactory implements GameTypeFactory {
    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.ALPHACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.THETACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.EPSILONCIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new AlphaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.EPSILONCIV); }
}
