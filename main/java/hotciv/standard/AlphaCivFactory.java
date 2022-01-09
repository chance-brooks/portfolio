package hotciv.standard;

import hotciv.framework.*;

public class AlphaCivFactory implements GameTypeFactory {
    public AgeStrategy createAgeStrategy(){
        return new AgeStrategyImpl(GameConstants.ALPHACIV);
    }

    public UnitActionStrategy createUnitActionStrategy(){
        return new UnitActionStrategyImpl(GameConstants.ALPHACIV);
    }

    public WinStrategy createWinStrategy(){
        return new WinStrategyImpl(GameConstants.ALPHACIV);
    }

    public WorldLayoutStrategy createWorldLayoutStrategy(){
        return new AlphaWorldLayoutStrategy();
    }

    public AttackStrategy createAttackStrategy(){ return new AttackStrategyImpl(GameConstants.ALPHACIV); }

}
